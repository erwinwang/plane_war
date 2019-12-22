#include <SDL.h>
#include <SDL_image.h>
#include <string.h>
#include "sprite.h"
#include "res.h"

struct SpriteManager* sprite_manager = NULL;

static SPRITE_LIST LIST_CreateList()
{
	SPRITE_LIST list = (SPRITE_LIST)malloc(sizeof(SPRITE_NODE));
	memset(list, -1, sizeof(SPRITE_NODE));
	list->next = NULL;
	return list;
}

static int LIST_IsEmpty(SPRITE_LIST list)
{
	// 
	if (list == NULL)
	{
		return -1;
	}

	//
	if (list->next == NULL)
	{
		return 1;
	}

	return 0;
}

static int LIST_InsertFirst(SPRITE_LIST list, SPRITE_NODE* node)
{
	// 
	if (list == NULL || node == NULL)
	{
		return -1;
	}

	if (LIST_IsEmpty(list) == 1)
	{
		list->next = node;
		node->next = NULL;
		return 1;
	}
	else if (LIST_IsEmpty(list) == 0)
	{
		SPRITE_NODE* p = list->next;
		list->next = node;
		node->next = p;
		return 1;
	}

	return 0;
}

void spritemgr_load(int type, char* filename)
{
	SDL_Surface* surface = IMG_Load(filename);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(sprite_manager->renderer, surface);
	SDL_FreeSurface(surface);

	switch (type)
	{
	case SPRITE_PLAYER:
		sprite_manager->sprite_player = texture;
		break;
	case SPRITE_SHIP_A:
		sprite_manager->sprite_ship_a = texture;
		break;
	case SPRITE_SHIP_B:
		sprite_manager->sprite_ship_b = texture;
		break;
	case SPRITE_SHIP_BOSS:
		sprite_manager->sprite_ship_boss = texture;
		break;
	case SPRITE_SUBMARINE:
		sprite_manager->texture_submarine = texture;
		break;
	default:
		break;
	}

	return;
}

void spritemgr_create_ship_a(int x, int y)
{
	struct Sprite* sprite = malloc(sizeof(struct Sprite));
	memset(sprite, 0, sizeof(struct Sprite));
	sprite->x0 = x;
	sprite->y0 = y;
	sprite->texture = sprite_manager->sprite_ship_a;
		
	int w = 0, h = 0;
	SDL_QueryTexture(sprite->texture, NULL, NULL, &w, &h);
	sprite->xc = sprite->x0 + w / 2;
	sprite->yc = sprite->y0 + h / 2;
	sprite->h = h;
	sprite->w = w;

	LIST_InsertFirst(sprite_manager->list_ship_a, sprite);
}

SPRITE_NODE* spritemgr_create_ship_b(int x, int y)
{
	struct Sprite* sprite = malloc(sizeof(struct Sprite));
	memset(sprite, 0, sizeof(struct Sprite));
	sprite->x0 = x;
	sprite->y0 = y;
	sprite->texture = sprite_manager->sprite_ship_b;

	int w = 0, h = 0;
	SDL_QueryTexture(sprite->texture, NULL, NULL, &w, &h);
	sprite->xc = sprite->x0 + w / 2;
	sprite->yc = sprite->y0 + h / 2;
	sprite->h = h;
	sprite->w = w;

	LIST_InsertFirst(sprite_manager->list_ship_b, sprite);

	return sprite;
}

void spritemgr_delete(struct Sprite* sprite)
{
	if (sprite)
	{
		free(sprite);
		return;
	}
}

void spritemgr_update()
{
	// Update list_ship_a
	struct Sprite* p = sprite_manager->list_ship_a->next;

	SPRITE_NODE* q = sprite_manager->list_ship_a;
	if (LIST_IsEmpty(sprite_manager->list_ship_a) == 0)
	{
		while (p)
		{
			if (p->is_dead == 1)
			{
				q->next = p->next;
				free(p);
				p = q->next;
				continue;
			}
			q = p;
			p = p->next;
		}
	}

	// Update list_ship_b
	p = sprite_manager->list_ship_b->next;
	q = sprite_manager->list_ship_b;
	if (LIST_IsEmpty(sprite_manager->list_ship_b) == 0)
	{
		while (p)
		{
			if (p->is_dead == 1)
			{
				q->next = p->next;
				free(p);
				p = q->next;
				continue;
			}
			q = p;
			p = p->next;
		}
	}

	// Update list_ship_a(x,y)
	p = sprite_manager->list_ship_a->next;
	while (p)
	{
		if (p->x0 + p->w < 0 || p->x0 > 640
			|| p->y0 + p->h < 0 || p->y0 > 480)
		{
			p->is_dead = 1;
			p = p->next;
			continue;
		}
		p->y0 += 5;
		p->yc = p->y0 + p->h / 2;
		p = p->next;
	}

	// Update list_ship_b(x,y)
	p = sprite_manager->list_ship_b->next;
	while (p)
	{
		if (p->x0 + p->w < 0 || p->x0 > 640
			|| p->y0 + p->h < 0 || p->y0 > 480)
		{
			p->is_dead = 1;
			p = p->next;
			continue;
		}
		p->y0 += 5;
		p->yc = p->y0 + p->h / 2;
		p = p->next;
	}

	return;
}

void spritemgr_quit()
{
	if (sprite_manager->sprite_player)
	{
		SDL_DestroyTexture(sprite_manager->sprite_player);
	}

	if (sprite_manager->sprite_ship_a)
	{
		SDL_DestroyTexture(sprite_manager->sprite_ship_a);
	}

	if (sprite_manager->sprite_ship_b)
	{
		SDL_DestroyTexture(sprite_manager->sprite_ship_b);
	}

	if (sprite_manager->sprite_ship_boss)
	{
		SDL_DestroyTexture(sprite_manager->sprite_ship_boss);
	}
	return;
}

void spritemgr_init(SDL_Renderer* renderer)
{
	sprite_manager = (struct SpriteManager*)malloc(sizeof(struct SpriteManager));
	if (sprite_manager)
	{
		memset(sprite_manager, 0, sizeof(struct SpriteManager));
		sprite_manager->renderer = renderer;

		sprite_manager->sprite_ship_a =
			IMG_LoadTexture(renderer, "media/pic/enemy.png");
		sprite_manager->sprite_ship_b = resmgr_get()->texture_ship_b;

		sprite_manager->list_ship_a = LIST_CreateList();
		sprite_manager->list_ship_b = LIST_CreateList();
	}
}

void spritemgr_draw()
{
	// Traverse list
	struct Sprite* p = sprite_manager->list_ship_a->next;
	while (p)
	{
		int w = 0, h = 0;
		SDL_QueryTexture(p->texture, NULL, NULL, &w, &h);
		SDL_Rect dstrect;
		dstrect.x = p->x0;
		dstrect.y = p->y0;
		dstrect.w = w;
		dstrect.h = h;
		SDL_RenderCopy(sprite_manager->renderer, p->texture, NULL, &dstrect);
		p = p->next;
	}
	p = sprite_manager->list_ship_b->next;
	while (p)
	{
		int w = 0, h = 0;
		SDL_QueryTexture(p->texture, NULL, NULL, &w, &h);
		SDL_Rect dstrect;
		dstrect.x = p->x0;
		dstrect.y = p->y0;
		dstrect.w = w;
		dstrect.h = h;
		SDL_RenderCopyEx(sprite_manager->renderer, p->texture, NULL, 
			&dstrect, 0, NULL, SDL_FLIP_VERTICAL);
		p = p->next;
	}
}

struct SpriteManager* spritemgr_get()
{
	return sprite_manager;
}
