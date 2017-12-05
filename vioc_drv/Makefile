
KDIR ?= /home/B070366/JEDI/SAT/TCC/bsp/kernel/

MODULE_NAME = vioc

obj-m := $(MODULE_NAME).o

OBJS += vioc_drv.o \
		vioc_attr.o

$(MODULE_NAME)-objs := $(OBJS)


all:
	make ARCH=arm -C $(KDIR) M=$(PWD) modules

clean:
	make ARCH=arm -C $(KDIR) M=$(PWD) clean
