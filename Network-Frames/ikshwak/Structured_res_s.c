#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <stdlib.h>

int main()
{
    int server_socket, client_socket;
    char recvBuff[1024];
    char sendBuff[1024];
    int ret;
    char *ptr;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(5000);

    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    listen(server_socket, 5);
    printf("Server waiting...\n");

    client_socket = accept(server_socket, NULL, NULL);

    ret = recv(client_socket, recvBuff, sizeof(recvBuff), 0);
    recvBuff[ret] = '\0';
    printf("Server received: %s\n", recvBuff);

    ptr = strtok(recvBuff, ";");          // OP=ADD
    if (ptr != NULL && strstr(ptr, "ADD") != NULL) {

        ptr = strtok(NULL, ";");          // A=10
        int x = atoi(ptr + 2);

        ptr = strtok(NULL, ";");          // B=20
        int y = atoi(ptr + 2);

        sprintf(sendBuff, "OP=ADD;A=%d;B=%d;RES=%d", x, y, x + y);
    }

    send(client_socket, sendBuff, strlen(sendBuff), 0);

    close(client_socket);
    close(server_socket);
    return 0;
}
