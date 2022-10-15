#include "head.h"

int sockfd = 0;
char tmpbuff[1024] = {0};
ssize_t nsize = 0;
int ret = 0;

void *send1(void *arg)
{
	struct sockaddr_in recvaddr;
	recvaddr.sin_family = AF_INET;
	recvaddr.sin_port = htons(50000);
	recvaddr.sin_addr.s_addr = inet_addr("169.254.7.188");

	while (1)
	{
		memset(tmpbuff, 0, sizeof(tmpbuff));
		gets(tmpbuff);
		nsize = sendto(sockfd, tmpbuff, sizeof(tmpbuff), 0, (struct sockaddr *)&recvaddr, sizeof(recvaddr));
		if (-1 == nsize)
		{
			perror("fail to sendto");
			return NULL;
		}
		if (!strcmp(tmpbuff, "quit"))
		{
			break;
		}
		printf("已发送\n");
	}

}
void *recv2(void *arg)
{

	struct sockaddr_in sendaddr;
	socklen_t addrlen = sizeof(sendaddr);
	sendaddr.sin_family = AF_INET;
	sendaddr.sin_port = htons(50002);
	sendaddr.sin_addr.s_addr = inet_addr("169.254.7.188");

	ret = bind(sockfd, (struct sockaddr *)&sendaddr, sizeof(sendaddr));
	if (-1 == ret)
	{
		perror("fail to bind");
		return NULL;
	}

	while (1)
	{
		memset(tmpbuff, 0, sizeof(tmpbuff));
		nsize = recvfrom(sockfd, tmpbuff, sizeof(tmpbuff), 0, (struct sockaddr *)&sendaddr, &addrlen);
		if (!strcmp(tmpbuff, "quit"))
		{
			break;
		}
		if (-1 == nsize)
		{
			perror("fail to recvaddr");
			return NULL;
		}
		printf("[%s/%d]: %s\n", inet_ntoa(sendaddr.sin_addr), ntohs(sendaddr.sin_port), tmpbuff);
	}

	close(sockfd);
}
int main(int argc, const char *argv[])
{

	pthread_t tid1;
	pthread_t tid2;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == sockfd)
	{
		perror("fail to sockfd");
		return -1;
	}

	pthread_create(&tid2, NULL, recv2, NULL);
	pthread_create(&tid1, NULL, send1, NULL);
	pthread_join(tid2, NULL);
	pthread_join(tid1, NULL);
//	pthread_create(&tid1, NULL, send1, NULL);
//	pthread_create(&tid2, NULL, recv2, NULL);
	

//	pthread_cancel(tid1);
//	pthread_cancel(tid2);


	return 0;
}

