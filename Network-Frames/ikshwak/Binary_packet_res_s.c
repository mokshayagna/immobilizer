#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdint.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define ADD 1
#define SUB 2
#define MUL 3
#define DIV 4

#pragma pack(1)
struct packet{
    uint8_t op_code;
    int32_t op1;
    int32_t op2;
    uint8_t checksum;
};
#pragma pack()

#pragma pack(1)
struct response_packet{
    uint8_t op_code;
    int32_t op1;
    int32_t op2;
    int32_t result;
};
#pragma pack()

uint8_t calculate_checksum(uint8_t *data, int len)
{
    uint8_t sum = 0;
    for (int i = 0; i < len; i++)
        sum ^= data[i];
    return sum;
}

void hex_converter(uint8_t *data, int len)
{
    printf("Send buffer (hex): ");
    for (int i = 0; i < len; i++)
        printf("%02X ", data[i]);
    printf("\n");
}

int main(){
    int server_socket, client_socket;
    int ret;
    char sendBuffer[1024];

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(5001);

    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    listen(server_socket, 5);
    printf("Server waiting...\n");

    client_socket = accept(server_socket, NULL, NULL);

    struct packet req;
    ret = recv(client_socket, &req, sizeof(req), 0);

    uint8_t recv_sum = req.checksum;
    req.checksum = 0;

    uint8_t calc_sum = calculate_checksum((uint8_t *)&req, 9);

    if (recv_sum != calc_sum) {
        printf("CHECKSUM NOT MATCHED\n");
        close(client_socket);
        close(server_socket);
        return 0;
    }

    printf("CHECKSUM MATCHED\n");

    uint8_t code = req.op_code;
    int32_t value1 = ntohl(req.op1);
    int32_t value2 = ntohl(req.op2);
    int result = 0;

    switch (code) {
        case ADD: result = value1 + value2; break;
        case SUB: result = value1 - value2; break;
        case MUL: result = value1 * value2; break;
        case DIV: result = (value2 != 0) ? (value1 / value2) : 0; break;
        default:  result = 0;
    }

    struct response_packet res;
    res.op_code = code;
    res.op1 = htonl(value1);
    res.op2 = htonl(value2);
    res.result = htonl(result);

    printf("Server sending:\n");
    printf("Opcode   : %u\n", code);
    printf("Operand1 : %d\n", value1);
    printf("Operand2 : %d\n", value2);
    printf("Result   : %d\n", result);

    hex_converter((uint8_t *)&res, sizeof(res));
    send(client_socket, &res, sizeof(res), 0);

    close(client_socket);
    close(server_socket);
    return 0;
}
