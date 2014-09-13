
struct rb_node {
	unsigned long  __rb_parent_color;
	struct rb_node *rb_right;
	struct rb_node *rb_left;
} __attribute__((aligned(sizeof(long))));
    /* The alignment might seem pointless, but allegedly CRIS needs it */

struct rb_root {
	struct rb_node *rb_node;
};

int my_insert(struct rb_root *root, struct mytype *data)
{
	struct rb_node *new = root->rb_node;
	struct rb_node *parent = NULL:
	while(new) {
		struct mytype *this = container_of(new, struct mytype, node);
		int result = strcmp(data->keystring,this->keystring);
		
		parent = new;

		if (result < 0)
			new = new->rb_left;
		else if (result > 0)
			new = new->rb_right;
		else 
			return FALSE;
	}

	/*Add new node and rebanlance tree*/
	rb_list_node(&data->node, parent, &new);
	reb_insert_color(&data->node,root);

	return TRUE;
}
