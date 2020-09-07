/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * Copyright (c) 2019 BigTreeTech [https://github.com/bigtreetech]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include <USBComposite.h>

#include "../../inc/MarlinConfigPre.h"
#if ENABLED(EMERGENCY_PARSER)
  #include "../../feature/e_parser.h"
#endif

class MarlinUSBCompositeSerial : public USBCompositeSerial {
public:
  MarlinUSBCompositeSerial() : USBCompositeSerial()
    #if ENABLED(EMERGENCY_PARSER)
      , emergency_state(EmergencyParser::State::EP_RESET)
    #endif
    { }

  #if ENABLED(EMERGENCY_PARSER)
    EmergencyParser::State emergency_state;
  #endif
};

extern USBMassStorage MarlinMSC;
extern MarlinUSBCompositeSerial MarlinCompositeSerial;

void MSC_SD_init();
