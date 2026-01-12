#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <stdlib.h>

typedef struct {
    int code;
    char name[4];
} op_structure;

op_structure op_table[] = {
    {1, "ADD"},
    {2, "SUB"},
    {3, "MUL"}
};

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
    server_addr.sin_port = htons(5001);

    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    listen(server_socket, 5);
    printf("Server waiting...\n");

    client_socket = accept(server_socket, NULL, NULL);

    ret = recv(client_socket, recvBuff, sizeof(recvBuff), 0);
    recvBuff[ret] = '\0';
    printf("Server received: %s\n", recvBuff);

    int op, a, b;
    ptr = strtok(recvBuff, ";");              // OP=1
    if (strncmp(ptr, "OP=", 3) == 0) {
        op = atoi(ptr + 3);

        ptr = strtok(NULL, ";");              // A=10
        a = atoi(ptr + 2);

        ptr = strtok(NULL, ";");              // B=20
        b = atoi(ptr + 2);
    }

    char *operation;
    for (int i = 0; i < 3; i++) {
        if (op_table[i].code == op) {
            operation = op_table[i].name;
            break;
        }
    }

    int result;
    if (strcmp(operation, "ADD") == 0)
        result = a + b;
    else if (strcmp(operation, "SUB") == 0)
        result = a - b;
    else if (strcmp(operation, "MUL") == 0)
        result = a * b;

    sprintf(sendBuff, "OP=%d;A=%d;B=%d;RES=%d", op, a, b, result);

    send(client_socket, sendBuff, strlen(sendBuff), 0);

    close(client_socket);
    close(server_socket);
    return 0;
}
