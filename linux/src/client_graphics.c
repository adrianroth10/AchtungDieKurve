// All functions needed for the graphics
#include"client_graphics.h"

int init_window, init_system;
static SDL_Color *colors;
static SDL_Surface *surface;
static SDL_Window *window;
static SDL_Renderer *renderer;
static TTF_Font *font;
static struct graphics_player *players;
static SDL_mutex *lock;

void init_colors()
{
	colors = malloc(10*sizeof(SDL_Color));
	colors[0] = (SDL_Color){.r = 255,
				.g = 255,
				.b = 255,
				.a = 255};
	colors[1] = (SDL_Color){.r = 255,
				.g = 0,
				.b = 0,
				.a = 255};
	colors[2] = (SDL_Color){.r = 0,
				.g = 255,
				.b = 0,
				.a = 255};
	colors[3] = (SDL_Color){.r = 0,
				.g = 0,
				.b = 255,
				.a = 255};
	colors[4] = (SDL_Color){.r = 0,
				.g = 255,
				.b = 255,
				.a = 255};
	colors[5] = (SDL_Color){.r = 255,
				.g = 0,
				.b = 255,
				.a = 255};
	colors[6] = (SDL_Color){.r = 255,
				.g = 255,
				.b = 0,
				.a = 255};
	colors[7] = (SDL_Color){.r = 255,
				.g = 128,
				.b = 0,
				.a = 255};
	colors[8] = (SDL_Color){.r = 128,
				.g = 128,
				.b = 128,
				.a = 255};
	colors[9] = (SDL_Color){.r = 255,
				.g = 51,
				.b = 153,
				.a = 255};
}

void init_sdl(struct graphics_player *p, int l)
{
	if (init_window) {
		return;
	}
	SDL_Surface *text;

	if (!init_system) {
		SDL_Init(SDL_INIT_VIDEO);
		TTF_Init();
		atexit(SDL_Quit);
		atexit(TTF_Quit);
	}
	if ((lock = SDL_CreateMutex()) == 0) {
		fprintf(stderr, "\n mutex init failed\n");
		exit(1);
	}
	window = SDL_CreateWindow("Achtung",
				  SDL_WINDOWPOS_UNDEFINED,
				  SDL_WINDOWPOS_UNDEFINED,
				  WIDTH,
				  HEIGHT,
				  0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	surface = SDL_GetWindowSurface(window);
	if (!window || !renderer || !surface) {
		fprintf(stderr, "Error loading window, renderer or surface: %s\n",
				SDL_GetError());
		exit(1);
	}
	SDL_RenderClear(renderer);
	init_colors();
	font = TTF_OpenFont("OpenSans-Regular.ttf", 12);
	players = p;

	init_window = 1;
	int y;
	for (y = 0; y<HEIGHT; y++) {
		color_pixel(0, WIDTH-MENU_WIDTH, y);
	}
	SDL_Rect r;
	char out[14];

	for (y = 0; y < l; y++) {
		r = (SDL_Rect) {WIDTH - MENU_WIDTH + 10,
				10 + y*20,
				70,
				20};
		players[y].r = r;
		snprintf(out, 14, "%s: %d", p[y].name, 0);
		text = TTF_RenderText_Solid(font, out, colors[y]);
		SDL_BlitSurface(text, NULL, surface, &r);
		SDL_FreeSurface(text);
	}

	update_window();
	init_system = 1;
}

void close_window()
{
	if (!init_window) {
		return;
	}
	SDL_LockMutex(lock);
	init_window = 0;
	free(colors);
	free(players);
	SDL_DestroyRenderer(renderer);
	SDL_FreeSurface(surface);
	SDL_DestroyWindow(window);
	TTF_CloseFont(font);
	SDL_UnlockMutex(lock);
	SDL_DestroyMutex(lock);
}

void clear_window()
{
	if (!init_window) {
		return;
	}
	SDL_LockMutex(lock);
	SDL_Rect r;
	r = (SDL_Rect) {0,0,WIDTH-MENU_WIDTH,HEIGHT};
	SDL_FillRect(surface, &r, SDL_MapRGB(surface->format, 0,0,0));
	SDL_UnlockMutex(lock);
}

void color_pixel(int color, int x, int y)
{
	if (!init_window) {
		return;
	}
	SDL_LockMutex(lock);
	Uint32 *pixels;
	SDL_Color c;

	c = colors[color];
	if (x<0 || y<0 || x>WIDTH-1 || y>HEIGHT-1) {
		SDL_UnlockMutex(lock);
		return;
	}
	pixels = (Uint32*) surface->pixels;
	pixels[x+y*WIDTH] = SDL_MapRGB(surface->format, c.r, c.g, c.b);
	SDL_UnlockMutex(lock);
}

void change_points(int color, int points)
{
	if (!init_window) {
		return;
	}
	SDL_LockMutex(lock);
	char out[14];
	SDL_Surface *text;

	snprintf(out, 14, "%s: %d", players[color].name, points);
	text = TTF_RenderText_Solid(font, out, colors[color]);
	if (text == NULL) {
		return;
	}
	SDL_FillRect(surface,
		     &(players[color].r),
		     SDL_MapRGB(surface->format, 0, 0, 0));
	SDL_BlitSurface(text, NULL, surface, &(players[color].r));
	SDL_FreeSurface(text);
	SDL_UnlockMutex(lock);
}

void update_window()
{
	if (!init_window) {
		return;
	}
	SDL_LockMutex(lock);
	SDL_Texture *texture;
	if ((texture = SDL_CreateTextureFromSurface(renderer, surface))
			== NULL) {
		fprintf(stderr, "error allocating texture");
		exit(1);
	}
	SDL_RenderClear(renderer);
	if (SDL_RenderCopy(renderer, texture, NULL, NULL) < 0) {
		fprintf(stderr, "error rendercopy");
		exit(1);
	}
	SDL_RenderPresent(renderer);
	SDL_DestroyTexture(texture);
	SDL_UnlockMutex(lock);
}

int get_event()
{
	if (!init_system) {
		return NONE;
	}
	SDL_Event event;
	if (!SDL_PollEvent(&event)) {
		return NONE;
	}
	if (event.type == SDL_QUIT) {
		return QUIT;
	} else if (event.type == SDL_KEYDOWN) {
		switch (event.key.keysym.sym) {
			case SDLK_LEFT:
				return DOWN_LEFT;
				break;
			case SDLK_RIGHT:
				return DOWN_RIGHT;
				break;
		}
	} else if (event.type == SDL_KEYUP) {
		switch (event.key.keysym.sym) {
			case SDLK_LEFT:
				return UP_1L;
				break;
			case SDLK_RIGHT:
				return UP_1R;
				break;
			case SDLK_q:
				return UP_2L;
				break;
			case SDLK_w:
				return UP_2R;
				break;
			case SDLK_n:
				return UP_3L;
				break;
			case SDLK_m:
				return UP_3R;
				break;
			case SDLK_z:
				return UP_4L;
				break;
			case SDLK_x:
				return UP_4R;
				break;
			case SDLK_7:
				return UP_5L;
				break;
			case SDLK_8:
				return UP_5R;
				break;
			case SDLK_u:
				return UP_ULRICA;
				break;
		}
	}
	return NONE;
}
