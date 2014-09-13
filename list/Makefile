OUTPUT=.
MODULE=.
SRC=.
INFO=.

obj-m = list.o

all:
	make -C /lib/modules/`uname -r`/build M=`pwd`
clean:
	rm $(OUTPUT)/*.o $(MODULE)/*.ko  $(INFO)/modules.order  $(INFO)/Module.symvers $(OUTPUT)/*.mod.c

