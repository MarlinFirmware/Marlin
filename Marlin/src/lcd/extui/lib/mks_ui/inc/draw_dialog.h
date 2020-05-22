#ifndef _LV_DRAW_DIALOG_
#define _LV_DRAW_DIALOG_




#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

#define DIALOG_TYPE_STOP			0
#define DIALOG_TYPE_PRINT_FILE		1
#define DIALOG_TYPE_REPRINT_NO_FILE		2

#define DIALOG_TYPE_M80_FAIL			3  //**
#define DIALOG_TYPE_MESSEGE_ERR1			4//**

#define DIALOG_TYPE_UPDATE_ESP_FIRMARE								5
#define DIALOG_TYPE_UPDATE_ESP_DATA										6
#define DIALOG_TYPE_UPLOAD_FILE												7
#define DIALOG_TYPE_UNBIND															8

#define DIALOG_TYPE_FILAMENT_LOAD_HEAT								9
#define DIALOG_TYPE_FILAMENT_HEAT_LOAD_COMPLETED		10
#define DIALOG_TYPE_FILAMENT_LOADING									11
#define DIALOG_TYPE_FILAMENT_LOAD_COMPLETED					12
#define DIALOG_TYPE_FILAMENT_UNLOAD_HEAT							13
#define DIALOG_TYPE_FILAMENT_HEAT_UNLOAD_COMPLETED	14
#define DIALOG_TYPE_FILAMENT_UNLOADING								15
#define DIALOG_TYPE_FILAMENT_UNLOAD_COMPLETED				16

#define DIALOG_TYPE_FILE_LOADING			17//**

#define DIALOG_TYPE_FILAMENT_NO_PRESS		18
#define DIALOG_TYPE_FINISH_PRINT        19

#define WIFI_ENABLE_TIPS 20

#define DIALOG_PAUSE_MESSAGE_PAUSING		21
#define DIALOG_PAUSE_MESSAGE_CHANGING	22
#define DIALOG_PAUSE_MESSAGE_UNLOAD		23
#define DIALOG_PAUSE_MESSAGE_WAITING		24
#define DIALOG_PAUSE_MESSAGE_INSERT		25
#define DIALOG_PAUSE_MESSAGE_LOAD		26
#define DIALOG_PAUSE_MESSAGE_PURGE		27
#define DIALOG_PAUSE_MESSAGE_RESUME		28
#define DIALOG_PAUSE_MESSAGE_HEAT		29
#define DIALOG_PAUSE_MESSAGE_HEATING		30
#define DIALOG_PAUSE_MESSAGE_OPTION		31

#define BTN_OK_X		100
#define BTN_OK_Y		180
#define BTN_CANCEL_X	280
#define BTN_CANCEL_Y	180

extern void lv_draw_dialog(uint8_t type);
extern void lv_clear_dialog();

//extern void disp_temp_ready_print();
#if defined(__cplusplus)
}    /* Make sure we have C-declarations in C++ programs */
#endif

#endif

