#include <SDL.h>
#include <SDL_surface.h>
#include <SDL_image.h>
#include "res.h"

struct ResManager res_manager = { NULL };

void resmgr_init(SDL_Renderer* renderer) 
{
//	// Map texture
//	SDL_Surface *map_surface = IMG_Load(renderer, "media/pic/1945.png");
//	
//	Uint32 rmask, gmask, bmask, amask;
//#if SDL_BYTEORDER == SDL_BIG_ENDIAN
//	rmask = 0xff000000;
//	gmask = 0x00ff0000;
//	bmask = 0x0000ff00;
//	amask = 0x000000ff;
//#else
//	rmask = 0x000000ff;
//	gmask = 0x0000ff00;
//	bmask = 0x00ff0000;
//	amask = 0xff000000;
//#endif
//	int height = 32;
//	int width = 32 * 6;
//	surface = SDL_CreateRGBSurface(0, width, height, 32,
//		rmask, gmask, bmask, amask);
//	SDL_FreeSurface(surface);
	
	//width = 268;
	//height = 400;
	SDL_Surface* surface = IMG_Load("media/pic/ship_b.png");
	/*surface = SDL_CreateRGBSurface(0, width, height, 32,
		rmask, gmask, bmask, amask);*/
	Uint32 color_key = SDL_MapRGB(surface->format, 0, 67, 171);
	SDL_SetColorKey(surface, SDL_TRUE, color_key);
	res_manager.texture_ship_b = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("media/pic/boom.png");
	color_key = SDL_MapRGB(surface->format, 0, 67, 171);
	SDL_SetColorKey(surface, SDL_TRUE, color_key);
	res_manager.texture_boom = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
}

void resmgr_quit()
{
}

struct ResManager* resmgr_get()
{
	return &res_manager;
}