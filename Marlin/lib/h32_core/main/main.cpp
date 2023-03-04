#include "../Arduino.h"
#include "init.h"

int main(void)
{
	board_init();
	setup();
	while (1)
	{
		loop();
	}
}
