#include <stdlib.h>
#include "string.h"

struct dlink_node
{
	struct dlink_node* next;
	struct dlink_node* prev;
};

struct dlink_list
{
	struct dlink_node* head;
};

struct dlink_node* dlink_node_create(void)
{
	struct  dlink_node* node = (struct dlink_node*)malloc(sizeof(struct dlink_node));
	node->next = NULL;
	node->prev = NULL;
}


int dlink_node_enqueue(struct dlink_list* list, struct dlink_node* node)
{
	struct dlink_node* head = list->head;

	if (NULL == head)
	{
		list->head = node;
	}else
	{
		node->next = head;
		head->prev = node;
		list->head = node;
	}
	return 1;
}

int dlink_node_dequeue(struct dlink_list* list, struct dlink_node* node)
{
	struct dlink_node* p = list->head;

	while(p)
	{
		if (p == node)
		{
			break;
		}
		p = p->next;
	}
	if (NULL == p)
	{
		return 0;
	}

	if (NULL == p->prev) // Head node
	{
		list->head = p->next;
		free(p);
	}else if (NULL == p->next) // Tail node
	{
		p->prev->next = NULL;
		free(p);
	}else // Other
	{
		p->prev->next = p->next;
		p->next->prev = p->prev;
		free(p);
	}

	return 1;
}