#ifndef CONFIGURATIONSTORE_H
#define CONFIGURATIONSTORE_H

#include "Configuration.h"

void Config_ResetDefault();

#ifndef DISABLE_M503
  void Config_PrintSettings(bool forReplay=false);
#else
  FORCE_INLINE void Config_PrintSettings(bool forReplay=false) {}
#endif

#ifdef EEPROM_SETTINGS
  void Config_StoreSettings();
  void Config_RetrieveSettings();
#else
  FORCE_INLINE void Config_StoreSettings() {}
  FORCE_INLINE void Config_RetrieveSettings() { Config_ResetDefault(); Config_PrintSettings(); }
#endif

#endif //CONFIGURATIONSTORE_H
