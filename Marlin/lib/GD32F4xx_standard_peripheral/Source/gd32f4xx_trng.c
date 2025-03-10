/*!
    \file    gd32f4xx_trng.c
    \brief   TRNG driver

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

#include "gd32f4xx_trng.h"

/*!
    \brief      deinitialize the TRNG
    \param[in]  none
    \param[out] none
    \retval     none
*/
void trng_deinit(void)
{
    rcu_periph_reset_enable(RCU_TRNGRST);
    rcu_periph_reset_disable(RCU_TRNGRST);
}

/*!
    \brief      enable the TRNG interface
    \param[in]  none
    \param[out] none
    \retval     none
*/
void trng_enable(void)
{
    TRNG_CTL |= TRNG_CTL_TRNGEN;
}

/*!
    \brief      disable the TRNG interface
    \param[in]  none
    \param[out] none
    \retval     none
*/
void trng_disable(void)
{
    TRNG_CTL &= ~TRNG_CTL_TRNGEN;
}

/*!
    \brief      get the true random data
    \param[in]  none
    \param[out] none
    \retval     the generated random data
*/
uint32_t trng_get_true_random_data(void)
{
    return (TRNG_DATA);
}

/*!
    \brief      enable the TRNG interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void trng_interrupt_enable(void) 
{
    TRNG_CTL |= TRNG_CTL_IE;
}

/*!
    \brief      disable the TRNG interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void trng_interrupt_disable(void)
{
    TRNG_CTL &= ~TRNG_CTL_IE;
}

/*!
    \brief      get the trng status flags
    \param[in]  flag: trng status flag, refer to trng_flag_enum
                only one parameter can be selected which is shown as below:
      \arg        TRNG_FLAG_DRDY: Random Data ready status
      \arg        TRNG_FLAG_CECS: Clock error current status
      \arg        TRNG_FLAG_SECS: Seed error current status
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus trng_flag_get(trng_flag_enum flag)
{
    if(RESET != (TRNG_STAT & flag)){
        return SET;
    }else{
        return RESET;
    }
}

/*!
    \brief      get the trng interrupt flags
    \param[in]  int_flag: trng interrupt flag, refer to trng_int_flag_enum
                only one parameter can be selected which is shown as below:
      \arg        TRNG_INT_FLAG_CEIF: clock error interrupt flag
      \arg        TRNG_INT_FLAG_SEIF: Seed error interrupt flag
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus trng_interrupt_flag_get(trng_int_flag_enum int_flag)
{
    if(RESET != (TRNG_STAT & int_flag)){
        return SET;
    }else{
        return RESET;
    }
}

/*!
    \brief      clear the trng interrupt flags
    \param[in]  int_flag: trng interrupt flag, refer to trng_int_flag_enum
                only one parameter can be selected which is shown as below:
      \arg        TRNG_INT_FLAG_CEIF: clock error interrupt flag
      \arg        TRNG_INT_FLAG_SEIF: Seed error interrupt flag
    \param[out] none
    \retval     none
*/
void trng_interrupt_flag_clear(trng_int_flag_enum int_flag)
{
    TRNG_STAT &= ~(uint32_t)int_flag;
}
