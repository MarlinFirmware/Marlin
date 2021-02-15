#include "TMCStepper.h"
#include "TMC_MACROS.h"

#define SET_REG0(SETTING) DRVCTRL_0_register.SETTING = B; DRVCTRL(DRVCTRL_0_register.sr)
#define SET_REG1(SETTING) DRVCTRL_1_register.SETTING = B; DRVCTRL(DRVCTRL_1_register.sr)
#define GET_REG0(SETTING) return DRVCTRL_0_register.SETTING
#define GET_REG1(SETTING) return DRVCTRL_1_register.SETTING

uint32_t TMC2660Stepper::DRVCTRL() {
	if(sdoff() == 1) return DRVCTRL_1_register.sr;
	else return DRVCTRL_0_register.sr;
}
void TMC2660Stepper::DRVCTRL(uint32_t data) {
	if(sdoff() == 1) {
		DRVCTRL_1_register.sr = data;
		write(DRVCTRL_1_register.address, DRVCTRL_1_register.sr);
	} else {
		DRVCTRL_0_register.sr = data;
		write(DRVCTRL_0_register.address, DRVCTRL_0_register.sr);
	}
}

// DRVCTRL (SPI)
void TMC2660Stepper::pha(bool B)    { if(sdoff() == 0) return; SET_REG1(pha); }
void TMC2660Stepper::ca(uint8_t B)  { if(sdoff() == 0) return; SET_REG1(ca);  }
void TMC2660Stepper::phb(bool B)    { if(sdoff() == 0) return; SET_REG1(phb); }
void TMC2660Stepper::cb(uint8_t B)  { if(sdoff() == 0) return; SET_REG1(cb);  }

bool TMC2660Stepper::pha()    { if(sdoff() == 0) sdoff(1); GET_REG1(pha); }
uint8_t TMC2660Stepper::ca()  { if(sdoff() == 0) sdoff(1); GET_REG1(ca);  }
bool TMC2660Stepper::phb()    { if(sdoff() == 0) sdoff(1); GET_REG1(phb); }
uint8_t TMC2660Stepper::cb()  { if(sdoff() == 0) sdoff(1); GET_REG1(cb);  }

// DRVCTRL (STEP/DIR)
void TMC2660Stepper::intpol(bool B) { if(sdoff()) return; SET_REG0(intpol); }
void TMC2660Stepper::dedge(bool B)  { if(sdoff()) return; SET_REG0(dedge);  }
void TMC2660Stepper::mres(uint8_t B){ if(sdoff()) return; SET_REG0(mres);   }

bool TMC2660Stepper::intpol() { if(sdoff()) sdoff(0); GET_REG0(intpol); }
bool TMC2660Stepper::dedge()  { if(sdoff()) sdoff(0); GET_REG0(dedge);  }
uint8_t TMC2660Stepper::mres(){ if(sdoff()) sdoff(0); GET_REG0(mres);   }
