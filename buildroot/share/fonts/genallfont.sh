#!/usr/bin/env bash

#####################################################################
# genallfont.sh for Marlin
#
# This script will generates font data for language headers
#
# Copyright 2015-2016 Yunhui Fu <yhfudev@gmail.com>
# License: GPL/BSD
#####################################################################
my_getpath () {
  PARAM_DN="$1"
  shift
  #readlink -f
  DN="${PARAM_DN}"
  FN=
  if [ ! -d "${DN}" ]; then
    FN=$(basename "${DN}")
    DN=$(dirname "${DN}")
  fi
  cd "${DN}" > /dev/null 2>&1
  DN=$(pwd)
  cd - > /dev/null 2>&1
  echo "${DN}/${FN}"
}
#DN_EXEC=`echo "$0" | ${EXEC_AWK} -F/ '{b=$1; for (i=2; i < NF; i ++) {b=b "/" $(i)}; print b}'`
DN_EXEC=$(dirname $(my_getpath "$0") )
if [ ! "${DN_EXEC}" = "" ]; then
    DN_EXEC="$(my_getpath "${DN_EXEC}")/"
else
    DN_EXEC="${DN_EXEC}/"
fi
#####################################################################
EXEC_WXGGEN="${DN_EXEC}/uxggenpages.sh"

EXEC_BDF2U8G=`which bdf2u8g`
echo "0 set EXEC_BDF2U8G=$EXEC_BDF2U8G"
if [ ! -x "${EXEC_BDF2U8G}" ]; then
    EXEC_BDF2U8G="${DN_EXEC}/bdf2u8g"
fi
if [ ! -x "${EXEC_BDF2U8G}" ]; then
    EXEC_BDF2U8G="${PWD}/bdf2u8g"
fi
if [ ! -x "${EXEC_BDF2U8G}" ]; then
  echo "ERR: Not found bdf2u8g!" >&2
  echo "plaese compile u8blib/tools/font/bdf2u8g/bdf2u8g and link to it from here!" >&2
  exit 1
fi

FN_NEWFONT="${DN_EXEC}/ISO10646-0-3.bdf"
if [ ! "$1" = "" ]; then
  FN_NEWFONT="$1"
fi

DN_WORK=./tmp1

(cd ${DN_EXEC}; gcc -o genpages genpages.c getline.c)

LANGS_DEFAULT="an bg ca zh_CN zh_TW cz da de el el-gr en es eu fi fr gl hr it jp-kana nl pl pt pt-br ru sk tr uk test"

for LANG in ${MARLIN_LANGS:=$LANGS_DEFAULT} ; do
    echo "INFO: generate Marlin language data for '${LANG}'" >&2

    rm -rf ${DN_WORK}/
    mkdir -p ${DN_WORK}
    cp Configuration.h    ${DN_WORK}/
    cp src/lcd/language/language_${LANG}.h ${DN_WORK}/
    cd ${DN_WORK}/
    ${EXEC_WXGGEN} "${FN_NEWFONT}"
    sed -e 's|fonts//|fonts/|g' -e 's|fonts//|fonts/|g' -e 's|[/0-9a-zA-Z_\-]*buildroot/share/fonts|buildroot/share/fonts|' -i fontutf8-data.h
    cd ../
    mv ${DN_WORK}/fontutf8-data.h src/lcd/dogm/language_data_${LANG}.h
    rm -rf ${DN_WORK}/
done


# generate default ASCII font (char range 0-255):
#   Marlin/src/lcd/dogm/dogm_font_data_ISO10646_1.h
#if [ "${MARLIN_LANGS}" == "${LANGS_DEFAULT}" ]; then
if [ 1 = 1 ]; then
    rm -rf ${DN_WORK}/
    mkdir -p ${DN_WORK}
    cd ${DN_WORK}/
    ${EXEC_BDF2U8G} -b 1 -e 127 ${FN_NEWFONT} ISO10646_1_5x7 tmp1.h
    ${EXEC_BDF2U8G} -b 1 -e 255 ${FN_NEWFONT} ISO10646_1_5x7 tmp2.h

    cat << EOF >tmp3.h
#include <U8glib.h>

#if defined(__AVR__) && ENABLED(NOT_EXTENDED_ISO10646_1_5X7)
  // reduced font (only sysmbols 1 - 127) - saves about 1278 bytes of FLASH

EOF
    cat tmp1.h >>tmp3.h
    cat << EOF >>tmp3.h
#else
  // extended (original) font (sysmbols 1 - 255)
EOF
    cat tmp2.h >>tmp3.h
    cat << EOF >>tmp3.h

#endif
EOF
    sed -e 's|#include "u8g.h"|#include <clib/u8g.h>|' -i tmp3.h

    cd ..
    mv ${DN_WORK}/tmp3.h src/lcd/dogm/dogm_font_data_ISO10646_1.h
fi


