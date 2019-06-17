#ifdef NUM_DIGITAL_PINS  //only present in ST's Arduino core (STM32duino, STM32Core)
  #include "../HAL_STM32/pinsDebug_STM32duino.h"
#elif defined( BOARD_NR_GPIO_PINS)  //only found in STM32GENERIC (Maple)
  #include "../HAL_STM32/pinsDebug_STM32GENERIC.h"
#else
  #error "M43 not supported for this board"
#endif



