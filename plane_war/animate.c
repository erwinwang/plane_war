#include <SDL.h>
#include <SDL_image.h>
#include "list.h"
#include "animate.h"
#include "res.h"

typedef struct animate
{
	struct animate* next;
	int type;
	int is_dead;
}ANIMATE_NODE, * ANIMATE_LIST;

typedef struct BOMB
{
	struct BOMB* next;
	int x, y;
	int during_frames;
	struct PARTICLE particles[8];
	SDL_Texture* texture_bomb;
	int w;
	int h;
	int is_dead;
}BOMB_NODE, *BOMB_LIST;

typedef struct BOOM
{
	struct BOOM* next;
	int x, y;
	int total_key_frames;
	int current_key_frame;
	int key_frame_width;
	int key_frame_height;
	SDL_Texture* texture_boom;
	int w;
	int h;
	int is_dead;
}BOOM_NODE, * BOOM_LIST;

struct ANIMATE_MANAGER
{
	BOMB_LIST list_bomb;
	BOOM_LIST list_boom;
	SDL_Renderer* renderer;

	SDL_Texture* texture_bomb;
	SDL_Texture* texture_boom;
	SDL_Texture* texture_explode;
	SDL_Texture* texture_bomb_torpedo;
	SDL_Texture* texture_submarine_rise;
};

static int LIST_InsertFirst(BOMB_LIST list, BOMB_NODE* node)
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
		BOMB_NODE* p = list->next;
		list->next = node;
		node->next = p;
		return 1;
	}

	return 0;
}

static int LIST_IsEmpty(BOMB_LIST list)
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

//
struct ANIMATE_MANAGER *animate_manager = NULL;

struct BOOM* animatemgr_boom_create(int x, int y)
{
	struct BOOM* boom = (struct BOOM*)SDL_malloc(sizeof(struct BOOM));
	if (boom)
	{
		SDL_memset(boom, 0, sizeof(struct BOOM));

		boom->x = x;
		boom->y = y;
		boom->total_key_frames = 6;
		boom->current_key_frame = -1;
		boom->is_dead = 0;

		boom->texture_boom = animate_manager->texture_boom;
		int w, h;
		SDL_QueryTexture(boom->texture_boom, NULL, NULL, &w, &h);
		boom->w = w;
		boom->h = h;
		boom->key_frame_width = boom->w / boom->total_key_frames - 1;
		boom->key_frame_height = boom->h - 2;
		
		LIST_InsertFirst(animate_manager->list_boom, boom);
	}

	return boom;
}

struct BOMB* animatemgr_bomb_create(int x, int y) 
{
	struct BOMB *bomb = (struct BOMB *)SDL_malloc(sizeof(struct BOMB));
	if (bomb)
	{
		SDL_memset(bomb, 0, sizeof(struct BOMB));

		bomb->x = x;
		bomb->y = y;
		bomb->during_frames = 4 * 20;
		bomb->is_dead = 0;

		for (int i = 0; i < 8; i++)
		{
			bomb->particles[i].x = x;
			bomb->particles[i].y = y;
		}

		bomb->particles[0].speed.xv = -1;
		bomb->particles[0].speed.yv = -1;

		bomb->particles[1].speed.xv = 0;
		bomb->particles[1].speed.yv = -1;

		bomb->particles[2].speed.xv = 1;
		bomb->particles[2].speed.yv = -1;

		bomb->particles[3].speed.xv = 1;
		bomb->particles[3].speed.yv = 0;

		bomb->particles[4].speed.xv = 1;
		bomb->particles[4].speed.yv = 1;

		bomb->particles[5].speed.xv = 0;
		bomb->particles[5].speed.yv = 1;

		bomb->particles[6].speed.xv = -1;
		bomb->particles[6].speed.yv = 1;

		bomb->particles[7].speed.xv = -1;
		bomb->particles[7].speed.yv = 0;

		LIST_InsertFirst(animate_manager->list_bomb, bomb);
	}

	return bomb;
}


//static struct BOMB_TORPEDO* animatemgr_bomb_torpedo_create(int x, int y)
//{
//	struct BOMB_TORPEDO *bomb = (struct BOMB_TORPEDO*)malloc(sizeof(struct BOMB_TORPEDO));
//	memset(bomb, 0, sizeof(struct BOMB_TORPEDO));
//
//	bomb->x = x;
//	bomb->y = y;
//	bomb->past_frames_count = 0;
//	bomb->max_frame = 6;
//	bomb->tile_width = 64;
//	bomb->tile_height = 64;
//	
//	int w, h;
//	SDL_QueryTexture(animate_manager->texture_bomb_torpedo, NULL, NULL, &w, &h);
//	bomb->w = w;
//	bomb->h = h;
//	
//	bomb->one_row_frames = bomb->w / bomb->tile_width;
//	return bomb;
//}
//
//static struct SUBMARINE_RISE* animatemgr_submarine_rise_create(int x, int y)
//{
//	struct SUBMARINE_RISE *submarine = (struct SUBMARINE_RISE*)malloc(sizeof(struct SUBMARINE_RISE));
//	memset(submarine, 0, sizeof(struct SUBMARINE_RISE));
//
//	submarine->x = x;
//	submarine->y = y;
//	submarine->past_frames_count = 0;
//	submarine->max_frame = 5;
//	submarine->tile_width = 32;
//	submarine->tile_height = 96;
//
//	//
//	submarine->current_frame = 5;
//
//	int w, h;
//	SDL_QueryTexture(animate_manager->texture_submarine_rise, NULL, NULL, &w, &h);
//	submarine->w = w;
//	submarine->h = h;
//
//	submarine->one_row_frames = submarine->w / submarine->tile_width;
//	return submarine;
//}
//
//static void animatemgr_submarine_rise_update(struct SUBMARINE_RISE* submarine)
//{
//	submarine->past_frames_count++;
//	submarine->y += 2;
//	if (0 == submarine->past_frames_count % 20)
//	{
//		if (submarine->current_frame > 0)
//		{
//			submarine->current_frame--;
//		}
//	}
//}
//
//static void animatemgr_submarine_rise_draw(struct SUBMARINE_RISE* submarine)
//{
//	SDL_Renderer* renderer = animate_manager->renderer;
//	SDL_Texture* texture = animate_manager->texture_submarine_rise;
//
//	int w = 0, h = 0;
//	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
//	SDL_Rect dstrect, srcrect;
//	dstrect.x = submarine->x;
//	dstrect.y = submarine->y;
//	dstrect.w = submarine->tile_width;
//	dstrect.h = submarine->tile_height;
//	srcrect.x = (submarine->current_frame % submarine->one_row_frames) * submarine->tile_width;
//	srcrect.y = (submarine->current_frame / submarine->one_row_frames) * submarine->tile_height;
//	srcrect.w = submarine->tile_width;
//	srcrect.h = submarine->tile_height;
//	SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
//}
//
//void animatemgr_delete(struct ANIMATE *animate) 
//{
//	if(animate)
//	{
//		free(animate->p);
//		free(animate);
//	}
//}
//
static void animatemgr_bomb_update(struct BOMB* bomb) 
{
	if (bomb->during_frames <= 0)
	{
		bomb->is_dead = 1;
	}
	else
	{
		bomb->during_frames--;
		for (int i = 0; i < 8; i++)
		{
			struct PARTICLE* p = &bomb->particles[i];
			p->x += p->speed.xv;
			p->y += p->speed.yv;
		}
	}
}
//
//static void animatemgr_bomb_torpedo_update(struct BOMB_TORPEDO* bomb)
//{
//	bomb->past_frames_count++;
//	if (0 == bomb->past_frames_count % 5)
//	{
//		bomb->current_frame++;
//	}
//}
//
//static void animatemgr_bomb_torpedo_draw(struct BOMB_TORPEDO* bomb)
//{
//	SDL_Renderer* renderer = animate_manager->renderer;
//	SDL_Texture* texture = animate_manager->texture_bomb_torpedo;
//
//	if (NULL != bomb)
//	{
//		/* code */
//		int w = 0, h = 0;
//		SDL_QueryTexture(texture, NULL, NULL, &w, &h);
//		SDL_Rect dstrect, srcrect;
//		dstrect.x = bomb->x;
//		dstrect.y = bomb->y;
//		dstrect.w = bomb->tile_width;
//		dstrect.h = bomb->tile_height;
//		srcrect.x = (bomb->current_frame % bomb->one_row_frames) * bomb->tile_width;
//		srcrect.y = (bomb->current_frame / bomb->one_row_frames) * bomb->tile_height;
//		srcrect.w = bomb->tile_width;
//		srcrect.h = bomb->tile_height;
//		SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
//	}
//	
//}
//
static void animatemgr_bomb_draw(struct BOMB* bomb)
{
	SDL_Renderer* renderer = animate_manager->renderer;
	SDL_Texture* texture = animate_manager->texture_bomb;

	int w = 0, h = 0;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	for (int i = 0; i < 8; i++)
	{
		struct PARTICLE* particle = &bomb->particles[i];
		SDL_Rect dstrect;
		dstrect.x = particle->x;
		dstrect.y = particle->y;
		dstrect.w = w;
		dstrect.h = h;
		SDL_RenderCopy(renderer, texture, NULL, &dstrect);
	}
}

static void animatemgr_boom_draw(struct BOOM* boom)
{
	SDL_Renderer* renderer = animate_manager->renderer;
	SDL_Texture* texture = animate_manager->texture_boom;

	SDL_Rect dstrect = { 0 }, srcrect = {0};
	dstrect.x = boom->x - 16;
	dstrect.y = boom->y - 16;
	dstrect.w = boom->key_frame_width;
	dstrect.h = boom->key_frame_height;
	srcrect.x = 1 + boom->current_key_frame * (boom->key_frame_width + 1);
	srcrect.y = 1;
	srcrect.w = boom->key_frame_width;
	srcrect.h = boom->key_frame_height;
	SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
}

void animatemgr_draw()
{
	SDL_Renderer* renderer = animate_manager->renderer;
			
	if (animate_manager->list_bomb)
	{
		if (!LIST_IsEmpty(animate_manager->list_bomb))
		{
			BOMB_NODE* p = animate_manager->list_bomb->next;
			while (p)
			{
				animatemgr_bomb_draw(p);
				p = p->next;
			}
		}
	}

	if (animate_manager->list_boom)
	{
		if (!LIST_IsEmpty(animate_manager->list_boom))
		{
			BOOM_NODE* p = animate_manager->list_boom->next;
			while (p)
			{
				animatemgr_boom_draw(p);
				p = p->next;
			}
		}
	}
}

void animatemgr_load(char* filename, int type)
{
	SDL_Texture* texture = IMG_LoadTexture(animate_manager->renderer, filename);

	switch (type)
	{
	case ANIMATE_BOMB:
		animate_manager->texture_bomb = texture;
		break;
	case ANIMATE_EXPLODE:
		animate_manager->texture_explode = texture;
		break;
	case ANIMATE_BOMB_TORPEDO:
		animate_manager->texture_bomb_torpedo = texture;
		break;
	case ANIMATE_SUBMARINE_RISE:
		animate_manager->texture_submarine_rise = texture;
		break;
	default:
		break;
	}
}

static void update_bomb_list(BOMB_LIST list)
{
	BOMB_NODE* q = list;
	BOMB_NODE* p = list->next;
	if (LIST_IsEmpty(list) == 0)
	{
		while (p)
		{
			if (p->is_dead == 1)
			{
				q->next = p->next;
				SDL_free(p);
				p = q->next;
				continue;
			}
			q = p;
			p = p->next;
		}
	}
}

static void boom_node_update()
{
}

static void boom_list_update() 
{
	BOOM_LIST list = animate_manager->list_boom;
	// Update boom list
	BOOM_NODE* q = list;
	BOOM_NODE* p = list->next;
	if (LIST_IsEmpty(list) == 0)
	{
		while (p)
		{
			if (p->is_dead == 1)
			{
				q->next = p->next;
				SDL_free(p);
				p = q->next;
				continue;
			}
			q = p;
			p = p->next;
		}
	}

	// Update boom node
	if (LIST_IsEmpty(list) == 0)
	{
		BOOM_NODE* p = animate_manager->list_boom->next;
		while (p)
		{
			//animatemgr_bomb_update(p);
			if (p->current_key_frame >= p->total_key_frames)
			{
				p->is_dead = 1;
			}
			else
			{
				p->current_key_frame++;
			}
			p = p->next;
		}
	}
}

void animatemgr_update()
{
	BOMB_LIST list = animate_manager->list_bomb;

	update_bomb_list(list);

	// Update its coords
	if (LIST_IsEmpty(list) == 0)
	{
		BOMB_NODE* p = animate_manager->list_bomb->next;
		while (p)
		{
			animatemgr_bomb_update(p);
			p = p->next;
		}
	}

	boom_list_update();
}

//void animatemgr_create(int x, int y, int type) 
//{
//	struct ANIMATE *animate = (struct ANIMATE *)malloc(sizeof(struct ANIMATE));
//	memset(animate, 0, sizeof(struct ANIMATE));
//	
//
//	switch (type)
//	{
//	case ANIMATE_BOMB:
//		animate->next = NULL;
//		animate->type = ANIMATE_BOMB;
//		animate->p = (void*)animatemgr_bomb_create(x, y);
//		break;
//	case ANIMATE_BOMB_TORPEDO:
//		animate->next = NULL;
//		animate->type = ANIMATE_BOMB_TORPEDO;
//		animate->p = (void*)animatemgr_bomb_torpedo_create(x, y);
//		break;
//	case ANIMATE_SUBMARINE_RISE:
//		animate->next = NULL;
//		animate->type = ANIMATE_SUBMARINE_RISE;
//		animate->p = (void*)animatemgr_submarine_rise_create(x, y);
// 		break;
//	default:
//		break;
//	}
//
//	// list add
//	if (animate_manager->head == NULL) // empty list
//	{
//		animate_manager->head = animate;
//	}
//	else
//	{
//		// Traverse list
//		struct ANIMATE* p = animate_manager->head;
//		struct ANIMATE* t = NULL;
//		while (p)
//		{
//			t = p;
//			p = p->next;
//		}
//		t->next = animate;
//	}
//
//}




static int LIST_bomb_destroy(BOMB_LIST list) 
{
	if (list)
	{
		if (!LIST_IsEmpty(list))
		{
			BOMB_NODE* p = list->next;
			while (p)
			{
				BOMB_NODE *q = p->next;
				SDL_free(p);
				p = q;
			}
			SDL_free(list);
			return 1;
		}
		SDL_free(list);
		return 1;
	}
	return -1;
}

static int LIST_boom_destroy(BOOM_LIST list)
{
	if (list)
	{
		if (!LIST_IsEmpty(list))
		{
			BOOM_NODE* p = list->next;
			while (p)
			{
				BOOM_NODE* q = p->next;
				SDL_free(p);
				p = q;
			}
			SDL_free(list);
			return 1;
		}
		SDL_free(list);
		return 1;
	}
	return -1;
}

static BOMB_LIST LIST_bomb_create()
{
	BOMB_LIST list = (BOMB_LIST)SDL_malloc(sizeof(BOMB_LIST));
	if (list)
	{
		SDL_memset(list, 0, sizeof(BOMB_LIST));
		list->next = NULL;
		return list;
	}
	return NULL;
}

static BOMB_LIST LIST_boom_create()
{
	BOOM_LIST list = (BOOM_LIST)SDL_malloc(sizeof(BOOM_LIST));
	if (list)
	{
		SDL_memset(list, 0, sizeof(BOOM_LIST));
		list->next = NULL;
		return list;
	}
	return NULL;
}

void animatemgr_quit() 
{
	if (NULL != animate_manager)
	{
		if (animate_manager->texture_bomb)
		{
			SDL_DestroyTexture(animate_manager->texture_bomb);
			animate_manager->texture_bomb = NULL;
		}
		if (animate_manager->list_bomb)
		{
			LIST_bomb_destroy(animate_manager->list_bomb);
			animate_manager->list_bomb = NULL;
		}
		if (animate_manager->list_boom)
		{
			LIST_boom_destroy(animate_manager->list_boom);
			animate_manager->list_boom = NULL;
		}
		SDL_free(animate_manager);
		animate_manager = NULL;
	}
}

//
void animatemgr_init(SDL_Renderer* renderer)
{
	if (NULL == animate_manager)
	{
		animate_manager = (struct ANIMATE_MANAGER*)SDL_malloc(sizeof(struct ANIMATE_MANAGER));
		if (animate_manager)
		{
			memset(animate_manager, 0, sizeof(struct ANIMATE_MANAGER));
			animate_manager->renderer = renderer;
			animate_manager->texture_bomb = IMG_LoadTexture(renderer, "media/pic/bomb.jpg");
			animate_manager->texture_boom = resmgr_get()->texture_boom;

			animate_manager->list_bomb = LIST_bomb_create();
			animate_manager->list_boom = LIST_boom_create();
		}
	}
}