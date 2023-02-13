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
/** \file hc32f46x_timera.h
 **
 ** A detailed description is available at
 ** @link TimeraGroup Timer A description @endlink
 **
 **   - 2018-11-08  1.0  Yangjp  First version for Device Driver Library of
 **                      Timera.
 **
 ******************************************************************************/
#ifndef __HC32F46x_TIMERA_H__
#define __HC32F46x_TIMERA_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"

#if (DDL_TIMERA_ENABLE == DDL_ON)

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

    /**
     *******************************************************************************
     ** \defgroup TimeraGroup Timer A(Timera)
     **
     ******************************************************************************/
    //@{

    /*******************************************************************************
     * Global type definitions ('typedef')
     ******************************************************************************/
    /**
     *******************************************************************************
     ** \brief Timera channel enumeration
     ******************************************************************************/
    typedef enum en_timera_channel
    {
        TimeraCh1 = 0u, ///< Timera channel 1
        TimeraCh2 = 1u, ///< Timera channel 2
        TimeraCh3 = 2u, ///< Timera channel 3
        TimeraCh4 = 3u, ///< Timera channel 4
        TimeraCh5 = 4u, ///< Timera channel 5
        TimeraCh6 = 5u, ///< Timera channel 6
        TimeraCh7 = 6u, ///< Timera channel 7
        TimeraCh8 = 7u, ///< Timera channel 8
    } en_timera_channel_t;

    /**
     *******************************************************************************
     ** \brief Clock division enumeration
     ******************************************************************************/
    typedef enum en_timera_clk_div
    {
        TimeraPclkDiv1 = 0u,     ///< Count clock: pclk
        TimeraPclkDiv2 = 1u,     ///< Count clock: pclk/2
        TimeraPclkDiv4 = 2u,     ///< Count clock: pclk/4
        TimeraPclkDiv8 = 3u,     ///< Count clock: pclk/8
        TimeraPclkDiv16 = 4u,    ///< Count clock: pclk/16
        TimeraPclkDiv32 = 5u,    ///< Count clock: pclk/32
        TimeraPclkDiv64 = 6u,    ///< Count clock: pclk/64
        TimeraPclkDiv128 = 7u,   ///< Count clock: pclk/128
        TimeraPclkDiv256 = 8u,   ///< Count clock: pclk/256
        TimeraPclkDiv512 = 9u,   ///< Count clock: pclk/512
        TimeraPclkDiv1024 = 10u, ///< Count clock: pclk/1024
    } en_timera_clk_div_t;

    /**
     *******************************************************************************
     ** \brief Count mode enumeration
     ******************************************************************************/
    typedef enum en_timera_count_mode
    {
        TimeraCountModeSawtoothWave = 0u,   ///< Sawtooth wave mode
        TimeraCountModeTriangularWave = 1u, ///< Triangular wave mode
    } en_timera_count_mode_t;

    /**
     *******************************************************************************
     ** \brief Count direction enumeration
     ******************************************************************************/
    typedef enum en_timera_count_dir
    {
        TimeraCountDirDown = 0u, ///< Counter counting down
        TimeraCountDirUp = 1u,   ///< Counter counting up
    } en_timera_count_dir_t;

    /**
     *******************************************************************************
     ** \brief Input port filter clock division enumeration
     ******************************************************************************/
    typedef enum en_timera_filter_clk_div
    {
        TimeraFilterPclkDiv1 = 0u,  ///< Filter clock: pclk
        TimeraFilterPclkDiv4 = 1u,  ///< Filter clock: pclk/4
        TimeraFilterPclkDiv16 = 2u, ///< Filter clock: pclk/16
        TimeraFilterPclkDiv64 = 3u, ///< Filter clock: pclk/64
    } en_timera_filter_clk_div_t;

    /**
     *******************************************************************************
     ** \brief Input port filter source enumeration
     **
     ** \note _<t>_ is unit number,range 1~6
     ******************************************************************************/
    typedef enum en_timera_filter_source
    {
        TimeraFilterSourceCh1 = 0u,   ///< TIMA_<t>_PWM1 input port
        TimeraFilterSourceCh2 = 1u,   ///< TIMA_<t>_PWM2 input port
        TimeraFilterSourceCh3 = 2u,   ///< TIMA_<t>_PWM3 input port
        TimeraFilterSourceCh4 = 3u,   ///< TIMA_<t>_PWM4 input port
        TimeraFilterSourceCh5 = 4u,   ///< TIMA_<t>_PWM5 input port
        TimeraFilterSourceCh6 = 5u,   ///< TIMA_<t>_PWM6 input port
        TimeraFilterSourceCh7 = 6u,   ///< TIMA_<t>_PWM7 input port
        TimeraFilterSourceCh8 = 7u,   ///< TIMA_<t>_PWM8 input port
        TimeraFilterSourceClkA = 8u,  ///< TIMA_<t>_CLKA input port
        TimeraFilterSourceClkB = 9u,  ///< TIMA_<t>_CLKB input port
        TimeraFilterSourceTrig = 10u, ///< TIMA_<t>_TRIG input port
    } en_timera_filter_source_t;

    /**
     *******************************************************************************
     ** \brief Timera interrupt request type enumeration
     ******************************************************************************/
    typedef enum en_timera_irq_type
    {
        TimeraIrqCaptureOrCompareCh1 = 0u, ///< Interrupt request when channel 1 trigger capture event or compare value equal count value
        TimeraIrqCaptureOrCompareCh2 = 1u, ///< Interrupt request when channel 2 trigger capture event or compare value equal count value
        TimeraIrqCaptureOrCompareCh3 = 2u, ///< Interrupt request when channel 3 trigger capture event or compare value equal count value
        TimeraIrqCaptureOrCompareCh4 = 3u, ///< Interrupt request when channel 4 trigger capture event or compare value equal count value
        TimeraIrqCaptureOrCompareCh5 = 4u, ///< Interrupt request when channel 5 trigger capture event or compare value equal count value
        TimeraIrqCaptureOrCompareCh6 = 5u, ///< Interrupt request when channel 6 trigger capture event or compare value equal count value
        TimeraIrqCaptureOrCompareCh7 = 6u, ///< Interrupt request when channel 7 trigger capture event or compare value equal count value
        TimeraIrqCaptureOrCompareCh8 = 7u, ///< Interrupt request when channel 8 trigger capture event or compare value equal count value
        TimeraIrqOverflow = 8u,            ///< Count overflow interrupt request
        TimeraIrqUnderflow = 9u,           ///< Count underflow interrupt request
    } en_timera_irq_type_t;

    /**
     *******************************************************************************
     ** \brief Timera flag type enumeration
     ******************************************************************************/
    typedef enum en_timera_flag_type
    {
        TimeraFlagCaptureOrCompareCh1 = 0u, ///< Match flag when channel 1 trigger capture complete or compare value equal count value
        TimeraFlagCaptureOrCompareCh2 = 1u, ///< Match flag when channel 2 trigger capture complete or compare value equal count value
        TimeraFlagCaptureOrCompareCh3 = 2u, ///< Match flag when channel 3 trigger capture complete or compare value equal count value
        TimeraFlagCaptureOrCompareCh4 = 3u, ///< Match flag when channel 4 trigger capture complete or compare value equal count value
        TimeraFlagCaptureOrCompareCh5 = 4u, ///< Match flag when channel 5 trigger capture complete or compare value equal count value
        TimeraFlagCaptureOrCompareCh6 = 5u, ///< Match flag when channel 6 trigger capture complete or compare value equal count value
        TimeraFlagCaptureOrCompareCh7 = 6u, ///< Match flag when channel 7 trigger capture complete or compare value equal count value
        TimeraFlagCaptureOrCompareCh8 = 7u, ///< Match flag when channel 8 trigger capture complete or compare value equal count value
        TimeraFlagOverflow = 8u,            ///< Count overflow flag
        TimeraFlagUnderflow = 9u,           ///< Count underflow flag
    } en_timera_flag_type_t;

    /**
     *******************************************************************************
     ** \brief Timera function mode selection enumeration
     ******************************************************************************/
    typedef enum en_timera_func_mode
    {
        TimeraModeCompareOutput = 0u, ///< Compare output function
        TimeraModeCaptureInput = 1u,  ///< Capture input function
    } en_timera_func_mode_t;

    /**
     *******************************************************************************
     ** \brief Timera count start port output status enumeration
     **
     ** \note _<t>_ is unit number,range 1~6
     ** \note PWMn is channel of unit,range 1-8
     ******************************************************************************/
    typedef enum en_timera_count_start_output
    {
        TimeraCountStartOutputLow = 0u,  ///< TIMA_<t>_PWMn port output low level
        TimeraCountStartOutputHigh = 1u, ///< TIMA_<t>_PWMn port output high level
        TimeraCountStartOutputKeep = 2u, ///< TIMA_<t>_PWMn port output to keep
    } en_timera_count_start_output_t;

    /**
     *******************************************************************************
     ** \brief Timera count stop port output status enumeration
     **
     ** \note _<t>_ is unit number,range 1~6
     ** \note PWMn is channel of unit,range 1-8
     ******************************************************************************/
    typedef enum en_timera_count_stop_output
    {
        TimeraCountStopOutputLow = 0u,  ///< TIMA_<t>_PWMn port output low level
        TimeraCountStopOutputHigh = 1u, ///< TIMA_<t>_PWMn port output high level
        TimeraCountStopOutputKeep = 2u, ///< TIMA_<t>_PWMn port output to keep
    } en_timera_count_stop_output_t;

    /**
     *******************************************************************************
     ** \brief Timera compare value match output status enumeration
     **
     ** \note _<t>_ is unit number,range 1~6
     ** \note PWMn is channel of unit,range 1-8
     ******************************************************************************/
    typedef enum en_timera_compare_match_output
    {
        TimeraCompareMatchOutputLow = 0u,     ///< TIMA_<t>_PWMn port output low level
        TimeraCompareMatchOutputHigh = 1u,    ///< TIMA_<t>_PWMn port output high level
        TimeraCompareMatchOutputKeep = 2u,    ///< TIMA_<t>_PWMn port output to keep
        TimeraCompareMatchOutputReverse = 3u, ///< TIMA_<t>_PWMn port output reverse
    } en_timera_compare_match_output_t;

    /**
     *******************************************************************************
     ** \brief Timera period value match output status enumeration
     **
     ** \note _<t>_ is unit number,range 1~6
     ** \note PWMn is channel of unit,range 1-8
     ******************************************************************************/
    typedef enum en_timera_period_match_output
    {
        TimeraPeriodMatchOutputLow = 0u,     ///< TIMA_<t>_PWMn port output low level
        TimeraPeriodMatchOutputHigh = 1u,    ///< TIMA_<t>_PWMn port output high level
        TimeraPeriodMatchOutputKeep = 2u,    ///< TIMA_<t>_PWMn port output to keep
        TimeraPeriodMatchOutputReverse = 3u, ///< TIMA_<t>_PWMn port output reverse
    } en_timera_period_match_output_t;

    /**
     *******************************************************************************
     ** \brief Timera specify output status enumeration
     **
     ** \note _<t>_ is unit number,range 1~6
     ** \note PWMn is channel of unit,range 1-8
     ******************************************************************************/
    typedef enum en_timera_specify_output
    {
        TimeraSpecifyOutputInvalid = 0u, ///< TIMA_<t>_PWMn port output invalid
        TimeraSpecifyOutputLow = 2u,     ///< TIMA_<t>_PWMn port output low level from next period
        TimeraSpecifyOutputHigh = 3u,    ///< TIMA_<t>_PWMn port output high level from next period
    } en_timera_specify_output_t;

    /**
     *******************************************************************************
     ** \brief Timera base init structure definition
     ******************************************************************************/
    typedef struct stc_timera_base_init
    {
        en_timera_clk_div_t enClkDiv;          ///< Count clock division select,This is invalid when counting internal or external event
        en_timera_count_mode_t enCntMode;      ///< Timera count mode
        en_timera_count_dir_t enCntDir;        ///< Timera count direction
        en_functional_state_t enSyncStartupEn; ///< Enable/disable synchronization startup when unit 1 startup,unit 1 set bit invalid
        uint16_t u16PeriodVal;                 ///< Period value
    } stc_timera_base_init_t;

    /**
     *******************************************************************************
     ** \brief Timera compare output init structure definition
     ******************************************************************************/
    typedef struct stc_timera_compare_init
    {
        uint16_t u16CompareVal;                                ///< Compare value
        en_timera_count_start_output_t enStartCountOutput;     ///< Port status set when count start
        en_timera_count_stop_output_t enStopCountOutput;       ///< Port status set when count stop
        en_timera_compare_match_output_t enCompareMatchOutput; ///< Port status set when compare value match
        en_timera_period_match_output_t enPeriodMatchOutput;   ///< Port status set when period value match
        en_timera_specify_output_t enSpecifyOutput;            ///< Specify port status,next period valid,priority more than other port status set
        en_functional_state_t enCacheEn;                       ///< Enable/Disable cache��Only unit 1��3��5��7 valid
        en_functional_state_t enTriangularTroughTransEn;       ///< Enable/Disable triangular wave trough transmit cache value��Only unit 1��3��5��7 valid
        en_functional_state_t enTriangularCrestTransEn;        ///< Enable/Disable triangular wave crest transmit cache value��Only unit 1��3��5��7 valid
        uint16_t u16CompareCacheVal;                           ///< Compare cache value,Only unit 1��3��5��7 valid
    } stc_timera_compare_init_t;

    /**
     *******************************************************************************
     ** \brief Timera capture input init structure definition
     ******************************************************************************/
    typedef struct stc_timera_capture_init
    {
        en_functional_state_t enCapturePwmRisingEn;    ///< Enable/Disable capture channel n active when TIMA_<t>_PWMn sample rising
        en_functional_state_t enCapturePwmFallingEn;   ///< Enable/Disable capture channel n active when TIMA_<t>_PWMn sample falling
        en_functional_state_t enCaptureSpecifyEventEn; ///< Enable/Disable capture channel n active when specify event trigger,event value is TMRA_HTSSR1
        en_timera_filter_clk_div_t enPwmClkDiv;        ///< TIMA_<t>_PWMn filter clock select
        en_functional_state_t enPwmFilterEn;           ///< Enable/Disable TIMA_<t>_PWMn filter functions
        en_functional_state_t enCaptureTrigRisingEn;   ///< Enable/Disable capture channel 4 active when TIMA_<t>_TRIG sample rising, only CCONR4 valid
        en_functional_state_t enCaptureTrigFallingEn;  ///< Enable/Disable capture channel 4 active when TIMA_<t>_TRIG sample falling, only CCONR4 valid
        en_timera_filter_clk_div_t enTrigClkDiv;       ///< TIMA_<t>_TRIG filter clock select, only CCONR4 valid
        en_functional_state_t enTrigFilterEn;          ///< Enable/Disable TIMA_<t>_TRIG filter functions , only CCONR4 valid
    } stc_timera_capture_init_t;

    /**
     *******************************************************************************
     ** \brief Timera Orthogonal coding init structure definition
     **
     ** \note _<t>_ is unit number,range 1~6
     ** \note PWMn is channel of unit,range 1-8
     ** \note n=2��4��6 when m=1��3��5 or n=1��3��5 when m=2��4��6
     ******************************************************************************/
    typedef struct stc_timera_orthogonal_coding_init
    {
        en_functional_state_t enIncClkALowAndClkBRisingEn;   ///< TIMA_<t>_CLKB sample rising edge hardware increase when TIMA_<t>_CLKA is low level
        en_functional_state_t enIncClkALowAndClkBFallingEn;  ///< TIMA_<t>_CLKB sample falling edge hardware increase when TIMA_<t>_CLKA is low level
        en_functional_state_t enIncClkAHighAndClkBRisingEn;  ///< TIMA_<t>_CLKB sample rising edge hardware increase when TIMA_<t>_CLKA is high level
        en_functional_state_t enIncClkAHighAndClkBFallingEn; ///< TIMA_<t>_CLKB sample falling edge hardware increase when TIMA_<t>_CLKA is high level
        en_functional_state_t enIncClkBLowAndClkARisingEn;   ///< TIMA_<t>_CLKA sample rising edge hardware increase when TIMA_<t>_CLKB is low level
        en_functional_state_t enIncClkBLowAndClkAFallingEn;  ///< TIMA_<t>_CLKA sample falling edge hardware increase when TIMA_<t>_CLKB is low level
        en_functional_state_t enIncClkBHighAndClkARisingEn;  ///< TIMA_<t>_CLKA sample rising edge hardware increase when TIMA_<t>_CLKB is high level
        en_functional_state_t enIncClkBHighAndClkAFallingEn; ///< TIMA_<t>_CLKA sample falling edge hardware increase when TIMA_<t>_CLKB is high level
        en_functional_state_t enIncTrigRisingEn;             ///< TIMA_<t>_TRIG sample rising edge hardware increase
        en_functional_state_t enIncTrigFallingEn;            ///< TIMA_<t>_TRIG sample falling edge hardware increase
        en_functional_state_t enIncSpecifyEventTriggerEn;    ///< TIMA_HTSSR0 register Specify event trigger hardware increase
        en_functional_state_t enIncAnotherUnitOverflowEn;    ///< Unit n generate count overflow hardware increase when current unit is m.
        en_functional_state_t enIncAnotherUnitUnderflowEn;   ///< Unit n generate count underflow hardware increase when current unit is m.
        en_functional_state_t enDecClkALowAndClkBRisingEn;   ///< TIMA_<t>_CLKB sample rising edge hardware increase when TIMA_<t>_CLKA is low level
        en_functional_state_t enDecClkALowAndClkBFallingEn;  ///< TIMA_<t>_CLKB sample falling edge hardware increase when TIMA_<t>_CLKA is low level
        en_functional_state_t enDecClkAHighAndClkBRisingEn;  ///< TIMA_<t>_CLKB sample rising edge hardware increase when TIMA_<t>_CLKA is high level
        en_functional_state_t enDecClkAHighAndClkBFallingEn; ///< TIMA_<t>_CLKB sample falling edge hardware increase when TIMA_<t>_CLKA is high level
        en_functional_state_t enDecClkBLowAndClkARisingEn;   ///< TIMA_<t>_CLKA sample rising edge hardware increase when TIMA_<t>_CLKB is low level
        en_functional_state_t enDecClkBLowAndClkAFallingEn;  ///< TIMA_<t>_CLKA sample falling edge hardware increase when TIMA_<t>_CLKB is low level
        en_functional_state_t enDecClkBHighAndClkARisingEn;  ///< TIMA_<t>_CLKA sample rising edge hardware increase when TIMA_<t>_CLKB is high level
        en_functional_state_t enDecClkBHighAndClkAFallingEn; ///< TIMA_<t>_CLKA sample falling edge hardware increase when TIMA_<t>_CLKB is high level
        en_functional_state_t enDecTrigRisingEn;             ///< TIMA_<t>_TRIG sample rising edge hardware increase
        en_functional_state_t enDecTrigFallingEn;            ///< TIMA_<t>_TRIG sample falling edge hardware increase
        en_functional_state_t enDecSpecifyEventTriggerEn;    ///< TIMA_HTSSR0 register Specify event trigger hardware increase
        en_functional_state_t enDecAnotherUnitUnderflowEn;   ///< Unit n generate count overflow hardware increase when current unit is m.
        en_functional_state_t enDecAnotherUnitOverflowEn;    ///< Unit n generate count underflow hardware increase when current unit is m.
        en_timera_filter_clk_div_t enClkAClkDiv;             ///< TIMA_<t>_CLKA filter clock select
        en_functional_state_t enClkAFilterEn;                ///< Enable/Disable TIMA_<t>_CLKA filter functions
        en_timera_filter_clk_div_t enClkBClkDiv;             ///< TIMA_<t>_CLKB filter clock select
        en_functional_state_t enClkBFilterEn;                ///< Enable/Disable TIMA_<t>_CLKB filter functions
        en_timera_filter_clk_div_t enTrigClkDiv;             ///< TIMA_<t>_TRIG filter clock select
        en_functional_state_t enTrigFilterEn;                ///< Enable/Disable TIMA_<t>_TRIG filter functions
    } stc_timera_orthogonal_coding_init_t;

    /**
     *******************************************************************************
     ** \brief Timera hardware startup config structure definition
     **
     ** \note _<t>_ is unit number,range 1~6
     ** \note TMRA_HTSSR0 trigger startup only when unit 2~6 valid,unit 1 is invalid
     ******************************************************************************/
    typedef struct stc_timera_hw_startup_cofig
    {
        en_functional_state_t enTrigRisingStartupEn;   ///< Hardware startup TIMA_<t> when TIMA_<t>_TRIG sample rising edge(sync start valid)
        en_functional_state_t enTrigFallingStartupEn;  ///< Hardware startup TIMA_<t> when TIMA_<t>_TRIG sample falling edge(sync start valid)
        en_functional_state_t enSpecifyEventStartupEn; ///< Hardware startup TIMA_<t> when TIMA_HTSSR0 register Specify event trigger
    } stc_timera_hw_startup_cofig_t;

    /**
     *******************************************************************************
     ** \brief Timera hardware stop config structure definition
     **
     ** \note _<t>_ is unit number,range 1~6
     ******************************************************************************/
    typedef struct stc_timera_hw_stop_cofig
    {
        en_functional_state_t enTrigRisingStopEn;   ///< Hardware stop TIMA_<t> when TIMA_<t>_TRIG sample rising edge
        en_functional_state_t enTrigFallingStopEn;  ///< Hardware stop TIMA_<t> when TIMA_<t>_TRIG sample falling edge
        en_functional_state_t enSpecifyEventStopEn; ///< Hardware stop TIMA_<t> when TIMA_HTSSR0 register Specify event trigger
    } stc_timera_hw_stop_cofig_t;

    /**
     *******************************************************************************
     ** \brief Timera hardware clear config structure definition
     **
     ** \note _<t>_ is unit number,range 1~6
     ** \note n=2��4��6 when m=1��3��5 or n=1��3��5 when m=2��4��6
     ******************************************************************************/
    typedef struct stc_timera_hw_clear_config
    {
        en_functional_state_t enTrigRisingClearEn;             ///< Hardware clear TIMA_<t> when TIMA_<t>_TRIG sample rising edge
        en_functional_state_t enTrigFallingClearEn;            ///< Hardware clear TIMA_<t> when TIMA_<t>_TRIG sample falling edge
        en_functional_state_t enSpecifyEventClearEn;           ///< Hardware clear TIMA_<t> when TIMA_HTSSR0 register Specify event trigger
        en_functional_state_t enAnotherUnitTrigRisingClearEn;  ///< Hardware clear TIMA_<t> when unit n TRIG port sample rising when current unit is m.
        en_functional_state_t enAnotherUnitTrigFallingClearEn; ///< Hardware clear TIMA_<t> when unit n TRIG port sample falling when current unit is m.
        en_functional_state_t enChannel3RisingClearEn;         ///< Hardware clear TIMA_<t> when TIMA_<t>_PWM3 sample rising edge
        en_functional_state_t enChannel3FallingClearEn;        ///< Hardware clear TIMA_<t> when TIMA_<t>_PWM3 sample falling edge
    } stc_timera_hw_clear_config_t;

    /**
     *******************************************************************************
     ** \brief Timera hardware trigger init structure definition
     ******************************************************************************/
    typedef struct stc_timera_hw_trigger_init
    {
        stc_timera_hw_startup_cofig_t stcHwStartup; ///< Hardware startup condition config
        stc_timera_hw_stop_cofig_t stcHwStop;       ///< Hardware stop condition config
        stc_timera_hw_clear_config_t stcHwClear;    ///< Hardware clear condition config
    } stc_timera_hw_trigger_init_t;

    /*******************************************************************************
     * Global pre-processor symbols/macros ('#define')
     ******************************************************************************/

    /*******************************************************************************
     * Global variable definitions ('extern')
     ******************************************************************************/

    /*******************************************************************************
      Global function prototypes (definition in C source)
     ******************************************************************************/
    /* Base functions */
    en_result_t TIMERA_DeInit(M4_TMRA_TypeDef *TIMERAx);
    en_result_t TIMERA_BaseInit(M4_TMRA_TypeDef *TIMERAx, const stc_timera_base_init_t *pstcBaseInit);
    en_result_t TIMERA_SetCurrCount(M4_TMRA_TypeDef *TIMERAx, uint16_t u16Cnt);
    uint16_t TIMERA_GetCurrCount(M4_TMRA_TypeDef *TIMERAx);
    en_result_t TIMERA_SetPeriodValue(M4_TMRA_TypeDef *TIMERAx, uint16_t u16Period);
    uint16_t TIMERA_GetPeriodValue(M4_TMRA_TypeDef *TIMERAx);
    en_result_t TIMERA_SyncStartupCmd(M4_TMRA_TypeDef *TIMERAx, en_functional_state_t enNewSta);
    en_result_t TIMERA_Cmd(M4_TMRA_TypeDef *TIMERAx, en_functional_state_t enNewSta);

    /* Compare output functions */
    en_result_t TIMERA_CompareInit(M4_TMRA_TypeDef *TIMERAx, en_timera_channel_t enChannel,
                                   const stc_timera_compare_init_t *pstcCompareInit);
    en_result_t TIMERA_SetCompareValue(M4_TMRA_TypeDef *TIMERAx, en_timera_channel_t enChannel,
                                       uint16_t u16CompareVal);
    uint16_t TIMERA_GetCompareValue(M4_TMRA_TypeDef *TIMERAx, en_timera_channel_t enChannel);
    en_result_t TIMERA_SetCacheValue(M4_TMRA_TypeDef *TIMERAx, en_timera_channel_t enChannel,
                                     uint16_t u16CompareCache);
    en_result_t TIMERA_CompareCacheCmd(M4_TMRA_TypeDef *TIMERAx, en_timera_channel_t enChannel,
                                       en_functional_state_t enNewSta);
    en_result_t TIMERA_SpecifyOutputSta(M4_TMRA_TypeDef *TIMERAx, en_timera_channel_t enChannel,
                                        en_timera_specify_output_t enOutputSta);
    en_result_t TIMERA_CompareCmd(M4_TMRA_TypeDef *TIMERAx, en_timera_channel_t enChannel,
                                  en_functional_state_t enNewSta);

    /* Capture input functions */
    en_result_t TIMERA_CaptureInit(M4_TMRA_TypeDef *TIMERAx, en_timera_channel_t enChannel,
                                   const stc_timera_capture_init_t *pstcCapInit);
    en_result_t TIMERA_CaptureFilterCmd(M4_TMRA_TypeDef *TIMERAx, en_timera_filter_source_t enFilterPort,
                                        en_functional_state_t enNewSta);
    uint16_t TIMERA_GetCaptureValue(M4_TMRA_TypeDef *TIMERAx, en_timera_channel_t enChannel);

    /* Orthogonal coding functions */
    en_result_t TIMERA_OrthogonalCodingInit(M4_TMRA_TypeDef *TIMERAx, const stc_timera_orthogonal_coding_init_t *pstcCodingInit);
    en_result_t TIMERA_SetOrthogonalCodingCount(M4_TMRA_TypeDef *TIMERAx, uint16_t u16CodingCnt);
    uint16_t TIMERA_GetOrthogonalCodingCount(M4_TMRA_TypeDef *TIMERAx);
    en_result_t TIMERA_OrthogonalCodingFilterCmd(M4_TMRA_TypeDef *TIMERAx, en_timera_filter_source_t enFilterPort,
                                                 en_functional_state_t enNewSta);

    /* Hardware control functions */
    en_result_t TIMERA_HwTriggerInit(M4_TMRA_TypeDef *TIMERAx, const stc_timera_hw_trigger_init_t *pstcHwTriggerInit);
    en_result_t TIMERA_HwStartupConfig(M4_TMRA_TypeDef *TIMERAx, const stc_timera_hw_startup_cofig_t *pstcHwStartup);
    en_result_t TIMERA_HwStopConfig(M4_TMRA_TypeDef *TIMERAx, const stc_timera_hw_stop_cofig_t *pstcHwStop);
    en_result_t TIMERA_HwClearConfig(M4_TMRA_TypeDef *TIMERAx, const stc_timera_hw_clear_config_t *pstcHwClear);

    /* interrupt and flags functions */
    en_result_t TIMERA_IrqCmd(M4_TMRA_TypeDef *TIMERAx, en_timera_irq_type_t enIrq,
                              en_functional_state_t enNewSta);
    en_flag_status_t TIMERA_GetFlag(M4_TMRA_TypeDef *TIMERAx, en_timera_flag_type_t enFlag);
    en_result_t TIMERA_ClearFlag(M4_TMRA_TypeDef *TIMERAx, en_timera_flag_type_t enFlag);

    /* Event config functions */
    en_result_t TIMERA_EventCmd(M4_TMRA_TypeDef *TIMERAx, en_timera_channel_t enChannel,
                                en_functional_state_t enNewSta);
    en_result_t TIMERA_SetCountTriggerSrc(en_event_src_t enTriggerSrc);
    en_result_t TIMERA_SetCaptureTriggerSrc(en_event_src_t enTriggerSrc);

    //@} // TimeraGroup

#ifdef __cplusplus
}
#endif

#endif /* DDL_TIMERA_ENABLE */

#endif /* __HC32F46x_TIMERA_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
