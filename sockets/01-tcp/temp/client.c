#include"sys/types.h"
#include"sys/socket.h"
#include"netinet/in.h"
#include"arpa/inet.h"
#include"stdio.h"
#include"stdlib.h"
#include"string.h"
#include"unistd.h"

int main()
{
	int socketfd;
	int connected;
	struct sockaddr_in myclientaddr;
	//char buff1[3][20]={"hi","how are you","bye"};
	char buff1[100];
	char sbuff[1024];
	int wretval = 0,rretval=0,i;

	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if(socketfd == -1)
	{
		perror("--->client: Failed to create a socket \n");
		exit(1);
	}

	myclientaddr.sin_family = AF_INET;
	myclientaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	myclientaddr.sin_port = htons(8000);

	printf("--->client: Connecting to the server\n");
	connected = connect(socketfd, (struct sockaddr *)&myclientaddr, sizeof(myclientaddr));
	if(connected == -1)
	{
		perror("--->client: connect system call failed\n");
		close(socketfd);
		exit(1);
	}
	//for(i=0;i<3;i++)
	while(1)
	{
	    sleep(2);
		gets(buff1);
		
		wretval=write(socketfd, buff1,strlen(buff1));

		if(wretval < 0)
		break;
	    
		printf("--->client: write buff1 retval:      %d\n",wretval);
		printf("--->client: send to server: %s\n",buff1);
		if(strcmp(buff1,"bye")==0)
		break;

		rretval=read(socketfd, sbuff,sizeof(sbuff));


		if(rretval < 0)
	    break;
	    
		sbuff[rretval]='\0';
		printf("--->client: wretval from server:     %d\n",rretval);
		printf("--->client: buffer read from server: %s\n",sbuff);

		if(strcmp(sbuff,"bye")==0)
		break;
	}
	close(socketfd);
	return 0;

}
