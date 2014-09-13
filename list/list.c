#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>

#define MAX_LIST 10

struct our_data {
	int i;
	struct list_head list;
};

static void list_test(void)
{
	int i;
	struct our_data *node,*next;

	LIST_HEAD(lhead);
	for (i=0; i < MAX_LIST; i++) {
		struct our_data *data;
		
		data = kmalloc(sizeof(*data),GFP_KERNEL);
		if (!data)
			goto clean;

		data->i = i;
		list_add(&data->list, &lhead);
	}

	list_for_each_entry(node, &lhead, list)
		printk("list entry : %d\n",node->i);
clean:
	list_for_each_entry_safe(node,next,&lhead,list)
		kfree(node);
}

static int __init linit(void)
{
	printk("call function is %s\n",__FUNCTION__);

	list_test();

	return 0;
}

static void __exit lexit(void)
{
	printk("call function is %s\n",__FUNCTION__);
}


MODULE_LICENSE("GPL");
MODULE_AUTHOR("mwg");
MODULE_DESCRIPTION("this is a test for list");

module_init(linit)
module_exit(lexit)
