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
/** \file hc32_ddl.h
 **
 ** A detailed description is available at
 ** @link Hc32DdlGroup Hc32 Series Ddl description @endlink
 **
 **   - 2018-10-22  1.0  Yangjp First version for Hc32 Series Device Driver
 **     Library.
 **
 ******************************************************************************/
#ifndef __HC32_DDL_H__
#define __HC32_DDL_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
 *******************************************************************************
 ** \defgroup Hc32DdlGroup Hc32 Series Device Driver Library(HC32DDL)
 **
 ******************************************************************************/
//@{

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/*! Defined use device driver library */
#if !defined(USE_DEVICE_DRIVER_LIB)
/**
 *******************************************************************************
 ** \brief Comment the line below if you will not use the device driver library.
 ** In this case, the application code will be based on direct access to
 ** peripherals registers.
 ******************************************************************************/
/* #define USE_DEVICE_DRIVER_LIB */
#endif /* USE_DEVICE_DRIVER_LIB */

/**
 *******************************************************************************
 ** \brief Hc32 Series device driver library version number v1.0.4
 ******************************************************************************/
#define HC32_DDL_VERSION_MAIN (0x01u) ///< [31:24] main version
#define HC32_DDL_VERSION_SUB1 (0x01u) ///< [23:16] sub1 version
#define HC32_DDL_VERSION_SUB2 (0x00u) ///< [15:8]  sub2 version
#define HC32_DDL_VERSION_RC (0x00u)   ///< [7:0]   release candidate
#define HC32_DDL_VERSION ((HC32_DDL_VERSION_MAIN << 24) | \
                          (HC32_DDL_VERSION_SUB1 << 16) | \
                          (HC32_DDL_VERSION_SUB2 << 8) |  \
                          (HC32_DDL_VERSION_RC))

/*! Use device driver library */
#if defined(USE_DEVICE_DRIVER_LIB)
/**
 *******************************************************************************
 ** \brief Include module's header file
 ******************************************************************************/
#if (DDL_ADC_ENABLE == DDL_ON)
#include "hc32f46x_adc.h"
#endif /* DDL_ADC_ENABLE */

#if (DDL_AES_ENABLE == DDL_ON)
#include "hc32f46x_aes.h"
#endif /* DDL_AES_ENABLE */

#if (DDL_CAN_ENABLE == DDL_ON)
#include "hc32f46x_can.h"
#endif /* DDL_CAN_ENABLE */

#if (DDL_CLK_ENABLE == DDL_ON)
#include "hc32f46x_clk.h"
#endif /* DDL_CLK_ENABLE */

#if (DDL_CMP_ENABLE == DDL_ON)
#include "hc32f46x_cmp.h"
#endif /* DDL_CMP_ENABLE */

#if (DDL_CRC_ENABLE == DDL_ON)
#include "hc32f46x_crc.h"
#endif /* DDL_CRC_ENABLE */

#if (DDL_DCU_ENABLE == DDL_ON)
#include "hc32f46x_dcu.h"
#endif /* DDL_DCU_ENABLE */

#if (DDL_DMAC_ENABLE == DDL_ON)
#include "hc32f46x_dmac.h"
#endif /* DDL_DMAC_ENABLE */

#if (DDL_EFM_ENABLE == DDL_ON)
#include "hc32f46x_efm.h"
#endif /* DDL_EFM_ENABLE */

#if (DDL_EMB_ENABLE == DDL_ON)
#include "hc32f46x_emb.h"
#endif /* DDL_EMB_ENABLE */

#if (DDL_EVENT_PORT_ENABLE == DDL_ON)
#include "hc32f46x_event_port.h"
#endif /* DDL_EVENT_PORT_ENABLE */

#if (DDL_EXINT_NMI_SWI_ENABLE == DDL_ON)
#include "hc32f46x_exint_nmi_swi.h"
#endif /* DDL_EXINT_NMI_SWI_ENABLE */

#if (DDL_GPIO_ENABLE == DDL_ON)
#include "hc32f46x_gpio.h"
#endif /* DDL_GPIO_ENABLE */

#if (DDL_HASH_ENABLE == DDL_ON)
#include "hc32f46x_hash.h"
#endif /* DDL_HASH_ENABLE */

#if (DDL_I2C_ENABLE == DDL_ON)
#include "hc32f46x_i2c.h"
#endif /* DDL_I2C_ENABLE */

#if (DDL_I2S_ENABLE == DDL_ON)
#include "hc32f46x_i2s.h"
#endif /* DDL_I2S_ENABLE */

#if (DDL_ICG_ENABLE == DDL_ON)
#include "hc32f46x_icg.h"
#endif /* DDL_ICG_ENABLE */

#if (DDL_INTERRUPTS_ENABLE == DDL_ON)
#include "hc32f46x_interrupts.h"
#endif /* DDL_INTERRUPTS_ENABLE */

#if (DDL_KEYSCAN_ENABLE == DDL_ON)
#include "hc32f46x_keyscan.h"
#endif /* DDL_KEYSCAN_ENABLE */

#if (DDL_MPU_ENABLE == DDL_ON)
#include "hc32f46x_mpu.h"
#endif /* DDL_MPU_ENABLE */

#if (DDL_OTS_ENABLE == DDL_ON)
#include "hc32f46x_ots.h"
#endif /* DDL_OTS_ENABLE */

#if (DDL_PWC_ENABLE == DDL_ON)
#include "hc32f46x_pwc.h"
#endif /* DDL_PWC_ENABLE */

#if (DDL_QSPI_ENABLE == DDL_ON)
#include "hc32f46x_qspi.h"
#endif /* DDL_QSPI_ENABLE */

#if (DDL_RMU_ENABLE == DDL_ON)
#include "hc32f46x_rmu.h"
#endif /* DDL_RMU_ENABLE */

#if (DDL_RTC_ENABLE == DDL_ON)
#include "hc32f46x_rtc.h"
#endif /* DDL_RTC_ENABLE */

#if (DDL_SDIOC_ENABLE == DDL_ON)
#include "hc32f46x_sdioc.h"
#endif /* DDL_SDIOC_ENABLE */

#if (DDL_SPI_ENABLE == DDL_ON)
#include "hc32f46x_spi.h"
#endif /* DDL_SPI_ENABLE */

#if (DDL_SRAM_ENABLE == DDL_ON)
#include "hc32f46x_sram.h"
#endif /* DDL_SRAM_ENABLE */

#if (DDL_SWDT_ENABLE == DDL_ON)
#include "hc32f46x_swdt.h"
#endif /* DDL_SWDT_ENABLE */

#if (DDL_TIMER0_ENABLE == DDL_ON)
#include "hc32f46x_timer0.h"
#endif /* DDL_TIMER0_ENABLE */

#if (DDL_TIMER4_CNT_ENABLE == DDL_ON)
#include "hc32f46x_timer4_cnt.h"
#endif /* DDL_TIMER4_CNT_ENABLE */

#if (DDL_TIMER4_EMB_ENABLE == DDL_ON)
#include "hc32f46x_timer4_emb.h"
#endif /* DDL_TIMER4_EMB_ENABLE */

#if (DDL_TIMER4_OCO_ENABLE == DDL_ON)
#include "hc32f46x_timer4_oco.h"
#endif /* DDL_TIMER4_OCO_ENABLE */

#if (DDL_TIMER4_PWM_ENABLE == DDL_ON)
#include "hc32f46x_timer4_pwm.h"
#endif /* DDL_TIMER4_PWM_ENABLE */

#if (DDL_TIMER4_SEVT_ENABLE == DDL_ON)
#include "hc32f46x_timer4_sevt.h"
#endif /* DDL_TIMER4_SEVT_ENABLE */

#if (DDL_TIMER6_ENABLE == DDL_ON)
#include "hc32f46x_timer6.h"
#endif /* DDL_TIMER6_ENABLE */

#if (DDL_TIMERA_ENABLE == DDL_ON)
#include "hc32f46x_timera.h"
#endif /* DDL_TIMERA_ENABLE */

#if (DDL_TRNG_ENABLE == DDL_ON)
#include "hc32f46x_trng.h"
#endif /* DDL_TRNG_ENABLE */

#if (DDL_USART_ENABLE == DDL_ON)
#include "hc32f46x_usart.h"
#endif /* DDL_USART_ENABLE */

#if (DDL_USBFS_ENABLE == DDL_ON)
#include "hc32f46x_usbfs.h"
#endif /* DDL_USBFS_ENABLE */

#if (DDL_UTILITY_ENABLE == DDL_ON)
#include "hc32f46x_utility.h"
#endif /* DDL_UTILITY_ENABLE */

#if (DDL_WDT_ENABLE == DDL_ON)
#include "hc32f46x_wdt.h"
#endif /* DDL_WDT_ENABLE */

#endif /* USE_DEVICE_DRIVER_LIB */

    /*******************************************************************************
     * Global variable definitions ('extern')
     ******************************************************************************/

    /*******************************************************************************
     * Global function prototypes (definition in C source)
     ******************************************************************************/

    //@} // Hc32DdlGroup

#ifdef __cplusplus
}
#endif

#endif /* __HC32_DDL_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
