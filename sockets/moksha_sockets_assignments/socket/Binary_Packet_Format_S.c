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


uint8_t calculate_checksum(uint8_t *data, int length)
{
    uint8_t checksum = 0;

    for (int i = 0; i < length; i++) {
        checksum ^= data[i];
    }

    return checksum;
}

void hex_converter(uint8_t *data, int len)
{
    printf("Send buffer (hex): ");
    for (int i = 0; i < len; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

int main(){
    int sockfd;
    int retval; 
    char sendBuff[1024]; 
    int server_fd, client_fd;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(5001);

   
    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    listen(server_fd, 5);
    printf("Server waiting...\n");

    client_fd = accept(server_fd, NULL, NULL);


    // ---------- structure ----------//
    struct packet req;
    retval= recv(client_fd, &req, sizeof(req), 0); 

    uint8_t received_checksum = req.checksum;
    req.checksum = 0;

    uint8_t checksum = calculate_checksum((uint8_t *)&req, 9);

    if(received_checksum != checksum){
        printf("CHECKSUM NOT MATCHED\n");
        close(client_fd);
        close(server_fd);
        return 0;
    }
    printf("CHECKSUM MATCHED");
    // -------- convert to little endian-----//
    uint8_t  op_code = req.op_code;
    int32_t  op1 = ntohl(req.op1);
    int32_t  op2 = ntohl(req.op2);
    int result = 0;

    switch (op_code) {
        case ADD:
            result = op1 + op2;
            break;
        case SUB:
            result = op1 - op2;
            break;
        case MUL:
            result = op1 * op2;
            break;
        case DIV:
            result = (op2 != 0) ? (op1 / op2) : 0;
            break;
        default:
            result = 0;
    }
    struct response_packet res;
    res.op_code = op_code;
    res.op1 = op1;
    res.op2 = op2;
    res.result = result;
    
    res.op1 = htonl(res.op1);
    res.op2 = htonl(res.op2);
    res.result = htonl(result);

    printf("Server sending:\n");
    printf("Opcode   : %u\n", req.op_code);
    printf("Operand1 : %d\n", ntohl(req.op1));
    printf("Operand2 : %d\n", ntohl(req.op2));
    printf("result : %u\n", result);

    hex_converter((uint8_t *)&res,sizeof(res));
    send(client_fd, &res, sizeof(res), 0);
    
    close(client_fd);
    close(server_fd);

    return 0;
}