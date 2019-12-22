#pragma once
extern struct Sprite* player;

void player_init(SDL_Renderer* renderer);
void player_quit();
void player_process_input();
void player_draw_image();
struct Sprite* player_get();
