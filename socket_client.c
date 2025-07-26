/* sockets.c */

/* standard libraries  */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define IP_ADDRESS "74.6.143.26" /* yahoo.com */
#define PORT 80 /* http */

int main () {
	int sockfd; /* file descriptor for the socket */
	
	struct sockaddr_in serverAddress;

	char *request, response[512];

	sockfd = socket(AF_INET, SOCK_STREAM, 0); /* creates the TCP socket */

	request = "HEAD / HTTP/1.0\r\n\r\n"; /* HTTP HEAD request */
	
	/* checks if the socket creation failed */
	if (sockfd < 0) {
		printf("socket() error\n");
		return -1;
	}

	serverAddress.sin_addr.s_addr = inet_addr(IP_ADDRESS);  /* sets server IP address (network byte order) */
	serverAddress.sin_port = htons(PORT);                   /* sets server port (network byte order) */
	serverAddress.sin_family = AF_INET;                     /* sets address family to IPv4 */
	
	/* tries to connect to the server */
	if (connect(sockfd, (struct sockaddr *) &serverAddress, sizeof(struct sockaddr_in)) == -1) {
		printf("connect() error\n");

		close(sockfd); /* closes the socket if connection fails */

		return -1;
	}

	write(sockfd, request, strlen(request)); /* sends the HTTP request to the server */

	memset(response, 0, 512); /* clears the response buffer */

	read(sockfd, response, 511); /* reads the response from the server */

	close(sockfd); /* closes the socket after communication is complete */

	printf("\n%s\n", response);

	return 0;
}
