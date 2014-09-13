
#ifndef _CALL_RCU_H
#define _CALL_RCU_H

#define MAX_THREAD 10

typedef struct _outdata{
	int count1;
	int count2;
	struct rcu_head rhead;
} ourdata;


extern int init_thread(void); 
extern void clean_thread(void);
#endif
