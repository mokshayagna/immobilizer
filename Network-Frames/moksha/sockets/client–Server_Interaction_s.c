#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main()
{
    int server_fd, client_fd;
    int retval;
    char recvBuffer[1024];

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

    /* 5. Accept client */
    client_fd = accept(server_fd, NULL, NULL);

    /* 6. Receive data in loop */
    while (1)
    {
        retval = recv(client_fd, recvBuffer, sizeof(recvBuffer), 0);
        if (retval <= 0)
            break;

        recvBuffer[retval] = '\0';
        printf("Server received: %s\n", recvBuffer);

        /* END check */
        if (strcmp(recvBuffer, "END") == 0)
        {
            printf("END received. Closing connection.\n");
            break;   // EXIT LOOP, DO NOT SEND
        }

        /* Send response */
        send(client_fd, recvBuffer, retval, 0);
        printf("Sent back to client\n");
    }

    /* 7. Close */
    close(client_fd);
    close(server_fd);

    return 0;
}
