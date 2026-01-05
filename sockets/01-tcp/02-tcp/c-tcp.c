#include <unistd.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>

int main()
{
    int sockfd = 0;
    char sendBuff[1024] = "hello world";
    char recvBuff[1024];
    struct sockaddr_in serv_addr;
    int retval = 0;

    printf("C: Client starting...\n");

    /* 1. Create socket */
    printf("C: Creating socket...\n");
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("C: socket creation failed");
        return -1;
    }
    printf("C: Socket created successfully (fd=%d)\n", sockfd);

    /* 2. Prepare server address */
    printf("C: Setting up server address...\n");
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(5000);

    printf("C: Server IP=127.0.0.1, Port=5000\n");

    /* 3. Connect to server */
    printf("C: Connecting to server...\n");
    retval = connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (retval < 0) {
        perror("C: connect failed");
        close(sockfd);
        return -1;
    }
    printf("C: Connected to server successfully\n");

    /* 4. Send data */
    printf("C: Sending data to server: \"%s\"\n", sendBuff);
    retval = send(sockfd, sendBuff, strlen(sendBuff), 0);
    if (retval < 0) {
        perror("C: send failed");
        close(sockfd);
        return -1;
    }
    printf("C: Sent %d bytes to server\n", retval);

    /* 5. Receive data */
    printf("C: Waiting for server response...\n");
    retval = recv(sockfd, recvBuff, sizeof(recvBuff) - 1, 0);
    if (retval < 0) {
        perror("C: recv failed");
        close(sockfd);
        return -1;
    }

    printf("C: Received %d bytes from server\n", retval);
    recvBuff[retval] = '\0';

    printf("C: Server response : \"%s\"\n", recvBuff);

    /* 6. Close socket */
    printf("C: Closing socket\n");
    close(sockfd);

    printf("C: Client exiting normally\n");
    return 0;
}
