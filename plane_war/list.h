#pragma once
struct element
{
	unsigned int key;
	void* ptr;
	struct element* next;
};

struct list
{
	int len;
	struct element* head;
	struct element* tail;
};

struct list* list_create(void);
int list_delete(struct list* l);
int list_enqueue(struct list* l, struct element* e);
int list_dequeue(struct list* l, struct element* e);
void* list_search(struct list* l, char *str);
unsigned int ELFhash(char *str);
