#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/rcupdate.h>
#include "callrcu.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("mwg");
MODULE_DESCRIPTION("this is a test for rw lock and multiple thread");

static struct task_struct *thread[MAX_THREAD];
static ourdata mydata;
static ourdata __rcu *p_mydata = &mydata;
static unsigned long reader_bitmap;

static void set_reader_num(int reader)
{
	//there is only one write thread.
	WARN_ON((MAX_THREAD - reader)!=1);

	reader_bitmap = 0;
	while(reader)
		reader_bitmap |= 1 << (--reader);
}

static void read_do(void)
{
	ourdata *data;
	rcu_read_lock();
	data = rcu_dereference(p_mydata);
	printk("reader: count1 = %d,count2 = %d\n",data->count1,data->count2);
	rcu_read_unlock();
}

static void rcu_free(struct rcu_head *head)
{
		ourdata *data;

		data=container_of(head, ourdata, rhead);
	
		kfree(data);
}

static void write_do(void)
{
	ourdata *data,*tmp=p_mydata;

	data = kmalloc(sizeof(ourdata), GFP_KERNEL);
	if (!data) {
		printk("writer kmalloc failed !\n");
		return;
	}

	
	memcpy(data, p_mydata, sizeof(ourdata));
	data->count1++;
	data->count2 += 10;
	printk("write: count1 = %d,count2 = %d\n",data->count1,data->count2);
	rcu_assign_pointer(p_mydata, data);
	if(tmp != &mydata) {
		call_rcu(&tmp->rhead,rcu_free);
	}

}

static void show_data(void)
{
	printk("exit count1 = %d, count2 =%d\n",mydata.count1,mydata.count2);
}

static int thread_do(void *data)
{
	long i = (long)data;
	long reader = reader_bitmap & 1<<i;
	printk("thread %ld run....%s\n", i, reader?"reader":"writer");
	while(!kthread_should_stop()) {
		if(reader)
			read_do();
		else { 
			write_do();
		}

		msleep(100);
	}
	return 0;
}

int init_thread(void)
{
	
	int i;

	set_reader_num(9);

	for (i=0; i<MAX_THREAD; i++) {
		thread[i] = kthread_run(thread_do, (void *)(long)i, "thread %d\n", i);
		if(IS_ERR(thread[i]))
			return -1;
	}

	return 0;
}

void clean_thread(void)
{
	long i;
	show_data();
	for (i=0 ;i<MAX_THREAD; i++) {
		long reader = reader_bitmap & 1<<i;
		if(thread[i]) {
			printk("thread %ld exit....%s\n", i, reader?"reader":"writer");
			kthread_stop(thread[i]);
		}
	}

}

static int __init kthread_init(void)
{
	printk("init thread\n");
	printk("Function name is %s\n", __FUNCTION__);
	

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
	printk("Function name is %s\n", __FUNCTION__);
	clean_thread();
}

module_init(kthread_init)
module_exit(kthread_exit)