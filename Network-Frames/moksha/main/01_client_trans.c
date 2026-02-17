#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdint.h>
#include "functions.h"

#define SERVER_PORT 5000

/* ===================== SECRET KEY ===================== */
uint8_t secret_key[16] = {
    0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,
    0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c
};

/* ===================== FRAME FROM SERVER ===================== */
typedef struct {
    uint8_t challenge_counter;   // server-generated
    uint8_t random_number[16];   // 16-byte random
} ChallengeFrame;

typedef struct {
    uint8_t challenge_counter;   // echoed by client
    uint8_t encrypted[16];       // AES(random)
} ResponseFrame;

/* ===================== UTILITY ===================== */
void print_hex(const char *label, uint8_t *data, int len)
{
    printf("%s: ", label);
    for (int i = 0; i < len; i++)
        printf("%02X ", data[i]);
    printf("\n");
}

int main(void)
{
    int sockfd;
    int retval;

    ChallengeFrame frame;
    uint8_t encrypted[16];

    /* Client-side tracking */
    uint8_t latest_counter_received = 0;
    uint8_t counter_used_for_encryption = 0;

    /* ===================== SOCKET SETUP ===================== */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    /* Send START command */
    send(sockfd, "start", 5, 0);
    printf("Transponder ECU started\n");

    int attempt = 0;
    while (1) {

        retval = recv(sockfd, &frame, sizeof(ChallengeFrame), 0);

        if (retval <= 0) {
            printf("Server closed connection\n");
            break;
        }

        attempt++;
        printf("\n==============================\n");
        printf(" TRANSPONDER ATTEMPT %d\n", attempt);
        printf("==============================\n");

        printf("Received Challenge Counter : %u\n", frame.challenge_counter);
        print_hex("Received Random Number    ", frame.random_number, 16);

        /* Encrypt */
        aes128_encrypt(frame.random_number, encrypted, secret_key);
        print_hex("Encrypted", encrypted, 16);

        /* Prepare response */
        ResponseFrame rframe;
        rframe.challenge_counter = frame.challenge_counter;
        memcpy(rframe.encrypted, encrypted, 16);

        printf("\n=== RESPONSE READY ===\n");
        printf("Challenge Counter : %u\n", rframe.challenge_counter);
        print_hex("Encrypted Random ", rframe.encrypted, 16);

        /* ---------------- NON-BLOCKING DELAY ---------------- */

        int delay;
        if (attempt == 1)
            delay = 6;   // simulate long delay
        else
            delay = 1;   // normal delay

        time_t start_time = time(NULL);

        while (1) {
            time_t now = time(NULL);

            if (now - start_time >= delay) {
                send(sockfd, &rframe, sizeof(rframe), 0);

                printf("\n=== RESPONSE SENT AFTER %d sec ===\n", delay);
                break;
            }

            /* Small sleep to avoid CPU spinning */
            usleep(10000);   // 10 ms
        }
    }

    close(sockfd);
    return 0;
}
