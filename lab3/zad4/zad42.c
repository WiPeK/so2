#include <linux/module.h>
#include <linux/kfifo.h>
#include <linux/string.h>

//fifo przechowywanie łancuchów znakow o roznej dlugosci, przekazane przez parametry inicjacji

static char *array[100];
static int noe = 0;
module_param_array(array, charp, &noe, 0644);
MODULE_PARM_DESC(array, "Char * array of parameters");

static struct kfifo fifo_queue;

static int __init fifomod_init(void)
{
	int i = 0;
	if(kfifo_alloc(&fifo_queue,sizeof(array), GFP_KERNEL)!=0){
		pr_notice("error alloc fifo");
		return -ENOMEM;
	}
	while(array[i] != NULL){
		if(kfifo_in(&fifo_queue, array[i], sizeof(char)*100)!= (sizeof(char)*100)){
			pr_notice("error enque");
		}
		pr_notice("test char* array %s", array[i++]);
	}
	return 0;
}

static void __exit fifomod_exit(void)
{
	
	while(!kfifo_is_empty(&fifo_queue)){
		char value[100];
		if(kfifo_out(&fifo_queue,value, sizeof(char)*100)!=sizeof(char)*100){
			pr_notice("error deque");
		}
		pr_notice("Dequeue %s", value);
	}
	kfifo_free(&fifo_queue);
}

module_init(fifomod_init);
module_exit(fifomod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Krzysztof Adamczyk");
MODULE_DESCRIPTION("lab3 zad4 fifo");
MODULE_VERSION("1.0");
