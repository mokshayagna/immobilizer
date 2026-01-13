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
    char retval;

    /* 1. Create socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    /* 2. Server address */
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(5000);

    /* 3. Bind */
    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    /* 4. Listen */
    listen(server_fd, 5);
    printf("Server waiting...\n");
    while(1){
        /* 5. Accept client */
        client_fd = accept(server_fd, NULL, NULL);
        printf("client connected");
        while(1){
        
        /* 6. Receive data */
        retval = recv(client_fd, recvBuffer, sizeof(recvBuffer), 0);
        if(retval <= 0){
            break;
        }
        recvBuffer[retval] = '\0';

            /* END condition */
            if (strcmp(recvBuffer, "END") == 0)
            {
                printf("Client sent END\n");
                break;
            }
            /* Send response */
            send(client_fd, recvBuffer, retval, 0);
        }

        /* 5. Close client socket */
        close(client_fd);
        printf("Client disconnected\n");
    }

    /* 6. Close server socket (never reached) */
    close(server_fd);
    return 0;
}
