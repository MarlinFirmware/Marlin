#ifndef _HAL_PLATFORMS_H_
#define _HAL_PLATFORMS_H_

#ifndef HAL_PLATFORM

#ifdef __AVR__
  #define HAL_PLATFORM HAL_AVR
#elif defined(ARDUINO_ARCH_SAM)
  #define HAL_PLATFORM HAL_DUE
#elif defined(__MK64FX512__) || defined(__MK66FX1M0__)
  #define HAL_PLATFORM HAL_TEENSY35_36
#elif defined(TARGET_LPC1768)
  #define HAL_PLATFORM HAL_LPC1768
#elif defined(__STM32F1__) || defined(TARGET_STM32F1)
  #define HAL_PLATFORM HAL_STM32F1
#elif defined(STM32F4)
  #define HAL_PLATFORM HAL_STM32F4
#elif defined(STM32F7)
  #define HAL_PLATFORM HAL_STM32F7
#else
  #error "Unsupported Platform!"
#endif

#endif // HAL_PLATFORM

#define XSTR_(M) #M
#define XSTR(M) XSTR_(M)
#define HAL_PATH(PATH, NAME) XSTR(PATH/HAL_PLATFORM/NAME)

#endif // _HAL_PLATFORMS_H_
