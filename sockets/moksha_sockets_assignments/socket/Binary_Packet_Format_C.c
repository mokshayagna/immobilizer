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
struct response_packet {
    uint8_t  op_code;
    int32_t  op1;
    int32_t  op2;
    int32_t  result;
};
#pragma pack()


struct packet client_frame_request(int op_code,int op1,int op2,int checksum){
    struct packet req_packet;


    req_packet.op_code = op_code;
    req_packet.op1 = op1;
    req_packet.op2 = op2;
    req_packet.checksum = checksum;

    return req_packet;
}

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
    printf("sent buffer (hex): ");
    for (int i = 0; i < len; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
}


int main(){
    int sockfd;
    int retval;
    char recvBuff[1024];
    char sendBuff[1024]; 
    int op_code = 1;
    int op1 = 10;
    int op2 = 20;
    int checksum = 0;
    struct packet req;


    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5001);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    //--------frame created----------//
    req = client_frame_request(op_code, op1, op2, 0);

   //--------converted to network--------//
    req.op1 = htonl(req.op1);
    req.op2 = htonl(req.op2);

    
    req.checksum = calculate_checksum((uint8_t *)&req, 9);

   
    printf("Client sending:\n");
    printf("Opcode   : %u\n", req.op_code);
    printf("Operand1 : %d\n", ntohl(req.op1));
    printf("Operand2 : %d\n", ntohl(req.op2));
    printf("Checksum : %u\n", req.checksum);
    

    hex_converter((uint8_t *)&req,sizeof(req));
    send(sockfd, &req, sizeof(req), 0);
    //------- response structure-----//
    struct response_packet res;
    retval = recv(sockfd, &res, sizeof(res), 0);

    hex_converter((uint8_t *)&res,sizeof(res));

    printf("Response received:\n");
    printf("Opcode   : %u\n", res.op_code);
    printf("Operand1 : %d\n", ntohl(res.op1));
    printf("Operand2 : %d\n", ntohl(res.op2));
    printf("Result   : %d\n", ntohl(res.result));


    close(sockfd);
    return 0;
    

}