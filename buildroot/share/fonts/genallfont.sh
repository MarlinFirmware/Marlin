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
#DN_EXEC=`echo "$0" | ${EXEC_AWK} -F/ '{b=$1; for (i=2; i < NF; i ++) {b=b "/" $(i)}; print b}'`
DN_EXEC=$(dirname $(my_getpath "$0") )

EXEC_WXGGEN="${DN_EXEC}/uxggenpages.sh"

EXEC_BDF2U8G=`which bdf2u8g`
[ -x "${EXEC_BDF2U8G}" ] || EXEC_BDF2U8G="${DN_EXEC}/bdf2u8g"
[ -x "${EXEC_BDF2U8G}" ] || EXEC_BDF2U8G="${PWD}/bdf2u8g"
[ -x "${EXEC_BDF2U8G}" ] || { EOL=$'\n' ; echo "ERR: Can't find bdf2u8g!${EOL}See uxggenpages.md for bdf2u8g build instructions." >&2 ; exit 1; }

#FN_FONT="${1:-}"
FN_FONT="${DN_EXEC}/marlin-6x12-3.bdf"
[ "$1" = "" ] || FN_FONT="$1"

DN_FONT0=`dirname ${FN_FONT}`
DN_FONT="$(my_getpath  ${DN_FONT0})"
FN_FONT="$(my_getpath "${DN_FONT}")/"`basename ${FN_FONT}`
[ -z "${FN_FONT}" ] && FN_FONT=${DN_DATA}/../${FN_FONT_BASE}.bdf
[ -f "${FN_FONT}" ] || FN_FONT=${DN_EXEC}/${FN_FONT_BASE}.bdf
[ -f "${FN_FONT}" ] || FN_FONT="$FONTHOME/wenquanyi/${FN_FONT_BASE}.bdf"
[ -f "${FN_FONT}" ] || FN_FONT="$FONTHOME/X11/misc/${FN_FONT_BASE}.bdf"
[ -f "${FN_FONT}" ] || FN_FONT="$FONTHOME/misc/${FN_FONT_BASE}.bdf"
#echo "genallfont.sh: FN_FONT=${FN_FONT}"

DN_WORK=./tmp1

(cd ${DN_EXEC}; gcc -o genpages genpages.c getline.c)

LANGS_DEFAULT="an bg ca cz da de el el-gr en es eu fi fr gl hr it jp-kana ko_KR nl pl pt pt-br ru sk tr uk zh_CN zh_TW test"

for LANG in ${MARLIN_LANGS:=$LANGS_DEFAULT} ; do
  echo "Generating Marlin language data for '${LANG}'" >&2
  rm -rf ${DN_WORK}/
  mkdir -p ${DN_WORK}
  cp Configuration.h ${DN_WORK}/
  cp src/lcd/language/language_${LANG}.h ${DN_WORK}/
  cd ${DN_WORK}/
  ${EXEC_WXGGEN} "${FN_FONT}"
  sed -i fontutf8-data.h -e 's|fonts//|fonts/|g' -e 's|fonts//|fonts/|g' -e 's|[/0-9a-zA-Z_\-]*buildroot/share/fonts|buildroot/share/fonts|' 2>/dev/null
  cd ../
  mv ${DN_WORK}/fontutf8-data.h src/lcd/dogm/fontdata/langdata_${LANG}.h
  rm -rf ${DN_WORK}/
done

#
# Generate default ASCII font (char range 0-255):
#   Marlin/src/lcd/dogm/fontdata/fontdata_ISO10646_1.h
#
#if [ "${MARLIN_LANGS}" == "${LANGS_DEFAULT}" ]; then
if [ 1 = 1 ]; then
  rm -rf ${DN_WORK}/
  mkdir -p ${DN_WORK}
  cd ${DN_WORK}/
  ${EXEC_BDF2U8G} -b 1 -e 127 ${FN_FONT} ISO10646_1_5x7 tmp1.h >/dev/null
  ${EXEC_BDF2U8G} -b 1 -e 255 ${FN_FONT} ISO10646_1_5x7 tmp2.h >/dev/null
  TMP1=$(cat tmp1.h) ; rm tmp1.h
  TMP2=$(cat tmp2.h) ; rm tmp2.h

  cat <<EOF >../src/lcd/dogm/fontdata/fontdata_ISO10646_1.h
/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2018 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include <U8glib.h>

#if defined(__AVR__) && ENABLED(NOT_EXTENDED_ISO10646_1_5X7)
  // reduced font (only symbols 1 - 127) - saves about 1278 bytes of FLASH

$TMP1
#else
  // extended (original) font (symbols 1 - 255)

$TMP2

#endif
EOF

  rmdir ../${DN_WORK}
  cd - >/dev/null
fi
