#include<linux/module.h>
#include<linux/slab.h>
#include<linux/string.h>

static struct example_struct {
	unsigned int id;
	char example_string[10];
} *example_struct_pointer[5];

static struct kmem_cache *example_cachep[5];

static int tmp = 1;

static void example_constructor(void *argument)
{
	static unsigned int id;
	static char test_string[] = "Test";
	struct example_struct *example = (struct example_struct *)argument;
	example->id = tmp++;
	strcpy(example->example_string,test_string);
	id++;
}

void print_example_struct(struct example_struct *example)
{
	pr_notice("Example struct id: %u\n",example->id);
	pr_notice("Example string field content: %s\n",example->example_string);
}

static int __init slabmod_init(void)
{
	int i;
	for(i = 0; i < 5; i++){
		char str[20] = "example cache";
		sprintf(str, " %d", i);
		example_cachep[i] = kmem_cache_create(str, sizeof(struct example_struct),0, SLAB_HWCACHE_ALIGN|SLAB_POISON|SLAB_RED_ZONE, example_constructor);
		if(IS_ERR(example_cachep[i])) {
			pr_alert("Error creating cache: %ld\n",PTR_ERR(example_cachep[i]));
			return -ENOMEM;
		}

		example_struct_pointer[i] = (struct example_struct *) kmem_cache_alloc(example_cachep[i],GFP_KERNEL);
		if(IS_ERR(example_struct_pointer[i])) {
			pr_alert("Error allocating form cache: %ld\n", PTR_ERR(example_struct_pointer[i]));
			kmem_cache_destroy(example_cachep[i]);
			return -ENOMEM;
		}
	}
	
	
	return 0;
}

static void __exit slabmod_exit(void)
{
	int i;
	for(i=0;i<5;i++){
		if(example_cachep[i]) {
			if(example_struct_pointer[i]) {
				print_example_struct(example_struct_pointer[i]);
				kmem_cache_free(example_cachep[i],example_struct_pointer[i]);
			}
			kmem_cache_destroy(example_cachep[i]);
		}	
	}
	
}

module_init(slabmod_init);
module_exit(slabmod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Arkadiusz Chrobot <a.chrobot@tu.kielce.pl>");
MODULE_DESCRIPTION("A module demonstrating useing of the slab allocator.");
MODULE_VERSION("1.0");
