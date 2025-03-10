/*!
    \file    gd32f4xx_dac.c
    \brief   DAC driver
    
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

#include "gd32f4xx_dac.h"

/* DAC register bit offset */
#define DAC1_REG_OFFSET           ((uint32_t)16U)
#define DH_12BIT_OFFSET           ((uint32_t)16U)
#define DH_8BIT_OFFSET            ((uint32_t)8U)

/*!
    \brief      deinitialize DAC
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dac_deinit(void)
{
    rcu_periph_reset_enable(RCU_DACRST);
    rcu_periph_reset_disable(RCU_DACRST);
}

/*!
    \brief      enable DAC
    \param[in]  dac_periph: DACx(x = 0,1)
    \param[out] none
    \retval     none
*/
void dac_enable(uint32_t dac_periph)
{
    if(DAC0 == dac_periph){
        DAC_CTL |= DAC_CTL_DEN0;
    }else{
        DAC_CTL |= DAC_CTL_DEN1;
    }
} 

/*!
    \brief      disable DAC
    \param[in]  dac_periph: DACx(x = 0,1)
    \param[out] none
    \retval     none
*/
void dac_disable(uint32_t dac_periph)
{
    if(DAC0 == dac_periph){
        DAC_CTL &= ~DAC_CTL_DEN0;
    }else{
        DAC_CTL &= ~DAC_CTL_DEN1;
    }
}

/*!
    \brief      enable DAC DMA function
    \param[in]  dac_periph: DACx(x = 0,1)
    \param[out] none
    \retval     none
*/
void dac_dma_enable(uint32_t dac_periph)
{
    if(DAC0 == dac_periph){
        DAC_CTL |= DAC_CTL_DDMAEN0;
    }else{
        DAC_CTL |= DAC_CTL_DDMAEN1;
    }
}

/*!
    \brief      disable DAC DMA function
    \param[in]  dac_periph: DACx(x = 0,1)
    \param[out] none
    \retval     none
*/
void dac_dma_disable(uint32_t dac_periph)
{
    if(DAC0 == dac_periph){
        DAC_CTL &= ~DAC_CTL_DDMAEN0;
    }else{
        DAC_CTL &= ~DAC_CTL_DDMAEN1;
    }
}

/*!
    \brief      enable DAC output buffer
    \param[in]  dac_periph: DACx(x = 0,1)
    \param[out] none
    \retval     none
*/
void dac_output_buffer_enable(uint32_t dac_periph)
{
    if(DAC0 == dac_periph){
        DAC_CTL &= ~DAC_CTL_DBOFF0;
    }else{
        DAC_CTL &= ~DAC_CTL_DBOFF1;
    }
}

/*!
    \brief      disable DAC output buffer
    \param[in]  dac_periph: DACx(x = 0,1)
    \param[out] none
    \retval     none
*/
void dac_output_buffer_disable(uint32_t dac_periph)
{
    if(DAC0 == dac_periph){
        DAC_CTL |= DAC_CTL_DBOFF0;
    }else{
        DAC_CTL |= DAC_CTL_DBOFF1;
    }
}

/*!
    \brief      get DAC output value
    \param[in]  dac_periph: DACx(x = 0,1)
    \param[out] none
    \retval     DAC output data
*/
uint16_t dac_output_value_get(uint32_t dac_periph)
{
    uint16_t data = 0U;
    if(DAC0 == dac_periph){
        /* store the DAC0 output value */
        data = (uint16_t)DAC0_DO;
    }else{
        /* store the DAC1 output value */
        data = (uint16_t)DAC1_DO;
    }
    return data;
}

/*!
    \brief      set the DAC specified data holding register value
    \param[in]  dac_periph: DACx(x = 0,1)
    \param[in]  dac_align: data alignment
                only one parameter can be selected which is shown as below:
      \arg        DAC_ALIGN_8B_R: data right 8 bit alignment
      \arg        DAC_ALIGN_12B_R: data right 12 bit alignment
      \arg        DAC_ALIGN_12B_L: data left 12 bit alignment
    \param[in]  data: data to be loaded
    \param[out] none
    \retval     none
*/
void dac_data_set(uint32_t dac_periph, uint32_t dac_align, uint16_t data)
{
    if(DAC0 == dac_periph){
        switch(dac_align){
        /* data right 12 bit alignment */
        case DAC_ALIGN_12B_R:
            DAC0_R12DH = data;
            break;
        /* data left 12 bit alignment */
        case DAC_ALIGN_12B_L:
            DAC0_L12DH = data;
            break;
        /* data right 8 bit alignment */
        case DAC_ALIGN_8B_R:
            DAC0_R8DH = data;
            break;
        default:
            break;
        }
    }else{
        switch(dac_align){
        /* data right 12 bit alignment */
        case DAC_ALIGN_12B_R:
            DAC1_R12DH = data;
            break;
        /* data left 12 bit alignment */
        case DAC_ALIGN_12B_L:
            DAC1_L12DH = data;
            break;
        /* data right 8 bit alignment */
        case DAC_ALIGN_8B_R:
            DAC1_R8DH = data;
            break;
        default:
            break;
        }
    }
}

/*!
    \brief      enable DAC trigger
    \param[in]  dac_periph: DACx(x = 0,1)
    \param[out] none
    \retval     none
*/
void dac_trigger_enable(uint32_t dac_periph)
{
    if(DAC0 == dac_periph){
        DAC_CTL |= DAC_CTL_DTEN0;
    }else{
        DAC_CTL |= DAC_CTL_DTEN1;
    }
}

/*!
    \brief      disable DAC trigger
    \param[in]  dac_periph: DACx(x = 0,1)
    \param[out] none
    \retval     none
*/
void dac_trigger_disable(uint32_t dac_periph)
{
    if(DAC0 == dac_periph){
        DAC_CTL &= ~DAC_CTL_DTEN0;
    }else{
        DAC_CTL &= ~DAC_CTL_DTEN1;
    }
}

/*!
    \brief      set DAC trigger source
    \param[in]  dac_periph: DACx(x = 0,1)
    \param[in]  triggersource: external triggers of DAC
                only one parameter can be selected which is shown as below:
      \arg        DAC_TRIGGER_T1_TRGO: TIMER1 TRGO
      \arg        DAC_TRIGGER_T3_TRGO: TIMER3 TRGO
      \arg        DAC_TRIGGER_T4_TRGO: TIMER4 TRGO
      \arg        DAC_TRIGGER_T5_TRGO: TIMER5 TRGO
      \arg        DAC_TRIGGER_T6_TRGO: TIMER6 TRGO
      \arg        DAC_TRIGGER_T7_TRGO: TIMER7 TRGO
      \arg        DAC_TRIGGER_EXTI_9: EXTI interrupt line9 event
      \arg        DAC_TRIGGER_SOFTWARE: software trigger
    \param[out] none
    \retval     none
*/
void dac_trigger_source_config(uint32_t dac_periph,uint32_t triggersource)
{
    if(DAC0 == dac_periph){
        /* configure DAC0 trigger source */
        DAC_CTL &= ~DAC_CTL_DTSEL0;
        DAC_CTL |= triggersource;
    }else{
        /* configure DAC1 trigger source */
        DAC_CTL &= ~DAC_CTL_DTSEL1;
        DAC_CTL |= (triggersource << DAC1_REG_OFFSET);
    }
}

/*!
    \brief      enable DAC software trigger
    \param[in]  dac_periph: DACx(x = 0,1)
    \retval     none
*/
void dac_software_trigger_enable(uint32_t dac_periph)
{
    if(DAC0 == dac_periph){
        DAC_SWT |= DAC_SWT_SWTR0;
    }else{
        DAC_SWT |= DAC_SWT_SWTR1;
    }
}

/*!
    \brief      disable DAC software trigger
    \param[in]  dac_periph: DACx(x = 0,1)
    \param[out] none
    \retval     none
*/
void dac_software_trigger_disable(uint32_t dac_periph)
{
    if(DAC0 == dac_periph){
        DAC_SWT &= ~DAC_SWT_SWTR0;
    }else{
        DAC_SWT &= ~DAC_SWT_SWTR1;
    }
}

/*!
    \brief      configure DAC wave mode
    \param[in]  dac_periph: DACx(x = 0,1)
    \param[in]  wave_mode: noise wave mode
                only one parameter can be selected which is shown as below:
      \arg        DAC_WAVE_DISABLE: wave disable
      \arg        DAC_WAVE_MODE_LFSR: LFSR noise mode
      \arg        DAC_WAVE_MODE_TRIANGLE: triangle noise mode
    \param[out] none
    \retval     none
*/
void dac_wave_mode_config(uint32_t dac_periph, uint32_t wave_mode)
{
    if(DAC0 == dac_periph){
        /* configure DAC0 wave mode */
        DAC_CTL &= ~DAC_CTL_DWM0;
        DAC_CTL |= wave_mode;
    }else{
        /* configure DAC1 wave mode */
        DAC_CTL &= ~DAC_CTL_DWM1;
        DAC_CTL |= (wave_mode << DAC1_REG_OFFSET);
    }
}

/*!
    \brief      configure DAC wave bit width
    \param[in]  dac_periph: DACx(x = 0,1)
    \param[in]  bit_width: noise wave bit width
                only one parameter can be selected which is shown as below:
      \arg        DAC_WAVE_BIT_WIDTH_1: bit width of the wave signal is 1
      \arg        DAC_WAVE_BIT_WIDTH_2: bit width of the wave signal is 2
      \arg        DAC_WAVE_BIT_WIDTH_3: bit width of the wave signal is 3
      \arg        DAC_WAVE_BIT_WIDTH_4: bit width of the wave signal is 4
      \arg        DAC_WAVE_BIT_WIDTH_5: bit width of the wave signal is 5
      \arg        DAC_WAVE_BIT_WIDTH_6: bit width of the wave signal is 6
      \arg        DAC_WAVE_BIT_WIDTH_7: bit width of the wave signal is 7
      \arg        DAC_WAVE_BIT_WIDTH_8: bit width of the wave signal is 8
      \arg        DAC_WAVE_BIT_WIDTH_9: bit width of the wave signal is 9
      \arg        DAC_WAVE_BIT_WIDTH_10: bit width of the wave signal is 10
      \arg        DAC_WAVE_BIT_WIDTH_11: bit width of the wave signal is 11
      \arg        DAC_WAVE_BIT_WIDTH_12: bit width of the wave signal is 12
    \param[out] none
    \retval     none
*/
void dac_wave_bit_width_config(uint32_t dac_periph, uint32_t bit_width)
{
    if(DAC0 == dac_periph){
        /* configure DAC0 wave bit width */
        DAC_CTL &= ~DAC_CTL_DWBW0;
        DAC_CTL |= bit_width;
    }else{
        /* configure DAC1 wave bit width */
        DAC_CTL &= ~DAC_CTL_DWBW1;
        DAC_CTL |= (bit_width << DAC1_REG_OFFSET);
    }
}

/*!
    \brief      configure DAC LFSR noise mode
    \param[in]  dac_periph: DACx(x = 0,1)
    \param[in]  unmask_bits: unmask LFSR bits in DAC LFSR noise mode
                only one parameter can be selected which is shown as below:
      \arg        DAC_LFSR_BIT0: unmask the LFSR bit0
      \arg        DAC_LFSR_BITS1_0: unmask the LFSR bits[1:0]
      \arg        DAC_LFSR_BITS2_0: unmask the LFSR bits[2:0]
      \arg        DAC_LFSR_BITS3_0: unmask the LFSR bits[3:0]
      \arg        DAC_LFSR_BITS4_0: unmask the LFSR bits[4:0]
      \arg        DAC_LFSR_BITS5_0: unmask the LFSR bits[5:0]
      \arg        DAC_LFSR_BITS6_0: unmask the LFSR bits[6:0]
      \arg        DAC_LFSR_BITS7_0: unmask the LFSR bits[7:0]
      \arg        DAC_LFSR_BITS8_0: unmask the LFSR bits[8:0]
      \arg        DAC_LFSR_BITS9_0: unmask the LFSR bits[9:0]
      \arg        DAC_LFSR_BITS10_0: unmask the LFSR bits[10:0]
      \arg        DAC_LFSR_BITS11_0: unmask the LFSR bits[11:0]
    \param[out] none
    \retval     none
*/
void dac_lfsr_noise_config(uint32_t dac_periph, uint32_t unmask_bits)
{
    if(DAC0 == dac_periph){
        /* configure DAC0 LFSR noise mode */
        DAC_CTL &= ~DAC_CTL_DWBW0;
        DAC_CTL |= unmask_bits;
    }else{
        /* configure DAC1 LFSR noise mode */
        DAC_CTL &= ~DAC_CTL_DWBW1;
        DAC_CTL |= (unmask_bits << DAC1_REG_OFFSET);
    }
}

/*!
    \brief      configure DAC triangle noise mode
    \param[in]  dac_periph: DACx(x = 0,1)
    \param[in]  amplitude: triangle amplitude in DAC triangle noise mode
                only one parameter can be selected which is shown as below:
      \arg        DAC_TRIANGLE_AMPLITUDE_1: triangle amplitude is 1
      \arg        DAC_TRIANGLE_AMPLITUDE_3: triangle amplitude is 3
      \arg        DAC_TRIANGLE_AMPLITUDE_7: triangle amplitude is 7
      \arg        DAC_TRIANGLE_AMPLITUDE_15: triangle amplitude is 15
      \arg        DAC_TRIANGLE_AMPLITUDE_31: triangle amplitude is 31
      \arg        DAC_TRIANGLE_AMPLITUDE_63: triangle amplitude is 63
      \arg        DAC_TRIANGLE_AMPLITUDE_127: triangle amplitude is 127
      \arg        DAC_TRIANGLE_AMPLITUDE_255: triangle amplitude is 255
      \arg        DAC_TRIANGLE_AMPLITUDE_511: triangle amplitude is 511
      \arg        DAC_TRIANGLE_AMPLITUDE_1023: triangle amplitude is 1023
      \arg        DAC_TRIANGLE_AMPLITUDE_2047: triangle amplitude is 2047
      \arg        DAC_TRIANGLE_AMPLITUDE_4095: triangle amplitude is 4095
    \param[out] none
    \retval     none
*/
void dac_triangle_noise_config(uint32_t dac_periph, uint32_t amplitude)
{
    if(DAC0 == dac_periph){
        /* configure DAC0 triangle noise mode */
        DAC_CTL &= ~DAC_CTL_DWBW0;
        DAC_CTL |= amplitude;
    }else{
        /* configure DAC1 triangle noise mode */
        DAC_CTL &= ~DAC_CTL_DWBW1;
        DAC_CTL |= (amplitude << DAC1_REG_OFFSET);
    }
}

/*!
    \brief      enable DAC concurrent mode
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dac_concurrent_enable(void)
{
    uint32_t ctl = 0U;
    ctl = DAC_CTL_DEN0 | DAC_CTL_DEN1;
    DAC_CTL |= (ctl);
}

/*!
    \brief      disable DAC concurrent mode
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dac_concurrent_disable(void)
{
    uint32_t ctl = 0U;
    ctl = DAC_CTL_DEN0 | DAC_CTL_DEN1;
    DAC_CTL &= (~ctl);
}

/*!
    \brief      enable DAC concurrent software trigger function
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dac_concurrent_software_trigger_enable(void)
{
    uint32_t swt = 0U;
    swt = DAC_SWT_SWTR0 | DAC_SWT_SWTR1;
    DAC_SWT |= (swt); 
}

/*!
    \brief      disable DAC concurrent software trigger function
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dac_concurrent_software_trigger_disable(void)
{
    uint32_t swt = 0U;
    swt = DAC_SWT_SWTR0 | DAC_SWT_SWTR1;
    DAC_SWT &= (~swt);
}

/*!
    \brief      enable DAC concurrent buffer function
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dac_concurrent_output_buffer_enable(void)
{
    uint32_t ctl = 0U;
    ctl = DAC_CTL_DBOFF0 | DAC_CTL_DBOFF1;
    DAC_CTL &= (~ctl);
}

/*!
    \brief      disable DAC concurrent buffer function
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dac_concurrent_output_buffer_disable(void)
{
    uint32_t ctl = 0U;
    ctl = DAC_CTL_DBOFF0 | DAC_CTL_DBOFF1;
    DAC_CTL |= (ctl);
}

/*!
    \brief      set DAC concurrent mode data holding register value
    \param[in]  dac_align: data alignment
                only one parameter can be selected which is shown as below:
      \arg        DAC_ALIGN_8B_R: data right 8b alignment
      \arg        DAC_ALIGN_12B_R: data right 12b alignment
      \arg        DAC_ALIGN_12B_L: data left 12b alignment
    \param[in]  data0: data to be loaded
    \param[in]  data1: data to be loaded
    \param[out] none
    \retval     none
*/
void dac_concurrent_data_set(uint32_t dac_align, uint16_t data0, uint16_t data1)
{
    uint32_t data = 0U;
    switch(dac_align){
    /* data right 12b alignment */
    case DAC_ALIGN_12B_R:
        data = ((uint32_t)data1 << DH_12BIT_OFFSET) | data0;
        DACC_R12DH = data;
        break;
    /* data left 12b alignment */
    case DAC_ALIGN_12B_L:
        data = ((uint32_t)data1 << DH_12BIT_OFFSET) | data0;
        DACC_L12DH = data;
        break;
    /* data right 8b alignment */
    case DAC_ALIGN_8B_R:
        data = ((uint32_t)data1 << DH_8BIT_OFFSET) | data0;
        DACC_R8DH = data;
        break;
    default:
        break;
    }
}

/*!
    \brief      enable DAC concurrent interrupt funcution
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dac_concurrent_interrupt_enable(void)
{
    uint32_t ctl = 0U;
    ctl = DAC_CTL_DDUDRIE0 | DAC_CTL_DDUDRIE1;
    DAC_CTL |= (ctl);
}

/*!
    \brief      disable DAC concurrent interrupt funcution
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dac_concurrent_interrupt_disable(void)
{
    uint32_t ctl = 0U;
    ctl = DAC_CTL_DDUDRIE0 | DAC_CTL_DDUDRIE1;
    DAC_CTL &= (~ctl);
}

/*!
    \brief      enable DAC interrupt(DAC DMA underrun interrupt)
    \param[in]  dac_periph: DACx(x = 0,1)
    \param[out] none
    \retval     none
*/
void dac_interrupt_enable(uint32_t dac_periph)
{
    if(DAC0 == dac_periph){
        DAC_CTL |= DAC_CTL_DDUDRIE0;
    }else{
        DAC_CTL |= DAC_CTL_DDUDRIE1;
    }
}

/*!
    \brief      disable DAC interrupt(DAC DMA underrun interrupt)
    \param[in]  dac_periph: DACx(x = 0,1)
    \param[out] none
    \retval     none
*/
void dac_interrupt_disable(uint32_t dac_periph)
{
    if(DAC0 == dac_periph){
        DAC_CTL &= ~DAC_CTL_DDUDRIE0;
    }else{
        DAC_CTL &= ~DAC_CTL_DDUDRIE1;
    }
}

/*!
    \brief      get the specified DAC flag (DAC DMA underrun flag)
    \param[in]  dac_periph: DACx(x = 0,1)
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus dac_flag_get(uint32_t dac_periph)
{
    FlagStatus temp_flag = RESET;
    if(DAC0 == dac_periph){
        /* check the DMA underrun flag */
        if(RESET != (DAC_STAT & DAC_STAT_DDUDR0)){
            temp_flag = SET;
        }
    }else{
        /* check the DMA underrun flag */
        if(RESET != (DAC_STAT & DAC_STAT_DDUDR1)){
            temp_flag = SET;
        }
    }
    return temp_flag;
}

/*!
    \brief      clear the specified DAC flag (DAC DMA underrun flag)
    \param[in]  dac_periph: DACx(x = 0,1)
    \param[out] none
    \retval     none
*/
void dac_flag_clear(uint32_t dac_periph)
{
    if(DAC0 == dac_periph){
        DAC_STAT |= DAC_STAT_DDUDR0;
    }else{
        DAC_STAT |= DAC_STAT_DDUDR1;
    }
}

/*!
    \brief      get the specified DAC interrupt flag (DAC DMA underrun interrupt flag)
    \param[in]  dac_periph: DACx(x = 0,1)
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus dac_interrupt_flag_get(uint32_t dac_periph)
{
    FlagStatus temp_flag = RESET;
    uint32_t ddudr_flag = 0U, ddudrie_flag = 0U;

    if(DAC0 == dac_periph){
        /* check the DMA underrun flag and DAC DMA underrun interrupt enable flag */
        ddudr_flag = DAC_STAT & DAC_STAT_DDUDR0;
        ddudrie_flag = DAC_CTL & DAC_CTL_DDUDRIE0;
        if((RESET != ddudr_flag) && (RESET != ddudrie_flag)){
            temp_flag = SET;
        }
    }else{
        /* check the DMA underrun flag and DAC DMA underrun interrupt enable flag */
        ddudr_flag = DAC_STAT & DAC_STAT_DDUDR1;
        ddudrie_flag = DAC_CTL & DAC_CTL_DDUDRIE1;
        if((RESET != ddudr_flag) && (RESET != ddudrie_flag)){
            temp_flag = SET;
        }
    }
    return temp_flag;
}

/*!
    \brief      clear the specified DAC interrupt flag (DAC DMA underrun interrupt flag)
    \param[in]  dac_periph: DACx(x = 0,1)
    \param[out] none
    \retval     none
*/
void dac_interrupt_flag_clear(uint32_t dac_periph)
{
    if(DAC0 == dac_periph){
        DAC_STAT |= DAC_STAT_DDUDR0;
    }else{
        DAC_STAT |= DAC_STAT_DDUDR1;
    }
}
