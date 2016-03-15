#include "Marlin.h"

#ifdef USE_WATCHDOG
#include <avr/wdt.h>

#include "watchdog.h"
#include "ultralcd.h"

//===========================================================================
//=============================private variables  ============================
//===========================================================================

//===========================================================================
//=============================functinos         ============================
//===========================================================================


/// intialise watch dog with a 4 sec interrupt time
void watchdog_init()
{
#ifdef WATCHDOG_RESET_MANUAL
    //We enable the watchdog timer, but only for the interrupt.
    //Take care, as this requires the correct order of operation, with interrupts disabled. See the datasheet of any AVR chip for details.
    wdt_reset();
    _WD_CONTROL_REG = _BV(_WD_CHANGE_BIT) | _BV(WDE);
    _WD_CONTROL_REG = _BV(WDIE) | WDTO_4S;
#else
    wdt_enable(WDTO_4S);
#endif
}

/// reset watchdog. MUST be called every 1s after init or avr will reset.
void watchdog_reset() 
{
    wdt_reset();
}

//===========================================================================
//=============================ISR               ============================
//===========================================================================

//Watchdog timer interrupt, called if main program blocks >1sec and manual reset is enabled.
#ifdef WATCHDOG_RESET_MANUAL
ISR(WDT_vect)
{ 
    //TODO: This message gets overwritten by the kill() call
    LCD_ALERTMESSAGEPGM("ERR:Please Reset");//16 characters so it fits on a 16x2 display
    lcd_update();
    SERIAL_ERROR_START;
    SERIAL_ERRORLNPGM("Something is wrong, please turn off the printer.");
    kill(); //kill blocks
    while(1); //wait for user or serial reset
}
#endif//RESET_MANUAL

#endif//USE_WATCHDOG
