/*
 * Author: Jayant Tang
 * Email: jayant97@foxmail.com
 */

#ifndef _LOG_H_
#define _LOH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "SEGGER_RTT.h"

#define LOG_DEBUG 1

#if LOG_DEBUG


#define LOG_PROTO(type,color,format,...)            \
        SEGGER_RTT_printf(0,"%s%s"format"%s", \
                          color,                    \
                          type,                     \
                          ##__VA_ARGS__,            \
                          RTT_CTRL_RESET)

#define LOG_STR( x )                                    \
        SEGGER_RTT_printf(0, "%s%s",                \
                          RTT_CTRL_TEXT_BRIGHT_GREEN,   \
                          x ,                           \
                          RTT_CTRL_RESET)

/* 清屏*/
#define LOG_CLEAR() SEGGER_RTT_WriteString(0, "  "RTT_CTRL_CLEAR)

/* 无颜色日志输出 */
#define LOG(format,...) LOG_PROTO("","",format,##__VA_ARGS__)
// #define LOG_STR(format) LOG_PROTO("","",format)
/* 有颜色格式日志输出 */
#define LOGI(format,...) LOG_PROTO("", RTT_CTRL_TEXT_BRIGHT_GREEN , format, ##__VA_ARGS__)
#define LOGW(format,...) LOG_PROTO("", RTT_CTRL_TEXT_BRIGHT_YELLOW, format, ##__VA_ARGS__)
#define LOGE(format,...) LOG_PROTO("", RTT_CTRL_TEXT_BRIGHT_RED   , format, ##__VA_ARGS__)

void* log_hex_dump(uint8_t data[], uint8_t len, uint8_t buffer[]);
void* log_hex_dump_line(uint8_t data[], uint8_t len, uint8_t buffer[]);
void log_hex(char const* head, uint8_t data[], uint8_t len);
#else
#define LOG_CLEAR()
#define LOG
#define LOGI
#define LOGW
#define LOGE

#endif

#ifdef __cplusplus
}
#endif

#endif // !_LOG_H_