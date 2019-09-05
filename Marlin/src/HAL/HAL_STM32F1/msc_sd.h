#pragma once

#include <USBComposite.h>

extern USBMassStorage MarlinMSC;
extern USBCompositeSerial MarlinCompositeSerial;

void MSC_SD_init();
