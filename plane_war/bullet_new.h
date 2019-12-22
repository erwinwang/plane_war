#pragma once
#define BULLET_PLAYER		0
#define BULLET_ENEMY		1
#define BULLET_TORPEDO		2

typedef struct bullet
{
	struct bullet *next;
	int x0;
	int y0;
	int w;
	int h;
	int xc;
	int yc;
	SDL_Texture* texture;
	int is_dead;
	int type;

	// TORPEDO
	int life;
	int xv;
	int yv;
	int xa;
	int ya;
}BULLET_NODE, *BULLET_LIST;

struct BulletManager
{
	BULLET_LIST list_player;
	BULLET_LIST list_enemy;
	BULLET_LIST list_boss;
	SDL_Texture* texture_bullet_player;
	SDL_Texture* texture_bullet_enemy;
	SDL_Texture* texture_bullet_boss;
	SDL_Texture* texture_torpedo;
	SDL_Renderer* renderer;
};

void bulletmgr_init(SDL_Renderer* renderer);
void bulletmgr_quit();
void bulletmgr_update(void);
void bulletmgr_create_player_bullet(int x, int y);
void bulletmgr_draw(void);
void bulletmgr_create_ship_bullet(int x, int y);
struct BulletManager* bulletmgr_get(void);


