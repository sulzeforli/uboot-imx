#!/bin/sh
export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabihf-
make distclean
make mx6dsysd_defconfig
make menuconfig
make u-boot.imx -j8

