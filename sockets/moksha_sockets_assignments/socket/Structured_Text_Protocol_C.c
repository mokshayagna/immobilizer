#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(){
    int sockfd;
    int retval;
    char recvBuff[1024];
    char sendBuff[1024]; 
    char x[]= "ADD";
    char y[] = "10";
    char z[]= "20";
    sprintf(sendBuff, "OP=%s;A=%s;B=%s;", x, y, z);
    printf("sendBuff:%s\n",sendBuff);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    send(sockfd, sendBuff, strlen(sendBuff), 0);
    
    retval = recv(sockfd, recvBuff, sizeof(recvBuff), 0);
    recvBuff[retval] = '\0';

    printf("Client received: %s\n", recvBuff);

    close(sockfd);
    return 0;

}