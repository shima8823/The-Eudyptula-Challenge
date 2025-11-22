obj-m += main.o

KDIR := /lib/modules/$(shell uname -r)/build

.PHONY: all modules_install depmod install mod_load mod_unload clean

all:
	make -C $(KDIR) M=$(PWD) modules

modules_install:
	make -C $(KDIR) M=$(PWD) modules_install

depmod:
	depmod -a

install: modules_install depmod

mod_load:
  modprobe main

mod_unload:
  modprobe -r main

clean:
	make -C $(KDIR) M=$(PWD) clean