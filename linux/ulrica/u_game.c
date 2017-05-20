#include"u_game.h"
#include"game_identifiers.h"
#include"game.h"

static struct players *p;
static struct graphics_player *p2;
int playing;
static int l, u = -1; 
static SDL_Thread* events_t;

#define send_to_all_game game_msg
void game_msg(char *input);
int string_to_int(char* in);
void cheating(int p);

void serv_send(char* msg, struct client c)
{}
void cli_send(char* msg)
{}
void cli_get_msg(char* msg)
{}
void init()
{}
void cli_close()
{}

static int thread_check_events2(void *data)
{
	int event, i;
	while(playing && (event = get_event()) != -1)
	{
		if(event == 0)
			continue;
		else if(event == UP_ULRICA && u != -1)
		{
			cheating(u);
		}
		for(i = 0; i < l; i++)
		{
			if(event == p[i].left)
			{
				change_dir(UP_1L, i);
			}
			else if(event == p[i].right)
			{
				change_dir(UP_1R, i);
			}
		}
	}
	playing = 0;
	close_window();
	return 0;
}

void print_controlls()
{
	int i;
	char left[] = {0, 'Q', 'N', 'Z', '7'};
	char right[] = {0, 'W', 'M', 'X', '8'};
	printf("\n%s:\nVänster: vänsterpil\nHöger: högerpil\n\n", p2[0].name);
	for(i = 1; i < l; i++)
	{
		printf("%s:\nVänster: %c\nHöger: %c\n\n", p2[i].name, left[i], right[i]);
	}
}

int main(int argc, char* argv[])
{
	char input[10], *ret;
	int i;
	struct clients clients;

	printf("Välkommen till AchtungDieUlrica!\n");
	delay(3000);
	do
	{
		printf("Hur många ska spela?\n");
		if((ret = fgets(input, 10, stdin)) == NULL)
		{
			return 1;
		}
		input[strlen(input)-1] = '\0';
		l = string_to_int(input);
	}while (l <= 0);
	
	if (l > 5)
	{
		printf("Oj va många ni var, ni blir 5 personer då\n");
		l = 5;
		delay(1000);
	}
	
	p = calloc(l, sizeof(struct players));
	p2 = calloc(l, sizeof(struct graphics_player));
	clients.size = l;
	for(i = 0; i < l; i++)
	{
		printf("Namn på spelare %d\n", i+1);
		if((ret = fgets(p2[i].name, 10, stdin)) == NULL)
		{
			return 1;
		}
		p2[i].name[strlen(p2[i].name)-1] = '\0';
		if(strcmp(p2[i].name, "Ulrica") == 0 || strcmp(p2[i].name, "ulrica") == 0)
		{
			printf("Vad kul att du ska vara med och spela %s!\n", p2[i].name);
			u = i;
			delay(1000);
		}
		p[i].left = 2*i + 3;
		p[i].right = 2*i + 4;
		strcpy(clients.client[i].name, p2[i].name);
	}
	print_controlls();
	delay(3000);
	init_sdl((struct graphics_player*)p2, l);
	playing = 1;
	events_t = SDL_CreateThread(thread_check_events2, NULL, NULL);
	play(0, 20, 0, &clients);
	SDL_WaitThread(events_t, NULL);
	free(p);
	return 0;
}
