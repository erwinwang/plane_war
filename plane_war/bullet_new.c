#include <SDL.h>
#include <SDL_image.h>
#include "bullet_new.h"

struct BulletManager *bullet_manager = NULL;

static BULLET_LIST LIST_CreateList()
{
	BULLET_LIST list = (BULLET_LIST)malloc(sizeof(BULLET_NODE));
	if (list)
	{
		memset(list, -1, sizeof(struct bullet));
		list->next = NULL;
	}
	return list;
}

static int LIST_DeleteList(BULLET_LIST list)
{
	if (list)
	{
		free(list);
		return 1;
	}
	return 0;
}

static int LIST_IsEmpty(BULLET_LIST list) 
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

static int LIST_InsertFirst(BULLET_LIST list, BULLET_NODE* node)
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
		BULLET_NODE* p = list->next;
		list->next = node;
		node->next = p;
		return 1;
	}

	return 0;
}

void bulletmgr_init(SDL_Renderer* renderer)
{
	bullet_manager = (struct BulletManager *)SDL_malloc(sizeof(struct BulletManager));
	SDL_memset(bullet_manager, 0, sizeof(struct BulletManager));
	if (bullet_manager)
	{
		bullet_manager->renderer = renderer;
		bullet_manager->texture_bullet_player =
			IMG_LoadTexture(renderer, "media/pic/bullet.png");
		bullet_manager->texture_torpedo =
			IMG_LoadTexture(renderer, "media/pic/torpedo.png");
		bullet_manager->texture_bullet_enemy =
			bullet_manager->texture_bullet_player;
			
		
		bullet_manager->list_player = LIST_CreateList();
		bullet_manager->list_enemy = LIST_CreateList();
	}
}

void bulletmgr_quit()
{
	if (bullet_manager)
	{
		if (bullet_manager->list_player)
		{
			BULLET_NODE* p = bullet_manager->list_player->next;
			while (p)
			{
				BULLET_NODE* q = p->next;
				free(p);
				p = q;
			}
			LIST_DeleteList(bullet_manager->list_player);
			bullet_manager->list_player = NULL;
		}

		if (bullet_manager->texture_bullet_enemy)
		{
			SDL_DestroyTexture(bullet_manager->texture_bullet_enemy);
			bullet_manager->texture_bullet_enemy = NULL;
		}
		if (bullet_manager->texture_bullet_player)
		{
			SDL_DestroyTexture(bullet_manager->texture_bullet_player);
			bullet_manager->texture_bullet_player = NULL;
		}
		if (bullet_manager->texture_torpedo)
		{
			SDL_DestroyTexture(bullet_manager->texture_torpedo);
			bullet_manager->texture_torpedo = NULL;
		}
		SDL_free(bullet_manager);
		bullet_manager = NULL;
	}
}

void bulletmgr_create_ship_bullet(int x, int y)
{
	BULLET_NODE* node = (BULLET_NODE*)malloc(sizeof(struct bullet));
	memset(node, 0, sizeof(struct bullet));
	node->next = NULL;
	node->xc = x;
	node->yc = y;
	int w, h;
	SDL_QueryTexture(bullet_manager->texture_bullet_enemy, NULL, NULL,
		&w, &h);
	node->w = w;
	node->h = h;
	node->x0 = x - w / 2;
	node->y0 = y - h / 2;
	node->texture = bullet_manager->texture_bullet_enemy;

	// 增加到链表
	LIST_InsertFirst(bullet_manager->list_enemy, node);
}

void bulletmgr_create_boss_bullet(int x, int y)
{
	for (int i = 0; i < 8; i++)
	{
		BULLET_NODE* node = (BULLET_NODE*)malloc(sizeof(struct bullet));
		memset(node, 0, sizeof(struct bullet));
		node->next = NULL;
		node->xc = x;
		node->yc = y;
		int w, h;
		SDL_QueryTexture(bullet_manager->texture_bullet_boss, NULL, NULL,
			&w, &h);
		node->w = w;
		node->h = h;
		node->x0 = x - w / 2;
		node->y0 = y - h / 2;
		node->texture = bullet_manager->texture_bullet_boss;

		// 增加到链表
		LIST_InsertFirst(bullet_manager->list_boss, node);
	}
	
}

void bulletmgr_create_player_bullet(int x, int y)
{
	BULLET_NODE* node = (BULLET_NODE*)malloc(sizeof(struct bullet));
	memset(node, 0, sizeof(struct bullet));
	node->next = NULL;
	node->x0 = x;
	node->y0 = y;
	int w, h;
	SDL_QueryTexture(bullet_manager->texture_bullet_player, NULL, NULL,
		&w, &h);
	node->w = w;
	node->h = h;
	node->xc = x + w / 2;
	node->yc = y + h / 2;
	node->texture = bullet_manager->texture_bullet_player;

	// 增加到链表
	LIST_InsertFirst(bullet_manager->list_player, node);
}

void bulletmgr_delete_player_bullet(BULLET_NODE* node)
{
	
	// 增加到链表
	LIST_InsertFirst(bullet_manager->list_player, node);
}

static void update_list(BULLET_LIST list)
{
	BULLET_NODE* q = list;
	BULLET_NODE* p = list->next;
	if (LIST_IsEmpty(list) == 0)
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
}

void bulletmgr_update()
{
	BULLET_LIST list = bullet_manager->list_player;

	if (LIST_IsEmpty(list) == 0)
	{
		// update list
		update_list(list);

		// update node status
		BULLET_NODE* p = list->next;
		while (p)
		{
			// bounds check
			if (p->x0 + p->w < 0 || p->y0 + p->h < 0
				|| p->x0 > 640 || p->y0 > 640)
			{
				p->is_dead = 1;
				p = p->next;
				continue;
			}

			// coor
			p->y0 -= 5;
			p->yc = p->y0 + p->h / 2;
			p = p->next;
		}
	}

	BULLET_LIST list_bullet_enemy = bullet_manager->list_enemy;

	if (LIST_IsEmpty(list_bullet_enemy) == 0)
	{
		// update list
		update_list(list_bullet_enemy);

		// update node status
		BULLET_NODE* p = list_bullet_enemy->next;
		while (p)
		{
			// bounds check
			if (p->x0 + p->w < 0 || p->y0 + p->h < 0
				|| p->x0 > 640 || p->y0 > 640)
			{
				p->is_dead = 1;
				p = p->next;
				continue;
			}

			// coor
			p->y0 += 7;
			p->yc = p->y0 + p->h / 2;
			p = p->next;
		}
	}
}

void bulletmgr_draw()
{
	BULLET_LIST list = bullet_manager->list_player;

	if (LIST_IsEmpty(list) == 0)
	{
		BULLET_NODE* p, *q;
		p = q = list->next;
		while (p)
		{
			SDL_Renderer* r = bullet_manager->renderer;
			SDL_Rect dstrect;
			dstrect.x = p->x0;
			dstrect.y = p->y0;
			dstrect.w = p->w;
			dstrect.h = p->h;
			SDL_RenderCopy(r, p->texture, NULL, &dstrect);
			p = p->next;
		}
	}

	list = bullet_manager->list_enemy;

	if (LIST_IsEmpty(list) == 0)
	{
		BULLET_NODE* p, * q;
		p = q = list->next;
		while (p)
		{
			int w = 0, h = 0;
			SDL_Renderer* r = bullet_manager->renderer;
			SDL_Rect dstrect;
			dstrect.x = p->x0;
			dstrect.y = p->y0 +p->h;
			dstrect.w = p->w;
			dstrect.h = p->h;
			SDL_RenderCopy(r, p->texture, NULL, &dstrect);
			p = p->next;
		}
	}
}

struct BulletManager* bulletmgr_get() 
{
	return bullet_manager;
}



