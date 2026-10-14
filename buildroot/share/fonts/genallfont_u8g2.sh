#!/usr/bin/env bash

#####################################################################
# genallfont_u8g2.sh for Marlin
#
# This script generates U8g2 font data for language headers
#
# Usage: genallfont_u8g2.sh [lang ...]
#
# Copyright 2015-2018 Yunhui Fu <yhfudev@gmail.com>
# License: GPL/BSD
#####################################################################

DN_EXEC=$(cd "$(dirname "$0")" && pwd)

#
# Get language arguments
#
LANG_ARG="$@"

#
# Use 6x12 combined font data for Western languages
#
FN_FONT="${DN_EXEC}/marlin-6x12-3.bdf"

#
# Find a BDF font locally or in the system font folders
#
find_bdf() {
  local F
  for F in "${DN_EXEC}/$1.bdf" "/usr/share/fonts/wenquanyi/$1.bdf" "/usr/share/fonts/X11/misc/$1.bdf" "/usr/share/fonts/misc/$1.bdf"; do
    [ -f "$F" ] && { echo "$F" ; return ; }
  done
  echo "Error: can't find font $1.bdf!" >&2
  exit 1
}

#
# Change to working directory 'Marlin'
#
OLDWD=`pwd`
[[ $(basename "$OLDWD") != 'Marlin' && -d "Marlin" ]] && cd Marlin
[[ -f "Configuration.h" ]] || { echo -n "cd to the 'Marlin' folder to run " ; basename $0 ; exit 1; }

#
# Compile the 'genpages.exe' and 'bdfconv' commands in-place
#
if [[ ! -x "${DN_EXEC}/genpages/genpages.exe" ]]; then
  echo "Building genpages.exe..."
  ( cd ${DN_EXEC}/genpages ; cc -o genpages.exe genpages.c getline.c )
  [[ -x "${DN_EXEC}/genpages/genpages.exe" ]] || { echo "Build of genpages.exe failed" ; exit 1 ; }
fi

#
# Download the 'bdfconv' source from the u8g2 project if needed
#
BDFCONV_COMMIT=e9baf290435b2cb7ef153bd1e5cdea48807163ef
BDFCONV_URL="https://raw.githubusercontent.com/olikraus/u8g2/${BDFCONV_COMMIT}/tools/font/bdfconv"
BDFCONV_FILES="Makefile main.c bdf_8x8.c bdf_font.c bdf_glyph.c bdf_kern.c bdf_map.c bdf_parser.c bdf_rle.c bdf_tga.c fd.c bdf_font.h bdf_glyph.h fd.h"

if [[ ! -x "${DN_EXEC}/bdfconv/bdfconv" ]]; then
  if [[ ! -f "${DN_EXEC}/bdfconv/main.c" ]]; then
    echo "Downloading bdfconv..."
    mkdir -p ${DN_EXEC}/bdfconv
    for F in $BDFCONV_FILES; do
      if command -v curl >/dev/null; then curl -fsSL -o ${DN_EXEC}/bdfconv/$F "${BDFCONV_URL}/$F"
      else wget -q -O ${DN_EXEC}/bdfconv/$F "${BDFCONV_URL}/$F"; fi \
        || { echo "Download of bdfconv/$F failed" ; rm -rf ${DN_EXEC}/bdfconv ; exit 1 ; }
    done
  fi
  echo "Building bdfconv..."
  ( cd ${DN_EXEC}/bdfconv ; make )
  [[ -x "${DN_EXEC}/bdfconv/bdfconv" ]] || { echo "Build of bdfconv failed" ; exit 1 ; }
fi
EXEC_BDFCONV="${DN_EXEC}/bdfconv/bdfconv"

#
# By default loop through all languages
#
LANGS_DEFAULT="an bg ca cz da de el el_CY el_gr en es eu fi fr fr_na gl hg hr hu id it jp_kana ko_KR nl pl pt pt_br ro ru sk sv tr uk vi zh_CN zh_TW test"

DN_WORK=$(mktemp -d)

#
# Generate data for language list MARLIN_LANGS or all if not provided
#
for ALANG in ${LANG_ARG:=$LANGS_DEFAULT} ; do
  echo "Generating Marlin U8g2 language data for '${ALANG}'" >&2
  case "$ALANG" in
     ko_* ) FONTFILE=$(find_bdf NanumGothic) ;;
        * ) FONTFILE="${FN_FONT}" ;;
  esac
  cp Configuration.h ${DN_WORK}/
  cp src/lcd/language/language_${ALANG}.h ${DN_WORK}/
  # Find and copy included language files
  sed -n 's/^#include "\(language_[^"]*\.h\)".*/\1/p' "src/lcd/language/language_${ALANG}.h" |
    while IFS= read -r f; do
      src="src/lcd/language/$f"
      [ -f "$src" ] && cp "$src" "$DN_WORK/"
    done
  cd "${DN_WORK}"
  ${DN_EXEC}/uxggenpages_u8g2.sh "${FONTFILE}" $ALANG || exit 1
  cd - >/dev/null
  mv ${DN_WORK}/fontutf8-data.h src/lcd/dogm/fontdata/langdata_${ALANG}_u8g2.h
  rm -rf ${DN_WORK}/*
done

rm -rf ${DN_WORK}

#
# Generate default ASCII font (char range 0-255):
#   Marlin/src/lcd/dogm/fontdata/fontdata_ISO10646_1_u8g2.h
#
DN_WORK=$(mktemp -d)
cd ${DN_WORK}
${EXEC_BDFCONV} -f 1 -b 0 -m '1-127' -n ISO10646_1_5x7_u8g2 -o tmp1.h ${FN_FONT} >/dev/null
${EXEC_BDFCONV} -f 1 -b 0 -m '1-255' -n ISO10646_1_5x7_u8g2 -o tmp2.h ${FN_FONT} >/dev/null
TMP1=$(python3 "${DN_EXEC}/u8g2_hexfont.py" tmp1.h) || exit 1
TMP2=$(python3 "${DN_EXEC}/u8g2_hexfont.py" tmp2.h) || exit 1
cd - >/dev/null
rm -rf ${DN_WORK}

cat <<EOF >src/lcd/dogm/fontdata/fontdata_ISO10646_1_u8g2.h
/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include <U8g2lib.h>

#if ALL(__AVR__, NOT_EXTENDED_ISO10646_1_5X7)
  // reduced font (only symbols 1 - 127) - saves about 1278 bytes of FLASH

$TMP1
#else
  // extended (original) font (symbols 1 - 255)

$TMP2

#endif
EOF

#
# Generate the small Info Screen font (char range 1-255):
#   Marlin/src/lcd/dogm/fontdata/fontdata_6x9_marlin_u8g2.h
#
DN_WORK=$(mktemp -d)
${EXEC_BDFCONV} -f 1 -b 0 -m '1-255' -n u8g2_font_6x9_marlin -o ${DN_WORK}/tmp.h ${DN_EXEC}/marlin-6x9.bdf >/dev/null
cat <<EOF >src/lcd/dogm/fontdata/fontdata_6x9_marlin_u8g2.h
/**
 * Generated automatically by buildroot/share/fonts/genallfont_u8g2.sh
 * Contents will be REPLACED by future processing!
 */
#pragma once

#include <U8g2lib.h>

$(python3 "${DN_EXEC}/u8g2_hexfont.py" ${DN_WORK}/tmp.h)
EOF
rm -rf ${DN_WORK}

cd "$OLDWD"
