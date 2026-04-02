# MakeFile

obj-m += r_w.o    # build hello.c as a module

# KDIR = "/home/vass/dev/OS/linux-kernel-modules/LKM"
KDIR := /lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR) M=$(PWD) modules  
clean:
	make -C $(KDIR) M=$(PWD) clean