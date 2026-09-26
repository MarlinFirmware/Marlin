/*
 * MIT License
 * Copyright (c) 2017 - 2022 _VIFEXTech
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef __MCU_CONFIG_H
#define __MCU_CONFIG_H

/*=========================
   MCU configuration
 *=========================*/
/* System tick */
#define SYSTICK_TICK_FREQ                   1000 // Hz
#define SYSTICK_PRIORITY                    0

/* Hardware Serial */
#define SERIAL_RX_BUFFER_SIZE               128
#define SERIAL_PREEMPTIONPRIORITY_DEFAULT   1
#define SERIAL_SUBPRIORITY_DEFAULT          3
#define SERIAL_CONFIG_DEFAULT               SERIAL_8N1

#define SERIAL_1_ENABLE                     1
#if SERIAL_1_ENABLE
#  define SERIAL_1_USART                    USART1
#  define SERIAL_1_IRQ_HANDLER_DEF()        void USART1_IRQHandler(void)
#endif

#define SERIAL_2_ENABLE                     1
#if SERIAL_2_ENABLE
#  define SERIAL_2_USART                    USART2
#  define SERIAL_2_IRQ_HANDLER_DEF()        void USART2_IRQHandler(void)
#endif

#define SERIAL_3_ENABLE                     1
#if SERIAL_3_ENABLE
#  define SERIAL_3_USART                    USART3
#  define SERIAL_3_IRQ_HANDLER_DEF()        void USART3_IRQHandler(void)
#endif

/* Wire (Software I2C) */
#define WIRE_USE_FULL_SPEED_I2C             0


// #define WIRE_SDA_PIN                        PB7
// #define WIRE_SCL_PIN                        PB6


#define WIRE_DELAY                          0
#define WIRE_BEGIN_TIMEOUT                  100 // ms
#define WIRE_BUFF_SIZE                      32

/* SPI Class */
#define SPI_CLASS_AVR_COMPATIBILITY_MODE    1
#define SPI_CLASS_PIN_DEFINE_ENABLE         1



#define SPI_CLASS_1_ENABLE                  1
#if SPI_CLASS_1_ENABLE
#  define SPI_CLASS_1_SPI                   SPI1
#endif

#define SPI_CLASS_2_ENABLE                  1
#if SPI_CLASS_2_ENABLE
#  define SPI_CLASS_2_SPI                   SPI2
#endif

#define SPI_CLASS_3_ENABLE                  1
#if SPI_CLASS_3_ENABLE
#  define SPI_CLASS_3_SPI                   SPI3
#endif

/* WString */
#define WSTRING_MEM_INCLUDE                 <stdlib.h>
#define WSTRING_MEM_REALLOC                 realloc
#define WSTRING_MEM_FREE                    free

/* Print */
#define PRINT_PRINTF_BUFFER_LENGTH          128

/* GPIO */
#define GPIO_DRIVE_DEFAULT                  GPIO_DRIVE_STRENGTH_STRONGER

/* External Interrupt  */
#define EXTI_PREEMPTIONPRIORITY_DEFAULT     2
#define EXTI_SUBPRIORITY_DEFAULT            1

/* Timer Interrupt */
#define TIMER_PREEMPTIONPRIORITY_DEFAULT    0
#define TIMER_SUBPRIORITY_DEFAULT           3

/* Tone */
#define TONE_TIMER_DEFAULT                  TIM1
#define TONE_PREEMPTIONPRIORITY_DEFAULT     0
#define TONE_SUBPRIORITY_DEFAULT            1

/* PWM */
#define PWM_RESOLUTION_DEFAULT              1000
#define PWM_FREQUENCY_DEFAULT               10000

#endif
