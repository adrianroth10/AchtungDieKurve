#include"server.h"

static SOCKET* listen_socket;
static struct sockaddr_in *listen_server;
static struct clients *clients;

void error(char *msg)
{
	fprintf(stderr, "%s\n", msg);
	exit(1);
}

void init(int port)
{
	SOCKET temp;
	if((temp = socket(AF_INET , SOCK_STREAM , 0 )) == -1){
		error("socket error");
	}
	listen_socket = malloc(sizeof(SOCKET));
	*listen_socket = temp;
	printf("Socket created.\n");

	listen_server->sin_family = AF_INET;
	(listen_server->sin_addr).s_addr = INADDR_ANY;
	listen_server->sin_port = htons( port );

	if( bind(*listen_socket ,(struct sockaddr *)listen_server , sizeof(*listen_server)) < 0){
		error("bind error");
	}
	printf("Bind done\n");

}

struct clients* serv_listen(int port)
{
	listen_server = calloc(1, sizeof(struct sockaddr_in));

	init(port);

	listen(*listen_socket , 3);
	printf("Socket is listening\n");

	clients = malloc(sizeof(struct clients));
	clients->first = NULL;
	clients->size = 0;
	atexit(serv_stop);
	return clients;
}

void serv_send(char *message, struct client c)
{
	if(write(c.socket, message, strlen(message))<0){
		error("error send"); 
	}
	//printf("data send\n");
}

void serv_get_msg(char *recv_msg, int size, struct client c)
{
	int length;
	if((length = recv(c.socket, recv_msg, size, 0)) < 0){
		error("recive error");
	}
	recv_msg[length] = '\0';
	//printf("message recieved\n");
}

struct client* serv_accept()
{
	int c;
	struct client *client;
	client = malloc(sizeof(struct client));
	c = sizeof(struct sockaddr_in);
	client->socket = accept(*listen_socket, (struct sockaddr *)&client->cli_addr, (socklen_t*)&c);
	if (client->socket < 0)
	{
		error("error accept");
	}
	printf("accept\n");
	return client;
}

void serv_disconnect(struct client *c)
{
	close(c->socket);
	free(c);
}

void serv_stop()
{
	struct node *node;
	struct node *last;
	node = clients->first;
	while(node != NULL)
	{
		serv_send("exit", *(node->client));
		serv_disconnect(node->client);
		last = node;
		node = node->next;
		free(last);
	}
	free(clients);
	close(*listen_socket);
	free(listen_socket);
	free(listen_server);
}
