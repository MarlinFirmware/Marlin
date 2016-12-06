#!/usr/bin/env bash
#
# findMissingTranslations.sh
#
# Locate all language strings needing an update based on English
#
# Usage: findMissingTranslations.sh [language codes]
#
# If no language codes are specified then all languages will be checked
#
IGNORE_DEFINES="LANGUAGE_EN_H MAPPER_NON SIMULATE_ROMFONT DISPLAY_CHARSET_ISO10646_1 MSG_X MSG_Y MSG_Z MSG_E MSG_H1 MSG_H2 MSG_H3 MSG_H4 MSG_MOVE_E1 MSG_MOVE_E2 MSG_MOVE_E3 MSG_MOVE_E4 MSG_N1 MSG_N2 MSG_N3 MSG_N4 MSG_DIAM_E1 MSG_DIAM_E2 MSG_DIAM_E3 MSG_DIAM_E4 MSG_E1 MSG_E2 MSG_E3 MSG_E4"

[ -d "Marlin" ] && cd "Marlin"

LANG="$@"
FILES=$(ls language_*.h | grep -v language_en.h | sed -E 's/language_([^\.]+)\.h/\1/')
declare -A STRING_MAP

echo -n "Building list of missing strings..."

for i in $(awk '/#define/{print $2}' language_en.h); do
  LANG_LIST=""
  for j in $FILES; do
    [[ $j == "test" ]] && continue
    [[ -n $LANG && ! "${j}" =~ $LANG ]] && continue
    t=$(grep -c "define ${i} " language_${j}.h)
    if [[ $t -eq 0 ]]; then
      for k in ${IGNORE_DEFINES}; do
        [[ $k == $i ]] && continue 2
      done
      LANG_LIST="$LANG_LIST $j"
    fi
  done
  [[ -z $LANG_LIST ]] && continue
  STRING_MAP["$i"]="$LANG_LIST"
done

echo

for K in $( printf "%s\n" "${!STRING_MAP[@]}" | sort ); do
  printf "%-35s :%s\n" "$K" "${STRING_MAP[$K]}"
done
