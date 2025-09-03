/**
 * app_config.h is included by the hc32f460 arduino build script for every source file.
 * it is used to configure the arduino core (and ddl) automatically according
 * to the settings in Configuration.h and Configuration_adv.h.
 */
#pragma once
#ifndef _HC32_APP_CONFIG_H_
#define _HC32_APP_CONFIG_H_

#include "../../inc/MarlinConfigPre.h"

//
// dev mode
//
#if ENABLED(MARLIN_DEV_MODE)
  #define __DEBUG 1
  #define __CORE_DEBUG 1
#endif

//
// Fault Handlers and Panic
//

#if ENABLED(POSTMORTEM_DEBUGGING)
  // disable arduino core fault handler, as we define our own
  #define CORE_DISABLE_FAULT_HANDLER 1
#endif

// force-enable panic handler so that we can use our custom one (in MinSerial)
#define PANIC_ENABLE 1

// use short filenames in ddl debug and core panic output
#define __DEBUG_SHORT_FILENAMES 1
#define __PANIC_SHORT_FILENAMES 1

// omit panic messages in core panic output
#define __OMIT_PANIC_MESSAGE 1

//
// Usart
//

// disable serial globals (Serial1, Serial2, ...), as we define our own
#define DISABLE_SERIAL_GLOBALS 1

// increase the size of the Usart buffers (both RX and TX)
// NOTE:
// the heap usage will increase by (SERIAL_BUFFER_SIZE - 64) * "number of serial ports used"
// if running out of heap, the system may become unstable
//#define SERIAL_BUFFER_SIZE 256

// enable support for Usart Clock Divider / Oversampling auto config
#define USART_AUTO_CLKDIV_OS_CONFIG 1

// enable USART_RX_DMA_SUPPORT core option when SERIAL_DMA is enabled
#if ENABLED(SERIAL_DMA)
  #define USART_RX_DMA_SUPPORT 1
#endif

//
// Misc.
//

// redirect printf to host serial
#define REDIRECT_PRINTF_TO_SERIAL 1

// F_CPU must be known at compile time, but on HC32F460 it's not.
// Thus we assume HCLK to be 200MHz, as that's what is configured in
// 'core_hook_sysclock_init' in 'sysclock.cpp'.
// If you face issues with this assumption, please double-check with the values
// printed by 'MarlinHAL::HAL_clock_frequencies_dump'.
// see also: HAL_TIMER_RATE in timers.h
#define F_CPU 200000000 // 200MHz HCLK

#endif // _HC32_APP_CONFIG_H_
