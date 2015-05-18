#include "GuiAction.h"

#include "Marlin.h"
#include "cardreader.h"
#include "ConfigurationStore.h"
#include "temperature.h"
#include "language.h"


	void action_homing()
	{
    	enquecommand_P(PSTR("G28"));
    	SERIAL_ECHOLN("G28");
	}

	void action_print()
	{
		char cmd[30];
		char* c;
		strcpy(cmd, card.longFilename);
		for (c = &cmd[0]; *c; c++)
		{
			/*if ((uint8_t)*c > 127) {
			SERIAL_ECHOLN(MSG_SD_BAD_FILENAME);
			LCD_MESSAGEPGM(MSG_SD_BAD_FILENAME);
			draw_status_screen();
			lcd_update();
			return;
			}*/
		}
		setTargetHotend0(200);
		fanSpeed = PREHEAT_FAN_SPEED;
		sprintf_P(cmd, PSTR("M23 %s"), card.filename);
		enquecommand_P(PSTR("G28"));
		enquecommand_P(PSTR("G1 Z10"));
		for(c = &cmd[4]; *c; c++)
		*c = tolower(*c);
		enquecommand(cmd);
		enquecommand_P(PSTR("M24"));

        }
