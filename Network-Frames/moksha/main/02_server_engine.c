#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <errno.h>
#include<time.h>
#define Nb 4
#define Nk 4
#define Nr 10
#define PER_CHALLENGE_TIMEOUT 5// seconds
#define TOTAL_AUTH_TIMEOUT     15

uint8_t secret_key[16] = {
        0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,
        0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c
    };

int main()
{
    int server_fd, client_fd;
    char recvBuffer[1024];
    uint8_t sendBuffer[16];
    uint8_t value[16];
    uint8_t rec_value[16];
    int retval;
    int waited_time = 0;
    int last_send = 0;

    /* 1. Create socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    /* 2. Server address */
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(5000);

    /* 3. Bind */
    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    /* 4. Listen */
    listen(server_fd, 5);
    printf("Server waiting...\n");

    /* 5. Accept client */
    client_fd = accept(server_fd, NULL, NULL);
        
    /* 6. Receive data */
    retval = recv(client_fd, recvBuffer, sizeof(recvBuffer), 0);
    recvBuffer[retval] = '\0';
    printf("Server received: %s\n", recvBuffer);

    if(strcmp(recvBuffer, "start") == 0)
    {
        printf("Engine ECU started processing the request.\n");
    }
    time_t total_start = time(NULL);
    int authenticated = 0;
    int attempts = 0;

    while (difftime(time(NULL), total_start) < TOTAL_AUTH_TIMEOUT) {
        attempts++;
        printf("\n--- Challenge Attempt %d ---\n", attempts);
        trng(sendBuffer, 16);

        printf("Generated Random Number: ");
        for (int i = 0; i < 16; i++) {
            printf("%02X ", sendBuffer[i]);
        }
        printf("\n");

        send(client_fd, sendBuffer, 16, 0);

        aes128_encrypt(sendBuffer, value, secret_key);

        printf("Encrypted aes-128 value in server: ");
        for (int i = 0; i < 16; i++) {
            printf("%02X ", value[i]);
        }
        printf("\n");

        /* -------- Wait for Response (PER CHALLENGE) -------- */
        time_t challenge_start = time(NULL);

        while (difftime(time(NULL), challenge_start) < PER_CHALLENGE_TIMEOUT) {

            retval = recv(client_fd, rec_value, sizeof(rec_value), MSG_DONTWAIT);

            if (retval > 0) {
                printf("Received reply within %.0f sec\n",
                    difftime(time(NULL), challenge_start));

                printf("Encrypted reply (hex): ");
                for (int i = 0; i < retval; i++) {
                    printf("%02X ", (unsigned char)rec_value[i]);
                }
                printf("\n");

                /* -------- Verify AES Response -------- */
                if (retval == 16 && memcmp(rec_value, value, 16) == 0) {
                    printf("AUTHENTICATION SUCCESS\n");
                    authenticated = 1;
                    break;
                }
            }

            if (retval == -1 && errno == EAGAIN) {
                usleep(500000); // 0.5 sec
                continue;
            }

            if (retval == 0) {
                printf("Client closed connection\n");
                break;
            }
        }

        if (authenticated)
            break;

        printf("No reply in %d sec → Sending NEW challenge\n",
            PER_CHALLENGE_TIMEOUT);
    }

    /* -------- Final Result -------- */
    if (!authenticated) {
        printf("TOTAL AUTH TIMEOUT (%d sec) → ACCESS DENIED\n",
            TOTAL_AUTH_TIMEOUT);
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
