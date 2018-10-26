#ifdef TARGET_LPC1768

// ---------------------
// Userspace entry point
// ---------------------
extern void setup();
extern void loop();

extern "C" {
  #include <lpc17xx_gpio.h>
}

#include <usb/usb.h>
#include <usb/usbcfg.h>
#include <usb/usbhw.h>
#include <usb/usbcore.h>
#include <usb/cdc.h>
#include <usb/cdcuser.h>
#include <usb/mscuser.h>

extern "C" {
  #include <debug_frmwrk.h>
  #include <chanfs/diskio.h>
  #include <chanfs/ff.h>
}

#include "../../inc/MarlinConfig.h"
#include "HAL.h"
#include "fastio.h"
#include "HAL_timers.h"
#include <stdio.h>
#include <stdarg.h>
#include <Arduino.h>
#include "serial.h"
#include "LPC1768_PWM.h"

static __INLINE uint32_t SysTick_Config(uint32_t ticks) {
  if (ticks > SysTick_LOAD_RELOAD_Msk) return 1;

  SysTick->LOAD = (ticks & SysTick_LOAD_RELOAD_Msk) - 1;        // Set reload register
  SysTick->VAL  = 0;                                            // Load the SysTick Counter Value
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                  SysTick_CTRL_TICKINT_Msk   |
                  SysTick_CTRL_ENABLE_Msk;                      // Enable SysTick IRQ and SysTick Timer

  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(0, 0, 0)); // Set Priority for Cortex-M3 System Interrupts
  return 0;
}

extern "C" {
  extern int isLPC1769();
  extern void disk_timerproc(void);
  volatile uint32_t _millis;

  void SysTick_Handler(void) {
    ++_millis;
    disk_timerproc();
  }

  // Runs after clock init and before global static constructors
  void SystemPostInit() {
    _millis = 0;                            // Initialize the millisecond counter value
    SysTick_Config(SystemCoreClock / 1000); // Start millisecond global counter

    // Runs before setup() to configure LED_PIN and used to indicate successful bootloader execution
    #if PIN_EXISTS(LED)
      SET_DIR_OUTPUT(LED_PIN);
      WRITE_PIN_CLR(LED_PIN);

      // MKS_SBASE has 3 other LEDs the bootloader uses during flashing. Clear them.
      SET_DIR_OUTPUT(P1_19);
      WRITE_PIN_CLR(P1_19);
      SET_DIR_OUTPUT(P1_20);
      WRITE_PIN_CLR(P1_20);
      SET_DIR_OUTPUT(P1_21);
      WRITE_PIN_CLR(P1_21);

      for (uint8_t i = 6; i--;) {
        TOGGLE(LED_PIN);
        delay(100);
      }
    #endif
  }
}

extern uint32_t MSC_SD_Init(uint8_t pdrv);

int main(void) {

  (void)MSC_SD_Init(0);

  USB_Init();                               // USB Initialization
  USB_Connect(TRUE);                        // USB Connect

  const uint32_t usb_timeout = millis() + 2000;
  while (!USB_Configuration && PENDING(millis(), usb_timeout)) {
    delay(50);
    #if PIN_EXISTS(LED)
      TOGGLE(LED_PIN);     // Flash quickly during USB initialization
    #endif
  }

  #if NUM_SERIAL > 0
    MYSERIAL0.begin(BAUDRATE);
    #if NUM_SERIAL > 1
      MYSERIAL1.begin(BAUDRATE);
    #endif
    SERIAL_PRINTF("\n\necho:%s (%dMhz) Initialized\n", isLPC1769() ? "LPC1769" : "LPC1768", SystemCoreClock / 1000000);
    SERIAL_FLUSHTX();
  #endif

  HAL_timer_init();

  LPC1768_PWM_init();

  setup();
  for (;;) loop();
}

#endif // TARGET_LPC1768
