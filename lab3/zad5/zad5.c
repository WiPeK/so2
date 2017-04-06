#include <linux/module.h>
#include <linux/rbtree.h>
#include <linux/string.h>
#include <linux/slab.h>

//rb przechowywanie łancuchów znakow o roznej dlugosci, przekazane przez parametry inicjacji

static char *array[100];
static int noe = 0;
module_param_array(array, charp, &noe, 0644);
MODULE_PARM_DESC(array, "Char * array of parameters");

struct example_struct
{
	char *data;
	struct rb_node node;
};

static struct rb_root root = RB_ROOT;

static int insert_node(struct rb_root *root, struct example_struct *node)
{
	struct rb_node **new_node = &(root->rb_node), *parent = NULL;
	
	while(*new_node){
		struct example_struct *this = rb_entry(*new_node, struct example_struct, node);
		
		parent = *new_node;
		if(strcmp(node->data, this->data) < 0){
			new_node = &((*new_node)->rb_left);
		}
		else if(strcmp(node->data, this->data) > 0){
			new_node = &((*new_node)->rb_right);
		}
		else
		{
			return 0;
		}
	}
	
	rb_link_node(&node->node, parent, new_node);
	rb_insert_color(&node->node, root);
	return 1;
}

static void print_inorder(void)
{
	struct rb_node *node;
	for(node = rb_first(&root); node; node = rb_next(node))
		pr_notice("rbtree %s", rb_entry(node, struct example_struct, node)->data);
}

static void rmv(struct rb_node *node)
{
	if(node){
		struct example_struct *cur = rb_entry(node, struct example_struct, node);
		rmv(node->rb_left);
		rmv(node->rb_right);
		if(cur){
			rb_erase(&cur->node, &root);
			kfree(cur);
		}
	}
}

static void remove(struct rb_root *root)
{
	struct rb_node *node = root->rb_node;
	rmv(node);
}

static int __init rbmod_init(void)
{
	int i = 0;
	struct example_struct *node = NULL;
	while(array[i] != NULL){
		node = (struct example_struct *)kmalloc(sizeof(struct example_struct), GFP_KERNEL);
		if(!IS_ERR(node)){
			node->data = (char *)kmalloc((sizeof(char)*101), GFP_KERNEL);
			strcpy(node->data, array[i]);
			if(!insert_node(&root, node)){
				pr_notice("error insert");
			}
		}
		else
		{
			pr_notice("err alloc rb");
		}
		pr_notice("test char* array %s", array[i++]);
	}
	return 0;
}

static void __exit rbmod_exit(void)
{
	print_inorder();
	remove(&root);
}

module_init(rbmod_init);
module_exit(rbmod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Krzysztof Adamczyk");
MODULE_DESCRIPTION("lab3 zad5 drzewo czerwono czarne");
MODULE_VERSION("1.0");
