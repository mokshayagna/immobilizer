#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <time.h>
#include <stdint.h>
#include "functions.h"

//void aes128_encrypt(uint8_t *pt,uint8_t *ct,uint8_t *key);

#define PER_CHALLENGE_TIMEOUT 5
#define TOTAL_AUTH_TIMEOUT   15
#define SERVER_PORT          5000

/* ===================== SECRET KEY ===================== */
uint8_t secret_key[16] = {
    0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,
    0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c
};
/* ===================== UTILITY ===================== */
void print_hex(const char *label, uint8_t *data, int len)
{
    printf("%s: ", label);
    for (int i = 0; i < len; i++)
        printf("%02X ", data[i]);
    printf("\n");
}

/* ===================== SOCKET FUNCTIONS ===================== */
int create_server_socket(void)
{
    int server_fd;
    struct sockaddr_in addr;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(SERVER_PORT);

    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 5);

    printf("Server waiting on port %d...\n", SERVER_PORT);
    return server_fd;
}

int wait_for_client(int server_fd)
{
    int client_fd = accept(server_fd, NULL, NULL);
    printf("Client connected\n");
    return client_fd;
}

/* ===================== AUTH FUNCTIONS ===================== */
int send_challenge(int client_fd, uint8_t *challenge)
{
    trng(challenge, 16);
    print_hex("Generated Random", challenge, 16);
    return send(client_fd, challenge, 16, 0);
}

int wait_and_verify_response(int client_fd, uint8_t *expected)
{
    uint8_t response[16];
    time_t start = time(NULL);
    int retval;

    while (difftime(time(NULL), start) < PER_CHALLENGE_TIMEOUT) {

        retval = recv(client_fd, response, 16, MSG_DONTWAIT);

        if (retval == 16) {
            print_hex("Received Encrypted Reply", response, 16);

            if (memcmp(response, expected, 16) == 0) {
                printf("AUTHENTICATION SUCCESS\n");
                return 1;
            }
        }

        if (retval == -1 && errno == EAGAIN) {
            usleep(500000);
            continue;
        }

        if (retval == 0) {
            printf("Client disconnected\n");
            return 0;
        }
    }

    printf("Challenge timeout\n");
    return 0;
}

int perform_authentication(int client_fd)
{
    uint8_t challenge[16];
    uint8_t encrypted[16];
    time_t total_start = time(NULL);
    int attempt = 0;

    while (difftime(time(NULL), total_start) < TOTAL_AUTH_TIMEOUT) {
        attempt++;
        printf("\n--- Challenge Attempt %d ---\n", attempt);

        send_challenge(client_fd, challenge);
        aes128_encrypt(challenge, encrypted, secret_key);
        print_hex("Server AES Value", encrypted, 16);

        if (wait_and_verify_response(client_fd, encrypted))
            return 1;
    }

    printf("TOTAL AUTH TIMEOUT → ACCESS DENIED\n");
    return 0;
}

/* ===================== MAIN ===================== */
int main(void)
{
    int server_fd, client_fd;
    char recvBuffer[1024];
    int retval;

    server_fd = create_server_socket();
    client_fd = wait_for_client(server_fd);

    retval = recv(client_fd, recvBuffer, sizeof(recvBuffer)-1, 0);
    recvBuffer[retval] = '\0';

    printf("Server received: %s\n", recvBuffer);

    if (strcmp(recvBuffer, "start") == 0)
        printf("Engine ECU started processing\n");

    perform_authentication(client_fd);

    close(client_fd);
    close(server_fd);
    return 0;
}
