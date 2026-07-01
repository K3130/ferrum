#!/bin/bash

#Файл для сборки, проверки разного


#########################
clear
make


#########################
dmesg -c > /dev/null 2>&1
insmod test_vnetint.ko



#########################
rmmod test_vnetint
dmesg
make clean
