#!/bin/bash -e

# Filter an input Configuration.h file to use customized defines
# call like so
#    cat /Marlin/Configuration.h | ./config.sh DEFINE1 DEFINE2 DEFINE3 > new-config.h
# First the DEFINEs which are set as environment variables will be output
# as
#    #define DEFINE1 $DEFINE1
#    #define DEFINE3 $DEFINE3
# (if 1 and 3 are set but 2 is not)
# next the input stream will be piped through, but stripped of any lines
# that #define _any_ of the arguments
#
# The result is that the source file's definitions of the arguments
# are replaced with those in the environment


PATTERNS_FILE="patterns.temp"
rm -f PATTERNS_FILE

for VAR in $*
do
    echo -e "#define $VAR" >> "$PATTERNS_FILE"
    if [[ -n "${!VAR}" ]]; then
        echo "    #define $VAR ${!VAR}"
    fi
done

grep -v -f "$PATTERNS_FILE" -
rm "$PATTERNS_FILE"
