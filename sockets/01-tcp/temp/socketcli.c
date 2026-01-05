#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

main()
{
	int sock_desc = socket(AF_INET, SOCK_STREAM, 0);
	char sbuff[1024];
	if(sock_desc == -1)
	{
		printf("cant create socket\n");
		return 0;
	}
	struct sockaddr_in client;
	memset(&client , 0, sizeof(client));
	client.sin_family = AF_INET;
	client.sin_addr.s_addr = inet_addr("192.168.1.30");
	client.sin_port = htons(7777);

	if(connect(sock_desc , (struct sockaddr*)&client, sizeof(client))==-1)
	{
		printf("cant connect to server\n");
		close(sock_desc);
		exit(1);
	}
char buf[3][100]={"hi","how are you","exit"};
char c = '\n';
char *p_buf;
int k,len;

while(1)
{
	gets(buf);
	len = strlen(buf);
	p_buf = buf;

	while(len>0);
	{
		k = write(sock_desc, p_buf,len);
		if(k == -1)
		{
			printf("cant write to server!\n");
			break;
		}
	p_buf+=k;
	len-=k;
	}
	k = read(sock_desc,&sbuff,sizeof(sbuff));
	if(k == -1)
	{
		printf("cant write to server!\n");
		break;
	}
	sbuff[k]='\0';
	if(strcmp(buf, "exit")==0)
	break;
}
close(sock_desc);
printf("client disconnected\n");
return 0;
}
