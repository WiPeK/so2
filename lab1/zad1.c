#include<linux/module.h>
#include<linux/types.h>

static int __init start_init(void){
	pr_notice("size s8 %d", sizeof(s8));
	pr_notice("size u8 %d", sizeof(u8));
	pr_notice("size s16 %d", sizeof(s16));
	pr_notice("size u16 %d", sizeof(u16));
	pr_notice("size s32 %d", sizeof(s32));
	pr_notice("size u32 %d", sizeof(u32));
	pr_notice("size s64 %d", sizeof(s64));
	pr_notice("size u64 %d", sizeof(u64));
	return 0;
}

static void __exit end_exit(void)
{

}

module_init(start_init);
module_exit(end_exit);

MODULE_LICENSE("GPL");
