#include "TMCStepper.h"
#include "TMC_MACROS.h"

/*
  Requested current = mA = I_rms/1000
  Equation for current:
  I_rms = (CS+1)/32 * V_fs/(R_sense+0.02ohm) * 1/sqrt(2)
  Solve for CS ->
  CS = 32*sqrt(2)*I_rms*(R_sense+0.02)/V_fs - 1

  Example:
  vsense = 0b0 -> V_fs = 0.325V
  mA = 1640mA = I_rms/1000 = 1.64A
  R_sense = 0.10 Ohm
  ->
  CS = 32*sqrt(2)*1.64*(0.10+0.02)/0.325 - 1 = 26.4
  CS = 26
*/

uint16_t TMCStepper::cs2rms(uint8_t CS) {
  return (float)(CS+1)/32.0 * (vsense() ? 0.180 : 0.325)/(Rsense+0.02) / 1.41421 * 1000;
}

void TMCStepper::rms_current(uint16_t mA) {
  uint8_t CS = 32.0*1.41421*mA/1000.0*(Rsense+0.02)/0.325 - 1;
  // If Current Scale is too low, turn on high sensitivity R_sense and calculate again
  if (CS < 16) {
    vsense(true);
    CS = 32.0*1.41421*mA/1000.0*(Rsense+0.02)/0.180 - 1;
  } else { // If CS >= 16, turn off high_sense_r
    vsense(false);
  }

  if (CS > 31)
    CS = 31;

  irun(CS);
  ihold(CS*holdMultiplier);
  //val_mA = mA;
}
void TMCStepper::rms_current(uint16_t mA, float mult) {
  holdMultiplier = mult;
  rms_current(mA);
}

uint16_t TMCStepper::rms_current() {
  return cs2rms(irun());
}

uint8_t TMCStepper::test_connection() {
  uint32_t drv_status = DRV_STATUS();
  switch (drv_status) {
      case 0xFFFFFFFF: return 1;
      case 0: return 2;
      default: return 0;
  }
}

void TMCStepper::hysteresis_end(int8_t value) { hend(value+3); }
int8_t TMCStepper::hysteresis_end() { return hend()-3; };

void TMCStepper::hysteresis_start(uint8_t value) { hstrt(value-1); }
uint8_t TMCStepper::hysteresis_start() { return hstrt()+1; }

void TMCStepper::microsteps(uint16_t ms) {
  switch(ms) {
    case 256: mres(0); break;
    case 128: mres(1); break;
    case  64: mres(2); break;
    case  32: mres(3); break;
    case  16: mres(4); break;
    case   8: mres(5); break;
    case   4: mres(6); break;
    case   2: mres(7); break;
    case   0: mres(8); break;
    default: break;
  }
}

uint16_t TMCStepper::microsteps() {
  switch(mres()) {
    case 0: return 256;
    case 1: return 128;
    case 2: return  64;
    case 3: return  32;
    case 4: return  16;
    case 5: return   8;
    case 6: return   4;
    case 7: return   2;
    case 8: return   0;
  }
  return 0;
}

void TMCStepper::blank_time(uint8_t value) {
  switch (value) {
    case 16: tbl(0b00); break;
    case 24: tbl(0b01); break;
    case 36: tbl(0b10); break;
    case 54: tbl(0b11); break;
  }
}

uint8_t TMCStepper::blank_time() {
  switch (tbl()) {
    case 0b00: return 16;
    case 0b01: return 24;
    case 0b10: return 36;
    case 0b11: return 54;
  }
  return 0;
}

///////////////////////////////////////////////////////////////////////////////////////
// R+C: GSTAT
uint8_t TMCStepper::GSTAT()  { return read(GSTAT_t::address); }
void  TMCStepper::GSTAT(uint8_t){ write(GSTAT_t::address, 0b111); }
bool  TMCStepper::reset()    { GSTAT_t r; r.sr = GSTAT(); return r.reset; }
bool  TMCStepper::drv_err()  { GSTAT_t r; r.sr = GSTAT(); return r.drv_err; }
bool  TMCStepper::uv_cp()    { GSTAT_t r; r.sr = GSTAT(); return r.uv_cp; }
///////////////////////////////////////////////////////////////////////////////////////
// W: TPOWERDOWN
uint8_t TMCStepper::TPOWERDOWN() { return TPOWERDOWN_register.sr; }
void TMCStepper::TPOWERDOWN(uint8_t input) {
  TPOWERDOWN_register.sr = input;
  write(TPOWERDOWN_register.address, TPOWERDOWN_register.sr);
}
///////////////////////////////////////////////////////////////////////////////////////
// R: TSTEP
uint32_t TMCStepper::TSTEP() { return read(TSTEP_t::address); }
///////////////////////////////////////////////////////////////////////////////////////
// W: TPWMTHRS
uint32_t TMCStepper::TPWMTHRS() { return TPWMTHRS_register.sr; }
void TMCStepper::TPWMTHRS(uint32_t input) {
  TPWMTHRS_register.sr = input;
  write(TPWMTHRS_register.address, TPWMTHRS_register.sr);
}

uint16_t TMCStepper::MSCNT() {
  return read(MSCNT_t::address);
}

uint32_t TMCStepper::MSCURACT() { return read(MSCURACT_t::address); }
int16_t TMCStepper::cur_a() {
  MSCURACT_t r{0};
  r.sr = MSCURACT();
  int16_t value = r.cur_a;
  if (value > 255) value -= 512;
  return value;
}
int16_t TMCStepper::cur_b() {
  MSCURACT_t r{0};
  r.sr = MSCURACT();
  int16_t value = r.cur_b;
  if (value > 255) value -= 512;
  return value;
}
