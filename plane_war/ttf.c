#include <SDL.h>
#include <SDL_ttf.h>

SDL_Texture* RenderText(char* message, char* fontFile,
	SDL_Color color, int fontSize, SDL_Renderer* renderer)
{
	//Open the font
	TTF_Font *font = NULL;
	font = TTF_OpenFont(fontFile, fontSize);
	if (font == NULL)
		return NULL;

	//Render the message to an SDL_Surface, as that's what TTF_RenderText_X returns
	SDL_Surface *surf = TTF_RenderUTF8_Solid(font, message, color);


	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	//Clean up unneeded stuff
	SDL_FreeSurface(surf);
	TTF_CloseFont(font);

	return texture;
}