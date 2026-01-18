#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include<time.h>

#define TIMEOUT_SEC 5

int main()
{
    int sockfd = 0; 
    int retval = 0;
    char sendBuff[1024] = "Hello";
    char recvBuffer[1024];

    printf("[INFO] Client start...\n");

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("[DEBUG] socket() = %d\n", sockfd);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    retval = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    printf("[DEBUG] connect() = %d\n", retval);
    if (retval < 0) {
        perror("[ERROR] connect");
        return -1;
    }

    retval = send(sockfd, sendBuff, strlen(sendBuff), 0); //Hello
    printf("[DEBUG] send() returned %d bytes\n", retval);

    time_t start = time(NULL);
    printf("[INFO] Waiting for server reply...\n");


    while (difftime(time(NULL), start) < TIMEOUT_SEC) {

    retval = recv(sockfd, recvBuffer, sizeof(recvBuffer), MSG_DONTWAIT);

    if (retval > 0) {
        recvBuffer[retval] = '\0';
        printf("[INFO] Received from server within %.0f sec: '%s'\n", difftime(time(NULL), start), recvBuffer);
        break;
    }

    if (retval == -1 && errno == EAGAIN) {
        usleep(500000);  // sleep 0.5 sec
        continue;
    }

    printf("[ERROR] recv failed or server closed\n");
    break;
}

// Check if timeout happened
    if (difftime(time(NULL), start) >= TIMEOUT_SEC) {
        printf("[ERROR] TIMEOUT: No reply in %d seconds\n", TIMEOUT_SEC);
    }

    close(sockfd);
    printf("[INFO] Client exit.\n");
    return 0;
}
