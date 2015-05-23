/*#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
int main() {
 int sockfd;
 int len;
 struct sockaddr_in address;
 int result;
 char ch = 'A';
 sockfd = socket(AF_INET, SOCK_STREAM, 0);
 address.sin_family = AF_INET;
 address.sin_addr.s_addr = inet_addr("127.0.0.1");
 address.sin_port = 9734;
 len = sizeof(address);
 listen(server_sockfd, 5);
 while(1) {
 char ch;
 printf("server waiting\n");
 client_len = sizeof(client_address);
 client_sockfd = accept(server_sockfd,
 (struct sockaddr *)&client_address, &client_len);
 read(client_sockfd, &ch, 1);
 ch++;
 write(client_sockfd, &ch, 1);
 close(client_sockfd);
 }
}*/

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
	int sockfd;
	int len;
	struct sockaddr_un address;
	int result;
	//char ch = 'A';
	char buf[100];
	printf(":");
	scanf("%s", &buf);
	
	sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	address.sun_family = AF_UNIX;
	strcpy(address.sun_path, "server_socket");
	len = sizeof(address);
	result = connect(sockfd, (struct sockaddr *)&address, len);
	if (result == -1) {
	 	perror("oops : client1");
	 	exit(1);
	}
	write(sockfd, &buf, strlen(buf));
	read(sockfd, &buf, strlen(buf));
	printf("char from server = %s\n", &buf);
	close(sockfd);
	exit(0);
}
