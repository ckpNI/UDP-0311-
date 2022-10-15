#include "head.h"

int UdpSendFile(const char *pIp,int port,const char *filename)
{
	int sockfd = 0;
	struct sockaddr_in recvaddr;
	msg_t msg;
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


	memset(&msg, 0, sizeof(msg));
	msg.msgtype = MSG_TITLE;
	strcpy(msg.msgcotent, filename);
	printf("%s\n", msg.msgcotent);
	nsize = sendto(sockfd, &msg, sizeof(msg), 0, (struct sockaddr *)&recvaddr, sizeof(recvaddr));
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

	memset(&msg, 0, sizeof(msg));
	while (1)
	{
		memset(msg.msgcotent, 0, sizeof(msg.msgcotent));
		msg.msgtype = MSG_CONTENT;
		msg.curlen = fread(msg.msgcotent, sizeof(char), sizeof(msg.msgcotent), fp);
		if (0 == msg.curlen)
		{
			break;
		}
		printf("surlen = %d,  %s\n", msg.curlen, msg.msgcotent);
		nsize = sendto(sockfd, &msg, sizeof(msg), 0, (struct sockaddr *)&recvaddr, sizeof(recvaddr));
		if (-1 == nsize)
		{
			perror("fail to sendto");
			return -1;
		}
	}
	fclose(fp);
	
	memset(&msg, 0, sizeof(msg));
	msg.msgtype = MSG_QUIT;
	sendto(sockfd, &msg, sizeof(msg), 0, (struct sockaddr *)&recvaddr, sizeof(recvaddr));
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
