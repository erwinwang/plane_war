#include <SDL.h>
#include <SDL_syswm.h>
#include <SDL_image.h>
#include <SDL_events.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <time.h>
#include <Windows.h>
#include "bullet_new.h"
#include "math.h"
#include "sprite.h"
#include "sound.h"
#include "level.h"
#include "animate.h"
#include "ttf.h"
#include "plane_war.h"
#include "collide.h"
#include "player.h"
#include "fsm.h"
#include "res.h"
#include "boss.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

extern const short int level_map0[30][10];
extern const short int level_sand_map0[30][10];

#define FPS 20
const int DELAY_TIME = 1000.0f / FPS;
int score = 0;
unsigned char buffer[64] = {0};

SDL_Color color = { 248, 248, 248, 0 };
SDL_Texture* font = NULL;
int g_game_state = GAME_SELECT;
SDL_Texture* menu = NULL;
SDL_Texture* game_over_texture = NULL;

Uint32 my_callbackfunc3000(Uint32 interval, void *param)
{
	SDL_Event event;
	SDL_UserEvent userevent;

	/* In this example, our callback pushes an SDL_USEREVENT event
	into the queue, and causes our callback to be called again at the
	same interval: */

	userevent.type = SDL_USEREVENT;
	userevent.code = 0;
	userevent.data1 = NULL;
	userevent.data2 = NULL;

	event.type = SDL_USEREVENT;
	event.user = userevent;

	SDL_PushEvent(&event);
	return(interval);
}

Uint32 my_callbackfunc5000(Uint32 interval, void* param)
{
	SDL_Event event;
	SDL_UserEvent userevent;

	/* In this example, our callback pushes an SDL_USEREVENT event
	into the queue, and causes our callback to be called again at the
	same interval: */

	userevent.type = SDL_USEREVENT;
	userevent.code = 1;
	userevent.data1 = NULL;
	userevent.data2 = NULL;

	event.type = SDL_USEREVENT;
	event.user = userevent;

	SDL_PushEvent(&event);
	return(interval);
}

int main()
{
	//_CrtSetBreakAlloc(83); //sound::struct element
	//_CrtSetBreakAlloc(103); //sprite_new::struct sprite
	//_CrtSetBreakAlloc(99); //sprite_new::struct SpriteManager
	//_CrtSetBreakAlloc(98);
	//_CrtSetBreakAlloc(97);//bullet_new::struct BulletManager
	//_CrtSetBreakAlloc(95);
	SDL_Rect camera = { 0, 64 * 30 - 480, 640, 480 };
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO
		| SDL_INIT_TIMER) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return 1;
	}

	TTF_Init();

	int flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;
	int initted = IMG_Init(flags);
	if ((initted&flags) != flags) {
		SDL_Log("IMG_Init: Failed to init required jpg and png support!\n");
		SDL_Log("IMG_Init: %s\n", IMG_GetError());
	}
	Sound_Init();
	// Load sound res.
	Sound_Load("media/sound/fire.ogg", "fire", SOUND_SFX);
	Sound_Load("media/sound/boom.ogg", "boom", SOUND_SFX);
	Sound_Load("media/sound/torpedo.ogg", "torpedo", SOUND_SFX);

	// Create window
	SDL_Window* window = SDL_CreateWindow("Plane War 1945", SDL_WINDOWPOS_UNDEFINED, 
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HIGHT, SDL_WINDOW_SHOWN);
	// Set window icon
	SDL_Surface* icon = IMG_Load("media/pic/icon.ico");
	SDL_SetWindowIcon(window, icon);
	SDL_FreeSurface(icon);
	//SDL_SetWindowTitle(window, "Plane War");
	SDL_SysWMinfo info = {0};
	SDL_GetWindowWMInfo(window, &info);
	if (info.info.win.window)
	{
		SetWindowText(info.info.win.window, 
			TEXT("Plane War"));
	}
	
	// Create renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	//
	resmgr_init(renderer);
	boss_init(renderer, 320, 0);
	
	// Menu and background
	SDL_Texture* menu_texture = IMG_LoadTexture(renderer, "media/pic/menu.png");

	// Bullet
	bulletmgr_init(renderer);

	// Sprite
	spritemgr_init(renderer);
	spritemgr_create_ship_a(320, 67);

	// Animate
	animatemgr_init(renderer);

	// Prepare ttf font
	font = RenderText("SCORE:0", "media/font/PressStart2P.ttf",
		color, 18, renderer);

	// Prepare background
	int rmask, gmask, bmask, amask;
	rmask = 0xFF000000; gmask = 0x00FF0000; bmask = 0x0000FF00; amask = 0x000000FF; // RGBA8888ģʽ
	SDL_Surface* background = SDL_CreateRGBSurface(0, 10 * 64, 30 * 64, 32, rmask, gmask, bmask, amask);

	// Level
	SDL_Rect rect_srcs[30 * 10] = { 0 };
	SDL_Rect srcrect = { 0, 0, 64, 64 };
	SDL_Surface* tile = IMG_Load("media/pic/tile.png");
	for (int row = 0; row < 30; row++)
	{
		for (int col = 0; col < 10; col++)
		{
			if (level_map0[row][col] != 0)
			//if (level_sand_map0[row][col] != 0)
			{
				int tile_index = level_map0[row][col];
				srcrect.x = (tile_index % 5) * 64;
				srcrect.y = (tile_index / 5) * 64;
				rect_srcs[row * col].h = rect_srcs[row * col].w = 64;
				rect_srcs[row * col].x = col * 64;
				rect_srcs[row * col].y = row * 64;
				SDL_BlitSurface(tile, &srcrect, background, &rect_srcs[row * col]);
			}
		}
	}
	SDL_Texture *texture_background = SDL_CreateTextureFromSurface(renderer, background);
	game_over_texture = IMG_LoadTexture(renderer, "media/pic/game_over.png");
	// Time
	srand(time(NULL));
	SDL_TimerID timer_id_sprite = SDL_AddTimer(3000, my_callbackfunc3000, NULL);
	SDL_TimerID timer_id_5 = SDL_AddTimer(3000, my_callbackfunc5000, NULL);

	SDL_Texture *text_stone = IMG_LoadTexture(renderer, "media/pic/stone.png");

	player_init(renderer);

	
	// Handle events
	int quit = 0;
	int i = 0;
	while (!quit)
	{
		unsigned int frame_start = SDL_GetTicks();
		
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			//If user closes the window
			if (e.type == SDL_QUIT)
			{
				quit = 1;
			}else if (e.type == SDL_USEREVENT)
			{
				if (g_game_state == GAME_RUNNING)
				{
					switch (e.user.code)
					{
					case 0:
						spritemgr_create_ship_a(rand() % (640 - 23), 0);
						//animatemgr_create(rand() % 640, 0, ANIMATE_SUBMARINE_RISE);
						break;
					case 1:
					{
						int x = rand() % (640 - 23);
						SPRITE_NODE* sprite = spritemgr_create_ship_b(x, 0);
						bulletmgr_create_ship_bullet(sprite->xc, 32);
					}
						break;
					case 10000:
						
						break;
					case 3000:
						g_game_state = GAME_OVER;
						//SDL_RemoveTimer();
						break;
					default:
						break;
					}
					
				}
			}
			else if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_j:
					if (g_game_state == GAME_RUNNING)
					{
						if (player_get())
						{
							if (player->is_dead == 0)
							{
								Sound_PlaySFX("fire", 0);
								bulletmgr_create_player_bullet(player->xc - 6, player->yc - 32);
							}
						}
					}
					break;
				case SDLK_k:
					//bulletmgr_create(plane.xc - 6, plane.yc - 32, BULLET_TORPEDO);
					break;
				case SDLK_ESCAPE:
					quit = 1;
					break;
				default:
					break;
				}
			}
			//else if (e.type == SDL_MOUSEBUTTONDOWN)
			//{
			//	if (e.button.button == SDL_BUTTON_LEFT)	
			//	{
			//		if (g_game_state == GAME_SELECT)
			//		{
			//			g_game_state = GAME_RUNNING;
			//		}
			//	}
			//}
		}

		// Rendering
		SDL_Rect dstrect;
		int w, h;
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
		SDL_RenderClear(renderer);

		if (g_game_state == GAME_RUNNING)
		{
			player_process_input();

			do_collision_check(renderer);

			// Update data
			bulletmgr_update();
			spritemgr_update();
			animatemgr_update();
			boss_update();

			// Camera
			if (camera.y < 0)
			{
				camera.y = 1920 - 480;
			}
			else
			{
				camera.y--;
			}
			dstrect.x = 0;
			dstrect.y = 0;
			dstrect.w = 640;
			dstrect.h = 480;
			SDL_RenderCopy(renderer, texture_background, &camera, &dstrect);

			//
			bulletmgr_draw();
			spritemgr_draw();
			animatemgr_draw();
			player_draw_image();
			boss_draw();


			//
			SDL_QueryTexture(font, NULL, NULL, &w, &h);
			dstrect.x = 10;
			dstrect.y = 10;
			dstrect.w = w;
			dstrect.h = h;
			SDL_RenderCopy(renderer, font, NULL, &dstrect);
			//SDL_DestroyTexture(font);
		}else if (g_game_state == GAME_SELECT)
		{
			font = RenderText("Press Space Key to start game.", "media/font/PressStart2P.ttf",
				color, 18, renderer);
			SDL_RenderCopy(renderer, menu_texture, NULL, NULL);
			int fw, fh;
			SDL_QueryTexture(font, NULL, NULL, &fw, &fh);
			SDL_Rect frect = {50, 400, fw, fh};
			SDL_RenderCopy(renderer, font, NULL, &frect);
			SDL_DestroyTexture(font);

			const Uint8* state = SDL_GetKeyboardState(NULL);
			if (state[SDL_SCANCODE_SPACE]) {
				g_game_state = GAME_RUNNING;
				font = RenderText("SCORE:0", "media/font/PressStart2P.ttf",
					color, 18, renderer);
				// Play background music
				Sound_Play("media/sound/bg.ogg");
			}
		}else if (g_game_state == GAME_OVER)
		{
			if (Mix_PlayingMusic())
			{
				Mix_HaltMusic();
			}
			
			SDL_snprintf(buffer, 64, "HISCORE:%d", score);
			font = RenderText(buffer, "media/font/PressStart2P.ttf",
				color, 18, renderer);
			//SDL_RenderCopy(renderer, game_over_texture, NULL, NULL);
			int fw, fh;
			SDL_QueryTexture(font, NULL, NULL, &fw, &fh);
			dstrect.x = 10;
			dstrect.y = 10;
			dstrect.w = fw;
			dstrect.h = fh;
			SDL_RenderCopy(renderer, font, NULL, &dstrect);
			SDL_DestroyTexture(font);

			SDL_snprintf(buffer, 64, "GAME OVER", score);
			font = RenderText(buffer, "media/font/PressStart2P.ttf",
				color, 60, renderer);
			SDL_QueryTexture(font, NULL, NULL, &fw, &fh);
			dstrect.x = 50;
			dstrect.y = 200;
			dstrect.w = fw;
			dstrect.h = fh;
			SDL_RenderCopy(renderer, font, NULL, &dstrect);
			SDL_DestroyTexture(font);
		}

		SDL_RenderPresent(renderer);
		
		unsigned int frame_time = SDL_GetTicks() - frame_start;
		if (frame_time < DELAY_TIME)
		{
			SDL_Delay((int)(DELAY_TIME - frame_time));
		}
	}

	// Destroy all texture
	SDL_DestroyTexture(texture_background);
	SDL_DestroyTexture(menu_texture);
	SDL_DestroyTexture(font);

	bulletmgr_quit();
	spritemgr_quit();
	animatemgr_quit();
	SDL_RemoveTimer(timer_id_sprite);

	// Destroy renderer and window
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	// Unload the dynamically loaded image libraries
	TTF_Quit();
	IMG_Quit();
	Sound_Quit();
	SDL_Quit();

	//_CrtDumpMemoryLeaks();
	return 0;
}
