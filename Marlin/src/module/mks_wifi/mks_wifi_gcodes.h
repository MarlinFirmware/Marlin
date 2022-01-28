#ifndef MKS_WIFI_GCODES_H
#define MKS_WIFI_GCODES_H

#include "../../gcode/gcode.h"
#include "../temperature.h"
#include "../../sd/cardreader.h"
#include "mks_wifi.h"
#include "mks_wifi_sd.h"

#ifdef MKS_WIFI

void mks_m991(void);
void mks_m997(void);
void mks_m115(void);
void mks_m105(void);
void mks_m23(char *filename);
void mks_m27(void);
void mks_m30(char *filename);
void mks_m20(bool longnames = false);
void mks_printListing(SdFile parent);
void mks_printLongPath(char * const path);

#endif

#endif