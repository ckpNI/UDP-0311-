#include "head.h"

int UdpSendFile(const char *pIp,int port,const char *filename)
{
	int sockfd = 0;
	struct sockaddr_in recvaddr;
	char tmp[1024] = {0};
	size_t ret = 0;
	ssize_t nsize = 0;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == sockfd)
	{
		perror("fail to sockfd");
		return -1;
	}

	recvaddr.sin_family = AF_INET;
	recvaddr.sin_port = htons(port);
	recvaddr.sin_addr.s_addr = inet_addr(pIp);

	nsize = sendto(sockfd, filename, sizeof(filename), 0, (struct sockaddr *)&recvaddr, sizeof(recvaddr));
	if (-1 == nsize)
	{
		perror("fail to sendto");
		return -1;
	}

	FILE *fp = NULL;
	fp = fopen(filename, "r");
	if (NULL == fp)
	{
		perror("fail to fopen");	
		return -1;
	}

	while (1)
	{
		memset(tmp, 0, sizeof(tmp));
		ret = fread(tmp, sizeof(char), sizeof(tmp), fp);
		if (0 == ret)
		{
			break;
		}
		nsize = sendto(sockfd, tmp, ret, 0, (struct sockaddr *)&recvaddr, sizeof(recvaddr));
		if (-1 == nsize)
		{
			perror("fail to sendto");
			return -1;
		}
	}

	sendto(sockfd, "__QUIT__", 8, 0, (struct sockaddr *)&recvaddr, sizeof(recvaddr));
	fclose(fp);
	close(sockfd);
	return 0;
}
int main(int argc, const char *argv[])
{
	int ret = 0;
	if (argc != 2)
	{
		perror("Usage:./a.out filename");
		return -1;
	}
	ret = UdpSendFile(SEND_UDP_IP, SEND_UDP_PORT, argv[1]);
	if (-1 == ret)
	{
		perror("发送失败！");
		return -1;
	}
	else 
	{
		printf("发送成功\n");
	}
	return 0;
}
