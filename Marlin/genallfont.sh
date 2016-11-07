#!/bin/sh

DN_WORK=./tmp1

DN_EXEC=../buildroot/share/fonts/
(cd ${DN_EXEC}; gcc -o genpages genpages.c getline.c)

LANGS="an bg ca zh_CN zh_TW cz da de el el-gr en es eu fi fr gl hr it kana_utf8 nl pl pt-br_utf8 pt_utf8 ru tr test"

for LANG in ${LANGS} ; do
    rm -rf ${DN_WORK}/
    mkdir -p ${DN_WORK}
    cp Configuration.h    ${DN_WORK}/
    cp language_${LANG}.h ${DN_WORK}/
    cd ${DN_WORK}/
    ../../buildroot/share/fonts/uxggenpages.sh ../../buildroot/share/fonts/ISO10646-0-3.bdf
    cd ../
    mv ${DN_WORK}/fontutf8-data.h language_data_${LANG}.h
    rm -rf ${DN_WORK}/
done

