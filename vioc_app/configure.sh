#!/bin/sh

PWD=`pwd`
PREFIX=$PWD/OUTPUT
CROSS_COMPILE=arm-none-linux-gnueabi-gcc

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
		CFLAGS="-O2 -s" \
		--host=arm-linux \
		--target=arm-linux \
		--prefix=$PREFIX
