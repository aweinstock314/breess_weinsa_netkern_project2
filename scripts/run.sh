#!/bin/sh
(cd initramfs; find . -print0 | cpio --null -ov --format=newc | gzip -9 > ../initramfs.cpio.gz)
qemu-system-x86_64 -enable-kvm -kernel linux-4.9.45-dev/arch/x86_64/boot/bzImage -initrd initramfs.cpio.gz -nographic -serial mon:stdio -append 'console=ttyS0'
