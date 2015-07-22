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

#define MACHINE_NAME "Witbox 2"
#define FIRMWARE_URL "http://www.bq.com/gb/support/witbox"
#define SOURCE_CODE_URL "http://github.com/bq/Marlin"
#define FIRMWARE_VER "2.0.0"
#define BUILD_VER ""

#ifndef LANGUAGE_CHOICE
	#define LANGUAGE_CHOICE 5  // Pick your language from the list above
#endif

// LCD Menu Messages
#if LANGUAGE_CHOICE == 1

const char MSG_SCREEN_EMERGENCY_TITLE[] PROGMEM      = "Emergency Stop active";
const char MSG_SCREEN_EMERGENCY_TEXT[] PROGMEM       = "Keep the button pressed to unlock the machine";
const char MSG_SCREEN_EMERGENCY_BOX[] PROGMEM        = "Keep pressed";

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
const char MSG_AUTOLEVEL[] PROGMEM                   = "Autolevel: On";
const char MSG_AUTOLEVEL_DISABLE[] PROGMEM           = "Autolevel: Off";
const char MSG_OFFSET[] PROGMEM                      = "Offset";

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
const char MSG_SCREEN_UNLOAD_WAIT[] PROGMEM          = "Please wait";
const char MSG_SCREEN_UNLOAD_CONTINUE[] PROGMEM      = "Press to continue";
const char MSG_SCREEN_UNLOAD_PULL[] PROGMEM          = "Extrude and pull";
const char MSG_SCREEN_UNLOAD_CONFIRM[] PROGMEM       = "Finished?";
const char MSG_SCREEN_UNLOADING_TEXT[] PROGMEM       = "Unloading...";

const char MSG_SCREEN_LOAD_TITLE[] PROGMEM           = "Load filament";
const char MSG_SCREEN_LOAD_TEXT1[] PROGMEM           = "Press the button and press the filament inside";
const char MSG_SCREEN_LOAD_TEXT2[] PROGMEM           = "Start process?";
const char MSG_SCREEN_LOAD_WAIT[] PROGMEM            = "Please wait";
const char MSG_SCREEN_LOAD_CONTINUE[] PROGMEM        = "Press to continue";
const char MSG_SCREEN_LOAD_PRESS[] PROGMEM           = "Push to continue";
const char MSG_SCREEN_LOAD_CONFIRM[] PROGMEM         = "Finished?";
const char MSG_SCREEN_LOADING_TEXT[] PROGMEM         = "Loading...";

const char MSG_SCREEN_LEVEL_TITLE[] PROGMEM          = "Level plate";
const char MSG_SCREEN_LEVEL_TEXT[] PROGMEM           = "Start leveling?";
const char MSG_SCREEN_LEVEL_WAIT[] PROGMEM           = "Please wait";
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
const char MSG_SCREEN_AUTOHOME_TITLE[] PROGMEM       = "Auto-home";
const char MSG_SCREEN_AUTOHOME_TEXT[] PROGMEM        = "Homing...";
const char MSG_SCREEN_AUTOHOME_WAIT[] PROGMEM        = "Please wait";

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
const char MSG_SCREEN_MOVE_HEAT_CONFIRM[] PROGMEM    = "Preheat extruder?";
const char MSG_SCREEN_MOVE_BOX0[] PROGMEM            = "Please wait";

const char MSG_SCREEN_BACK2MAIN[] PROGMEM            = "Back";

const char MSG_SCREEN_PRINT_PRINTING[] PROGMEM       = "Printing";
const char MSG_SCREEN_PRINT_TITLE[] PROGMEM          = "Print finished";
const char MSG_SCREEN_PRINT_COMPLETED[] PROGMEM      = "Remove the piece from the plate";
const char MSG_SCREEN_PRINT_COMPLETED_BOX[] PROGMEM  = "Push to continue";
const char MSG_SCREEN_PRINT_PAUSE[] PROGMEM          = "Pause";
const char MSG_SCREEN_PRINT_PAUSED[] PROGMEM         = "Paused";
const char MSG_SCREEN_PRINT_PAUSING[] PROGMEM        = "Pausing...";
const char MSG_SCREEN_PRINT_STOPPED[] PROGMEM        = "Stopped";

const char MSG_SCREEN_TEMP_TITLE[] PROGMEM           = "Change temperature";
const char MSG_SCREEN_LIGHT[] PROGMEM                = "Led light on";
const char MSG_SCREEN_AUTOLEVEL[] PROGMEM            = "Autolevel";
const char MSG_SCREEN_PRINT[] PROGMEM                = "Confirm";
const char MSG_SCREEN_INFO[] PROGMEM                 = "FW info";
const char MSG_SCREEN_INFO_TEXT[] PROGMEM            = "FW info";
const char MSG_SCREEN_INFO_BOX[] PROGMEM             = "Push to continue"; 

const char MSG_SCREEN_OFFSET_TITLE[] PROGMEM         = "Offset";
const char MSG_SCREEN_OFFSET_TEXT[] PROGMEM          = "Adjust offset?";
const char MSG_SCREEN_OFFSET_DIALOG_TEXT[] PROGMEM   = "Adjust the distance between the extruder & plate";
const char MSG_SCREEN_OFFSET_DIALOG_BOX[] PROGMEM    = "Push to continue";
const char MSG_SCREEN_OFFSET_FINISH[] PROGMEM        = "Finished?";
const char MSG_SCREEN_OFFSET_WAIT[] PROGMEM          = "Please wait";
const char MSG_SCREEN_OFFSET_WAIT_BOX[] PROGMEM      = "Push to continue";
const char MSG_SCREEN_OFFSET_HOME_TEXT[] PROGMEM     = "Performing homing operation";
const char MSG_SCREEN_OFFSET_PLANE_TEXT[] PROGMEM    = "Calculating the offset plane...";

const char MSG_SCREEN_STOP_CONFIRM[] PROGMEM         = "Stop";
const char MSG_SCREEN_STOP_BACK[] PROGMEM            = "Back";
const char MSG_SCREEN_STOP_OK[] PROGMEM              = "Confirm";

const char MSG_SCREEN_CHANGE_TITLE[] PROGMEM         = "Change filament";
const char MSG_SCREEN_CHANGE_BOX[] PROGMEM           = "Push to continue";
const char MSG_SCREEN_CHANGE_CONFIRM[] PROGMEM       = "Change filament";
const char MSG_SCREEN_CHANGE_START[] PROGMEM         = "Start";
const char MSG_SCREEN_CHANGE_PULLOUT[] PROGMEM       = "Pull out";
const char MSG_SCREEN_CHANGE_INSERT[] PROGMEM        = "Insert & press";
const char MSG_SCREEN_CHANGE_RETRY[] PROGMEM         = "Finished?";
const char MSG_SCREEN_CHANGE_PAUSING[] PROGMEM       = "Pausing...";

const char MSG_SCREEN_HEATING_MAIN_TITLE[] PROGMEM   = "Change temperature";
const char MSG_SCREEN_HEATING_MAIN_BOX[] PROGMEM     = "Back";

const char MSG_SCREEN_SPEED[] PROGMEM                = "Change speed";
const char MSG_SELECTOR_BOX[] PROGMEM                = "Select a value";
const char MSG_OK1[] PROGMEM                         = "OK";

const char MSG_SCREEN_SERIAL_TITLE[] PROGMEM         = "Serial control";
#endif


#if LANGUAGE_CHOICE == 5

const char MSG_SCREEN_EMERGENCY_TITLE[] PROGMEM      = "Parada Emerg. activa";
const char MSG_SCREEN_EMERGENCY_TEXT[] PROGMEM       = "Deja pulsado el boton para desbloquear la maquina";
const char MSG_SCREEN_EMERGENCY_BOX[] PROGMEM        = "Manten pulsado";

const char MSG_NOSD[] PROGMEM                        = "Inserta tarjeta SD";
const char MSG_SD[] PROGMEM                          = "Tarjeta SD";
const char MSG_FILAMENT_UNLOAD[] PROGMEM             = "Expulsar filamento";
const char MSG_FILAMENT_LOAD[] PROGMEM               = "Insertar filamento";
const char MSG_LEVELING[] PROGMEM                    = "Nivelar base";
const char MSG_HOMING[] PROGMEM                      = "Auto-home";
const char MSG_SETTINGS[] PROGMEM                    = "Opciones";
const char MSG_STEPPERS[] PROGMEM                    = "Desactivar motores";
const char MSG_STEPPERS_OFF[] PROGMEM                = "Motores : OFF";
const char MSG_MOVEAXIS[] PROGMEM                    = "Mover ejes";
const char MSG_TEMPERATURE[] PROGMEM                 = "Temperatura";
const char MSG_LIGHTLED_DISABLE[] PROGMEM            = "Luz Led : OFF";
const char MSG_LIGHTLED[] PROGMEM                    = "Luz Led : ON";
const char MSG_INFO[] PROGMEM                        = "FW info";
const char MSG_PLAY[] PROGMEM                        = "Continuar";
const char MSG_PAUSE[] PROGMEM                       = "Pausar";
const char MSG_STOP[] PROGMEM                        = "Detener";
const char MSG_CHANGE_FILAMENT[] PROGMEM             = "Cambiar filamento";
const char MSG_CHANGE_SPEED[] PROGMEM                = "Cambiar velocidad";
const char MSG_BACK[] PROGMEM                        = "Volver";
const char MSG_RETRY[] PROGMEM                       = "Reintentar";
const char MSG_OK2[] PROGMEM                         = "Confirmar";
const char MSG_MOVE_X[] PROGMEM                      = "Mover eje X";
const char MSG_MOVE_Y[] PROGMEM                      = "Mover eje Y";
const char MSG_MOVE_Z[] PROGMEM                      = "Mover eje Z";
const char MSG_MOVE_E[] PROGMEM                      = "Mover eje E";
const char MSG_AUTOLEVEL[] PROGMEM                   = "Autolevel : ON";
const char MSG_AUTOLEVEL_DISABLE[] PROGMEM           = "Autolevel : OFF";
const char MSG_OFFSET[] PROGMEM                      = "Ajustar offset";

const char MSG_SCREEN_LOGO[] PROGMEM                 = "BQ logo";
const char MSG_SCREEN_LOGO_TEXT[] PROGMEM            = "Test";
const char MSG_SCREEN_LOGO_BOX[] PROGMEM             = "Test";

const char MSG_SCREEN_SD_LIST[] PROGMEM              = "Tarjeta SD";
const char MSG_SCREEN_SD_CONFIRM[] PROGMEM           = "Confirmar";
const char MSG_SCREEN_SD_BACK[] PROGMEM              = "Volver";
const char MSG_SCREEN_SD_LIST_BACK[] PROGMEM         = "Volver al menu";
const char MSG_SCREEN_SD_LIST_PREV[] PROGMEM         = "Carpeta anterior";

const char MSG_SCREEN_UNLOAD_TITLE[] PROGMEM         = "Descargar filamento";
const char MSG_SCREEN_UNLOAD_TEXT1[] PROGMEM         = "Pulsar y extraer el filamento cuando comience a extruir";
const char MSG_SCREEN_UNLOAD_TEXT2[] PROGMEM         = "Iniciar proceso?";
const char MSG_SCREEN_UNLOAD_WAIT[] PROGMEM          = "Por favor, espera";
const char MSG_SCREEN_UNLOAD_CONTINUE[] PROGMEM      = "Pulsa para continuar";
const char MSG_SCREEN_UNLOAD_PULL[] PROGMEM          = "Extruir y sacar";
const char MSG_SCREEN_UNLOAD_CONFIRM[] PROGMEM       = "Finalizado?";
const char MSG_SCREEN_UNLOADING_TEXT[] PROGMEM       = "Descargando filamento";

const char MSG_SCREEN_LOAD_TITLE[] PROGMEM           = "Cargar filamento";
const char MSG_SCREEN_LOAD_TEXT1[] PROGMEM           = "Pulsar e introducir el filamento";
const char MSG_SCREEN_LOAD_TEXT2[] PROGMEM           = "Iniciar proceso?";
const char MSG_SCREEN_LOAD_WAIT[] PROGMEM            = "Por favor, espera";
const char MSG_SCREEN_LOAD_CONTINUE[] PROGMEM        = "Pulsa para continuar";
const char MSG_SCREEN_LOAD_PRESS[] PROGMEM           = "Pulsa para continuar";
const char MSG_SCREEN_LOAD_CONFIRM[] PROGMEM         = "Finalizado?";
const char MSG_SCREEN_LOADING_TEXT[] PROGMEM         = "Cargando filamento";

const char MSG_SCREEN_LEVEL_TITLE[] PROGMEM          = "Nivelar base";
const char MSG_SCREEN_LEVEL_TEXT[] PROGMEM           = "Comenzar nivelado?";
const char MSG_SCREEN_LEVEL_WAIT[] PROGMEM           = "Por favor, espera";
const char MSG_SCREEN_LEVEL_CONTINUE[] PROGMEM       = "Pulsa para continuar";
const char MSG_SCREEN_LEVEL_TEXT0[] PROGMEM          = "Realizando homing";
const char MSG_SCREEN_LEVEL_TEXT1[] PROGMEM          = "Ajusta el tornillo izquierdo hasta que se encienda la luz roja";
const char MSG_SCREEN_LEVEL_TEXT2[] PROGMEM          = "Repite el paso anterior utilizando el tornillo derecho";
const char MSG_SCREEN_LEVEL_TEXT3[] PROGMEM          = "Ahora, ajusta el tornillo central";
const char MSG_SCREEN_LEVEL_TEXT4[] PROGMEM          = "Finalmente ajusta el plato hasta que aparezca la luz";
const char MSG_SCREEN_LEVEL_BOX0[] PROGMEM           = "Por favor espera";
const char MSG_SCREEN_LEVEL_BOX1[] PROGMEM           = "Pulsa para continuar";
const char MSG_SCREEN_LEVEL_BOX2[] PROGMEM           = "Pulsa para continuar";
const char MSG_SCREEN_LEVEL_BOX3[] PROGMEM           = "Pulsa para continuar";
const char MSG_SCREEN_LEVEL_BOX4[] PROGMEM           = "Pulsa para continuar";
const char MSG_SCREEN_LEVEL_CONFIRM[] PROGMEM        = "Finalizado?";

const char MSG_SCREEN_AUTOHOME[] PROGMEM             = "Auto-home";
const char MSG_SCREEN_AUTOHOME_TITLE[] PROGMEM       = "Auto-home";
const char MSG_SCREEN_AUTOHOME_TEXT[] PROGMEM        = "Homing...";
const char MSG_SCREEN_AUTOHOME_WAIT[] PROGMEM        = "Por favor, espera";

const char MSG_SCREEN_SETTINGS[] PROGMEM             = "Opciones";
const char MSG_SCREEN_SETTINGS_TEXT[] PROGMEM        = "Selecciona opcion";
const char MSG_SCREEN_STEPPER[] PROGMEM              = "Motores activos";

const char MSG_SCREEN_MOVE_TITLE[] PROGMEM           = "Mover ejes";
const char MSG_SCREEN_MOVE_TEXT[] PROGMEM            = "Selecciona eje";
const char MSG_SCREEN_MOVE_X[] PROGMEM               = "Eje X / mm";
const char MSG_SCREEN_MOVE_Y[] PROGMEM               = "Eje Y / mm";
const char MSG_SCREEN_MOVE_Z[] PROGMEM               = "Eje Z / mm";
const char MSG_SCREEN_MOVE_E[] PROGMEM               = "Extrusor / mm";
const char MSG_SCREEN_MOVE_2MOVE[] PROGMEM           = "Volver";
const char MSG_SCREEN_MOVE_01MM[] PROGMEM            = "Mover 0.1mm";
const char MSG_SCREEN_MOVE_1MM[] PROGMEM             = "Mover 1mm";
const char MSG_SCREEN_MOVE_10MM[] PROGMEM            = "Mover 10mm";
const char MSG_SCREEN_MOVE_HEAT_CONFIRM[] PROGMEM    = "Calentar extrusor?";
const char MSG_SCREEN_MOVE_BOX0[] PROGMEM            = "Por favor, espera";

const char MSG_SCREEN_BACK2MAIN[] PROGMEM            = "Volver";

const char MSG_SCREEN_PRINT_PRINTING[] PROGMEM       = "Imprimiendo";
const char MSG_SCREEN_PRINT_TITLE[] PROGMEM          = "Impresion finalizada";
const char MSG_SCREEN_PRINT_COMPLETED[] PROGMEM      = "Retire la pieza del plato";
const char MSG_SCREEN_PRINT_COMPLETED_BOX[] PROGMEM  = "Pulsa para continuar";
const char MSG_SCREEN_PRINT_PAUSE[] PROGMEM          = "Pausa";
const char MSG_SCREEN_PRINT_PAUSED[] PROGMEM         = "En pausa";
const char MSG_SCREEN_PRINT_PAUSING[] PROGMEM        = "Pausando...";
const char MSG_SCREEN_PRINT_STOPPED[] PROGMEM        = "Parada";

const char MSG_SCREEN_TEMP_TITLE[] PROGMEM           = "Cambiar temperatura";
const char MSG_SCREEN_LIGHT[] PROGMEM                = "Luces encendidas";
const char MSG_SCREEN_AUTOLEVEL[] PROGMEM            = "Autolevel";
const char MSG_SCREEN_PRINT[] PROGMEM                = "Confirmar";
const char MSG_SCREEN_INFO[] PROGMEM                 = "FW info";
const char MSG_SCREEN_INFO_TEXT[] PROGMEM            = "FW info";
const char MSG_SCREEN_INFO_BOX[] PROGMEM             = "Pulsa para continuar";

const char MSG_SCREEN_OFFSET_TITLE[] PROGMEM         = "Ajustar offset";
const char MSG_SCREEN_OFFSET_TEXT[] PROGMEM          = "Ajustar offset?";
const char MSG_SCREEN_OFFSET_DIALOG_TEXT[] PROGMEM   = "Pulsa y luego gira el boton hasta que el extrusor toque la base";
const char MSG_SCREEN_OFFSET_DIALOG_BOX[] PROGMEM    = "Pulsa para continuar";
const char MSG_SCREEN_OFFSET_FINISH[] PROGMEM        = "Finalizado?";
const char MSG_SCREEN_OFFSET_WAIT[] PROGMEM          = "Espera...";
const char MSG_SCREEN_OFFSET_WAIT_BOX[] PROGMEM      = "Pulsa para continuar";
const char MSG_SCREEN_OFFSET_HOME_TEXT[] PROGMEM     = "Realizando homing...";
const char MSG_SCREEN_OFFSET_PLANE_TEXT[] PROGMEM    = "Calculando el plano de offset...";

const char MSG_SCREEN_STOP_CONFIRM[] PROGMEM         = "Detener";
const char MSG_SCREEN_STOP_BACK[] PROGMEM            = "Volver";
const char MSG_SCREEN_STOP_OK[] PROGMEM              = "Confirmar";

const char MSG_SCREEN_CHANGE_TITLE[] PROGMEM         = "Cambiar filamento";
const char MSG_SCREEN_CHANGE_BOX[] PROGMEM           = "Pulsa para continuar";
const char MSG_SCREEN_CHANGE_CONFIRM[] PROGMEM       = "Cambiar filamento";
const char MSG_SCREEN_CHANGE_START[] PROGMEM         = "Empezar";
const char MSG_SCREEN_CHANGE_PULLOUT[] PROGMEM       = "Pulsa y extrae el filamento cuando comience a extruir";
const char MSG_SCREEN_CHANGE_INSERT[] PROGMEM        = "Por ultimo, pulsa e introduce el filamento";
const char MSG_SCREEN_CHANGE_RETRY[] PROGMEM         = "Finalizado?";
const char MSG_SCREEN_CHANGE_PAUSING[] PROGMEM       = "Pausando...";

const char MSG_SCREEN_HEATING_MAIN_TITLE[] PROGMEM   = "Cambiar temperatura";
const char MSG_SCREEN_HEATING_MAIN_BOX[] PROGMEM     = "Volver";

const char MSG_SCREEN_SPEED[] PROGMEM                = "Cambiar velocidad";
const char MSG_SELECTOR_BOX[] PROGMEM                = "Selecciona valor";
const char MSG_OK1[] PROGMEM                         = "OK";

const char MSG_SCREEN_SERIAL_TITLE[] PROGMEM         = "Control serie";

#endif

#endif // ifndef LANGUAGE_H
