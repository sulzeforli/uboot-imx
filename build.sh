#!/bin/sh
. /opt/fsl-imx-xwayland/4.14-sumo/environment-setup-cortexa9hf-neon-poky-linux-gnueabi
export ARCH=arm
export CROSS_COMPILE=arm-poky-linux-gnueabi-
#export ARCH=arm
#export CROSS_COMPILE=arm-linux-gnueabihf-
make distclean
make mx6dsysd_defconfig
make menuconfig
make u-boot.imx -j8

