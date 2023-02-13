/*******************************************************************************
 * Copyright (C) 2016, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software is owned and published by:
 * Huada Semiconductor Co., Ltd. ("HDSC").
 *
 * BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
 * BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
 *
 * This software contains source code for use with HDSC
 * components. This software is licensed by HDSC to be adapted only
 * for use in systems utilizing HDSC components. HDSC shall not be
 * responsible for misuse or illegal use of this software for devices not
 * supported herein. HDSC is providing this software "AS IS" and will
 * not be responsible for issues arising from incorrect user implementation
 * of the software.
 *
 * Disclaimer:
 * HDSC MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
 * REGARDING THE SOFTWARE (INCLUDING ANY ACCOMPANYING WRITTEN MATERIALS),
 * ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING,
 * WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED
 * WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED
 * WARRANTY OF NONINFRINGEMENT.
 * HDSC SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT,
 * NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT
 * LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION,
 * LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR
 * INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA,
 * SAVINGS OR PROFITS,
 * EVEN IF Disclaimer HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
 * INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED
 * FROM, THE SOFTWARE.
 *
 * This software may be replicated in part or whole for the licensed use,
 * with the restriction that this Disclaimer and Copyright notice must be
 * included with each copy of this software, whether used in part or whole,
 * at all times.
 */
/******************************************************************************/
/** \file hc32f46x_icg.h
 **
 ** A detailed description is available at
 ** @link IcgGroup Initialize configure description @endlink
 **
 **   - 2018-10-15  1.0  yangjp First version for Device Driver Library of ICG.
 **
 ******************************************************************************/
#ifndef __HC32F46x_ICG_H__
#define __HC32F46x_ICG_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"

#if (DDL_ICG_ENABLE == DDL_ON)

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
 *******************************************************************************
 ** \defgroup IcgGroup Initialize Configure(ICG)
 **
 ******************************************************************************/
//@{

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief SWDT running state after reset
 ******************************************************************************/
#define SWDT_AUTO_START_AFTER_RESET ((uint16_t)0x0000) ///< SWDT Auto Start after reset
#define SWDT_STOP_AFTER_RESET ((uint16_t)0x0001)       ///< SWDT stop after reset

/**
 *******************************************************************************
 ** \brief SWDT count underflow or refresh error trigger event type
 ******************************************************************************/
#define SWDT_INTERRUPT_REQUEST ((uint16_t)0x0000) ///< WDT trigger interrupt request
#define SWDT_RESET_REQUEST ((uint16_t)0x0002)     ///< WDT trigger reset request

/**
 *******************************************************************************
 ** \brief SWDT count underflow cycle
 ******************************************************************************/
#define SWDT_COUNT_UNDERFLOW_CYCLE_256 ((uint16_t)0x0000)   ///< 256 clock cycle
#define SWDT_COUNT_UNDERFLOW_CYCLE_4096 ((uint16_t)0x0004)  ///< 4096 clock cycle
#define SWDT_COUNT_UNDERFLOW_CYCLE_16384 ((uint16_t)0x0008) ///< 16384 clock cycle
#define SWDT_COUNT_UNDERFLOW_CYCLE_65536 ((uint16_t)0x000C) ///< 65536 clock cycle

/**
 *******************************************************************************
 ** \brief SWDT count clock division
 ******************************************************************************/
#define SWDT_COUNT_SWDTCLK_DIV1 ((uint16_t)0x0000)    ///< SWDTCLK
#define SWDT_COUNT_SWDTCLK_DIV16 ((uint16_t)0x0040)   ///< SWDTCLK/16
#define SWDT_COUNT_SWDTCLK_DIV32 ((uint16_t)0x0050)   ///< SWDTCLK/32
#define SWDT_COUNT_SWDTCLK_DIV64 ((uint16_t)0x0060)   ///< SWDTCLK/64
#define SWDT_COUNT_SWDTCLK_DIV128 ((uint16_t)0x0070)  ///< SWDTCLK/128
#define SWDT_COUNT_SWDTCLK_DIV256 ((uint16_t)0x0080)  ///< SWDTCLK/256
#define SWDT_COUNT_SWDTCLK_DIV2048 ((uint16_t)0x00B0) ///< SWDTCLK/2048

/**
 *******************************************************************************
 ** \brief SWDT allow refresh percent range
 ******************************************************************************/
#define SWDT_100PCT ((uint16_t)0x0000)               ///< 100%
#define SWDT_0To25PCT ((uint16_t)0x0100)             ///< 0%~25%
#define SWDT_25To50PCT ((uint16_t)0x0200)            ///< 25%~50%
#define SWDT_0To50PCT ((uint16_t)0x0300)             ///< 0%~50%
#define SWDT_50To75PCT ((uint16_t)0x0400)            ///< 50%~75%
#define SWDT_0To25PCT_50To75PCT ((uint16_t)0x0500)   ///< 0%~25% & 50%~75%
#define SWDT_25To75PCT ((uint16_t)0x0600)            ///< 25%~75%
#define SWDT_0To75PCT ((uint16_t)0x0700)             ///< 0%~75%
#define SWDT_75To100PCT ((uint16_t)0x0800)           ///< 75%~100%
#define SWDT_0To25PCT_75To100PCT ((uint16_t)0x0900)  ///< 0%~25% & 75%~100%
#define SWDT_25To50PCT_75To100PCT ((uint16_t)0x0A00) ///< 25%~50% & 75%~100%
#define SWDT_0To50PCT_75To100PCT ((uint16_t)0x0B00)  ///< 0%~50% & 75%~100%
#define SWDT_50To100PCT ((uint16_t)0x0C00)           ///< 50%~100%
#define SWDT_0To25PCT_50To100PCT ((uint16_t)0x0D00)  ///< 0%~25% & 50%~100%
#define SWDT_25To100PCT ((uint16_t)0x0E00)           ///< 25%~100%
#define SWDT_0To100PCT ((uint16_t)0x0F00)            ///< 0%~100%

/**
 *******************************************************************************
 ** \brief SWDT count control in the sleep/stop mode
 ******************************************************************************/
#define SWDT_SPECIAL_MODE_COUNT_CONTINUE ((uint16_t)0x0000) ///< SWDT count continue in the sleep/stop mode
#define SWDT_SPECIAL_MODE_COUNT_STOP ((uint16_t)0x1000)     ///< SWDT count stop in the sleep/stop mode

/**
 *******************************************************************************
 ** \brief WDT running state after reset
 ******************************************************************************/
#define WDT_AUTO_START_AFTER_RESET ((uint16_t)0x0000) ///< WDT Auto Start after reset
#define WDT_STOP_AFTER_RESET ((uint16_t)0x0001)       ///< WDT stop after reset

/**
 *******************************************************************************
 ** \brief WDT count underflow or refresh error trigger event type
 ******************************************************************************/
#define WDT_INTERRUPT_REQUEST ((uint16_t)0x0000) ///< WDT trigger interrupt request
#define WDT_RESET_REQUEST ((uint16_t)0x0002)     ///< WDT trigger reset request

/**
 *******************************************************************************
 ** \brief WDT count underflow cycle
 ******************************************************************************/
#define WDT_COUNT_UNDERFLOW_CYCLE_256 ((uint16_t)0x0000)   ///< 256 clock cycle
#define WDT_COUNT_UNDERFLOW_CYCLE_4096 ((uint16_t)0x0004)  ///< 4096 clock cycle
#define WDT_COUNT_UNDERFLOW_CYCLE_16384 ((uint16_t)0x0008) ///< 16384 clock cycle
#define WDT_COUNT_UNDERFLOW_CYCLE_65536 ((uint16_t)0x000C) ///< 65536 clock cycle

/**
 *******************************************************************************
 ** \brief WDT count clock division
 ******************************************************************************/
#define WDT_COUNT_PCLK3_DIV4 ((uint16_t)0x0020)    ///< PCLK3/4
#define WDT_COUNT_PCLK3_DIV64 ((uint16_t)0x0060)   ///< PCLK3/64
#define WDT_COUNT_PCLK3_DIV128 ((uint16_t)0x0070)  ///< PCLK3/128
#define WDT_COUNT_PCLK3_DIV256 ((uint16_t)0x0080)  ///< PCLK3/256
#define WDT_COUNT_PCLK3_DIV512 ((uint16_t)0x0090)  ///< PCLK3/512
#define WDT_COUNT_PCLK3_DIV1024 ((uint16_t)0x00A0) ///< PCLK3/1024
#define WDT_COUNT_PCLK3_DIV2048 ((uint16_t)0x00B0) ///< PCLK3/2048
#define WDT_COUNT_PCLK3_DIV8192 ((uint16_t)0x00D0) ///< PCLK3/8192

/**
 *******************************************************************************
 ** \brief WDT allow refresh percent range
 ******************************************************************************/
#define WDT_100PCT ((uint16_t)0x0000)               ///< 100%
#define WDT_0To25PCT ((uint16_t)0x0100)             ///< 0%~25%
#define WDT_25To50PCT ((uint16_t)0x0200)            ///< 25%~50%
#define WDT_0To50PCT ((uint16_t)0x0300)             ///< 0%~50%
#define WDT_50To75PCT ((uint16_t)0x0400)            ///< 50%~75%
#define WDT_0To25PCT_50To75PCT ((uint16_t)0x0500)   ///< 0%~25% & 50%~75%
#define WDT_25To75PCT ((uint16_t)0x0600)            ///< 25%~75%
#define WDT_0To75PCT ((uint16_t)0x0700)             ///< 0%~75%
#define WDT_75To100PCT ((uint16_t)0x0800)           ///< 75%~100%
#define WDT_0To25PCT_75To100PCT ((uint16_t)0x0900)  ///< 0%~25% & 75%~100%
#define WDT_25To50PCT_75To100PCT ((uint16_t)0x0A00) ///< 25%~50% & 75%~100%
#define WDT_0To50PCT_75To100PCT ((uint16_t)0x0B00)  ///< 0%~50% & 75%~100%
#define WDT_50To100PCT ((uint16_t)0x0C00)           ///< 50%~100%
#define WDT_0To25PCT_50To100PCT ((uint16_t)0x0D00)  ///< 0%~25% & 50%~100%
#define WDT_25To100PCT ((uint16_t)0x0E00)           ///< 25%~100%
#define WDT_0To100PCT ((uint16_t)0x0F00)            ///< 0%~100%

/**
 *******************************************************************************
 ** \brief WDT count control in the sleep mode
 ******************************************************************************/
#define WDT_SPECIAL_MODE_COUNT_CONTINUE ((uint16_t)0x0000) ///< WDT count continue in the sleep mode
#define WDT_SPECIAL_MODE_COUNT_STOP ((uint16_t)0x1000)     ///< WDT count stop in the sleep mode

/**
 *******************************************************************************
 ** \brief HRC frequency select
 ******************************************************************************/
#define HRC_FREQUENCY_20MHZ ((uint16_t)0x0000) ///< HRC frequency 20MHZ
#define HRC_FREQUENCY_16MHZ ((uint16_t)0x0001) ///< HRC frequency 16MHZ

/**
 *******************************************************************************
 ** \brief HRC oscillation state control
 ******************************************************************************/
#define HRC_OSCILLATION_START ((uint16_t)0x0000) ///< HRC oscillation start
#define HRC_OSCILLATION_STOP ((uint16_t)0x0100)  ///< HRC oscillation stop

/**
 *******************************************************************************
 ** \brief VDU0 threshold voltage select
 ******************************************************************************/
#define VDU0_VOLTAGE_THRESHOLD_1P5 ((uint8_t)0x00) ///< VDU0 voltage threshold 1.9V
#define VDU0_VOLTAGE_THRESHOLD_2P0 ((uint8_t)0x01) ///< VDU0 voltage threshold 2.0V
#define VDU0_VOLTAGE_THRESHOLD_2P1 ((uint8_t)0x02) ///< VDU0 voltage threshold 2.1V
#define VDU0_VOLTAGE_THRESHOLD_2P3 ((uint8_t)0x03) ///< VDU0 voltage threshold 2.3V

/**
 *******************************************************************************
 ** \brief VDU0 running state after reset
 ******************************************************************************/
#define VDU0_START_AFTER_RESET ((uint8_t)0x00) ///< VDU0 start after reset
#define VDU0_STOP_AFTER_RESET ((uint8_t)0x04)  ///< VDU0 stop after reset

/**
 *******************************************************************************
 ** \brief NMI pin filter sample clock division
 ******************************************************************************/
#define NMI_PIN_FILTER_PCLK3_DIV1 ((uint8_t)0x00)  ///< PCLK3
#define NMI_PIN_FILTER_PCLK3_DIV8 ((uint8_t)0x04)  ///< PCLK3/8
#define NMI_PIN_FILTER_PCLK3_DIV32 ((uint8_t)0x08) ///< PCLK3/32
#define NMI_PIN_FILTER_PCLK3_DIV64 ((uint8_t)0x0C) ///< PCLK3/64

/**
 *******************************************************************************
 ** \brief NMI pin trigger edge type
 ******************************************************************************/
#define NMI_PIN_TRIGGER_EDGE_FALLING ((uint8_t)0x00) ///< Falling edge trigger
#define NMI_PIN_TRIGGER_EDGE_RISING ((uint8_t)0x10)  ///< Rising edge trigger

/**
 *******************************************************************************
 ** \brief Enable or disable NMI pin interrupt request
 ******************************************************************************/
#define NMI_PIN_IRQ_DISABLE ((uint8_t)0x00) ///< Disable NMI pin interrupt request
#define NMI_PIN_IRQ_ENABLE ((uint8_t)0x20)  ///< Enable NMI pin interrupt request

/**
 *******************************************************************************
 ** \brief Enable or disable NMI digital filter function
 ******************************************************************************/
#define NMI_DIGITAL_FILTER_DISABLE ((uint8_t)0x00) ///< Disable NMI digital filter
#define NMI_DIGITAL_FILTER_ENABLE ((uint8_t)0x40)  ///< Enable NMI digital filter

/**
 *******************************************************************************
 ** \brief Enable or disable NMI pin ICG function
 ******************************************************************************/
#define NMI_PIN_ICG_FUNCTION_DISABLE ((uint8_t)0x80) ///< Disable NMI pin ICG function
#define NMI_PIN_ICG_FUNCTION_ENABLE ((uint8_t)0x00)  ///< Enable NMI pin ICG function

/**
 *******************************************************************************
 ** \brief ICG start configure function on/off
 ******************************************************************************/
#ifndef ICG_FUNCTION_ON
#define ICG_FUNCTION_ON (1u)
#endif

#ifndef ICG_FUNCTION_OFF
#define ICG_FUNCTION_OFF (0u)
#endif

/**
 *******************************************************************************
 ** \brief SWDT hardware start configuration
 ******************************************************************************/
/*!< Enable or disable SWDT hardware start */
#define ICG0_SWDT_HARDWARE_START (ICG_FUNCTION_OFF)

/*!< SWDT register config */
#define ICG0_SWDT_AUTS (SWDT_STOP_AFTER_RESET)
#define ICG0_SWDT_ITS (SWDT_RESET_REQUEST)
#define ICG0_SWDT_PERI (SWDT_COUNT_UNDERFLOW_CYCLE_16384)
#define ICG0_SWDT_CKS (SWDT_COUNT_SWDTCLK_DIV2048)
#define ICG0_SWDT_WDPT (SWDT_0To100PCT)
#define ICG0_SWDT_SLTPOFF (SWDT_SPECIAL_MODE_COUNT_STOP)

/*!< SWDT register config value */
#if ICG0_SWDT_HARDWARE_START == ICG_FUNCTION_ON
#define ICG0_SWDT_REG_CONFIG (ICG0_SWDT_AUTS | ICG0_SWDT_ITS | ICG0_SWDT_PERI | \
                              ICG0_SWDT_CKS | ICG0_SWDT_WDPT | ICG0_SWDT_SLTPOFF)
#else
#define ICG0_SWDT_REG_CONFIG ((uint16_t)0xFFFF)
#endif

/**
 *******************************************************************************
 ** \brief WDT hardware start configuration
 ******************************************************************************/
/*!< Enable or disable WDT hardware start */
#define ICG0_WDT_HARDWARE_START (ICG_FUNCTION_OFF)

/*!< WDT register config */
#define ICG0_WDT_AUTS (WDT_STOP_AFTER_RESET)
#define ICG0_WDT_ITS (WDT_RESET_REQUEST)
#define ICG0_WDT_PERI (WDT_COUNT_UNDERFLOW_CYCLE_16384)
#define ICG0_WDT_CKS (WDT_COUNT_PCLK3_DIV8192)
#define ICG0_WDT_WDPT (WDT_0To100PCT)
#define ICG0_WDT_SLPOFF (WDT_SPECIAL_MODE_COUNT_STOP)

/*!< WDT register config value */
#if ICG0_WDT_HARDWARE_START == ICG_FUNCTION_ON
#define ICG0_WDT_REG_CONFIG (ICG0_WDT_AUTS | ICG0_WDT_ITS | ICG0_WDT_PERI | \
                             ICG0_WDT_CKS | ICG0_WDT_WDPT | ICG0_WDT_SLPOFF)
#else
#define ICG0_WDT_REG_CONFIG ((uint16_t)0xFFFF)
#endif

/**
 *******************************************************************************
 ** \brief HRC hardware start configuration
 ******************************************************************************/
/*!< Enable or disable HRC hardware start */
#define ICG1_HRC_HARDWARE_START (ICG_FUNCTION_OFF)

/*!< HRC register config */
#define ICG1_HRC_FREQSEL (HRC_FREQUENCY_16MHZ)
#define ICG1_HRC_STOP (HRC_OSCILLATION_START)

/*!< HRC register config value */
#if ICG1_HRC_HARDWARE_START == ICG_FUNCTION_ON
#define ICG1_HRC_REG_CONFIG (ICG1_HRC_FREQSEL | ICG1_HRC_STOP)
#else
#define ICG1_HRC_REG_CONFIG ((uint16_t)0xFFFF)
#endif

/**
 *******************************************************************************
 ** \brief VDU0 hardware start configuration
 ******************************************************************************/
/*!< Enable or disable VDU0 hardware start */
#define ICG1_VDU0_HARDWARE_START (ICG_FUNCTION_OFF)

/*!< VDU0 register config */
#define ICG1_VDU0_BOR_LEV (VDU0_VOLTAGE_THRESHOLD_2P3)
#define ICG1_VDU0_BORDIS (VDU0_STOP_AFTER_RESET)

/*!< VDU0 register config value */
#if ICG1_VDU0_HARDWARE_START == ICG_FUNCTION_ON
#define ICG1_VDU0_REG_CONFIG (ICG1_VDU0_BOR_LEV | ICG1_VDU0_BORDIS)
#else
#define ICG1_VDU0_REG_CONFIG ((uint8_t)0xFF)
#endif

/**
 *******************************************************************************
 ** \brief NMI hardware start configuration
 ******************************************************************************/
/*!< Enable or disable NMI hardware start */
#define ICG1_NMI_HARDWARE_START (ICG_FUNCTION_OFF)

/*!< NMI register config */
#define ICG1_NMI_SMPCLK (NMI_PIN_FILTER_PCLK3_DIV1)
#define ICG1_NMI_TRG (NMI_PIN_TRIGGER_EDGE_RISING)
#define ICG1_NMI_IMR (NMI_PIN_IRQ_DISABLE)
#define ICG1_NMI_NFEN (NMI_DIGITAL_FILTER_DISABLE)
#define ICG1_NMI_ICGENA (NMI_PIN_ICG_FUNCTION_DISABLE)

/*!< NMI register config value */
#if ICG1_NMI_HARDWARE_START == ICG_FUNCTION_ON
#define ICG1_NMI_REG_CONFIG (ICG1_NMI_SMPCLK | ICG1_NMI_TRG | \
                             ICG1_NMI_IMR | ICG1_NMI_NFEN | ICG1_NMI_ICGENA)
#else
#define ICG1_NMI_REG_CONFIG ((uint8_t)0xFF)
#endif

/**
 *******************************************************************************
 ** \brief ICG registers configuration
 ******************************************************************************/
/*!< ICG0 register value */
#define ICG0_REGISTER_CONSTANT (((uint32_t)ICG0_WDT_REG_CONFIG << 16) | \
                                ((uint32_t)ICG0_SWDT_REG_CONFIG))
/*!< ICG1 register value */
#define ICG1_REGISTER_CONSTANT (((uint32_t)ICG1_NMI_REG_CONFIG << 24) |  \
                                ((uint32_t)ICG1_VDU0_REG_CONFIG << 16) | \
                                ((uint32_t)ICG1_HRC_REG_CONFIG))
/*!< ICG2~7 register reserved value */
#define ICG2_REGISTER_CONSTANT ((uint32_t)0xFFFFFFFFul)
#define ICG3_REGISTER_CONSTANT ((uint32_t)0xFFFFFFFFul)
#define ICG4_REGISTER_CONSTANT ((uint32_t)0xFFFFFFFFul)
#define ICG5_REGISTER_CONSTANT ((uint32_t)0xFFFFFFFFul)
#define ICG6_REGISTER_CONSTANT ((uint32_t)0xFFFFFFFFul)
#define ICG7_REGISTER_CONSTANT ((uint32_t)0xFFFFFFFFul)

  /*******************************************************************************
   * Global variable definitions ('extern')
   ******************************************************************************/

  /*******************************************************************************
    Global function prototypes (definition in C source)
   ******************************************************************************/

  //@} // IcgGroup

#ifdef __cplusplus
}
#endif

#endif /* DDL_ICG_ENABLE */

#endif /* __HC32F46x_ICG_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
