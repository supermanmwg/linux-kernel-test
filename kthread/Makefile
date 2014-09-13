OUTPUT=./output
MODULE=./module
SRC=./src
INFO=./info
obj-m = spinlock.o
obj-m += rcuthread.o
obj-m += callrcu.o
obj-m += mutex.o
obj-m += semaphore.o
obj-m += rwsem.o
obj-m += completion.o
obj-m += rwlock.o
obj-m += test_bit.o

spinlock-y = $(SRC)/spinlock/mthread.o
rcuthread-y = $(SRC)/rcu/rcu_thread.o 
callrcu-y = $(SRC)/call_rcu/rcu_thread.o 
mutex-y = $(SRC)/mutex/mutex.o 
semaphore-y = $(SRC)/semaphore/semaphore.o 
rwsem-y = $(SRC)/rwsem/rwsem.o 
completion-y = $(SRC)/completion/completion.o 
rwlock-y = $(SRC)/rwlock/rwlock.o 
test_bit-y = $(SRC)/test_bit/test_bit.o 

all:
	make -C /lib/modules/`uname -r`/build  EXTRA_CFLAGS="-I$(shell pwd)/include -DSTATIC_INIT_THREAD" M=`pwd`
	mv *.o *.mod.c $(OUTPUT) 
	mv *.ko $(MODULE)
	mv modules.order Module.symvers $(INFO)
	

clean:
	rm $(OUTPUT)/*.o $(MODULE)/*.ko  $(INFO)/modules.order  $(INFO)/Module.symvers $(OUTPUT)/*.mod.c

