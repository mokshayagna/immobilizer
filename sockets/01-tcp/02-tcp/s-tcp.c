#include "stdio.h"
#include "ctype.h"
#include "string.h"
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
	int sockfd = 0, newfd = 0;
	int len = 0;
	int retval = 0, i;
	struct sockaddr_in serv_addr;
	struct sockaddr_in client;
	char sendBuff[1025];
	char recvBuff[1025];

	sockfd = socket(AF_INET,SOCK_STREAM, 0);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(5000);

	bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	listen(sockfd, 10);

	newfd = accept(sockfd, (struct sockaddr*)&client, &len);
	retval = recv(newfd, recvBuff, sizeof(recvBuff), 0);
	recvBuff[retval] = '\0';
	printf("Client reqest :%s, len :%d\n", recvBuff, len);

	for(i = 0; sendBuff[i] = toupper(recvBuff[i]); i++);

	len = strlen(sendBuff);
	printf("S: sendBuff :%s, len :%d\n", sendBuff, len);
	retval = send(newfd, sendBuff, strlen(sendBuff), 0);
	printf("S: send retval :%d\n", retval);

	close(sockfd);
	close(newfd);
}
