#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdint.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define ADD 1
#define SUB 2
#define MUL 3
#define DIV 4

#define PROTOCOL_MAGIC   0xABCD
#define PROTOCOL_VERSION 1

#pragma pack(1)
struct packet{
    uint16_t magic;
    uint8_t  version;
    uint8_t  opcode;
    int32_t  op1;
    int32_t  op2;
    uint16_t crc;
};
#pragma pack()

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
    printf("Send buffer (hex): ");
    for (int i = 0; i < len; i++)
        printf("%02X ", data[i]);
    printf("\n");
}

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(5001);

    bind(server_fd, (struct sockaddr*)&server_addr,sizeof(server_addr));

    listen(server_fd, 5);
    printf("Server waiting...\n");

    client_fd = accept(server_fd, NULL, NULL);

    /* ---------- Receive request ---------- */
    struct packet req;
    recv(client_fd, &req, sizeof(req), 0);

    /* ---------- CRC verification ---------- */
    uint16_t received_crc = ntohs(req.crc);
    req.crc = 0;

    uint16_t calc_crc =
        calculate_crc((uint8_t *)&req,
                      sizeof(req));

    if (received_crc != calc_crc) {
        printf("CRC NOT MATCHED\n");
        close(client_fd);
        close(server_fd);
        return 0;
    }

    printf("CRC MATCHED\n");

    /* ---------- Convert to host order ---------- */
    uint16_t magic = ntohs(req.magic);
    uint8_t  version = req.version;
    uint8_t  opcode = req.opcode;
    int32_t  op1 = ntohl(req.op1);             // Big endian ---> Little  (00 00 00 14)----> (14 00 00 00) network --->HOST
    int32_t  op2 = ntohl(req.op2);

    if (magic != PROTOCOL_MAGIC && version != PROTOCOL_VERSION) {
        printf("Invalid protocol\n");
        close(client_fd);
        close(server_fd);
        return 0;
    }

    /* ---------- Perform operation ---------- */
    uint32_t result = 0;

    switch (opcode) {
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

    printf("result:%d\n",result);
    /* ---------- Build response ---------- */
    struct response_packet res;
    res.magic   = htons(PROTOCOL_MAGIC);
    res.version = PROTOCOL_VERSION;
    res.opcode  = opcode;
    res.op1     = htonl(op1);               //Little → Big endian  (14 00 00 00)  --->(00 00 00 14)  HOST---> network
    res.op2     = htonl(op2);
    res.result  = htonl(result);
    res.crc     = 0;

    res.crc = calculate_crc((uint8_t *)&res,
                            sizeof(res));
    res.crc = htons(res.crc);

    printf("Server sending response:\n");
    hex_converter((uint8_t *)&res, sizeof(res));

    send(client_fd, &res, sizeof(res), 0);

    close(client_fd);
    close(server_fd);
    return 0;
}
