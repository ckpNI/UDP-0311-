#ifndef __HEAD_H__
#define __HEAD_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define SEND_UDP_IP		"169.254.7.94"
#define SEND_UDP_PORT	50000

#define RECV_UDP_IP		"169.254.7.94"
#define RECV_UDP_PORT	50000

#define MSG_TITLE	100
#define MSG_CONTENT	200
#define MSG_QUIT	300

typedef struct msg1
{
	int msgtype;
	char msgcotent[512];
	int curlen;
}msg_t;

#endif
