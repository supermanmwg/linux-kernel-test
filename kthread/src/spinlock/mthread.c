#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <mthread.h>

#define MAX_THREAD 10
MODULE_LICENSE("GPL");
MODULE_AUTHOR("mwg");
MODULE_DESCRIPTION("this is a test for rw lock and multiple thread");

static struct task_struct *thread[MAX_THREAD];
static ourdata mydata;


#ifdef STATIC_INIT_THREAD
static DEFINE_SPINLOCK(thread_lock);
#else
static spinlock_t thread_lock;
static void create_threadlock(void)
{
	spin_lock_init(&thread_lock);
	printk("dynamic init thread\n");
}
#endif


static int thread_do(void *data)
{
	printk("thread %ld run....\n",(long)(data));
	while(!kthread_should_stop())
	{
		spin_lock(&thread_lock);
		mydata.count1++;
		mydata.count2 += 10;
//		printk("count1 = %d,count2 =%d\n",mydata.count1,\
				mydata.count2);
		spin_unlock(&thread_lock);
		msleep(10);
	}
	return 0;
}

static int init_thread(void)
{
	
	int i;
	for (i=0; i<MAX_THREAD; i++)
	{
		thread[i] = kthread_run(thread_do,(void *)(long)i,"thread %d\n",i);
		if(IS_ERR(thread[i]))
			return -1;
	}

	return 0;
}

static void clean_thread(void)
{
	int i;
	for (i=0 ;i<MAX_THREAD; i++)
	{
		if(thread[i])
			kthread_stop(thread[i]);
	}
}

static int __init kthread_init(void)
{
	printk("init thread\n");
	printk("Function name is %s\n",__FUNCTION__);

#ifndef STATIC_INIT_THREAD
	create_threadlock();
#else 
	printk("static init threadlock\n");
#endif
	
	if(init_thread())
		goto err;

	return 0;
	
err:
	clean_thread();
	return -1;
}

static void __exit kthread_exit(void)
{
	printk("exit thread\n");
	printk("Function name is %s\n",__FUNCTION__);
	clean_thread();
}

module_init(kthread_init)
module_exit(kthread_exit)
