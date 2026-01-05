#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main()
{
	int clientSocket, nBytes, retval = 0;
	char buffer[1024]= "C: I am client, how are you server";
	struct sockaddr_in serverAddr;
	socklen_t addr_size;

	clientSocket = socket(PF_INET, SOCK_DGRAM, 0);

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(7891);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	addr_size = sizeof (serverAddr);

	nBytes = strlen(buffer) + 1;

	retval  = sendto(clientSocket, buffer, nBytes, 0, (struct sockaddr *)&serverAddr, addr_size);
	printf ("C: sendto retval :%d\n", retval);

	nBytes = recvfrom(clientSocket, buffer, 1024, 0, NULL, NULL);

	printf ("C: recvfrom retval :%d\n", nBytes);
	printf ("C: Received from server: %s\n", buffer);

	sleep(3);
	close(clientSocket);
	return 0;
}
