#include <SDL.h>
#include "math.h"
#include "sprite.h"
#include "bullet_new.h"
#include "collide.h"
#include "player.h"
#include "ttf.h"
#include "sound.h"
#include "fsm.h"
#include "animate.h"
#include "boss.h"

extern int score;
extern unsigned char buffer[];
extern SDL_Texture* font;
extern SDL_Color color;
extern int g_game_state;

int collide1(struct Sprite* sprite_a, struct Sprite* sprite_b)
{
	// Params check
	if (NULL == sprite_a) return -1;
	if (NULL == sprite_b) return -1;

	int xa = sprite_a->xc;
	int xb = sprite_b->xc;
	int ya = sprite_a->yc;
	int yb = sprite_b->yc;

	// ¾ØÐÎÅö×²¼ì²âËã·¨
	if ((MATH_Abs(xa - xb) < sprite_a->w / 2 + sprite_b->w / 2)
		&& (MATH_Abs(ya - yb) < sprite_a->h / 2 + sprite_b->h / 2))
	{
		return 1;
	}
	return 0;
}

int collide2(struct bullet* bullet, struct Sprite* sprite)
{
	// Params check
	if (NULL == bullet) return -1;
	if (NULL == sprite) return -1;

	int xa = bullet->xc;
	int xb = sprite->xc;
	int ya = bullet->yc;
	int yb = sprite->yc;

	// ¾ØÐÎÅö×²¼ì²âËã·¨
	if ((MATH_Abs(xa - xb) < bullet->w / 2 + sprite->w / 2)
		&& (MATH_Abs(ya - yb) < bullet->h / 2 + sprite->h / 2))
	{
		return 1;
	}
	return 0;
}

int collide3(struct bullet* bullet, BOSS* sprite)
{
	// Params check
	if (NULL == bullet) return -1;
	if (NULL == sprite) return -1;

	int xa = bullet->xc;
	int xb = sprite->xc;
	int ya = bullet->yc;
	int yb = sprite->yc;

	// ¾ØÐÎÅö×²¼ì²âËã·¨
	if ((MATH_Abs(xa - xb) < bullet->w / 2 + sprite->w / 2)
		&& (MATH_Abs(ya - yb) < bullet->h / 2 + sprite->h / 2))
	{
		return 1;
	}
	return 0;
}

int collide4(struct Sprite* player, BOSS* sprite)
{
	// Params check
	if (NULL == player) return -1;
	if (NULL == sprite) return -1;

	int xa = player->xc;
	int xb = sprite->xc;
	int ya = player->yc;
	int yb = sprite->yc;

	// ¾ØÐÎÅö×²¼ì²âËã·¨
	if ((MATH_Abs(xa - xb) < player->w / 2 + sprite->w / 2)
		&& (MATH_Abs(ya - yb) < player->h / 2 + sprite->h / 2))
	{
		return 1;
	}
	return 0;
}

Uint32 player_cb(Uint32 interval, void* param)
{
	SDL_Event event;
	SDL_UserEvent userevent;

	/* In this example, our callback pushes a function
	into the queue, and causes our callback to be called again at the
	same interval: */

	userevent.type = SDL_USEREVENT;
	userevent.code = 3000;
	userevent.data1 = 0;
	userevent.data2 = param;

	event.type = SDL_USEREVENT;
	event.user = userevent;

	SDL_PushEvent(&event);
	return(interval);
}

void do_collision_check(SDL_Renderer* renderer) 
{
	// player_a with ship_a or ship_b
	struct Sprite* p = spritemgr_get()->list_ship_a->next;
	struct Sprite* player_a = player_get();
	while (p)
	{
		if (collide1(player_a, p) == 1)
		{
			p->is_dead = 1;
			player_a->is_dead = 1;
			//g_game_state = GAME_OVER;
			animatemgr_boom_create(player_a->xc, player_a->yc);
			animatemgr_boom_create(p->xc, p->yc);
			SDL_AddTimer(3000, player_cb, 0);
		}
		p = p->next;
	}
	p = spritemgr_get()->list_ship_b->next;
	player_a = player_get();
	while (p)
	{
		if (collide1(player_a, p) == 1)
		{
			p->is_dead = 1;
			player_a->is_dead = 1;
			//g_game_state = GAME_OVER;
			animatemgr_boom_create(player_a->xc, player_a->yc);
			animatemgr_boom_create(p->xc, p->yc);
			SDL_AddTimer(3000, player_cb, 0);
		}
		p = p->next;
	}
	struct bullet* b = bulletmgr_get()->list_enemy->next;
	player_a = player_get();
	while (b)
	{
		if (collide2(b, player_a) == 1)
		{
			b->is_dead = 1;
			player_a->is_dead = 1;
			//g_game_state = GAME_OVER;
			animatemgr_boom_create(player_a->xc, player_a->yc);
			SDL_AddTimer(3000, player_cb, 0);
		}
		b = b->next;
	}

	// player bullet with ship_a
	b = bulletmgr_get()->list_player->next;
	while (b)
	{
		struct Sprite* enemy = spritemgr_get()->list_ship_a->next;
		while (enemy)
		{
			if (collide2(b, enemy) == 1)
			{
				b->is_dead = 1;
				enemy->is_dead = 1;
				score += 100;
				SDL_snprintf(buffer, 64, "SCORE:%d", score);
				font = RenderText(buffer, "media/font/PressStart2P.ttf",
					color, 18, renderer);
				Sound_PlaySFX("boom", 0);

				//animatemgr_bomb_create(enemy->xc, enemy->yc);
				animatemgr_boom_create(enemy->xc, enemy->yc);
			}
			enemy = enemy->next;
		}
		b = b->next;
	}

	// player bullet with ship_b
	b = bulletmgr_get()->list_player->next;
	while (b)
	{
		struct Sprite* ship_b = spritemgr_get()->list_ship_b->next;
		while (ship_b)
		{
			if (collide2(b, ship_b) == 1)
			{
				b->is_dead = 1;
				ship_b->is_dead = 1;
				score += 100;
				SDL_snprintf(buffer, 64, "SCORE:%d", score);
				font = RenderText(buffer, "media/font/PressStart2P.ttf",
					color, 18, renderer);
				Sound_PlaySFX("boom", 0);

				//animatemgr_bomb_create(ship_b->xc, ship_b->yc);
				animatemgr_boom_create(ship_b->xc, ship_b->yc);
			}
			ship_b = ship_b->next;
		}
		b = b->next;
	}
	//player bullet with boss_1
	b = bulletmgr_get()->list_player->next;
	boss_plane.text_curr = boss_plane.texture;
	while (b)
	{
		if (!boss_plane.is_dead && collide3(b, &boss_plane) == 1)
		{
			boss_plane.text_curr = boss_plane.texture_hit;
			b->is_dead = 1;
			boss_plane.hp--;
			if(boss_plane.hp <= 0)
			{
				boss_plane.state = BOSS_STATE_EXPLOSION;
				boss_plane.is_dead = 1;
				score += 10000;
			}

			SDL_snprintf(buffer, 64, "SCORE:%d", score);
			font = RenderText(buffer, "media/font/PressStart2P.ttf",
				color, 18, renderer);
			//Sound_PlaySFX("boom", 0);
		}
		b = b->next;
	}

	if(collide4(player_a, &boss_plane) == 1)
	{
		//g_game_state = GAME_OVER;
		animatemgr_boom_create(player_a->xc, player_a->yc);
		player_a->is_dead = 1;
		SDL_AddTimer(3000, player_cb, 0);
	}
	return;
}