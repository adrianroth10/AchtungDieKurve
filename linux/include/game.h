#ifndef GAME_H
#define GAME_H

#include<string.h>
#include"extras.h"
#include"server.h"
#include"geometry.h"
#include"extern_server.h"
#include"events.h"

#define WIDTH (640)
#define HEIGHT (480)
#define MENU_WIDTH (80)
#define W (WIDTH-MENU_WIDTH)
#define Y(y) ((y)*W)

struct player{
	int points;
	char name[10];
	int x;
	int y;
	int dir;
	short playing;
};

void play(int mode, int ps, int s, struct clients *c);
void go();
void change_dir(int id, int event);

#endif
