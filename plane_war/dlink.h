struct dlink_node
{
	struct dlink_node* next;
	struct dlink_node* prev;
};

struct dlink_list
{
	struct dlink_node* head;
};

struct dlink_node* dlink_node_create(void);
int dlink_node_enqueue(struct dlink_list* list, struct dlink_node* node);
int dlink_node_dequeue(struct dlink_list* list, struct dlink_node* node);