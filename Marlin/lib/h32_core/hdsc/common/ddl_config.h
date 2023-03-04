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
/** \file ddl_config.h
 **
 ** A detailed description is available at
 ** @link DdlConfigGroup Ddl Config description @endlink
 **
 **   - 2018-9-27  1.0  Yangjp First version for Device Driver Library config.
 **
 ******************************************************************************/
#ifndef __DDL_CONFIG_H__
#define __DDL_CONFIG_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
 *******************************************************************************
 ** \defgroup DdlConfigGroup Device Driver Library config(DDLCONFIG)
 **
 ******************************************************************************/
//@{

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/*! Chip module on-off define */
#define DDL_ON 1u
#define DDL_OFF 0u

/**
 *******************************************************************************
 ** \brief This is the list of modules to be used in the device driver library
 ** Select the modules you need to use to DDL_ON.
 **
 ** \note DDL_ICG_ENABLE and DDL_UTILITY_ENABLE must be turned on(DDL_ON) to
 ** ensure that the chip works properly.
 ******************************************************************************/
#define DDL_ICG_ENABLE DDL_ON
#define DDL_UTILITY_ENABLE DDL_ON

#define DDL_ADC_ENABLE DDL_ON
#define DDL_AES_ENABLE DDL_OFF
#define DDL_CAN_ENABLE DDL_OFF
#define DDL_CMP_ENABLE DDL_OFF
#define DDL_CLK_ENABLE DDL_ON
#define DDL_DCU_ENABLE DDL_OFF
#define DDL_DMAC_ENABLE DDL_ON
#define DDL_EFM_ENABLE DDL_ON
#define DDL_EMB_ENABLE DDL_OFF
#define DDL_EXINT_NMI_SWI_ENABLE DDL_ON
#define DDL_GPIO_ENABLE DDL_ON
#define DDL_HASH_ENABLE DDL_OFF
#define DDL_I2C_ENABLE DDL_OFF
#define DDL_I2S_ENABLE DDL_OFF
#define DDL_INTERRUPTS_ENABLE DDL_ON
#define DDL_KEYSCAN_ENABLE DDL_OFF
#define DDL_MPU_ENABLE DDL_OFF
#define DDL_OTS_ENABLE DDL_ON
#define DDL_PGA_ENABLE DDL_OFF
#define DDL_PWC_ENABLE DDL_ON
#define DDL_QSPI_ENABLE DDL_OFF
#define DDL_RMU_ENABLE DDL_ON
#define DDL_RTC_ENABLE DDL_OFF
#define DDL_SDIOC_ENABLE DDL_ON
#define DDL_SPI_ENABLE DDL_OFF
#define DDL_SRAM_ENABLE DDL_ON
#define DDL_SWDT_ENABLE DDL_OFF
#define DDL_TIMER0_ENABLE DDL_ON
#define DDL_TIMER4_CNT_ENABLE DDL_OFF
#define DDL_TIMER4_EMB_ENABLE DDL_OFF
#define DDL_TIMER4_OCO_ENABLE DDL_OFF
#define DDL_TIMER4_PWM_ENABLE DDL_OFF
#define DDL_TIMER4_SEVT_ENABLE DDL_OFF
#define DDL_TIMER6_ENABLE DDL_OFF
#define DDL_TIMERA_ENABLE DDL_ON
#define DDL_TRNG_ENABLE DDL_OFF
#define DDL_USART_ENABLE DDL_ON
#define DDL_USBFS_ENABLE DDL_OFF
#define DDL_WDT_ENABLE DDL_ON

/*! Midware module on-off define */
#define MW_ON 1u
#define MW_OFF 0u

/**
 *******************************************************************************
 ** \brief This is the list of Midware modules to use
 ** Select the modules you need to use to MW_ON.
 ******************************************************************************/
#define MW_SD_CARD_ENABLE MW_OFF
#define MW_USB_ENABLE MW_OFF

    /*******************************************************************************
     * Global variable definitions ('extern')
     ******************************************************************************/

    /*******************************************************************************
     * Global function prototypes (definition in C source)
     ******************************************************************************/

    //@} // DdlConfigGroup

#ifdef __cplusplus
}
#endif

#endif /* __DDL_CONFIG_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
