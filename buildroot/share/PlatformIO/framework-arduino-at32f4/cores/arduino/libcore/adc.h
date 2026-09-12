/*
 * MIT License
 * Copyright (c) 2017 - 2022 _VIFEXTech
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef __ADC_H
#define __ADC_H

#include "at32f403a_407_adc.h"
#include "mcu_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    ADC_DMA_RES_OK                                = 0,
    ADC_DMA_RES_NOT_ADC_CHANNEL                   = -1,
    ADC_DMA_RES_DUPLICATE_REGISTRATION            = -2,
    ADC_DMA_RES_MAX_NUM_OF_REGISTRATIONS_EXCEEDED = -3,
} ADC_DMA_Res_Type;

uint16_t ADC_DMA_GetValue_Stabilized(uint8_t ADC_Channel);
void             ADCx_Init(adc_type* ADCx);
uint16_t         ADCx_GetValue(adc_type* ADCx, uint16_t ADC_Channel);
void             ADC_DMA_Init(void);
ADC_DMA_Res_Type ADC_DMA_Register(uint8_t ADC_Channel);
uint16_t         ADC_DMA_GetValue(uint8_t ADC_Channel);
uint8_t          ADC_DMA_GetRegisterCount(void);

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H */
