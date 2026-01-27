#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "functions.h"
#define Nb 4
#define Nk 4
#define Nr 10

uint8_t secret_key[16] = {
        0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,
        0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c
    };

/* ===================== MAIN ===================== */

int main()
{
    int sockfd = 0; 
    int retval = 0;
    char sendBuff[1024] = "start";
    uint8_t recvBuffer[16];
    uint8_t value[16];

    /* 1. Create socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    /* 2. Server address */
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    /* 3. Connect */
    connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    /* 4. Send data */
    send(sockfd, sendBuff, strlen(sendBuff), 0);  //sent start to engine ECU
    

   int attempt = 0;

    while (1) {

        retval = recv(sockfd, recvBuffer, sizeof(recvBuffer), 0);

        if (retval <= 0) {
            printf("Server closed connection or error\n");
            break;
        }

        attempt++;
        printf("\n--- Transponder Attempt %d ---\n", attempt);

        printf("Received Random Number: ");
        for (int i = 0; i < retval; i++) {
            printf("%02X ", recvBuffer[i]);
        }
        printf("\n");

        aes128_encrypt(recvBuffer, value, secret_key);

        printf("Encrypted value in Transponder ECU: ");
        for (int i = 0; i < 16; i++) {
            printf("%02X ", value[i]);
        }
        printf("\n");

        /* DEBUG DELAY */
        if (attempt == 1) {
            printf("DEBUG: First attempt → delaying response (>5 sec)\n");
            sleep(6);
        } else {
            printf("DEBUG: Quick response (<5 sec)\n");
            sleep(1);
        }
        printf("Sending Encrypted Reply\n");
        send(sockfd, value, sizeof(value), 0);
    }


    printf("Closing connection.\n");
    close(sockfd);

    return 0;
}