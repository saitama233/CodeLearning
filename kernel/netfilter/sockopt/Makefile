TARGET = socketopt
OBJS = netfilter_sockopt_kernel.o
MDIR = drivers/misc

EXTRA_CFLAGS = -DEXPORT_SYMTAB
CURRENT = $(shell uname -r)
KDIR = /lib/modules/$(CURRENT)/build
PWD = $(shell pwd)
DEST = /lib/modules/$(CURRENT)/kernel/$(MDIR)

obj-m := $(TARGET).o

$(TARGET)-objs := $(OBJS)

default:
	make -C  $(KDIR) SUBDIRS=$(PWD) modules
	gcc -o user netfilter_sockopt_user.c
# $(TARGET).o: $(OBJS)
# 	$(LD) $(LD_RFLAG) -r -o $@ $(OBJS)

insmod:
	insmod $(TARGET).ko
rmmod:
	rmmod $(TARGET).ko

clean:
	-/bin/rm -rf *.o *.ko *.mod.c *.order *.symvers .*.mk .*.o.cmd .*.ko.cmd .tmp_versions
	-rm -rf user

-include $(KDIR)/Rules.make
