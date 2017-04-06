#include<linux/module.h>
#include<linux/list.h>
#include<linux/slab.h>
#include<linux/random.h>

struct example_struct 
{
	int random_value;
	struct list_head list_element;
};

static LIST_HEAD(head);

static int __init listmod_init(void)
{
	struct example_struct *element;
	struct list_head *entry;
	u8 i;
	
	for(i=0;i<4;i++) {
		element = (struct example_struct *)kmalloc(sizeof(struct example_struct),GFP_KERNEL);
		if(!IS_ERR(element)) {
			get_random_bytes((int *)&element->random_value,sizeof(element->random_value));
			INIT_LIST_HEAD(&element->list_element);
			list_add_tail(&element->list_element,&head);
		}
	}
	list_for_each(entry, &head) {
		element = list_entry(entry, struct example_struct, list_element);
		pr_notice("Element's value: %u\n",element->random_value);
	}

	return 0;
}

static void __exit listmod_exit(void)
{
	struct example_struct *element, *next;

	 list_for_each_entry_safe(element, next, &head, list_element) {
		 list_del(&element->list_element);
		 pr_notice("Element's value: %u\n",element->random_value);
		 kfree(element);
	 }
}

module_init(listmod_init);
module_exit(listmod_exit);

MODULE_AUTHOR("Arkadiusz Chrobot <a.chrobot@tu.kielce.pl>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("An exemplary kernel module that demonstrates the usage of a kernel list.");
MODULE_VERSION("1.0");
