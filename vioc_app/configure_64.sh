#!/bin/sh

PWD=`pwd`
PREFIX=$PWD/OUTPUT

#CROSS_COMPILE=arm-none-linux-gnueabi-gcc
CROSS_COMPILE=aarch64-linux-gnu-gcc

###########################
# Select TCC Architecture #
# - __ARCH_TCC898X__      #
# - __ARCH_TCC899X__      #
# - __ARCH_TCC803X__      #
###########################
#arch_chip=TCC898X
arch_chip=TCC899X
#arch_chip=TCC803X
ARCH="__ARCH_${arch_chip}__"

echo ""
echo $ARCH
echo ""
sleep 1

echo "-----------------"
echo " autoreconf -vfi "
echo "-----------------"
autoreconf -vfi

echo ""
echo "-----------------"
echo " configure       "
echo "-----------------"
./configure \
		CC=$CROSS_COMPILE \
		CFLAGS="-s -Wall -Werror -static -D$ARCH -D__ARM64__" \
		--host=aarch64-linux-gnu \
		--target=aarch64-linux-gnu \
		--prefix=$PREFIX
