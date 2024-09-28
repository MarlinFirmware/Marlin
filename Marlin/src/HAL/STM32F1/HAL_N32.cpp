/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

/**
 * HAL for stm32duino.com based on Libmaple and compatible (STM32F1)
 * Specifically for VOXELAB_N32. TODO: Rework for generic N32 MCU.
 */

#if defined(__STM32F1__) && defined(VOXELAB_N32)

#include "../../inc/MarlinConfig.h"
#include "HAL_N32.h"
#include "HAL.h"

#include "adc.h"

void ADC_Init(ADC_Module* NS_ADCx, ADC_InitType* ADC_InitStruct) {
  uint32_t tmpreg1 = 0;
  uint8_t tmpreg2  = 0;

  /*---------------------------- ADCx CTRL1 Configuration -----------------*/
  /* Get the ADCx CTRL1 value */
  tmpreg1 = NS_ADCx->CTRL1;
  /* Clear DUALMOD and SCAN bits */
  tmpreg1 &= CTRL1_CLR_MASK;
  /* Configure ADCx: Dual mode and scan conversion mode */
  /* Set DUALMOD bits according to WorkMode value */
  /* Set SCAN bit according to MultiChEn value */
  tmpreg1 |= (uint32_t)(ADC_InitStruct->WorkMode | ((uint32_t)ADC_InitStruct->MultiChEn << 8));
  /* Write to ADCx CTRL1 */
  NS_ADCx->CTRL1 = tmpreg1;

  /*---------------------------- ADCx CTRL2 Configuration -----------------*/
  /* Get the ADCx CTRL2 value */
  tmpreg1 = NS_ADCx->CTRL2;
  /* Clear CONT, ALIGN and EXTSEL bits */
  tmpreg1 &= CTRL2_CLR_MASK;
  /* Configure ADCx: external trigger event and continuous conversion mode */
  /* Set ALIGN bit according to DatAlign value */
  /* Set EXTSEL bits according to ExtTrigSelect value */
  /* Set CONT bit according to ContinueConvEn value */
  tmpreg1 |= (uint32_t)(ADC_InitStruct->DatAlign | ADC_InitStruct->ExtTrigSelect
                        | ((uint32_t)ADC_InitStruct->ContinueConvEn << 1));
  /* Write to ADCx CTRL2 */
  NS_ADCx->CTRL2 = tmpreg1;

  /*---------------------------- ADCx RSEQ1 Configuration -----------------*/
  /* Get the ADCx RSEQ1 value */
  tmpreg1 = NS_ADCx->RSEQ1;
  /* Clear L bits */
  tmpreg1 &= RSEQ1_CLR_MASK;
  /* Configure ADCx: regular channel sequence length */
  /* Set L bits according to ChsNumber value */
  tmpreg2 |= (uint8_t)(ADC_InitStruct->ChsNumber - (uint8_t)1);
  tmpreg1 |= (uint32_t)tmpreg2 << 20;
  /* Write to ADCx RSEQ1 */
  NS_ADCx->RSEQ1 = tmpreg1;
}

/**================================================================
 *       ADC reset
 ================================================================*/
void ADC_DeInit(ADC_Module* NS_ADCx) {
  uint32_t reg_temp;

  if (NS_ADCx == NS_ADC1) {
    /* Enable ADC1 reset state */
    reg_temp        = ADC_RCC_AHBPRST;
    reg_temp       |= RCC_AHB_PERIPH_ADC1;
    ADC_RCC_AHBPRST = reg_temp;             // ADC module reunion position
    ADC_RCC_AHBPRST = 0x00000000;           // ADC module reset and clear
  }
  else if (NS_ADCx == NS_ADC2) {
    /* Enable ADC2 reset state */
    reg_temp        = ADC_RCC_AHBPRST;
    reg_temp       |= RCC_AHB_PERIPH_ADC2;
    ADC_RCC_AHBPRST = reg_temp;             // ADC module reunion position
    ADC_RCC_AHBPRST = 0x00000000;           // ADC module reset and clear
  }
  else if (NS_ADCx == NS_ADC3) {
    /* Enable ADC2 reset state */
    reg_temp        = ADC_RCC_AHBPRST;
    reg_temp       |= RCC_AHB_PERIPH_ADC3;
    ADC_RCC_AHBPRST = reg_temp;             // ADC module reunion position
    ADC_RCC_AHBPRST = 0x00000000;           // ADC module reset and clear
  }
  else if (NS_ADCx == NS_ADC4) {
    /* Enable ADC3 reset state */
    reg_temp        = ADC_RCC_AHBPRST;
    reg_temp       |= RCC_AHB_PERIPH_ADC4;
    ADC_RCC_AHBPRST = reg_temp;             // ADC module reunion position
    ADC_RCC_AHBPRST = 0x00000000;           // ADC module reset and clear
  }
}

/**================================================================
 *       ADC module enable
 ================================================================*/
void ADC_Enable(ADC_Module* NS_ADCx, uint32_t Cmd) {
  if (Cmd)
    /* Set the AD_ON bit to wake up the ADC from power down mode */
    NS_ADCx->CTRL2 |= CTRL2_AD_ON_SET;
  else
    /* Disable the selected ADC peripheral */
    NS_ADCx->CTRL2 &= CTRL2_AD_ON_RESET;
}

/**================================================================
 *       Get the ADC status logo bit
 ================================================================*/
uint32_t ADC_GetFlagStatusNew(ADC_Module* NS_ADCx, uint8_t ADC_FLAG_NEW) {
  uint32_t bitstatus = 0;

  /* Check the status of the specified ADC flag */
  if ((NS_ADCx->CTRL3 & ADC_FLAG_NEW) != (uint8_t)0)
    /* ADC_FLAG_NEW is set */
    bitstatus = 1;
  else
    /* ADC_FLAG_NEW is reset */
    bitstatus = 0;
  /* Return the ADC_FLAG_NEW status */
  return bitstatus;
}

/**================================================================
 *       Open ADC calibration
 ================================================================*/
void ADC_StartCalibration(ADC_Module* NS_ADCx) {
  /* Enable the selected ADC calibration process */
  if (NS_ADCx->CALFACT == 0)
    NS_ADCx->CTRL2 |= CTRL2_CAL_SET;
}

/**================================================================
 *       Enable ADC DMA
 ================================================================*/
void ADC_EnableDMA(ADC_Module* NS_ADCx, uint32_t Cmd) {
  if (Cmd != 0)
    /* Enable the selected ADC DMA request */
    NS_ADCx->CTRL2 |= CTRL2_DMA_SET;
  else
    /* Disable the selected ADC DMA request */
    NS_ADCx->CTRL2 &= CTRL2_DMA_RESET;
}

/**================================================================
 *        Configure ADC interrupt enable enable
 ================================================================*/
void ADC_ConfigInt(ADC_Module* NS_ADCx, uint16_t ADC_IT, uint32_t Cmd) {
  uint8_t itmask = 0;

  /* Get the ADC IT index */
  itmask = (uint8_t)ADC_IT;
  if (Cmd != 0)
    /* Enable the selected ADC interrupts */
    NS_ADCx->CTRL1 |= itmask;
  else
    /* Disable the selected ADC interrupts */
    NS_ADCx->CTRL1 &= (~(uint32_t)itmask);
}

/**================================================================
 *        Get ADC calibration status
 ================================================================*/
uint32_t ADC_GetCalibrationStatus(ADC_Module* NS_ADCx) {
  uint32_t bitstatus = 0;

  /* Check the status of CAL bit */
  if ((NS_ADCx->CTRL2 & CTRL2_CAL_SET) != (uint32_t)0)
    /* CAL bit is set: calibration on going */
    bitstatus = 1;
  else
    /* CAL bit is reset: end of calibration */
    bitstatus = 0;

  if (NS_ADCx->CALFACT != 0)
    bitstatus = 0;
  /* Return the CAL bit status */
  return bitstatus;
}

/**================================================================
 *        Configure the ADC channel
 ================================================================*/
void ADC_ConfigRegularChannel(ADC_Module* NS_ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime) {
  uint32_t tmpreg1 = 0, tmpreg2 = 0;

  if (ADC_Channel == ADC_CH_18) {
    tmpreg1         = NS_ADCx->SAMPT3;
    tmpreg1        &= (~0x00000007);
    tmpreg1        |= ADC_SampleTime;
    NS_ADCx->SAMPT3 = tmpreg1;
  }
  else if (ADC_Channel > ADC_CH_9) { /* if ADC_CH_10 ... ADC_CH_17 is selected */
    /* Get the old register value */
    tmpreg1 = NS_ADCx->SAMPT1;
    /* Calculate the mask to clear */
    tmpreg2 = SAMPT1_SMP_SET << (3 * (ADC_Channel - 10));
    /* Clear the old channel sample time */
    tmpreg1 &= ~tmpreg2;
    /* Calculate the mask to set */
    tmpreg2 = (uint32_t)ADC_SampleTime << (3 * (ADC_Channel - 10));
    /* Set the new channel sample time */
    tmpreg1 |= tmpreg2;
    /* Store the new register value */
    NS_ADCx->SAMPT1 = tmpreg1;
  }
  else { /* ADC_Channel include in ADC_Channel_[0..9] */
    /* Get the old register value */
    tmpreg1 = NS_ADCx->SAMPT2;
    /* Calculate the mask to clear */
    tmpreg2 = SAMPT2_SMP_SET << (3 * ADC_Channel);
    /* Clear the old channel sample time */
    tmpreg1 &= ~tmpreg2;
    /* Calculate the mask to set */
    tmpreg2 = (uint32_t)ADC_SampleTime << (3 * ADC_Channel);
    /* Set the new channel sample time */
    tmpreg1 |= tmpreg2;
    /* Store the new register value */
    NS_ADCx->SAMPT2 = tmpreg1;
  }
  /* For Rank 1 to 6 */
  if (Rank < 7) {
    /* Get the old register value */
    tmpreg1 = NS_ADCx->RSEQ3;
    /* Calculate the mask to clear */
    tmpreg2 = SQR3_SEQ_SET << (5 * (Rank - 1));
    /* Clear the old SQx bits for the selected rank */
    tmpreg1 &= ~tmpreg2;
    /* Calculate the mask to set */
    tmpreg2 = (uint32_t)ADC_Channel << (5 * (Rank - 1));
    /* Set the SQx bits for the selected rank */
    tmpreg1 |= tmpreg2;
    /* Store the new register value */
    NS_ADCx->RSEQ3 = tmpreg1;
  }
  /* For Rank 7 to 12 */
  else if (Rank < 13) {
    /* Get the old register value */
    tmpreg1 = NS_ADCx->RSEQ2;
    /* Calculate the mask to clear */
    tmpreg2 = SQR2_SEQ_SET << (5 * (Rank - 7));
    /* Clear the old SQx bits for the selected rank */
    tmpreg1 &= ~tmpreg2;
    /* Calculate the mask to set */
    tmpreg2 = (uint32_t)ADC_Channel << (5 * (Rank - 7));
    /* Set the SQx bits for the selected rank */
    tmpreg1 |= tmpreg2;
    /* Store the new register value */
    NS_ADCx->RSEQ2 = tmpreg1;
  }
  /* For Rank 13 to 16 */
  else {
    /* Get the old register value */
    tmpreg1 = NS_ADCx->RSEQ1;
    /* Calculate the mask to clear */
    tmpreg2 = SQR1_SEQ_SET << (5 * (Rank - 13));
    /* Clear the old SQx bits for the selected rank */
    tmpreg1 &= ~tmpreg2;
    /* Calculate the mask to set */
    tmpreg2 = (uint32_t)ADC_Channel << (5 * (Rank - 13));
    /* Set the SQx bits for the selected rank */
    tmpreg1 |= tmpreg2;
    /* Store the new register value */
    NS_ADCx->RSEQ1 = tmpreg1;
  }
}

/**================================================================
 *        Start ADC conversion
 ================================================================*/
void ADC_EnableSoftwareStartConv(ADC_Module* NS_ADCx, uint32_t Cmd) {
  if (Cmd != 0)
    /* Enable the selected ADC conversion on external event and start the selected
       ADC conversion */
    NS_ADCx->CTRL2 |= CTRL2_EXT_TRIG_SWSTART_SET;
  else
    /* Disable the selected ADC conversion on external event and stop the selected
       ADC conversion */
    NS_ADCx->CTRL2 &= CTRL2_EXT_TRIG_SWSTART_RESET;
}

/**================================================================
 *        Get the ADC status logo bit
 ================================================================*/
uint32_t ADC_GetFlagStatus(ADC_Module* NS_ADCx, uint8_t ADC_FLAG) {
  uint32_t bitstatus = 0;

  /* Check the status of the specified ADC flag */
  if ((NS_ADCx->STS & ADC_FLAG) != (uint8_t)0)
    /* ADC_FLAG is set */
    bitstatus = 1;
  else
    /* ADC_FLAG is reset */
    bitstatus = 0;
  /* Return the ADC_FLAG status */
  return bitstatus;
}

/**================================================================
 *        Clear status logo bit
 ================================================================*/
void ADC_ClearFlag(ADC_Module* NS_ADCx, uint8_t ADC_FLAG) {
  /* Clear the selected ADC flags */
  NS_ADCx->STS &= ~(uint32_t)ADC_FLAG;
}

/**================================================================
 *        Get ADC sampling value
 ================================================================*/
uint16_t ADC_GetDat(ADC_Module* NS_ADCx) {
  /* Return the selected ADC conversion value */
  return (uint16_t)NS_ADCx->DAT;
}

/**================================================================
 *        Initialize ADC clock
 ================================================================*/

void enable_adc_clk(uint8_t cmd) {
  uint32_t reg_temp;
  if (cmd) {
    /** Make PWR clock */
    reg_temp           = ADC_RCC_APB1PCLKEN;
    reg_temp          |= RCC_APB1Periph_PWR;
    ADC_RCC_APB1PCLKEN = reg_temp;

    /** Enable expansion mode */
    reg_temp   = NS_PWR_CR3;
    reg_temp  |= 0x00000001;
    NS_PWR_CR3 = reg_temp;

    /** Make ADC clock */
    reg_temp  = ADC_RCC_AHBPCLKEN;
    reg_temp |= ( RCC_AHB_PERIPH_ADC1 |
                  RCC_AHB_PERIPH_ADC2 |
                  RCC_AHB_PERIPH_ADC3 |
                  RCC_AHB_PERIPH_ADC4   );
    ADC_RCC_AHBPCLKEN = reg_temp;

    /** Reset */
    reg_temp  = ADC_RCC_AHBPRST;
    reg_temp |= ( RCC_AHB_PERIPH_ADC1 |
                  RCC_AHB_PERIPH_ADC2 |
                  RCC_AHB_PERIPH_ADC3 |
                  RCC_AHB_PERIPH_ADC4   );
    ADC_RCC_AHBPRST  = reg_temp;              // ADC module reunion position
    ADC_RCC_AHBPRST &= ~reg_temp;             // ADC module reset and clear

    /** Set ADC 1M clock */
    reg_temp  = ADC_RCC_CFG2;
    reg_temp &= CFG2_ADC1MSEL_RESET_MASK;     // HSI as an ADC 1M clock
    reg_temp &= CFG2_ADC1MPRES_RESET_MASK;
    reg_temp |= 7 << 11;                      // Adc1m 8m / 8 = 1m

    /** Set the ADC PLL frequency split coefficient */
    reg_temp &= CFG2_ADCPLLPRES_RESET_MASK;
    reg_temp |= RCC_ADCPLLCLK_DIV4;           // ADC PLL frequency split coefficient

    /** Set the ADC HCLK frequency frequency coefficient */
    reg_temp    &= CFG2_ADCHPRES_RESET_MASK;
    reg_temp    |= RCC_ADCHCLK_DIV4;          // ADC HCLK frequency split coefficient
    ADC_RCC_CFG2 = reg_temp;                  // Write to register
  }
  else {
    /** Turn off the ADC clock */
    reg_temp  = ADC_RCC_AHBPCLKEN;
    reg_temp &= ~(  RCC_AHB_PERIPH_ADC1 |
                    RCC_AHB_PERIPH_ADC2 |
                    RCC_AHB_PERIPH_ADC3 |
                    RCC_AHB_PERIPH_ADC4   );
    ADC_RCC_AHBPCLKEN = reg_temp;
  }

}

/**================================================================
 *        Initialize ADC peripheral parameters
 ================================================================*/
void ADC_Initial(ADC_Module* NS_ADCx) {
  ADC_InitType ADC_InitStructure;

  /* ADC configuration ------------------------------------------------------*/
  ADC_InitStructure.WorkMode       = ADC_WORKMODE_INDEPENDENT;    // Independent mode
  ADC_InitStructure.MultiChEn      = 1;                           // Multi-channel enable
  ADC_InitStructure.ContinueConvEn = 1;                           // Continuous enable
  ADC_InitStructure.ExtTrigSelect  = ADC_EXT_TRIGCONV_NONE;       // Non-trigger
  ADC_InitStructure.DatAlign       = ADC_DAT_ALIGN_R;             // Right alignment
  ADC_InitStructure.ChsNumber      = 2;                           // Scan channel number
  ADC_Init(NS_ADCx, &ADC_InitStructure);

  /* ADC regular channel14 configuration */
  ADC_ConfigRegularChannel(NS_ADCx, ADC2_Channel_05_PC4, 2, ADC_SAMP_TIME_55CYCLES5);
  ADC_ConfigRegularChannel(NS_ADCx, ADC2_Channel_12_PC5, 1, ADC_SAMP_TIME_55CYCLES5);

  /** 使能ADC DMA */
  ADC_EnableDMA(NS_ADCx, 1);

  /* Enable ADC */
  ADC_Enable(NS_ADCx, 1);
  while(ADC_GetFlagStatusNew(NS_ADCx, ADC_FLAG_RDY) == 0);

  /* Start ADC calibration */
  ADC_StartCalibration(NS_ADCx);
  while (ADC_GetCalibrationStatus(NS_ADCx));

  /* Start ADC Software Conversion */
  ADC_EnableSoftwareStartConv(NS_ADCx, 1);
}

/**================================================================
 *        Single independent sampling
 ================================================================*/
uint16_t ADC_GetData(ADC_Module* NS_ADCx, uint8_t ADC_Channel) {
  uint16_t dat;

  /** Set channel parameters */
  ADC_ConfigRegularChannel(NS_ADCx, ADC_Channel, 1, ADC_SAMP_TIME_239CYCLES5);

  /* Start ADC Software Conversion */
  ADC_EnableSoftwareStartConv(NS_ADCx, 1);
  while(ADC_GetFlagStatus(NS_ADCx, ADC_FLAG_ENDC) == 0);

  ADC_ClearFlag(NS_ADCx, ADC_FLAG_ENDC);
  ADC_ClearFlag(NS_ADCx, ADC_FLAG_STR);
  dat = ADC_GetDat(NS_ADCx);
  return dat;
}

void DMA_DeInit(DMA_ChannelType* DMAyChx) {

  /* Disable the selected DMAy Channelx */
  DMAyChx->CHCFG &= (uint16_t)(~DMA_CHCFG1_CHEN);

  /* Reset DMAy Channelx control register */
  DMAyChx->CHCFG = 0;

  /* Reset DMAy Channelx remaining bytes register */
  DMAyChx->TXNUM = 0;

  /* Reset DMAy Channelx peripheral address register */
  DMAyChx->PADDR = 0;

  /* Reset DMAy Channelx memory address register */
  DMAyChx->MADDR = 0;

  if (DMAyChx == DMA1_CH1) {
    /* Reset interrupt pending bits for DMA1 Channel1 */
    DMA1->INTCLR |= DMA1_CH1_INT_MASK;
  }
  else if (DMAyChx == DMA1_CH2) {
    /* Reset interrupt pending bits for DMA1 Channel2 */
    DMA1->INTCLR |= DMA1_CH2_INT_MASK;
  }
  else if (DMAyChx == DMA1_CH3) {
    /* Reset interrupt pending bits for DMA1 Channel3 */
    DMA1->INTCLR |= DMA1_CH3_INT_MASK;
  }
  else if (DMAyChx == DMA1_CH4) {
    /* Reset interrupt pending bits for DMA1 Channel4 */
    DMA1->INTCLR |= DMA1_CH4_INT_MASK;
  }
  else if (DMAyChx == DMA1_CH5) {
    /* Reset interrupt pending bits for DMA1 Channel5 */
    DMA1->INTCLR |= DMA1_CH5_INT_MASK;
  }
  else if (DMAyChx == DMA1_CH6) {
    /* Reset interrupt pending bits for DMA1 Channel6 */
    DMA1->INTCLR |= DMA1_CH6_INT_MASK;
  }
  else if (DMAyChx == DMA1_CH7) {
    /* Reset interrupt pending bits for DMA1 Channel7 */
    DMA1->INTCLR |= DMA1_CH7_INT_MASK;
  }
  else if (DMAyChx == DMA1_CH8) {
    /* Reset interrupt pending bits for DMA1 Channel8 */
    DMA1->INTCLR |= DMA1_CH8_INT_MASK;
  }
  else if (DMAyChx == DMA2_CH1) {
    /* Reset interrupt pending bits for DMA2 Channel1 */
    DMA2->INTCLR |= DMA2_CH1_INT_MASK;
  }
  else if (DMAyChx == DMA2_CH2) {
    /* Reset interrupt pending bits for DMA2 Channel2 */
    DMA2->INTCLR |= DMA2_CH2_INT_MASK;
  }
  else if (DMAyChx == DMA2_CH3) {
    /* Reset interrupt pending bits for DMA2 Channel3 */
    DMA2->INTCLR |= DMA2_CH3_INT_MASK;
  }
  else if (DMAyChx == DMA2_CH4) {
    /* Reset interrupt pending bits for DMA2 Channel4 */
    DMA2->INTCLR |= DMA2_CH4_INT_MASK;
  }
  else if (DMAyChx == DMA2_CH5) {
    /* Reset interrupt pending bits for DMA2 Channel5 */
    DMA2->INTCLR |= DMA2_CH5_INT_MASK;
  }
  else if (DMAyChx == DMA2_CH6) {
    /* Reset interrupt pending bits for DMA2 Channel6 */
    DMA2->INTCLR |= DMA2_CH6_INT_MASK;
  }
  else if (DMAyChx == DMA2_CH7) {
    /* Reset interrupt pending bits for DMA2 Channel7 */
    DMA2->INTCLR |= DMA2_CH7_INT_MASK;
  }
  else if (DMAyChx == DMA2_CH8)
    /* Reset interrupt pending bits for DMA2 Channel8 */
    DMA2->INTCLR |= DMA2_CH8_INT_MASK;
}

void DMA_Init(DMA_ChannelType* DMAyChx, DMA_InitType* DMA_InitParam) {
  uint32_t tmpregister = 0;

  /*--------------------------- DMAy Channelx CHCFG Configuration --------------*/
  /* Get the DMAyChx CHCFG value */
  tmpregister = DMAyChx->CHCFG;
  /* Clear MEM2MEM, PL, MSIZE, PSIZE, MINC, PINC, CIRC and DIR bits */
  tmpregister &= CCR_CLEAR_Mask;
  /* Configure DMAy Channelx: data transfer, data size, priority level and mode */
  /* Set DIR bit according to Direction value */
  /* Set CIRC bit according to CircularMode value */
  /* Set PINC bit according to PeriphInc value */
  /* Set MINC bit according to DMA_MemoryInc value */
  /* Set PSIZE bits according to PeriphDataSize value */
  /* Set MSIZE bits according to MemDataSize value */
  /* Set PL bits according to Priority value */
  /* Set the MEM2MEM bit according to Mem2Mem value */
  tmpregister |= DMA_InitParam->Direction | DMA_InitParam->CircularMode | DMA_InitParam->PeriphInc
               | DMA_InitParam->DMA_MemoryInc | DMA_InitParam->PeriphDataSize | DMA_InitParam->MemDataSize
               | DMA_InitParam->Priority | DMA_InitParam->Mem2Mem;

  /* Write to DMAy Channelx CHCFG */
  DMAyChx->CHCFG = tmpregister;

  /*--------------------------- DMAy Channelx TXNUM Configuration --------------*/
  /* Write to DMAy Channelx TXNUM */
  DMAyChx->TXNUM = DMA_InitParam->BufSize;

  /*--------------------------- DMAy Channelx PADDR Configuration --------------*/
  /* Write to DMAy Channelx PADDR */
  DMAyChx->PADDR = DMA_InitParam->PeriphAddr;

  /*--------------------------- DMAy Channelx MADDR Configuration --------------*/
  /* Write to DMAy Channelx MADDR */
  DMAyChx->MADDR = DMA_InitParam->MemAddr;
}

void DMA_EnableChannel(DMA_ChannelType* DMAyChx, uint32_t Cmd) {
  if (Cmd != 0) {
    /* Enable the selected DMAy Channelx */
    DMAyChx->CHCFG |= DMA_CHCFG1_CHEN;
  }
  else {
    /* Disable the selected DMAy Channelx */
    DMAyChx->CHCFG &= (uint16_t)(~DMA_CHCFG1_CHEN);
  }
}

/**================================================================
 *        Initialize the DMA of ADC
 ================================================================*/
void ADC_DMA_init() {
  DMA_InitType DMA_InitStructure;
  uint32_t reg_temp;

  /** Make DMA clock */
  reg_temp = ADC_RCC_AHBPCLKEN;
  reg_temp |= ( RCC_AHB_PERIPH_DMA1 |
                RCC_AHB_PERIPH_DMA2    );
  ADC_RCC_AHBPCLKEN = reg_temp;

  /* DMA channel configuration*/
  DMA_DeInit(USE_DMA_CH);
  DMA_InitStructure.PeriphAddr     = (uint32_t)&USE_ADC->DAT;
  DMA_InitStructure.MemAddr        = (uint32_t)adc_results;
  DMA_InitStructure.Direction      = DMA_DIR_PERIPH_SRC;            // Peripheral-> memory
  DMA_InitStructure.BufSize        = 2;
  DMA_InitStructure.PeriphInc      = DMA_PERIPH_INC_DISABLE;
  DMA_InitStructure.DMA_MemoryInc  = DMA_MEM_INC_ENABLE;            // Memory ++
  DMA_InitStructure.PeriphDataSize = DMA_PERIPH_DATA_SIZE_HALFWORD;
  DMA_InitStructure.MemDataSize    = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.CircularMode   = DMA_MODE_CIRCULAR;
  DMA_InitStructure.Priority       = DMA_PRIORITY_HIGH;
  DMA_InitStructure.Mem2Mem        = DMA_M2M_DISABLE;
  DMA_Init(USE_DMA_CH, &DMA_InitStructure);

  /* Enable DMA channel1 */
  DMA_EnableChannel(USE_DMA_CH, 1);
}

/**=============================================================================
 *        n32g452 - end
 ==============================================================================*/

#define NS_PINRT(V...)       do{ SERIAL_ECHO_START(); SERIAL_ECHOLNPAIR(V); }while(0)

// Init the AD in continuous capture mode
void MarlinHAL::adc_init() {
  uint32_t reg_temp;

  //SERIAL_ECHO_MSG("\r\n n32g45x HAL_adc_init\r\n");

  // GPIO settings
  reg_temp = ADC_RCC_APB2PCLKEN;
  reg_temp |= 0x0f;                 // Make PORT mouth clock
  ADC_RCC_APB2PCLKEN = reg_temp;

  //reg_temp = NS_GPIOC_PL_CFG;
  //reg_temp &= 0XFF00FFFF;
  //NS_GPIOC_PL_CFG = reg_temp;     // PC4/5 analog input

  enable_adc_clk(1);                // Make ADC clock
  ADC_DMA_init();                   // DMA initialization
  ADC_Initial(NS_ADC2);             // ADC initialization

  delay(2);
  //NS_PINRT("get adc1 = ", adc_results[0], "\r\n");
  //NS_PINRT("get adc2 = ", adc_results[1], "\r\n");
}

#endif // __STM32F1__ && VOXELAB_N32
