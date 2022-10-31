#!/usr/bin/env bash

#####################################################################
# get-bdf2u8g.sh for Marlin
#
# This script downloads, patches, and builds bdf2u8g
# License: CC-SA
#
# Execute from within buildroot/share/fonts
#
#####################################################################

HERE=$(pwd)

for CMD in curl unzip patch make; do
  which $CMD >/dev/null || { echo "'$CMD' is required for this script." ; exit 1 ; }
done

[[ $HERE =~ 'buildroot/share/fonts'$ ]] || { echo "Change to buildroot/share/fonts to run $(basename $0)" ; exit 1; }

# Download u8glib
curl -LJO https://github.com/olikraus/u8glib/archive/master.zip
unzip u8glib-master.zip >/dev/null 2>&1

# Patch and build bdf2u8g
cd u8glib-master/tools/font/bdf2u8g
patch -p0 <../../../../u8glib-bdf2u8g.patch bdf2u8g.c >/dev/null 2>&1
make >/dev/null 2>&1
mv bdf2u8g ../../../../

# Clean up
cd -
rm -rf u8glib-master*

cd "$HERE"
