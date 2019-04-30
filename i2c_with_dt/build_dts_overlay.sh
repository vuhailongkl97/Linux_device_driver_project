#! /bin/bash

echo "get device tree > dt"
dtc -I fs /sys/firmware/devicetree/base > dt
echo "build .dtbo"
dtc -O dtb -o BB-SPI0-01-00A0.dtbo -b 0 -@ BB-SPI0-01-00A0.dts

echo "cp BB-SPI0-01-00A0.dtbo /lib/firmware/"
cp BB-SPI0-01-00A0.dtbo /lib/firmware/

echo "you must update /boot/uEnv.txt for active overlay"
