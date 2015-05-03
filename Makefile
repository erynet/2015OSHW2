#LELDIR = /lib/modules/$(shell uname -r)/build

obj-m := syscall_wrapper.o

#SRCS_P = process_test.c
#SRCS_T = thread_test.c
#LDFLAGS = -lpthread

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	gcc -O3 process_test.c -o process_test
	gcc -O3 thread_test.c -o thread_test -lpthread

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm -rf process_test
	rm -rf thread_test
	rm -rf *.o
	rm -rf *.c~
