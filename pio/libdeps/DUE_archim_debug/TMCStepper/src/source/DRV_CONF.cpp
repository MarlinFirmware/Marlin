#include "TMCStepper.h"
#include "TMC_MACROS.h"

#define SET_REG(SETTING) DRV_CONF_register.SETTING = B; write(DRV_CONF_register.address, DRV_CONF_register.sr);
#define GET_REG(SETTING) return DRV_CONF_register.SETTING;

// W: DRV_CONF
uint32_t TMC2160Stepper::DRV_CONF() { return DRV_CONF_register.sr; }
void TMC2160Stepper::DRV_CONF(uint32_t input) {
	DRV_CONF_register.sr = input;
	write(DRV_CONF_register.address, DRV_CONF_register.sr);
}

void TMC2160Stepper::bbmtime(uint8_t B)		{ SET_REG(bbmtime); 	}
void TMC2160Stepper::bbmclks(uint8_t B)		{ SET_REG(bbmclks); 	}
void TMC2160Stepper::otselect(uint8_t B)	{ SET_REG(otselect); 	}
void TMC2160Stepper::drvstrength(uint8_t B)	{ SET_REG(drvstrength); }
void TMC2160Stepper::filt_isense(uint8_t B)	{ SET_REG(filt_isense); }
uint8_t TMC2160Stepper::bbmtime()			{ GET_REG(bbmtime);		}
uint8_t TMC2160Stepper::bbmclks()			{ GET_REG(bbmclks);		}
uint8_t TMC2160Stepper::otselect()			{ GET_REG(otselect);	}
uint8_t TMC2160Stepper::drvstrength()		{ GET_REG(drvstrength);	}
uint8_t TMC2160Stepper::filt_isense()		{ GET_REG(filt_isense);	}
