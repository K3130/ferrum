#obj-m += netif.o

#include ferrum/Kbuild
#module-objs := netif.o $(ferrum-objs)


include ferrum/Kbuild

obj-m += netif.o

netif-objs := test_netif.o $(ferrum-objs)

ccflags-y += -I$(src)/ferrum/include


KDIR ?= /lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

load:
	insmod netif.ko

unload:
	rmmod netif
