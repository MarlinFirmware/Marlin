// Extracted from Prusa-Error-Codes repo
// Subject to automation and optimization
// BEWARE - this file shall be included only into mmu2_error_converter.cpp, not anywhere else!
#pragma once
#include "inttypes.h"
#include "src/core/language.h"
#include "src/lcd/marlinui.h"
#include <avr/pgmspace.h>
#include "buttons.h"
#include "../strlen_cx.h"
#include "../mmu2_supported_version.h"
#include "../messages.h"
#include "../ultralcd.h"

// // TODO: This needs to be replaced with whatever we are using to translate text
// #define PSTR(s) (s)

namespace MMU2 {

static constexpr uint8_t ERR_MMU_CODE = 4;

typedef enum : uint16_t {
    ERR_UNDEF = 0,

    ERR_MECHANICAL = 100,
    ERR_MECHANICAL_FINDA_DIDNT_TRIGGER = 101,
    ERR_MECHANICAL_FINDA_FILAMENT_STUCK = 102,
    ERR_MECHANICAL_FSENSOR_DIDNT_TRIGGER = 103,
    ERR_MECHANICAL_FSENSOR_FILAMENT_STUCK = 104,

    ERR_MECHANICAL_PULLEY_CANNOT_MOVE = 105,
    ERR_MECHANICAL_FSENSOR_TOO_EARLY = 106,
    ERR_MECHANICAL_INSPECT_FINDA = 107,
    ERR_MECHANICAL_LOAD_TO_EXTRUDER_FAILED = 108,
    ERR_MECHANICAL_SELECTOR_CANNOT_HOME = 115,
    ERR_MECHANICAL_SELECTOR_CANNOT_MOVE = 116,
    ERR_MECHANICAL_IDLER_CANNOT_HOME = 125,
    ERR_MECHANICAL_IDLER_CANNOT_MOVE = 126,

    ERR_TEMPERATURE = 200,
    ERR_TEMPERATURE_WARNING_TMC_PULLEY_TOO_HOT = 201,
    ERR_TEMPERATURE_WARNING_TMC_SELECTOR_TOO_HOT = 211,
    ERR_TEMPERATURE_WARNING_TMC_IDLER_TOO_HOT = 221,

    ERR_TEMPERATURE_TMC_PULLEY_OVERHEAT_ERROR = 202,
    ERR_TEMPERATURE_TMC_SELECTOR_OVERHEAT_ERROR = 212,
    ERR_TEMPERATURE_TMC_IDLER_OVERHEAT_ERROR = 222,


    ERR_ELECTRICAL = 300,
    ERR_ELECTRICAL_TMC_PULLEY_DRIVER_ERROR = 301,
    ERR_ELECTRICAL_TMC_SELECTOR_DRIVER_ERROR = 311,
    ERR_ELECTRICAL_TMC_IDLER_DRIVER_ERROR = 321,

    ERR_ELECTRICAL_TMC_PULLEY_DRIVER_RESET = 302,
    ERR_ELECTRICAL_TMC_SELECTOR_DRIVER_RESET = 312,
    ERR_ELECTRICAL_TMC_IDLER_DRIVER_RESET = 322,

    ERR_ELECTRICAL_TMC_PULLEY_UNDERVOLTAGE_ERROR = 303,
    ERR_ELECTRICAL_TMC_SELECTOR_UNDERVOLTAGE_ERROR = 313,
    ERR_ELECTRICAL_TMC_IDLER_UNDERVOLTAGE_ERROR = 323,

    ERR_ELECTRICAL_TMC_PULLEY_DRIVER_SHORTED = 304,
    ERR_ELECTRICAL_TMC_SELECTOR_DRIVER_SHORTED = 314,
    ERR_ELECTRICAL_TMC_IDLER_DRIVER_SHORTED = 324,

    ERR_ELECTRICAL_MMU_PULLEY_SELFTEST_FAILED = 305,
    ERR_ELECTRICAL_MMU_SELECTOR_SELFTEST_FAILED = 315,
    ERR_ELECTRICAL_MMU_IDLER_SELFTEST_FAILED = 325,

    ERR_ELECTRICAL_MMU_MCU_ERROR = 306,

    ERR_CONNECT = 400,
    ERR_CONNECT_MMU_NOT_RESPONDING = 401,
    ERR_CONNECT_COMMUNICATION_ERROR = 402,


    ERR_SYSTEM = 500,
    ERR_SYSTEM_FILAMENT_ALREADY_LOADED = 501,
    ERR_SYSTEM_INVALID_TOOL = 502,
    ERR_SYSTEM_QUEUE_FULL = 503,
    ERR_SYSTEM_FW_UPDATE_NEEDED = 504,
    ERR_SYSTEM_FW_RUNTIME_ERROR = 505,
    ERR_SYSTEM_UNLOAD_MANUALLY = 506,
    ERR_SYSTEM_FILAMENT_EJECTED = 507,
    ERR_SYSTEM_FILAMENT_CHANGE = 508,

    ERR_OTHER_UNKNOWN_ERROR = 900
} err_num_t;

// Avr gcc has serious trouble understanding static data structures in PROGMEM
// and inadvertedly falls back to copying the whole structure into RAM (which is obviously unwanted).
// But since this file ought to be generated in the future from yaml prescription,
// it really makes no difference if there are "nice" data structures or plain arrays.
static const constexpr uint16_t errorCodes[] PROGMEM = {
    ERR_MECHANICAL_FINDA_DIDNT_TRIGGER,
    ERR_MECHANICAL_FINDA_FILAMENT_STUCK,
    ERR_MECHANICAL_FSENSOR_DIDNT_TRIGGER,
    ERR_MECHANICAL_FSENSOR_FILAMENT_STUCK,
    ERR_MECHANICAL_PULLEY_CANNOT_MOVE,
    ERR_MECHANICAL_FSENSOR_TOO_EARLY,
    ERR_MECHANICAL_INSPECT_FINDA,
    ERR_MECHANICAL_LOAD_TO_EXTRUDER_FAILED,
    ERR_MECHANICAL_SELECTOR_CANNOT_HOME,
    ERR_MECHANICAL_SELECTOR_CANNOT_MOVE,
    ERR_MECHANICAL_IDLER_CANNOT_HOME,
    ERR_MECHANICAL_IDLER_CANNOT_MOVE,
    ERR_TEMPERATURE_WARNING_TMC_PULLEY_TOO_HOT,
    ERR_TEMPERATURE_WARNING_TMC_SELECTOR_TOO_HOT,
    ERR_TEMPERATURE_WARNING_TMC_IDLER_TOO_HOT,
    ERR_TEMPERATURE_TMC_PULLEY_OVERHEAT_ERROR,
    ERR_TEMPERATURE_TMC_SELECTOR_OVERHEAT_ERROR,
    ERR_TEMPERATURE_TMC_IDLER_OVERHEAT_ERROR,
    ERR_ELECTRICAL_TMC_PULLEY_DRIVER_ERROR,
    ERR_ELECTRICAL_TMC_SELECTOR_DRIVER_ERROR,
    ERR_ELECTRICAL_TMC_IDLER_DRIVER_ERROR,
    ERR_ELECTRICAL_TMC_PULLEY_DRIVER_RESET,
    ERR_ELECTRICAL_TMC_SELECTOR_DRIVER_RESET,
    ERR_ELECTRICAL_TMC_IDLER_DRIVER_RESET,
    ERR_ELECTRICAL_TMC_PULLEY_UNDERVOLTAGE_ERROR,
    ERR_ELECTRICAL_TMC_SELECTOR_UNDERVOLTAGE_ERROR,
    ERR_ELECTRICAL_TMC_IDLER_UNDERVOLTAGE_ERROR,
    ERR_ELECTRICAL_TMC_PULLEY_DRIVER_SHORTED,
    ERR_ELECTRICAL_TMC_SELECTOR_DRIVER_SHORTED,
    ERR_ELECTRICAL_TMC_IDLER_DRIVER_SHORTED,
    ERR_ELECTRICAL_MMU_PULLEY_SELFTEST_FAILED,
    ERR_ELECTRICAL_MMU_SELECTOR_SELFTEST_FAILED,
    ERR_ELECTRICAL_MMU_IDLER_SELFTEST_FAILED,
    ERR_ELECTRICAL_MMU_MCU_ERROR,
    ERR_CONNECT_MMU_NOT_RESPONDING,
    ERR_CONNECT_COMMUNICATION_ERROR,
    ERR_SYSTEM_FILAMENT_ALREADY_LOADED, 
    ERR_SYSTEM_INVALID_TOOL, 
    ERR_SYSTEM_QUEUE_FULL, 
    ERR_SYSTEM_FW_UPDATE_NEEDED, 
    ERR_SYSTEM_FW_RUNTIME_ERROR,
    ERR_SYSTEM_UNLOAD_MANUALLY,
    ERR_SYSTEM_FILAMENT_EJECTED,
    ERR_SYSTEM_FILAMENT_CHANGE,
    ERR_OTHER_UNKNOWN_ERROR
};

// @@TODO some of the strings are duplicates, can be merged into one     01234567890123456789
static const char MSG_TITLE_FINDA_DIDNT_TRIGGER[] PROGMEM     = PSTR("FINDA DIDNT TRIGGER"); ////MSG_TITLE_FINDA_DIDNT_TRIGGER c=20
static const char MSG_TITLE_FINDA_FILAMENT_STUCK[] PROGMEM    = PSTR("FINDA FILAM. STUCK"); ////MSG_TITLE_FINDA_FILAMENT_STUCK c=20
static const char MSG_TITLE_FSENSOR_DIDNT_TRIGGER[] PROGMEM   = PSTR("FSENSOR DIDNT TRIGG."); ////MSG_TITLE_FSENSOR_DIDNT_TRIGGER c=20
static const char MSG_TITLE_FSENSOR_FILAMENT_STUCK[] PROGMEM  = PSTR("FSENSOR FIL. STUCK"); ////MSG_TITLE_FSENSOR_FILAMENT_STUCK c=20
static const char MSG_TITLE_PULLEY_CANNOT_MOVE[] PROGMEM      = PSTR("PULLEY CANNOT MOVE"); ////MSG_TITLE_PULLEY_CANNOT_MOVE c=20
static const char MSG_TITLE_FSENSOR_TOO_EARLY[] PROGMEM       = PSTR("FSENSOR TOO EARLY"); ////MSG_TITLE_FSENSOR_TOO_EARLY c=20
static const char MSG_TITLE_INSPECT_FINDA[] PROGMEM           = PSTR("INSPECT FINDA"); ////MSG_TITLE_INSPECT_FINDA c=20
static const char MSG_TITLE_LOAD_TO_EXTRUDER_FAILED[] PROGMEM = PSTR("LOAD TO EXTR. FAILED"); ////MSG_TITLE_LOAD_TO_EXTRUDER_FAILED c=20
static const char MSG_TITLE_SELECTOR_CANNOT_MOVE[] PROGMEM    = PSTR("SELECTOR CANNOT MOVE"); ////MSG_TITLE_SELECTOR_CANNOT_MOVE c=20
static const char MSG_TITLE_SELECTOR_CANNOT_HOME[] PROGMEM    = PSTR("SELECTOR CANNOT HOME"); ////MSG_TITLE_SELECTOR_CANNOT_HOME c=20
static const char MSG_TITLE_IDLER_CANNOT_MOVE[] PROGMEM       = PSTR("IDLER CANNOT MOVE"); ////MSG_TITLE_IDLER_CANNOT_MOVE c=20
static const char MSG_TITLE_IDLER_CANNOT_HOME[] PROGMEM       = PSTR("IDLER CANNOT HOME"); ////MSG_TITLE_IDLER_CANNOT_HOME c=20
static const char MSG_TITLE_TMC_WARNING_TMC_TOO_HOT[] PROGMEM = PSTR("WARNING TMC TOO HOT"); ////MSG_TITLE_TMC_WARNING_TMC_TOO_HOT c=20
//static const char MSG_TITLE_WARNING_TMC_PULLEY_TOO_HOT[] PROGMEM = PSTR("WARNING TMC TOO HOT");
//static const char MSG_TITLE_WARNING_TMC_SELECTOR_TOO_HOT[] PROGMEM = PSTR("WARNING TMC TOO HOT");
//static const char MSG_TITLE_WARNING_TMC_IDLER_TOO_HOT[] PROGMEM = PSTR("WARNING TMC TOO HOT");
static const char MSG_TITLE_TMC_OVERHEAT_ERROR[] PROGMEM      = PSTR("TMC OVERHEAT ERROR"); ////MSG_TITLE_TMC_OVERHEAT_ERROR c=20
//static const char MSG_TITLE_TMC_PULLEY_OVERHEAT_ERROR[] PROGMEM = PSTR("TMC OVERHEAT ERROR");
//static const char MSG_TITLE_TMC_SELECTOR_OVERHEAT_ERROR[] PROGMEM = PSTR("TMC OVERHEAT ERROR");
//static const char MSG_TITLE_TMC_IDLER_OVERHEAT_ERROR[] PROGMEM = PSTR("TMC OVERHEAT ERROR");
static const char MSG_TITLE_TMC_DRIVER_ERROR[] PROGMEM        = PSTR("TMC DRIVER ERROR"); ////MSG_TITLE_TMC_DRIVER_ERROR c=20
//static const char MSG_TITLE_TMC_PULLEY_DRIVER_ERROR[] PROGMEM = PSTR("TMC DRIVER ERROR");
//static const char MSG_TITLE_TMC_SELECTOR_DRIVER_ERROR[] PROGMEM = PSTR("TMC DRIVER ERROR");
//static const char MSG_TITLE_TMC_IDLER_DRIVER_ERROR[] PROGMEM = PSTR("TMC DRIVER ERROR");
static const char MSG_TITLE_TMC_DRIVER_RESET[] PROGMEM        = PSTR("TMC DRIVER RESET"); ////MSG_TITLE_TMC_DRIVER_RESET c=20
//static const char MSG_TITLE_TMC_PULLEY_DRIVER_RESET[] PROGMEM = PSTR("TMC DRIVER RESET");
//static const char MSG_TITLE_TMC_SELECTOR_DRIVER_RESET[] PROGMEM = PSTR("TMC DRIVER RESET");
//static const char MSG_TITLE_TMC_IDLER_DRIVER_RESET[] PROGMEM = PSTR("TMC DRIVER RESET");
static const char MSG_TITLE_TMC_UNDERVOLTAGE_ERROR[] PROGMEM  = PSTR("TMC UNDERVOLTAGE ERR"); ////MSG_TITLE_TMC_UNDERVOLTAGE_ERROR c=20
//static const char MSG_TITLE_TMC_PULLEY_UNDERVOLTAGE_ERROR[] PROGMEM = PSTR("TMC UNDERVOLTAGE ERR");
//static const char MSG_TITLE_TMC_SELECTOR_UNDERVOLTAGE_ERROR[] PROGMEM = PSTR("TMC UNDERVOLTAGE ERR");
//static const char MSG_TITLE_TMC_IDLER_UNDERVOLTAGE_ERROR[] PROGMEM = PSTR("TMC UNDERVOLTAGE ERR");
static const char MSG_TITLE_TMC_DRIVER_SHORTED[] PROGMEM      = PSTR("TMC DRIVER SHORTED"); ////MSG_TITLE_TMC_DRIVER_SHORTED c=20
//static const char MSG_TITLE_TMC_PULLEY_DRIVER_SHORTED[] PROGMEM = PSTR("TMC DRIVER SHORTED");
//static const char MSG_TITLE_TMC_SELECTOR_DRIVER_SHORTED[] PROGMEM = PSTR("TMC DRIVER SHORTED");
//static const char MSG_TITLE_TMC_IDLER_DRIVER_SHORTED[] PROGMEM = PSTR("TMC DRIVER SHORTED");
static const char MSG_TITLE_SELFTEST_FAILED[] PROGMEM      = PSTR("MMU SELFTEST FAILED"); ////MSG_TITLE_SELFTEST_FAILED c=20
//static const char MSG_TITLE_MMU_PULLEY_SELFTEST_FAILED[] PROGMEM      = PSTR("MMU SELFTEST FAILED");
//static const char MSG_TITLE_MMU_SELECTOR_SELFTEST_FAILED[] PROGMEM      = PSTR("MMU SELFTEST FAILED");
//static const char MSG_TITLE_MMU_IDLER_SELFTEST_FAILED[] PROGMEM      = PSTR("MMU SELFTEST FAILED");
static const char MSG_TITLE_MMU_MCU_ERROR[] PROGMEM           = PSTR("MMU MCU ERROR"); ////MSG_TITLE_MMU_MCU_ERROR c=20
static const char MSG_TITLE_MMU_NOT_RESPONDING[] PROGMEM    = PSTR("MMU NOT RESPONDING"); ////MSG_TITLE_MMU_NOT_RESPONDING c=20
static const char MSG_TITLE_COMMUNICATION_ERROR[] PROGMEM     = PSTR("COMMUNICATION ERROR"); ////MSG_TITLE_COMMUNICATION_ERROR c=20
static const char MSG_TITLE_FILAMENT_ALREADY_LOADED[] PROGMEM = PSTR("FIL. ALREADY LOADED"); ////MSG_TITLE_FILAMENT_ALREADY_LOADED c=20
static const char MSG_TITLE_INVALID_TOOL[] PROGMEM            = PSTR("INVALID TOOL"); ////MSG_TITLE_INVALID_TOOL c=20
static const char MSG_TITLE_QUEUE_FULL[] PROGMEM              = PSTR("QUEUE FULL"); ////MSG_TITLE_QUEUE_FULL c=20
static const char MSG_TITLE_FW_UPDATE_NEEDED[] PROGMEM        = PSTR("MMU FW UPDATE NEEDED"); ////MSG_TITLE_FW_UPDATE_NEEDED c=20
static const char MSG_TITLE_FW_RUNTIME_ERROR[] PROGMEM      = PSTR("FW RUNTIME ERROR"); ////MSG_TITLE_FW_RUNTIME_ERROR c=20
static const char MSG_TITLE_UNLOAD_MANUALLY[] PROGMEM         = PSTR("UNLOAD MANUALLY"); ////MSG_TITLE_UNLOAD_MANUALLY c=20
static const char MSG_TITLE_FILAMENT_EJECTED[] PROGMEM        = PSTR("FILAMENT EJECTED"); ////MSG_TITLE_FILAMENT_EJECTED c=20
static const char MSG_TITLE_FILAMENT_CHANGE[] PROGMEM         = PSTR("FILAMENT CHANGE"); ////MSG_TITLE_FILAMENT_CHANGE c=20
static const char MSG_TITLE_UNKNOWN_ERROR[] PROGMEM           = PSTR("UNKNOWN ERROR"); ////MSG_TITLE_UNKNOWN_ERROR c=20

static const char * const errorTitles [] PROGMEM = {
    PSTR(MSG_TITLE_FINDA_DIDNT_TRIGGER),
    PSTR(MSG_TITLE_FINDA_FILAMENT_STUCK),
    PSTR(MSG_TITLE_FSENSOR_DIDNT_TRIGGER),
    PSTR(MSG_TITLE_FSENSOR_FILAMENT_STUCK),
    PSTR(MSG_TITLE_PULLEY_CANNOT_MOVE),
    PSTR(MSG_TITLE_FSENSOR_TOO_EARLY),
    PSTR(MSG_TITLE_INSPECT_FINDA),
    PSTR(MSG_TITLE_LOAD_TO_EXTRUDER_FAILED),
    PSTR(MSG_TITLE_SELECTOR_CANNOT_HOME),
    PSTR(MSG_TITLE_SELECTOR_CANNOT_MOVE),
    PSTR(MSG_TITLE_IDLER_CANNOT_HOME),
    PSTR(MSG_TITLE_IDLER_CANNOT_MOVE),
    PSTR(MSG_TITLE_TMC_WARNING_TMC_TOO_HOT),
    PSTR(MSG_TITLE_TMC_WARNING_TMC_TOO_HOT),
    PSTR(MSG_TITLE_TMC_WARNING_TMC_TOO_HOT),
    PSTR(MSG_TITLE_TMC_OVERHEAT_ERROR),
    PSTR(MSG_TITLE_TMC_OVERHEAT_ERROR),
    PSTR(MSG_TITLE_TMC_OVERHEAT_ERROR),
    PSTR(MSG_TITLE_TMC_DRIVER_ERROR),
    PSTR(MSG_TITLE_TMC_DRIVER_ERROR),
    PSTR(MSG_TITLE_TMC_DRIVER_ERROR),
    PSTR(MSG_TITLE_TMC_DRIVER_RESET),
    PSTR(MSG_TITLE_TMC_DRIVER_RESET),
    PSTR(MSG_TITLE_TMC_DRIVER_RESET),
    PSTR(MSG_TITLE_TMC_UNDERVOLTAGE_ERROR),
    PSTR(MSG_TITLE_TMC_UNDERVOLTAGE_ERROR),
    PSTR(MSG_TITLE_TMC_UNDERVOLTAGE_ERROR),
    PSTR(MSG_TITLE_TMC_DRIVER_SHORTED),
    PSTR(MSG_TITLE_TMC_DRIVER_SHORTED),
    PSTR(MSG_TITLE_TMC_DRIVER_SHORTED),
    PSTR(MSG_TITLE_SELFTEST_FAILED),
    PSTR(MSG_TITLE_SELFTEST_FAILED),
    PSTR(MSG_TITLE_SELFTEST_FAILED),
    PSTR(MSG_TITLE_MMU_MCU_ERROR),
    PSTR(MSG_TITLE_MMU_NOT_RESPONDING),
    PSTR(MSG_TITLE_COMMUNICATION_ERROR),
    PSTR(MSG_TITLE_FILAMENT_ALREADY_LOADED),
    PSTR(MSG_TITLE_INVALID_TOOL),
    PSTR(MSG_TITLE_QUEUE_FULL),
    PSTR(MSG_TITLE_FW_UPDATE_NEEDED),
    PSTR(MSG_TITLE_FW_RUNTIME_ERROR),
    PSTR(MSG_TITLE_UNLOAD_MANUALLY),
    PSTR(MSG_TITLE_FILAMENT_EJECTED),
    PSTR(MSG_TITLE_FILAMENT_CHANGE),
    PSTR(MSG_TITLE_UNKNOWN_ERROR)
};

// @@TODO looking at the texts, they can be composed of several parts and/or parametrized (could save a lot of space ;) )
// Moreover, some of them have been disabled in favour of saving some more code size.
static const char MSG_DESC_FINDA_DIDNT_TRIGGER[] PROGMEM = PSTR("FINDA didn't trigger while loading the filament. Ensure the filament can move and FINDA works."); ////MSG_DESC_FINDA_DIDNT_TRIGGER c=20 r=8
static const char MSG_DESC_FINDA_FILAMENT_STUCK[] PROGMEM = PSTR("FINDA didn't switch off while unloading filament. Try unloading manually. Ensure filament can move and FINDA works."); ////MSG_DESC_FINDA_FILAMENT_STUCK c=20 r=8
static const char MSG_DESC_FSENSOR_DIDNT_TRIGGER[] PROGMEM = PSTR("Filament sensor didn't trigger while loading the filament. Ensure the sensor is calibrated and the filament reached it."); ////MSG_DESC_FSENSOR_DIDNT_TRIGGER c=20 r=8
static const char MSG_DESC_FSENSOR_FILAMENT_STUCK[] PROGMEM = PSTR("Filament sensor didn't switch off while unloading filament. Ensure filament can move and the sensor works."); ////MSG_DESC_FSENSOR_FILAMENT_STUCK c=20 r=8
static const char MSG_DESC_PULLEY_CANNOT_MOVE[] PROGMEM = PSTR("Pulley motor stalled. Ensure the pulley can move and check the wiring."); ////MSG_DESC_PULLEY_CANNOT_MOVE c=20 r=8
static const char MSG_DESC_FSENSOR_TOO_EARLY[] PROGMEM = PSTR("Filament sensor triggered too early while loading to extruder. Check there isn't anything stuck in PTFE tube. Check that sensor reads properly."); ////MSG_DESC_FSENSOR_TOO_EARLY c=20 r=8
static const char MSG_DESC_INSPECT_FINDA[] PROGMEM = PSTR("Selector can't move due to FINDA detecting a filament. Make sure no filament is in Selector and FINDA works properly."); ////MSG_DESC_INSPECT_FINDA c=20 r=8
static const char MSG_DESC_LOAD_TO_EXTRUDER_FAILED[] PROGMEM = PSTR("Loading to extruder failed. Inspect the filament tip shape. Refine the sensor calibration, if needed."); ////MSG_DESC_LOAD_TO_EXTRUDER_FAILED c=20 r=8
static const char MSG_DESC_SELECTOR_CANNOT_HOME[] PROGMEM = PSTR("The Selector cannot home properly. Check for anything blocking its movement."); ////MSG_DESC_SELECTOR_CANNOT_HOME c=20 r=8
static const char MSG_DESC_CANNOT_MOVE[] PROGMEM = PSTR("Can't move Selector or Idler."); /////MSG_DESC_CANNOT_MOVE c=20 r=4
//static const char MSG_DESC_SELECTOR_CANNOT_MOVE[] PROGMEM = PSTR("The Selector cannot move. Check for anything blocking its movement. Check if the wiring is correct.");
static const char MSG_DESC_IDLER_CANNOT_HOME[] PROGMEM = PSTR("The Idler cannot home properly. Check for anything blocking its movement."); ////MSG_DESC_IDLER_CANNOT_HOME c=20 r=8
//static const char MSG_DESC_IDLER_CANNOT_MOVE[] PROGMEM = PSTR("The Idler cannot move properly. Check for anything blocking its movement. Check if the wiring is correct.");
static const char MSG_DESC_TMC[] PROGMEM = PSTR("More details online."); ////MSG_DESC_TMC c=20 r=8
//static const char MSG_DESC_WARNING_TMC_PULLEY_TOO_HOT[] PROGMEM = PSTR("TMC driver for the Pulley motor is almost overheating. Make sure there is sufficient airflow near the MMU board.");
//static const char MSG_DESC_WARNING_TMC_SELECTOR_TOO_HOT[] PROGMEM = PSTR("TMC driver for the Selector motor is almost overheating. Make sure there is sufficient airflow near the MMU board.");
//static const char MSG_DESC_WARNING_TMC_IDLER_TOO_HOT[] PROGMEM = PSTR("TMC driver for the Idler motor is almost overheating. Make sure there is sufficient airflow near the MMU board.");
//static const char MSG_DESC_TMC_PULLEY_OVERHEAT_ERROR[] PROGMEM = PSTR("TMC driver for the Pulley motor is overheated. Cool down the MMU board and reset MMU.");
//static const char MSG_DESC_TMC_SELECTOR_OVERHEAT_ERROR[] PROGMEM = PSTR("TMC driver for the Selector motor is overheated. Cool down the MMU board and reset MMU.");
//static const char MSG_DESC_TMC_IDLER_OVERHEAT_ERROR[] PROGMEM = PSTR("TMC driver for the Idler motor is overheated. Cool down the MMU board and reset MMU.");
//static const char MSG_DESC_TMC_PULLEY_DRIVER_ERROR[] PROGMEM = PSTR("TMC driver for the Pulley motor is not responding. Try resetting the MMU. If the issue persists contact support.");
//static const char MSG_DESC_TMC_SELECTOR_DRIVER_ERROR[] PROGMEM = PSTR("TMC driver for the Selector motor is not responding. Try resetting the MMU. If the issue persists contact support.");
//static const char MSG_DESC_TMC_IDLER_DRIVER_ERROR[] PROGMEM = PSTR("TMC driver for the Idler motor is not responding. Try resetting the MMU. If the issue persists contact support.");
//static const char MSG_DESC_TMC_PULLEY_DRIVER_RESET[] PROGMEM = PSTR("TMC driver for the Pulley motor was restarted. There is probably an issue with the electronics. Check the wiring and connectors.");
//static const char MSG_DESC_TMC_SELECTOR_DRIVER_RESET[] PROGMEM = PSTR("TMC driver for the Selector motor was restarted. There is probably an issue with the electronics. Check the wiring and connectors.");
//static const char MSG_DESC_TMC_IDLER_DRIVER_RESET[] PROGMEM = PSTR("TMC driver for the Idler motor was restarted. There is probably an issue with the electronics. Check the wiring and connectors.");
//static const char MSG_DESC_TMC_PULLEY_UNDERVOLTAGE_ERROR[] PROGMEM = PSTR("Not enough current for the Pulley TMC driver. There is probably an issue with the electronics. Check the wiring and connectors.");
//static const char MSG_DESC_TMC_SELECTOR_UNDERVOLTAGE_ERROR[] PROGMEM = PSTR("Not enough current for the Selector TMC driver. There is probably an issue with the electronics. Check the wiring and connectors.");
//static const char MSG_DESC_TMC_IDLER_UNDERVOLTAGE_ERROR[] PROGMEM = PSTR("Not enough current for the Idler TMC driver. There is probably an issue with the electronics. Check the wiring and connectors.");
//static const char MSG_DESC_TMC_PULLEY_DRIVER_SHORTED[] PROGMEM = PSTR("Short circuit on the Pulley TMC driver. Check the wiring and connectors. If the issue persists contact support.");
//static const char MSG_DESC_TMC_SELECTOR_DRIVER_SHORTED[] PROGMEM = PSTR("Short circuit on the Selector TMC driver. Check the wiring and connectors. If the issue persists contact support.");
//static const char MSG_DESC_TMC_IDLER_DRIVER_SHORTED[] PROGMEM = PSTR("Short circuit on the Idler TMC driver. Check the wiring and connectors. If the issue persists contact support.");
//static const char MSG_DESC_MMU_PULLEY_SELFTEST_FAILED[] PROGMEM = PSTR("MMU selftest failed on the Pulley TMC driver. Check the wiring and connectors. If the issue persists contact support.");
//static const char MSG_DESC_MMU_SELECTOR_SELFTEST_FAILED[] PROGMEM = PSTR("MMU selftest failed on the Selector TMC driver. Check the wiring and connectors. If the issue persists contact support.");
//static const char MSG_DESC_MMU_IDLER_SELFTEST_FAILED[] PROGMEM = PSTR("MMU selftest failed on the Idler TMC driver. Check the wiring and connectors. If the issue persists contact support.");
//static const char MSG_DESC_MMU_MCU_ERROR[] PROGMEM = PSTR("MMU detected a power-related issue. Check the wiring and connectors. If the issue persists, contact support."); ////MSG_DESC_MMU_MCU_ERROR c=20 r=8
static const char MSG_DESC_MMU_NOT_RESPONDING[] PROGMEM = PSTR("MMU not responding. Check the wiring and connectors."); ////MSG_DESC_MMU_NOT_RESPONDING c=20 r=4
static const char MSG_DESC_COMMUNICATION_ERROR[] PROGMEM = PSTR("MMU not responding correctly. Check the wiring and connectors."); ////MSG_DESC_COMMUNICATION_ERROR c=20 r=4
static const char MSG_DESC_FILAMENT_ALREADY_LOADED[] PROGMEM = PSTR("Cannot perform the action, filament is already loaded. Unload it first."); ////MSG_DESC_FILAMENT_ALREADY_LOADED c=20 r=8
static const char MSG_DESC_INVALID_TOOL[] PROGMEM = PSTR("Requested filament tool is not available on this hardware. Check the G-code for tool index out of range (T0-T4)."); ////MSG_DESC_INVALID_TOOL c=20 r=8
static const char MSG_DESC_QUEUE_FULL[] PROGMEM = PSTR("MMU Firmware internal error, please reset the MMU."); ////MSG_DESC_QUEUE_FULL c=20 r=8
static const char MSG_DESC_FW_RUNTIME_ERROR[] PROGMEM = PSTR("Internal runtime error. Try resetting the MMU or updating the firmware."); ////MSG_DESC_FW_RUNTIME_ERROR c=20 r=8
static const char MSG_DESC_UNLOAD_MANUALLY[] PROGMEM = PSTR("Filament detected unexpectedly. Ensure no filament is loaded. Check the sensors and wiring."); ////MSG_DESC_UNLOAD_MANUALLY c=20 r=8
static const char MSG_DESC_FILAMENT_EJECTED[] PROGMEM = PSTR("Remove the ejected filament from the front of the MMU."); ////MSG_DESC_FILAMENT_EJECTED c=20 r=8
static const char MSG_DESC_FILAMENT_CHANGE[] PROGMEM = PSTR("M600 Filament Change. Load a new filament or eject the old one."); ////MSG_DESC_FILAMENT_CHANGE c=20 r=8
static const char MSG_DESC_UNKNOWN_ERROR[] PROGMEM    = PSTR("Unexpected error occurred."); ////MSG_DESC_UNKNOWN_ERROR c=20 r=8

// Read explanation in mmu2_protocol_logic.cpp -> supportedMmuFWVersion
static constexpr char MSG_DESC_FW_UPDATE_NEEDED[] PROGMEM = PSTR("MMU FW version is incompatible with printer FW.Update to version 3.0.2."); ////MSG_DESC_FW_UPDATE_NEEDED c=20 r=8
static constexpr uint8_t szFWUN = sizeof(MSG_DESC_FW_UPDATE_NEEDED);
// at least check the individual version characters in MSG_DESC_FW_UPDATE_NEEDED
static_assert(MSG_DESC_FW_UPDATE_NEEDED[szFWUN - 7] == ('0' + mmuVersionMajor));
static_assert(MSG_DESC_FW_UPDATE_NEEDED[szFWUN - 5] == ('0' + mmuVersionMinor));
static_assert(MSG_DESC_FW_UPDATE_NEEDED[szFWUN - 3] == ('0' + mmuVersionPatch));

static const char * const errorDescs[] PROGMEM = {
    PSTR(MSG_DESC_FINDA_DIDNT_TRIGGER),
    PSTR(MSG_DESC_FINDA_FILAMENT_STUCK),
    PSTR(MSG_DESC_FSENSOR_DIDNT_TRIGGER),
    PSTR(MSG_DESC_FSENSOR_FILAMENT_STUCK),
    PSTR(MSG_DESC_PULLEY_CANNOT_MOVE),
    PSTR(MSG_DESC_FSENSOR_TOO_EARLY),
    PSTR(MSG_DESC_INSPECT_FINDA),
    PSTR(MSG_DESC_LOAD_TO_EXTRUDER_FAILED),
    PSTR(MSG_DESC_SELECTOR_CANNOT_HOME),
    PSTR(MSG_DESC_CANNOT_MOVE),
    PSTR(MSG_DESC_IDLER_CANNOT_HOME),
    PSTR(MSG_DESC_CANNOT_MOVE),
    PSTR(MSG_DESC_TMC), // descWARNING_TMC_PULLEY_TOO_HOT
    PSTR(MSG_DESC_TMC), // descWARNING_TMC_SELECTOR_TOO_HOT
    PSTR(MSG_DESC_TMC), // descWARNING_TMC_IDLER_TOO_HOT
    PSTR(MSG_DESC_TMC), // descTMC_PULLEY_OVERHEAT_ERROR
    PSTR(MSG_DESC_TMC), // descTMC_SELECTOR_OVERHEAT_ERROR
    PSTR(MSG_DESC_TMC), // descTMC_IDLER_OVERHEAT_ERROR
    PSTR(MSG_DESC_TMC), // descTMC_PULLEY_DRIVER_ERROR
    PSTR(MSG_DESC_TMC), // descTMC_SELECTOR_DRIVER_ERROR
    PSTR(MSG_DESC_TMC), // descTMC_IDLER_DRIVER_ERROR
    PSTR(MSG_DESC_TMC), // descTMC_PULLEY_DRIVER_RESET
    PSTR(MSG_DESC_TMC), // descTMC_SELECTOR_DRIVER_RESET
    PSTR(MSG_DESC_TMC), // descTMC_IDLER_DRIVER_RESET
    PSTR(MSG_DESC_TMC), // descTMC_PULLEY_UNDERVOLTAGE_ERROR
    PSTR(MSG_DESC_TMC), // descTMC_SELECTOR_UNDERVOLTAGE_ERROR
    PSTR(MSG_DESC_TMC), // descTMC_IDLER_UNDERVOLTAGE_ERROR
    PSTR(MSG_DESC_TMC), // descTMC_PULLEY_DRIVER_SHORTED
    PSTR(MSG_DESC_TMC), // descTMC_SELECTOR_DRIVER_SHORTED
    PSTR(MSG_DESC_TMC), // descTMC_IDLER_DRIVER_SHORTED
    PSTR(MSG_DESC_TMC), // descMMU_PULLEY_SELFTEST_FAILED
    PSTR(MSG_DESC_TMC), // descMMU_SELECTOR_SELFTEST_FAILED
    PSTR(MSG_DESC_TMC), // descMMU_IDLER_SELFTEST_FAILED
    PSTR(MSG_DESC_TMC), // descMSG_DESC_MMU_MCU_ERROR
    PSTR(MSG_DESC_MMU_NOT_RESPONDING),
    PSTR(MSG_DESC_COMMUNICATION_ERROR),
    PSTR(MSG_DESC_FILAMENT_ALREADY_LOADED),
    PSTR(MSG_DESC_INVALID_TOOL),
    PSTR(MSG_DESC_QUEUE_FULL),
    PSTR(MSG_DESC_FW_UPDATE_NEEDED),
    PSTR(MSG_DESC_FW_RUNTIME_ERROR),
    PSTR(MSG_DESC_UNLOAD_MANUALLY),
    PSTR(MSG_DESC_FILAMENT_EJECTED),
    PSTR(MSG_DESC_FILAMENT_CHANGE),
    PSTR(MSG_DESC_UNKNOWN_ERROR)
};

// we have max 3 buttons/operations to select from
// one of them is "More" to show the explanation text normally hidden in the next screens.
// It is displayed with W (Double down arrow, special character from CGRAM)
// 01234567890123456789
// >bttxt   >bttxt   >W
// Therefore at least some of the buttons, which can occur on the screen together, can only be 8-chars long max @@TODO.
// Beware - we only have space for 2 buttons on the LCD while the MMU has 3 buttons
// -> the left button on the MMU is not used/rendered on the LCD (it is also almost unused on the MMU side)
static const char MSG_BTN_RETRY[] PROGMEM = PSTR("Retry"); ////MSG_BTN_RETRY c=8
//static const char MSG_BTN_DONE[] PROGMEM = PSTR("Done"); //Reuse MSG_DONE c=8
static const char MSG_BTN_RESET_MMU[] PROGMEM = PSTR("ResetMMU"); ////MSG_BTN_RESET_MMU c=8
static const char MSG_BTN_UNLOAD[] PROGMEM = PSTR("Unload"); ////MSG_BTN_UNLOAD c=8
static const char MSG_BTN_LOAD[] PROGMEM = PSTR("Load"); ////MSG_BTN_LOAD c=8
static const char MSG_BTN_EJECT[] PROGMEM = PSTR("Eject"); ////MSG_BTN_EJECT c=8
//static const char MSG_BTN_TUNE_MMU[] PROGMEM = PSTR("Tune"); //Reuse MSG_TUNE c=8
static const char MSG_BTN_STOP[] PROGMEM = PSTR("Stop"); ////MSG_BTN_STOP c=8
static const char MSG_BTN_DISABLE_MMU[] PROGMEM = PSTR("Disable"); ////MSG_BTN_DISABLE_MMU c=8
static const char MSG_BTN_MORE[] PROGMEM = LCD_STR_ARROW_2_DOWN;

// Used to parse the buttons from Btns().
static const char * const btnOperation[] PROGMEM = {
    PSTR(MSG_BTN_RETRY),
    PSTR(MSG_DONE),
    PSTR(MSG_BTN_RESET_MMU),
    PSTR(MSG_BTN_UNLOAD),
    PSTR(MSG_BTN_LOAD),
    PSTR(MSG_BTN_EJECT),
    PSTR(MSG_TUNE),
    PSTR(MSG_BTN_STOP),
    PSTR(MSG_BTN_DISABLE_MMU),
};

// We have 8 different operations/buttons at this time, so we need at least 4 bits to encode each.
// Since one of the buttons is always "More", we can skip that one.
// Therefore we need just 1 byte to describe the necessary buttons for each screen.
uint8_t constexpr Btns(ButtonOperations bMiddle, ButtonOperations bRight){
    return ((uint8_t)bRight) << 4 | ((uint8_t)bMiddle);
}

static const uint8_t errorButtons[] PROGMEM = {
    Btns(ButtonOperations::Retry, ButtonOperations::NoOperation),//FINDA_DIDNT_TRIGGER
    Btns(ButtonOperations::Retry, ButtonOperations::NoOperation),//FINDA_FILAMENT_STUCK
    Btns(ButtonOperations::Retry, ButtonOperations::NoOperation),//FSENSOR_DIDNT_TRIGGER
    Btns(ButtonOperations::Retry, ButtonOperations::NoOperation),//FSENSOR_FILAMENT_STUCK

    Btns(ButtonOperations::Retry, ButtonOperations::NoOperation),//PULLEY_CANNOT_MOVE
    Btns(ButtonOperations::Retry, ButtonOperations::NoOperation),//FSENSOR_TOO_EARLY
    Btns(ButtonOperations::Retry, ButtonOperations::NoOperation),//INSPECT_FINDA
    Btns(ButtonOperations::Continue, ButtonOperations::NoOperation),//LOAD_TO_EXTRUDER_FAILED
    Btns(ButtonOperations::Retry, ButtonOperations::Tune),//SELECTOR_CANNOT_HOME
    Btns(ButtonOperations::Retry, ButtonOperations::NoOperation),//SELECTOR_CANNOT_MOVE
    Btns(ButtonOperations::Retry, ButtonOperations::Tune),//IDLER_CANNOT_HOME
    Btns(ButtonOperations::Retry, ButtonOperations::NoOperation),//IDLER_CANNOT_MOVE

    Btns(ButtonOperations::Continue, ButtonOperations::ResetMMU),//WARNING_TMC_PULLEY_TOO_HOT
    Btns(ButtonOperations::Continue, ButtonOperations::ResetMMU),//WARNING_TMC_SELECTOR_TOO_HOT
    Btns(ButtonOperations::Continue, ButtonOperations::ResetMMU),//WARNING_TMC_IDLER_TOO_HOT

    Btns(ButtonOperations::ResetMMU, ButtonOperations::NoOperation),//TMC_PULLEY_OVERHEAT_ERROR
    Btns(ButtonOperations::ResetMMU, ButtonOperations::NoOperation),//TMC_SELECTOR_OVERHEAT_ERROR
    Btns(ButtonOperations::ResetMMU, ButtonOperations::NoOperation),//TMC_IDLER_OVERHEAT_ERROR
    Btns(ButtonOperations::ResetMMU, ButtonOperations::NoOperation),//TMC_PULLEY_DRIVER_ERROR
    Btns(ButtonOperations::ResetMMU, ButtonOperations::NoOperation),//TMC_SELECTOR_DRIVER_ERROR
    Btns(ButtonOperations::ResetMMU, ButtonOperations::NoOperation),//TMC_IDLER_DRIVER_ERROR
    Btns(ButtonOperations::ResetMMU, ButtonOperations::NoOperation),//TMC_PULLEY_DRIVER_RESET
    Btns(ButtonOperations::ResetMMU, ButtonOperations::NoOperation),//TMC_SELECTOR_DRIVER_RESET
    Btns(ButtonOperations::ResetMMU, ButtonOperations::NoOperation),//TMC_IDLER_DRIVER_RESET
    Btns(ButtonOperations::ResetMMU, ButtonOperations::NoOperation),//TMC_PULLEY_UNDERVOLTAGE_ERROR
    Btns(ButtonOperations::ResetMMU, ButtonOperations::NoOperation),//TMC_SELECTOR_UNDERVOLTAGE_ERROR
    Btns(ButtonOperations::ResetMMU, ButtonOperations::NoOperation),//TMC_IDLER_UNDERVOLTAGE_ERROR
    Btns(ButtonOperations::ResetMMU, ButtonOperations::NoOperation),//TMC_PULLEY_DRIVER_SHORTED
    Btns(ButtonOperations::ResetMMU, ButtonOperations::NoOperation),//TMC_SELECTOR_DRIVER_SHORTED
    Btns(ButtonOperations::ResetMMU, ButtonOperations::NoOperation),//TMC_IDLER_DRIVER_SHORTED
    Btns(ButtonOperations::ResetMMU, ButtonOperations::NoOperation),//MMU_PULLEY_SELFTEST_FAILED
    Btns(ButtonOperations::ResetMMU, ButtonOperations::NoOperation),//MMU_SELECTOR_SELFTEST_FAILED
    Btns(ButtonOperations::ResetMMU, ButtonOperations::NoOperation),//MMU_IDLER_SELFTEST_FAILED
    Btns(ButtonOperations::ResetMMU, ButtonOperations::NoOperation),//MMU_MCU_ERROR
    Btns(ButtonOperations::ResetMMU, ButtonOperations::DisableMMU),//MMU_NOT_RESPONDING
    Btns(ButtonOperations::ResetMMU, ButtonOperations::DisableMMU),//COMMUNICATION_ERROR

    Btns(ButtonOperations::Unload, ButtonOperations::Continue),//FILAMENT_ALREADY_LOADED
    Btns(ButtonOperations::StopPrint, ButtonOperations::ResetMMU),//INVALID_TOOL
    Btns(ButtonOperations::ResetMMU, ButtonOperations::NoOperation),//QUEUE_FULL
    Btns(ButtonOperations::ResetMMU, ButtonOperations::DisableMMU),//FW_UPDATE_NEEDED
    Btns(ButtonOperations::ResetMMU, ButtonOperations::NoOperation),//FW_RUNTIME_ERROR
    Btns(ButtonOperations::Retry, ButtonOperations::NoOperation),//UNLOAD_MANUALLY
    Btns(ButtonOperations::Continue, ButtonOperations::NoOperation),//FILAMENT_EJECTED
    Btns(ButtonOperations::Eject, ButtonOperations::Load),//FILAMENT_CHANGE
    Btns(ButtonOperations::ResetMMU, ButtonOperations::NoOperation),//UNKOWN_ERROR
};

static_assert( sizeof(errorCodes) / sizeof(errorCodes[0]) == sizeof(errorDescs) / sizeof (errorDescs[0]));
static_assert( sizeof(errorCodes) / sizeof(errorCodes[0]) == sizeof(errorTitles) / sizeof (errorTitles[0]));
static_assert( sizeof(errorCodes) / sizeof(errorCodes[0]) == sizeof(errorButtons) / sizeof (errorButtons[0]));

} // namespace MMU2
