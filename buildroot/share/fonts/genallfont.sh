#!/usr/bin/env bash

#####################################################################
# genallfont.sh for Marlin
#
# This script generates font data for language headers
#
# Copyright 2015-2018 Yunhui Fu <yhfudev@gmail.com>
# License: GPL/BSD
#####################################################################
my_getpath() {
  local PARAM_DN="$1"
  shift
  #readlink -f
  local DN="${PARAM_DN}"
  local FN=
  if [ ! -d "${DN}" ]; then
    FN=$(basename "${DN}")
    DN=$(dirname "${DN}")
  fi
  cd "${DN}" > /dev/null 2>&1
  DN=$(pwd)
  cd - > /dev/null 2>&1
  echo -n "${DN}"
  [[ -z "$FN" ]] || echo -n "/${FN}"
}

DN_EXEC=$(dirname $(my_getpath "$0") )

#
# Get language arguments
#
LANG_ARG="$@"

#
# Use 6x12 combined font data for Western languages
#
FN_FONT="${DN_EXEC}/marlin-6x12-3.bdf"

#
# Change to working directory 'Marlin'
#
OLDWD=`pwd`
[[ $(basename "$OLDWD") != 'Marlin' && -d "Marlin" ]] && cd Marlin
[[ -f "Configuration.h" ]] || { echo -n "cd to the 'Marlin' folder to run " ; basename $0 ; exit 1; }

#
# Compile the 'genpages.exe' and 'bdf2u8g.exe' commands in-place
#
if [[ ! -x "${DN_EXEC}/genpages/genpages.exe" ]]; then
  echo "Building genpages.exe..."
  ( cd ${DN_EXEC}/genpages ; cc -o genpages.exe genpages.c getline.c )
  [[ -x "${DN_EXEC}/genpages/genpages.exe" ]] || { echo "Build of genpages.exe failed" ; exit 1 ; }
fi

if [[ ! -x "${DN_EXEC}/bdf2u8g/bdf2u8g.exe" ]]; then
  echo "Building bdf2u8g.exe..."
  ( cd ${DN_EXEC}/bdf2u8g ; make )
  [[ -x "${DN_EXEC}/bdf2u8g/bdf2u8g.exe" ]] || { echo "Build of bdf2u8g.exe failed" ; exit 1 ; }
fi

#
# By default loop through all languages
#
LANGS_DEFAULT="an bg ca cz da de el el_CY en es eu fi fr gl hr hu it jp_kana ko_KR nl pl pt pt_br ro ru sk sv tr uk vi zh_CN zh_TW test"

#
# Generate data for language list MARLIN_LANGS or all if not provided
#
for ALANG in ${LANG_ARG:=$LANGS_DEFAULT} ; do
  echo "Generating Marlin language data for '${ALANG}'" >&2
  case "$ALANG" in
     zh_* ) FONTFILE="wenquanyi_12pt" ;;
     ko_* ) FONTFILE="${DN_EXEC}/NanumGothic.bdf" ;;
        * ) FONTFILE="${DN_EXEC}/marlin-6x12-3.bdf" ;;
  esac
  DN_WORK=$(mktemp -d)
  cp Configuration.h ${DN_WORK}/
  cp src/lcd/language/language_${ALANG}.h ${DN_WORK}/
  cd "${DN_WORK}"
  ${DN_EXEC}/uxggenpages.sh "${FONTFILE}" $ALANG
  sed -i fontutf8-data.h -e 's|fonts//|fonts/|g' -e 's|fonts//|fonts/|g' -e 's|[/0-9a-zA-Z_\-]*buildroot/share/fonts|buildroot/share/fonts|' 2>/dev/null
  cd - >/dev/null
  mv ${DN_WORK}/fontutf8-data.h src/lcd/dogm/fontdata/langdata_${ALANG}.h
  rm -rf ${DN_WORK}
done

#
# Generate default ASCII font (char range 0-255):
#   Marlin/src/lcd/dogm/fontdata/fontdata_ISO10646_1.h
#
EXEC_BDF2U8G="${DN_EXEC}/bdf2u8g/bdf2u8g.exe"
#if [ "${MARLIN_LANGS}" == "${LANGS_DEFAULT}" ]; then
if [ 1 = 1 ]; then
  DN_WORK=$(mktemp -d)
  cd ${DN_WORK}
  ${EXEC_BDF2U8G} -b 1 -e 127 ${FN_FONT} ISO10646_1_5x7 tmp1.h >/dev/null
  ${EXEC_BDF2U8G} -b 1 -e 255 ${FN_FONT} ISO10646_1_5x7 tmp2.h >/dev/null
  TMP1=$(cat tmp1.h)
  TMP2=$(cat tmp2.h)
  cd - >/dev/null
  rm -rf ${DN_WORK}

  cat <<EOF >src/lcd/dogm/fontdata/fontdata_ISO10646_1.h
/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include <U8glib-HAL.h>

#if defined(__AVR__) && ENABLED(NOT_EXTENDED_ISO10646_1_5X7)
  // reduced font (only symbols 1 - 127) - saves about 1278 bytes of FLASH

$TMP1
#else
  // extended (original) font (symbols 1 - 255)

$TMP2

#endif
EOF

fi

cd "$OLDWD"
