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
    send(sockfd, sendBuff, strlen(sendBuff), 0);

    retval = recv(sockfd, recvBuffer, sizeof(recvBuffer), 0);
    recvBuffer[retval] = '\0';
    printf("CLinet received: %s\n", recvBuffer);

    close(sockfd);

    return 0;
}
