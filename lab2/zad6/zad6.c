#include<linux/module.h>
#include<linux/slab.h>

static struct list_node
{
	int data;
	struct list_node *next;
} *list_pointer;

static void push(struct list_node ** list_pointer, int number)
{
	struct list_node *new_node = (struct list_node *)kmalloc(sizeof(struct list_node),GFP_KERNEL);
	new_node->data = number;

	if(*list_pointer){
		new_node->next = (*list_pointer)->next;
		(*list_pointer)->next = new_node;
	}
	else
		new_node->next = new_node;
	(*list_pointer) = new_node;
}

static void print_list(struct list_node *list_pointer)
{
	struct list_node *tmp = list_pointer;
	if(tmp){
		do{
			pr_notice("cykliczna %d", tmp->data);
			tmp = tmp->next;
		}while(tmp != list_pointer);
	}
}

static void remove(struct list_node **list_pointer)
{
	if(*list_pointer){
		struct list_node *start = *list_pointer;
		do{
			struct list_node *next = (*list_pointer)->next;
			kfree(*list_pointer);
			*list_pointer = next;
		}while(*list_pointer != start);
		*list_pointer = NULL;
	}
}

static int __init clist_init(void){
	int i;
	for(i=10; i < 110; i +=10){
		push(&list_pointer, i);
	}
	return 0;
}

static void __exit clist_exit(void){
	print_list(list_pointer);
	remove(&list_pointer);
}


module_init(clist_init);
module_exit(clist_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Krzysztof Adamczyk");
MODULE_DESCRIPTION("zad6 cycle list");
MODULE_VERSION("1.0");
