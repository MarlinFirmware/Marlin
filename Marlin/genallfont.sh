#!/bin/sh

DN_WORK=./tmp1

DN_EXEC=../buildroot/share/fonts/
(cd ${DN_EXEC}; gcc -o genpages genpages.c getline.c)

LANGS="cn bg kana_utf8 ru pt_utf8"

for LANG in ${LANGS} ; do
    rm -rf ${DN_WORK}/
    mkdir -p ${DN_WORK}
    cp language_${LANG}.h ${DN_WORK}/
    cd ${DN_WORK}/
    ../../buildroot/share/fonts/uxggenpages.sh
    cd ../
    mv ${DN_WORK}/fontutf8-data.h language_data_${LANG}.h
    rm -rf ${DN_WORK}/
done

