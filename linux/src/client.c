// Helping functions for the client
#include"client.h"

static struct server *server;
static char name[10];

void error(const char *msg)
{
	fprintf(stderr, "%s\n", msg);
	exit(1);
}

void init_socket()
{
	SOCKET temp;
	if ((temp = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		error("socket error");
	}
	server->socket = temp;
	printf("Socket created.\n");
}

void connect_to_server(int port, char *ip_adress)
{
	server->serv_addr.sin_addr.s_addr = inet_addr(ip_adress);
	server->serv_addr.sin_family = AF_INET;
	server->serv_addr.sin_port = htons(port);

	printf("connecting\n");
	while (connect(server->socket,
			(struct sockaddr *)&(server->serv_addr),
			sizeof(server->serv_addr)) < 0);
	printf("Connected\n");
}


void init(int port, char *ip_adress)
{
	server = malloc(sizeof(struct server));

	init_socket();
	connect_to_server(port, ip_adress);
	printf("Name: ");
	if (fgets(name, 10, stdin) == NULL) {
		error("error EOF");
	}
	name[strlen(name)-1] = 0;
	cli_send(name);
	atexit(cli_close);
}

void cli_get_msg(char *recv_msg, int length)
{
	int recv_size;
	if ((recv_size = recv(server->socket,
					recv_msg, length, 0)) < 0) {
		error("recive failed: ");
	}
	recv_msg[recv_size] = '\0';
	//printf("message recieved\n");
}

void cli_send(char *message)
{
	if (send(server->socket, message, strlen(message), 0) < 0) {
		error("send failed: ");
	}
	//printf("send succeed\n");
}

void cli_close()
{
	cli_send("exit");
	close(server->socket);
	free(server);
}
