#include "HAL_platform.h"

#if HAL_SS_PLATFORM == HAL_PLATFORM_LPC1768

#include "HAL_softserial_LCP1768.h"

#define FORCE_INLINE __attribute__((always_inline)) inline

#define HAL_SOFTSERIAL_TIMER_ISR extern "C" void RIT_IRQHandler(void)

extern "C" void RIT_IRQHandler(void);

FORCE_INLINE static void HAL_softserial_timer_isr_prologue() {
  LPC_RIT->RICTRL |= 1;
}

#define HAL_softserial_timer_isr_epilogue()

void HAL_softSerial_init() {
  RIT_Init(LPC_RIT);
  NVIC_SetPriority(RIT_IRQn, NVIC_EncodePriority(0, INTERRUPT_PRIORITY, 0));
}

void HAL_softserial_setSpeed(uint32_t speed) {
  NVIC_DisableIRQ(RIT_IRQn);
  if (speed != 0) {
    uint32_t clock_rate, cmp_value;
    // Get PCLK value of RIT
    clock_rate = CLKPWR_GetPCLK(CLKPWR_PCLKSEL_RIT);
    cmp_value = clock_rate/(speed*OVERSAMPLE);
    LPC_RIT->RICOMPVAL = cmp_value;
    LPC_RIT->RICOUNTER	= 0x00000000;
    /* Set timer enable clear bit to clear timer to 0 whenever
    * counter value equals the contents of RICOMPVAL
    */
    LPC_RIT->RICTRL |= (1<<1);
    NVIC_EnableIRQ(RIT_IRQn);
  }
}

#endif
