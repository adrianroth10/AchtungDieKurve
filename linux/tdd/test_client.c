//#include"client.h"
#include"client_graphics.h"
#include<stdio.h>

void test_init(int port, char *ip_adress)
{
}

void test_cli_get_msg(char *recv_msg, int length)
{
	if (fgets(recv_msg, length, stdin) == NULL)
	{
		*recv_msg = 0;
		return;
	}
	recv_msg[strlen(recv_msg)-1] = '\0';
}

void test_cli_send(char *message)
{
	printf("%s\n", message);
}

void test_cli_close()
{
}




void test_init_sdl(struct graphics_player *p, int length)
{
	int i;
	for(i = 0; i<length; i++)
	{
		printf("%s ", p[i].name);
	}
	printf("\n");
	init_sdl(p, length);
}

void test_close_window()
{
	close_window();
}

void test_clear_window()
{
	printf("1\n");
	clear_window();
}

void test_color_pixel(int color, int x, int y)
{
	printf("3 %d %d %d\n", color, x, y);
	color_pixel(color, x, y);
}

void test_update_window()
{
	update_window();
}

int test_get_event()
{
	return get_event();
}

void test_change_points(int color, int points)
{
	printf("2 %d %d\n", color, points);
	change_points(color, points);
}

char *test_fgets(char *str, int n, FILE *stream)
{
	char *c;
	while(1);
	return c;
}
