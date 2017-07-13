#ifndef SERVER_H
#define SERVER_H

#include<arpa/inet.h>
#include<ctype.h>
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>

#ifdef DEBUG
#include"test_server.h"
#endif

typedef int SOCKET;

struct client {
  unsigned short uid;
  SOCKET socket;
  struct sockaddr_in cli_addr;
  pthread_t thread;
  char name[10];
};

struct clients {
	struct node* first;
	int size;
};

struct node {
	struct client* client;
	struct node* next;
};

struct clients* serv_listen (int port);
struct client* serv_accept ();
void serv_send (char *msg, struct client c);
void serv_get_msg (char *recv_msg, int size, struct client c);
void serv_disconnect(struct client *c);
void serv_stop();

#endif
