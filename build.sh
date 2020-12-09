#!/bin/sh
set -e
. ./headers.sh

nasm -f elf kernel/arch/i386/realmode.S -o kernel/arch/i386/realmode.o
for PROJECT in $PROJECTS; do
  (cd $PROJECT && DESTDIR="$SYSROOT" $MAKE install)
done
