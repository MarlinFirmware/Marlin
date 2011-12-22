#ifdef USE_WATCHDOG
#include "Marlin.h"
#include "watchdog.h"

//===========================================================================
//=============================private variables  ============================
//===========================================================================

static volatile uint8_t timeout_seconds=0;

void(* ctrlaltdelete) (void) = 0; //does not work on my atmega2560

//===========================================================================
//=============================functinos         ============================
//===========================================================================


/// intialise watch dog with a 1 sec interrupt time
void wd_init() 
{
  WDTCSR |= (1<<WDCE )|(1<<WDE ); //allow changes
  WDTCSR = (1<<WDCE )|(1<<WDE )|(1<<WDP3 )|(1<<WDP0); // Reset after 8 sec.
//  WDTCSR = (1<<WDIF)|(1<<WDIE)| (1<<WDCE )|(1<<WDE )|  (1<<WDP3) | (1<<WDP0);
}

/// reset watchdog. MUST be called every 1s after init or avr will reset.
void wd_reset() 
{
  wdt_reset();
}

//===========================================================================
//=============================ISR               ============================
//===========================================================================

//Watchdog timer interrupt, called if main program blocks >1sec
ISR(WDT_vect) 
{ 
  if(timeout_seconds++ >= WATCHDOG_TIMEOUT)
  {
 
    #ifdef RESET_MANUAL
      LCD_MESSAGEPGM("Please Reset!");
      LCD_STATUS;
      SERIAL_ERROR_START;
      SERIAL_ERRORLNPGM("Something is wrong, please turn off the printer.");
    #else
      LCD_MESSAGEPGM("Timeout, resetting!");
      LCD_STATUS;
    #endif 
    //disable watchdog, it will survife reboot.
    WDTCSR |= (1<<WDCE) | (1<<WDE);
    WDTCSR = 0;
    #ifdef RESET_MANUAL
      kill(); //kill blocks
      while(1); //wait for user or serial reset
    #else
      ctrlaltdelete();
    #endif
  }
}

#endif /* USE_WATCHDOG */
