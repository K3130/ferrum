

#########################
obj-m := test_vnetint.o



#########################
test_vnetint-y := ferrum/src/log.o main.o



#########################
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean


install:
	insmod test_vnetint.ko

remove:
	rmmod test_vnetint

info:
	modinfo test_vnetint.ko


logs:
	dmesg | tail -20
