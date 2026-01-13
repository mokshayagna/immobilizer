#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <stdlib.h>

typedef struct {
    int op_code;
    char name[4];
}op_structure;

op_structure op_table[] = {
    {1,"ADD"},
    {2,"SUB"},
    {3,"MUL"}
};

int main()
{
    int server_fd, client_fd;
    char recvBuffer[1024];
    char sendBuffer[1024];
    int retval;
    char *token;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(5001);

   
    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    listen(server_fd, 5);
    printf("Server waiting...\n");

    client_fd = accept(server_fd, NULL, NULL);

    retval = recv(client_fd, recvBuffer, sizeof(recvBuffer), 0); 
    recvBuffer[retval] = '\0';
    printf("Server received: %s\n", recvBuffer); //OP=1;A=10;B=20;


    int op,a,b;
    token = strtok(recvBuffer,";"); //op = 1
    if(strncmp(token,"OP=",3)==0){
        op = atoi(token+3); //converts "1" to 1
        token = strtok(NULL,";"); // A=10
        a = atoi(token+2);
        token = strtok(NULL,";"); // A=10
        b = atoi(token+2);
    }

    char *operation;
    for(int i=0;i<3;i++){
        if (op_table[i].op_code == op){            // 1 == 1
            operation = op_table[i].name;   //operation = ADD
            break;
        }
    }

    int result;
    if (strcmp(operation,"ADD")==0){
        result = a+b;
    }

    else if(strcmp(operation,"SUB")==0){
        result = a-b;
    }

    else if(strcmp(operation,"MUL")==0){
        result = a*b;
    }
    
    sprintf(sendBuffer,"OP=%d;A=%d;B=%d;RES=%d",op,a,b,result);
    send(client_fd, sendBuffer, strlen(sendBuffer), 0);
    close(client_fd);
    close(server_fd);
    return 0;
}


  
/**
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>

int main()
{
    int server_fd, client_fd;
    char recvBuffer[1024];
    char sendBuffer[1024];
    int retval;
    char *token;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(5001);


    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    listen(server_fd, 5);
    printf("Server waiting...\n");


    client_fd = accept(server_fd, NULL, NULL);

  
    retval = recv(client_fd, recvBuffer, sizeof(recvBuffer)-1, 0);
    recvBuffer[retval] = '\0';

    printf("Server received: %s\n", recvBuffer);  
 

    int op, a, b;

 
    token = strtok(recvBuffer, ";");     // OP=1
    if (strncmp(token, "OP=", 3) == 0) {
        op = atoi(token + 3);             // extract opcode

        token = strtok(NULL, ";");        // A=10
        a = atoi(token + 2);

        token = strtok(NULL, ";");        // B=20
        b = atoi(token + 2);
    }

  
    int result = 0;

    switch (op) {
        case 1:   // ADD
            result = a + b;
            break;

        case 2:   // SUB
            result = a - b;
            break;

        case 3:   // MUL
            result = a * b;
            break;

        case 4:   // DIV
            if (b != 0)
                result = a / b;
            else
                result = 0;
            break;

        default:
            result = 0;   // invalid opcode
    }

    sprintf(sendBuffer, "OP=%d;A=%d;B=%d;RES=%d", op, a, b, result);
    send(client_fd, sendBuffer, strlen(sendBuffer), 0);

  
    close(client_fd);
    close(server_fd);

    return 0;
}
*/