#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <time.h>
#include <stdint.h>
#include "functions.h"

/* ===================== DEFINES ===================== */
#define PER_CHALLENGE_TIMEOUT 5
#define TOTAL_AUTH_TIMEOUT   15
#define SERVER_PORT          5001
#define POST_RELEASE_TIMEOUT 6

#define ENGINE_OFF 0
#define ENGINE_ON  1

/* ===================== SECRET KEY ===================== */
uint8_t secret_key[16] = {
    0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,
    0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c
};

/* ===================== FRAME STRUCTURES ===================== */
typedef struct {
    uint8_t challenge_counter;
    uint8_t random_number[16];
} ChallengeFrame;

typedef struct {
    uint8_t challenge_counter;
    uint8_t encrypted[16];
} ResponseFrame;

/* ===================== GLOBAL STATE ===================== */
int engine_state = ENGINE_OFF;
int auth_valid = 0;
time_t last_engine_off_time = 0;

/* ===================== UTILITY ===================== */
void print_hex(const char *label, uint8_t *data, int len)
{
    printf("[DEBUG] %s : ", label);
    for (int i = 0; i < len; i++)
        printf("%02X ", data[i]);
    printf("\n");
}

/* ===================== SOCKET ===================== */
int create_server_socket(void)
{
    int fd;
    struct sockaddr_in addr;

    fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(SERVER_PORT);

    bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(fd, 5);

    printf("[INFO] Engine ECU listening on port %d\n", SERVER_PORT);
    return fd;
}

int wait_for_client(int server_fd)
{
    int client_fd = accept(server_fd, NULL, NULL);
    printf("[INFO] Transponder ECU connected\n");
    return client_fd;
}

/* ===================== AUTH ===================== */
int wait_and_verify_response(int client_fd,uint8_t expected_counter,uint8_t *expected_aes)
{
    ResponseFrame response;
    time_t start = time(NULL);
    int ret;

    printf("[DEBUG] Waiting for transponder response...\n");

    while (difftime(time(NULL), start) < PER_CHALLENGE_TIMEOUT) {

        ret = recv(client_fd, &response, sizeof(response), MSG_DONTWAIT);

        if (ret == sizeof(response)) {

            print_hex("Client AES", response.encrypted, 16);
            print_hex("Expected AES", expected_aes, 16);

            if (response.challenge_counter != expected_counter) {
                printf("[WARN] Counter mismatch → ignored\n");
                continue;
            }

            if (memcmp(response.encrypted, expected_aes, 16) == 0) {
                printf("[INFO] Authentication SUCCESS\n");
                return 1;
            } else {
                printf("[ERROR] AES mismatch\n");
                return 0;
            }
        }

        if (ret == -1 && errno == EAGAIN)
            usleep(300000);
    }

    printf("[ERROR] Challenge timeout\n");
    return 0;
}

int perform_authentication(int client_fd)
{
    ChallengeFrame frame;
    uint8_t encrypted[16];
    uint8_t counter = 1;
    time_t start = time(NULL);

    printf("[INFO] Authentication started\n");

    while (difftime(time(NULL), start) < TOTAL_AUTH_TIMEOUT) {

        trng(frame.random_number, 16);
        frame.challenge_counter = counter;

        print_hex("Generated Random", frame.random_number, 16);

        send(client_fd, &frame, sizeof(frame), 0);
        printf("[DEBUG] Challenge sent\n");

        aes128_encrypt(frame.random_number, encrypted, secret_key);
        print_hex("Engine AES", encrypted, 16);

        if (wait_and_verify_response(client_fd, counter, encrypted))
            return 1;

        counter++;
    }

    return 0;
}

/* ===================== POST RELEASE ===================== */
int check_post_release(void)
{
    if (!auth_valid) {
        printf("[DEBUG] No previous authentication\n");
        return 0;
    }

    if (last_engine_off_time == 0) {
        printf("[DEBUG] No ENGINE OFF recorded\n");
        return 0;
    }

    double diff = difftime(time(NULL), last_engine_off_time);
    printf("[DEBUG] Time since ENGINE OFF: %.0f sec\n", diff);

    if (diff <= POST_RELEASE_TIMEOUT) {
        printf("[INFO] Post-release VALID → Skip auth\n");
        return 1;
    }

    printf("[INFO] Post-release EXPIRED\n");
    auth_valid = 0;
    return 0;
}

/* ===================== MAIN ===================== */
int main(void)
{
    int server_fd, client_fd;
    char recvBuffer[64];

    server_fd = create_server_socket();
    client_fd = wait_for_client(server_fd);

    while (1) {

        memset(recvBuffer, 0, sizeof(recvBuffer));
        int r = recv(client_fd, recvBuffer, sizeof(recvBuffer)-1, 0);

        if (r <= 0) {
            printf("[ERROR] Client disconnected\n");
            break;
        }

        printf("\n[INFO] Command: %s\n", recvBuffer);

        if (strcmp(recvBuffer, "start") != 0)
            continue;

        if (engine_state == ENGINE_ON) {
            printf("[WARN] Engine already ON\n");
            continue;
        }

        printf("[INFO] START accepted\n");

        if (check_post_release()) {
            engine_state = ENGINE_ON;
            printf("[ENGINE] ON (Post-release)\n");

            sleep(2);

            engine_state = ENGINE_OFF;
            last_engine_off_time = time(NULL);
            printf("[ENGINE] OFF\n");
            continue;
        }

        if (perform_authentication(client_fd)) {
            engine_state = ENGINE_ON;
            auth_valid = 1;

            printf("[ENGINE] ON (Authenticated)\n");

            sleep(2);

            engine_state = ENGINE_OFF;
            last_engine_off_time = time(NULL);
            printf("[ENGINE] OFF\n");
        }
        else {
            printf("[ENGINE] OFF (Auth Failed)\n");
        }
    }

    close(client_fd);
    close(server_fd);
    return 0;
}