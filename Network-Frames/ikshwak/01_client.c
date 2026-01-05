#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sock;
    struct sockaddr_in addr;
    char buffer[1024];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    connect(sock, (struct sockaddr*)&addr, sizeof(addr));

    send(sock, "add 10 20", 9, 0);

    recv(sock, buffer, sizeof(buffer), 0);
    printf("%s\n", buffer);

    close(sock);
    return 0;
}
