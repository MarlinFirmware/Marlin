#include "TMCStepper.h"
#include "TMC_MACROS.h"

#define SET_REG(SETTING) IHOLD_IRUN_register.SETTING = B; write(IHOLD_IRUN_register.address, IHOLD_IRUN_register.sr);
#define GET_REG(SETTING) return IHOLD_IRUN_register.SETTING;

// IHOLD_IRUN
uint32_t TMCStepper::IHOLD_IRUN() { return IHOLD_IRUN_register.sr; }
void TMCStepper::IHOLD_IRUN(uint32_t input) {
	IHOLD_IRUN_register.sr = input;
	write(IHOLD_IRUN_register.address, IHOLD_IRUN_register.sr);
}

void 	TMCStepper::ihold(uint8_t B) 		{ SET_REG(ihold);		}
void 	TMCStepper::irun(uint8_t B)  		{ SET_REG(irun); 		}
void 	TMCStepper::iholddelay(uint8_t B)	{ SET_REG(iholddelay); 	}

uint8_t TMCStepper::ihold() 				{ GET_REG(ihold);		}
uint8_t TMCStepper::irun()  				{ GET_REG(irun); 		}
uint8_t TMCStepper::iholddelay()  			{ GET_REG(iholddelay);	}
