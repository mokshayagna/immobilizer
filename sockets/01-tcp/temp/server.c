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
	struct sockaddr_in mysockaddr;
	int socketfd;
	//char buff1[3][20]={"hellow","i am fine","bye"};
	char buff1[100];
	char buffer[100];
	int newfd;
    int wretval = 0,rretval = 0,i;
	socketfd = socket(AF_INET, SOCK_STREAM,0);
	if(socketfd == -1)
	{
		perror("failed to  create a socket \r\n");
		exit(1);
	}

	memset(&mysockaddr, 0, sizeof(mysockaddr));

	mysockaddr.sin_family = AF_INET;
	mysockaddr.sin_port = htons(8000);
	mysockaddr.sin_addr.s_addr = INADDR_ANY;

	if(bind(socketfd, (struct sockaddr *)&mysockaddr, sizeof(mysockaddr)) == -1)
	{
		perror("bind system call failed\r\n");
		close(socketfd);
		exit(1);
	}

	if(listen(socketfd, 5) == -1)
	{
		perror("listen system call failed\r\n");
		close(socketfd);
		exit(1);
	}

	printf("--->server : Going for accepting connection \r\n");
	newfd = accept(socketfd,NULL,NULL);
	if(newfd == -1)
	{
		perror("accept system call failed\r\n");
		close(socketfd);
		exit(1);
	}

	printf("--->server : Got the connection request from client \r\n\n");
	while(1)
	{
			sleep(2);
	        rretval = read(newfd, buffer,sizeof(buffer));			
			
			if(rretval < 0)
			break;
	    	
			buffer[rretval]='\0';
			printf("--->server: buff1 read retval : %d\n",rretval);
	        printf("--->server:  receive from client :%s\r\n",buffer);
			
			if(strcmp(buffer,"bye")==0)
			break;
			
			gets(buff1);

            wretval=write(newfd, buff1, strlen(buff1));
			
			if(wretval < 0)
		    break;
		    
			printf("--->server: buff1 write retval : %d\n",wretval);
	        printf("--->server:  send to client :%s\r\n",buff1);

			if(strcmp(buff1,"bye")==0)
			break;
	}
	close(socketfd);
	close(newfd);
	return 0;

}

