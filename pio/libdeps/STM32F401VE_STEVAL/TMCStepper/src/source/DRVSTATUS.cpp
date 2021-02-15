#include "TMCStepper.h"
#include "TMC_MACROS.h"

#define GET_REG00(SETTING) DRVSTATUS(); return READ_RDSEL00_register.SETTING
#define GET_REG01(SETTING) DRVSTATUS(); return READ_RDSEL01_register.SETTING
#define GET_REG10(SETTING) DRVSTATUS(); return READ_RDSEL10_register.SETTING

uint32_t TMC2660Stepper::DRVSTATUS() {
	uint32_t response = read()&0xFFCFF;
	READ_RDSEL00_register.sr = response & 0xFF;
	READ_RDSEL01_register.sr = response & 0xFF;
	READ_RDSEL10_register.sr = response & 0xFF;
	switch(rdsel()) {
		case 0b00: READ_RDSEL00_register.sr |= response & 0xFFC00; break;
		case 0b01: READ_RDSEL01_register.sr |= response & 0xFFC00; break;
		case 0b10: READ_RDSEL10_register.sr |= response & 0xFFC00; break;
		default: return 0;
	}
	return response;
}

uint16_t TMC2660Stepper::mstep()	{ if(rdsel() != 0b00) rdsel(0b00); GET_REG00(mstep); 	}
uint8_t TMC2660Stepper::se() 		{ if(rdsel() != 0b10) rdsel(0b10); GET_REG10(se); 		}
bool TMC2660Stepper::stst() 		{ GET_REG00(stst); }
bool TMC2660Stepper::olb() 			{ GET_REG00(olb);	 }
bool TMC2660Stepper::ola() 			{ GET_REG00(ola);	 }
bool TMC2660Stepper::s2gb() 		{ GET_REG00(s2gb); }
bool TMC2660Stepper::s2ga() 		{ GET_REG00(s2ga); }
bool TMC2660Stepper::otpw() 		{ GET_REG00(otpw); }
bool TMC2660Stepper::ot() 			{ GET_REG00(ot);	 }
bool TMC2660Stepper::sg() 			{ GET_REG00(sg_value);	 }

uint16_t TMC2660Stepper::sg_result(){
	uint16_t out = 0;
	if (rdsel() == 0b00) rdsel(0b01);
	DRVSTATUS();
	switch(rdsel()) {
		case 0b01: out = READ_RDSEL01_register.sg_result; break;
		case 0b10: out = READ_RDSEL10_register.sg_result; break;
		default: break;
	}
	return out;
}
