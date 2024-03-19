/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2024 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * SpoolJoin.h
 */

#include "../../MarlinCore.h"

  #include <stdint.h>

  // See documentation here: https://help.prusa3d.com/article/spooljoin-mmu2s_134252

  namespace SpoolJoin {

  class SpoolJoin {
public:
    SpoolJoin();

    enum class EEPROM : uint8_t {
      Unknown, ///< SpoolJoin is unknown while printer is booting up
      Enabled, ///< SpoolJoin is enabled in EEPROM
      Disabled, ///< SpoolJoin is disabled in EEPROM
      Empty = 0xFF ///< EEPROM has not been set before and all bits are 1 (0xFF) - either a new printer or user erased the memory
    };

    // @brief Contrary to Prusa's implementation we store the enabled status in a variable
    static int epprom_addr;
    static bool enabled;

    /// @brief Called when EEPROM is ready to be read
    void initSpoolJoinStatus();

    /// @brief Toggle SpoolJoin
    static void toggleSpoolJoin();

    /// @brief Check if SpoolJoin is enabled
    /// @returns true if enabled, false if disabled
    bool isSpoolJoinEnabled();

    /// @brief Update the saved MMU slot number so SpoolJoin can determine the next slot to use
    /// @param slot number of the slot to set
    void setSlot(uint8_t slot);

    /// @brief Fetch the next slot number should count from 0 to 4.
    /// When filament slot 4 is depleted, the next slot should be 0.
    /// @returns the next slot, ranges from 0 to 4
    uint8_t nextSlot();

private:
    /// @brief Currently used slot, ranges from 0 to 4
    uint8_t currentMMUSlot;
  };

  extern SpoolJoin spooljoin;

  } // namespace SpoolJoin
