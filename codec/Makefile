obj-m := bugg-sph061lu4h.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

install:
	sudo cp bugg-sph061lu4h.ko /lib/modules/$(shell uname -r)
	sudo depmod -a
