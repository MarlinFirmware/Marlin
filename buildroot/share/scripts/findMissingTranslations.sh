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

LANGHOME="Marlin/src/lcd/language"

[ -d $LANGHOME ] && cd $LANGHOME

FILES=$(ls language_*.h | grep -v -E "(_en|_test)\.h" | sed -E 's/language_([^\.]+)\.h/\1/')
declare -A STRING_MAP

# Get files matching the given arguments
TEST_LANGS=$FILES
if [[ -n $@ ]]; then
  TEST_LANGS=""
  for K in "$@"; do
    for F in $FILES; do
      [[ "$F" != "${F%$K*}" ]] && TEST_LANGS="$TEST_LANGS $F"
    done
  done
fi

echo -n "Building list of missing strings..."

for i in $(awk '/Language_Str/{print $3}' language_en.h); do
  [[ $i == "MSG_CUBED" ]] && continue
  LANG_LIST=""
  for j in $TEST_LANGS; do
    [[ $(grep -c " ${i} " language_${j}.h) -eq 0 ]] && LANG_LIST="$LANG_LIST $j"
  done
  [[ -z $LANG_LIST ]] && continue
  STRING_MAP[$i]=$LANG_LIST
done

echo

for K in $( printf "%s\n" "${!STRING_MAP[@]}" | sort ); do
  case "$#" in
    1 ) echo $K ;;
    * ) printf "%-35s :%s\n" "$K" "${STRING_MAP[$K]}" ;;
  esac
done
