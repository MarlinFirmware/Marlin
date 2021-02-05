#!/usr/bin/env bash

if [ "$1" != "internal" ]; then
  echo "Don't call this script directly, use build_all_configs.sh instead."
  exit 1
fi

SED=$(which gsed || which sed)

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

  # Shamelessly copied from mftest script, skimmed the dependencies
  MB=$( grep -E "^\s*#define MOTHERBOARD" Marlin/Configuration.h | awk '{ print $3 }' | $SED 's/BOARD_//' )
  [[ -z $MB ]] && { echo "Error - Can't read MOTHERBOARD setting." ; exit 2 ; }
  BLINE=$( grep -E "define\s+BOARD_$MB\b" Marlin/src/core/boards.h )
  BNUM=$( $SED -E 's/^.+BOARD_[^ ]+ +([0-9]+).+$/\1/' <<<"$BLINE" )
  BDESC=$( $SED -E 's/^.+\/\/ *(.+)$/\1/' <<<"$BLINE" )
  [[ -z $BNUM ]] && { echo "Error - Can't find $MB in boards list." ; exit 2 ; }
  ENVS=( $( grep -EA1 "MB\(.*\b$MB\b.*\)" Marlin/src/pins/pins.h | grep -E "#include.+//.+(env|$SYS):[^ ]+" | grep -oE "(env|$SYS):[^ ]+" | $SED -E "s/(env|$SYS)://" ) )
  [[ -z $ENVS ]] && { errout "Error - Can't find target(s) for $MB ($BNUM)." ; exit 2 ; }
  ECOUNT=${#ENVS[*]}

  echo "Deduced env: $ENVS ($ECOUNT)"
  echo "Building the firmware now..."
  platformio run --environment $ENVS || { echo "Failed"; exit 3; }
done

echo "Success"
exit 0
