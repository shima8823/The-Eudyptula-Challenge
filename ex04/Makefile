obj-m += main.o

KDIR := /lib/modules/$(shell uname -r)/build

.PHONY: all install_modules install_rules uninstall_modules uninstall_rules mod_load mod_unload clean

all:
	make -C $(KDIR) M=$(PWD) modules

install_modules:
	make -C $(KDIR) M=$(PWD) modules_install
	depmod -a

install_rules:
	cp 99-usb-keyboard.rules /etc/udev/rules.d/
	udevadm control --reload-rules

uninstall_modules:
	rm -f "$(shell modinfo -n main)"
	depmod -a

uninstall_rules:
	rm -f /etc/udev/rules.d/99-usb-keyboard.rules
	udevadm control --reload-rules

# manual
mod_load:
	modprobe main

# manual
mod_unload:
	modprobe -r main

clean:
	make -C $(KDIR) M=$(PWD) clean