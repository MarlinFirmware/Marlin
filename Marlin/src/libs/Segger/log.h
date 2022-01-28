#ifndef LOG_H
#define LOG_H

#ifdef MARLIN_CONFIG_MY
    #define DEBUG_TO_UART
#endif
//#define DEBUG_TO_SEGGER_RTT

#ifdef DEBUG_TO_UART
    #include <stdarg.h>
    #include "../../module/mks_wifi/debug_to_uart.h"
    #define DEBUG_MSG
    #define INFO_MSG
    #define WARNING_MSG
    #define ERROR_MSG
#endif

#ifdef DEBUG_TO_SEGGER_RTT
    #include "SEGGER_RTT.h"
    #define DEBUG_MSG
    #define INFO_MSG
    #define WARNING_MSG
    #define ERROR_MSG
#endif

#ifndef DEBUG_TO_SEGGER_RTT
    #ifndef DEBUG_TO_UART
        #include <stdarg.h>
        #define DEBUG(fmt, args...)
        #define INFO(fmt, args...)
        #define WARNING(fmt, args...)
        #define ERROR(fmt, args...)
    #endif
#endif

#ifdef DEBUG_TO_UART
    #ifdef DEBUG_MSG
        #define DEBUG(fmt, args...) debug_to_uart((char*)"[D] %-20s:%-4d :" fmt "\n", __func__, __LINE__, ## args)
        #define DEBUG_FLUSH() debug_to_uart_flush()
    #else
        #define DEBUG(fmt, args...)
        #define DEBUG_FLUSH()
    #endif
    #ifdef INFO_MSG
        #define INFO(fmt, args...)  debug_to_uart((char*)"[I] %-20s:%-4d :" fmt "\n", __func__, __LINE__, ## args)
    #else
        #define INFO(fmt, args...)
    #endif
    #ifdef WARNING_MSG
        #define WARNING(fmt, args...) debug_to_uart((char*)"[W] %-20s:%-4d :" fmt "\n",  __func__, __LINE__, ## args)
    #else
        #define WARNING(fmt, args...)
    #endif
    #ifdef ERROR_MSG
        #define ERROR(fmt, args...) debug_to_uart((char*)"[E] %-20s:%-4d :" fmt "\n",  __func__, __LINE__,## args)
    #else
        #define ERROR(fmt, args...)
    #endif

#endif

#ifdef DEBUG_TO_SEGGER_RTT

    #ifdef DEBUG_MSG
        #define DEBUG(fmt, args...) SEGGER_RTT_printf(0,"[D] %-20s:%-4d :" fmt "\r\n", __func__, __LINE__, ## args)
    #else
        #define DEBUG(fmt, args...)
    #endif
    #ifdef INFO_MSG
        #define INFO(fmt, args...)  SEGGER_RTT_printf(0,"[I] %-20s:%-4d :" fmt "\n", __func__, __LINE__, ## args)
    #else
        #define INFO(fmt, args...)
    #endif
    #ifdef WARNING_MSG
        #define WARNING(fmt, args...) SEGGER_RTT_printf(0,"[W] %-20s:%-4d :" fmt "\n",  __func__, __LINE__, ## args)
    #else
        #define WARNING(fmt, args...)
    #endif
    #ifdef ERROR_MSG
        #define ERROR(fmt, args...) SEGGER_RTT_printf(0,"[E] %-20s:%-4d :" fmt "\n",  __func__, __LINE__,## args)
    #else
        #define ERROR(fmt, args...)
    #endif

    #endif

#endif

