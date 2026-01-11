#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(){
    int client_fd;
    int ret;
    char recvBuffer[1024];
    char sendBuffer[1024];

    char op[] = "ADD";
    char a[] = "10";
    char b[] = "20";

    sprintf(sendBuffer, "OP=%s;A=%s;B=%s;", op, a, b);
    printf("sendBuff:%s\n", sendBuffer);

    client_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    send(client_fd, sendBuffer, strlen(sendBuffer), 0);

    ret = recv(client_fd, recvBuffer, sizeof(recvBuffer), 0);
    recvBuffer[ret] = '\0';

    printf("Client received: %s\n", recvBuffer);

    close(client_fd);
    return 0;
}
