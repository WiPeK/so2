#include<linux/module.h>
#include<linux/slab.h>


static struct stack_node {
    int data;
    struct stack_node *next;
} *top, *tmp;

static struct stack_node *push(struct stack_node *top, int number)
{
    struct stack_node *new_node = (struct stack_node *)kmalloc(sizeof(struct stack_node), GFP_KERNEL);
    if(new_node!=NULL) {
        new_node->data = number;
        new_node->next = top;
        top = new_node;
    }
    return top;
}

static int pop(struct stack_node **top)
{
    int result = -1;
    if(*top) {
        result = (*top)->data;
        tmp = (*top)->next;
        kfree(*top);
        *top = tmp;
    }
    return result;
}

static int __init stack_init(void)
{
    int i;
    for(i=0; i<10; i++)
        top=push(top,i);
	return 0;
}

static void __exit stack_exit(void)
{
	while(top)
        pr_notice("stos: %d ",pop(&top));
}

module_init(stack_init);
module_exit(stack_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Krzysztof Adamczyk");
MODULE_DESCRIPTION("zad2 stack");
MODULE_VERSION("1.0");
