/*******************************************************************************
 * Copyright (C) 2016, Huada Semiconductor Co.,Ltd All rights reserved.
 *
 * This software is owned and published by:
 * Huada Semiconductor Co.,Ltd ("HDSC").
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
/** \file hc32f46x_interrupts.h
 **
 ** A detailed description is available at
 ** @link InterruptGroup Interrupt description @endlink
 **
 **   - 2018-10-12  1.0  Zhangxl First version for Device Driver Library of
 **     interrupt.
 **
 ******************************************************************************/
#ifndef __HC32F46X_INTERRUPTS_H___
#define __HC32F46X_INTERRUPTS_H___

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"

#if (DDL_INTERRUPTS_ENABLE == DDL_ON)

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

    /**
     *******************************************************************************
     ** \defgroup InterruptGroup Interrupt
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
     ** \brief IRQ registration structure definition
     ******************************************************************************/
    typedef struct stc_irq_regi_conf
    {
        en_int_src_t enIntSrc;
        IRQn_Type enIRQn;
        func_ptr_t pfnCallback;
    } stc_irq_regi_conf_t;

    /**
     *******************************************************************************
     ** \brief stop mode interrupt wakeup source enumeration
     ******************************************************************************/
    typedef enum en_int_wkup_src
    {
        Extint0WU = 1u << 0,
        Extint1WU = 1u << 1,
        Extint2WU = 1u << 2,
        Extint3WU = 1u << 3,
        Extint4WU = 1u << 4,
        Extint5WU = 1u << 5,
        Extint6WU = 1u << 6,
        Extint7WU = 1u << 7,
        Extint8WU = 1u << 8,
        Extint9WU = 1u << 9,
        Extint10WU = 1u << 10,
        Extint11WU = 1u << 11,
        Extint12WU = 1u << 12,
        Extint13WU = 1u << 13,
        Extint14WU = 1u << 14,
        Extint15WU = 1u << 15,
        SwdtWU = 1u << 16,
        Vdu1WU = 1u << 17,
        Vdu2WU = 1u << 18,
        CmpWU = 1u << 19,
        WakeupTimerWU = 1u << 20,
        RtcAlarmWU = 1u << 21,
        RtcPeriodWU = 1u << 22,
        Timer0WU = 1u << 23,
        Usart1RxWU = 1u << 25,
    } en_int_wkup_src_t;

    /**
     *******************************************************************************
     ** \brief event enumeration
     ******************************************************************************/
    typedef enum en_evt
    {
        Event0 = 1u << 0,
        Event1 = 1u << 1,
        Event2 = 1u << 2,
        Event3 = 1u << 3,
        Event4 = 1u << 4,
        Event5 = 1u << 5,
        Event6 = 1u << 6,
        Event7 = 1u << 7,
        Event8 = 1u << 8,
        Event9 = 1u << 9,
        Event10 = 1u << 10,
        Event11 = 1u << 11,
        Event12 = 1u << 12,
        Event13 = 1u << 13,
        Event14 = 1u << 14,
        Event15 = 1u << 15,
        Event16 = 1u << 16,
        Event17 = 1u << 17,
        Event18 = 1u << 18,
        Event19 = 1u << 19,
        Event20 = 1u << 20,
        Event21 = 1u << 21,
        Event22 = 1u << 22,
        Event23 = 1u << 23,
        Event24 = 1u << 24,
        Event25 = 1u << 25,
        Event26 = 1u << 26,
        Event27 = 1u << 27,
        Event28 = 1u << 28,
        Event29 = 1u << 29,
        Event30 = 1u << 30,
        Event31 = 1u << 31,
    } en_evt_t;

    /**
     *******************************************************************************
     ** \brief interrupt enumeration
     ******************************************************************************/
    typedef enum en_int
    {
        Int0 = 1u << 0,
        Int1 = 1u << 1,
        Int2 = 1u << 2,
        Int3 = 1u << 3,
        Int4 = 1u << 4,
        Int5 = 1u << 5,
        Int6 = 1u << 6,
        Int7 = 1u << 7,
        Int8 = 1u << 8,
        Int9 = 1u << 9,
        Int10 = 1u << 10,
        Int11 = 1u << 11,
        Int12 = 1u << 12,
        Int13 = 1u << 13,
        Int14 = 1u << 14,
        Int15 = 1u << 15,
        Int16 = 1u << 16,
        Int17 = 1u << 17,
        Int18 = 1u << 18,
        Int19 = 1u << 19,
        Int20 = 1u << 20,
        Int21 = 1u << 21,
        Int22 = 1u << 22,
        Int23 = 1u << 23,
        Int24 = 1u << 24,
        Int25 = 1u << 25,
        Int26 = 1u << 26,
        Int27 = 1u << 27,
        Int28 = 1u << 28,
        Int29 = 1u << 29,
        Int30 = 1u << 30,
        Int31 = 1u << 31,
    } en_int_t;

/*! Bit mask definition*/
#define BIT_MASK_00 (1ul << 0)
#define BIT_MASK_01 (1ul << 1)
#define BIT_MASK_02 (1ul << 2)
#define BIT_MASK_03 (1ul << 3)
#define BIT_MASK_04 (1ul << 4)
#define BIT_MASK_05 (1ul << 5)
#define BIT_MASK_06 (1ul << 6)
#define BIT_MASK_07 (1ul << 7)
#define BIT_MASK_08 (1ul << 8)
#define BIT_MASK_09 (1ul << 9)
#define BIT_MASK_10 (1ul << 10)
#define BIT_MASK_11 (1ul << 11)
#define BIT_MASK_12 (1ul << 12)
#define BIT_MASK_13 (1ul << 13)
#define BIT_MASK_14 (1ul << 14)
#define BIT_MASK_15 (1ul << 15)
#define BIT_MASK_16 (1ul << 16)
#define BIT_MASK_17 (1ul << 17)
#define BIT_MASK_18 (1ul << 18)
#define BIT_MASK_19 (1ul << 19)
#define BIT_MASK_20 (1ul << 20)
#define BIT_MASK_21 (1ul << 21)
#define BIT_MASK_22 (1ul << 22)
#define BIT_MASK_23 (1ul << 23)
#define BIT_MASK_24 (1ul << 24)
#define BIT_MASK_25 (1ul << 25)
#define BIT_MASK_26 (1ul << 26)
#define BIT_MASK_27 (1ul << 27)
#define BIT_MASK_28 (1ul << 28)
#define BIT_MASK_29 (1ul << 29)
#define BIT_MASK_30 (1ul << 30)
#define BIT_MASK_31 (1ul << 31)

/*! Default Priority for IRQ, Possible values are 0 (high priority) to 15 (low priority) */
#define DDL_IRQ_PRIORITY_DEFAULT 15u

/*! Interrupt priority level 00 ~ 15*/
#define DDL_IRQ_PRIORITY_00 (0u)
#define DDL_IRQ_PRIORITY_01 (1u)
#define DDL_IRQ_PRIORITY_02 (2u)
#define DDL_IRQ_PRIORITY_03 (3u)
#define DDL_IRQ_PRIORITY_04 (4u)
#define DDL_IRQ_PRIORITY_05 (5u)
#define DDL_IRQ_PRIORITY_06 (6u)
#define DDL_IRQ_PRIORITY_07 (7u)
#define DDL_IRQ_PRIORITY_08 (8u)
#define DDL_IRQ_PRIORITY_09 (9u)
#define DDL_IRQ_PRIORITY_10 (10u)
#define DDL_IRQ_PRIORITY_11 (11u)
#define DDL_IRQ_PRIORITY_12 (12u)
#define DDL_IRQ_PRIORITY_13 (13u)
#define DDL_IRQ_PRIORITY_14 (14u)
#define DDL_IRQ_PRIORITY_15 (15u)

    /**
     *******************************************************************************
     ** \brief AOS software trigger function
     **
     ******************************************************************************/
    __STATIC_INLINE void AOS_SW_Trigger(void)
    {
        bM4_AOS_INT_SFTTRG_STRG = 1u;
    }

    /*******************************************************************************
     * Global variable definitions ('extern')
     ******************************************************************************/
    extern en_result_t enIrqRegistration(const stc_irq_regi_conf_t *pstcIrqRegiConf);
    extern en_result_t enIrqResign(IRQn_Type enIRQn);
    extern en_result_t enShareIrqEnable(en_int_src_t enIntSrc);
    extern en_result_t enShareIrqDisable(en_int_src_t enIntSrc);
    extern en_result_t enIntWakeupEnable(uint32_t u32WakeupSrc);
    extern en_result_t enIntWakeupDisable(uint32_t u32WakeupSrc);
    extern en_result_t enEventEnable(uint32_t u32Event);
    extern en_result_t enEventDisable(uint32_t u32Event);
    extern en_result_t enIntEnable(uint32_t u32Int);
    extern en_result_t enIntDisable(uint32_t u32Int);

    /*******************************************************************************
     * Global function prototypes (definition in C source)
     ******************************************************************************/
    __WEAKDEF void NMI_IrqHandler(void);
    __WEAKDEF void HardFault_IrqHandler(void);
    __WEAKDEF void MemManage_IrqHandler(void);
    __WEAKDEF void BusFault_IrqHandler(void);
    __WEAKDEF void UsageFault_IrqHandler(void);
    __WEAKDEF void SVC_IrqHandler(void);
    __WEAKDEF void DebugMon_IrqHandler(void);
    __WEAKDEF void PendSV_IrqHandler(void);
    __WEAKDEF void SysTick_IrqHandler(void);

    __WEAKDEF void Extint00_IrqHandler(void);
    __WEAKDEF void Extint01_IrqHandler(void);
    __WEAKDEF void Extint02_IrqHandler(void);
    __WEAKDEF void Extint03_IrqHandler(void);
    __WEAKDEF void Extint04_IrqHandler(void);
    __WEAKDEF void Extint05_IrqHandler(void);
    __WEAKDEF void Extint06_IrqHandler(void);
    __WEAKDEF void Extint07_IrqHandler(void);
    __WEAKDEF void Extint08_IrqHandler(void);
    __WEAKDEF void Extint09_IrqHandler(void);
    __WEAKDEF void Extint10_IrqHandler(void);
    __WEAKDEF void Extint11_IrqHandler(void);
    __WEAKDEF void Extint12_IrqHandler(void);
    __WEAKDEF void Extint13_IrqHandler(void);
    __WEAKDEF void Extint14_IrqHandler(void);
    __WEAKDEF void Extint15_IrqHandler(void);

    __WEAKDEF void Dma1Tc0_IrqHandler(void);
    __WEAKDEF void Dma1Tc1_IrqHandler(void);
    __WEAKDEF void Dma1Tc2_IrqHandler(void);
    __WEAKDEF void Dma1Tc3_IrqHandler(void);
    __WEAKDEF void Dma2Tc0_IrqHandler(void);
    __WEAKDEF void Dma2Tc1_IrqHandler(void);
    __WEAKDEF void Dma2Tc2_IrqHandler(void);
    __WEAKDEF void Dma2Tc3_IrqHandler(void);
    __WEAKDEF void Dma1Btc0_IrqHandler(void);
    __WEAKDEF void Dma1Btc1_IrqHandler(void);
    __WEAKDEF void Dma1Btc2_IrqHandler(void);
    __WEAKDEF void Dma1Btc3_IrqHandler(void);
    __WEAKDEF void Dma2Btc0_IrqHandler(void);
    __WEAKDEF void Dma2Btc1_IrqHandler(void);
    __WEAKDEF void Dma2Btc2_IrqHandler(void);
    __WEAKDEF void Dma2Btc3_IrqHandler(void);
    __WEAKDEF void Dma1Err0_IrqHandler(void);
    __WEAKDEF void Dma1Err1_IrqHandler(void);
    __WEAKDEF void Dma1Err2_IrqHandler(void);
    __WEAKDEF void Dma1Err3_IrqHandler(void);
    __WEAKDEF void Dma2Err0_IrqHandler(void);
    __WEAKDEF void Dma2Err1_IrqHandler(void);
    __WEAKDEF void Dma2Err2_IrqHandler(void);
    __WEAKDEF void Dma2Err3_IrqHandler(void);

    __WEAKDEF void EfmPgmEraseErr_IrqHandler(void);
    __WEAKDEF void EfmColErr_IrqHandler(void);
    __WEAKDEF void EfmOpEnd_IrqHandler(void);
    __WEAKDEF void QspiInt_IrqHandler(void);
    __WEAKDEF void Dcu1_IrqHandler(void);
    __WEAKDEF void Dcu2_IrqHandler(void);
    __WEAKDEF void Dcu3_IrqHandler(void);
    __WEAKDEF void Dcu4_IrqHandler(void);

    __WEAKDEF void Timer01GCMA_IrqHandler(void);
    __WEAKDEF void Timer01GCMB_IrqHandler(void);
    __WEAKDEF void Timer02GCMA_IrqHandler(void);
    __WEAKDEF void Timer02GCMB_IrqHandler(void);

    __WEAKDEF void MainOscStop_IrqHandler(void);
    __WEAKDEF void WakeupTimer_IrqHandler(void);
    __WEAKDEF void Swdt_IrqHandler(void);

    __WEAKDEF void Timer61GCMA_IrqHandler(void);
    __WEAKDEF void Timer61GCMB_IrqHandler(void);
    __WEAKDEF void Timer61GCMC_IrqHandler(void);
    __WEAKDEF void Timer61GCMD_IrqHandler(void);
    __WEAKDEF void Timer61GCME_IrqHandler(void);
    __WEAKDEF void Timer61GCMF_IrqHandler(void);
    __WEAKDEF void Timer61GOV_IrqHandler(void);
    __WEAKDEF void Timer61GUD_IrqHandler(void);
    __WEAKDEF void Timer61GDT_IrqHandler(void);
    __WEAKDEF void Timer61SCMA_IrqHandler(void);
    __WEAKDEF void Timer61SCMB_IrqHandler(void);

    __WEAKDEF void Timer62GCMA_IrqHandler(void);
    __WEAKDEF void Timer62GCMB_IrqHandler(void);
    __WEAKDEF void Timer62GCMC_IrqHandler(void);
    __WEAKDEF void Timer62GCMD_IrqHandler(void);
    __WEAKDEF void Timer62GCME_IrqHandler(void);
    __WEAKDEF void Timer62GCMF_IrqHandler(void);
    __WEAKDEF void Timer62GOV_IrqHandler(void);
    __WEAKDEF void Timer62GUD_IrqHandler(void);
    __WEAKDEF void Timer62GDT_IrqHandler(void);
    __WEAKDEF void Timer62SCMA_IrqHandler(void);
    __WEAKDEF void Timer62SCMB_IrqHandler(void);

    __WEAKDEF void Timer63GCMA_IrqHandler(void);
    __WEAKDEF void Timer63GCMB_IrqHandler(void);
    __WEAKDEF void Timer63GCMC_IrqHandler(void);
    __WEAKDEF void Timer63GCMD_IrqHandler(void);
    __WEAKDEF void Timer63GCME_IrqHandler(void);
    __WEAKDEF void Timer63GCMF_IrqHandler(void);
    __WEAKDEF void Timer63GOV_IrqHandler(void);
    __WEAKDEF void Timer63GUD_IrqHandler(void);
    __WEAKDEF void Timer63GDT_IrqHandler(void);
    __WEAKDEF void Timer63SCMA_IrqHandler(void);
    __WEAKDEF void Timer63SCMB_IrqHandler(void);

    __WEAKDEF void TimerA1OV_IrqHandler(void);
    __WEAKDEF void TimerA1UD_IrqHandler(void);
    __WEAKDEF void TimerA1CMP_IrqHandler(void);
    __WEAKDEF void TimerA2OV_IrqHandler(void);
    __WEAKDEF void TimerA2UD_IrqHandler(void);
    __WEAKDEF void TimerA2CMP_IrqHandler(void);
    __WEAKDEF void TimerA3OV_IrqHandler(void);
    __WEAKDEF void TimerA3UD_IrqHandler(void);
    __WEAKDEF void TimerA3CMP_IrqHandler(void);
    __WEAKDEF void TimerA4OV_IrqHandler(void);
    __WEAKDEF void TimerA4UD_IrqHandler(void);
    __WEAKDEF void TimerA4CMP_IrqHandler(void);
    __WEAKDEF void TimerA5OV_IrqHandler(void);
    __WEAKDEF void TimerA5UD_IrqHandler(void);
    __WEAKDEF void TimerA5CMP_IrqHandler(void);
    __WEAKDEF void TimerA6OV_IrqHandler(void);
    __WEAKDEF void TimerA6UD_IrqHandler(void);
    __WEAKDEF void TimerA6CMP_IrqHandler(void);

    __WEAKDEF void UsbGlobal_IrqHandler(void);

    __WEAKDEF void Usart1RxErr_IrqHandler(void);
    __WEAKDEF void Usart1RxEnd_IrqHandler(void);
    __WEAKDEF void Usart1TxEmpty_IrqHandler(void);
    __WEAKDEF void Usart1TxEnd_IrqHandler(void);
    __WEAKDEF void Usart1RxTO_IrqHandler(void);
    __WEAKDEF void Usart2RxErr_IrqHandler(void);
    __WEAKDEF void Usart2RxEnd_IrqHandler(void);
    __WEAKDEF void Usart2TxEmpty_IrqHandler(void);
    __WEAKDEF void Usart2TxEnd_IrqHandler(void);
    __WEAKDEF void Usart2RxTO_IrqHandler(void);
    __WEAKDEF void Usart3RxErr_IrqHandler(void);
    __WEAKDEF void Usart3RxEnd_IrqHandler(void);
    __WEAKDEF void Usart3TxEmpty_IrqHandler(void);
    __WEAKDEF void Usart3TxEnd_IrqHandler(void);
    __WEAKDEF void Usart3RxTO_IrqHandler(void);
    __WEAKDEF void Usart4RxErr_IrqHandler(void);
    __WEAKDEF void Usart4RxEnd_IrqHandler(void);
    __WEAKDEF void Usart4TxEmpty_IrqHandler(void);
    __WEAKDEF void Usart4TxEnd_IrqHandler(void);
    __WEAKDEF void Usart4RxTO_IrqHandler(void);

    __WEAKDEF void Spi1RxEnd_IrqHandler(void);
    __WEAKDEF void Spi1TxEmpty_IrqHandler(void);
    __WEAKDEF void Spi1Err_IrqHandler(void);
    __WEAKDEF void Spi1Idle_IrqHandler(void);
    __WEAKDEF void Spi2RxEnd_IrqHandler(void);
    __WEAKDEF void Spi2TxEmpty_IrqHandler(void);
    __WEAKDEF void Spi2Err_IrqHandler(void);
    __WEAKDEF void Spi2Idle_IrqHandler(void);
    __WEAKDEF void Spi3RxEnd_IrqHandler(void);
    __WEAKDEF void Spi3TxEmpty_IrqHandler(void);
    __WEAKDEF void Spi3Err_IrqHandler(void);
    __WEAKDEF void Spi3Idle_IrqHandler(void);
    __WEAKDEF void Spi4RxEnd_IrqHandler(void);
    __WEAKDEF void Spi4TxEmpty_IrqHandler(void);
    __WEAKDEF void Spi4Err_IrqHandler(void);
    __WEAKDEF void Spi4Idle_IrqHandler(void);

    __WEAKDEF void Timer41GCMUH_IrqHandler(void);
    __WEAKDEF void Timer41GCMUL_IrqHandler(void);
    __WEAKDEF void Timer41GCMVH_IrqHandler(void);
    __WEAKDEF void Timer41GCMVL_IrqHandler(void);
    __WEAKDEF void Timer41GCMWH_IrqHandler(void);
    __WEAKDEF void Timer41GCMWL_IrqHandler(void);
    __WEAKDEF void Timer41GOV_IrqHandler(void);
    __WEAKDEF void Timer41GUD_IrqHandler(void);
    __WEAKDEF void Timer41ReloadU_IrqHandler(void);
    __WEAKDEF void Timer41ReloadV_IrqHandler(void);
    __WEAKDEF void Timer41ReloadW_IrqHandler(void);
    __WEAKDEF void Timer42GCMUH_IrqHandler(void);
    __WEAKDEF void Timer42GCMUL_IrqHandler(void);
    __WEAKDEF void Timer42GCMVH_IrqHandler(void);
    __WEAKDEF void Timer42GCMVL_IrqHandler(void);
    __WEAKDEF void Timer42GCMWH_IrqHandler(void);
    __WEAKDEF void Timer42GCMWL_IrqHandler(void);
    __WEAKDEF void Timer42GOV_IrqHandler(void);
    __WEAKDEF void Timer42GUD_IrqHandler(void);
    __WEAKDEF void Timer42ReloadU_IrqHandler(void);
    __WEAKDEF void Timer42ReloadV_IrqHandler(void);
    __WEAKDEF void Timer42ReloadW_IrqHandler(void);
    __WEAKDEF void Timer43GCMUH_IrqHandler(void);
    __WEAKDEF void Timer43GCMUL_IrqHandler(void);
    __WEAKDEF void Timer43GCMVH_IrqHandler(void);
    __WEAKDEF void Timer43GCMVL_IrqHandler(void);
    __WEAKDEF void Timer43GCMWH_IrqHandler(void);
    __WEAKDEF void Timer43GCMWL_IrqHandler(void);
    __WEAKDEF void Timer43GOV_IrqHandler(void);
    __WEAKDEF void Timer43GUD_IrqHandler(void);
    __WEAKDEF void Timer43ReloadU_IrqHandler(void);
    __WEAKDEF void Timer43ReloadV_IrqHandler(void);
    __WEAKDEF void Timer43ReloadW_IrqHandler(void);

    __WEAKDEF void Emb1_IrqHandler(void);
    __WEAKDEF void Emb2_IrqHandler(void);
    __WEAKDEF void Emb3_IrqHandler(void);
    __WEAKDEF void Emb4_IrqHandler(void);

    __WEAKDEF void I2s1Tx_IrqHandler(void);
    __WEAKDEF void I2s1Rx_IrqHandler(void);
    __WEAKDEF void I2s1Err_IrqHandler(void);
    __WEAKDEF void I2s2Tx_IrqHandler(void);
    __WEAKDEF void I2s2Rx_IrqHandler(void);
    __WEAKDEF void I2s2Err_IrqHandler(void);
    __WEAKDEF void I2s3Tx_IrqHandler(void);
    __WEAKDEF void I2s3Rx_IrqHandler(void);
    __WEAKDEF void I2s3Err_IrqHandler(void);
    __WEAKDEF void I2s4Tx_IrqHandler(void);
    __WEAKDEF void I2s4Rx_IrqHandler(void);
    __WEAKDEF void I2s4Err_IrqHandler(void);

    __WEAKDEF void I2c1RxEnd_IrqHandler(void);
    __WEAKDEF void I2c1TxEnd_IrqHandler(void);
    __WEAKDEF void I2c1TxEmpty_IrqHandler(void);
    __WEAKDEF void I2c1Err_IrqHandler(void);
    __WEAKDEF void I2c2RxEnd_IrqHandler(void);
    __WEAKDEF void I2c2TxEnd_IrqHandler(void);
    __WEAKDEF void I2c2TxEmpty_IrqHandler(void);
    __WEAKDEF void I2c2Err_IrqHandler(void);
    __WEAKDEF void I2c3RxEnd_IrqHandler(void);
    __WEAKDEF void I2c3TxEnd_IrqHandler(void);
    __WEAKDEF void I2c3TxEmpty_IrqHandler(void);
    __WEAKDEF void I2c3Err_IrqHandler(void);

    __WEAKDEF void Lvd1_IrqHandler(void);
    __WEAKDEF void Lvd2_IrqHandler(void);

    __WEAKDEF void FcmErr_IrqHandler(void);
    __WEAKDEF void FcmEnd_IrqHandler(void);
    __WEAKDEF void FcmOV_IrqHandler(void);

    __WEAKDEF void Wdt_IrqHandler(void);

    __WEAKDEF void ADC1A_IrqHandler(void);
    __WEAKDEF void ADC1B_IrqHandler(void);
    __WEAKDEF void ADC1ChCmp_IrqHandler(void);
    __WEAKDEF void ADC1SeqCmp_IrqHandler(void);
    __WEAKDEF void ADC2A_IrqHandler(void);
    __WEAKDEF void ADC2B_IrqHandler(void);
    __WEAKDEF void ADC2ChCmp_IrqHandler(void);
    __WEAKDEF void ADC2SeqCmp_IrqHandler(void);

    __WEAKDEF void Sdio1_IrqHandler(void);
    __WEAKDEF void Sdio2_IrqHandler(void);

    __WEAKDEF void Can_IrqHandler(void);

    //@} // InterruptGroup

#ifdef __cplusplus
}
#endif

#endif /* DDL_INTERRUPTS_ENABLE */

#endif /* __HC32F46X_INTERRUPTS_H___ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
