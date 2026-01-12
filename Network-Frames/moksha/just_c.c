#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    int sockfd;
    int retval;
    char recvBuff[1024];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    char *res[] = {"sweety","moksha","END"};
    int total = sizeof(res) / sizeof(res[0]);

    for (int i = 0; i < total; i++)
    {
        if (strcmp(res[i], "END") == 0)
            break;

        send(sockfd, res[i], strlen(res[i]), 0);
        printf("Client sent: %s\n", res[i]);

        retval = recv(sockfd, recvBuff, sizeof(recvBuff), 0);
        recvBuff[retval] = '\0';
        printf("Server replied: %s\n", recvBuff);
    }

    close(sockfd);
    return 0;
}
