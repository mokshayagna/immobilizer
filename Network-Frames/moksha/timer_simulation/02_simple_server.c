#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
int main()
{
    int server_fd, client_fd;
    char recvBuffer[1024];
    char sendBuffer[1024] = "hello";
    char retval;

    /* 1. Create socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    /* 2. Server address */
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(5001);

    /* 3. Bind */
    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    /* 4. Listen */
    listen(server_fd, 5);
    printf("Server waiting...\n");

    /* 5. Accept client */
    client_fd = accept(server_fd, NULL, NULL);
        
    /* 6. Receive data */
    retval = recv(client_fd, recvBuffer, sizeof(recvBuffer), 0);
    recvBuffer[retval] = '\0';
    printf("Server received: %s\n", recvBuffer); //HELLO

    retval = send(client_fd, sendBuffer, strlen(recvBuffer), 0); //hello

    close(client_fd);
    close(server_fd);

    return 0;
}





    