#include "TMCStepper.h"
#include "TMC_MACROS.h"

#define SET_REG(SETTING) SMARTEN_register.SETTING = B; write(SMARTEN_register.address, SMARTEN_register.sr)
#define GET_REG(SETTING) return SMARTEN_register.SETTING

uint32_t TMC2660Stepper::SMARTEN() { return SMARTEN_register.sr; }
void TMC2660Stepper::SMARTEN(uint32_t data) {
  SMARTEN_register.sr = data;
  write(SMARTEN_register.address, SMARTEN_register.sr);
}

void TMC2660Stepper::seimin(bool B) 	{ SET_REG(seimin);	}
void TMC2660Stepper::sedn(uint8_t B) 	{ SET_REG(sedn); 	}
void TMC2660Stepper::semax(uint8_t B) 	{ SET_REG(semax);	}
void TMC2660Stepper::seup(uint8_t B) 	{ SET_REG(seup);	}
void TMC2660Stepper::semin(uint8_t B) 	{ SET_REG(semin);	}

bool TMC2660Stepper::seimin() 	{ GET_REG(seimin);	}
uint8_t TMC2660Stepper::sedn() 	{ GET_REG(sedn);	}
uint8_t TMC2660Stepper::semax() { GET_REG(semax);	}
uint8_t TMC2660Stepper::seup() 	{ GET_REG(seup);	}
uint8_t TMC2660Stepper::semin() { GET_REG(semin);	}
