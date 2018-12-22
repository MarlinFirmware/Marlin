#!/bin/bash

#This script copies custom configs to their respective directories for building

while true; do
  read -p "Copy custom configs to build locations? [y/n] " yn
  case $yn in
    [Yy]* )
      build_dir="/mnt/c/Users/christine/dev/Marlin/Marlin"
      config_dir="$build_dir/src/config/custom/Marlin"

      #mod_today=$(find "$config_dir" -type f -mtime -1 -daystart)
      #for each in $mod_today; do
      #  cp -idr "$each" "$build_dir"
      #done

      cp -ir "$config_dir/." "$build_dir"
      break
      ;;
    [Nn]* )
      echo "No changes have been made"
      break
      ;;
    * )
      echo "Select Y/N"
      ;;
  esac
done

echo "Script done"
