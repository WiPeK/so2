#include<linux/module.h>
#include<linux/slab.h>

struct fifo_node
{
	int data;
	struct fifo_node *next;
};

static struct fifo_pointers
{
	struct fifo_node *head, *tail;
} fifo;

static void push(struct fifo_pointers *fifo, int data)
{
	struct fifo_node *new_node = (struct fifo_node *)kmalloc(sizeof(struct fifo_node),GFP_KERNEL);
	if(new_node){
			new_node->data = data;
			new_node->next = NULL;
			if(fifo->head == NULL)
				fifo->head = fifo->tail = new_node;
			else{
				fifo->tail->next = new_node;
				fifo->tail = new_node;
			}
	}
}

static void print(struct fifo_pointers fifo)
{
	pr_notice("print queue");
	while(fifo.head){
		pr_notice("fifo %d", fifo.head->data);
		fifo.head = fifo.head->next;
	}
}

static void remove(struct fifo_pointers *fifo)
{
	while(fifo->head){
		struct fifo_node *tmp = fifo->head->next;
		kfree(fifo->head);
		fifo->head = tmp;
		if(tmp == NULL)
			fifo->tail = NULL;
	}
}

static int __init fifo_init(void){
	int i;
	for(i=10; i < 110; i +=10){
		push(&fifo, i);
	}
	return 0;
}

static void __exit fifo_exit(void){
	print(fifo);
	remove(&fifo);
}


module_init(fifo_init);
module_exit(fifo_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Krzysztof Adamczyk");
MODULE_DESCRIPTION("zad5 fifo");
MODULE_VERSION("1.0");
