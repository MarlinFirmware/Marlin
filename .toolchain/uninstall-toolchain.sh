#!/bin/bash
VERSION=12.2.rel1
HOST_ARCH=x86_64

TOOLCHAIN_PATH=/usr/share/arm-gnu-toolchain-$VERSION-$HOST_ARCH-arm-none-eabi

# remove symlinks
sudo rm /usr/bin/arm-none-eabi-*

# remove toolchain
sudo rm -r $TOOLCHAIN_PATH/
