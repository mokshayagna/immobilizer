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
    int retval;

    printf("[INFO] Starting server on port 5001...\n");

    /* 1. Create socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    printf("[DEBUG] socket() returned %d\n", server_fd);

    /* 2. Server address */
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(5000);
    printf("[DEBUG] Server IP ANY, Port 5000\n");

    /* 3. Bind */
    retval = bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    printf("[DEBUG] bind() returned %d\n", retval);

    /* 4. Listen */
    retval = listen(server_fd, 5);
    printf("[DEBUG] listen() backlog=5, returned %d\n", retval);

    printf("[INFO] Server waiting for client...\n");

    /* 5. Accept client */
    client_fd = accept(server_fd, NULL, NULL);
    printf("[DEBUG] accept() returned client fd=%d\n", client_fd);

    /* 6. Receive data */
    retval = recv(client_fd, recvBuffer, sizeof(recvBuffer), 0);
    printf("[DEBUG] recv() returned %d bytes\n", retval);

    if (retval <= 0) {
        printf("[ERROR] recv failed or connection closed\n");
        close(client_fd);
        close(server_fd);
        return -1;
    }

    recvBuffer[retval] = '\0';
    printf("[INFO] Server received: '%s'\n", recvBuffer);

    printf("[INFO] Sleeping for 15 seconds...\n");
    sleep(15);
    printf("[INFO] Sleep done.\n");

    /* 7. Send data */
    retval = send(client_fd, sendBuffer, strlen(sendBuffer), 0);
    printf("[DEBUG] send() returned %d bytes\n", retval);

    printf("[INFO] Closing sockets...\n");
    close(client_fd);
    close(server_fd);

    printf("[INFO] Server exit.\n");
    return 0;
}
