#pragma once

#include "../../core/macros.h"

#ifndef SD_SCK_PIN
  #define SD_SCK_PIN        -1 // P0_15
#endif
#ifndef SD_MISO_PIN
  #define SD_MISO_PIN       -1 // P0_17
#endif
#ifndef SD_MOSI_PIN
  #define SD_MOSI_PIN       -1 // P0_18
#endif
#ifndef SD_SS_PIN
  #define SD_SS_PIN         -1 // P1_23
#endif
#if !defined(SDSS) || SDSS == P_NC // gets defaulted in pins.h
  #undef SDSS
  #define SDSS          SD_SS_PIN
#endif
