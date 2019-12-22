#pragma once

#define BOSS_STATE_INIT			0
#define BOSS_STATE_NORMAL		1
#define BOSS_STATE_EXPLOSION	2
#define BOSS_STATE_DEAD			3

typedef struct {
	SDL_Renderer* renderer;
	int xc;
	int yc;
	int x0;
	int y0;
	int w;
	int h;
	int is_dead;
	int hp;
	int state;
	SDL_Texture* text_curr;
	SDL_Texture* texture;
	SDL_Texture* texture_hit;
	SDL_Texture* texture_explosion;
	SDL_TimerID id_timer;
	SDL_TimerID id_timer_bullet;
}BOSS;

extern BOSS boss_plane;
void boss_init(SDL_Renderer* renderer, int x, int y);
void boss_quit();
void boss_draw();
void boss_update();
