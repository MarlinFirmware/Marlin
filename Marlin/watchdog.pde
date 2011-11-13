#ifdef USE_WATCHDOG
#include "watchdog.h"
#include  <avr/wdt.h>
#include  <avr/interrupt.h>

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
  WDTCSR = (1<<WDCE )|(1<<WDE ); //allow changes
  WDTCSR = (1<<WDIF)|(1<<WDIE)| (1<<WDCE )|(1<<WDE )|  (1<<WDP2 )|(1<<WDP1)|(0<<WDP0);
}

/// reset watchdog. MUST be called every 1s after init or avr will reset.
void wd_reset() 
{
  wdt_reset();
  timeout_seconds=0; //reset counter for resets
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
      SERIAL_ERROR_START;
      SERIAL_ERRORLNPGM("Something is wrong, please turn off the printer.");
    #else
      LCD_MESSAGEPGM("Timeout, resetting!");
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
