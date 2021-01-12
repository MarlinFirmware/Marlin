#ifndef CONFIGURATION_STORE_H
#define CONFIGURATION_STORE_H

#include "Configuration.h"

#include "Marlin.h"
#include "language.h"
#include "planner.h"
#include "temperature.h"
#include "ultralcd.h"
#include "configuration_store.h"
#include "blinkm.h"

void _EEPROM_writeData(int &pos, uint8_t* value, uint8_t size);
void _EEPROM_readData(int &pos, uint8_t* value, uint8_t size);

#define EEPROM_WRITE_VAR(pos, value) _EEPROM_writeData(pos, (uint8_t*)&value, sizeof(value))
#define EEPROM_READ_VAR(pos, value) _EEPROM_readData(pos, (uint8_t*)&value, sizeof(value))

void Config_ResetDefault();

#if DISABLED(DISABLE_M503)
  void Config_PrintSettings(bool forReplay=false);
#else
  FORCE_INLINE void Config_PrintSettings(bool forReplay=false) {}
#endif

#if ENABLED(EEPROM_SETTINGS)
  void Config_StoreSettings();
  void Config_RetrieveSettings();
#else
  FORCE_INLINE void Config_StoreSettings() {}
  FORCE_INLINE void Config_RetrieveSettings() { Config_ResetDefault(); Config_PrintSettings(); }
#endif

#endif //CONFIGURATION_STORE_H
