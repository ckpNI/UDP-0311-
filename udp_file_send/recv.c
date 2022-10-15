#include "head.h"

int UdpRecvFile(const char *pIp, int port)
{
	int sockfd = 0;
	struct sockaddr_in recvaddr;
	ssize_t nsize = 0;
	int  ret = 0;
	char tmp[1024] = {0};
	char filename[256] = {0};
	socklen_t addrlen = 0;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == sockfd)
	{
		perror("fail to socket");
		return -1;
	}

	recvaddr.sin_family = AF_INET;
	recvaddr.sin_port = htons(port);
	recvaddr.sin_addr.s_addr = inet_addr(pIp);
	ret = bind(sockfd, (struct sockaddr *)&recvaddr, sizeof(recvaddr));	
	if (-1 == ret)
	{
		perror("fail to bind");
		return -1;
	}

	recvfrom(sockfd, tmp, sizeof(tmp), 0, (struct sockaddr *)&recvaddr, &addrlen);
	strcpy(filename, tmp);

	FILE *fp = NULL;
	fp = fopen(filename, "w");
	if (NULL == fp)
	{
		perror("fail to fopen");
		return -1;
	}

	while (1)
	{
		memset(tmp, 0, sizeof(tmp));
		nsize = recvfrom(sockfd, tmp, sizeof(tmp), 0, (struct sockaddr *)&recvaddr, &addrlen);
		if (0 >= nsize)
		{
			break;
		}

		if (!strcmp(tmp, "__QUIT__"))
		{
			break;
		}
		fwrite(tmp, 1, nsize, fp);
	}

	fclose(fp);
	close(sockfd);
}


int main(int argc, const char *argv[])
{
	int ret = 0;
	ret = UdpRecvFile(RECV_UDP_IP, RECV_UDP_PORT);
	if (0 == ret)
	{
		printf("recv success!\n");
	}
	return 0;
}
