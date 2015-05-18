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
