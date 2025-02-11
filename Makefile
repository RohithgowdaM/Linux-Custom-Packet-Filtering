obj-m += simple_net_driver.o

all:
	@echo "Building the simple_net_driver module..."
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	@echo "Cleaning up..."
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
