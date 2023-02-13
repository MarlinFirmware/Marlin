#!/bin/bash
GCCVER=10.3-2021.10

# see https://askubuntu.com/a/1243405

# extract toolchain
sudo tar vxjf gcc-arm-none-eabi-$GCCVER-x86_64-linux.tar.bz2 -C /usr/share/

# create symlinks
sudo ln -s /usr/share/gcc-arm-none-eabi-$GCCVER/bin/arm-none-eabi-gcc /usr/bin/arm-none-eabi-gcc 
sudo ln -s /usr/share/gcc-arm-none-eabi-$GCCVER/bin/arm-none-eabi-g++ /usr/bin/arm-none-eabi-g++
sudo ln -s /usr/share/gcc-arm-none-eabi-$GCCVER/bin/arm-none-eabi-gdb /usr/bin/arm-none-eabi-gdb
sudo ln -s /usr/share/gcc-arm-none-eabi-$GCCVER/bin/arm-none-eabi-size /usr/bin/arm-none-eabi-size
sudo ln -s /usr/share/gcc-arm-none-eabi-$GCCVER/bin/arm-none-eabi-objcopy /usr/bin/arm-none-eabi-objcopy

# install dependencies
#sudo apt install libncurses-dev
#sudo ln -s /usr/lib/x86_64-linux-gnu/libncurses.so.6 /usr/lib/x86_64-linux-gnu/libncurses.so.5
#sudo ln -s /usr/lib/x86_64-linux-gnu/libtinfo.so.6 /usr/lib/x86_64-linux-gnu/libtinfo.so.5
sudo apt install libncurses5 -y

# test commands are accessible
arm-none-eabi-gcc --version
arm-none-eabi-g++ --version
arm-none-eabi-gdb --version
arm-none-eabi-size --version
