#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/ioctl.h>

#define BUFF_SIZE 256

int main()
{
	int server_sockfd, client_sockfd;
	int server_len, client_len;
	struct sockaddr_un server_address;
	struct sockaddr_un client_address;
	int result;
	fd_set readfds, testfds;
	char buf[BUFF_SIZE];
	int FD_MAX;
	
	unlink("server_socket");
	server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	server_address.sun_family = AF_UNIX;
	strcpy(server_address.sun_path, "server_socket");
	server_len = sizeof(server_address);
	bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
	
	listen(server_sockfd, 5);
	
	FD_ZERO(&readfds);
	FD_SET(server_sockfd, &readfds);

	while(1) {
		int fd;
		int nread;
		
		testfds = readfds;
		
		printf("server waiting\n");
		
		result = select(FD_SETSIZE, &testfds, (fd_set *)0, (fd_set *)0, (struct timeval *) 0);
		if (result < 1) {
			perror("server");
			exit(1);
		}
		
		for (fd = 0; fd < FD_SETSIZE; fd++) {
			if (FD_ISSET(fd, &testfds)) {
				if (fd == server_sockfd) {
				 	client_len = sizeof(client_address);
					client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
					FD_SET(client_sockfd, &readfds);
					printf("adding client on fd %d\n", client_sockfd);
				}
				else {
					ioctl(fd, FIONREAD, &nread);
					if (nread == 0) {
						close(fd);
						FD_CLR(fd, &readfds);
						printf("removing client on fd %d\n", fd);
					}
					else {
						read(fd, &buf, strlen(buf));
						for (fd = 0; fd < FD_SETSIZE; fd++) {
							
						//sleep(1);
							//printf ("serving client on fd %d\n", fd);
							write(fd, &fd, sizeof(int));
						//ch++;
//						write(fd, fd, sizeof(fd));
							write(fd, &buf, strlen(buf));
						}
					}
				}
			}
		}
	}
}
		
