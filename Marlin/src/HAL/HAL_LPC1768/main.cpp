#ifdef TARGET_LPC1768

#include <usb/usb.h>
#include <usb/usbcfg.h>
#include <usb/usbhw.h>
#include <usb/usbcore.h>
#include <usb/cdc.h>
#include <usb/cdcuser.h>
#include <usb/mscuser.h>
#include <CDCSerial.h>
#include <usb/mscuser.h>

extern "C" {
  #include <debug_frmwrk.h>
}

#include "../../sd/cardreader.h"
#include "../../inc/MarlinConfig.h"
#include "HAL.h"
#include "HAL_timers.h"

extern uint32_t MSC_SD_Init(uint8_t pdrv);
extern "C" int isLPC1769();
extern "C" void disk_timerproc(void);

void SysTick_Callback() {
  disk_timerproc();
}

void HAL_init() {

  // Support the 4 LEDs some LPC176x boards have
  #if PIN_EXISTS(LED)
    SET_DIR_OUTPUT(LED_PIN);
    WRITE_PIN_CLR(LED_PIN);
    #if PIN_EXISTS(LED2)
      SET_DIR_OUTPUT(LED2_PIN);
      WRITE_PIN_CLR(LED2_PIN);
      #if PIN_EXISTS(LED3)
        SET_DIR_OUTPUT(LED3_PIN);
        WRITE_PIN_CLR(LED3_PIN);
        #if PIN_EXISTS(LED4)
          SET_DIR_OUTPUT(LED4_PIN);
          WRITE_PIN_CLR(LED4_PIN);
        #endif
      #endif
    #endif

    // Flash status LED 3 times to indicate Marlin has started booting
    for (uint8_t i = 0; i < 6; ++i) {
      TOGGLE(LED_PIN);
      delay(100);
    }
  #endif

  //debug_frmwrk_init();
  //_DBG("\n\nDebug running\n");
  // Initialise the SD card chip select pins as soon as possible
  #if PIN_EXISTS(SS)
    WRITE(SS_PIN, HIGH);
    SET_OUTPUT(SS_PIN);
  #endif

  #if defined(ONBOARD_SD_CS) && ONBOARD_SD_CS > -1
    WRITE(ONBOARD_SD_CS, HIGH);
    SET_OUTPUT(ONBOARD_SD_CS);
  #endif

  USB_Init();                               // USB Initialization
  USB_Connect(FALSE);                       // USB clear connection
  delay(1000);                              // Give OS time to notice
  USB_Connect(TRUE);

  #if DISABLED(USB_SD_DISABLED)
    MSC_SD_Init(0);                         // Enable USB SD card access
  #endif

  const millis_t usb_timeout = millis() + 2000;
  while (!USB_Configuration && PENDING(millis(), usb_timeout)) {
    delay(50);
    HAL_idletask();
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
}

// HAL idle task
void HAL_idletask(void) {
  #if ENABLED(SDSUPPORT) && ENABLED(SHARED_SD_CARD)
    // If Marlin is using the SD card we need to lock it to prevent access from
    // a PC via USB.
    // Other HALs use IS_SD_PRINTING() and IS_SD_FILE_OPEN() to check for access but
    // this will not reliably detect delete operations. To be safe we will lock
    // the disk if Marlin has it mounted. Unfortuately there is currently no way
    // to unmount the disk from the LCD menu.
    // if (IS_SD_PRINTING() || IS_SD_FILE_OPEN())
    if (card.flag.cardOK)
      MSC_Aquire_Lock();
    else
      MSC_Release_Lock();
  #endif
  // Perform USB stack housekeeping
  MSC_RunDeferredCommands();
}

#endif // TARGET_LPC1768
