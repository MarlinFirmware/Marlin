#include "pins_arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

// Pin number
const PinName digitalPin[] = {
  PE_2,  // D0
  PE_3,  // D1
  PE_4,  // D2
  PE_5,  // D3
  PE_6,  // D4
  PI_8,  // D5
  PC_13, // D6
  PC_14, // D7
  PC_15, // D8
  PI_9,  // D9
  PI_10, // D10
  PI_11, // D11
  PF_0,  // D12
  PF_1,  // D13
  PF_2,  // D14
  PH_0,  // D15
  PH_1,  // D16
  PB_2,  // D17
  PF_15, // D18
  PG_0,  // D19
  PG_1,  // D20
  PE_7,  // D21
  PE_8,  // D22
  PE_9,  // D23
  PE_10, // D24
  PE_11, // D25
  PE_12, // D26
  PE_13, // D27
  PE_14, // D28
  PE_15, // D29
  PB_10, // D30
  PB_11, // D31
  PH_6,  // D32
  PH_7,  // D33
  PH_8,  // D34
  PH_9,  // D35
  PH_10, // D36
  PH_11, // D37
  PH_12, // D38
  PB_12, // D39
  PB_13, // D40
  PB_14, // D41
  PB_15, // D42
  PD_8,  // D43
  PD_9,  // D44
  PD_10, // D45
  PD_11, // D46
  PD_12, // D47
  PD_13, // D48
  PD_14, // D49
  PD_15, // D50
  PG_2,  // D51
  PG_3,  // D52
  PG_4,  // D53
  PG_5,  // D54
  PG_6,  // D55
  PG_7,  // D56
  PG_8,  // D57
  PC_6,  // D58
  PC_7,  // D59
  PC_8,  // D60
  PC_9,  // D61
  PA_8,  // D62
  PA_9,  // D63
  PA_10, // D64
  PA_11, // D65
  PA_12, // D66
  PA_13, // D67
  PH_13, // D68
  PH_14, // D69
  PH_15, // D70
  PI_0,  // D71
  PI_1,  // D72
  PI_2,  // D73
  PI_3,  // D74
  PA_14, // D75
  PA_15, // D76
  PC_10, // D77
  PC_11, // D78
  PC_12, // D79
  PD_0,  // D80
  PD_1,  // D81
  PD_2,  // D82
  PD_3,  // D83
  PD_4,  // D84
  PD_5,  // D85
  PD_6,  // D86
  PD_7,  // D87
  PG_9,  // D88
  PG_10, // D89
  PG_11, // D90
  PG_12, // D91
  PG_13, // D92
  PG_14, // D93
  PG_15, // D94
  PB_3,  // D95
  PB_4,  // D96
  PB_5,  // D97
  PB_6,  // D98
  PB_7,  // D99
  PB_8,  // D100
  PB_9,  // D101
  PE_0,  // D102
  PE_1,  // D103
  PI_4,  // D104
  PI_5,  // D105
  PI_6,  // D106
  PI_7,  // D107
  PA_0,  // D108 / A0
  PA_1,  // D109 / A1
  PA_2,  // D110 / A2
  PA_3,  // D111 / A3
  PA_4,  // D112 / A4
  PA_5,  // D113 / A5
  PA_6,  // D114 / A6
  PA_7,  // D115 / A7
  PB_0,  // D116 / A8
  PB_1,  // D117 / A9
  PH_2,  // D118 / A10
  PH_3,  // D119 / A11
  PH_4,  // D120 / A12
  PH_5,  // D121 / A13
  PC_0,  // D122 / A14
  PC_1,  // D123 / A15
  PC_2,  // D124 / A16
  PC_3,  // D125 / A17
  PC_4,  // D126 / A18
  PC_5,  // D127 / A19
  PF_3,  // D128 / A20
  PF_4,  // D129 / A21
  PF_5,  // D130 / A22
  PF_6,  // D131 / A23
  PF_7,  // D132 / A24
  PF_8,  // D133 / A25
  PF_9,  // D134 / A26
  PF_10, // D135 / A27
  PF_11, // D136 / A28
  PF_12, // D137 / A29
  PF_13, // D138 / A30
  PF_14, // D139 / A31
};

#ifdef __cplusplus
}
#endif

// ----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
}
#endif
