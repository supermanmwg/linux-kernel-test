#ifndef _RCUTHREAD_H
#define _RCUHREAD_H

#define MAX_THREAD 10

typedef struct _outdata{
	int count1;
	int count2;
} ourdata;


extern int init_thread(void); 
extern void clean_thread(void);
#endif
