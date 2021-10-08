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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

////////////////////////////
// VENDOR VERSION EXAMPLE //
////////////////////////////

/**
 * Marlin release version identifier
 */
#define SHORT_BUILD_VERSION "2.0.9_DW7.4.2"

/**
 * Verbose version identifier which should contain a reference to the location
 * from where the binary was downloaded or the source code was compiled.
 */

#if(ENABLED(MachineMini))
  #define VerChar1 "M"
#elif(ENABLED(MachineEnder2))
  #define VerChar1 "E2"
#elif(ENABLED(MachineEnder3))
  #define VerChar1 "E3"
#elif ENABLED(MachineEnder3V2)
  #define VerChar1 "E3V2"
#elif ENABLED(MachineEnder3Max)
  #define VerChar1 "E3M"
#elif(ENABLED(MachineEnder4))
  #define VerChar1 "E4"
#elif(ENABLED(MachineEnder5))
  #define VerChar1 "E5"
#elif(ENABLED(MachineEnder6))
  #define VerChar1 "E6"
#elif(ENABLED(MachineSermoonD1))
  #define VerChar1 "D1"
#elif(ENABLED(MachineEnder5Plus))
  #define VerChar1 "E5P"
#elif(ENABLED(MachineCR20))
  #define VerChar1 "2"
#elif(ENABLED(MachineCR10Orig))
  #define VerChar1 "O"
#elif(ENABLED(MachineCR10Std))
  #define VerChar1 "S"
#elif(ENABLED(MachineCRX))
  #define VerChar1 "X"
#elif(ENABLED(MachineCR6))
  #define VerChar1 "CR6"
#elif(ENABLED(MachineCR6Max))
  #define VerChar1 "CR6M"
#elif(ENABLED(MachineS4))
  #define VerChar1 "4"
#elif(ENABLED(MachineS5) || ENABLED(MachineCR10Max))
  #define VerChar1 "5"
#elif ENABLED(MachineCR2020)
  #define VerChar1 "20"
#endif

#if(ENABLED(HotendStock))
#define VerChar2 "S"
#elif(ENABLED(HotendE3D))
#define VerChar2 "E"
#elif ENABLED(HotendMosquito)
#define VerChar2 "M"
#endif

#if(ENABLED(HotendAllMetal))
#define VerChar3 "M"
#else
#define VerChar3 "S"
#endif


#if(ENABLED(BedAC))
#define VerChar4 "A"
#elif(ENABLED(BedDC))
#define VerChar4 "D"
#else
#define VerChar4 "N"
#endif

#if(ENABLED(ABL_EZABL))
#define VerChar5 "A"
#elif(ENABLED(ABL_BLTOUCH))
#define VerChar5 "B"
#else
#define VerChar5 "N"
#endif

#if(ENABLED(ABL_UBL))
#define VerChar6 "U"
#elif(ENABLED(ABL_BI))
#define VerChar6 "B"
#else
#define VerChar6 "N"
#endif

#define DETAILED_BUILD_VERSION SHORT_BUILD_VERSION " TM3D " VerChar1 VerChar2 VerChar3 VerChar4 VerChar5 VerChar6

/**
 * The STRING_DISTRIBUTION_DATE represents when the binary file was built,
 * here we define this default string as the date where the latest release
 * version was tagged.
 */
#define STRING_DISTRIBUTION_DATE "2021-09-14"

/**
 * Defines a generic printer name to be output to the LCD after booting Marlin.
 */
#if(ENABLED(MachineMini))
  #define CUSTOM_MACHINE_NAME "Mini SuPeR"
#elif(ENABLED(MachineEnder2))
  #define CUSTOM_MACHINE_NAME "TM3D Ender2"
#elif(ENABLED(MachineEnder3))
  #define CUSTOM_MACHINE_NAME "TM3D Ender3"
#elif(ENABLED(MachineEnder3Max))
  #define CUSTOM_MACHINE_NAME "TM3D E3Max"
#elif(ENABLED(MachineEnder6))
  #define CUSTOM_MACHINE_NAME "TM3D Ender6"
#elif(ENABLED(MachineEnder3V2))
  #define CUSTOM_MACHINE_NAME "TM3D Ender3V2"
#elif(ENABLED(MachineEnder4))
  #define CUSTOM_MACHINE_NAME "TM3D Ender4"
#elif(ENABLED(MachineEnder5))
  #define CUSTOM_MACHINE_NAME "TM3D Ender5"
#elif(ENABLED(MachineEnder5Plus))
  #define CUSTOM_MACHINE_NAME "TM3D E5 Plus"
#elif(ENABLED(MachineCR20))
  #define CUSTOM_MACHINE_NAME "SuPeR CR-20"
#elif(ENABLED(MachineCR10Orig))
  #define CUSTOM_MACHINE_NAME "SuPeR CR-10"
#elif(ENABLED(MachineCRX))
  #define CUSTOM_MACHINE_NAME "TM3D CR-X"
#elif ENABLED(MachineCR10SProV2)
  #define CUSTOM_MACHINE_NAME "TM3D 10SProV2"
#elif(ENABLED(MachineCR10SPro))
  #define CUSTOM_MACHINE_NAME "TM3D 10S Pro"
#elif(ENABLED(MachineCR10Max))
  #define CUSTOM_MACHINE_NAME "TM3D Max"
#elif(ENABLED(MachineCR10SV3))
  #define CUSTOM_MACHINE_NAME "TM3D CR10V3"
#elif(ENABLED(MachineCR10SV2))
  #define CUSTOM_MACHINE_NAME "TM3D CR10V2"
#elif(ENABLED(MachineCR10Std))
  #define CUSTOM_MACHINE_NAME "300 SuPeR"
#elif(ENABLED(MachineS4))
  #define CUSTOM_MACHINE_NAME "400 SuPeR"
#elif(ENABLED(MachineS5))
  #define CUSTOM_MACHINE_NAME "500 SuPeR"
#elif ENABLED(MachineCR2020)
  #define CUSTOM_MACHINE_NAME "TM3D 2020"
#elif ENABLED(MachineSermoonD1)
  #define CUSTOM_MACHINE_NAME "SermoonD1"
#endif

/**
 * The SOURCE_CODE_URL is the location where users will find the Marlin Source
 * Code which is installed on the device. In most cases —unless the manufacturer
 * has a distinct Github fork— the Source Code URL should just be the main
 * Marlin repository.
 */
#define SOURCE_CODE_URL "https://github.com/InsanityAutomation/Marlin/tree/CrealityDwin_2.0"

/**
 * Default generic printer UUID.
 */
//#define DEFAULT_MACHINE_UUID "cede2a2f-41a2-4748-9b12-c55c62f367ff"

/**
 * The WEBSITE_URL is the location where users can get more information such as
 * documentation about a specific Marlin release.
 */
#define WEBSITE_URL "tinymachines3d.com"

/**
 * Set the vendor info the serial USB interface, if changable
 * Currently only supported by DUE platform
 */
//#define  USB_DEVICE_VENDOR_ID           0x0000
//#define  USB_DEVICE_PRODUCT_ID          0x0000
//#define  USB_DEVICE_MANUFACTURE_NAME    WEBSITE_URL