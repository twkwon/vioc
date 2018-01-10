#!/bin/sh

PWD=`pwd`
PREFIX=$PWD/OUTPUT
CROSS_COMPILE=arm-none-linux-gnueabi-gcc

###########################
# Select TCC Architecture #
# - __ARCH_TCC898X__      #
# - __ARCH_TCC899X__      #
###########################
ARCH=__ARCH_TCC898X__
#ARCH=__ARCH_TCC899X__

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
		CFLAGS="-s -Wall -Werror -static -D$ARCH" \
		--host=arm-linux \
		--target=arm-linux \
		--prefix=$PREFIX
