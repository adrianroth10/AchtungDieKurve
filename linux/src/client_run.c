// Runnning the client for the game
// Using the helping functions from client.c
#include"client_graphics.h"
#include"game_identifiers.h"
#include"client.h"

int playing;
static int l;
static pthread_t sendh, recvh;
SDL_Thread *events;

int string_to_int(char* in)
{
	int nbr, minus, i;
	if (in == NULL || strlen(in) == 0) {
		return -2;
	}
	if (in[0] == '-') {
		minus = -1;
		i = 1;
	} else {
		minus = 1;
		i = 0;
	}
	nbr = 0;
	for (; i < strlen(in); i++) {
		if (!isdigit(in[i])) {
			return -2;
		}
		nbr *= 10;
		nbr += in[i] - '0';
	}
	nbr *= minus;
	return nbr;
}

static int thread_check_events(void *data)
{
	int event;
	char msg[3];
	msg[0] = GAME_MSG;
	msg[2] = '\0';
	while (playing && (event = get_event()) != -1) {
		if (event == 0) {
			continue;
		}
		msg[1] = event + '0';
		cli_send(msg);
	}
	playing = 0;
	char send_msg[4];
	snprintf(send_msg, 4, "%c%d", GAME_MSG, QUIT);
	cli_send(send_msg);
	close_window();
	while (!playing) {
		if (get_event() == -1) {
			exit(3);
		}
	}
	return 0;
}

void go_cli()
{
	char *name;
	int i;
	struct graphics_player *players;
	players = calloc(l, sizeof(struct graphics_player));
	for (i = 0; i < l; i++) {
		name = strtok(NULL, " ");
		if(name == NULL)
			return;
		strcpy(players[i].name, name);
	}
	init_sdl((struct graphics_player*)players, l);
	playing = 1;
	printf("go\n");
	events = SDL_CreateThread(thread_check_events, NULL, players);
}

void points(int length)
{
	char *p, *points;
	int i, ip, ipoints;
	for (i = 0; i < length; i++) {
		p = strtok(NULL, " ");
		points = strtok(NULL, " ");
		ip = string_to_int(p);
		ipoints = string_to_int(points);
		if (ip == -2 || ipoints == -2) {
			return;
		}
		change_points(ip, ipoints);
	}
}

void pixels(int length)
{
	char *col, *x, *y;
	int i, icol, ix, iy;
	for (i = 0; i < length; i++) {
		col = strtok(NULL, " ");
		x = strtok(NULL, " ");
		y = strtok(NULL, " ");
		icol = string_to_int(col);
		ix = string_to_int(x);
		iy = string_to_int(y);
		if (icol == -2 || ix == -2 || iy == -2) {
			return;
		}
		color_pixel(icol, ix, iy);
	}
}

char* chat_msg(char* first)
{
	char print[100], *token;
	int index = snprintf(print, 100, "%s", first);
	token = strtok(NULL, " ");
	while (token != NULL) {
		if (token[0] == GAME_MSG ||
		    (strncmp(token, GAME_START, 2) == 0 &&
		    !playing && isdigit(token[2]))) {
			break;
		}
		index += snprintf(print + index,
				  100 - index,
				  " %s",
				  token);
		token = strtok(NULL, " ");
	}
	printf("%s\n", print);
	return token;
}

void game_msg(char *recv_msg)
{
	char *type;
	int length;
	type = strtok(recv_msg, " ");
	while (type != NULL) {
		if (strcmp(type, "exit") == 0) {
			printf("server closed\n");
			exit(3);
		} else if (strncmp(type, GAME_START, 2) == 0 &&
			   !playing &&
			   isdigit(type[2])) {
			l = type[2] - '0';
			go_cli();
		} else if (type[0] != GAME_MSG) {
			type = chat_msg(type);
			continue;
		} else if (type[1] == '1') {
			clear_window();
		} else if (type[1] == '2' && isdigit(type[2])) {
			length = type[2] - '0';
			points(length);
		} else if (type[1] == '3' && isdigit(type[2])) {
			length = type[2] - '0';
			pixels(length);
		}
		type = strtok(NULL, " ");
	}
	update_window();
}

void* thread_recv(void *data)
{
	printf("receiving\n");
	char *recv_msg;
	recv_msg = calloc(256, sizeof(char));
	while (1) {
		cli_get_msg(recv_msg, 256);
		game_msg(recv_msg);
	}
	free(recv_msg);
	return NULL;
}

void* thread_send(void *data)
{
	printf("sending\n");
	char send_msg[100];
	while (1) {
		if (fgets(send_msg, 100, stdin) == NULL) {
			printf("error fgets");
			exit(1);
		}
		send_msg[strlen(send_msg)-1] = '\0';
		if (strcmp(send_msg, "exit") == 0) {
			exit(3);
		}
		cli_send(send_msg);
	}
	return NULL;
}

int main(int argc, char* argv[])
{
	int port = 8888;
	char *ip_adress = "127.0.0.1";//"83.254.155.234";
	init(port, ip_adress);


	pthread_create(&sendh, NULL, thread_send, NULL);
	pthread_create(&recvh, NULL, thread_recv, NULL);

	pthread_join(sendh, NULL);
	pthread_join(recvh, NULL);
	cli_close();

	return 0;
}
