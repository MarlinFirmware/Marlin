#ifndef LANGUAGE_ES_H
#define LANGUAGE_ES_H

#include <avr/pgmspace.h>

const char ES_SCREEN_EMERGENCY_TITLE[] PROGMEM      = "Parada Emerg. activa";
const char ES_SCREEN_EMERGENCY_TEXT[] PROGMEM       = "Deja pulsado el boton para desbloquear la maquina";
const char ES_SCREEN_EMERGENCY_BOX[] PROGMEM        = "Manten pulsado";

const char ES_NOSD[] PROGMEM                        = "Inserta tarjeta SD";
const char ES_SD[] PROGMEM                          = "Tarjeta SD";
const char ES_FILAMENT_UNLOAD[] PROGMEM             = "Expulsar filamento";
const char ES_FILAMENT_LOAD[] PROGMEM               = "Insertar filamento";
const char ES_LEVELING[] PROGMEM                    = "Nivelar base";
const char ES_HOMING[] PROGMEM                      = "Auto-home";
const char ES_SETTINGS[] PROGMEM                    = "Opciones";
const char ES_STEPPERS[] PROGMEM                    = "Desactivar motores";
const char ES_STEPPERS_OFF[] PROGMEM                = "Motores : OFF";
const char ES_MOVEAXIS[] PROGMEM                    = "Mover ejes";
const char ES_TEMPERATURE[] PROGMEM                 = "Temperatura";
const char ES_LIGHTLED_DISABLE[] PROGMEM            = "Luz Led : OFF";
const char ES_LIGHTLED[] PROGMEM                    = "Luz Led : ON";
const char ES_INFO[] PROGMEM                        = "FW info";
const char ES_PLAY[] PROGMEM                        = "Continuar";
const char ES_PAUSE[] PROGMEM                       = "Pausar";
const char ES_STOP[] PROGMEM                        = "Detener";
const char ES_CHANGE_FILAMENT[] PROGMEM             = "Cambiar filamento";
const char ES_CHANGE_SPEED[] PROGMEM                = "Cambiar velocidad";
const char ES_BACK[] PROGMEM                        = "Volver";
const char ES_RETRY[] PROGMEM                       = "Reintentar";
const char ES_OK2[] PROGMEM                         = "Confirmar";
const char ES_MOVE_X[] PROGMEM                      = "Mover eje X";
const char ES_MOVE_Y[] PROGMEM                      = "Mover eje Y";
const char ES_MOVE_Z[] PROGMEM                      = "Mover eje Z";
const char ES_MOVE_E[] PROGMEM                      = "Mover eje E";
const char ES_AUTOLEVEL[] PROGMEM                   = "Autolevel : ON";
const char ES_AUTOLEVEL_DISABLE[] PROGMEM           = "Autolevel : OFF";
const char ES_OFFSET[] PROGMEM                      = "Ajustar offset";

const char ES_SCREEN_LOGO[] PROGMEM                 = "BQ logo";
const char ES_SCREEN_LOGO_TEXT[] PROGMEM            = "Test";
const char ES_SCREEN_LOGO_BOX[] PROGMEM             = "Test";

const char ES_SCREEN_SD_LIST[] PROGMEM              = "Tarjeta SD";
const char ES_SCREEN_SD_CONFIRM[] PROGMEM           = "Confirmar";
const char ES_SCREEN_SD_TITLE[] PROGMEM             = "Imprimiendo";
const char ES_SCREEN_SD_TEXT[] PROGMEM              = "Empezando la impresion";
const char ES_SCREEN_SD_WAIT[] PROGMEM              = "Por favor, espera";
const char ES_SCREEN_SD_LIST_BACK[] PROGMEM         = "Volver al menu";
const char ES_SCREEN_SD_LIST_PREV[] PROGMEM         = "Carpeta anterior";

const char ES_SCREEN_UNLOAD_TITLE[] PROGMEM         = "Descargar filamento";
const char ES_SCREEN_UNLOAD_TEXT1[] PROGMEM         = "Pulsar y extraer el filamento cuando comience a extruir";
const char ES_SCREEN_UNLOAD_TEXT2[] PROGMEM         = "Iniciar proceso?";
const char ES_SCREEN_UNLOAD_WAIT[] PROGMEM          = "Por favor, espera";
const char ES_SCREEN_UNLOAD_CONTINUE[] PROGMEM      = "Pulsa para continuar";
const char ES_SCREEN_UNLOAD_PULL[] PROGMEM          = "Extruir y sacar";
const char ES_SCREEN_UNLOAD_CONFIRM[] PROGMEM       = "Finalizado?";
const char ES_SCREEN_UNLOADING_TEXT[] PROGMEM       = "Descargando filamento";

const char ES_SCREEN_LOAD_TITLE[] PROGMEM           = "Cargar filamento";
const char ES_SCREEN_LOAD_TEXT1[] PROGMEM           = "Pulsar e introducir el filamento";
const char ES_SCREEN_LOAD_TEXT2[] PROGMEM           = "Iniciar proceso?";
const char ES_SCREEN_LOAD_WAIT[] PROGMEM            = "Por favor, espera";
const char ES_SCREEN_LOAD_CONTINUE[] PROGMEM        = "Pulsa para continuar";
const char ES_SCREEN_LOAD_PRESS[] PROGMEM           = "Pulsa para continuar";
const char ES_SCREEN_LOAD_CONFIRM[] PROGMEM         = "Finalizado?";
const char ES_SCREEN_LOADING_TEXT[] PROGMEM         = "Cargando filamento";

const char ES_SCREEN_LEVEL_TITLE[] PROGMEM          = "Nivelar base";
const char ES_SCREEN_LEVEL_TEXT[] PROGMEM           = "Comenzar nivelado?";
const char ES_SCREEN_LEVEL_WAIT[] PROGMEM           = "Por favor, espera";
const char ES_SCREEN_LEVEL_CONTINUE[] PROGMEM       = "Pulsa para continuar";
const char ES_SCREEN_LEVEL_TEXT0[] PROGMEM          = "Realizando homing";
const char ES_SCREEN_LEVEL_TEXT1[] PROGMEM          = "Ajusta el tornillo izquierdo hasta que se encienda la luz roja";
const char ES_SCREEN_LEVEL_TEXT2[] PROGMEM          = "Repite el paso anterior utilizando el tornillo derecho";
const char ES_SCREEN_LEVEL_TEXT3[] PROGMEM          = "Ahora, ajusta el tornillo central";
const char ES_SCREEN_LEVEL_TEXT4[] PROGMEM          = "Finalmente ajusta el plato hasta que aparezca la luz";
const char ES_SCREEN_LEVEL_BOX0[] PROGMEM           = "Por favor espera";
const char ES_SCREEN_LEVEL_BOX1[] PROGMEM           = "Pulsa para continuar";
const char ES_SCREEN_LEVEL_BOX2[] PROGMEM           = "Pulsa para continuar";
const char ES_SCREEN_LEVEL_BOX3[] PROGMEM           = "Pulsa para continuar";
const char ES_SCREEN_LEVEL_BOX4[] PROGMEM           = "Pulsa para continuar";
const char ES_SCREEN_LEVEL_CONFIRM[] PROGMEM        = "Finalizado?";

const char ES_SCREEN_AUTOHOME[] PROGMEM             = "Auto-home";
const char ES_SCREEN_AUTOHOME_TITLE[] PROGMEM       = "Auto-home";
const char ES_SCREEN_AUTOHOME_TEXT[] PROGMEM        = "Homing...";
const char ES_SCREEN_AUTOHOME_WAIT[] PROGMEM        = "Por favor, espera";

const char ES_SCREEN_SETTINGS[] PROGMEM             = "Opciones";
const char ES_SCREEN_SETTINGS_TEXT[] PROGMEM        = "Selecciona opcion";
const char ES_SCREEN_STEPPER[] PROGMEM              = "Motores activos";

const char ES_SCREEN_MOVE_TITLE[] PROGMEM           = "Mover ejes";
const char ES_SCREEN_MOVE_TEXT[] PROGMEM            = "Selecciona eje";
const char ES_SCREEN_MOVE_X[] PROGMEM               = "Eje X / mm";
const char ES_SCREEN_MOVE_Y[] PROGMEM               = "Eje Y / mm";
const char ES_SCREEN_MOVE_Z[] PROGMEM               = "Eje Z / mm";
const char ES_SCREEN_MOVE_E[] PROGMEM               = "Extrusor / mm";
const char ES_SCREEN_MOVE_2MOVE[] PROGMEM           = "Volver";
const char ES_SCREEN_MOVE_01MM[] PROGMEM            = "Mover 0.1mm";
const char ES_SCREEN_MOVE_1MM[] PROGMEM             = "Mover 1mm";
const char ES_SCREEN_MOVE_10MM[] PROGMEM            = "Mover 10mm";
const char ES_SCREEN_MOVE_HEAT_CONFIRM[] PROGMEM    = "Calentar extrusor?";
const char ES_SCREEN_MOVE_BOX0[] PROGMEM            = "Por favor, espera";

const char ES_SCREEN_BACK2MAIN[] PROGMEM            = "Volver";

const char ES_SCREEN_PRINT_PRINTING[] PROGMEM       = "Imprimiendo";
const char ES_SCREEN_PRINT_TITLE[] PROGMEM          = "Impresion finalizada";
const char ES_SCREEN_PRINT_COMPLETED[] PROGMEM      = "Retire la pieza del plato";
const char ES_SCREEN_PRINT_COMPLETED_BOX[] PROGMEM  = "Pulsa para continuar";
const char ES_SCREEN_PRINT_PAUSE[] PROGMEM          = "Pausa";
const char ES_SCREEN_PRINT_PAUSED[] PROGMEM         = "En pausa";
const char ES_SCREEN_PRINT_PAUSING[] PROGMEM        = "Pausando...";
const char ES_SCREEN_PRINT_STOPPED[] PROGMEM        = "Parada";

const char ES_SCREEN_TEMP_TITLE[] PROGMEM           = "Cambiar temperatura";
const char ES_SCREEN_LIGHT[] PROGMEM                = "Luces encendidas";
const char ES_SCREEN_AUTOLEVEL[] PROGMEM            = "Autolevel";
const char ES_SCREEN_PRINT[] PROGMEM                = "Confirmar";
const char ES_SCREEN_INFO[] PROGMEM                 = "FW info";
const char ES_SCREEN_INFO_TEXT[] PROGMEM            = "FW info";
const char ES_SCREEN_INFO_BOX[] PROGMEM             = "Pulsa para continuar";

const char ES_SCREEN_OFFSET_TITLE[] PROGMEM         = "Ajustar offset";
const char ES_SCREEN_OFFSET_TEXT[] PROGMEM          = "Ajustar offset?";
const char ES_SCREEN_OFFSET_DIALOG_TEXT[] PROGMEM   = "Pulsa y luego gira el boton hasta que el extrusor toque la base";
const char ES_SCREEN_OFFSET_DIALOG_BOX[] PROGMEM    = "Pulsa para continuar";
const char ES_SCREEN_OFFSET_FINISH[] PROGMEM        = "Finalizado?";
const char ES_SCREEN_OFFSET_WAIT[] PROGMEM          = "Espera...";
const char ES_SCREEN_OFFSET_WAIT_BOX[] PROGMEM      = "Pulsa para continuar";
const char ES_SCREEN_OFFSET_HOME_TEXT[] PROGMEM     = "Realizando homing...";
const char ES_SCREEN_OFFSET_PLANE_TEXT[] PROGMEM    = "Calculando el plano de offset...";

const char ES_SCREEN_STOP_CONFIRM[] PROGMEM         = "Detener";
const char ES_SCREEN_STOP_BACK[] PROGMEM            = "Volver";
const char ES_SCREEN_STOP_OK[] PROGMEM              = "Confirmar";

const char ES_SCREEN_CHANGE_TITLE[] PROGMEM         = "Cambiar filamento";
const char ES_SCREEN_CHANGE_BOX[] PROGMEM           = "Pulsa para continuar";
const char ES_SCREEN_CHANGE_CONFIRM[] PROGMEM       = "Cambiar filamento";
const char ES_SCREEN_CHANGE_START[] PROGMEM         = "Empezar";
const char ES_SCREEN_CHANGE_PULLOUT[] PROGMEM       = "Pulsa y extrae el filamento cuando comience a extruir";
const char ES_SCREEN_CHANGE_INSERT[] PROGMEM        = "Por ultimo, pulsa e introduce el filamento";
const char ES_SCREEN_CHANGE_RETRY[] PROGMEM         = "Finalizado?";
const char ES_SCREEN_CHANGE_PAUSING[] PROGMEM       = "Pausando...";

const char ES_SCREEN_HEATING_MAIN_TITLE[] PROGMEM   = "Cambiar temperatura";
const char ES_SCREEN_HEATING_MAIN_BOX[] PROGMEM     = "Volver";

const char ES_SCREEN_SPEED[] PROGMEM                = "Cambiar velocidad";
const char ES_SELECTOR_BOX[] PROGMEM                = "Selecciona valor";
const char ES_OK1[] PROGMEM                         = "OK";

const char ES_SCREEN_SERIAL_TITLE[] PROGMEM         = "Control serie";

#endif //LANGUAGE_ES_H
