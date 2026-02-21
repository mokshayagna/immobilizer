#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdint.h>
#include "functions.h"

#define SERVER_PORT 5001

uint8_t secret_key[16] = {
    0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,
    0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c
};

typedef struct {
    uint8_t challenge_counter;
    uint8_t random_number[16];
} ChallengeFrame;

typedef struct {
    uint8_t challenge_counter;
    uint8_t encrypted[16];
} ResponseFrame;

void print_hex(const char *label, uint8_t *data, int len)
{
    printf("[DEBUG] %s : ", label);
    for (int i = 0; i < len; i++)
        printf("%02X ", data[i]);
    printf("\n");
}

void handle_authentication(int sockfd)
{
    ChallengeFrame frame;
    ResponseFrame rframe;
    uint8_t encrypted[16];

    recv(sockfd, &frame, sizeof(frame), 0);

    printf("[CLIENT] Challenge Counter: %u\n", frame.challenge_counter);
    print_hex("Random", frame.random_number, 16);

    aes128_encrypt(frame.random_number, encrypted, secret_key);
    print_hex("Encrypted", encrypted, 16);

    rframe.challenge_counter = frame.challenge_counter;
    memcpy(rframe.encrypted, encrypted, 16);

    sleep(1);
    send(sockfd, &rframe, sizeof(rframe), 0);
    printf("[CLIENT] Response sent\n");
}

int main(void)
{
    int sockfd;

    printf("[CLIENT] Transponder ECU starting...\n");

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    printf("[CLIENT] Connected to Engine ECU\n");

    /* ================= INITIAL START ================= */
    printf("\n=============================\n");
    printf(" CASE 0 → INITIAL AUTH\n");
    printf("=============================\n");

    printf("[DRIVER] START\n");
    send(sockfd, "start", 5, 0);

    handle_authentication(sockfd);

    /* ================= CASE 1 ================= */
    printf("\n=============================\n");
    printf(" CASE 1 → Restart BEFORE 6 sec\n");
    printf("=============================\n");

    sleep(2);   // within post-release window

    printf("[DRIVER] START (within 6 sec)\n");
    send(sockfd, "start", 5, 0);

    printf("[CLIENT] Post-release VALID → No challenge expected\n");
    sleep(3);   // simulate engine ON/OFF

    /* ================= CASE 2 ================= */
    printf("\n=============================\n");
    printf(" CASE 2 → Restart AFTER 6 sec\n");
    printf("=============================\n");

    sleep(7);   // exceed post-release timeout

    printf("[DRIVER] START (after 6 sec)\n");
    send(sockfd, "start", 5, 0);

    printf("[CLIENT] Post-release INVALID → Authentication required\n");

    handle_authentication(sockfd);

    printf("\n[CLIENT] Demo complete ✅\n");

    close(sockfd);
    return 0;
}