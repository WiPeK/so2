#include <linux/module.h>
#include <linux/radix-tree.h>
#include <linux/string.h>
#include <linux/slab.h>

//pozycyjne drzewo przechowywanie łancuchów znakow o roznej dlugosci, przekazane przez parametry inicjacji

static char *array[100];
static int noe = 0;
module_param_array(array, charp, &noe, 0644);
MODULE_PARM_DESC(array, "Char * array of parameters");

static RADIX_TREE(root, GFP_KERNEL);
static unsigned long i = 0;

static int __init rtmod_init(void)
{
	while(array[i] != NULL){
		if(radix_tree_insert(&root, i, (void*)array[i]))
		{
			pr_notice("error inserting to tree");
		}
		pr_notice("test char* array %s", array[i++]);
	}
	return 0;
}

static void print(void)
{
	unsigned long j = 0;
	while(j <= i){
		char * tmp = (char*)radix_tree_lookup(&root, j);
		if(tmp){
			pr_notice("Value from tree %s for index %d", tmp, (int)j);
		}
		j++;
	}
}

static void remove(void)
{
	unsigned long j = 0;
	int *tmp = NULL;
	while(j<=i){
		tmp = (int*)radix_tree_delete(&root, j);
		j++;
	}
}

static void __exit rtmod_exit(void)
{
	print();
	remove();
}

module_init(rtmod_init);
module_exit(rtmod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Krzysztof Adamczyk");
MODULE_DESCRIPTION("lab3 zad6 drzewo pozycyjne");
MODULE_VERSION("1.0");
