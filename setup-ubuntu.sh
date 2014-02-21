#!/bin/bash -e

if [ "$(id -u)" != "0" ]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi

ARDUINO_INSTALL_DIR=/usr/share/arduino
DEPENDENCIES_URL=https://github.com/MissionSt/Marlin/archive/Printrboard-Dependencies.tar.gz
TARGET_DIR=$ARDUINO_INSTALL_DIR/hardware
STRIP_COUNT=4

apt-get install -yq make arduino-core curl git
curl -L $DEPENDENCIES_URL | tar -zx -C $TARGET_DIR --strip-components=$STRIP_COUNT

echo "Installed to $TARGET_DIR  Contents are: "
ls -lh $TARGET_DIR
