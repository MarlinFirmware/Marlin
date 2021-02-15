#include "TMCStepper.h"
#include "TMC_MACROS.h"

#define SET_REG(SETTING) SGCSCONF_register.SETTING = B; write(SGCSCONF_register.address, SGCSCONF_register.sr);
#define GET_REG(SETTING) return SGCSCONF_register.SETTING;

uint32_t TMC2660Stepper::SGCSCONF() { return SGCSCONF_register.sr; }
void TMC2660Stepper::SGCSCONF(uint32_t data) {
  SGCSCONF_register.sr = data;
  write(SGCSCONF_register.address, SGCSCONF_register.sr);
}

void TMC2660Stepper::sfilt(bool B) 	{ SET_REG(sfilt); }
void TMC2660Stepper::sgt(uint8_t B) { SET_REG(sgt); }
void TMC2660Stepper::cs(uint8_t B) 	{ SET_REG(cs); }

bool TMC2660Stepper::sfilt() { GET_REG(sfilt); }
uint8_t TMC2660Stepper::sgt(){ GET_REG(sgt); }
uint8_t TMC2660Stepper::cs() { GET_REG(cs); }
