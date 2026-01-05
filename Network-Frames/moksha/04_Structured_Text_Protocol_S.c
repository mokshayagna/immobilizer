#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <stdlib.h>

int main()
{
    int server_fd, client_fd;
    char recvBuffer[1024];
    char sendBuffer[1024];
    int retval;
    char *token;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(6001);

   
    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    listen(server_fd, 5);
    printf("Server waiting...\n");

    client_fd = accept(server_fd, NULL, NULL);

    retval = recv(client_fd, recvBuffer, sizeof(recvBuffer), 0);
    recvBuffer[retval] = '\0';
    printf("Server received: %s\n", recvBuffer);


    //strtok is an inbuilt function which split string into tokens(words) based on space or something mentioned//
    token = strtok(recvBuffer, ";");  // OP=ADD
    if (strstr(token, "ADD") != NULL){  //ADD in token
        token = strtok(NULL,";");         //A=10
        int a = atoi(token+2);           
        token = strtok(NULL,";");               // B=20
        int b = atoi(token+2);

        sprintf(sendBuffer,"OP=ADD;A=%d;B=%d;RES=%d",a,b,a+b);
    }
    
    send(client_fd, sendBuffer, strlen(sendBuffer), 0);
    close(client_fd);
    close(server_fd);
    return 0;
}


    