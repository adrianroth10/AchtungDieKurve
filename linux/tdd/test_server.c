#include"extras.h"
#include"server.h"

#ifndef DEF_ACCEPTS
#define DEF_ACCEPTS (1)
#endif

static int accepts = 1, recvs, id;

void test_serv_listen(int port)
{
}

struct client test_serv_accept()
{
	if(accepts == DEF_ACCEPTS + 1)
	{
		while(1);
	}
	struct client c;
	char n[10];
	snprintf(n, 10, "%d", accepts);
	strcpy(c.name, n);
	accepts++;
	return c;
}
void test_serv_send(char* msg, struct client c)
{
	printf("%s\n", msg);
}

void test_serv_get_msg(char *recv_msg, int size, struct client c)
{
	if(!recvs)
	{
		recvs++;
		id = c.uid;
	}
	if(c.uid != id)
	{
		while(1);
	}
	if (fgets(recv_msg, size, stdin) == NULL)
	{
		recv_msg[0] = 0;
		return;
	}
	recv_msg[strlen(recv_msg)-1] = '\0';
}

void test_serv_disconnect(struct client c)
{
}

void test_serv_stop()
{
}

void test_delay(int ms)
{
	delay(ms);
}

void test_srand(unsigned int seed)
{
	srand(1);
}
