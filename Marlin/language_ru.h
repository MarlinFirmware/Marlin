/**
 * Russian
 *
 * LCD Menu Messages
 * Please note these are limited to 17 characters!
 *
 */
#ifndef LANGUAGE_RU_H
#define LANGUAGE_RU_H

#define LANGUAGE_RU

    #define MSG_SDPRINTING 				        "Печать с SD карты" 	//MG Only
	#define MSG_FILAMENTCHANGENOW 				" -=  нажмите OK =- " 	//MG Only
	#define MSG_MG_EXTEND 						"Дополнения Magnum"			//MG Only
	#define MSG_MG_PASTA_ENABLE 				"Паста " 		//MG Only
	#define MSG_MG_PASTA_DIA 					"Изм. Df " 		//MG Only
	#define MSG_MG_PASTA_AUTOZ 					"Авто Zm " 		//MG Only	

	#define WELCOME_MSG 				        "-= готов к печати =-"
	#define MSG_SD_INSERTED						"Карта вставлена"
	#define MSG_SD_REMOVED						"Карта извлечена"
	#define MSG_MAIN							"Меню" //"Меню \003"
	#define MSG_AUTOSTART						"Автостарт"
	#define MSG_DISABLE_STEPPERS 				"Выкл. двигатели"
	#define MSG_AUTO_HOME						"Парковка каретки"
	#define MSG_SET_HOME_OFFSETS				"Запомнить ноль"
	#define MSG_SET_ORIGIN						"Установить ноль"
	#define MSG_PREHEAT_PLA 					"Преднагрев PLA"
	#define MSG_PREHEAT_PLA0					"Преднагрев PLA"  // для двухэкструдерного надо
	#define MSG_PREHEAT_PLA1					"Преднагрев PLA1" // по другому написать обе строки
	#define MSG_PREHEAT_PLA2					"Преднагрев PLA2"
	#define MSG_PREHEAT_PLA012 					"Преднаг. PLA все"
	#define MSG_PREHEAT_PLA_BEDONLY 			"Преднагрев стола"
	#define MSG_PREHEAT_PLA_SETTINGS  			"Настройки PLA"
	#define MSG_PREHEAT_ABS						"Преднагрев ABS"
	#define MSG_PREHEAT_ABS0					"Преднагрев ABS"  // для двухэкструдерного надо
	#define MSG_PREHEAT_ABS1					"Преднагрев ABS1" // по другому написать обе строки
	#define MSG_PREHEAT_ABS2					"Преднагрев ABS2"
	#define MSG_PREHEAT_ABS012 					"Преднаг. ABS все "
	#define MSG_PREHEAT_ABS_BEDONLY 			"Преднагрев стола"
	#define MSG_PREHEAT_ABS_SETTINGS  			"Настройки ABS"
	#define MSG_COOLDOWN						"Выключить нагрев"
	#define MSG_SWITCH_PS_ON					"Switch Power On"
	#define MSG_SWITCH_PS_OFF					"Switch Power Off"
	#define MSG_EXTRUDE							"Экструзия"
	#define MSG_RETRACT							"Ретракт"
	#define MSG_MOVE_AXIS  						"Движение по осям"
	#define MSG_MOVE_X                          "Движение по оси X"
	#define MSG_MOVE_Y                          "Движение по оси Y"
	#define MSG_MOVE_Z                          "Движение по оси Z"
	#define MSG_MOVE_E                          "Экструдер"
	#define MSG_MOVE_E1 						"Экструдер 2"
	#define MSG_MOVE_E2 						"Экструдер 3"
	#define MSG_MOVE_01MM                       "Сдвинуть на 0.1mm"
	#define MSG_MOVE_1MM                        "Сдвинуть на  1 mm"
	#define MSG_MOVE_10MM                       "Сдвинуть на 10 mm"
	#define MSG_SPEED							"Скорость"
	#define MSG_NOZZLE							"\002 Экструдер t\001"
	#define MSG_NOZZLE1							"\002 Экструдер-2 t\001"
	#define MSG_NOZZLE2							"\002 Экструдер-3 t\001"
	#define MSG_BED								"- Стол t\001"
	#define MSG_FAN_SPEED						"Обдув модели"
	#define MSG_FLOW							"Подача прутка" //это для всех экструдеров (отключено в меню)
	#define MSG_FLOW0							"Подача прутка" //это для экструдера 0
	#define MSG_FLOW1							"Поток1"
	#define MSG_FLOW2 							"Поток2"
	#define MSG_CONTROL							"Настроить принтер"
	#define MSG_MIN								"\002 Минимум"
	#define MSG_MAX								"\002 Максимум"
	#define MSG_FACTOR							"\002 Фактор"
	#define MSG_AUTOTEMP						"Autotemp"
	#define MSG_ON								"Вкл. "
	#define MSG_OFF								"Выкл. "
	#define MSG_PID_P							"PID-P "
	#define MSG_PID_I							"PID-I "
	#define MSG_PID_D							"PID-D "
	#define MSG_PID_C							"PID-C "
	#define MSG_ACC								"Ускорение"//"Acc:"
	#define MSG_VXY_JERK						"XY Jerk"
	#define MSG_VZ_JERK                         "Z  Jerk"
	#define MSG_VE_JERK                         "E  Jerk"
	#define MSG_VMAX							"Max скорость"
	#define MSG_X								"X"
	#define MSG_Y								"Y"
	#define MSG_Z								"Z"
	#define MSG_E								"E"
	#define MSG_VMIN							"Мин. скорость"
	#define MSG_VTRAV_MIN						"Мин. скорость T"
	#define MSG_AMAX							"Amax "
	#define MSG_A_RETRACT						"A-retract:"
	#define MSG_XSTEPS							"X шаг/mm:"
	#define MSG_YSTEPS							"Y шаг/mm:"
	#define MSG_ZSTEPS							"Z шаг/mm:"
	#define MSG_ESTEPS							"E шаг/mm:"
	#define MSG_RECTRACT    					"Ретракт"
	#define MSG_TEMPERATURE  					"Нагрев и обдув" //"Температура     \x7E"
	#define MSG_MOTION							"Скоростной режим"//"Скорости        \x7E"	
	#define MSG_VOLUMETRIC                      "Filament"
	#define MSG_VOLUMETRIC_ENABLED		        "E in mm3"
	#define MSG_FILAMENT_SIZE_EXTRUDER_0        "Fil. Dia. 1"
	#define MSG_FILAMENT_SIZE_EXTRUDER_1        "Fil. Dia. 2"
	#define MSG_FILAMENT_SIZE_EXTRUDER_2        "Fil. Dia. 3"
	#define MSG_CONTRAST  						"LCD contrast"
	#define MSG_STORE_EPROM  					"Сохранить в EPROM"
	#define MSG_LOAD_EPROM  					"Загруз.  из EPROM"
	#define MSG_RESTORE_FAILSAFE				"Сброс настроек"
	#define MSG_REFRESH							"\004 Обновить"
	#define MSG_WATCH							"Выход из меню" //"Обзор           \003"
	#define MSG_PREPARE							"Управление" //"Действия        \x7E"
	#define MSG_TUNE							"Подстройка печати" //"Настройки       \x7E" - во время печати
	#define MSG_PAUSE_PRINT  					"Пауза печати"
	#define MSG_RESUME_PRINT					"Продолжить печать"
	#define MSG_STOP_PRINT 						"Остановить печать"
	#define MSG_CARD_MENU						"Меню SD карты"
	#define MSG_NO_CARD							"Нет SD карты"
	#define MSG_DWELL							"Пауза (#G4)"
	#define MSG_USERWAIT  						"ОК для продолжения"
	#define MSG_RESUMING 						"Продолжение печати"
	#define MSG_PRINT_ABORTED                   "Печать отменена"
	#define MSG_NO_MOVE							"Нет движения."
	#define MSG_KILLED							"Остановлен. #M112" //"Printer halted. kill() called!"
	#define MSG_STOPPED							"Остановлен. #M999" //"Printer stopped due to errors. Fix the error and use M999 to restart. (Temperature is reset. Set it after restarting)"
	#define MSG_CONTROL_RETRACT					"Ретракт mm:"
	#define MSG_CONTROL_RETRACT_SWAP			"Ретракт свап mm:" //свап - смена экструдера
	#define MSG_CONTROL_RETRACTF				"Ретракт V:"
	#define MSG_CONTROL_RETRACT_ZLIFT			"Ретракт подьем Z:"
	#define MSG_CONTROL_RETRACT_RECOVER			"unРетракт +mm:"
	#define MSG_CONTROL_RETRACT_RECOVER_SWAP	"unРетракт своп +:"
	#define MSG_CONTROL_RETRACT_RECOVERF		"unРетракт V:"
	#define MSG_AUTORETRACT						"Авто Ретракт:"
	#define MSG_FILAMENTCHANGE 					"Замена прутка"
	#define MSG_INIT_SDCARD 					"Обновить SD карту"
	#define MSG_CNG_SDCARD 						"Сменить  SD карту"
	#define MSG_ZPROBE_OUT 						"Z probe out. bed"
	#define MSG_POSITION_UNKNOWN 				"Home X/Y before Z"
	#define MSG_ZPROBE_ZOFFSET 					"Z Offset"
	#define MSG_BABYSTEP_X 						"Babystep X"
	#define MSG_BABYSTEP_Y 						"Babystep Y"
	#define MSG_BABYSTEP_Z 						"Babystep Z"
	#define MSG_ENDSTOP_ABORT 					"Endstop abort"
	#define MSG_CONTRAST 						"Contrast"

#ifdef DELTA_CALIBRATION_MENU
    #define MSG_DELTA_CALIBRATE             "Delta Calibration"
    #define MSG_DELTA_CALIBRATE_X           "Calibrate X"
    #define MSG_DELTA_CALIBRATE_Y           "Calibrate Y"
    #define MSG_DELTA_CALIBRATE_Z           "Calibrate Z"
    #define MSG_DELTA_CALIBRATE_CENTER      "Calibrate Center"
#endif // DELTA_CALIBRATION_MENU

#endif // LANGUAGE_RU_H
