#!/usr/bin/env bash
#
# make_lang_na.sh
#
# Create non-accented language files given a list of accented language files.
#

which gsed >/dev/null || { echo "gsed is required for this script." ; exit 1 ; }
which perl >/dev/null || { echo "perl is required for this script." ; exit 1 ; }

#
# Get language arguments
#
[ $# ] || { echo "One or more language codes (such as 'fr') must be supplied." ; exit 1 ; }

LANG_ARG="$@"

#
# Change to working directory 'Marlin'
#
OLDWD=`pwd`
[[ $(basename "$OLDWD") != 'Marlin' && -d "Marlin" ]] && cd Marlin
[[ -f "Configuration.h" ]] || { echo -n "cd to the 'Marlin' folder to run " ; basename $0 ; exit 1; }

#
# Generate a non-accented language file
#
for ALANG in $LANG_ARG ; do
  echo "Generating a non-accented language for '${ALANG}'" >&2
  OUTFILE=src/lcd/language/language_${ALANG}_na.h
  cp src/lcd/language/language_${ALANG}.h $OUTFILE
  perl -pi -e 's/\s*#define DISPLAY_CHARSET_.+\n*//g' $OUTFILE
  perl -pi -e 's/\s*constexpr .+ CHARSIZE.+\n*//g' $OUTFILE
  perl -pi -e "s/namespace Language_${ALANG}/#define DISPLAY_CHARSET_ISO10646_1\n#define NOT_EXTENDED_ISO10646_1_5X7\n\nnamespace Language_${ALANG}_na/" $OUTFILE
  gsed -i 'y/āáǎàâäēéěèêīíǐìïîōóǒòöôūúǔùǖǘǚǜüûĀÁǍÀĒÉĚÈÊĪÍǏÌÎŌÓǑÒÔŪÚǓÙǕǗǙǛÜÛÇçÑñ/aaaaaaeeeeeiiiiiioooooouuuuuuuuuuAAAAEEEEEIIIIIOOOOOUUUUUUUUUUCcNn/' $OUTFILE
  perl -pi -e 's/ß/ss/g' $OUTFILE
done

cd "$OLDWD"
