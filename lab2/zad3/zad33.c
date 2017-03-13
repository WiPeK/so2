#include<linux/module.h>
#include<linux/slab.h>

static struct list_node{
	int data;
	struct list_node *previous, *next;
} *list_pointer, *first_node;

static struct list_node *create_list(int number){
	first_node = (struct list_node *)kmalloc(sizeof(struct list_node), GFP_KERNEL);
	if(first_node){
		first_node->data = number;
		first_node->previous = first_node->next = NULL;
	}
	return first_node;
}

static struct list_node *add_at_front(struct list_node *list_pointer, struct list_node *new_node){
	new_node->next = list_pointer;
	list_pointer->previous = new_node;
	return new_node;
}

static struct list_node *find_spot(struct list_node *list_pointer, int number){
	struct list_node *previous = NULL;
	while(list_pointer&&list_pointer->data < number){
		previous = list_pointer;
		list_pointer = list_pointer->next;
	}
	return previous;
}

static void add_in_middle(struct list_node *node, struct list_node *new_node){
	new_node->previous = node;
	new_node->next = node->next;
	node->next->previous = new_node;
	node->next = new_node;
}

static void add_at_back(struct list_node *node, struct list_node *new_node){
	node->next = new_node;
	new_node->previous = node;
}

static struct list_node *add_node(struct list_node *list_pointer, int number){
	if(list_pointer){
		struct list_node *new_node = (struct list_node *)kmalloc(sizeof(struct list_node), GFP_KERNEL); 
		if(new_node){
			new_node->data = number;
			new_node->previous = new_node->next = NULL;
			if(list_pointer->data >= number){
				return add_at_front(list_pointer, new_node);
			}
			else{
				struct list_node *node = find_spot(list_pointer, number);
				if(node->next){
					add_in_middle(node, new_node);
				}
				else
					add_at_back(node, new_node);
			}
		}
	}
	return list_pointer;
}

static void print_list(struct list_node *list_pointer){
	struct list_node *backward_pointer = NULL;
	while(list_pointer){
		backward_pointer = list_pointer;
		pr_notice("list to end %d", list_pointer->data);
		list_pointer = list_pointer->next;
	}
	while(backward_pointer){
		pr_notice("list to start %d", backward_pointer->data);
		backward_pointer = backward_pointer->previous;
	}
}

static void remove_list(struct list_node **list_pointer){
	while(*list_pointer){
		struct list_node *next = (*list_pointer)->next;
		kfree(*list_pointer);
		*list_pointer = next;
	}
}

static int __init tlist_init(void)
{
	int i;
    list_pointer = create_list(1);
    for(i = 10; i < 100; i += 10){
		list_pointer = add_node(list_pointer, i);
	}
	return 0;
}

static void __exit tlist_exit(void)
{
	print_list(list_pointer);
	remove_list(&list_pointer);
}

module_init(tlist_init);
module_exit(tlist_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Krzysztof Adamczyk");
MODULE_DESCRIPTION("zad2 stack");
MODULE_VERSION("1.0");
