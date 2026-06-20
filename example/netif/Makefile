obj-m += netif.o

include $(src)/ferrum/Kbuild
module-objs := netif.o $(ferrum-objs)

KDIR ?= /lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

load:
	insmod netif.ko

unload:
	rmmod netif