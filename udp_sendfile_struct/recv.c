#include "head.h"
int UdpRecvFile(const char *pIp, int port)
{
	int sockfd = 0;
	struct sockaddr_in recvaddr;
	ssize_t nsize = 0;
	int  ret = 0;
	msg_t tmpmsg;
	socklen_t addrlen = 0;
	FILE *fp = NULL;

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

	while (1)
	{
		memset(&tmpmsg, 0, sizeof(tmpmsg));
		recvfrom(sockfd, &tmpmsg, sizeof(tmpmsg), 0, (struct sockaddr *)&recvaddr, &addrlen);
		printf("%d\n", tmpmsg.msgtype);
		if (tmpmsg.msgtype == MSG_TITLE)
		{
			printf("=======%s\n", tmpmsg.msgcotent);
			fp = fopen(tmpmsg.msgcotent, "w");
			if (NULL == fp)
			{
				perror("fail to fopen");
				return -1;
			}
		}
		else if (tmpmsg.msgtype == MSG_CONTENT)
		{

			printf("=======%s\n", tmpmsg.msgcotent);
			fwrite(tmpmsg.msgcotent, 1, tmpmsg.curlen, fp);
		}
		else if (tmpmsg.msgtype == MSG_QUIT)
		{
			fclose(fp);
			break;
		}
	}
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
