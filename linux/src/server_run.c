#include"server.h"
#include"game_identifiers.h"
#include"game.h"
#include"extern_server.h"

int playing;
static struct clients *clients;
static int mode = 1, win_points = 20, speed;
static pthread_t handle_play;

void send_to_all(char *send_msg, int uid)
{
	struct node *node;
	node = clients->first;
	while(node != 0)
	{
		if(node->client->uid != uid)
			serv_send(send_msg, *(node->client));
		node = node->next;
	}
}

void* thread_play(void *data)
{
	playing = 1;
	play(mode, win_points, speed, clients);
	playing = 0;
	return NULL;
}	

int game_check(char* recv_msg, int id)
{
	recv_msg[0] = '.';
	if(strncmp(".-1", recv_msg, 3) == 0)
	{
		playing = 0;
		return 3;
	}
	else if(isdigit(recv_msg[1]))
	{
		change_dir(id, recv_msg[1] - '0');
		return 3;
	}
	return 2;
}

int check(char *recv_msg, int id)
{
	char *token;
	if(playing && recv_msg[0] == GAME_MSG)
	{
		return game_check(recv_msg, id);
	}
	if(recv_msg[0] != GAME_SETTINGS)
	{
		return 0;
	}
	if(strcmp(recv_msg, GAME_START) == 0 && !playing)
	{
		pthread_create(&handle_play, NULL, thread_play, NULL);
		return 3;
	}

	recv_msg[0] = '.';
	token = strtok(recv_msg, "=");
	if(token == NULL)
		return 2;
	else if(strcmp(token, ".mode") == 0)
	{
		token = strtok(NULL, "=");
		if(token == NULL)
			return 2;
		if(isdigit(token[0]))
		{
			mode = token[0] - '0';
			return 1;
		}
	}
	else if(strcmp(token, ".points") == 0)
	{
		int points;
		token = strtok(NULL, " ");
		if (token == NULL)
			return 2;
		if((points = atoi(token)) != 0)
		{
			win_points = points;
			return 1;
		}
	}
	return 2;
}

void* thread_cli(void *data){
	struct client *client;
	client = (struct client*)data;
	printf("id: %hu, size: %d\n", client->uid, clients->size);
	int check_ret = 0;

	serv_get_msg(client->name, 10, *client);
	char recv_msg[256];
	char send_msg[256];
	while(1){ 
		serv_get_msg(recv_msg, 256, *client);
		if(strcmp(recv_msg, "exit") == 0)
			break;
		check_ret = check(recv_msg, client->uid);
		if(check_ret == 1)
		{
			serv_send("Server: changed", *client);
			continue;
		}
		else if (check_ret == 2)
		{
			serv_send("Server: not changed", *client);
			continue;
		}
		else if (check_ret == 3)
			continue;
		snprintf(send_msg, 256, "%s: %s ", client->name, recv_msg);
		send_to_all(send_msg, client->uid);
	}
	playing = 0;
	struct node *node, *last;
	node = clients->first;
	last = NULL;
	while(node != NULL)
	{
		if(node->client->uid == client->uid)
			break;
		last = node;
		node = node->next;
	}
	serv_disconnect(client);
	if(node == NULL)
	{
		fprintf(stderr, "error, client not found");
		exit(1);
	}
	if(last == NULL)
	{
		clients->first = node->next;
	}
	else
	{
		last->next = node->next;
	}
	free(node);
	clients->size--;
	printf("disconnect\n");
	return NULL;
}

void run()
{
	struct client *client;
	struct node *node, *last;
	int id = 1;

	while(1)
	{
		client = serv_accept();
		if(clients->size == 9)
		{
			serv_send("Server: Too many clients, please try again later", *client);
			serv_send("exit", *client);
			serv_disconnect(client);
			continue;
		}
		client->uid = id;
		node = malloc(sizeof(struct node));
		node->next = NULL;
		node->client = client;
		if(clients->size == 0)
		{
			clients->first = node;
		}
		else
		{
			last->next = node;
		}
		last = node;
		id++;
		clients->size++;
		pthread_create(&(client->thread), NULL, thread_cli, client);
	}
}

void* thread_input(void* data)
{
	char input[100];
	while(1)
	{
		if(fgets(input, 100, stdin) == NULL)
		{
			fprintf(stderr, "error fgets");
			exit(1);
		}
		if(strcmp(input, "exit\n") == 0)
			exit(3);
	}
	return NULL;
}

int main(int argc, char *argv[]){
	pthread_t *input;
	input = malloc(sizeof(pthread_t));
	clients = serv_listen(8888);
	pthread_create(input, NULL, thread_input, NULL);
	run();

	getchar();
	return 0;
}
