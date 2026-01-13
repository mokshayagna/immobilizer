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
    char recvBuff[1024];

    /* socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    /* server address */
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    /* connect */
    connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    /* requests array */
    char *requests[] = {"Moksha", "yagna"};
    int total_requests = sizeof(requests) / sizeof(requests[0]);

    /* send requests one by one */
    for (int i = 0; i < total_requests; i++)
    {
        send(sockfd, requests[i], strlen(requests[i]), 0);

        retval = recv(sockfd, recvBuff, sizeof(recvBuff), 0);
        recvBuff[retval] = '\0';

        printf("Response %d from server: %s\n", i + 1, recvBuff);
    }

    /* close */
    close(sockfd);

    return 0;
}
