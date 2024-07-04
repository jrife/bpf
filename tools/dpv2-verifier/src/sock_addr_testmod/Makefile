obj-m += sock_addr_testmod.o

ROOT_DIR:=$(realpath $(shell dirname $(firstword $(MAKEFILE_LIST))))
KERN_DIR=/lib/modules/$(shell uname -r)/build/

host:
	make -C $(KERN_DIR) M=$(ROOT_DIR) modules
install:
	make -C $(KERN_DIR) M=$(ROOT_DIR) modules_install
uninstall:
	make -C $(KERN_DIR) M=$(ROOT_DIR) modules_uninstall
clean:
	make -C $(KERN_DIR) M=$(ROOT_DIR) clean
help:
	make -C $(KERN_DIR) M=$(ROOT_DIR) help
