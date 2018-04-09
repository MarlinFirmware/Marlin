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
echo "1 set EXEC_BDF2U8G=$EXEC_BDF2U8G"
fi
if [ ! -x "${EXEC_BDF2U8G}" ]; then
    EXEC_BDF2U8G="${PWD}/bdf2u8g"
echo "2 set EXEC_BDF2U8G=$EXEC_BDF2U8G"
fi
if [ ! -x "${EXEC_BDF2U8G}" ]; then
  echo "Not found bdf2u8g!"
  echo "plaese compile u8blib/tools/font/bdf2u8g/bdf2u8g and link to it from here!"

  exit 1
fi

FN_NEWFONT="${DN_EXEC}/ISO10646-0-3.bdf"
if [ ! "$1" = "" ]; then
  FN_NEWFONT="$1"
fi

DN_WORK=./tmp1

(cd ${DN_EXEC}; gcc -o genpages genpages.c getline.c)

LANGS="an bg ca zh_CN zh_TW cz da de el el-gr en es eu fi fr gl hr it jp-kana nl pl pt pt-br ru sk tr uk test"

for LANG in ${LANGS} ; do
    rm -rf ${DN_WORK}/
    mkdir -p ${DN_WORK}
    cp Configuration.h    ${DN_WORK}/
    cp src/lcd/language/language_${LANG}.h ${DN_WORK}/
    cd ${DN_WORK}/
    ${EXEC_WXGGEN} "${FN_NEWFONT}"
    cd ../
    mv ${DN_WORK}/fontutf8-data.h src/lcd/dogm/language_data_${LANG}.h
    rm -rf ${DN_WORK}/
done

