/*****************************************************************************
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
/** \file hc32f46x_event_port.h
 **
 ** A detailed description is available at
 ** @link EventPortGroup EventPort description @endlink
 **
 **   - 2018-12-07  1.0  Zhangxl First version for Device Driver Library of EventPort.
 **
 ******************************************************************************/

#ifndef __HC32F46X_EVENT_PORT_H__
#define __HC32F46X_EVENT_PORT_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"

#if (DDL_EVENT_PORT_ENABLE == DDL_ON)

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

    /**
     *******************************************************************************
     ** \defgroup EventPortGroup Event Port (EventPort)
     **
     ******************************************************************************/
    //@{

    /*******************************************************************************
     * Global type definitions ('typedef')
     ******************************************************************************/
    /**
     *******************************************************************************
     ** \brief Event Port Index enumeration
     **
     ******************************************************************************/
    typedef enum en_event_port
    {
        EventPort1 = 1, ///< Event port 1
        EventPort2 = 2, ///< Event port 2
        EventPort3 = 3, ///< Event port 3
        EventPort4 = 4, ///< Event port 4
    } en_event_port_t;

    /**
     *******************************************************************************
     ** \brief Event Port Pin enumeration
     **
     ******************************************************************************/
    typedef enum en_event_pin
    {
        EventPin00 = 1u << 0,  ///< Event port Pin 00
        EventPin01 = 1u << 1,  ///< Event port Pin 01
        EventPin02 = 1u << 2,  ///< Event port Pin 02
        EventPin03 = 1u << 3,  ///< Event port Pin 03
        EventPin04 = 1u << 4,  ///< Event port Pin 04
        EventPin05 = 1u << 5,  ///< Event port Pin 05
        EventPin06 = 1u << 6,  ///< Event port Pin 06
        EventPin07 = 1u << 7,  ///< Event port Pin 07
        EventPin08 = 1u << 8,  ///< Event port Pin 08
        EventPin09 = 1u << 9,  ///< Event port Pin 09
        EventPin10 = 1u << 10, ///< Event port Pin 10
        EventPin11 = 1u << 11, ///< Event port Pin 11
        EventPin12 = 1u << 12, ///< Event port Pin 12
        EventPin13 = 1u << 13, ///< Event port Pin 13
        EventPin14 = 1u << 14, ///< Event port Pin 14
        EventPin15 = 1u << 15, ///< Event port Pin 15
        EventPinAll = 0xFFFF,  ///< All event pins are selected
    } en_event_pin_t;

    /**
     *******************************************************************************
     ** \brief Event Port direction enumeration
     **
     ******************************************************************************/
    typedef enum en_event_port_dir
    {
        EventPortIn = 0,  ///< Event Port direction 'IN'
        EventPortOut = 1, ///< Event Port direction 'OUT'
    } en_event_port_dir_t;

    /**
     *******************************************************************************
     ** \brief Enumeration to filter clock setting for Event port detect
     **
     ** \note
     ******************************************************************************/
    typedef enum en_ep_flt_clk
    {
        Pclk1Div1 = 0u,  ///< PCLK1 as EP filter clock source
        Pclk1Div8 = 1u,  ///< PCLK1 div8 as EP filter clock source
        Pclk1Div32 = 2u, ///< PCLK1 div32 as EP filter clock source
        Pclk1Div64 = 3u, ///< PCLK1 div64 as EP filter clock source
    } en_ep_flt_clk_t;

    /**
     *******************************************************************************
     ** \brief Event port init structure definition
     ******************************************************************************/
    typedef struct stc_event_port_init
    {
        en_event_port_dir_t enDirection;       ///< Input/Output setting
        en_functional_state_t enReset;         ///< Corresponding pin reset after triggered
        en_functional_state_t enSet;           ///< Corresponding pin set after triggered
        en_functional_state_t enRisingDetect;  ///< Rising edge detect enable
        en_functional_state_t enFallingDetect; ///< Falling edge detect enable
        en_functional_state_t enFilter;        ///< Filter clock source select
        en_ep_flt_clk_t enFilterClk;           ///< Filter clock, ref@ en_ep_flt_clk_t for details
    } stc_event_port_init_t;

    /*******************************************************************************
     * Global pre-processor symbols/macros ('#define')
     ******************************************************************************/

    /*******************************************************************************
     * Global variable definitions ('extern')
     ******************************************************************************/

    /*******************************************************************************
      Global function prototypes (definition in C source)
     ******************************************************************************/
    extern en_result_t EVENTPORT_Init(en_event_port_t enEventPort,
                                      uint16_t u16EventPin, const stc_event_port_init_t *pstcEventPortInit);
    extern en_result_t EVENTPORT_DeInit(void);
    extern en_result_t EVENTPORT_SetTriggerSrc(en_event_port_t enEventPort,
                                               en_event_src_t enTriggerSrc);
    extern uint16_t EVENTPORT_GetData(en_event_port_t enEventPort);
    extern en_flag_status_t EVENTPORT_GetBit(en_event_port_t enEventPort,
                                             en_event_pin_t enEventPin);
    extern en_result_t EVENTPORT_SetBits(en_event_port_t enEventPort,
                                         en_event_pin_t u16EventPin);
    extern en_result_t EVENTPORT_ResetBits(en_event_port_t enEventPort,
                                           en_event_pin_t u16EventPin);

    //@} // EventPortGroup

#ifdef __cplusplus
}
#endif

#endif /* DDL_EVENT_PORT_ENABLE */

#endif /* __HC32F46X_EVENT_PORT_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
