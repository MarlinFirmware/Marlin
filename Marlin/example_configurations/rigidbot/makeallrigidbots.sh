#!/bin/bash

RBCE_PREFIX="RBCE_v0.4"

MAKEFILE="Makefile"
CONFIGURATION_H="Configuration.h"
CONFIGURATION_ADV_H="Configuration_adv.h"

# Move to working directory (lazy approach compared to allowing defined source and build directories)
cd ../..

# Preserve originals
cp -p $MAKEFILE ${MAKEFILE}.premangle
cp -p $CONFIGURATION_H ${CONFIGURATION_H}.premangle
cp -p $CONFIGURATION_ADV_H ${CONFIGURATION_ADV_H}.premangle

# Prep makefile for all RigidBot builds
sed -i '' -e 's/^HARDWARE_MOTHERBOARD .*/HARDWARE_MOTHERBOARD \?= 42/' $MAKEFILE
sed -i '' -e 's/^AVR_TOOLS_PATH .*/AVR_TOOLS_PATH \?= \/Applications\/Arduino.app\/Contents\/Resources\/Java\/hardware\/tools\/avr\/bin\//' $MAKEFILE
sed -i '' -e 's/^LIQUID_TWI2 .*/LIQUID_TWI2 \?= 0/' $MAKEFILE
sed -i '' -e 's/^WIRE .*/WIRE \?= 0/' $MAKEFILE

BUILD_DIR=`grep "^BUILD_DIR" $MAKEFILE | cut -d\= -f2`
echo "Build dir is $BUILD_DIR/"

# Prep Configuration.h for all RigidBot builds
sed -i '' -e 's/^#define BAUDRATE.*/#define BAUDRATE 115200/' $CONFIGURATION_H
### If HARDWARE_MOTHERBOARD is set in Makefile, this isn't really needed	
sed -i '' -e 's/^#define MOTHERBOARD.*/#define MOTHERBOARD 42/' $CONFIGURATION_H
### Thermistor types
sed -i '' -e 's/^#define TEMP_SENSOR_0.*/#define TEMP_SENSOR_0 1/' $CONFIGURATION_H
sed -i '' -e 's/^#define TEMP_SENSOR_1.*/#define TEMP_SENSOR_1 1/' $CONFIGURATION_H
sed -i '' -e 's/^#define TEMP_SENSOR_2.*/#define TEMP_SENSOR_2 0/' $CONFIGURATION_H
sed -i '' -e 's/^#define TEMP_SENSOR_BED.*/#define TEMP_SENSOR_BED 1/' $CONFIGURATION_H
### Community sourced PID constants for IAP redesigned hotend
sed -i '' -e 's/^[ ]*#define[ ]*DEFAULT_Kp.*/#define DEFAULT_Kp 14.36/' $CONFIGURATION_H
sed -i '' -e 's/^[ ]*#define[ ]*DEFAULT_Ki.*/#define DEFAULT_Ki 0.66/' $CONFIGURATION_H
sed -i '' -e 's/^[ ]*#define[ ]*DEFAULT_Kd.*/#define DEFAULT_Kd 79.25/' $CONFIGURATION_H
### IAP default heated bed PID constants
sed -i '' -e 's/^[ ]*#define[ ]*DEFAULT_bedKp.*/#define DEFAULT_bedKp 355/' $CONFIGURATION_H
sed -i '' -e 's/^[ ]*#define[ ]*DEFAULT_bedKi.*/#define DEFAULT_bedKi 66.5/' $CONFIGURATION_H
sed -i '' -e 's/^[ ]*#define[ ]*DEFAULT_bedKd.*/#define DEFAULT_bedKd 480/' $CONFIGURATION_H
### Enable thermal runaway protection
sed -i '' -e 's/^#define THERMAL_RUNAWAY_PROTECTION_PERIOD.*/#define THERMAL_RUNAWAY_PROTECTION_PERIOD 40/' $CONFIGURATION_H
sed -i '' -e 's/^#define THERMAL_RUNAWAY_PROTECTION_HYSTERESIS.*/#define THERMAL_RUNAWAY_PROTECTION_HYSTERESIS 4/' $CONFIGURATION_H
sed -i '' -e 's/^#define THERMAL_RUNAWAY_PROTECTION_BED_PERIOD.*/#define THERMAL_RUNAWAY_PROTECTION_BED_PERIOD 20/' $CONFIGURATION_H
sed -i '' -e 's/^#define THERMAL_RUNAWAY_PROTECTION_BED_HYSTERESIS.*/#define THERMAL_RUNAWAY_PROTECTION_BED_HYSTERESIS 2/' $CONFIGURATION_H
### Use "predefined mechanics" bundle for the RigidBot: endstops, build volume, feedrate, acceleration, jerk
sed -i '' -e 's/^\/\/#include \"mechanics/rigidbot.h\"/#include \"mechanics/rigidbot.h\"/' $CONFIGURATION_H
### Enable IAP's "Stepper Reset Fix"
sed -i '' -e 's/^\/\/#define STEPPER_RESET_FIX.*/#define STEPPER_RESET_FIX/' $CONFIGURATION_H
### Enable EEPROM support, chitchat, but not overwrite
sed -i '' -e 's/^\/\/#define EEPROM_SETTINGS.*/#define EEPROM_SETTINGS/' $CONFIGURATION_H
sed -i '' -e 's/^\/\/#define EEPROM_CHITCHAT.*/#define EEPROM_CHITCHAT/' $CONFIGURATION_H
sed -i '' -e 's/^.*#define EEPROM_OVERWRITE.*/\/\/#define EEPROM_OVERWRITE/' $CONFIGURATION_H
### Preheat settings for the LCD
sed -i '' -e 's/^#define PLA_PREHEAT_HOTEND_TEMP.*/#define PLA_PREHEAT_HOTEND_TEMP 200/' $CONFIGURATION_H
sed -i '' -e 's/^#define ABS_PREHEAT_HOTEND_TEMP.*/#define ABS_PREHEAT_HOTEND_TEMP 230/' $CONFIGURATION_H


# Prep Configuration_adv.h for all RigidBot builds
sed -i '' -e 's/^#define ENDSTOPS_ONLY_FOR_HOMING.*/\/\/#define ENDSTOPS_ONLY_FOR_HOMING/' $CONFIGURATION_ADV_H
sed -i '' -e 's/^#define BUFSIZE.*/#define BUFSIZE 8/' $CONFIGURATION_ADV_H


##################
## RigidPanel LCD
##################
sed -i '' -e 's/^U8GLIB .*/U8GLIB \?= 0/' $MAKEFILE
sed -i '' -e 's/^RIGIDPANEL .*/RIGIDPANEL \?= 1/' $MAKEFILE
sed -i '' -e 's/^#define REPRAP_DISCOUNT_SMART_CONTROLLER/\/\/#define REPRAP_DISCOUNT_SMART_CONTROLLER/' $CONFIGURATION_H
sed -i '' -e 's/^\/\/#define RIGIDPANEL/#define RIGIDPANEL/' $CONFIGURATION_H
   ###############
   ## Regular Single
   ###############
   sed -i '' -e 's/^[\/]*#define RIGIDBOT_SIZE.*/#define RIGIDBOT_SIZE 1/' $CONFIGURATION_H
   sed -i '' -e 's/^#define EXTRUDERS.*/#define EXTRUDERS 1/' $CONFIGURATION_H
   make clean
   make
   mv -f ${BUILD_DIR}/Marlin.hex ${RBCE_PREFIX}_Regular_Single_RigidPanel.hex
   ###############
   ## Regular Dual
   ###############
   sed -i '' -e 's/^[\/]*#define RIGIDBOT_SIZE.*/#define RIGIDBOT_SIZE 1/' $CONFIGURATION_H
   sed -i '' -e 's/^#define EXTRUDERS.*/#define EXTRUDERS 2/' $CONFIGURATION_H
   make clean
   make
   mv -f ${BUILD_DIR}/Marlin.hex ${RBCE_PREFIX}_Regular_Dual_RigidPanel.hex
   ###############
   ## Big Single
   ###############
   sed -i '' -e 's/^[\/]*#define RIGIDBOT_SIZE.*/#define RIGIDBOT_SIZE 2/' $CONFIGURATION_H
   sed -i '' -e 's/^#define EXTRUDERS.*/#define EXTRUDERS 1/' $CONFIGURATION_H
   make clean
   make
   mv -f ${BUILD_DIR}/Marlin.hex ${RBCE_PREFIX}_Big_Single_RigidPanel.hex
   ###############
   ## Big Dual
   ###############
   sed -i '' -e 's/^[\/]*#define RIGIDBOT_SIZE.*/#define RIGIDBOT_SIZE 2/' $CONFIGURATION_H
   sed -i '' -e 's/^#define EXTRUDERS.*/#define EXTRUDERS 2/' $CONFIGURATION_H
   make clean
   make
   mv -f ${BUILD_DIR}/Marlin.hex ${RBCE_PREFIX}_Big_Dual_RigidPanel.hex


##################
## RepRap Discount Full Graphic LCD
##################
sed -i '' -e 's/^U8GLIB .*/U8GLIB \?= 1/' $MAKEFILE
sed -i '' -e 's/^RIGIDPANEL .*/RIGIDPANEL \?= 0/' $MAKEFILE
sed -i '' -e 's/^\/\/#define REPRAP_DISCOUNT_SMART_CONTROLLER/#define REPRAP_DISCOUNT_SMART_CONTROLLER/' $CONFIGURATION_H
sed -i '' -e 's/^#define RIGIDPANEL/\/\/#define RIGIDPANEL/' $CONFIGURATION_H
   ###############
   ## Regular Single
   ###############
   sed -i '' -e 's/^[\/]*#define RIGIDBOT_SIZE.*/#define RIGIDBOT_SIZE 1/' $CONFIGURATION_H
   sed -i '' -e 's/^#define EXTRUDERS.*/#define EXTRUDERS 1/' $CONFIGURATION_H
   make clean
   make
   mv -f ${BUILD_DIR}/Marlin.hex ${RBCE_PREFIX}_Regular_Single_FullGfx.hex
   ###############
   ## Regular Dual
   ###############
   sed -i '' -e 's/^[\/]*#define RIGIDBOT_SIZE.*/#define RIGIDBOT_SIZE 1/' $CONFIGURATION_H
   sed -i '' -e 's/^#define EXTRUDERS.*/#define EXTRUDERS 2/' $CONFIGURATION_H
   make clean
   make
   mv -f ${BUILD_DIR}/Marlin.hex ${RBCE_PREFIX}_Regular_Dual_FullGfx.hex
   ###############
   ## Big Single
   ###############
   sed -i '' -e 's/^[\/]*#define RIGIDBOT_SIZE.*/#define RIGIDBOT_SIZE 2/' $CONFIGURATION_H
   sed -i '' -e 's/^#define EXTRUDERS.*/#define EXTRUDERS 1/' $CONFIGURATION_H
   make clean
   make
   mv -f ${BUILD_DIR}/Marlin.hex ${RBCE_PREFIX}_Big_Single_FullGfx.hex
   ###############
   ## Big Dual
   ###############
   sed -i '' -e 's/^[\/]*#define RIGIDBOT_SIZE.*/#define RIGIDBOT_SIZE 2/' $CONFIGURATION_H
   sed -i '' -e 's/^#define EXTRUDERS.*/#define EXTRUDERS 2/' $CONFIGURATION_H
   make clean
   make
   mv -f ${BUILD_DIR}/Marlin.hex ${RBCE_PREFIX}_Big_Dual_FullGfx.hex


# Clean up
mv ${MAKEFILE}.premangle $MAKEFILE
mv ${CONFIGURATION_H}.premangle $CONFIGURATION_H
mv ${CONFIGURATION_ADV_H}.premangle $CONFIGURATION_ADV_H

