#! /usr/bin/env sh

# Quick build script for RAMBo

# Check if build dir exists
build_dir="$HOME/projects/Marlin/build/"
build_dir_check_rm

mkdir $build_dir
# Build with arduino's cli gcc wrapper
arduino-builder -build-options-file /Users/pizzyflavin/projects/Marlin/build.options.json -build-path $build_dir Marlin/Marlin.ino

# Rename hex to not confuse avrdude with .ino ext
hexfile="$HOME/projects/Marlin/build/Marlin.ino.hex"
target_hex="$HOME/projects/Marlin/build/fw.hex"
used_hex="$HOME/projects/Marlin/build/MarlinUsed.ino.hex"

if [ -f $hexfile ]; then
    mv $hexfile $target_hex
    echo "New hex file, renaming to fw.hex..."
elif [ -f $used_hex ]; then
    echo "Using old hex file..."
else
    echo "ERROR: hex file not found!"
    
    exit 1
fi

avrdude -p m2560 -b 115200 -c wiring -P /dev/tty.usbmodem1461 -D -U flash:w:$target_hex

# rename hex file back to what it was
mv $target_hex $build_dir/MarlinUsed.ino.hex
build_dir_check_rm

build_dir_check_rm() {
    if [ -d $build_dir ]; then
        rm -r $build_dir
    fi
}
