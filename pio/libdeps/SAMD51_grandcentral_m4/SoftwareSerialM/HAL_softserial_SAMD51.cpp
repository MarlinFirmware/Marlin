#include "HAL_platform.h"

#if HAL_SS_PLATFORM == HAL_PLATFORM_SAMD51

#include "HAL_softserial_SAMD51.h"

void HAL_softSerial_init() {
  NVIC_SetPriority(SS_TIMERIRQ, INTERRUPT_PRIORITY);
}

void HAL_softserial_setSpeed(uint32_t speed) {
  Disable_Irq(SS_TIMERIRQ);
  if (speed != 0) {
    // Disable timer interrupt
    SS_TC_DEV->COUNT16.INTENCLR.reg = TC_INTENCLR_OVF;  // disable overflow interrupt

    // TCn clock setup
    const uint8_t clockID = GCLK_CLKCTRL_IDs[TCC_INST_NUM + SS_TIMER];  // TC clock are preceeded by TCC ones
    GCLK->PCHCTRL[clockID].bit.CHEN = false;
    while(GCLK->PCHCTRL[clockID].bit.CHEN) ;
    GCLK->PCHCTRL[clockID].reg = GCLK_PCHCTRL_GEN_GCLK0 | GCLK_PCHCTRL_CHEN;  // 120MHz startup code programmed
    while(!GCLK->PCHCTRL[clockID].bit.CHEN) ;

    // Stop timer, just in case, to be able to reconfigure it
    SS_TC_DEV->COUNT16.CTRLA.bit.ENABLE = false;
    while(SS_TC_DEV->COUNT16.SYNCBUSY.bit.ENABLE) ;

    // Reset timer
    SS_TC_DEV->COUNT16.CTRLA.bit.SWRST = true;
    while(SS_TC_DEV->COUNT16.SYNCBUSY.bit.SWRST) ;

    // Wave mode, reset counter on overflow on 0 (I use count down to prevent double buffer use)
    SS_TC_DEV->COUNT16.WAVE.reg = TC_WAVE_WAVEGEN_MFRQ;
    SS_TC_DEV->COUNT16.CTRLA.reg = TC_CTRLA_MODE_COUNT16 | TC_CTRLA_PRESCALER_DIV1;
    SS_TC_DEV->COUNT16.CTRLBSET.reg = TC_CTRLBCLR_DIR;
    while(SS_TC_DEV->COUNT16.SYNCBUSY.bit.CTRLB) ;

    // Set compare value
    SS_TC_DEV->COUNT16.COUNT.reg = SS_TC_DEV->COUNT16.CC[0].reg = F_CPU / (speed * OVERSAMPLE);

    // Enable interrupt on compare
    SS_TC_DEV->COUNT16.INTFLAG.reg = TC_INTFLAG_OVF;    // reset pending interrupt
    SS_TC_DEV->COUNT16.INTENSET.reg = TC_INTENSET_OVF;  // enable overflow interrupt

    // And start timer
    SS_TC_DEV->COUNT16.CTRLA.bit.ENABLE = true;
    while(SS_TC_DEV->COUNT16.SYNCBUSY.bit.ENABLE) ;

    SS_TC_DEV->COUNT16.INTFLAG.reg = TC_INTFLAG_OVF;
    NVIC_EnableIRQ(SS_TIMERIRQ);
  }
}

#endif
