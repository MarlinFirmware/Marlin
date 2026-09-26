/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Startup code for the Atmel AT91SAM3U4E.
 *
 * Marlin ships its own rather than using the one in PlatformIO's
 * framework-cmsis-atmel package: that file is written against a generic SAM3U
 * vector layout and does not compile against the package's own sam3u4e.h,
 * which has a different set of reserved slots (it refers to pvReserved7, 12
 * and 16, none of which exist in the SAM3U4E DeviceVectors struct).
 *
 * Responsibilities, in order:
 *   1. Provide the vector table, with the initial stack pointer from the
 *      linker script and weak aliases so any handler can be overridden simply
 *      by defining a function of the same name.
 *   2. Copy .data and .ramfunc from flash into SRAM, and zero .bss.
 *   3. Point VTOR at the vector table.
 *   4. Run the C++ static constructors, then call main().
 *
 * Note this deliberately does NOT call SystemInit(); the clock tree is brought
 * up at the top of main() in HAL/SAM3U/main.cpp.
 */

#ifdef __SAM3U4E__

#include <sam3u4e.h>

// Linker script symbols
extern uint32_t _sfixed;
extern uint32_t _etext;
extern uint32_t _srelocate;
extern uint32_t _erelocate;
extern uint32_t _szero;
extern uint32_t _ezero;
extern uint32_t _estack;

extern int main(void);
extern void __libc_init_array(void);

void Reset_Handler(void);

/**
 * Anything that has not been given a real implementation lands here. Spinning
 * (rather than returning) keeps an unexpected interrupt from being silently
 * ignored - it shows up as a hang at a recognizable address under a debugger.
 */
void Dummy_Handler(void) { for (;;) { } }

/* Cortex-M3 core handlers */
void NMI_Handler        (void) __attribute__ ((weak, alias("Dummy_Handler")));
void HardFault_Handler  (void) __attribute__ ((weak, alias("Dummy_Handler")));
void MemManage_Handler  (void) __attribute__ ((weak, alias("Dummy_Handler")));
void BusFault_Handler   (void) __attribute__ ((weak, alias("Dummy_Handler")));
void UsageFault_Handler (void) __attribute__ ((weak, alias("Dummy_Handler")));
void SVC_Handler        (void) __attribute__ ((weak, alias("Dummy_Handler")));
void DebugMon_Handler   (void) __attribute__ ((weak, alias("Dummy_Handler")));
void PendSV_Handler     (void) __attribute__ ((weak, alias("Dummy_Handler")));
void SysTick_Handler    (void) __attribute__ ((weak, alias("Dummy_Handler")));

/* Peripheral handlers */
void SUPC_Handler   (void) __attribute__ ((weak, alias("Dummy_Handler")));
void RSTC_Handler   (void) __attribute__ ((weak, alias("Dummy_Handler")));
void RTC_Handler    (void) __attribute__ ((weak, alias("Dummy_Handler")));
void RTT_Handler    (void) __attribute__ ((weak, alias("Dummy_Handler")));
void WDT_Handler    (void) __attribute__ ((weak, alias("Dummy_Handler")));
void PMC_Handler    (void) __attribute__ ((weak, alias("Dummy_Handler")));
void EFC0_Handler   (void) __attribute__ ((weak, alias("Dummy_Handler")));
void EFC1_Handler   (void) __attribute__ ((weak, alias("Dummy_Handler")));
void UART_Handler   (void) __attribute__ ((weak, alias("Dummy_Handler")));
void PIOA_Handler   (void) __attribute__ ((weak, alias("Dummy_Handler")));
void PIOB_Handler   (void) __attribute__ ((weak, alias("Dummy_Handler")));
void PIOC_Handler   (void) __attribute__ ((weak, alias("Dummy_Handler")));
void USART0_Handler (void) __attribute__ ((weak, alias("Dummy_Handler")));
void USART1_Handler (void) __attribute__ ((weak, alias("Dummy_Handler")));
void USART2_Handler (void) __attribute__ ((weak, alias("Dummy_Handler")));
void USART3_Handler (void) __attribute__ ((weak, alias("Dummy_Handler")));
void HSMCI_Handler  (void) __attribute__ ((weak, alias("Dummy_Handler")));
void TWI0_Handler   (void) __attribute__ ((weak, alias("Dummy_Handler")));
void TWI1_Handler   (void) __attribute__ ((weak, alias("Dummy_Handler")));
void SPI_Handler    (void) __attribute__ ((weak, alias("Dummy_Handler")));
void SSC_Handler    (void) __attribute__ ((weak, alias("Dummy_Handler")));
void TC0_Handler    (void) __attribute__ ((weak, alias("Dummy_Handler")));
void TC1_Handler    (void) __attribute__ ((weak, alias("Dummy_Handler")));
void TC2_Handler    (void) __attribute__ ((weak, alias("Dummy_Handler")));
void PWM_Handler    (void) __attribute__ ((weak, alias("Dummy_Handler")));
void ADC12B_Handler (void) __attribute__ ((weak, alias("Dummy_Handler")));
void ADC_Handler    (void) __attribute__ ((weak, alias("Dummy_Handler")));
void DMAC_Handler   (void) __attribute__ ((weak, alias("Dummy_Handler")));
void UDPHS_Handler  (void) __attribute__ ((weak, alias("Dummy_Handler")));

__attribute__ ((section(".vectors")))
const DeviceVectors exception_table = {

  /* Initial stack pointer, from the linker script */
  .pvStack = (void*) (&_estack),

  /* Cortex-M3 handlers */
  .pfnReset_Handler      = (void*) Reset_Handler,
  .pfnNMI_Handler        = (void*) NMI_Handler,
  .pfnHardFault_Handler  = (void*) HardFault_Handler,
  .pfnMemManage_Handler  = (void*) MemManage_Handler,
  .pfnBusFault_Handler   = (void*) BusFault_Handler,
  .pfnUsageFault_Handler = (void*) UsageFault_Handler,
  .pfnReserved1_Handler  = (void*) (0UL),
  .pfnReserved2_Handler  = (void*) (0UL),
  .pfnReserved3_Handler  = (void*) (0UL),
  .pfnReserved4_Handler  = (void*) (0UL),
  .pfnSVC_Handler        = (void*) SVC_Handler,
  .pfnDebugMon_Handler   = (void*) DebugMon_Handler,
  .pfnReserved5_Handler  = (void*) (0UL),
  .pfnPendSV_Handler     = (void*) PendSV_Handler,
  .pfnSysTick_Handler    = (void*) SysTick_Handler,

  /* Peripheral handlers, in peripheral ID order */
  .pfnSUPC_Handler   = (void*) SUPC_Handler,    /*  0 */
  .pfnRSTC_Handler   = (void*) RSTC_Handler,    /*  1 */
  .pfnRTC_Handler    = (void*) RTC_Handler,     /*  2 */
  .pfnRTT_Handler    = (void*) RTT_Handler,     /*  3 */
  .pfnWDT_Handler    = (void*) WDT_Handler,     /*  4 */
  .pfnPMC_Handler    = (void*) PMC_Handler,     /*  5 */
  .pfnEFC0_Handler   = (void*) EFC0_Handler,    /*  6 */
  .pfnEFC1_Handler   = (void*) EFC1_Handler,    /*  7 */
  .pfnUART_Handler   = (void*) UART_Handler,    /*  8 */
  .pvReserved9       = (void*) (0UL),           /*  9 */
  .pfnPIOA_Handler   = (void*) PIOA_Handler,    /* 10 */
  .pfnPIOB_Handler   = (void*) PIOB_Handler,    /* 11 */
  .pfnPIOC_Handler   = (void*) PIOC_Handler,    /* 12 */
  .pfnUSART0_Handler = (void*) USART0_Handler,  /* 13 */
  .pfnUSART1_Handler = (void*) USART1_Handler,  /* 14 */
  .pfnUSART2_Handler = (void*) USART2_Handler,  /* 15 */
  .pfnUSART3_Handler = (void*) USART3_Handler,  /* 16 */
  .pfnHSMCI_Handler  = (void*) HSMCI_Handler,   /* 17 */
  .pfnTWI0_Handler   = (void*) TWI0_Handler,    /* 18 */
  .pfnTWI1_Handler   = (void*) TWI1_Handler,    /* 19 */
  .pfnSPI_Handler    = (void*) SPI_Handler,     /* 20 */
  .pfnSSC_Handler    = (void*) SSC_Handler,     /* 21 */
  .pfnTC0_Handler    = (void*) TC0_Handler,     /* 22 - stepper */
  .pfnTC1_Handler    = (void*) TC1_Handler,     /* 23 - temperature */
  .pfnTC2_Handler    = (void*) TC2_Handler,     /* 24 - tone */
  .pfnPWM_Handler    = (void*) PWM_Handler,     /* 25 */
  .pfnADC12B_Handler = (void*) ADC12B_Handler,  /* 26 */
  .pfnADC_Handler    = (void*) ADC_Handler,     /* 27 */
  .pfnDMAC_Handler   = (void*) DMAC_Handler,    /* 28 */
  .pfnUDPHS_Handler  = (void*) UDPHS_Handler    /* 29 */
};

void Reset_Handler(void) {

  /* Copy .data and .ramfunc from their load address in flash into SRAM */
  uint32_t *pSrc = &_etext, *pDest = &_srelocate;
  if (pSrc != pDest)
    while (pDest < &_erelocate) *pDest++ = *pSrc++;

  /* Zero .bss */
  for (pDest = &_szero; pDest < &_ezero;) *pDest++ = 0;

  /* Point the vector table at our own copy */
  SCB->VTOR = ((uint32_t)&_sfixed & SCB_VTOR_TBLOFF_Msk);

  /* C++ static constructors and C library init */
  __libc_init_array();

  main();

  /* main() never returns, but if it somehow does, stop here */
  for (;;) { }
}

#endif // __SAM3U4E__
