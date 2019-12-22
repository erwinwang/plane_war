#pragma once
struct ResManager
{
	SDL_Texture* texture_ship_a;
	SDL_Texture* texture_ship_b;
	SDL_Texture* texture_boom;
};


struct ResManager* resmgr_get();
void resmgr_init(SDL_Renderer* renderer);
