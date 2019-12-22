#include <SDL.h>
#include <SDL_image.h>
#include "boss.h"

BOSS boss_plane = {0};

static int bStart = 0;

Uint32 cb(Uint32 interval, void* param)
{
	bStart = 1;
	return 0;
}

Uint32 cb_boss_bullet(Uint32 interval, void* param)
{
	BOSS* plane = (BOSS*)param;
	
	return interval;
}

void boss_init(SDL_Renderer* renderer, int x, int y)
{
	boss_plane.state = BOSS_STATE_INIT;
	boss_plane.renderer = renderer;
	boss_plane.texture = IMG_LoadTexture(renderer, "media/pic/boss.png");
	boss_plane.texture_hit = IMG_LoadTexture(renderer, "media/pic/boss-11.png");
	boss_plane.texture_explosion = IMG_LoadTexture(renderer, "media/pic/explosion.png");;
	boss_plane.text_curr = boss_plane.texture;
	int w, h;
	SDL_QueryTexture(boss_plane.texture, NULL, NULL, &w, &h);
	boss_plane.w = w;
	boss_plane.h = h;
	boss_plane.xc = x;
	boss_plane.yc = h/2;
	boss_plane.x0 = x - w / 2;
	boss_plane.y0 = 0;
	boss_plane.is_dead = 0;
	boss_plane.hp = 49;
	boss_plane.id_timer = SDL_AddTimer(8000, cb, NULL);
	boss_plane.state = BOSS_STATE_NORMAL;
}

void boss_quit()
{
	SDL_RemoveTimer(boss_plane.id_timer);
	boss_plane.id_timer = -1;

}

void boss_update()
{
	static int bFlag = 1;
	if (bStart)
	{
		if (bFlag)
		{
			boss_plane.id_timer_bullet = SDL_AddTimer(6000, cb_boss_bullet, &boss_plane);
			bFlag = 0;
		}
		
		// bullet & movement
		/*boss_plane.x0 += 1;
		boss_plane.y0 += 1;
		boss_plane.xc = boss_plane.x0 + boss_plane.w / 2;
		boss_plane.yc = boss_plane.y0 + boss_plane.h / 2;*/
	}
}

void boss_draw()
{
	if (bStart)
	{
		static int key_frame_index = 0;
		if (boss_plane.state == BOSS_STATE_NORMAL)
		{
			SDL_Rect dstrect = { 0 };
			dstrect.x = boss_plane.x0;
			dstrect.y = boss_plane.y0;
			dstrect.w = boss_plane.w;
			dstrect.h = boss_plane.h;
			SDL_RenderCopy(boss_plane.renderer, boss_plane.text_curr,
				NULL, &dstrect);
		}

		if (boss_plane.state == BOSS_STATE_EXPLOSION)
		{
			if (key_frame_index > 6)
			{
				boss_plane.state = BOSS_STATE_DEAD;
				goto label_end;
			}
			boss_plane.text_curr = boss_plane.texture_explosion;
			SDL_Rect srcrect = { 0 };
			srcrect.x = (key_frame_index % 4) * 64;
			srcrect.y = (key_frame_index / 4) * 64;
			srcrect.w = 64;
			srcrect.h = 64;
			SDL_Rect dstrect = { 0 };
			dstrect.x = boss_plane.x0;
			dstrect.y = boss_plane.y0;
			dstrect.w = boss_plane.w;
			dstrect.h = boss_plane.h;
			SDL_RenderCopy(boss_plane.renderer, boss_plane.text_curr,
				&srcrect, &dstrect);
			key_frame_index++;
		}
	}
	
label_end:
	return;
}
