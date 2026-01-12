#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdint.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define PROTOCOL_MAGIC   0xABCD
#define PROTOCOL_VERSION 1


#pragma pack(1)
struct packet{
    uint16_t magic;     // Protocol ID
    uint8_t  version;   // Protocol version
    uint8_t  opcode;    
    int32_t  op1;
    int32_t  op2;
    uint16_t crc;       // crc value
};
#pragma pack()

struct packet client_frame_request(int opcode,int op1,int op2,int crc){
    struct packet req;

    req.magic = PROTOCOL_MAGIC;
    req.version = PROTOCOL_VERSION;
    req.opcode = opcode;
    req.op1 = op1;
    req.op2 = op2;
    req.crc = crc;

    return req;
}


#pragma pack(1)
struct response_packet{
    uint16_t magic;
    uint8_t  version;
    uint8_t  opcode;
    int32_t  op1;
    int32_t  op2;
    uint16_t crc;
    uint32_t result;
};
#pragma pack()


uint16_t calculate_crc(uint8_t *data, int len)
{
    uint16_t crc = 0;
    for (int i = 0; i < len; i++)
        crc ^= data[i];
    return crc;
}

void hex_converter(uint8_t *data, int len)
{
    printf("sent buffer (hex): ");
    for (int i = 0; i < len; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

int main(){
    struct sockaddr_in server_addr;
    int retval;
    int sockfd; 
    int opcode = 1;
    int op1 = 10;
    int op2 = 20;
    int crc = 0;

    /* ---- Create socket ---- */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5001);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr *)&server_addr,sizeof(server_addr));


    struct packet req = client_frame_request(opcode, op1, op2,crc);

    req.magic = htons(req.magic);
    req.op1   = htonl(req.op1);
    req.op2   = htonl(req.op2);

    req.crc = calculate_crc((uint8_t *)&req, sizeof(req));
    req.crc = htons(req.crc);

    printf("Client sending:\n");
    printf("Opcode   : %u\n", req.opcode);
    printf("Operand1 : %d\n", ntohl(req.op1));
    printf("Operand2 : %d\n", ntohl(req.op2));
    printf("Crc : %u\n", ntohs(req.crc));

    printf("Client sending packet (hex):\n");
    hex_converter((uint8_t *)&req, sizeof(req));

    /* ---- Send packet ---- */
    send(sockfd, &req, sizeof(req), 0);

    struct response_packet res;
    retval = recv(sockfd, &res, sizeof(res), 0);

    hex_converter((uint8_t *)&res,sizeof(res));

    printf("Response received:\n");
    printf("Opcode   : %u\n", req.opcode);
    printf("Operand1 : %d\n", ntohl(res.op1));
    printf("Operand2 : %d\n", ntohl(res.op2));
    printf("Result   : %d\n", ntohl(res.result));

    close(sockfd);
    return 0;

}
