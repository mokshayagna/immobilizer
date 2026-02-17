#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <errno.h>
#include "functions.h"

#define SERVER_PORT 5000

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
    printf("[DEBUG] %s : ", label);
    for (int i = 0; i < len; i++)
        printf("%02X ", data[i]);
    printf("\n");
}

int main(void)
{
    int sockfd;
    int retval;
    int attempt = 0;

    ChallengeFrame frame;
    ResponseFrame rframe;
    uint8_t encrypted[16];

    printf("[CLIENT] Transponder ECU starting...\n");

    /* ===================== SOCKET SETUP ===================== */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    printf("[CLIENT] Connected to Engine ECU\n");

    /* ---------- FIRST DRIVER ACTION ---------- */
    printf("\n[DRIVER] Turning key → START (1st attempt)\n");
    send(sockfd, "start", 5, 0);

    /* ===================== MAIN LOOP ===================== */
    while (1) {

        printf("\n[DEBUG] Waiting for challenge from Engine ECU...\n");

        retval = recv(sockfd, &frame, sizeof(ChallengeFrame), MSG_DONTWAIT);

        /* ---------- NO DATA RECEIVED ---------- */
        if (retval == -1 && errno == EAGAIN) {
            printf("[DEBUG] No challenge received\n");
            printf("[DEBUG] → Engine ECU likely skipped authentication (POST-RELEASE)\n");
            usleep(500000);
            continue;
        }
        /* ---------- CONNECTION CLOSED ---------- */
        if (retval == 0) {
            printf("[CLIENT] Engine ECU closed connection\n");
            break;
        }

        /* ---------- CHALLENGE RECEIVED ---------- */
        attempt++;

        printf("\n==============================\n");
        printf(" TRANSPONDER ATTEMPT %d\n", attempt);
        printf("==============================\n");

        printf("[CLIENT] Challenge Counter : %u\n", frame.challenge_counter);
        print_hex("Random Number", frame.random_number, 16);

        /* Encrypt challenge */
        aes128_encrypt(frame.random_number, encrypted, secret_key);
        print_hex("Encrypted Random", encrypted, 16);

        rframe.challenge_counter = frame.challenge_counter;
        memcpy(rframe.encrypted, encrypted, 16);

        /* ---------- DELAY LOGIC (UNCHANGED) ---------- */
        if (attempt == 1) {
            printf("[SIM] First attempt → simulate slow response (timeout)\n");
            sleep(6);
        } else {
            printf("[SIM] Normal response\n");
            sleep(1);
        }

        send(sockfd, &rframe, sizeof(rframe), 0);
        printf("[CLIENT] Response sent to Engine ECU\n");

        /* ---------- DRIVER TRIES AGAIN ---------- */
        if (attempt == 2) {
            printf("\n[ENGINE] OFF (simulated)\n");
            printf("[DRIVER] Turning key → START again (post-release test)\n");
            send(sockfd, "start", 5, 0);
        }
    }

    close(sockfd);
    return 0;
}
