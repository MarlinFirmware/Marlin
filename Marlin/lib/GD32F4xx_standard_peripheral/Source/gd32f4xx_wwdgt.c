/*!
    \file    gd32f4xx_wwdgt.c
    \brief   WWDGT driver
    
    \version 2016-08-15, V1.0.0, firmware for GD32F4xx
    \version 2018-12-12, V2.0.0, firmware for GD32F4xx
    \version 2020-09-30, V2.1.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "gd32f4xx_wwdgt.h"

/* write value to WWDGT_CTL_CNT bit field */
#define CTL_CNT(regval)             (BITS(0,6) & ((uint32_t)(regval) << 0))
/* write value to WWDGT_CFG_WIN bit field */
#define CFG_WIN(regval)             (BITS(0,6) & ((uint32_t)(regval) << 0))

/*!
    \brief      reset the window watchdog timer configuration
    \param[in]  none
    \param[out] none
    \retval     none
*/
void wwdgt_deinit(void)
{
    rcu_periph_reset_enable(RCU_WWDGTRST);
    rcu_periph_reset_disable(RCU_WWDGTRST);
}

/*!
    \brief      start the window watchdog timer counter
    \param[in]  none
    \param[out] none
    \retval     none
*/
void wwdgt_enable(void)
{
    WWDGT_CTL |= WWDGT_CTL_WDGTEN;
}

/*!
    \brief      configure the window watchdog timer counter value
    \param[in]  counter_value: 0x00 - 0x7F
    \param[out] none
    \retval     none
*/
void wwdgt_counter_update(uint16_t counter_value)
{
    uint32_t reg = 0U;
    
    reg = (WWDGT_CTL & (~WWDGT_CTL_CNT));
    reg |= CTL_CNT(counter_value);
    
    WWDGT_CTL = reg;
}

/*!
    \brief      configure counter value, window value, and prescaler divider value  
    \param[in]  counter: 0x00 - 0x7F   
    \param[in]  window: 0x00 - 0x7F
    \param[in]  prescaler: wwdgt prescaler value
                only one parameter can be selected which is shown as below:
      \arg        WWDGT_CFG_PSC_DIV1: the time base of window watchdog counter = (PCLK1/4096)/1
      \arg        WWDGT_CFG_PSC_DIV2: the time base of window watchdog counter = (PCLK1/4096)/2
      \arg        WWDGT_CFG_PSC_DIV4: the time base of window watchdog counter = (PCLK1/4096)/4
      \arg        WWDGT_CFG_PSC_DIV8: the time base of window watchdog counter = (PCLK1/4096)/8
    \param[out] none
    \retval     none
*/
void wwdgt_config(uint16_t counter, uint16_t window, uint32_t prescaler)
{
    uint32_t reg_cfg = 0U, reg_ctl = 0U;

    /* clear WIN and PSC bits, clear CNT bit */
    reg_cfg = (WWDGT_CFG &(~(WWDGT_CFG_WIN|WWDGT_CFG_PSC)));
    reg_ctl = (WWDGT_CTL &(~WWDGT_CTL_CNT));
  
    /* configure WIN and PSC bits, configure CNT bit */
    reg_cfg |= CFG_WIN(window);
    reg_cfg |= prescaler;
    reg_ctl |= CTL_CNT(counter);
    
    WWDGT_CTL = reg_ctl;
    WWDGT_CFG = reg_cfg;
}

/*!
    \brief      enable early wakeup interrupt of WWDGT
    \param[in]  none
    \param[out] none
    \retval     none
*/
void wwdgt_interrupt_enable(void)
{
    WWDGT_CFG |= WWDGT_CFG_EWIE;
}

/*!
    \brief      check early wakeup interrupt state of WWDGT
    \param[in]  none
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus wwdgt_flag_get(void)
{
    if(RESET != (WWDGT_STAT & WWDGT_STAT_EWIF)){
        return SET;
    }

    return RESET;
}

/*!
    \brief      clear early wakeup interrupt state of WWDGT
    \param[in]  none
    \param[out] none
    \retval     none
*/
void wwdgt_flag_clear(void)
{
    WWDGT_STAT &= (~WWDGT_STAT_EWIF);
}
