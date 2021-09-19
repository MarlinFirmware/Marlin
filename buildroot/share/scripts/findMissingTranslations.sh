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

FILES=$(ls language_*.h | grep -v -E "(_en|_test)\.h" | sed -E 's/language_([^\.]+)\.h/\1/' | tr '\n' ' ')

# Get files matching the given arguments
TEST_LANGS=""
if [[ -n $@ ]]; then
  for K in "$@"; do
    for F in $FILES; do
      [[ "$F" != "${F%$K*}" ]] && TEST_LANGS+="$F "
    done
  done
  [[ -z $TEST_LANGS ]] && { echo "No languages matching $@." ; exit 0 ; }
else
  TEST_LANGS=$FILES
fi

echo "Missing strings for $TEST_LANGS..."

for WORD in $(awk '/Language_Str/{print $3}' language_en.h); do
  [[ $WORD == "MSG_CUBED" ]] && continue
  LANG_LIST=""
  for LANG in $TEST_LANGS; do
    if [[ $(grep -c " ${WORD} " language_${LANG}.h) -eq 0 ]]; then
      INHERIT=$(awk '/using namespace/{print $3}' language_${LANG}.h | sed -E 's/Language_([a-zA-Z_]+)\s*;/\1/')
      if [[ -z $INHERIT || $INHERIT == "en" ]]; then
        LANG_LIST+=" $LANG"
      elif [[ $(grep -c " ${WORD} " language_${INHERIT}.h) -eq 0 ]]; then
        LANG_LIST+=" $LANG"
      fi
    fi
  done
  [[ -z $LANG_LIST ]] && continue;
  printf "%-38s :%s\n" "$WORD" "$LANG_LIST"
done
