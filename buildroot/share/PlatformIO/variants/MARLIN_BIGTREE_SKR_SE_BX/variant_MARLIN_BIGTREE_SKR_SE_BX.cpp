/*
 *******************************************************************************
 * Copyright (c) 2020-2021, STMicroelectronics
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 *******************************************************************************
 */
#include "pins_arduino.h"

// Digital PinName array
const PinName digitalPin[] = {
  PA_0,   // D1/A0
  PA_1,   // D2/A1
  PA_2,   // D3/A2
  PA_3,   // D4/A3
  PA_4,   // D5/A4
  PA_5,   // D6/A5
  PA_6,   // D7/A6
  PA_7,   // D8/A7
  PA_8,   // D9
  PA_9,   // D10
  PA_10,  // D11
  PA_11,  // D12
  PA_12,  // D13
  PA_13,  // D14
  PA_14,  // D15
  PA_15,  // D16
  PB_0,   // D17/A8
  PB_1,   // D18/A9
  PB_2,   // D19
  PB_3,   // D20
  PB_4,   // D21
  PB_5,   // D22
  PB_6,   // D23
  PB_7,   // D24
  PB_8,   // D25
  PB_9,   // D26
  PB_10,  // D27
  PB_11,  // D28
  PB_12,  // D29
  PB_13,  // D30
  PB_14,  // D31
  PB_15,  // D32
  PC_0,   // D33/A10
  PC_1,   // D34/A11
  PC_4,   // D35/A12
  PC_5,   // D36/A13
  PC_6,   // D37
  PC_7,   // D38
  PC_8,   // D39
  PC_9,   // D40
  PC_10,  // D41
  PC_11,  // D42
  PC_12,  // D43
  PC_13,  // D44
  PC_14,  // D45
  PC_15,  // D46
  PD_0,   // D47
  PD_1,   // D48
  PD_2,   // D49
  PD_3,   // D50
  PD_4,   // D51
  PD_5,   // D52
  PD_6,   // D53
  PD_7,   // D54
  PD_8,   // D55
  PD_9,   // D56
  PD_10,  // D57
  PD_11,  // D58
  PD_12,  // D59
  PD_13,  // D60
  PD_14,  // D61
  PD_15,  // D62
  PE_0,   // D63
  PE_1,   // D64
  PE_2,   // D65
  PE_3,   // D66
  PE_4,   // D67
  PE_5,   // D68
  PE_6,   // D69
  PE_7,   // D70
  PE_8,   // D71
  PE_9,   // D72
  PE_10,  // D73
  PE_11,  // D74
  PE_12,  // D75
  PE_13,  // D76
  PE_14,  // D77
  PE_15,  // D78
  PF_0,   // D79
  PF_1,   // D80
  PF_2,   // D81
  PF_3,   // D82/A14
  PF_4,   // D83/A15
  PF_5,   // D84/A16
  PF_6,   // D85/A17
  PF_7,   // D86/A18
  PF_8,   // D87/A19
  PF_9,   // D88/A20
  PF_10,  // D89/A21
  PF_11,  // D90/A22
  PF_12,  // D91/A23
  PF_13,  // D92/A24
  PF_14,  // D93/A25
  PF_15,  // D94
  PG_0,   // D95
  PG_1,   // D96
  PG_2,   // D97
  PG_3,   // D98
  PG_4,   // D99
  PG_5,   // D100
  PG_6,   // D101
  PG_7,   // D102
  PG_8,   // D103
  PG_9,   // D104
  PG_10,  // D105
  PG_11,  // D106
  PG_12,  // D107
  PG_13,  // D108
  PG_14,  // D109
  PG_15,  // D110
  PH_0,   // D111
  PH_1,   // D112
  PH_2,   // D113/A26
  PH_3,   // D114/A27
  PH_4,   // D115/A28
  PH_5,   // D116/A29
  PH_6,   // D117
  PH_7,   // D118
  PH_8,   // D119
  PH_9,   // D120
  PH_10,  // D121
  PH_11,  // D122
  PH_12,  // D123
  PH_13,  // D124
  PH_14,  // D125
  PH_15,  // D126
  PI_0,   // D127
  PI_1,   // D128
  PI_2,   // D129
  PI_3,   // D130
  PI_4,   // D131
  PI_5,   // D132
  PI_6,   // D133
  PI_7,   // D134
  PI_8,   // D135
  PI_9,   // D136
  PI_10,  // D137
  PI_11,  // D138
  PC_2_C, // D139/A30
  PC_3_C  // D140/A31
};

// Analog (Ax) pin number array
const uint32_t analogInputPin[] = {
  0,  // A0,  PA0
  1,  // A1,  PA1
  2,  // A2,  PA2
  3,  // A3,  PA3
  4,  // A4,  PA4
  5,  // A5,  PA5
  6,  // A6,  PA6
  7,  // A7,  PA7
  16, // A8,  PB0
  17, // A9,  PB1
  32, // A10, PC0
  33, // A11, PC1
  34, // A12, PC4
  35, // A13, PC5
  81, // A14, PF3
  82, // A15, PF4
  83, // A16, PF5
  84, // A17, PF6
  85, // A18, PF7
  86, // A19, PF8
  87, // A20, PF9
  88, // A21, PF10
  89, // A22, PF11
  90, // A23, PF12
  91, // A24, PF13
  92, // A25, PF14
  112, // A26, PH2
  113, // A27, PH3
  114, // A28, PH4
  115, // A29, PH5
  138, // A30, PC2_C
  139 // A31, PC3_C
};

void SystemClockStartupInit() {
  // Confirm is called only once time to avoid hang up caused by repeated calls in USB wakeup interrupt
  static bool first_call = true;
  if (!first_call) return;
  first_call = false;

  // Clear all clock setting register
  RCC->CR = 0x00000001;
  RCC->CFGR = 0x00000000;
  RCC->D1CFGR = 0x00000000;
  RCC->D2CFGR = 0x00000000;
  RCC->D3CFGR = 0x00000000;
  RCC->PLLCKSELR = 0x00000000;
  RCC->PLLCFGR = 0x00000000;
  RCC->CIER = 0x00000000;

  // AXI_TARG7_FN_MOD for SRAM
  *((volatile uint32_t*)0x51008108)=0x00000001;

  // Enable L1-Cache
  SCB_EnableICache();
  SCB_EnableDCache();
  SCB->CACR |= 1<<2;

  PWR->CR3 &= ~(1 << 2);  // SCUEN=0
  PWR->D3CR |= 3 << 14;   // VOS=3,Scale1,1.15~1.26V core voltage
  while((PWR->D3CR & (1 << 13)) == 0);	// Wait for the voltage to stabilize
  RCC->CR |= 1<<16;       // Enable HSE

  uint16_t timeout = 0;
  while(((RCC->CR & (1 << 17)) == 0) && (timeout < 0X7FFF)) {
    timeout++;            // Wait for HSE RDY
  }

    if(timeout == 0X7FFF) {
    Error_Handler();
  } else {
    RCC->PLLCKSELR |= 2 << 0;         // PLLSRC[1:0] = 2, HSE for PLL clock source
    RCC->PLLCKSELR |= 5 << 4;         // DIVM1[5:0] = pllm,     Prescaler for PLL1
    RCC->PLL1DIVR |= (160 - 1) << 0;  // DIVN1[8:0] = plln - 1, Multiplication factor for PLL1 VCO
    RCC->PLL1DIVR |= (2 - 1) << 9;	  // DIVP1[6:0] = pllp - 1, PLL1 DIVP division factor
    RCC->PLL1DIVR |= (4 - 1) << 16;   // DIVQ1[6:0] = pllq - 1, PLL1 DIVQ division factor
    RCC->PLL1DIVR |= 1 << 24;		      // DIVR1[6:0] = pllr - 1, PLL1 DIVR division factor
    RCC->PLLCFGR |= 2 << 2;           // PLL1 input (ref1_ck) clock range frequency is between 4 and 8 MHz
    RCC->PLLCFGR |= 0 << 1;           // PLL1 VCO selection, 0: 192 to 836 MHz, 1 : 150 to 420 MHz
    RCC->PLLCFGR |= 3 << 16;          // pll1_q_ck and pll1_p_ck output is enabled
    RCC->CR |= 1 << 24;               // PLL1 enable
    while((RCC->CR & (1 << 25)) == 0); // PLL1 clock ready flag

    // PLL2 DIVR clock frequency = 220MHz, so that SDRAM clock can be set to 110MHz
    RCC->PLLCKSELR |= 25 << 12;       // DIVM2[5:0] = 25, Prescaler for PLL2
    RCC->PLL2DIVR |= (440 - 1) << 0;	// DIVN2[8:0] = 440 - 1, Multiplication factor for PLL2 VCO
    RCC->PLL2DIVR |= (2 - 1) << 9;    // DIVP2[6:0] = 2-1, PLL2 DIVP division factor
    RCC->PLL2DIVR |= (2 - 1) << 24;   // DIVR2[6:0] = 2-1, PLL2 DIVR division factor
    RCC->PLLCFGR |= 0 << 6;           // PLL2RGE[1:0]=0, PLL2 input (ref2_ck) clock range frequency is between 1 and 2 MHz
    RCC->PLLCFGR |= 0 << 5;           // PLL2 VCO selection, 0: 192 to 836 MHz, 1: 150 to 420 MHz
    RCC->PLLCFGR |= 1 << 19;          // pll2_p_ck output is enabled
    RCC->PLLCFGR |= 1 << 21;          // pll2_r_ck output is enabled
    RCC->D1CCIPR &= ~(3 << 0);        // clear FMC kernel clock source selection
    RCC->D1CCIPR |= 2 << 0;           // pll2_r_ck clock selected as kernel peripheral clock
    RCC->CR |= 1 << 26;               // PLL2 enable
    while((RCC->CR&(1<<27)) == 0);    // PLL2 clock ready flag

    RCC->D1CFGR |= 8 << 0;  // rcc_hclk3 = sys_d1cpre_ck / 2 = 400 / 2 = 200MHz.  AHB1/2/3/4
    RCC->D1CFGR |= 0 << 8;  // sys_ck not divided, sys_d1cpre_ck = sys_clk / 1 = 400 / 1 = 400MHz, System Clock = 400MHz
    RCC->CFGR |= 3 << 0;    // PLL1 selected as system clock (pll1_p_ck). 400MHz
    while(1) {
      timeout = (RCC->CFGR & (7 << 3)) >> 3; // System clock switch status
      if(timeout == 3) break; // Wait for SW[2:0] = 3 (011: PLL1 selected as system clock (pll1_p_ck))
    }

    FLASH->ACR |= 2 << 0;   // LATENCY[2:0] = 2 (@VOS1 Level,maxclock=210MHz)
    FLASH->ACR |= 2 << 4;   // WRHIGHFREQ[1:0] = 2, flash access frequency < 285MHz

    RCC->D1CFGR |= 4 << 4;  // D1PPRE[2:0] = 4, rcc_pclk3 = rcc_hclk3 / 2 = 100MHz, APB3.
    RCC->D2CFGR |= 4 << 4;  // D2PPRE1[2:0] = 4, rcc_pclk1 = rcc_hclk1 / 2 = 100MHz, APB1.
    RCC->D2CFGR |= 4 << 8;  // D2PPRE2[2:0] = 4, rcc_pclk2 = rcc_hclk1 / 2 = 100MHz, APB2.
    RCC->D3CFGR |= 4 << 4;  // D3PPRE[2:0] = 4, rcc_pclk4 = rcc_hclk4 / 2 = 100MHz, APB4.

    RCC->CR |= 1 << 7;      // CSI clock enable
    RCC->APB4ENR |= 1 << 1; // SYSCFG peripheral clock enable
    SYSCFG->CCCSR |= 1 << 0;
  }

  // USB clock, (use HSI48 clock)
  RCC->CR |= 1 << 12;   // HSI48 clock enabl
  while((RCC->CR & (1 << 13)) == 0);// 1: HSI48 clock is ready
  RCC->APB1HENR |= 1 << 1;      // CRS peripheral clock enabled
  RCC->APB1HRSTR |= 1 << 1;     // Resets CRS
  RCC->APB1HRSTR &= ~(1 << 1);  // Does not reset CRS
  CRS->CFGR &= ~(3 << 28);      // USB2 SOF selected as SYNC signal source
  CRS->CR |= 3 << 5;            // Automatic trimming and Frequency error counter enabled
  RCC->D2CCIP2R &= ~(3 << 20);  // Clear USBOTG 1 and 2 kernel clock source selection
  RCC->D2CCIP2R |= 3 << 20;     // HSI48_ck clock is selected as kernel clock
}

uint8_t MPU_Convert_Bytes_To_POT(uint32_t nbytes)
{
  uint8_t count = 0;
  while(nbytes != 1)
  {
    nbytes >>= 1;
    count++;
  }
  return count;
}

uint8_t MPU_Set_Protection(uint32_t baseaddr, uint32_t size, uint32_t rnum, uint8_t ap, uint8_t sen, uint8_t cen, uint8_t ben)
{
  uint32_t tempreg = 0;
  uint8_t rnr = 0;
  if ((size % 32) || size == 0) return 1;
  rnr = MPU_Convert_Bytes_To_POT(size) - 1;
  SCB->SHCSR &= ~(1 << 16);	        //disable MemManage
  MPU->CTRL &= ~(1 << 0);		        //disable MPU
  MPU->RNR = rnum;
  MPU->RBAR = baseaddr;
  tempreg |= 0 << 28;
  tempreg |= ((uint32_t)ap) << 24;
  tempreg |= 0 << 19;
  tempreg |= ((uint32_t)sen) << 18;
  tempreg |= ((uint32_t)cen) << 17;
  tempreg |= ((uint32_t)ben) << 16;
  tempreg |= 0 << 8;
  tempreg |= rnr << 1;
  tempreg |= 1 << 0;
  MPU->RASR = tempreg;
  MPU->CTRL = (1 << 2) | (1 << 0);  //enable PRIVDEFENA
  SCB->SHCSR |= 1 << 16;		        //enable MemManage
  return 0;
}

void MPU_Memory_Protection(void)
{
	MPU_Set_Protection(0x20000000, 128 * 1024, 1, MPU_REGION_FULL_ACCESS, 0, 1, 1);       // protect DTCM 128k,  Sharing is prohibited, cache is allowed, and buffering is allowed

	MPU_Set_Protection(0x24000000, 512 * 1024, 2, MPU_REGION_FULL_ACCESS, 0, 1, 1);       // protect AXI SRAM,  Sharing is prohibited, cache is allowed, and buffering is allowed
	MPU_Set_Protection(0x30000000, 512 * 1024, 3, MPU_REGION_FULL_ACCESS, 0, 1, 1);       // protect SRAM1~SRAM3, Sharing is prohibited, cache is allowed, and buffering is allowed
	MPU_Set_Protection(0x38000000, 64 * 1024, 4, MPU_REGION_FULL_ACCESS, 0, 1, 1);        // protect SRAM4, Sharing is prohibited, cache is allowed, and buffering is allowed

	MPU_Set_Protection(0x60000000, 64 * 1024 * 1024, 5, MPU_REGION_FULL_ACCESS, 0, 0, 0);   // protect LCD FMC  64M, No sharing, no cache, no buffering
	MPU_Set_Protection(0XC0000000, 32 * 1024 * 1024, 6, MPU_REGION_FULL_ACCESS, 0, 1, 1);   // protect SDRAM  32M, Sharing is prohibited, cache is allowed, and buffering is allowed
	MPU_Set_Protection(0X80000000, 256 * 1024 * 1024, 7, MPU_REGION_FULL_ACCESS, 0, 0, 0);  // protect NAND FLASH 256M, No sharing, no cache, no buffering
}

/**
  * @brief  System Clock Configuration
  * @param  None
  * @retval None
  */
WEAK void SystemClock_Config(void)
{
  SystemClockStartupInit();

  MPU_Memory_Protection();

  /* Update current SystemCoreClock value */
  SystemCoreClockUpdate();

  /* Configure the Systick interrupt time */
  HAL_SYSTICK_Config(SystemCoreClock/1000);

  /* Configure the Systick */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}
