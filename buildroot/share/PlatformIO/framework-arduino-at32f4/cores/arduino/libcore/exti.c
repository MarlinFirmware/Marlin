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

#include "exti.h"
#include "gpio.h"

#define EXTI_GetPortSourceGPIOx(Pin) GPIO_GetPortNum(Pin)
#define EXTI_GetPinSourcex(Pin)      GPIO_GetPinNum(Pin)

static EXTI_CallbackFunction_t EXTI_Function[16] = {0};

/**
* @brief Get the interrupt channel for external interrupts
* @param Pin: Pin number
* @retval Channel number
*/
static IRQn_Type EXTI_GetIRQn(uint8_t Pin)
{
    IRQn_Type EXINTx_IRQn = EXINT0_IRQn;
    uint8_t Pinx = GPIO_GetPinNum(Pin);

    if(Pinx <= 4)
    {
        switch(Pinx)
        {
        case 0:
            EXINTx_IRQn = EXINT0_IRQn;
            break;
        case 1:
            EXINTx_IRQn = EXINT1_IRQn;
            break;
        case 2:
            EXINTx_IRQn = EXINT2_IRQn;
            break;
        case 3:
            EXINTx_IRQn = EXINT3_IRQn;
            break;
        case 4:
            EXINTx_IRQn = EXINT4_IRQn;
            break;
        }
    }
    else if(Pinx >= 5 && Pinx <= 9)
    {
        EXINTx_IRQn = EXINT9_5_IRQn;
    }
    else if(Pinx >= 10 && Pinx <= 15)
    {
        EXINTx_IRQn = EXINT15_10_IRQn;
    }

    return EXINTx_IRQn;
}

/**
* @brief External interrupt initialization
* @param Pin: Pin number
* @param Function: Callback function
* @param Trigger_Mode: Trigger mode
* @param PreemptionPriority: Preemption priority
* @param SubPriority: Subpriority
* @retval None
*/

void EXTIx_Init(
    uint8_t Pin,
    EXTI_CallbackFunction_t Function,
    exint_polarity_config_type line_polarity,
    uint8_t PreemptionPriority,
    uint8_t SubPriority
)
{
    exint_init_type exint_init_struct;
    uint8_t Pinx;
    uint32_t Portx;

    if(!IS_PIN(Pin))
        return;

    Pinx = GPIO_GetPinNum(Pin);
    Portx = GPIO_GetPortNum(Pin); // Get the port number (0=A, 1=B, 2=C etc.)

    if(Pinx > 15)
        return;

    EXTI_Function[Pinx] = Function;

    // 1. Open the SCFG/IOMUX line for the external interrupt controller (We trigger both macros)
    #ifdef CRM_SCFG_PERIPH_CLOCK
        crm_periph_clock_enable(CRM_SCFG_PERIPH_CLOCK, TRUE);
    #endif
    #ifdef CRM_IOMUX_PERIPH_CLOCK
        crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);
    #endif

    // 2. Activate the clock line of the port you are using (e.g., GPIOB)
    // In the AT32 library, Port0=GPIOA, Port1=GPIOB.
    if(Portx == 0)      crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
    else if(Portx == 1) crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
    else if(Portx == 2) crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, TRUE);
    else if(Portx == 3) crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, TRUE);
    // ----------------------------------------

    // Configure interrupt line
    gpio_exint_line_config(Portx, (gpio_pins_source_type)Pinx);
    
    exint_default_para_init(&exint_init_struct);
    exint_init_struct.line_select = 1 << Pinx;
    exint_init_struct.line_mode = EXINT_LINE_INTERRUPUT;
    exint_init_struct.line_polarity = line_polarity;
    exint_init_struct.line_enable = TRUE;
    exint_init(&exint_init_struct);

    nvic_irq_enable(EXTI_GetIRQn(Pin), PreemptionPriority, SubPriority);
}



/**
* @brief External interrupt initialization (Arduino)
* @param Pin: Pin number
* @param function: Callback function
* @param Trigger_Mode: Trigger mode
* @retval None
*/
void attachInterrupt(uint8_t Pin, EXTI_CallbackFunction_t Function, exint_polarity_config_type line_polarity)
{
    EXTIx_Init(
        Pin,
        Function,
        line_polarity,
        EXTI_PREEMPTIONPRIORITY_DEFAULT,
        EXTI_SUBPRIORITY_DEFAULT
    );
}

/**
* @brief Disables the given interrupt (Arduino)
* @param Pin: Pin number
* @retval None
*/
void detachInterrupt(uint8_t Pin)
{
    if(!IS_PIN(Pin))
        return;

    nvic_irq_disable(EXTI_GetIRQn(Pin));
}

#define EXTIx_IRQHANDLER(n) \
do{\
    if(exint_flag_get(EXINT_LINE_##n) != RESET)\
    {\
        if(EXTI_Function[n]) EXTI_Function[n]();\
        exint_flag_clear(EXINT_LINE_##n);\
    }\
}while(0)

/**
* @brief External interrupt entry point, channel 0
* @param None
* @retval None
*/
void EXINT0_IRQHandler(void)
{
    EXTIx_IRQHANDLER(0);
}

/**
* @brief External interrupt entry point, channel 1
* @param None
* @retval None
*/
void EXINT1_IRQHandler(void) 
{
    EXTIx_IRQHANDLER(1);
}

/**
* @brief External interrupt entry point, channel 2
* @param None
* @retval None
*/
void EXINT2_IRQHandler(void) 
{
    EXTIx_IRQHANDLER(2);
}

/**
* @brief External interrupt entry point, channel 3
* @param None
* @retval None
*/
void EXINT3_IRQHandler(void) 
{
    EXTIx_IRQHANDLER(3);
}

/**
* @brief External interrupt entry point, channel 4
* @param None
* @retval None
*/
void EXINT4_IRQHandler(void) 
{
    EXTIx_IRQHANDLER(4);
}

/**
* @brief External interrupt entry point, channels 9~5
* @param None
* @retval None
*/
void EXINT9_5_IRQHandler(void) 
{
    EXTIx_IRQHANDLER(5);
    EXTIx_IRQHANDLER(6);
    EXTIx_IRQHANDLER(7);
    EXTIx_IRQHANDLER(8);
    EXTIx_IRQHANDLER(9);
}

/**
* @brief External interrupt entry point, channels 15~10
* @param None
* @retval None
*/
void EXINT15_10_IRQHandler(void) 
{
    EXTIx_IRQHANDLER(10);
    EXTIx_IRQHANDLER(11);
    EXTIx_IRQHANDLER(12);
    EXTIx_IRQHANDLER(13);
    EXTIx_IRQHANDLER(14);
    EXTIx_IRQHANDLER(15);
}

