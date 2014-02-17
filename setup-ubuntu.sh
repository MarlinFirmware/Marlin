#!/bin/bash -e

if [ "$(id -u)" != "0" ]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi

export ARDUINO_INSTALL_DIR=/usr/share/arduino
export S3_REGION=us-west-2
export S3_BUCKET=missionst-blobs

apt-get install -yq make arduino-core curl
curl https://s3-$S3_REGION.amazonaws.com/$S3_BUCKET/teensy-cores.tgz | tar -zx -C $ARDUINO_INSTALL_DIR/hardware/