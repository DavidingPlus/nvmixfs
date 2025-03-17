KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
SRC_DIR := src


all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD)/$(SRC_DIR) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD)/$(SRC_DIR) clean
