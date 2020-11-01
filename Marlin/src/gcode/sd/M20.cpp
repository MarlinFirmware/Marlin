/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../inc/MarlinConfig.h"

#if ENABLED(SDSUPPORT)

#include "../gcode.h"
#include "../../sd/cardreader.h"

/**
 * M20: List SD card to serial output
 */
void GcodeSuite::M20() {
  if (card.flag.mounted) {
    if ( parser.seen('S' ) and parser.seen('L' ) ) {
        card.ls(TRUE,TRUE);
    
    } else if ( parser.seen('S' ) ) {
        card.ls(TRUE,FALSE); // Request Short (default DSoc 8,3) filenames if user used 'M20 S'
    
    } else if ( parser.seen('L' ) ) {
        #if ENABLED(LONG_FILENAME_HOST_SUPPORT)
          card.ls(FALSE,TRUE); // Request Short (default DSoc 8,3) filenames if user used 'M20 S'
        #else
          SERIAL_ECHOLNPGM("ERROR: Long_FileName_Host_Support Not Enabled!");
        #endif
    
    } else {
        #if ENABLED(LONG_FILENAME_HOST_SUPPORT) 
    
          #if ( ENABLED(M20_Reports_LONG_FileNames) and ENABLED(M20_Reports_DOS_FileNames) )
            card.ls(TRUE,TRUE);

          #elif ENABLED(M20_Reports_LONG_FileNames)  
            card.ls(FALSE,TRUE);

          #elif ENABLED(M20_Reports_DOS_FileNames) 
            card.ls(TRUE,FALSE);
          #endif

        #else
          card.ls(TRUE,FALSE);
        #endif
    } 
  }
  else
    SERIAL_ECHO_MSG(STR_NO_MEDIA);
}

#endif // SDSUPPORT
