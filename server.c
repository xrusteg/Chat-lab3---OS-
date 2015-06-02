#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUFF_SIZE 256

char clients[100];
int num_clients;
int fd;
void *sender() {
	//int fd;
	int i, nread;
	char buf[BUFF_SIZE] = {0};
	printf("INT THREAD\n");
	//ioctl(fd, FIONREAD, &nread);
	read(fd, buf, BUFF_SIZE-1);
	for (i = 0; i < num_clients; i++) {
		printf(":%s\n", &buf);
		write(clients[i], &buf, strlen(buf));
	}
}
int main()
{
	pthread_t a_thread;
	int server_sockfd, client_sockfd;
	int server_len, client_len;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;
	int result, i;
	fd_set readfds, testfds;
	char clients[100];
	num_clients = 0;
	int FD_MAX;
	//int fd;
	
	pthread_attr_t thread_attr;
	pthread_attr_init(&thread_attr);
	
	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl (INADDR_ANY);
	server_address.sin_port = htons(9735);
	server_len = sizeof(server_address);
	bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
	
	listen(server_sockfd, 5);

	FD_ZERO(&readfds);
	FD_SET(server_sockfd, &readfds);

	pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&a_thread, &thread_attr, &sender, (void *)0);

	while(1) {

		int nread;
		
		testfds = readfds;
		
		printf("server waiting\n");
		
		result = select(FD_SETSIZE, &testfds, (fd_set *)0, (fd_set *)0, (struct timeval *) 0);
		if (result < 1) {
			printf("SELECT\n");
			perror("server");
			
			//exit(1);
		}
		
		for (fd = 0; fd < FD_SETSIZE; fd++) {
			if (FD_ISSET(fd, &testfds)) {
				if (fd == server_sockfd) {
				 	client_len = sizeof(client_address);
					client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
					FD_SET(client_sockfd, &readfds);
					clients[num_clients] = client_sockfd;
					num_clients++;
					printf("adding client on fd %d\n", client_sockfd);
				}
				else {
					//sender();
					/*//ioctl(fd, FIONREAD, &nread);
					char buf[BUFF_SIZE] = {0};
					//if (nread == 0) {
						//close(fd);
						//FD_CLR(fd, &readfds);
						//printf("removing client on fd %d\n", fd);
					//}
					//else {
						//ioctl(fd, FIONREAD, &nread);

						read(fd, buf, BUFF_SIZE-1);
						for (i = 0; i < num_clients; i++) {
						//strcpy(buf, "qwe");							
						//sleep(1);
						//printf("!\n");
							printf(":%s\n", &buf);
							//write(fd, &fd, sizeof(int));
						//ch++;
//						write(fd, fd, sizeof(fd));
							//sleep(1);
							write(clients[i], &buf, strlen(buf));*/
						}
					//}
				}
			}
		}
	}


		
