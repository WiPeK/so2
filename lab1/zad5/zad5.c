#include<linux/module.h>
#include<linux/types.h>
#include<linux/string.h>

static int __init start_init(void){
	pr_notice("Nazwa pliku %s", __FILE__);
	pr_notice("Nr linijki %d", __LINE__);
	return 0;
}

static void __exit end_exit(void)
{

}

module_init(start_init);
module_exit(end_exit);

MODULE_LICENSE("GPL");
