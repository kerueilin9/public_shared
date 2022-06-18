#!/bin/bash

mknod /dev/demo c 60 0
make clean
make
sudo rmmod demo.ko
sudo insmod demo.ko
