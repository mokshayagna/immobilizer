#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    int sockfd = 0; 
    int retval = 0;
    char sendBuff[1024] = "Hello";
    char recvBuffer[1024];

    printf("[INFO] Client starting...\n");

    /* 1. Create socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("[DEBUG] socket() returned sockfd=%d\n", sockfd);

    /* 2. Server address */
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    printf("[DEBUG] Connecting to 127.0.0.1:5000\n");

    /* 3. Connect */
    retval = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    printf("[DEBUG] connect() returned %d\n", retval);
    if (retval < 0) {
        perror("[ERROR] connect failed");
        close(sockfd);
        return -1;
    }
    printf("[INFO] Connected to server\n");

    /* 4. Send data */
    retval = send(sockfd, sendBuff, strlen(sendBuff), 0);
    printf("[DEBUG] send() returned %d bytes, data='%s'\n", retval, sendBuff);

    printf("[INFO] Waiting for reply from server...\n");

    /* 5. Receive response */
    retval = recv(sockfd, recvBuffer, sizeof(recvBuffer), 0);
    printf("[DEBUG] recv() returned %d bytes\n", retval);

    if (retval <= 0) {
        printf("[ERROR] recv failed or server closed connection\n");
        close(sockfd);
        return -1;
    }

    recvBuffer[retval] = '\0';
    printf("[INFO] Client received: '%s'\n", recvBuffer);

    /* 6. Close */
    printf("[INFO] Closing socket...\n");
    close(sockfd);
    printf("[INFO] Client exit.\n");

    return 0;
}
