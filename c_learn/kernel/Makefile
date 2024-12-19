obj-m:=hello_kernel.o
CURRENT_PATH:=$(shell pwd)
LINUX_KERNAL:=$(shell uname -r)
LINUX_KERNAL_PATH:=/lib/modules/$(LINUX_KERNAL)/build

all:
	make -C $(LINUX_KERNAL_PATH) M=$(CURRENT_PATH) modules
clean:
	make -C $(LINUX_KERNAL_PATH) M=$(CURRENT_PATH) clean