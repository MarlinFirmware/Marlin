/**
  **************************************************************************
  * @file     at32f403a_407.h
  * @version  v2.0.9
  * @date     2022-04-25
  * @brief    at32f403a_407 header file
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */
#ifndef __AT32F403A_407_H
#define __AT32F403A_407_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined (__CC_ARM)
 #pragma anon_unions
#endif

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup AT32F403A_407
  * @{
  */

/** @addtogroup Library_configuration_section
  * @{
  */

/**
  * tip: to avoid modifying this file each time you need to switch between these
  *      devices, you can define the device in your toolchain compiler preprocessor.
  */

#if !defined (AT32F403AVCT7) && !defined (AT32F403ARCT7) && !defined (AT32F403ACCT7) && \
    !defined (AT32F403ACCU7) && !defined (AT32F403AVGT7) && !defined (AT32F403ACGT7) && \
    !defined (AT32F403ARGT7) && !defined (AT32F403ACGU7) && !defined (AT32F403AVET7) && \
    !defined (AT32F403ARET7) && !defined (AT32F403ACET7) && !defined (AT32F403ACEU7) && \
    !defined (AT32F407RGT7)  && !defined (AT32F407VGT7)  && !defined (AT32F407RCT7)  && \
    !defined (AT32F407VCT7)  && !defined (AT32F407RET7)  && !defined (AT32F407VET7)  && \
    !defined (AT32F407AVCT7) && !defined (AT32F407AVGT7)

    //#error "Please select first the target device used in your application (in at32f403a_407.h file)"
#endif

#if defined (AT32F403AVCT7) || defined (AT32F403ARCT7) || defined (AT32F403ACCT7) || \
    defined (AT32F403ACCU7) || defined (AT32F403AVGT7) || defined (AT32F403ACGT7) || \
    defined (AT32F403ARGT7) || defined (AT32F403ACGU7) || defined (AT32F403AVET7) || \
    defined (AT32F403ACET7) || defined (AT32F403ARET7) || defined (AT32F403ACEU7)

 #define AT32F403Axx
 #endif

#if defined (AT32F407RGT7)  || defined (AT32F407VGT7)  || defined (AT32F407RCT7)  || \
    defined (AT32F407VCT7)  || defined (AT32F407VET7)  || defined (AT32F407RET7)  || \
    defined (AT32F407AVCT7) || defined (AT32F407AVGT7)

    #define AT32F407xx
#endif

#ifndef USE_STDPERIPH_DRIVER
/**
  * @brief comment the line below if you will not use the peripherals drivers.
  * in this case, these drivers will not be included and the application code will
  * be based on direct access to peripherals registers
  */
  #ifdef _RTE_
    #include "RTE_Components.h"
    #ifdef RTE_DEVICE_STDPERIPH_FRAMEWORK
      #define USE_STDPERIPH_DRIVER
    #endif
  #endif
#endif

/**
  * @brief at32f403a_407 standard peripheral library version number
  */
#define __AT32F403A_407_LIBRARY_VERSION_MAJOR    (0x02) /*!< [31:24] major version */
#define __AT32F403A_407_LIBRARY_VERSION_MIDDLE   (0x00) /*!< [23:16] middle version */
#define __AT32F403A_407_LIBRARY_VERSION_MINOR    (0x09) /*!< [15:8]  minor version */
#define __AT32F403A_407_LIBRARY_VERSION_RC       (0x00) /*!< [7:0]  release candidate */
#define __AT32F403A_407_LIBRARY_VERSION          ((__AT32F403A_407_LIBRARY_VERSION_MAJOR << 24)  | \
                                                  (__AT32F403A_407_LIBRARY_VERSION_MIDDLE << 16) | \
                                                  (__AT32F403A_407_LIBRARY_VERSION_MINOR << 8)   | \
                                                  (__AT32F403A_407_LIBRARY_VERSION_RC))

/**
  * @}
  */

/** @addtogroup Configuration_section_for_CMSIS
  * @{
  */

/**
  * @brief configuration of the cortex-m4 processor and core peripherals
  */
#define __CM4_REV                 0x0001U  /*!< core revision r0p1                           */
#define __MPU_PRESENT             1        /*!< mpu present                                  */
#define __NVIC_PRIO_BITS          4        /*!< at32 uses 4 bits for the priority levels     */
#define __Vendor_SysTickConfig    0        /*!< set to 1 if different systick config is used */
#define __FPU_PRESENT             1U       /*!< fpu present                                  */

/**
  * @brief at32f403a_407 interrupt number definition, according to the selected device
  *        in @ref Library_configuration_section
  */
typedef enum IRQn
{
    /******  cortex-m4 processor exceptions numbers ***************************************************/
    Reset_IRQn                  = -15,    /*!< 1 reset vector, invoked on power up and warm reset   */
    NonMaskableInt_IRQn         = -14,    /*!< 2 non maskable interrupt                             */
    HardFault_IRQn              = -13,    /*!< 3 hard fault, all classes of fault                   */
    MemoryManagement_IRQn       = -12,    /*!< 4 cortex-m4 memory management interrupt              */
    BusFault_IRQn               = -11,    /*!< 5 cortex-m4 bus fault interrupt                      */
    UsageFault_IRQn             = -10,    /*!< 6 cortex-m4 usage fault interrupt                    */
    SVCall_IRQn                 = -5,     /*!< 11 cortex-m4 sv call interrupt                       */
    DebugMonitor_IRQn           = -4,     /*!< 12 cortex-m4 debug monitor interrupt                 */
    PendSV_IRQn                 = -2,     /*!< 14 cortex-m4 pend sv interrupt                       */
    SysTick_IRQn                = -1,     /*!< 15 cortex-m4 system tick interrupt                   */

    /******  at32 specific interrupt numbers *********************************************************/
    WWDT_IRQn                   = 0,      /*!< window watchdog timer interrupt                      */
    PVM_IRQn                    = 1,      /*!< pvm through exint line detection interrupt           */
    TAMPER_IRQn                 = 2,      /*!< tamper interrupt                                     */
    RTC_IRQn                    = 3,      /*!< rtc global interrupt                                 */
    FLASH_IRQn                  = 4,      /*!< flash global interrupt                               */
    CRM_IRQn                    = 5,      /*!< crm global interrupt                                 */
    EXINT0_IRQn                 = 6,      /*!< external line0 interrupt                             */
    EXINT1_IRQn                 = 7,      /*!< external line1 interrupt                             */
    EXINT2_IRQn                 = 8,      /*!< external line2 interrupt                             */
    EXINT3_IRQn                 = 9,      /*!< external line3 interrupt                             */
    EXINT4_IRQn                 = 10,     /*!< external line4 interrupt                             */
    DMA1_Channel1_IRQn          = 11,     /*!< dma1 channel 1 global interrupt                      */
    DMA1_Channel2_IRQn          = 12,     /*!< dma1 channel 2 global interrupt                      */
    DMA1_Channel3_IRQn          = 13,     /*!< dma1 channel 3 global interrupt                      */
    DMA1_Channel4_IRQn          = 14,     /*!< dma1 channel 4 global interrupt                      */
    DMA1_Channel5_IRQn          = 15,     /*!< dma1 channel 5 global interrupt                      */
    DMA1_Channel6_IRQn          = 16,     /*!< dma1 channel 6 global interrupt                      */
    DMA1_Channel7_IRQn          = 17,     /*!< dma1 channel 7 global interrupt                      */

#if defined (AT32F403Axx)
    ADC1_2_IRQn                 = 18,     /*!< adc1 and adc2 global interrupt                       */
    USBFS_H_CAN1_TX_IRQn        = 19,     /*!< usb device high priority or can1 tx interrupts       */
    USBFS_L_CAN1_RX0_IRQn       = 20,     /*!< usb device low priority or can1 rx0 interrupts       */
    CAN1_RX1_IRQn               = 21,     /*!< can1 rx1 interrupt                                   */
    CAN1_SE_IRQn                = 22,     /*!< can1 se interrupt                                    */
    EXINT9_5_IRQn               = 23,     /*!< external line[9:5] interrupts                        */
    TMR1_BRK_TMR9_IRQn          = 24,     /*!< tmr1 brake interrupt                                 */
    TMR1_OVF_TMR10_IRQn         = 25,     /*!< tmr1 overflow interrupt                              */
    TMR1_TRG_HALL_TMR11_IRQn    = 26,     /*!< tmr1 trigger and hall interrupt                      */
    TMR1_CH_IRQn                = 27,     /*!< tmr1 channel interrupt                               */
    TMR2_GLOBAL_IRQn            = 28,     /*!< tmr2 global interrupt                                */
    TMR3_GLOBAL_IRQn            = 29,     /*!< tmr3 global interrupt                                */
    TMR4_GLOBAL_IRQn            = 30,     /*!< tmr4 global interrupt                                */
    I2C1_EVT_IRQn               = 31,     /*!< i2c1 event interrupt                                 */
    I2C1_ERR_IRQn               = 32,     /*!< i2c1 error interrupt                                 */
    I2C2_EVT_IRQn               = 33,     /*!< i2c2 event interrupt                                 */
    I2C2_ERR_IRQn               = 34,     /*!< i2c2 error interrupt                                 */
    SPI1_IRQn                   = 35,     /*!< spi1 global interrupt                                */
    SPI2_I2S2EXT_IRQn           = 36,     /*!< spi2 global interrupt                                */
    USART1_IRQn                 = 37,     /*!< usart1 global interrupt                              */
    USART2_IRQn                 = 38,     /*!< usart2 global interrupt                              */
    USART3_IRQn                 = 39,     /*!< usart3 global interrupt                              */
    EXINT15_10_IRQn             = 40,     /*!< external line[15:10] interrupts                      */
    RTCAlarm_IRQn               = 41,     /*!< rtc alarm through exint line interrupt               */
    USBFSWakeUp_IRQn            = 42,     /*!< usb device wakeup from suspend through exint line interrupt */
    TMR8_BRK_TMR12_IRQn         = 43,     /*!< tmr8 brake interrupt                                 */
    TMR8_OVF_TMR13_IRQn         = 44,     /*!< tmr8 overflow interrupt                              */
    TMR8_TRG_HALL_TMR14_IRQn    = 45,     /*!< tmr8 trigger and hall interrupt                      */
    TMR8_CH_IRQn                = 46,     /*!< tmr8 channel interrupt                               */
    ADC3_IRQn                   = 47,     /*!< adc3 global interrupt                                */
    XMC_IRQn                    = 48,     /*!< xmc global interrupt                                 */
    SDIO1_IRQn                  = 49,     /*!< sdio1 global interrupt                               */
    TMR5_GLOBAL_IRQn            = 50,     /*!< tmr5 global interrupt                                */
    SPI3_I2S3EXT_IRQn           = 51,     /*!< spi3 global interrupt                                */
    UART4_IRQn                  = 52,     /*!< uart4 global interrupt                               */
    UART5_IRQn                  = 53,     /*!< uart5 global interrupt                               */
    TMR6_GLOBAL_IRQn            = 54,     /*!< tmr6 global interrupt                                */
    TMR7_GLOBAL_IRQn            = 55,     /*!< tmr7 global interrupt                                */
    DMA2_Channel1_IRQn          = 56,     /*!< dma2 channel 1 global interrupt                      */
    DMA2_Channel2_IRQn          = 57,     /*!< dma2 channel 2 global interrupt                      */
    DMA2_Channel3_IRQn          = 58,     /*!< dma2 channel 3 global interrupt                      */
    DMA2_Channel4_5_IRQn        = 59,     /*!< dma2 channel 4 and channel 5 global interrupt        */
    SDIO2_IRQn                  = 60,     /*!< sdio2 global interrupt                               */
    I2C3_EVT_IRQn               = 61,     /*!< i2c3 event interrupt                                 */
    I2C3_ERR_IRQn               = 62,     /*!< i2c3 error interrupt                                 */
    SPI4_IRQn                   = 63,     /*!< spi4 global interrupt                                */
    CAN2_TX_IRQn                = 68,     /*!< can2 tx interrupt                                    */
    CAN2_RX0_IRQn               = 69,     /*!< can2 rx0 interrupt                                   */
    CAN2_RX1_IRQn               = 70,     /*!< can2 rx1 interrupt                                   */
    CAN2_SE_IRQn                = 71,     /*!< can2 se interrupt                                    */
    ACC_IRQn                    = 72,     /*!< acc interrupt                                        */
    USBFS_MAPH_IRQn             = 73,     /*!< usb map hp interrupt                                 */
    USBFS_MAPL_IRQn             = 74,     /*!< usb map lp interrupt                                 */
    DMA2_Channel6_7_IRQn        = 75,     /*!< dma2 channel 6 and channel 7 global interrupt        */
    USART6_IRQn                 = 76,     /*!< usart6 interrupt                                     */
    UART7_IRQn                  = 77,     /*!< uart7 interrupt                                      */
    UART8_IRQn                  = 78,     /*!< uart8 interrupt                                      */
#endif

#if defined (AT32F407xx)
    ADC1_2_IRQn                 = 18,     /*!< adc1 and adc2 global interrupt                       */
    USBFS_H_CAN1_TX_IRQn        = 19,     /*!< usb device high priority or can1 tx interrupts       */
    USBFS_L_CAN1_RX0_IRQn       = 20,     /*!< usb device low priority or can1 rx0 interrupts       */
    CAN1_RX1_IRQn               = 21,     /*!< can1 rx1 interrupt                                   */
    CAN1_SE_IRQn                = 22,     /*!< can1 se interrupt                                    */
    EXINT9_5_IRQn               = 23,     /*!< external line[9:5] interrupts                        */
    TMR1_BRK_TMR9_IRQn          = 24,     /*!< tmr1 brake interrupt                                 */
    TMR1_OVF_TMR10_IRQn         = 25,     /*!< tmr1 overflow interrupt                              */
    TMR1_TRG_HALL_TMR11_IRQn    = 26,     /*!< tmr1 trigger and hall interrupt                      */
    TMR1_CH_IRQn                = 27,     /*!< tmr1 channel interrupt                               */
    TMR2_GLOBAL_IRQn            = 28,     /*!< tmr2 global interrupt                                */
    TMR3_GLOBAL_IRQn            = 29,     /*!< tmr3 global interrupt                                */
    TMR4_GLOBAL_IRQn            = 30,     /*!< tmr4 global interrupt                                */
    I2C1_EVT_IRQn               = 31,     /*!< i2c1 event interrupt                                 */
    I2C1_ERR_IRQn               = 32,     /*!< i2c1 error interrupt                                 */
    I2C2_EVT_IRQn               = 33,     /*!< i2c2 event interrupt                                 */
    I2C2_ERR_IRQn               = 34,     /*!< i2c2 error interrupt                                 */
    SPI1_IRQn                   = 35,     /*!< spi1 global interrupt                                */
    SPI2_I2S2EXT_IRQn           = 36,     /*!< spi2 global interrupt                                */
    USART1_IRQn                 = 37,     /*!< usart1 global interrupt                              */
    USART2_IRQn                 = 38,     /*!< usart2 global interrupt                              */
    USART3_IRQn                 = 39,     /*!< usart3 global interrupt                              */
    EXINT15_10_IRQn             = 40,     /*!< external line[15:10] interrupts                      */
    RTCAlarm_IRQn               = 41,     /*!< rtc alarm through exint line interrupt               */
    USBFSWakeUp_IRQn            = 42,     /*!< usb device wakeup from suspend through exint line interrupt */
    TMR8_BRK_TMR12_IRQn         = 43,     /*!< tmr8 brake interrupt                                 */
    TMR8_OVF_TMR13_IRQn         = 44,     /*!< tmr8 overflow interrupt                              */
    TMR8_TRG_HALL_TMR14_IRQn    = 45,     /*!< tmr8 trigger and hall interrupt                      */
    TMR8_CH_IRQn                = 46,     /*!< tmr8 channel interrupt                               */
    ADC3_IRQn                   = 47,     /*!< adc3 global interrupt                                */
    XMC_IRQn                    = 48,     /*!< xmc global interrupt                                 */
    SDIO1_IRQn                  = 49,     /*!< sdio1 global interrupt                               */
    TMR5_GLOBAL_IRQn            = 50,     /*!< tmr5 global interrupt                                */
    SPI3_I2S3EXT_IRQn           = 51,     /*!< spi3 global interrupt                                */
    UART4_IRQn                  = 52,     /*!< uart4 global interrupt                               */
    UART5_IRQn                  = 53,     /*!< uart5 global interrupt                               */
    TMR6_GLOBAL_IRQn            = 54,     /*!< tmr6 global interrupt                                */
    TMR7_GLOBAL_IRQn            = 55,     /*!< tmr7 global interrupt                                */
    DMA2_Channel1_IRQn          = 56,     /*!< dma2 channel 1 global interrupt                      */
    DMA2_Channel2_IRQn          = 57,     /*!< dma2 channel 2 global interrupt                      */
    DMA2_Channel3_IRQn          = 58,     /*!< dma2 channel 3 global interrupt                      */
    DMA2_Channel4_5_IRQn        = 59,     /*!< dma2 channel 4 and channel 5 global interrupt        */
    SDIO2_IRQn                  = 60,     /*!< sdio2 global interrupt                               */
    I2C3_EVT_IRQn               = 61,     /*!< i2c3 event interrupt                                 */
    I2C3_ERR_IRQn               = 62,     /*!< i2c3 error interrupt                                 */
    SPI4_IRQn                   = 63,     /*!< spi4 global interrupt                                */
    CAN2_TX_IRQn                = 68,     /*!< can2 tx interrupt                                    */
    CAN2_RX0_IRQn               = 69,     /*!< can2 rx0 interrupt                                   */
    CAN2_RX1_IRQn               = 70,     /*!< can2 rx1 interrupt                                   */
    CAN2_SE_IRQn                = 71,     /*!< can2 se interrupt                                    */
    ACC_IRQn                    = 72,     /*!< acc interrupt                                        */
    USBFS_MAPH_IRQn             = 73,     /*!< usb map hp interrupt                                 */
    USBFS_MAPL_IRQn             = 74,     /*!< usb map lp interrupt                                 */
    DMA2_Channel6_7_IRQn        = 75,     /*!< dma2 channel 6 and channel 7 global interrupt        */
    USART6_IRQn                 = 76,     /*!< usart6 interrupt                                     */
    UART7_IRQn                  = 77,     /*!< uart7 interrupt                                      */
    UART8_IRQn                  = 78,     /*!< uart8 interrupt                                      */
    EMAC_IRQn                   = 79,     /*!< emac interrupt                                       */
    EMAC_WKUP_IRQn              = 80,     /*!< emac wakeup interrupt                                */
#endif

} IRQn_Type;

/**
  * @}
  */

#include "core_cm4.h"
#include "system_at32f403a_407.h"
#include <stdint.h>

/** @addtogroup Exported_types
  * @{
  */

typedef int32_t  INT32;
typedef int16_t  INT16;
typedef int8_t   INT8;
typedef uint32_t UINT32;
typedef uint16_t UINT16;
typedef uint8_t  UINT8;

typedef int32_t  s32;
typedef int16_t  s16;
typedef int8_t   s8;

typedef const int32_t sc32;   /*!< read only */
typedef const int16_t sc16;   /*!< read only */
typedef const int8_t  sc8;    /*!< read only */

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef __I int32_t vsc32;    /*!< read only */
typedef __I int16_t vsc16;    /*!< read only */
typedef __I int8_t  vsc8;     /*!< read only */

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef const uint32_t uc32;  /*!< read only */
typedef const uint16_t uc16;  /*!< read only */
typedef const uint8_t  uc8;   /*!< read only */

typedef __IO uint32_t vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef __I uint32_t vuc32;   /*!< read only */
typedef __I uint16_t vuc16;   /*!< read only */
typedef __I uint8_t  vuc8;    /*!< read only */

/**
  * @brief flag status
  */
typedef enum {RESET = 0, SET = !RESET} flag_status;

/**
  * @brief confirm state
  */
typedef enum {FALSE = 0, TRUE = !FALSE} confirm_state;

/**
  * @brief error status
  */
typedef enum {ERROR = 0, SUCCESS = !ERROR} error_status;

/**
  * @}
  */

/** @addtogroup Exported_macro
  * @{
  */

#define REG8(addr)                       *(volatile uint8_t *)(addr)
#define REG16(addr)                      *(volatile uint16_t *)(addr)
#define REG32(addr)                      *(volatile uint32_t *)(addr)

#define MAKE_VALUE(reg_offset, bit_num)  (uint32_t)(((reg_offset) << 16) | (bit_num & 0x1F))

#define PERIPH_REG(periph_base, value)   REG32((periph_base + (value >> 16)))
#define PERIPH_REG_BIT(value)            (0x1U << (value & 0x1F))

/**
  * @}
  */

/** @addtogroup Peripheral_memory_map
  * @{
  */

#define FLASH_BASE                       ((uint32_t)0x08000000)
#define SPIM_FLASH_BASE                  ((uint32_t)0x08400000)
#define USD_BASE                         ((uint32_t)0x1FFFF800)
#define SRAM_BASE                        ((uint32_t)0x20000000)
#define PERIPH_BASE                      ((uint32_t)0x40000000)
#define XMC_REG_BASE                     ((uint32_t)0xA0000000)
#define DEBUG_BASE                       ((uint32_t)0xE0042000)

#define APB1PERIPH_BASE                  (PERIPH_BASE + 0x00000)
#define APB2PERIPH_BASE                  (PERIPH_BASE + 0x10000)
#define AHBPERIPH_BASE                   (PERIPH_BASE + 0x20000)

#if defined (AT32F403Axx)
/* apb1 bus base address */
#define TMR2_BASE                        (APB1PERIPH_BASE + 0x0000)
#define TMR3_BASE                        (APB1PERIPH_BASE + 0x0400)
#define TMR4_BASE                        (APB1PERIPH_BASE + 0x0800)
#define TMR5_BASE                        (APB1PERIPH_BASE + 0x0C00)
#define TMR6_BASE                        (APB1PERIPH_BASE + 0x1000)
#define TMR7_BASE                        (APB1PERIPH_BASE + 0x1400)
#define TMR12_BASE                       (APB1PERIPH_BASE + 0x1800)
#define TMR13_BASE                       (APB1PERIPH_BASE + 0x1C00)
#define TMR14_BASE                       (APB1PERIPH_BASE + 0x2000)
#define RTC_BASE                         (APB1PERIPH_BASE + 0x2800)
#define WWDT_BASE                        (APB1PERIPH_BASE + 0x2C00)
#define WDT_BASE                         (APB1PERIPH_BASE + 0x3000)
#define SPI2_BASE                        (APB1PERIPH_BASE + 0x3800)
#define SPI3_BASE                        (APB1PERIPH_BASE + 0x3C00)
#define SPI4_BASE                        (APB1PERIPH_BASE + 0x4000)
#define USART2_BASE                      (APB1PERIPH_BASE + 0x4400)
#define USART3_BASE                      (APB1PERIPH_BASE + 0x4800)
#define UART4_BASE                       (APB1PERIPH_BASE + 0x4C00)
#define UART5_BASE                       (APB1PERIPH_BASE + 0x5000)
#define I2C1_BASE                        (APB1PERIPH_BASE + 0x5400)
#define I2C2_BASE                        (APB1PERIPH_BASE + 0x5800)
#define USBFS_BASE                       (APB1PERIPH_BASE + 0x5C00)
#define CAN1_BASE                        (APB1PERIPH_BASE + 0x6400)
#define CAN2_BASE                        (APB1PERIPH_BASE + 0x6800)
#define BPR_BASE                         (APB1PERIPH_BASE + 0x6C00)
#define PWC_BASE                         (APB1PERIPH_BASE + 0x7000)
#define DAC_BASE                         (APB1PERIPH_BASE + 0x7400)
/* apb2 bus base address */
#define IOMUX_BASE                       (APB2PERIPH_BASE + 0x0000)
#define EXINT_BASE                       (APB2PERIPH_BASE + 0x0400)
#define GPIOA_BASE                       (APB2PERIPH_BASE + 0x0800)
#define GPIOB_BASE                       (APB2PERIPH_BASE + 0x0C00)
#define GPIOC_BASE                       (APB2PERIPH_BASE + 0x1000)
#define GPIOD_BASE                       (APB2PERIPH_BASE + 0x1400)
#define GPIOE_BASE                       (APB2PERIPH_BASE + 0x1800)
#define ADC1_BASE                        (APB2PERIPH_BASE + 0x2400)
#define ADC2_BASE                        (APB2PERIPH_BASE + 0x2800)
#define TMR1_BASE                        (APB2PERIPH_BASE + 0x2C00)
#define SPI1_BASE                        (APB2PERIPH_BASE + 0x3000)
#define TMR8_BASE                        (APB2PERIPH_BASE + 0x3400)
#define USART1_BASE                      (APB2PERIPH_BASE + 0x3800)
#define ADC3_BASE                        (APB2PERIPH_BASE + 0x3C00)
#define TMR9_BASE                        (APB2PERIPH_BASE + 0x4C00)
#define TMR10_BASE                       (APB2PERIPH_BASE + 0x5000)
#define TMR11_BASE                       (APB2PERIPH_BASE + 0x5400)
#define ACC_BASE                         (APB2PERIPH_BASE + 0x5800)
#define I2C3_BASE                        (APB2PERIPH_BASE + 0x5C00)
#define USART6_BASE                      (APB2PERIPH_BASE + 0x6000)
#define UART7_BASE                       (APB2PERIPH_BASE + 0x6400)
#define UART8_BASE                       (APB2PERIPH_BASE + 0x6800)
#define I2S2EXT_BASE                     (APB2PERIPH_BASE + 0x6C00)
#define I2S3EXT_BASE                     (APB2PERIPH_BASE + 0x7000)
#define SDIO1_BASE                       (APB2PERIPH_BASE + 0x8000)
/* ahb bus base address */
#define DMA1_BASE                        (AHBPERIPH_BASE + 0x0000)
#define DMA1_CHANNEL1_BASE               (AHBPERIPH_BASE + 0x0008)
#define DMA1_CHANNEL2_BASE               (AHBPERIPH_BASE + 0x001C)
#define DMA1_CHANNEL3_BASE               (AHBPERIPH_BASE + 0x0030)
#define DMA1_CHANNEL4_BASE               (AHBPERIPH_BASE + 0x0044)
#define DMA1_CHANNEL5_BASE               (AHBPERIPH_BASE + 0x0058)
#define DMA1_CHANNEL6_BASE               (AHBPERIPH_BASE + 0x006C)
#define DMA1_CHANNEL7_BASE               (AHBPERIPH_BASE + 0x0080)
#define DMA2_BASE                        (AHBPERIPH_BASE + 0x0400)
#define DMA2_CHANNEL1_BASE               (AHBPERIPH_BASE + 0x0408)
#define DMA2_CHANNEL2_BASE               (AHBPERIPH_BASE + 0x041C)
#define DMA2_CHANNEL3_BASE               (AHBPERIPH_BASE + 0x0430)
#define DMA2_CHANNEL4_BASE               (AHBPERIPH_BASE + 0x0444)
#define DMA2_CHANNEL5_BASE               (AHBPERIPH_BASE + 0x0458)
#define DMA2_CHANNEL6_BASE               (AHBPERIPH_BASE + 0x046C)
#define DMA2_CHANNEL7_BASE               (AHBPERIPH_BASE + 0x0480)
#define CRM_BASE                         (AHBPERIPH_BASE + 0x1000)
#define FLASH_REG_BASE                   (AHBPERIPH_BASE + 0x2000)
#define CRC_BASE                         (AHBPERIPH_BASE + 0x3000)
#define SDIO2_BASE                       (AHBPERIPH_BASE + 0x3400)
#define XMC_BANK1_REG_BASE               (XMC_REG_BASE + 0x0000)
#define XMC_BANK1E_REG_BASE              (XMC_REG_BASE + 0x0104)
#define XMC_BANK1E_H_BASE                (XMC_REG_BASE + 0x0220)
#define XMC_BANK2_REG_BASE               (XMC_REG_BASE + 0x0060)
#define XMC_BANK3_REG_BASE               (XMC_REG_BASE + 0x0080)
#define XMC_BANK4_REG_BASE               (XMC_REG_BASE + 0x00A0)
#endif

#if defined (AT32F407xx)
/* apb1 bus base address */
#define TMR2_BASE                        (APB1PERIPH_BASE + 0x0000)
#define TMR3_BASE                        (APB1PERIPH_BASE + 0x0400)
#define TMR4_BASE                        (APB1PERIPH_BASE + 0x0800)
#define TMR5_BASE                        (APB1PERIPH_BASE + 0x0C00)
#define TMR6_BASE                        (APB1PERIPH_BASE + 0x1000)
#define TMR7_BASE                        (APB1PERIPH_BASE + 0x1400)
#define TMR12_BASE                       (APB1PERIPH_BASE + 0x1800)
#define TMR13_BASE                       (APB1PERIPH_BASE + 0x1C00)
#define TMR14_BASE                       (APB1PERIPH_BASE + 0x2000)
#define RTC_BASE                         (APB1PERIPH_BASE + 0x2800)
#define WWDT_BASE                        (APB1PERIPH_BASE + 0x2C00)
#define WDT_BASE                         (APB1PERIPH_BASE + 0x3000)
#define SPI2_BASE                        (APB1PERIPH_BASE + 0x3800)
#define SPI3_BASE                        (APB1PERIPH_BASE + 0x3C00)
#define SPI4_BASE                        (APB1PERIPH_BASE + 0x4000)
#define USART2_BASE                      (APB1PERIPH_BASE + 0x4400)
#define USART3_BASE                      (APB1PERIPH_BASE + 0x4800)
#define UART4_BASE                       (APB1PERIPH_BASE + 0x4C00)
#define UART5_BASE                       (APB1PERIPH_BASE + 0x5000)
#define I2C1_BASE                        (APB1PERIPH_BASE + 0x5400)
#define I2C2_BASE                        (APB1PERIPH_BASE + 0x5800)
#define USBFS_BASE                       (APB1PERIPH_BASE + 0x5C00)
#define CAN1_BASE                        (APB1PERIPH_BASE + 0x6400)
#define CAN2_BASE                        (APB1PERIPH_BASE + 0x6800)
#define BPR_BASE                         (APB1PERIPH_BASE + 0x6C00)
#define PWC_BASE                         (APB1PERIPH_BASE + 0x7000)
#define DAC_BASE                         (APB1PERIPH_BASE + 0x7400)
/* apb2 bus base address */
#define IOMUX_BASE                       (APB2PERIPH_BASE + 0x0000)
#define EXINT_BASE                       (APB2PERIPH_BASE + 0x0400)
#define GPIOA_BASE                       (APB2PERIPH_BASE + 0x0800)
#define GPIOB_BASE                       (APB2PERIPH_BASE + 0x0C00)
#define GPIOC_BASE                       (APB2PERIPH_BASE + 0x1000)
#define GPIOD_BASE                       (APB2PERIPH_BASE + 0x1400)
#define GPIOE_BASE                       (APB2PERIPH_BASE + 0x1800)
#define ADC1_BASE                        (APB2PERIPH_BASE + 0x2400)
#define ADC2_BASE                        (APB2PERIPH_BASE + 0x2800)
#define TMR1_BASE                        (APB2PERIPH_BASE + 0x2C00)
#define SPI1_BASE                        (APB2PERIPH_BASE + 0x3000)
#define TMR8_BASE                        (APB2PERIPH_BASE + 0x3400)
#define USART1_BASE                      (APB2PERIPH_BASE + 0x3800)
#define ADC3_BASE                        (APB2PERIPH_BASE + 0x3C00)
#define TMR9_BASE                        (APB2PERIPH_BASE + 0x4C00)
#define TMR10_BASE                       (APB2PERIPH_BASE + 0x5000)
#define TMR11_BASE                       (APB2PERIPH_BASE + 0x5400)
#define ACC_BASE                         (APB2PERIPH_BASE + 0x5800)
#define I2C3_BASE                        (APB2PERIPH_BASE + 0x5C00)
#define USART6_BASE                      (APB2PERIPH_BASE + 0x6000)
#define UART7_BASE                       (APB2PERIPH_BASE + 0x6400)
#define UART8_BASE                       (APB2PERIPH_BASE + 0x6800)
#define I2S2EXT_BASE                     (APB2PERIPH_BASE + 0x6C00)
#define I2S3EXT_BASE                     (APB2PERIPH_BASE + 0x7000)
#define SDIO1_BASE                       (APB2PERIPH_BASE + 0x8000)
/* ahb bus base address */
#define DMA1_BASE                        (AHBPERIPH_BASE + 0x0000)
#define DMA1_CHANNEL1_BASE               (AHBPERIPH_BASE + 0x0008)
#define DMA1_CHANNEL2_BASE               (AHBPERIPH_BASE + 0x001C)
#define DMA1_CHANNEL3_BASE               (AHBPERIPH_BASE + 0x0030)
#define DMA1_CHANNEL4_BASE               (AHBPERIPH_BASE + 0x0044)
#define DMA1_CHANNEL5_BASE               (AHBPERIPH_BASE + 0x0058)
#define DMA1_CHANNEL6_BASE               (AHBPERIPH_BASE + 0x006C)
#define DMA1_CHANNEL7_BASE               (AHBPERIPH_BASE + 0x0080)
#define DMA2_BASE                        (AHBPERIPH_BASE + 0x0400)
#define DMA2_CHANNEL1_BASE               (AHBPERIPH_BASE + 0x0408)
#define DMA2_CHANNEL2_BASE               (AHBPERIPH_BASE + 0x041C)
#define DMA2_CHANNEL3_BASE               (AHBPERIPH_BASE + 0x0430)
#define DMA2_CHANNEL4_BASE               (AHBPERIPH_BASE + 0x0444)
#define DMA2_CHANNEL5_BASE               (AHBPERIPH_BASE + 0x0458)
#define DMA2_CHANNEL6_BASE               (AHBPERIPH_BASE + 0x046C)
#define DMA2_CHANNEL7_BASE               (AHBPERIPH_BASE + 0x0480)
#define CRM_BASE                         (AHBPERIPH_BASE + 0x1000)
#define FLASH_REG_BASE                   (AHBPERIPH_BASE + 0x2000)
#define CRC_BASE                         (AHBPERIPH_BASE + 0x3000)
#define SDIO2_BASE                       (AHBPERIPH_BASE + 0x3400)
#define EMAC_BASE                        (AHBPERIPH_BASE + 0x8000)
#define XMC_BANK1_REG_BASE               (XMC_REG_BASE + 0x0000)
#define XMC_BANK2_REG_BASE               (XMC_REG_BASE + 0x0060)
#define EMAC_MMC_BASE                    (EMAC_BASE + 0x0100)
#define EMAC_PTP_BASE                    (EMAC_BASE + 0x0700)
#define EMAC_DMA_BASE                    (EMAC_BASE + 0x1000)
#endif

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#include "at32f403a_407_def.h"
#include "at32f403a_407_conf.h"

#ifdef __cplusplus
}
#endif

#endif
