#ifndef LANGUAGE_H
#define LANGUAGE_H

// NOTE: IF YOU CHANGE THIS FILE / MERGE THIS FILE WITH CHANGES
//
//   ==> ALWAYS TRY TO COMPILE MARLIN WITH/WITHOUT "ULTIPANEL" / "ULTRALCD" / "SDSUPPORT" #define IN "Configuration.h" 
//   ==> ALSO TRY ALL AVAILABLE "LANGUAGE_CHOICE" OPTIONS

// Languages
// 1  English
// 2  Polish
// 3  French
// 4  German
// 5  Spanish
// 6  Russian
// 7  Italian
// 8  Portuguese
// 9  Finnish
// 10 Aragonese
// 11 Dutch

#ifndef LANGUAGE_CHOICE
	#define LANGUAGE_CHOICE 1  // Pick your language from the list above
#endif

#define PROTOCOL_VERSION "1.0"

#if defined(WITBOX)
	#define MACHINE_NAME "bq Witbox 2"
	#define FIRMWARE_URL "http://www.bqreaders.com/descargas-witbox.html"
	#define FIRMWARE_VER "v1.4.2"
	#define BUILD_VER ""
#elif defined(CUSTOM_MENDEL_NAME)
	#define MACHINE_NAME CUSTOM_MENDEL_NAME
#else
	// Default firmware set to Mendel
	#define MACHINE_NAME "Mendel"
	#define FIRMWARE_URL "https://github.com/ErikZalm/Marlin/"
#endif

#ifndef MACHINE_UUID
   #define MACHINE_UUID "00000000-0000-0000-0000-000000000000"
#endif


#define STRINGIFY_(n) #n
#define STRINGIFY(n) STRINGIFY_(n)


// Common LCD messages
/* nothing here as of yet */

// Common serial messages
#define MSG_MARLIN "Marlin"

// Serial Console Messages

#define MSG_Enqueing "enqueing \""
#define MSG_POWERUP "PowerUp"
#define MSG_EXTERNAL_RESET " External Reset"
#define MSG_BROWNOUT_RESET " Brown out Reset"
#define MSG_WATCHDOG_RESET " Watchdog Reset"
#define MSG_SOFTWARE_RESET " Software Reset"
#define MSG_AUTHOR " | Author: "
#define MSG_CONFIGURATION_VER " Last Updated: "
#define MSG_FREE_MEMORY " Free Memory: "
#define MSG_PLANNER_BUFFER_BYTES "  PlannerBufferBytes: "
#define MSG_OK "ok"
#define MSG_FILE_SAVED "Done saving file."
#define MSG_ERR_LINE_NO "Line Number is not Last Line Number+1, Last Line: "
#define MSG_ERR_CHECKSUM_MISMATCH "checksum mismatch, Last Line: "
#define MSG_ERR_NO_CHECKSUM "No Checksum with line number, Last Line: "
#define MSG_ERR_NO_LINENUMBER_WITH_CHECKSUM "No Line Number with checksum, Last Line: "
#define MSG_FILE_PRINTED "Done printing file"
#define MSG_BEGIN_FILE_LIST "Begin file list"
#define MSG_END_FILE_LIST "End file list"
#define MSG_M104_INVALID_EXTRUDER "M104 Invalid extruder "
#define MSG_M105_INVALID_EXTRUDER "M105 Invalid extruder "
#define MSG_M200_INVALID_EXTRUDER "M200 Invalid extruder "
#define MSG_M218_INVALID_EXTRUDER "M218 Invalid extruder "
#define MSG_ERR_NO_THERMISTORS "No thermistors - no temperature"
#define MSG_M109_INVALID_EXTRUDER "M109 Invalid extruder "
#define MSG_HEATING "Heating..."
#define MSG_HEATING_COMPLETE "Heating done."
#define MSG_BED_HEATING "Bed Heating."
#define MSG_BED_DONE "Bed done."
#define MSG_M115_REPORT "FIRMWARE_NAME:Marlin V1; Sprinter/grbl mashup for gen6 FIRMWARE_URL:" FIRMWARE_URL " PROTOCOL_VERSION:" PROTOCOL_VERSION " MACHINE_TYPE:" MACHINE_NAME " EXTRUDER_COUNT:" STRINGIFY(EXTRUDERS) " UUID:" MACHINE_UUID "\n"
#define MSG_COUNT_X " Count X: "
#define MSG_ERR_KILLED "Printer halted. kill() called!"
#define MSG_ERR_STOPPED "Printer stopped due to errors. Fix the error and use M999 to restart. (Temperature is reset. Set it after restarting)"
#define MSG_RESEND "Resend: "
#define MSG_UNKNOWN_COMMAND "Unknown command: \""
#define MSG_ACTIVE_EXTRUDER "Active Extruder: "
#define MSG_INVALID_EXTRUDER "Invalid extruder"
#define MSG_X_MIN "x_min: "
#define MSG_X_MAX "x_max: "
#define MSG_Y_MIN "y_min: "
#define MSG_Y_MAX "y_max: "
#define MSG_Z_MIN "z_min: "
#define MSG_Z_MAX "z_max: "
#define MSG_M119_REPORT "Reporting endstop status"
#define MSG_ENDSTOP_HIT "TRIGGERED"
#define MSG_ENDSTOP_OPEN "open"
#define MSG_HOTEND_OFFSET "Hotend offsets:"

#define MSG_SD_CANT_OPEN_SUBDIR "Cannot open subdir"
#define MSG_SD_INIT_FAIL "SD init fail"
#define MSG_SD_VOL_INIT_FAIL "volume.init failed"
#define MSG_SD_OPENROOT_FAIL "openRoot failed"
#define MSG_SD_CARD_OK "SD card ok"
#define MSG_SD_WORKDIR_FAIL "workDir open failed"
#define MSG_SD_OPEN_FILE_FAIL "open failed, File: "
#define MSG_SD_FILE_OPENED "File opened: "
#define MSG_SD_SIZE " Size: "
#define MSG_SD_FILE_SELECTED "File selected"
#define MSG_SD_WRITE_TO_FILE "Writing to file: "
#define MSG_SD_PRINTING_BYTE "SD printing byte "
#define MSG_SD_NOT_PRINTING "Not SD printing"
#define MSG_SD_ERR_WRITE_TO_FILE "error writing to file"
#define MSG_SD_CANT_ENTER_SUBDIR "Cannot enter subdir: "

#define MSG_STEPPER_TOO_HIGH "Steprate too high: "
#define MSG_ENDSTOPS_HIT "endstops hit: "
#define MSG_ERR_COLD_EXTRUDE_STOP " cold extrusion prevented"
#define MSG_ERR_LONG_EXTRUDE_STOP " too long extrusion prevented"
#define MSG_BABYSTEPPING_X "Babystepping X"
#define MSG_BABYSTEPPING_Y "Babystepping Y"
#define MSG_BABYSTEPPING_Z "Babystepping Z"
#define MSG_SERIAL_ERROR_MENU_STRUCTURE "Error in menu structure"

//Marlin messages

#define WELCOME_MSG "Ready"
#define MSG_ON "On"
#define MSG_OFF "Off"
#define MSG_SD_BAD_FILENAME "Bad filename!"
#define MSG_HEATING_PROCESS "Heating..."
#define MSG_HEATING_DONE "Heating done."
#define MSG_BED_HEATING_PROCESS "Bed Heating."
#define MSG_BED_HEATING_DONE "Bed done."
#define MSG_ENDSTOPS_TOUCH "Endstops hit: "
#define MSG_STOPPED "Stopped"
#define MSG_BED "Bed"
#define MSG_DWELL "Sleep..."
#define MSG_END_HOUR "hours"
#define MSG_END_MINUTE "minutes"
#define MSG_ZPROBE_OUT "Z probe out bed"
#define MSG_POSITION_UNKNOWN "Home X/Y before Z"
#define MSG_PRINTING "Printing..."
#define MSG_USERWAIT "Wait for user..."
#define MSG_KILLED "KILLED"
#define MSG_NO_MOVE "No move"
#define MSG_PAUSED "Press to resume"

// LCD Menu Messages
#if LANGUAGE_CHOICE == 1

const char MSG_NOSD[] PROGMEM                        = "Insert SD card";
const char MSG_SD[] PROGMEM                          = "SD card";
const char MSG_FILAMENT_UNLOAD[] PROGMEM             = "Unload filament";
const char MSG_FILAMENT_LOAD[] PROGMEM               = "Load filament";
const char MSG_LEVELING[] PROGMEM                    = "Level plate";
const char MSG_HOMING[] PROGMEM                      = "Auto-home";
const char MSG_SETTINGS[] PROGMEM                    = "Settings";
const char MSG_STEPPERS[] PROGMEM                    = "Disable steppers";
const char MSG_STEPPERS_OFF[] PROGMEM                = "Steppers disabled";
const char MSG_MOVEAXIS[] PROGMEM                    = "Move axis";
const char MSG_TEMPERATURE[] PROGMEM                 = "Change temperature";
const char MSG_LIGHTLED_DISABLE[] PROGMEM            = "Led light off";
const char MSG_LIGHTLED[] PROGMEM                    = "Led light on";
const char MSG_INFO[] PROGMEM                        = "FW info";
const char MSG_PLAY[] PROGMEM                        = "Play";
const char MSG_PAUSE[] PROGMEM                       = "Pause";
const char MSG_STOP[] PROGMEM                        = "Stop";
const char MSG_CHANGE_FILAMENT[] PROGMEM             = "Change filament";
const char MSG_CHANGE_SPEED[] PROGMEM                = "Change speed";
const char MSG_BACK[] PROGMEM                        = "Back";
const char MSG_RETRY[] PROGMEM                       = "Retry";
const char MSG_OK2[] PROGMEM                         = "Confirm";
const char MSG_MOVE_X[] PROGMEM                      = "Move X";
const char MSG_MOVE_Y[] PROGMEM                      = "Move Y";
const char MSG_MOVE_Z[] PROGMEM                      = "Move Z";
const char MSG_MOVE_E[] PROGMEM                      = "Move E";

const char MSG_SCREEN_LOGO[] PROGMEM                 = "BQ logo";
const char MSG_SCREEN_LOGO_TEXT[] PROGMEM            = "Test";
const char MSG_SCREEN_LOGO_BOX[] PROGMEM             = "Test";

const char MSG_SCREEN_SD_LIST[] PROGMEM              = "SD Card";
const char MSG_SCREEN_SD_CONFIRM[] PROGMEM           = "Confirm Print";
const char MSG_SCREEN_SD_BACK[] PROGMEM              = "Back";
const char MSG_SCREEN_SD_LIST_BACK[] PROGMEM         = "Back to main menu";
const char MSG_SCREEN_SD_LIST_PREV[] PROGMEM         = "Previous folder";

const char MSG_SCREEN_UNLOAD_TITLE[] PROGMEM         = "Unload filament";
const char MSG_SCREEN_UNLOAD_TEXT1[] PROGMEM         = "Press the button let it extrude the filament & pull";
const char MSG_SCREEN_UNLOAD_TEXT2[] PROGMEM         = "Start process?";
const char MSG_SCREEN_UNLOAD_ABORT[] PROGMEM         = "Press to abort";
const char MSG_SCREEN_UNLOAD_CONTINUE[] PROGMEM      = "Press to continue";
const char MSG_SCREEN_UNLOAD_PULL[] PROGMEM          = "Extrude and pull";
const char MSG_SCREEN_UNLOAD_CONFIRM[] PROGMEM       = "Finished?";

const char MSG_SCREEN_LOAD_TITLE[] PROGMEM           = "Load filament";
const char MSG_SCREEN_LOAD_TEXT1[] PROGMEM           = "Press the button and press the filament inside";
const char MSG_SCREEN_LOAD_TEXT2[] PROGMEM           = "Start process?";
const char MSG_SCREEN_LOAD_ABORT[] PROGMEM           = "Press to abort";
const char MSG_SCREEN_LOAD_CONTINUE[] PROGMEM        = "Press to continue";
const char MSG_SCREEN_LOAD_PRESS[] PROGMEM           = "Push to continue";
const char MSG_SCREEN_LOAD_CONFIRM[] PROGMEM         = "Finished?";

const char MSG_SCREEN_LEVEL_TITLE[] PROGMEM          = "Level plate";
const char MSG_SCREEN_LEVEL_TEXT[] PROGMEM           = "Start leveling?";
const char MSG_SCREEN_LEVEL_ABORT[] PROGMEM          = "Push to abort";
const char MSG_SCREEN_LEVEL_CONTINUE[] PROGMEM       = "Push to continue";
const char MSG_SCREEN_LEVEL_TEXT0[] PROGMEM          = "Performing homing operation";
const char MSG_SCREEN_LEVEL_TEXT1[] PROGMEM          = "Adjust first point 1mm between the plate and extruder";
const char MSG_SCREEN_LEVEL_TEXT2[] PROGMEM          = "Adjust second point 1mm between the plate and extruder";
const char MSG_SCREEN_LEVEL_TEXT3[] PROGMEM          = "Adjust third point 1mm between the plate and extruder";
const char MSG_SCREEN_LEVEL_TEXT4[] PROGMEM          = "Adjust center point 1mm between the plate and extruder";
const char MSG_SCREEN_LEVEL_BOX0[] PROGMEM           = "Please wait";
const char MSG_SCREEN_LEVEL_BOX1[] PROGMEM           = "Push to continue";
const char MSG_SCREEN_LEVEL_BOX2[] PROGMEM           = "Push to continue";
const char MSG_SCREEN_LEVEL_BOX3[] PROGMEM           = "Push to continue";
const char MSG_SCREEN_LEVEL_BOX4[] PROGMEM           = "Push to continue";
const char MSG_SCREEN_LEVEL_CONFIRM[] PROGMEM        = "Finished?";

const char MSG_SCREEN_AUTOHOME[] PROGMEM             = "Auto-home";
const char MSG_SCREEN_SETTINGS[] PROGMEM             = "Settings";
const char MSG_SCREEN_SETTINGS_TEXT[] PROGMEM        = "Choose an option";
const char MSG_SCREEN_STEPPER[] PROGMEM              = "Steppers on";

const char MSG_SCREEN_MOVE_TITLE[] PROGMEM           = "Move axis";
const char MSG_SCREEN_MOVE_TEXT[] PROGMEM            = "Select axis";
const char MSG_SCREEN_MOVE_X[] PROGMEM               = "X axis / Select mm";
const char MSG_SCREEN_MOVE_Y[] PROGMEM               = "Y axis / Select mm";
const char MSG_SCREEN_MOVE_Z[] PROGMEM               = "Z axis / Select mm";
const char MSG_SCREEN_MOVE_E[] PROGMEM               = "Extruder / Select mm";
const char MSG_SCREEN_MOVE_2MOVE[] PROGMEM           = "Back";
const char MSG_SCREEN_MOVE_01MM[] PROGMEM            = "0.1mm";
const char MSG_SCREEN_MOVE_1MM[] PROGMEM             = "1mm";
const char MSG_SCREEN_MOVE_10MM[] PROGMEM            = "10mm";

const char MSG_SCREEN_BACK2MAIN[] PROGMEM            = "Back";

const char MSG_SCREEN_PRINT_PRINTING[] PROGMEM       = "Printing";
const char MSG_SCREEN_PRINT_PAUSED[] PROGMEM         = "Paused";

const char MSG_SCREEN_TEMP_TITLE[] PROGMEM           = "Change temperature";
const char MSG_SCREEN_LIGHT[] PROGMEM                = "Led light on";
const char MSG_SCREEN_PRINT[] PROGMEM                = "Confirm";
const char MSG_SCREEN_INFO[] PROGMEM                 = "FW info";
const char MSG_SCREEN_INFO_TEXT[] PROGMEM            = "FW info";
const char MSG_SCREEN_INFO_BOX[] PROGMEM             = "FW info"; 

const char MSG_SCREEN_STOP_CONFIRM[] PROGMEM         = "Stop";
const char MSG_SCREEN_STOP_BACK[] PROGMEM            = "Back";
const char MSG_SCREEN_STOP_OK[] PROGMEM              = "Confirm";

const char MSG_SCREEN_CHANGE_TITLE[] PROGMEM         = "Title";
const char MSG_SCREEN_CHANGE_BOX[] PROGMEM           = "Test";
const char MSG_SCREEN_CHANGE_CONFIRM[] PROGMEM       = "Change filament";
const char MSG_SCREEN_CHANGE_START[] PROGMEM         = "Start";
const char MSG_SCREEN_CHANGE_PULLOUT[] PROGMEM       = "Pull out";
const char MSG_SCREEN_CHANGE_INSERT[] PROGMEM        = "Insert & press";
const char MSG_SCREEN_CHANGE_RETRY[] PROGMEM         = "Finished?";

const char MSG_SCREEN_SPEED[] PROGMEM                = "Change speed";
const char MSG_SELECTOR_BOX[] PROGMEM                = "Select a value";
const char MSG_OK1[] PROGMEM                         = "OK";
#endif


#if LANGUAGE_CHOICE == 5

const char MSG_NOSD[] PROGMEM                        = "Inserte tarjeta SD";
const char MSG_SD[] PROGMEM                          = "Tarjeta SD";
const char MSG_FILAMENT_UNLOAD[] PROGMEM             = "Expulsar filamento";
const char MSG_FILAMENT_LOAD[] PROGMEM               = "Insertar filamento";
const char MSG_LEVELING[] PROGMEM                    = "Nivelar base";
const char MSG_HOMING[] PROGMEM                      = "Auto-home";
const char MSG_SETTINGS[] PROGMEM                    = "Configuracion";
const char MSG_STEPPERS[] PROGMEM                    = "Desactivar motores";
const char MSG_STEPPERS_OFF[] PROGMEM                = "Motores desactivados";
const char MSG_MOVEAXIS[] PROGMEM                    = "Mover ejes";
const char MSG_TEMPERATURE[] PROGMEM                 = "Temp 0/200C";
const char MSG_LIGHTLED_DISABLE[] PROGMEM            = "Luces apagadas";
const char MSG_LIGHTLED[] PROGMEM                    = "Luces encendidas";
const char MSG_INFO[] PROGMEM                        = "FW info";
const char MSG_PLAY[] PROGMEM                        = "Imprimir";
const char MSG_PAUSE[] PROGMEM                       = "Pausar";
const char MSG_STOP[] PROGMEM                        = "Parar";
const char MSG_CHANGE_FILAMENT[] PROGMEM             = "Cambiar filamento";
const char MSG_CHANGE_SPEED[] PROGMEM                = "Cambiar velocidad";
const char MSG_BACK[] PROGMEM                        = "Volver";
const char MSG_RETRY[] PROGMEM                       = "Reintentar";
const char MSG_OK2[] PROGMEM                         = "Confirmar";
const char MSG_MOVE_X[] PROGMEM                      = "Mover X";
const char MSG_MOVE_Y[] PROGMEM                      = "Mover Y";
const char MSG_MOVE_Z[] PROGMEM                      = "Mover Z";
const char MSG_MOVE_E[] PROGMEM                      = "Mover E";

const char MSG_SCREEN_LOGO[] PROGMEM                 = "BQ logo";
const char MSG_SCREEN_LOGO_TEXT[] PROGMEM            = "Test";
const char MSG_SCREEN_LOGO_BOX[] PROGMEM             = "Test";

const char MSG_SCREEN_SD_LIST[] PROGMEM              = "Tarjeta SD";
const char MSG_SCREEN_SD_CONFIRM[] PROGMEM           = "Confirmar Impresion";
const char MSG_SCREEN_SD_BACK[] PROGMEM              = "Volver";
const char MSG_SCREEN_SD_LIST_BACK[] PROGMEM         = "Volver al menu";
const char MSG_SCREEN_SD_LIST_PREV[] PROGMEM         = "Carpeta anterior";

const char MSG_SCREEN_UNLOAD_TITLE[] PROGMEM         = "Descargar filamento";
const char MSG_SCREEN_UNLOAD_TEXT1[] PROGMEM         = "Pulse el botón deje extruir el filamento y extraigalo";
const char MSG_SCREEN_UNLOAD_TEXT2[] PROGMEM         = "Iniciar proceso?";
const char MSG_SCREEN_UNLOAD_ABORT[] PROGMEM         = "Pulse para cancelar";
const char MSG_SCREEN_UNLOAD_CONTINUE[] PROGMEM      = "Pulse para continuar";
const char MSG_SCREEN_UNLOAD_PULL[] PROGMEM          = "Extruir y sacar";
const char MSG_SCREEN_UNLOAD_CONFIRM[] PROGMEM       = "Finalizado?";

const char MSG_SCREEN_LOAD_TITLE[] PROGMEM           = "Cargar filamento";
const char MSG_SCREEN_LOAD_TEXT1[] PROGMEM           = "Pulse el botón e introduzca el filamento";
const char MSG_SCREEN_LOAD_TEXT2[] PROGMEM           = "Iniciar proceso?";
const char MSG_SCREEN_LOAD_ABORT[] PROGMEM           = "Pulse para cancelar";
const char MSG_SCREEN_LOAD_CONTINUE[] PROGMEM        = "Pulse para continuar";
const char MSG_SCREEN_LOAD_PRESS[] PROGMEM           = "Pulse para continuar";
const char MSG_SCREEN_LOAD_CONFIRM[] PROGMEM         = "Finalizado?";

const char MSG_SCREEN_LEVEL_TITLE[] PROGMEM          = "Nivelar base";
const char MSG_SCREEN_LEVEL_TEXT[] PROGMEM           = "Comenzar nivelacion?";
const char MSG_SCREEN_LEVEL_ABORT[] PROGMEM          = "Pulse para cancelar";
const char MSG_SCREEN_LEVEL_CONTINUE[] PROGMEM       = "Pulse para continuar";
const char MSG_SCREEN_LEVEL_TEXT0[] PROGMEM          = "Realizando homing";
const char MSG_SCREEN_LEVEL_TEXT1[] PROGMEM          = "Ajuste el primer punto 1mm entre la cama y el extrusor";
const char MSG_SCREEN_LEVEL_TEXT2[] PROGMEM          = "Ajuste el segundo punto 1mm entre la cama y el extrusor";
const char MSG_SCREEN_LEVEL_TEXT3[] PROGMEM          = "Ajuste el tercer punto 1mm entre la cama y el extrusor";
const char MSG_SCREEN_LEVEL_TEXT4[] PROGMEM          = "Ajuste el punto central 1mm entre la cama y el extrusor";
const char MSG_SCREEN_LEVEL_BOX0[] PROGMEM           = "Por favor espere";
const char MSG_SCREEN_LEVEL_BOX1[] PROGMEM           = "Pulse para continuar";
const char MSG_SCREEN_LEVEL_BOX2[] PROGMEM           = "Pulse para continuar";
const char MSG_SCREEN_LEVEL_BOX3[] PROGMEM           = "Pulse para continuar";
const char MSG_SCREEN_LEVEL_BOX4[] PROGMEM           = "Pulse para continuar";
const char MSG_SCREEN_LEVEL_CONFIRM[] PROGMEM        = "Finalizado?";

const char MSG_SCREEN_AUTOHOME[] PROGMEM             = "Auto-home";
const char MSG_SCREEN_SETTINGS[] PROGMEM             = "Configuracion";
const char MSG_SCREEN_SETTINGS_TEXT[] PROGMEM        = "Seleccione opcion";
const char MSG_SCREEN_STEPPER[] PROGMEM              = "Motores activos";

const char MSG_SCREEN_MOVE_TITLE[] PROGMEM           = "Mover ejes";
const char MSG_SCREEN_MOVE_TEXT[] PROGMEM            = "Seleccione eje";
const char MSG_SCREEN_MOVE_X[] PROGMEM               = "Eje X / mm";
const char MSG_SCREEN_MOVE_Y[] PROGMEM               = "Eje Y / mm";
const char MSG_SCREEN_MOVE_Z[] PROGMEM               = "Eje Z / mm";
const char MSG_SCREEN_MOVE_E[] PROGMEM               = "Extrusor / mm";
const char MSG_SCREEN_MOVE_2MOVE[] PROGMEM           = "Volver";
const char MSG_SCREEN_MOVE_01MM[] PROGMEM            = "0.1mm";
const char MSG_SCREEN_MOVE_1MM[] PROGMEM             = "1mm";
const char MSG_SCREEN_MOVE_10MM[] PROGMEM            = "10mm";

const char MSG_SCREEN_BACK2MAIN[] PROGMEM            = "Volver";

const char MSG_SCREEN_PRINT_PRINTING[] PROGMEM       = "Imprimiendo";
const char MSG_SCREEN_PRINT_PAUSED[] PROGMEM         = "En pausa";

const char MSG_SCREEN_TEMP_TITLE[] PROGMEM           = "Cambiar temperatura";
const char MSG_SCREEN_LIGHT[] PROGMEM                = "Luces encendidas";
const char MSG_SCREEN_PRINT[] PROGMEM                = "Confirmar";
const char MSG_SCREEN_INFO[] PROGMEM                 = "FW info";
const char MSG_SCREEN_INFO_TEXT[] PROGMEM            = "FW info";
const char MSG_SCREEN_INFO_BOX[] PROGMEM             = "FW info";

const char MSG_SCREEN_STOP_CONFIRM[] PROGMEM         = "Parar";
const char MSG_SCREEN_STOP_BACK[] PROGMEM            = "Vovler";
const char MSG_SCREEN_STOP_OK[] PROGMEM              = "Confirmar";

const char MSG_SCREEN_CHANGE_TITLE[] PROGMEM         = "Titulo";
const char MSG_SCREEN_CHANGE_BOX[] PROGMEM           = "Test";
const char MSG_SCREEN_CHANGE_CONFIRM[] PROGMEM       = "Cambiar filamento";
const char MSG_SCREEN_CHANGE_START[] PROGMEM         = "Empezar";
const char MSG_SCREEN_CHANGE_PULLOUT[] PROGMEM       = "Tire del filamento";
const char MSG_SCREEN_CHANGE_INSERT[] PROGMEM        = "Insertar & pulsar";
const char MSG_SCREEN_CHANGE_RETRY[] PROGMEM         = "OK?";

const char MSG_SCREEN_SPEED[] PROGMEM                = "Cambiar velocidad";
const char MSG_SELECTOR_BOX[] PROGMEM                = "Seleccione un valor";
const char MSG_OK1[] PROGMEM                         = "OK";

#endif

#endif // ifndef LANGUAGE_H
