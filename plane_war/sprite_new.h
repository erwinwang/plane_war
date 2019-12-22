#pragma once

enum sprite_type{
	SPRITE_PLAYER,
	SPRITE_SHIP_A,
	SPRITE_SHIP_B,
	SPRITE_SHIP_BOSS,
	SPRITE_SUBMARINE
};

typedef struct Sprite
{
	struct Sprite* next;
	int x0;
	int y0;
	int xc;
	int yc;
	int w;
	int h;
	SDL_Texture* texture;
	int is_dead;
}SPRITE_NODE, *SPRITE_LIST;

struct SpriteManager 
{
	SPRITE_LIST list_ship_a;
	SPRITE_LIST list_ship_b;

	SDL_Texture* sprite_player;
	SDL_Texture* sprite_ship_a;
	SDL_Texture* sprite_ship_b;
	SDL_Texture* sprite_ship_boss;
	SDL_Texture* texture_submarine;
	SDL_Renderer* renderer;
};

struct SpriteManager* spritemgr_get();
void spritemgr_init(SDL_Renderer* renderer);
void spritemgr_draw();
void spritemgr_quit();
void spritemgr_update();
void spritemgr_create_ship_a(int x, int y);
SPRITE_NODE* spritemgr_create_ship_b(int x, int y);