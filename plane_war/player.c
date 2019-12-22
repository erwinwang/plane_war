#include <SDL.h>
#include <SDL_image.h>
#include "sprite.h"

struct Sprite *player = NULL;
SDL_Renderer* rend = NULL;

void player_init(SDL_Renderer* renderer) 
{
	rend = renderer;
	player = (struct Sprite*)malloc(sizeof(struct Sprite));
	if (player)
	{
		player->next = NULL;
		player->x0 = 300;
		player->y0 = 400;
		player->xc = 332;
		player->yc = 432;
		player->texture = IMG_LoadTexture(renderer, "media/pic/player.png");
		int w, h;
		w = h = 0;
		SDL_QueryTexture(player->texture, NULL, NULL, &w, &h);
		player->w = w;
		player->h = h;
		player->is_dead = 0;
	}
}

void player_quit() 
{
	if (player)
	{
		free(player);
		player = NULL;
	}
}

void player_process_input() 
{
	if (player)
	{
		const Uint8* state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_W]) {
			player->yc -= 6;
			player->y0 -= 6;
			if (player->y0 < 0)
			{
				player->y0 = 0;
				player->yc = player->y0 + player->h / 2;
			}
		}
		if (state[SDL_SCANCODE_S]) {
			player->yc += 6;
			player->y0 += 6;
			if (player->y0 + player->h > 480)
			{
				player->y0 = 480 - player->h;
				player->yc = player->y0 + player->h / 2;
			}
		}
		if (state[SDL_SCANCODE_A]) {
			player->xc -= 6;
			player->x0 -= 6;
			if (player->x0 < 0)
			{
				player->x0 = 0;
				player->xc = player->x0 + player->w / 2;
			}
		}
		if (state[SDL_SCANCODE_D]) {
			player->xc += 5;
			player->x0 += 5;
			if (player->x0 + player->w > 640)
			{
				player->x0 = 640 - player->w;
				player->xc = player->x0 + player->w / 2;
			}
		}
	}
	
}

void player_update_status() 
{

}

void player_draw_image() 
{
	SDL_Rect dstrect;
	if (player)
	{
		if (player->is_dead == 0)
		{
			dstrect.x = player->x0;
			dstrect.y = player->y0;
			dstrect.w = player->w;
			dstrect.h = player->h;
			SDL_RenderCopy(rend, player->texture, NULL, &dstrect);
		}
	}
	
}

struct Sprite* player_get() 
{
	if (player)
	{
		if (player->is_dead == 1)
		{
			free(player);
			player = NULL;
		}
	}
	return player;
}