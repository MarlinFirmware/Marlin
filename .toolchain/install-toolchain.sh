#!/bin/bash
VERSION=12.2.rel1
HOST_ARCH=x86_64

TOOLCHAIN_PATH=/usr/share/arm-gnu-toolchain-$VERSION-$HOST_ARCH-arm-none-eabi

# see https://askubuntu.com/a/1243405

# extract the toolchain
sudo tar vxf ./arm-gnu-toolchain-$VERSION-$HOST_ARCH-arm-none-eabi.tar.xz -C /usr/share/

# create symlinks
sudo ln -s $TOOLCHAIN_PATH/bin/arm-none-eabi-* /usr/bin/

# install dependencies
#sudo apt install libncurses-dev
#sudo ln -s /usr/lib/x86_64-linux-gnu/libncurses.so.6 /usr/lib/x86_64-linux-gnu/libncurses.so.5
#sudo ln -s /usr/lib/x86_64-linux-gnu/libtinfo.so.6 /usr/lib/x86_64-linux-gnu/libtinfo.so.5
sudo apt install libncurses5 -y

# test commands are accessible
arm-none-eabi-gcc --version
arm-none-eabi-g++ --version
arm-none-eabi-objcopy --version
arm-none-eabi-readelf --version
