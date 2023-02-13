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
/** \file hc32f46x_timer6.h
 **
 ** A detailed description is available at
 ** @link Timer6Group Timer 6 description @endlink
 **
 **   - 2018-11-19  1.0  Husj  First version for Device Driver Library of Timer6.
 **
 ******************************************************************************/
#ifndef __HC32F46x_TIMER6_H__
#define __HC32F46x_TIMER6_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"

#if (DDL_TIMER6_ENABLE == DDL_ON)

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

    /**
     *******************************************************************************
     ** \defgroup Timer6Group Timer6
     **
     ******************************************************************************/
    //@{

    /*******************************************************************************
     * Global type definitions ('typedef')
     ******************************************************************************/
    /**
     *******************************************************************************
     ** \brief Timer6 channel enumeration
     ******************************************************************************/

    /**
    ******************************************************************************
    ** \brief Timer6 PWMx port define
    *****************************************************************************/
    typedef enum en_timer6_chx_port
    {
        Timer6PWMA = 0u, ///< PWM A port
        Timer6PWMB = 1u, ///< PWM B port
    } en_timer6_chx_port_t;

    /**
    ******************************************************************************
    ** \brief Timer6 TRIG port define
    *****************************************************************************/
    typedef enum en_timer6_input_port
    {
        Timer6xCHA = 0u,  ///< PWM A port
        Timer6xCHB = 1u,  ///< PWM B port
        Timer6TrigA = 2u, ///< Timer6x trigger A port
        Timer6TrigB = 3u, ///< Timer6x trigger B port
    } en_timer6_input_port_t;

    /**
     ******************************************************************************
     ** \brief Timer6 general control - Z Phase input mask periods
     **
     ** \note
     ******************************************************************************/
    typedef enum en_timer6_gconr_zmsk
    {
        Timer6ZMaskDis = 0u,   ///< Z Phase input mask disable
        Timer6ZMask4Cyl = 1u,  ///< Mask Z phase input during 4 periods when over fllow or under fllow of position count
        Timer6ZMask8Cyl = 2u,  ///< Mask Z phase input during 8 periods when over fllow or under fllow of position count
        Tiemr6ZMask16Cyl = 3u, ///< Mask Z phase input during 16 periods when over fllow or under fllow of position count
    } en_timer6_gconr_zmsk_t;

    /**
     *******************************************************************************
     ** \brief Clock division enumeration
     ******************************************************************************/
    typedef enum en_timer6_clk_div
    {
        Timer6PclkDiv1 = 0u,    ///< Count clock: pclk
        Timer6PclkDiv2 = 1u,    ///< Count clock: pclk/2
        Timer6PclkDiv4 = 2u,    ///< Count clock: pclk/4
        Timer6PclkDiv8 = 3u,    ///< Count clock: pclk/8
        Timer6PclkDiv16 = 4u,   ///< Count clock: pclk/16
        Timer6PclkDiv64 = 5u,   ///< Count clock: pclk/64
        Timer6PclkDiv256 = 6u,  ///< Count clock: pclk/256
        Timer6PclkDiv1024 = 7u, ///< Count clock: pclk/1024
    } en_timer6_clk_div_t;

    /**
     *******************************************************************************
     ** \brief Count mode enumeration
     ******************************************************************************/
    typedef enum en_timer6_count_mode
    {
        Timer6CntSawtoothMode = 0u,    ///< Sawtooth wave mode
        Timer6CntTriangularModeA = 4u, ///< Triangular wave mode
        Timer6CntTriangularModeB = 5u, ///< Triangular wave mode
    } en_timer6_count_mode_t;

    /**
     *******************************************************************************
     ** \brief Count direction enumeration
     ******************************************************************************/
    typedef enum en_timer6_count_dir
    {
        Timer6CntDirDown = 0u, ///< Counter counting up
        Timer6CntDirUp = 1u,   ///< Counter counting down
    } en_timer6_count_dir_t;

    /**
     ******************************************************************************
     ** \brief Timer6 period register
     **
     ** \note
     ******************************************************************************/
    typedef enum en_timer6_period
    {
        Timer6PeriodA = 0u, ///< Period Register A
        Timer6PeriodB = 1u, ///< Period Register B
        Timer6PeriodC = 2u, ///< Period Register C
    } en_timer6_period_t;

    /**
     ******************************************************************************
     ** \brief Timer6 general compare match register
     **
     ** \note
     ******************************************************************************/
    typedef enum en_timer6_compare
    {
        Timer6GenCompareA = 0u, ///< General Compare Match A Register
        Timer6GenCompareB = 1u, ///< General Compare Match B Register
        Timer6GenCompareC = 2u, ///< General Compare Match C Register
        Timer6GenCompareD = 3u, ///< General Compare Match D Register
        Timer6GenCompareE = 4u, ///< General Compare Match E Register
        Timer6GenCompareF = 5u, ///< General Compare Match F Register
    } en_timer6_compare_t;

    /**
     ******************************************************************************
     ** \brief Timer6 special compare match register
     **
     ** \note
     ******************************************************************************/
    typedef enum en_timer6_special_compare
    {
        Timer6SpclCompA = 0u, ///< Special Compare Match A Register
        Timer6SpclCompB = 1u, ///< Special Compare Match B Register
        Timer6SpclCompC = 2u, ///< Special Compare Match C Register
        Timer6SpclCompD = 3u, ///< Special Compare Match D Register
        Timer6SpclCompE = 4u, ///< Special Compare Match E Register
        Timer6SpclCompF = 5u, ///< Special Compare Match F Register
    } en_timer6_special_compare_t;

    /**
     ******************************************************************************
     ** \brief Timer6 dead timer value register
     **
     ** \note
     ******************************************************************************/
    typedef enum en_timer6_dead_time_reg
    {
        Timer6DeadTimUpAR = 0u,  ///< Dead timer for up count A Register
        Timer6DeadTimUpBR = 1u,  ///< Dead timer for up count B Register
        Timer6DeadTimDwnAR = 2u, ///< Dead timer for down count A Register
        Timer6DeadTimDwnBR = 3u, ///< Dead timer for down count B Register
    } en_timer6_dead_time_reg_t;

    /**
     ******************************************************************************
     ** \brief Timer6 port control - port output control of brake status
     **
     ** \note
     ******************************************************************************/
    typedef enum en_timer6_pconr_disval
    {
        Timer6PWMxDisValNorm = 0u, ///< PWMx port output normal when brake
        Timer6PWMxDisValHiZ = 1u,  ///< PWMx port Hi-z when brake
        Timer6PWMxDisValLow = 2u,  ///< PWMx port output low level when brake
        Timer6PWMxDisValHigh = 3u, ///< PWMx port output high level when brake
    } en_timer6_pconr_disval_t;

    /**
     ******************************************************************************
     ** \brief Timer6 port - PWMx port output status when CNTER value match GCMxR or PERAR
     **
     ** \note
     ******************************************************************************/
    typedef enum en_timer6_pconr_cmpc
    {
        Timer6PWMxCompareLow = 0u,  ///< PWMx port output Low level when CNTER value match with GCMxR or PERAR
        Timer6PWMxCompareHigh = 1u, ///< PWMx port output high level when CNTER value match with GCMxR or PERAR
        Timer6PWMxCompareKeep = 2u, ///< PWMx port output keep former level when CNTER value match GCMxR or PERAR
        Timer6PWMxCompareInv = 3u,  ///< PWMx port output inverse level when CNTER value match with GCMxR or PERAR
    } en_timer6_pconr_cmpc_t;

    /**
     ******************************************************************************
     ** \brief Timer6 port- PWMx port output set(STA STP)
     **
     ** \note
     ******************************************************************************/
    typedef enum en_timer6_pconr_port_out
    {
        Timer6PWMxPortOutLow = 0u,  ///< PWMx port output set low level
        Timer6PWMxPortOutHigh = 1u, ///< PWMx port output set high level
    } en_timer6_pconr_port_out_t;

    /**
     *******************************************************************************
     ** \brief Timer6 function mode selection enumeration
     ******************************************************************************/
    typedef enum en_timer6_func_mode
    {
        Timer6ModeCompareOutput = 0u, ///< Compare output function
        Timer6ModeCaptureInput = 1u,  ///< Capture input function
    } en_timer6_func_mode_t;

    /**
     ******************************************************************************
     ** \brief Timer6 port - PWMx STA STP function selection
     **
     ** \note
     ******************************************************************************/
    typedef enum en_timer6_pconr_stastps
    {
        Timer6PWMxStateSelSS = 0u,   ///< PWMx output status is decide by STACx STPCx when CNTER start and stop
        Timer6PWMxStateSelKeep = 1u, ///< PWMx output status keep former level when CNTER start and stop
    } en_timer6_pconr_stastps_t;

    /**
     ******************************************************************************
     ** \brief Timer6 buffer - General compare register / Period register transfer function selection
     **
     ** \note
     ** For General compare register:
     ** Single buffer stransfer: Compare Ouput: GCMCR-->GCMAR / GCMDR-->GCMBR  Capture Input: GCMAR-->GCMCR / GCMDR-->GCMBR
     ** Double buffer stransfer: Compare Ouput: GCMER-->GCMCR-->GCMAR / GCMFR-->GCMDR-->GCMBR  Capture Input: GCMAR-->GCMCR-->GCMER /GCMFR-->GCMDR-->GCMBR
     ** For Period register:
     ** Single buffer stransfer: PERBR-->PERAR
     ** Double buffer stransfer: PERCR-->PERBR-->PERAR
     ******************************************************************************/
    typedef enum en_timer6_buf_gcmp_prd
    {
        Timer6GcmpPrdSingleBuf = 0u, ///< Single buffer stransfer
        Timer6GcmpPrdDoubleBuf = 1u, ///< Double buffer stransfer
    } en_timer6_buf_gcmp_prd_t;

    /**
     ******************************************************************************
     ** \brief Timer6 buffer - Special compare register transfer function selection
     **
     ** \note
     ******************************************************************************/
    typedef enum en_timer6_buf_spcl_cmp
    {
        Timer6SpclSingleBuf = 0u, ///< Single buffer stransfer: Compare Ouput: SCMCR-->SCMAR / SCMDR-->SCMBR
        Timer6SpclDoubleBuf = 1u, ///< Double buffer stransfer: Compare Ouput: SCMER-->SCMCR-->SCMAR / SCMFR-->SCMDR-->SCMBR
    } en_timer6_buf_spcl_cmp_t;

    /**
     ******************************************************************************
     ** \brief Timer6 buffer - Special compare register transfer opportunity selection
     **
     ** \note
     ******************************************************************************/
    typedef enum en_timer6_buf_spcl_opt
    {
        Timer6SplcOptNone = 0u,      ///< No transfer
        Timer6SplcOptOverFlow = 1u,  ///< Transfer when over flow (About sawtooth mode,accord to the count direction)
        Timer6SplcOptUnderFlow = 2u, ///< Transfer when under flow (About sawtooth mode,accord to the count direction)
        Timer6SplcOptBoth = 3u,      ///< Transfer when over flow or under flow (About sawtooth mode,accord to the count direction)
    } en_timer6_buf_spcl_opt_t;

    /**
     ******************************************************************************
     ** \brief ADT dead timer control - PWMx dead timer separate set
     **
     ** \note
     ******************************************************************************/
    typedef enum en_timer6_dconr_sepa
    {
        Timer6PWMxDtSeparate = 0u, ///< The dead timer of up count and down count separate set by DTUAR and DTDAR
        Timer6PWMxDtEqual = 1u,    ///< the values of DTUAR and DTDAR are equal automatically
    } en_timer6_dconr_sepa_t;

    /**
     ******************************************************************************
     ** \brief ADT filter control- TRIx/PWMx port filter sample clock selection
     **
     ** \note
     ******************************************************************************/
    typedef enum en_timer6_fconr_fltclk
    {
        Timer6FltClkPclk0Div1 = 0u,  ///< PCLK0
        Timer6FltClkPclk0Div4 = 1u,  ///< PCLK0/4
        Timer6FltClkPclk0Div16 = 2u, ///< PCLK0/16
        Timer6FltClkPclk0Div64 = 3u, ///< PCLK0/64
    } en_timer6_fconr_fltclk_t;

    /**
     ******************************************************************************
     ** \brief Timer6 valid  period repeat- TIMx valid period repeat function selection(trigger interrupt or AOS event)
     **
     ** \note
     ******************************************************************************/
    typedef enum en_timer6_vperr_pcnts
    {
        Timer6PeriodCnts0 = 0u, ///< Valid period repeat function disable
        Timer6PeriodCnts1 = 1u, ///< Enable every other one period
        Timer6PeriodCnts2 = 2u, ///< Enable every other two periods
        Timer6PeriodCnts3 = 3u, ///< Enable every other three periods
        Timer6PeriodCnts4 = 4u, ///< Enable every other four periods
        Timer6PeriodCnts5 = 5u, ///< Enable every other five periods
        Timer6PeriodCnts6 = 6u, ///< Enable every other six periods
        Timer6PeriodCnts7 = 7u, ///< Enable every other seven periods
    } en_timer6_vperr_pcnts_t;

    /**
     ******************************************************************************
     ** \brief Timer6 valid  period repeat- Count condition select
     **
     ** \note
     ******************************************************************************/
    typedef enum en_timer6_vperr_pcnte
    {
        Timer6PeriodCnteDisable = 0u, ///< Valid period repeat function disable
        Timer6PeriodCnteMin = 1u,     ///< Over flow and under flow point of Sawtooth wave mode, or under flow point of Triangular wave mode
        Timer6PeriodCnteMax = 2u,     ///< Over flow and under flow point of Sawtooth wave mode, or voer flow point of Triangular wave mode
        Timer6PeriodCnteBoth = 3u,    ///< Over flow and under flow point of Sawtooth wave mode, or voer flow and under flow point of Triangular wave mode
    } en_timer6_vperr_pcnte_t;

    /**
     ******************************************************************************
     ** \brief Timer6 Hardware(Start/Stop/Clear/Capture) event trigger select
     **
     ** \note
     ******************************************************************************/

    typedef enum en_timer6_hw_trig
    {
        Timer6HwTrigAos0 = 0u,         ///< Hardware trigger event from AOS0(HTSSR0)
        Timer6HwTrigAos1 = 1u,         ///< Hardware trigger event from AOS1(HTSSR1)
        Timer6HwTrigPWMARise = 4u,     ///< Hardware trigger event from PWMA rising
        Timer6HwTrigPWMAFall = 5u,     ///< Hardware trigger event from PWMA falling
        Timer6HwTrigPWMBRise = 6u,     ///< Hardware trigger event from PWMA rising
        Timer6HwTrigPWMBFall = 7u,     ///< Hardware trigger event from PWMA falling
        Timer6HwTrigTimTriARise = 8u,  ///< Hardware trigger event from TRIGA rising
        Timer6HwTrigTimTriAFall = 9u,  ///< Hardware trigger event from TRIGA falling
        Timer6HwTrigTimTriBRise = 10u, ///< Hardware trigger event from TRIGB rising
        Timer6HwTrigTimTriBFall = 11u, ///< Hardware trigger event from TRIGB falling
        Timer6HwTrigEnd = 16u,
    } en_timer6_hw_trig_t;

    /**
     ******************************************************************************
     ** \brief Timer6 hardware (up count/down count) event trigger select
     **
     ** \note
     ******************************************************************************/

    typedef enum en_timer6_hw_cnt
    {
        Timer6HwCntPWMALowPWMBRise = 0u,  ///< PWMB Rising trigger when PWMA is low level
        Timer6HwCntPWMALowPWMBFall = 1u,  ///< PWMB falling trigger when PWMA is low level
        Timer6HwCntPWMAHighPWMBRise = 2u, ///< PWMB Rising trigger when PWMA is high level
        Timer6HwCntPWMAHighPWMBFall = 3u, ///< PWMB falling trigger when PWMA is high level
        Timer6HwCntPWMBLowPWMARise = 4u,  ///< PWMA Rising trigger when PWMB is low level
        Timer6HwCntPWMBLowPWMAFall = 5u,  ///< PWMA falling trigger when PWMB is low level
        Timer6HwCntPWMBHighPWMARise = 6u, ///< PWMA Rising trigger when PWMB is high level
        Timer6HwCntPWMBHighPWMAFall = 7u, ///< PWMA falling trigger when PWMB is high level
        Timer6HwCntTRIGARise = 8u,        ///< TRIGA rising trigger
        Timer6HwCntTRIGAFall = 9u,        ///< TRIGA falling trigger
        Timer6HwCntTRIGBRise = 10u,       ///< TRIGB rising trigger
        Timer6HwCntTRIGBFall = 11u,       ///< TRIGB falling trigger
        Timer6HwCntAos0 = 16u,            ///< AOS0 trigger
        Timer6HwCntAos1 = 17u,            ///< AOS1 trigger
        Timer6HwCntMax = 18u,
    } en_timer6_hw_cnt_t;

    /**
     ******************************************************************************
     ** \brief Timer6 interrupt type
     **
     ** \note
     ******************************************************************************/
    typedef enum en_timer6_irq_type
    {
        Timer6INTENA = 0u,    ///< Interrupt of count equal to GCMA (or capture input A)
        Timer6INTENB = 1u,    ///< Interrupt of count equal to GCMB (or capture input B)
        Timer6INTENC = 2u,    ///< Interrupt of count equal to GCMC
        Timer6INTEND = 3u,    ///< Interrupt of count equal to GCMD
        Timer6INTENE = 4u,    ///< Interrupt of count equal to GCME
        Timer6INTENF = 5u,    ///< Interrupt of count equal to GCMF
        Timer6INTENOVF = 6u,  ///< Interrupt of over flow of sawtooth wave mode or peak point of triangular wave mode
        Timer6INTENUDF = 7u,  ///< Interrupt of under flow of sawtooth wave mode or valley point of triangular wave mode
        Timer6INTENDTE = 8u,  ///< Interrupt of dead timer error
        Timer6INTENSAU = 16u, ///< Interrupt of count up equally compared with SCMA
        Timer6INTENSAD = 17u, ///< Interrupt of count down equally compared with SCMA
        Timer6INTENSBU = 18u, ///< Interrupt of count up equally compared with SCMB
        Timer6INTENSBD = 19u, ///< Interrupt of count down equally compared with SCMB
    } en_timer6_irq_type_t;

    /**
     ******************************************************************************
     ** \brief Timer6 status flag
     **
     ** \note
     ******************************************************************************/
    typedef enum en_timer6_status
    {
        Timer6CMAF = 0u,    ///< Status flag of count equal to GCMA (or capture input A)
        Timer6CMBF = 1u,    ///< Status flag of count equal to GCMB (or capture input B)
        Timer6CMCF = 2u,    ///< Status flag of count equal to GCMC
        Timer6CMDF = 3u,    ///< Status flag of count equal to GCMD
        Timer6CMEF = 4u,    ///< Status flag of count equal to GCME
        Timer6CMFF = 5u,    ///< Status flag of count equal to GCMF
        Timer6OVFF = 6u,    ///< Status flag of over flow of sawtooth wave mode or peak point of triangular wave mode
        Timer6UDFF = 7u,    ///< Status flag of under flow of sawtooth wave mode or valley point of triangular wave mode
        Timer6DTEF = 8u,    ///< Status flag of dead timer error
        Timer6CMSAUF = 9u,  ///< Status flag of count up equally compared with SCMA
        Timer6CMSADF = 10u, ///< Status flag of count down equally compared with SCMA
        Timer6CMSBUF = 11u, ///< Status flag of count up equally compared with SCMB
        Timer6CMSBDF = 12u, ///< Status flag of count down equally compared with SCMB
        Timer6VPERNUM = 21, ///< Number of valid period
        Timer6DIRF = 31,    ///< Count direction
    } en_timer6_status_t;

    /**
     ******************************************************************************
     ** \brief Timer6 software synchronous config
     ** \note
     ******************************************************************************/
    typedef struct stc_timer6_sw_sync
    {
        bool bTimer61; ///< Timer6 unit1
        bool bTimer62; ///< Timer6 unit2
        bool bTimer63; ///< Timer6 unit3
    } stc_timer6_sw_sync_t;

    /**
     ******************************************************************************
     ** \brief Timer6 base init structure definition
     ** \note
     ******************************************************************************/
    typedef struct stc_timer6_basecnt_cfg
    {
        en_timer6_count_mode_t enCntMode; ///< Count mode
        en_timer6_count_dir_t enCntDir;   ///< Count direction
        en_timer6_clk_div_t enCntClkDiv;  ///< Count clock division select
    } stc_timer6_basecnt_cfg_t;

    /**
     ******************************************************************************
     ** \brief Timer6 Trig port config
     ** \note
     ******************************************************************************/
    typedef struct stc_timer6_port_trig_cfg
    {
        bool bFltEn;                       ///< trig source capture input filter enable
        en_timer6_fconr_fltclk_t enFltClk; ///< Filter clock
    } stc_tiemr6_port_trig_cfg_t;

    /**
     ******************************************************************************
     ** \brief Timer6 port output config
     ** \note
     ******************************************************************************/
    typedef struct stc_timer6_port_output_cfg
    {
        en_timer6_func_mode_t enPortMode;    ///< Port mode
        bool bOutEn;                         ///< Output enable / disable
        en_timer6_pconr_cmpc_t enPerc;       ///< Port state when counter match the period
        en_timer6_pconr_cmpc_t enCmpc;       ///< Port state when counter match GCMAR(GCMBR)
        en_timer6_pconr_stastps_t enStaStp;  ///< Post state selection when count start/stop
        en_timer6_pconr_port_out_t enStaOut; ///< Port state when count start
        en_timer6_pconr_port_out_t enStpOut; ///< port stop when count stop
        en_timer6_pconr_disval_t enDisVal;   ///< Port output state when brake
    } stc_timer6_port_output_cfg_t;

    /**
     ******************************************************************************
     ** \brief Timer6 port input config
     ** \note
     ******************************************************************************/
    typedef struct stc_timer6_port_input_cfg
    {
        en_timer6_input_port_t enPortSel;  ///< Port select
        en_timer6_func_mode_t enPortMode;  ///< Port mode
        bool bFltEn;                       ///< trig source capture input filter enable
        en_timer6_fconr_fltclk_t enFltClk; ///< Filter clock
    } stc_timer6_port_input_cfg_t;

    /**
     ******************************************************************************
     ** \brief Timer6 hardware dead time function config
     ** \note
     ******************************************************************************/
    typedef struct stc_timer6_deadtime_cfg
    {
        bool bEnDeadtime;     ///< Enable hardware dead time function
        bool bEnDtBufUp;      ///< Enable buffer transfer for up count dead time register(DTUBR-->DTUAR)
        bool bEnDtBufDwn;     ///< Enable buffer transfer for down count dead time register(DTDBR-->DTDAR)
        bool bEnDtEqualUpDwn; ///< Enable down count dead time register equal to up count DT register
    } stc_timer6_deadtime_cfg_t;

    /**
     ******************************************************************************
     ** \brief Timer6 valid period config
     ** \note
     ******************************************************************************/
    typedef struct stc_timer6_validper_cfg
    {
        en_timer6_vperr_pcnts_t enValidCntNum; ///< Valid period selection
        en_timer6_vperr_pcnte_t enValidCdtEn;  ///< Count condition of valid period
        bool bPeriodSCMA;                      ///< Sepcial signal A valid period selection enable
        bool bPeriodSCMB;                      ///< Sepcial signal A valid period selection enable
    } stc_timer6_validper_cfg_t;

    /**
     ******************************************************************************
     ** \brief Timer6 general compare register buffer transfer config
     ** \note
     ******************************************************************************/
    typedef struct stc_timer6_gcmp_buf_cfg
    {
        bool bEnGcmpTransBuf;                        ///< Enable/Disable buffer transfer
        en_timer6_buf_gcmp_prd_t enGcmpBufTransType; ///< Sigle or double buffer transfer
    } stc_timer6_gcmp_buf_cfg_t;

    /**
     ******************************************************************************
     ** \brief Timer6 period register buffer transfer config
     ** \note
     ******************************************************************************/
    typedef struct stc_timer6_period_buf_cfg
    {
        bool bEnPeriodTransBuf;                        ///< Enable/Disable buffer transfer
        en_timer6_buf_gcmp_prd_t enPeriodBufTransType; ///< Sigle or double buffer transfer
    } stc_timer6_period_buf_cfg_t;

    /**
     ******************************************************************************
     ** \brief Timer6 Specila compare register buffer transfer config
     ** \note
     ******************************************************************************/
    typedef struct stc_timer6_spcl_buf_cfg
    {
        bool bEnSpclTransBuf;                        ///< Enable/Disable buffer transfer
        en_timer6_buf_spcl_cmp_t enSpclBufTransType; ///< Sigle or double buffer transfer
        en_timer6_buf_spcl_opt_t enSpclBufOptType;   ///< Buffer transfer opportunity
    } stc_timer6_spcl_buf_cfg_t;

    /**
     ******************************************************************************
     ** \brief Timer6 Z phase input mask config
     ** \note
     ******************************************************************************/
    typedef struct stc_timer6_zmask_cfg
    {
        en_timer6_gconr_zmsk_t enZMaskCycle; ///< Z phase input mask periods selection
        bool bFltPosCntMaksEn;               ///< As position count timer, position counter clear function enable(TRUE) or disable(FALSE) during the time of Z phase input mask
        bool bFltRevCntMaksEn;               ///< As revolution count timer, the counter function enable(TRUE) or disable(FALSE) during the time of Z phase input mask
    } stc_timer6_zmask_cfg_t;

    /*******************************************************************************
     * Global pre-processor symbols/macros ('#define')
     ******************************************************************************/

    /*******************************************************************************
     * Global variable definitions ('extern')
     ******************************************************************************/

    /*******************************************************************************
      Global function prototypes (definition in C source)
     ******************************************************************************/
    /* IRQ config */
    en_result_t Timer6_ConfigIrq(M4_TMR6_TypeDef *TMR6x, en_timer6_irq_type_t enTimer6Irq, bool bEn);
    /* Get status(flag) */
    uint8_t Timer6_GetStatus(M4_TMR6_TypeDef *TMR6x, en_timer6_status_t enStatus);

    /* Base functions */
    en_result_t Timer6_DeInit(M4_TMR6_TypeDef *TMR6x);
    en_result_t Timer6_Init(M4_TMR6_TypeDef *TMR6x, const stc_timer6_basecnt_cfg_t *pstcTimer6BaseCntCfg);
    /* Timer6 unit start count*/
    en_result_t Timer6_StartCount(M4_TMR6_TypeDef *TMR6x);
    /* Timer6 unit stop count*/
    en_result_t Timer6_StopCount(M4_TMR6_TypeDef *TMR6x);
    /* Timer6 unit Set Count Value*/
    en_result_t Timer6_SetCount(M4_TMR6_TypeDef *TMR6x, uint16_t u16Value);
    /* Timer6 unit Get Count Value*/
    uint16_t Timer6_GetCount(M4_TMR6_TypeDef *TMR6x);
    /* Timer6 unit Clear Count Value*/
    en_result_t Timer6_ClearCount(M4_TMR6_TypeDef *TMR6x);

    /* Timer6 unit Set Period and buffer Value*/
    en_result_t Timer6_SetPeriod(M4_TMR6_TypeDef *TMR6x, en_timer6_period_t enTimer6Periodx, uint16_t u16Period);
    /* Timer6 unit set general compare register value*/
    en_result_t Timer6_SetGeneralCmpValue(M4_TMR6_TypeDef *TMR6x, en_timer6_compare_t enTimer6Compare, uint16_t u16Compare);
    /* Timer6 unit set specoal compare register value*/
    en_result_t Timer6_SetSpecialCmpValue(M4_TMR6_TypeDef *TMR6x, en_timer6_special_compare_t enTimer6SpclCmp, uint16_t u16SpclCmp);
    /* Timer6 unit get general compare register value*/
    uint16_t Timer6_GetGeneralCmpValue(M4_TMR6_TypeDef *TMR6x, en_timer6_compare_t enTimer6Compare);

    /* Timer6 unit set period buffer transfer function*/
    en_result_t Timer6_SetPeriodBuf(M4_TMR6_TypeDef *TMR6x, const stc_timer6_period_buf_cfg_t *pstcTimer6PrdBufCfg);
    /* Timer6 unit set general compare buffer transfer function*/
    en_result_t Timer6_SetGeneralBuf(M4_TMR6_TypeDef *TMR6x, en_timer6_chx_port_t enTimer6PWMPort, const stc_timer6_gcmp_buf_cfg_t *pstcTimer6GenBufCfg);
    /* Timer6 unit set special compare buffer transfer function*/
    en_result_t Timer6_SetSpecialBuf(M4_TMR6_TypeDef *TMR6x, en_timer6_special_compare_t enTimer6SpclCmp, const stc_timer6_spcl_buf_cfg_t *pstcTimer6SpclBufCfg);

    /* Timer6 unit Set valid period Value*/
    en_result_t Timer6_SetValidPeriod(M4_TMR6_TypeDef *TMR6x, const stc_timer6_validper_cfg_t *pstcTimer6ValidPerCfg);

    /* Config Input prot and filter function */
    en_result_t Timer6_PortInputConfig(M4_TMR6_TypeDef *TMR6x, const stc_timer6_port_input_cfg_t *pstcTimer6PortInputCfg);
    /* Config output prot function */
    en_result_t Timer6_PortOutputConfig(M4_TMR6_TypeDef *TMR6x, en_timer6_chx_port_t enTimer6PWMPort, const stc_timer6_port_output_cfg_t *pstcTimer6PortOutCfg);

    /* Set dead time register value */
    en_result_t Timer6_SetDeadTimeValue(M4_TMR6_TypeDef *TMR6x, en_timer6_dead_time_reg_t enTimer6DTReg, uint16_t u16DTValue);
    /* Config dead time function */
    en_result_t Timer6_ConfigDeadTime(M4_TMR6_TypeDef *TMR6x, const stc_timer6_deadtime_cfg_t *pstcTimer6DTCfg);

    /* Config Software Synchrony Stop */
    en_result_t Timer6_SwSyncStart(const stc_timer6_sw_sync_t *pstcTimer6SwSyncStart);
    /* Config Software Synchrony Start */
    en_result_t Timer6_SwSyncStop(const stc_timer6_sw_sync_t *pstcTimer6SwSyncStop);
    /* Config Software Synchrony Clear */
    en_result_t Timer6_SwSyncClear(const stc_timer6_sw_sync_t *pstcTimer6SwSyncClear);
    /* Get Software Synchrony Status */
    en_result_t Timer6_GetSwSyncState(stc_timer6_sw_sync_t *pstcTimer6SwSyncState);

    /* Config Hardware up count event */
    en_result_t Timer6_ConfigHwCntUp(M4_TMR6_TypeDef *TMR6x, en_timer6_hw_cnt_t enTimer6HwCntUp);
    /* Clear Hardware up count event */
    en_result_t Timer6_ClearHwCntUp(M4_TMR6_TypeDef *TMR6x);
    /* Config Hardware down count event */
    en_result_t Timer6_ConfigHwCntDwn(M4_TMR6_TypeDef *TMR6x, en_timer6_hw_cnt_t enTimer6HwCntDwn);
    /* Clear Hardware down count event */
    en_result_t Timer6_ClearHwCntDwn(M4_TMR6_TypeDef *TMR6x);

    /* Config Hardware start event */
    en_result_t Timer6_ConfigHwStart(M4_TMR6_TypeDef *TMR6x, en_timer6_hw_trig_t enTimer6HwStart);
    /* Clear Hardware start event */
    en_result_t Timer6_ClearHwStart(M4_TMR6_TypeDef *TMR6x);
    /* Enable Hardware start event */
    en_result_t Timer6_EnableHwStart(M4_TMR6_TypeDef *TMR6x);
    /* Dsiable Hardware start event */
    en_result_t Timer6_DisableHwStart(M4_TMR6_TypeDef *TMR6x);

    /* Config Hardware stop event */
    en_result_t Timer6_ConfigHwStop(M4_TMR6_TypeDef *TMR6x, en_timer6_hw_trig_t enTimer6HwStop);
    /* Clear Hardware stop event */
    en_result_t Timer6_ClearHwStop(M4_TMR6_TypeDef *TMR6x);
    /* Enable Hardware stop event */
    en_result_t Timer6_EnableHwStop(M4_TMR6_TypeDef *TMR6x);
    /* Disable Hardware stop event */
    en_result_t Timer6_DisableHwStop(M4_TMR6_TypeDef *TMR6x);

    /* Config Hardware clear event */
    en_result_t Timer6_ConfigHwClear(M4_TMR6_TypeDef *TMR6x, en_timer6_hw_trig_t enTimer6HwClear);
    /* Clear Hardware clear event */
    en_result_t Timer6_ClearHwClear(M4_TMR6_TypeDef *TMR6x);
    /* Enable Hardware clear event */
    en_result_t Timer6_EnableHwClear(M4_TMR6_TypeDef *TMR6x);
    /* Dsiable Hardware clear event */
    en_result_t Timer6_DisableHwClear(M4_TMR6_TypeDef *TMR6x);

    /* Config Hardware capture event A */
    en_result_t Timer6_ConfigHwCaptureA(M4_TMR6_TypeDef *TMR6x, en_timer6_hw_trig_t enTimer6HwCaptureA);
    /* Clear Hardware capture event A */
    en_result_t Timer6_ClearHwCaptureA(M4_TMR6_TypeDef *TMR6x);
    /* Config Hardware capture event B */
    en_result_t Timer6_ConfigHwCaptureB(M4_TMR6_TypeDef *TMR6x, en_timer6_hw_trig_t enTimer6HwCaptureB);
    /* Clear Hardware capture event B */
    en_result_t Timer6_ClearHwCaptureB(M4_TMR6_TypeDef *TMR6x);
    /* Set trigger source 0 of hardware event */
    en_result_t Timer6_SetTriggerSrc0(en_event_src_t enTriggerSrc);
    /* Set trigger source 1 of hardware event */
    en_result_t Timer6_SetTriggerSrc1(en_event_src_t enTriggerSrc);

    /* Z phase input mask config */
    en_result_t Timer6_ConfigZMask(M4_TMR6_TypeDef *TMR6x, const stc_timer6_zmask_cfg_t *pstcTimer6ZMaskCfg);

    //@} // Timer6Group

#ifdef __cplusplus
}
#endif

#endif /* DDL_TIMER6_ENABLE */

#endif /* __HC32F46x_TIMER6_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
