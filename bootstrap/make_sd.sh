#!/bin/sh

device=/dev/sdb

if [ $1 != "" ]; then
    device=$1
fi

make -C bl1 clean
make -C bl2_blink clean

make -C bl1
make -C bl2_blink
../tools/mmcboot/run $device bl1/bl1.bin bl2_blink/img.bin
sync
