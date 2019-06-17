#if !(defined(NUM_DIGITAL_PINS) || defined(BOARD_NR_GPIO_PINS))
  #error "M43 not supported for this board"
#endif


// strange - STM32F4 comes to HAL_STM32 rather than HAL_STM32F4 for these files
#ifdef STM32F4
  #ifdef NUM_DIGITAL_PINS  //only present in ST's Arduino core (STM32duino, STM32Core)
    #include "pinsDebug_STM32duino.h"
  #elif defined( BOARD_NR_GPIO_PINS)  //only found in STM32GENERIC (Maple)
    #include "pinsDebug_STM32GENERIC.h"
  #else
    #error "M43 not supported for this board"
  #endif
#endif



