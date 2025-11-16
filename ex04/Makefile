obj-m += main.o

KDIR := /lib/modules/$(shell uname -r)/build

.PHONY: all modules_install depmod install clean

all:
	make -C $(KDIR) M=$(PWD) modules

modules_install:
	make -C $(KDIR) M=$(PWD) modules_install

depmod:
	depmod -a

install: modules_install depmod

clean:
	make -C $(KDIR) M=$(PWD) clean