#!/usr/bin/env bash

#####################################################################
# uxggenpages_u8g2.sh for u8g2
#
# Generate one U8g2 font holding every glyph above U+00FF used by the
# language files in the current directory.
#
# Usage: uxggenpages_u8g2.sh <font.bdf> <lang>
# Output: ./fontutf8-data.h
#
# License: BSD
#####################################################################

DN_EXEC=$(cd "$(dirname "$0")" && pwd)

EXEC_GENPAGES=${DN_EXEC}/genpages/genpages.exe
[ -x "${EXEC_GENPAGES}" ] || { echo "Error: genpages not found!" ; exit 1; }

EXEC_BDFCONV=${DN_EXEC}/bdfconv/bdfconv
[ -x "${EXEC_BDFCONV}" ] || { echo "Error: bdfconv not found!" ; exit 1; }

FN_FONT="$1"
LANG="$2"
[ -f "${FN_FONT}" ] || { echo "Error: can't find font ${FN_FONT}!" ; exit 1; }
[ -n "${LANG}" ] || { echo "Error: no language given!" ; exit 1; }

FONT_NAME="fontdata_${LANG}"

# genpages prints "page offset char" for each character above U+00FF,
# where the codepoint is page * 128 + offset. Join runs into ranges.
RANGES=$(grep -Hrn _UxGT . | grep '"' \
  | sed 's/_UxGT("/\n&/g;s/[^\n]*\n_UxGT("\([^"]*\)[^\n]*/\1 /g;s/.$//' \
  | "${EXEC_GENPAGES}" \
  | awk '{ print $1 * 128 + $2 }' \
  | sort -n | uniq \
  | awk '
      NR == 1 { b = e = $1; next }
      $1 == e + 1 { e = $1; next }
      { r = r (r ? "," : "") b "-" e; b = e = $1 }
      END { if (NR) print r (r ? "," : "") b "-" e }')

if [ -n "${RANGES}" ]; then
  "${EXEC_BDFCONV}" -f 1 -b 0 -m "${RANGES}" -n "${FONT_NAME}" -o fontdata.tmp "${FN_FONT}" >/dev/null \
    || { echo "Error: bdfconv failed for ${LANG}!" ; exit 1; }
  FONT_DATA=$(python3 "${DN_EXEC}/u8g2_hexfont.py" fontdata.tmp) \
    || { echo "Error: hex conversion failed for ${LANG}!" ; exit 1; }
  rm -f fontdata.tmp
else
  FONT_DATA="// No characters above U+00FF
static constexpr const uint8_t *${FONT_NAME} = nullptr;"
fi

cat <<EOF >fontutf8-data.h
/**
 * Generated automatically by buildroot/share/fonts/uxggenpages_u8g2.sh
 * Contents will be REPLACED by future processing!
 * Use genallfont_u8g2.sh to generate font data for updated languages.
 */
#pragma once

#include <U8g2lib.h>

${FONT_DATA}
EOF
