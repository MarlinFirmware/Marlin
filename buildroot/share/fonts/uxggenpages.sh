#!/bin/bash
#####################################################################
# genpages.sh for u8g
#
# This script will generate u8g c files for specified fonts
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
EXEC_GENPAGES=${DN_EXEC}/genpages
if [ ! -x "${EXEC_GENPAGES}" ]; then
    EXEC_GENPAGES="$(my_getpath ${DN_EXEC}/../../../genpages)"
fi
if [ ! -x "${EXEC_GENPAGES}" ]; then
    echo "Error: not found genpages!"
    exit 1
fi
echo "EXEC_GENPAGES=${EXEC_GENPAGES}"

EXEC_BDF2U8G=${DN_EXEC}/bdf2u8g
if [ ! -x "${EXEC_BDF2U8G}" ]; then
    EXEC_BDF2U8G=${DN_EXEC}/../../../bdf2u8g
fi
if [ ! -x "${EXEC_BDF2U8G}" ]; then
    EXEC_BDF2U8G=$(which bdf2u8g)
fi
if [ ! -x "${EXEC_BDF2U8G}" ]; then
    echo "Error: not found bdf2u8g!"
    exit 1
fi

DN_CUR=$(pwd)
DN_DATA=$(pwd)/datatmp
mkdir -p "${DN_DATA}"

#####################################################################
#FN_FONT_BASE=unifont
#FN_FONT_BASE=wenquanyi_12pt
FN_FONT_BASE=wenquanyi_9pt

FN_FONT="${1:-}"
if [ -z "${FN_FONT}" ]; then
    FN_FONT=${DN_DATA}/../${FN_FONT_BASE}.bdf
fi
if [ ! -f "${FN_FONT}" ]; then
    FN_FONT=${DN_EXEC}/${FN_FONT_BASE}.bdf
fi
if [ ! -f "${FN_FONT}" ]; then
    FN_FONT="/usr/share/fonts/wenquanyi/${FN_FONT_BASE}.bdf"
fi
if [ ! -f "${FN_FONT}" ]; then
    FN_FONT="/usr/share/fonts/X11/misc/${FN_FONT_BASE}.bdf"
fi
echo "FN_FONT=${FN_FONT}"
if [ ! -f "${FN_FONT}" ]; then
    FN_FONT="/usr/share/fonts/misc/${FN_FONT_BASE}.bdf"
fi
echo "FN_FONT2=${FN_FONT}"
if [ ! -f "${FN_FONT}" ]; then
    FN_FONT_PCF="/usr/share/fonts/X11/misc/${FN_FONT_BASE}.pcf"
    if [ ! -f "${FN_FONT_PCF}" ]; then
        FN_FONT_PCF="/usr/share/fonts/misc/${FN_FONT_BASE}.pcf"
    fi
    if [ ! -f "${FN_FONT_PCF}" ]; then
        FN_FONT_PCF="/usr/share/fonts/wenquanyi/${FN_FONT_BASE}.pcf"
    fi
    if [ -f "${FN_FONT_PCF}" ]; then
        EXEC_PCF2BDF=$(which pcf2bdf)
        if [ ! -x "${EXEC_PCF2BDF}" ]; then
            echo "Error: not found pcf2bdf!"
            echo "  Please install pcf2bdf."
            exit 1
        fi
        FN_FONT="./${FN_FONT_BASE}.bdf"
        echo ${EXEC_PCF2BDF} -o "${FN_FONT}" "${FN_FONT_PCF}"
        ${EXEC_PCF2BDF} -o "${FN_FONT}" "${FN_FONT_PCF}"
    fi
fi

if [ ! -f "${FN_FONT}" ]; then
    echo "Error: not found font ${FN_FONT}!"
    exit 1
fi

#####################################################################

#(cd ${DN_EXEC}; gcc -o genpages genpages.c getline.c)

rm -f tmpa tmpb
#rm -f ${DN_EXEC}/fontpage_*.h
rm -f fontpage_*.h

cat << EOF > tmp-proc-page.awk
BEGIN {
    cur_page=0;
    val_begin=0;
    val_pre=0;
    utf8_pre="";
    utf8_begin="";
}{
    page=\$1;
    val_real=\$2;
    utf8=\$3;
    # assert (val_real < 128);
    val=val_real + 128;
    if (cur_page != page) {
        if (cur_page != 0) {
            if (val_begin != 0) {
                print cur_page " " val_begin " " val_pre " " utf8_begin " " utf8_pre;
            }
        }
        cur_page=page;
        val_begin=val;
        val_pre=val;
        utf8_begin=utf8;
        utf8_pre=utf8;
    } else {
        if (val_pre + 1 != val) {
            if (cur_page != 0) {
                print cur_page " " val_begin " " val_pre " " utf8_begin " " utf8_pre;
            }
            val_begin=val;
            val_pre=val;
            utf8_begin=utf8;
            utf8_pre=utf8;
        } else {
            val_pre = val;
            utf8_pre=utf8;
        }
    }
} END {
    if (cur_page != 0) {
        print cur_page " " val_begin " " val_pre " " utf8_begin " " utf8_pre;
    }
}
EOF

grep -Hrn _UxGT . | grep '"' | \
  sed 's/_UxGT("/\n&/g;s/[^\n]*\n_UxGT("\([^"]*\)[^\n]*/\1 /g;s/.$//' | \
  ${EXEC_GENPAGES} | \
  sort -k 1n -k 2n | uniq | \
  gawk -v EXEC_PREFIX=${DN_EXEC} -f tmp-proc-page.awk | \
  while read PAGE BEGIN END UTF8BEGIN UTF8END; do \
    if [ ! -f ${DN_DATA}/fontpage_${PAGE}_${BEGIN}_${END}.h ]; then \
      ${EXEC_BDF2U8G} -u ${PAGE} -b ${BEGIN} -e ${END} ${FN_FONT} fontpage_${PAGE}_${BEGIN}_${END} ${DN_DATA}/fontpage_${PAGE}_${BEGIN}_${END}.h > /dev/null 2>&1 ;
      #sed -i 's|#include "u8g.h"|#include "utility/u8g.h"|' ${DN_DATA}/fontpage_${PAGE}_${BEGIN}_${END}.h ;
    fi ;\
    grep -A 10000000000 u8g_fntpgm_uint8_t ${DN_DATA}/fontpage_${PAGE}_${BEGIN}_${END}.h >> tmpa ;\
    echo "    FONTDATA_ITEM(${PAGE}, ${BEGIN}, ${END}, fontpage_${PAGE}_${BEGIN}_${END}), // '${UTF8BEGIN}' -- '${UTF8END}'" >> tmpb ;\
  done

rm -f fontutf8-data.h
echo "/* Generated by $0 */" >> fontutf8-data.h
echo "/* This file is generated automatically, any changes to the file may be REPLACED by future processing. */" >> fontutf8-data.h
echo "/* Please turn to the tools call this script to change the contents! */" >> fontutf8-data.h
echo "" >> fontutf8-data.h
echo "#include <u8g.h>" >> fontutf8-data.h
#echo "#include \"fontutf8u8g.h\"" >> fontutf8-data.h
echo "" >> fontutf8-data.h
cat tmpa >> fontutf8-data.h
echo "" >> fontutf8-data.h
echo "#define FONTDATA_ITEM(page, begin, end, data) {page, begin, end, NUM_ARRAY(data), data}" >> fontutf8-data.h
echo "static const uxg_fontinfo_t g_fontinfo[] PROGMEM = {" >> fontutf8-data.h
cat tmpb >> fontutf8-data.h
echo "};" >> fontutf8-data.h

rm -f tmpa tmpb tmp-proc-page.awk
