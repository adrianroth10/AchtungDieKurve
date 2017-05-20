#include<stdio.h>
#include<string.h>
#include"../client/client_graphics.h"
#include"../server/extras.h"

int main(int argc, char* argv[])
{
	struct graphics_player *p;
	p = malloc(sizeof(struct graphics_player));
	strcpy(p->name, "TEST");
	init_sdl(p,1);
	delay(3000);
	int i;
	for (i = 0; i<100; i++)
	{
		color_pixel(100,i,1);
	}
	change_points(0,4);
	update_window();
	printf("press leftarrow\n");
	while(get_event() != UP_LEFT);
	clear_window();
	update_window();
	printf("press rightarrow\n");
	while(get_event() != DOWN_RIGHT);
	quit_sdl();
	return 0;
}
