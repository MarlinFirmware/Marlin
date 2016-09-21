#!/bin/bash
IGNORE_DEFINES="LANGUAGE_EN_H MAPPER_NON SIMULATE_ROMFONT DISPLAY_CHARSET_ISO10646_1 MSG_H1 MSG_H2 MSG_H3 MSG_H4 MSG_MOVE_E1 MSG_MOVE_E2 MSG_MOVE_E3 MSG_MOVE_E4 MSG_N1 MSG_N2 MSG_N3 MSG_N4 MSG_DIAM_E1 MSG_DIAM_E2 MSG_DIAM_E3 MSG_DIAM_E4 MSG_E1 MSG_E2 MSG_E3 MSG_E4"

for i in `awk '/#define/{print $2}' language_en.h`; do
  for j in `ls language_*.h | grep -v language_en.h`; do
    t=$(grep -c "${i}" ${j})
    if [ "$t" -eq 0 ]; then
      for k in ${IGNORE_DEFINES}; do
        [ "${k}" == "${i}" ] && continue 2;
      done
      echo "${j},${i}"
    fi
  done
done
