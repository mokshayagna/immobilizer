#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <time.h>
#include <stdint.h>
#include "functions.h"

#define PER_CHALLENGE_TIMEOUT 5
#define TOTAL_AUTH_TIMEOUT   15
#define SERVER_PORT          5000

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

/* ===================== UTILITY ===================== */
void print_hex(const char *label, uint8_t *data, int len)
{
    printf("%s: ", label);
    for (int i = 0; i < len; i++)
        printf("%02X ", data[i]);
    printf("\n");
}

void debug_print_frame(const char *tag, ChallengeFrame *frame)
{
    printf("\n[%s]\n", tag);
    printf("Challenge Counter : %u\n", frame->challenge_counter);
    print_hex("Random Number     ", frame->random_number, 16);
}

void flush_socket(int sockfd){
    uint8_t buffer[256];
    while(1){
        int ret = recv(sockfd, buffer ,sizeof(buffer),MSG_DONTWAIT);
        if(ret > 0){
            continue;
        }
        if (ret == -1 && errno == EAGAIN) {
            // No more data available
            break;
        }

        if (ret <= 0) {
            // Socket closed or error
            break;
        }
    }
}


/* ===================== CHALLENGE PREPARATION ===================== */
void prepare_challenge(ChallengeFrame *frame, uint8_t counter)
{
    printf("\n=== PREPARE CHALLENGE ===\n");

    trng(frame->random_number, 16);
    print_hex("Generated Random", frame->random_number, 16);

    frame->challenge_counter = counter;
    printf("Assigned Counter : %u\n", frame->challenge_counter);

    debug_print_frame("FINAL CHALLENGE (to be sent)", frame);
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
int wait_and_verify_response(int client_fd,uint8_t expected_counter,uint8_t *expected_aes)
{
    ResponseFrame response;
    time_t start = time(NULL);
    int retval;

    while (difftime(time(NULL), start) < PER_CHALLENGE_TIMEOUT) {

        retval = recv(client_fd, &response,sizeof(ResponseFrame), MSG_DONTWAIT);

        if (retval == sizeof(ResponseFrame)) {

            printf("\n=== CLIENT RESPONSE RECEIVED ===\n");
            printf("Response Counter : %u\n", response.challenge_counter);
            print_hex("Client Response  ", response.encrypted, 16);

            /* 🔹 ADDED: counter validation */
        
            if (response.challenge_counter != expected_counter) {
                printf("❌ OLD / STALE RESPONSE → IGNORED\n");
                continue;
            }
            
            print_hex("Expected AES     ", expected_aes, 16);

            if (memcmp(response.encrypted, expected_aes, 16) == 0) {
                printf("\n✅ AUTHENTICATION SUCCESS\n");
                return 1;
            } else {
                printf("❌ AES MISMATCH\n");
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

    printf("⏱ Challenge timeout\n");
    return 0;
}


int perform_authentication(int client_fd)
{
    ChallengeFrame frame;
    uint8_t encrypted[16];
    uint8_t counter = 1;

    time_t total_start = time(NULL);
    int attempt = 0;

    while (difftime(time(NULL), total_start) < TOTAL_AUTH_TIMEOUT) {
        attempt++;

        printf("\n==============================\n");
        printf(" CHALLENGE ATTEMPT %d\n", attempt);
        printf("==============================\n");

        /* Send challenge */
        prepare_challenge(&frame, counter);

        printf("\n=== SENDING CHALLENGE TO CLIENT ===\n");
        printf("Bytes sent : %lu\n", sizeof(ChallengeFrame));

        send(client_fd, &frame, sizeof(ChallengeFrame), 0);

        /* Encrypt random only */
        printf("\n=== SERVER ENCRYPTION ===\n");
        print_hex("Plain Random", frame.random_number, 16);

        aes128_encrypt(frame.random_number, encrypted, secret_key);
        print_hex("AES Number generated in engine:", encrypted, 16);

        /* Wait for response */
        if (wait_and_verify_response(client_fd,counter,encrypted)) {
            return 1;
        }

        printf("→ Invalid response, retrying...\n");

        counter++;
        if (counter == 0)
            counter = 1;
    }

    printf("\nTOTAL AUTH TIMEOUT → ACCESS DENIED\n");
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
