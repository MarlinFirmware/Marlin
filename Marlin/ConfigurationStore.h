#ifndef CONFIG_STORE_H
#define CONFIG_STORE_H

#include "Configuration.h"

#ifdef EEPROM_SETTINGS
void Config_StoreSettings();
void Config_RetrieveSettings();
#else
FORCE_INLINE void Config_StoreSettings() {}
FORCE_INLINE void Config_RetrieveSettings() {}
#endif

#ifdef EEPROM_CHITCHAT
void Config_PrintSettings();
#else
FORCE_INLINE void Config_PrintSettings() {}
#endif

void Config_ResetDefault();

#endif


