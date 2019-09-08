/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * power_loss_recovery.h - Resume an SD print after power-loss
 */

#include "../sd/cardreader.h"
#include "../inc/MarlinConfig.h"

#if ENABLED(MIXING_EXTRUDER)
  #include "../feature/mixing.h"
#endif

#if !defined(POWER_LOSS_STATE) && PIN_EXISTS(POWER_LOSS)
  #define POWER_LOSS_STATE HIGH
#endif

//#define DEBUG_POWER_LOSS_RECOVERY
//#define SAVE_EACH_CMD_MODE
//#define SAVE_INFO_INTERVAL_MS 0

typedef struct {
  uint8_t valid_head;

  // Machine state
  float current_position[NUM_AXIS];

  #if HAS_HOME_OFFSET
    float home_offset[XYZ];
  #endif
  #if HAS_POSITION_SHIFT
    float position_shift[XYZ];
  #endif

  uint16_t feedrate;

  #if EXTRUDERS > 1
    uint8_t active_extruder;
  #endif

  int16_t target_temperature[HOTENDS];

  #if HAS_HEATED_BED
    int16_t target_temperature_bed;
  #endif

  #if FAN_COUNT
    uint8_t fan_speed[FAN_COUNT];
  #endif

  #if HAS_LEVELING
    bool leveling;
    float fade;
  #endif

  #if ENABLED(FWRETRACT)
    float retract[EXTRUDERS], retract_hop;
  #endif

  // Mixing extruder and gradient
  #if ENABLED(MIXING_EXTRUDER)
    //uint_fast8_t selected_vtool;
    //mixer_comp_t color[NR_MIXING_VIRTUAL_TOOLS][MIXING_STEPPERS];
    #if ENABLED(GRADIENT_MIX)
      gradient_t gradient;
    #endif
  #endif

  // Relative mode
  bool relative_mode, relative_modes_e;

  // Command queue
  uint8_t queue_length, queue_index_r;
  char queue_buffer[BUFSIZE][MAX_CMD_SIZE];

  // SD Filename and position
  char sd_filename[MAXPATHNAMELENGTH];
  uint32_t sdpos;

  // Job elapsed time
  millis_t print_job_elapsed;

  uint8_t valid_foot;

} job_recovery_info_t;

class PrintJobRecovery {
  public:
    static const char filename[5];

    static SdFile file;
    static job_recovery_info_t info;

    static void init();

    static inline void setup() {
      #if PIN_EXISTS(POWER_LOSS)
        #if ENABLED(POWER_LOSS_PULL)
          #if POWER_LOSS_STATE == LOW
            SET_INPUT_PULLUP(POWER_LOSS_PIN);
          #else
            SET_INPUT_PULLDOWN(POWER_LOSS_PIN);
          #endif
        #else
          SET_INPUT(POWER_LOSS_PIN);
        #endif
      #endif
    }

    static bool enabled;
    static void enable(const bool onoff);
    static void changed();

    static void check();
    static void resume();

    static inline bool exists() { return card.jobRecoverFileExists(); }
    static inline void open(const bool read) { card.openJobRecoveryFile(read); }
    static inline void close() { file.close(); }

    static void purge();
    static void load();
    static void save(const bool force=
      #if ENABLED(SAVE_EACH_CMD_MODE)
        true
      #else
        false
      #endif
      , const bool save_queue=true
    );

  static inline bool valid() { return info.valid_head && info.valid_head == info.valid_foot; }

  #if ENABLED(DEBUG_POWER_LOSS_RECOVERY)
    static void debug(PGM_P const prefix);
  #else
    static inline void debug(PGM_P const prefix) { UNUSED(prefix); }
  #endif

  private:
    static void write();
};

extern PrintJobRecovery recovery;
