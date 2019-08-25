#!/bin/sh
set -e
. ./build.sh
 
mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub
 
cp sysroot/boot/terraos.kernel isodir/boot/terraos.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "TerraOS" {
	multiboot /boot/terraos.kernel
}
EOF
grub-mkrescue -o terraos.iso isodir