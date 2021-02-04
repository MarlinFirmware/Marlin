#!/bin/bash
if [ "$1" != "internal" ]; then
  echo "Don't call this script directly, use build_all_configs.sh instead"
  exit 1
fi

shopt -s nullglob
for sub in $3/*/; do
  [ -d "${sub}" ] || continue
  base=`basename "$sub"`

  if [ ! -f $sub/Configuration.h ] || [ ! -f $sub/Configuration_adv.h ]; then
    echo "No configuration files found in $sub"
    continue
  fi

  echo "Copying configuration file from $2 to current working directory"
  cp $sub/Configuration.h Marlin/
  cp $sub/Configuration_adv.h Marlin/
  env=`cat $sub/platformio.env`

  echo "Building the firmware now..."
  platformio run --environment $env || { echo "Failed"; exit 3; }
done

echo "Success"
exit 0