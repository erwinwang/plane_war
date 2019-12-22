#include "list.h"
#include <stdlib.h>
#include <string.h>

unsigned int ELFhash(char *str)
{
	unsigned int hash = 0;
	unsigned int x = 0;
	while (*str)
	{
		hash = (hash << 4) + *str;
		if ((x = hash & 0xf0000000) != 0)
		{
			hash ^= (x >> 24);   //影响5-8位，杂糅一次 
			hash &= ~x;   //清空高四位 
		}
		str++;
	}
	return (hash & 0x7fffffff);
}

int list_enqueue(struct list* l, struct element* e)
{
	if (l->head == NULL)
	{
		struct element* p = (struct element*)malloc(sizeof(struct element));
		//strncpy(p->key, e->key, 32);
		p->key = e->key;
		p->ptr = e->ptr;
		p->next = NULL;
		l->head = l->tail = p;
		l->len++;
		return 1;
	}

	struct element* p = (struct element*)malloc(sizeof(struct element));
	//strncpy(p->key, e->id, 32);
	p->key = e->key;
	p->ptr = e->ptr;
	p->next = NULL;
	struct element* tmp = l->head;
	while (tmp->next)
	{
		tmp = tmp->next;
	}
	tmp->next = p;
	l->len++;
	return 1;
}

int list_dequeue(struct list* l, struct element* e)
{
	return 1;
}

void* list_search(struct list* l, char *str)
{
	unsigned int key = ELFhash(str);
	struct element* p = l->head;
	while (p)
	{
		if (key == p->key)
		{
			return p->ptr;
		}
		p = p->next;
	}
	return 0;
}

struct list* list_create(void)
{
	struct list* l = (struct list*)malloc(sizeof(struct list));
	memset(l, 0, sizeof(struct list));
	l->len = l->head = l->tail = 0;
	return l;
}

int list_delete(struct list* l)
{
	struct element* p = l->head;
	while (p)
	{
		free(p);
		p = p->next;
	}
	free(l);
	return 1;
}