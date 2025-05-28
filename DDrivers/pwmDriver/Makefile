obj-m := pwm.o
pwm-objs:=pwmDD.o utils.o
KDIR := /home/araujoj/buildroot-2023.02.8/output/build/linux-custom
ARCH ?= arm

CROSS_COMPILE ?= /home/araujoj/buildroot-2023.02.8/output/host/bin/arm-buildroot-linux-gnueabihf-

export ARCH CROSS_COMPILE

KBUILD_TARGETS	:= clean help modules modules_install

.PHONY: all $(KBUILD_TARGETS)

all: modules
$(KBUILD_TARGETS):
	#$(MAKE) -C $(KDIR) M=$(shell pwd) $@ EXTRA_CFLAGS="$(MY_CFLAGS)"
	$(MAKE) -C $(KDIR) M=$(shell pwd) $@

# /home/araujoj/buildroot/output/host/bin/aarch64-buildroot-linux-gnu-gcc relayDDtest.c -o relay.elf
# scp -O pwm.ko   root@10.42.0.99:/etc