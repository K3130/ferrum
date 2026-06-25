#!/bin/bash


clear

make

dmesg -c > /dev/null 2>&1

insmod netif.ko

dmesg -c

cat /proc/devices | grep ferrum_netif

#create dev
mknod -m0666 /dev/netif c 510 0
ls -l /dev/ | grep netif

cat /dev/netif

#########################
rmmod netif
dmesg
make clean
