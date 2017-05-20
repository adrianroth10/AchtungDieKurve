#ifndef CLIENT_H
#define CLIENT_H

#include<arpa/inet.h>
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h> 

#ifdef DEBUG
#include"test_client.h"
#endif

typedef int SOCKET;

struct server{
        SOCKET socket;
        struct sockaddr_in serv_addr;
};

void init(int port, char *ip_adress);
void cli_get_msg(char *recv_msg, int length);
void cli_send(char *message);
void cli_close();

#endif
