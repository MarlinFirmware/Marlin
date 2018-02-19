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
#include "include/Arduino.h"
#include "serial.h"
#include "LPC1768_PWM.h"

static __INLINE uint32_t SysTick_Config(uint32_t ticks) {
  if (ticks > SysTick_LOAD_RELOAD_Msk)
    return (1); /* Reload value impossible */

  SysTick->LOAD = (ticks & SysTick_LOAD_RELOAD_Msk) - 1; /* set reload register */
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(0, 0, 0)); /* set Priority for Cortex-M3 System Interrupts */
  SysTick->VAL = 0; /* Load the SysTick Counter Value */
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
  SysTick_CTRL_TICKINT_Msk |
  SysTick_CTRL_ENABLE_Msk; /* Enable SysTick IRQ and SysTick Timer */
  return (0); /* Function successful */
}

extern "C" {
extern void disk_timerproc(void);
volatile uint32_t _millis;
void SysTick_Handler(void) {
  ++_millis;
  disk_timerproc(); /* Disk timer process */
}
}

// runs after clock init and before global static constructors
extern "C" void SystemPostInit() {
  _millis = 0;     // initialise the millisecond counter value;
  SysTick_Config(SystemCoreClock / 1000);     // start millisecond global counter
  GPIO_SetDir(4, 1UL << 28, 1);

  for (int i = 0; i < 4; ++i) {
    GPIO_SetValue(4, 1UL << 28);
    delay(100);
    GPIO_ClearValue(4, 1UL << 28);
    delay(100);
  }
}

// detect 17x[4-8] (100MHz) or 17x9 (120MHz)
static bool isLPC1769() {
    #define IAP_LOCATION 0x1FFF1FF1
    uint32_t command[1];
    uint32_t result[5];
    typedef void (*IAP)(uint32_t*, uint32_t*);
    IAP iap = (IAP) IAP_LOCATION;

    command[0] = 54;
    iap(command, result);

    return ((result[1] & 0x00100000) != 0);
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
      TOGGLE(LED_PIN);     // Flash fast while USB initialisation completes
    #endif
  }

  #if NUM_SERIAL > 0
    MYSERIAL0.begin(BAUDRATE);
    #if NUM_SERIAL > 1
      MYSERIAL1.begin(BAUDRATE);
    #endif
    SERIAL_PRINTF("\n\n%s (%dMhz) UART0 Initialised\n", isLPC1769() ? "LPC1769" : "LPC1768", SystemCoreClock / 1000000);
    SERIAL_FLUSHTX();
  #endif

  HAL_timer_init();

  LPC1768_PWM_init();

  setup();
  while (true) {
    loop();
  }
}

#endif // TARGET_LPC1768
