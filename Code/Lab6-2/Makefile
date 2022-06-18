obj-m := demo.o
kernel_DIR := /usr/src/linux-headers-4.9.201-tegra-ubuntu18.04_aarch64/kernel-4.9/
PWD := $(shell pwd)

CROSS_COMPILE = aarch64-linux-gnu-
CXX = $(CROSS_COMPILE)g++
LD = $(CROSS_COMPILE)ld

EXE = Lab6_2
SRC = Lab6_2.cpp

all: 
	make -C $(kernel_DIR) M=$(PWD)
	$(CXX) $(SRC) -lpthread -o $(EXE) 
clean: 
	rm *.o *.ko *.mod.c $(EXE)
.PHONY: clean
