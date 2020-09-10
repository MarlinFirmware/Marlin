#!/usr/bin/env bash

#####################################################################
# genpages.sh for u8g
#
# This script will generate u8g c files for specified fonts
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

#####################################################################

EXEC_GENPAGES=${DN_EXEC}/genpages
[ -x "${EXEC_GENPAGES}" ] || EXEC_GENPAGES="$(my_getpath ${DN_EXEC}/../../../genpages)"
[ -x "${EXEC_GENPAGES}" ] || EXEC_GENPAGES=$(which genpages)
[ -x "${EXEC_GENPAGES}" ] || { echo "Error: genpages not found!" ; exit 1; }
#echo "EXEC_GENPAGES=${EXEC_GENPAGES}"

EXEC_BDF2U8G=${DN_EXEC}/bdf2u8g
[ -x "${EXEC_BDF2U8G}" ] || EXEC_BDF2U8G="${DN_EXEC}/../../../bdf2u8g"
[ -x "${EXEC_BDF2U8G}" ] || EXEC_BDF2U8G=$(which bdf2u8g)
[ -x "${EXEC_BDF2U8G}" ] || { echo "Error: bdf2u8g not found!" ; echo "Please compile u8glib/tools/font/bdf2u8g/bdf2u8g and link to it from here!"; exit 1; }

DN_CUR=$(pwd)
DN_DATA=$(pwd)/datatmp
mkdir -p "${DN_DATA}"

#####################################################################

FONTHOME=/usr/share/fonts

FN_FONT_BASE="marlin-6x12-3"
#FN_FONT_BASE=unifont
#FN_FONT_BASE=wenquanyi_12pt
#FN_FONT_BASE=wenquanyi_9pt

FN_FONT="${1:-}"
DN_FONT0=`dirname ${FN_FONT}`
DN_FONT="$(my_getpath  ${DN_FONT0})"
FN_FONT="$(my_getpath "${DN_FONT}")/"`basename ${FN_FONT}`
[ -z "${FN_FONT}" ] && FN_FONT=${DN_DATA}/../${FN_FONT_BASE}.bdf
[ -f "${FN_FONT}" ] || FN_FONT=${DN_EXEC}/${FN_FONT_BASE}.bdf
[ -f "${FN_FONT}" ] || FN_FONT="$FONTHOME/wenquanyi/${FN_FONT_BASE}.bdf"
[ -f "${FN_FONT}" ] || FN_FONT="$FONTHOME/X11/misc/${FN_FONT_BASE}.bdf"
[ -f "${FN_FONT}" ] || FN_FONT="$FONTHOME/misc/${FN_FONT_BASE}.bdf"
#echo "uxggenpages.sh: FN_FONT=${FN_FONT}"

if [ ! -f "${FN_FONT}" ]; then
  FN_FONT_PCF="$FONTHOME/X11/misc/${FN_FONT_BASE}.pcf"
  [ -f "${FN_FONT_PCF}" ] || FN_FONT_PCF="$FONTHOME/misc/${FN_FONT_BASE}.pcf"
  [ -f "${FN_FONT_PCF}" ] || FN_FONT_PCF="$FONTHOME/wenquanyi/${FN_FONT_BASE}.pcf"
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

[ -f "${FN_FONT}" ] || { echo "Error: can't find font ${FN_FONT}!" ; exit 1; }

#####################################################################

#(cd ${DN_EXEC}; gcc -o genpages genpages.c getline.c)

rm -f tmpa tmpb
touch tmpa tmpb
#rm -f ${DN_EXEC}/fontpage_*.h
rm -f fontpage_*.h

cat << EOF >"proc.awk"
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

AWK=$(which gawk || which awk)

grep -Hrn _UxGT . | grep '"' \
  | sed 's/_UxGT("/\n&/g;s/[^\n]*\n_UxGT("\([^"]*\)[^\n]*/\1 /g;s/.$//' \
  | ${EXEC_GENPAGES} \
  | sort -k 1n -k 2n | uniq \
  | "$AWK" -v EXEC_PREFIX=${DN_EXEC} -f "proc.awk" \
  | while read PAGE BEGIN END UTF8BEGIN UTF8END; do \
    if [ ! -f ${DN_DATA}/fontpage_${PAGE}_${BEGIN}_${END}.h ]; then \
      ${EXEC_BDF2U8G} -u ${PAGE} -b ${BEGIN} -e ${END} ${FN_FONT} fontpage_${PAGE}_${BEGIN}_${END} ${DN_DATA}/fontpage_${PAGE}_${BEGIN}_${END}.h > /dev/null 2>&1 ;
    fi ; \
    grep -A 10000000000 u8g_fntpgm_uint8_t ${DN_DATA}/fontpage_${PAGE}_${BEGIN}_${END}.h >> tmpa ; \
    echo "    FONTDATA_ITEM(${PAGE}, ${BEGIN}, ${END}, fontpage_${PAGE}_${BEGIN}_${END}), // '${UTF8BEGIN}' -- '${UTF8END}'" >> tmpb ;\
  done

TMPA=$(cat tmpa)
TMPB=$(cat tmpb)

EOL=$'\n'
[[ ! "$TMPA" == "" ]] && TMPA="$EOL$TMPA$EOL"
[[ ! "$TMPB" == "" ]] && TMPB="$EOL$TMPB$EOL"

rm -f tmpa tmpb "proc.awk"

cat <<EOF >fontutf8-data.h
/**
 * Generated automatically by buildroot/share/fonts/uxggenpages.sh
 * Contents will be REPLACED by future processing!
 * Use genallfont.sh to generate font data for updated languages.
 */
#include <U8glib.h>
$TMPA
#define FONTDATA_ITEM(page, begin, end, data) { page, begin, end, COUNT(data), data }
static const uxg_fontinfo_t g_fontinfo[] PROGMEM = {$TMPB};
EOF
