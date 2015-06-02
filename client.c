#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/ioctl.h>

#define BUFF_SIZE 256

int sockfd;

void *listener() {
	int nread;

	while (1) {
		//ioctl(sockfd, FIONREAD, &nread);
		char buf[BUFF_SIZE] = {0};
		nread = read(sockfd, buf, BUFF_SIZE-1);
		printf(":%s", &buf);
		if (nread == 0)
		{
			exit(1);
		}
	}
}

int main() {

	pthread_t a_thread;

	int len;
	struct sockaddr_in address;
	int result;
	char buf[BUFF_SIZE];
	int num;

	pthread_attr_t thread_attr;
	pthread_attr_init(&thread_attr);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);


	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(9735);
	len = sizeof(address);
	result = connect(sockfd, (struct sockaddr *)&address, len);
	if (result == -1) {
	 	perror("oops : client1");
	 	exit(1);
	}
	pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&a_thread, &thread_attr, &listener, (void *)0);

	while (1)
	{
		fgets(buf, BUFF_SIZE, stdin);
		write(sockfd, buf, strlen(buf));
	}
}
