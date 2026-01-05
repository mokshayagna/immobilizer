#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

main()
{
	char buf[100];
	struct sockaddr_in client;
	int sock_desc, k;
	sock_desc = socket(AF_INET, SOCK_STREAM,0);
	memset(&client,0,sizeof(client));
	client.sin_family = AF_INET;
	client.sin_addr.s_addr = inet_addr("192.168.1.130");
	client.sin_port = 7777;
	k = connect(sock_desc, (struct sockaddr*)&client, sizeof(client));
	while(1)
	{
		gets(buf);
		k = send(sock_desc, buf,100,0);
		if(strcmp(buf,"exit")==0)
		break;
	}
close(sock_desc);
return 0;
}
