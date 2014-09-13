#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/init.h>
#include <linux/atomic.h>
#include <linux/delay.h>

MODULE_AUTHOR("mwg");
MODULE_DESCRIPTION("this is a test for test_bit");
MODULE_LICENSE("GPL");

#define MAX_THREAD 10

static unsigned long bits;
static struct task_struct *threads[MAX_THREAD];

static void show_bits(void)
{
	int i;
	int num,set;

	num = sizeof(bits) * 8;
	for (i=0; i < num; i++) {
		set = test_bit(i,&bits);
		if (set) {
			printk("bit %d is set\n",i);
		}
	}
}

static int thread_do(void *data)
{
	long i = (long)data;
	printk("thread %ld run ...",i);
	set_bit(i,&bits);
	while(!kthread_should_stop()) {
			msleep(10);
	}
	printk("sb %ld is exit\n",i);

	return 0;
}


static int __init minit(void)
{
	long i;
	for (i=0; i < MAX_THREAD; i++) {
		threads[i] = kthread_run(thread_do,(void *)(long)i,"thread%ld\n",i);
		if (IS_ERR(threads[i])) 
			return 1;
	}

	return 0;
	
}

static void __exit mexit(void)
{
	long i;
	show_bits();
	for (i=0; i < MAX_THREAD; i++) {
		if(threads[i]) {
			printk("thread %ld exit ...\n",i);
			kthread_stop(threads[i]);
		}
	}

}
module_init(minit)
module_exit(mexit)


