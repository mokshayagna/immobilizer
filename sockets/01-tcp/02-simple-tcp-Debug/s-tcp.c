#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>

#define SERVER_PORT 5000

int main(int argc, char *argv[])
{
    int sockfd = 0, newfd = 0;
    socklen_t len = 0;
    int retval = 0, i;
    struct sockaddr_in serv_addr;
    struct sockaddr_in client;
    char sendBuff[1025];
    char recvBuff[1025];

    printf("S: Server starting...\n");

    printf("S: Creating socket...\n");
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("S: socket creation failed");
        return -1;
    }
    printf("S: Socket created successfully (fd=%d)\n", sockfd);

    printf("S: Setting up server address...\n");
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SERVER_PORT);

    printf("S: Binding to port %d on all interfaces\n", SERVER_PORT);

    retval = bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (retval < 0) {
        perror("S: bind failed");
        close(sockfd);
        return -1;
    }
    printf("S: Bind successful\n");

    printf("S: Listening for incoming connections...\n");
    retval = listen(sockfd, 10);
    if (retval < 0) {
        perror("S: listen failed");
        close(sockfd);
        return -1;
    }

    printf("S: Waiting to accept a client connection...\n");
    len = sizeof(client);
    newfd = accept(sockfd, (struct sockaddr*)&client, &len);
    if (newfd < 0) {
        perror("S: accept failed");
        close(sockfd);
        return -1;
    }

    printf("S: Client connected from %s:%d (fd=%d)\n",
           inet_ntoa(client.sin_addr),
           ntohs(client.sin_port),
           newfd);

    printf("S: Waiting to receive data from client...\n");
    retval = recv(newfd, recvBuff, sizeof(recvBuff) - 1, 0);
    if (retval < 0) {
        perror("S: recv failed");
        close(newfd);
        close(sockfd);
        return -1;
    }

    recvBuff[retval] = '\0';
    printf("S: Received %d bytes from client\n", retval);
    printf("S: Client request : \"%s\"\n", recvBuff);

    printf("S: Processing data (toupper)...\n");
    for (i = 0; recvBuff[i] != '\0'; i++) {
        sendBuff[i] = toupper(recvBuff[i]);
    }
    sendBuff[i] = '\0';

    printf("S: Processed response : \"%s\"\n", sendBuff);

    retval = send(newfd, sendBuff, strlen(sendBuff), 0);
    if (retval < 0) {
        perror("S: send failed");
    } else {
        printf("S: Sent %d bytes to client\n", retval);
    }

    printf("S: Closing client socket\n");
    close(newfd);

    printf("S: Closing server socket\n");
    close(sockfd);

    printf("S: Server exiting normally\n");
    return 0;
}
