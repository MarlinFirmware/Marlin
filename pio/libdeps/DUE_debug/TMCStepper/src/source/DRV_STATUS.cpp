#include "TMCStepper.h"
#include "TMC_MACROS.h"

#define GET_REG(NS, SETTING) NS::DRV_STATUS_t r{0}; r.sr = DRV_STATUS(); return r.SETTING

uint32_t TMC2130Stepper::DRV_STATUS() { return read(DRV_STATUS_t::address); }

uint16_t TMC2130Stepper::sg_result(){ GET_REG(TMC2130_n, sg_result); 	}
bool TMC2130Stepper::fsactive()		{ GET_REG(TMC2130_n, fsactive); 	}
uint8_t TMC2130Stepper::cs_actual()	{ GET_REG(TMC2130_n, cs_actual); 	}
bool TMC2130Stepper::stallguard()	{ GET_REG(TMC2130_n, stallGuard); 	}
bool TMC2130Stepper::ot()			{ GET_REG(TMC2130_n, ot); 			}
bool TMC2130Stepper::otpw()			{ GET_REG(TMC2130_n, otpw); 		}
bool TMC2130Stepper::s2ga()			{ GET_REG(TMC2130_n, s2ga); 		}
bool TMC2130Stepper::s2gb()			{ GET_REG(TMC2130_n, s2gb); 		}
bool TMC2130Stepper::ola()			{ GET_REG(TMC2130_n, ola); 			}
bool TMC2130Stepper::olb()			{ GET_REG(TMC2130_n, olb); 			}
bool TMC2130Stepper::stst()			{ GET_REG(TMC2130_n, stst); 		}

uint32_t TMC2208Stepper::DRV_STATUS() {
	return read(TMC2208_n::DRV_STATUS_t::address);
}

bool 		TMC2208Stepper::otpw()		{ GET_REG(TMC2208_n, otpw); 		}
bool 		TMC2208Stepper::ot() 		{ GET_REG(TMC2208_n, ot); 	 		}
bool 		TMC2208Stepper::s2ga() 		{ GET_REG(TMC2208_n, s2ga); 		}
bool 		TMC2208Stepper::s2gb() 		{ GET_REG(TMC2208_n, s2gb); 		}
bool 		TMC2208Stepper::s2vsa() 	{ GET_REG(TMC2208_n, s2vsa);		}
bool 		TMC2208Stepper::s2vsb() 	{ GET_REG(TMC2208_n, s2vsb);		}
bool 		TMC2208Stepper::ola() 		{ GET_REG(TMC2208_n, ola);  		}
bool 		TMC2208Stepper::olb() 		{ GET_REG(TMC2208_n, olb);  		}
bool 		TMC2208Stepper::t120() 		{ GET_REG(TMC2208_n, t120); 		}
bool 		TMC2208Stepper::t143() 		{ GET_REG(TMC2208_n, t143); 		}
bool 		TMC2208Stepper::t150() 		{ GET_REG(TMC2208_n, t150); 		}
bool 		TMC2208Stepper::t157() 		{ GET_REG(TMC2208_n, t157); 		}
uint16_t 	TMC2208Stepper::cs_actual()	{ GET_REG(TMC2208_n, cs_actual);	}
bool 		TMC2208Stepper::stealth() 	{ GET_REG(TMC2208_n, stealth);		}
bool 		TMC2208Stepper::stst() 		{ GET_REG(TMC2208_n, stst); 		}
