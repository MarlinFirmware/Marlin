#include "TMCStepper.h"
#include "TMC_MACROS.h"

TMC5130Stepper::TMC5130Stepper(uint16_t pinCS, float RS, int8_t link) : TMC2160Stepper(pinCS, RS, link)
  { defaults(); }
TMC5130Stepper::TMC5130Stepper(uint16_t pinCS, float RS, uint16_t pinMOSI, uint16_t pinMISO, uint16_t pinSCK, int8_t link):
  TMC2160Stepper(pinCS, RS, pinMOSI, pinMISO, pinSCK, link)
  { defaults(); }
TMC5130Stepper::TMC5130Stepper(uint16_t pinCS, uint16_t pinMOSI, uint16_t pinMISO, uint16_t pinSCK, int8_t link) :
  TMC2160Stepper(pinCS, default_RS, pinMOSI, pinMISO, pinSCK, link)
  { defaults(); }

void TMC5130Stepper::begin() {
  TMC2160Stepper::begin();

  XTARGET(0);
  XACTUAL(0);
  //while (( RAMP_STAT() & cfg.VZERO_bm) != cfg.VZERO_bm) {}
}

void TMC5130Stepper::defaults() {
  OUTPUT_register.sr = 1;
  ENC_CONST_register.sr = 65536;
  //MSLUT0_register.sr = ???;
  //MSLUT1_register.sr = ???;
  //MSLUT2_register.sr = ???;
  //MSLUT3_register.sr = ???;
  //MSLUT4_register.sr = ???;
  //MSLUT5_register.sr = ???;
  //MSLUT6_register.sr = ???;
  //MSLUT7_register.sr = ???;
  //MSLUTSEL_register.sr = ???;
  //MSLUTSTART_register.start_sin = 0;
  //MSLUTSTART_register.start_sin90 = 247;
  PWMCONF_register.sr = 0x00050480;
}

void TMC5130Stepper::push() {
    IHOLD_IRUN(IHOLD_IRUN_register.sr);
    TPOWERDOWN(TPOWERDOWN_register.sr);
    TPWMTHRS(TPWMTHRS_register.sr);
    GCONF(GCONF_register.sr);
    TCOOLTHRS(TCOOLTHRS_register.sr);
    THIGH(THIGH_register.sr);
    XDIRECT(XDIRECT_register.sr);
    VDCMIN(VDCMIN_register.sr);
    CHOPCONF(CHOPCONF_register.sr);
    COOLCONF(COOLCONF_register.sr);
    DCCTRL(DCCTRL_register.sr);
    PWMCONF(PWMCONF_register.sr);
    ENCM_CTRL(ENCM_CTRL_register.sr);
    DRV_CONF(DRV_CONF_register.sr);
    SLAVECONF(SLAVECONF_register.sr);
    TMC_OUTPUT(OUTPUT_register.sr);
    X_COMPARE(X_COMPARE_register.sr);
    RAMPMODE(RAMPMODE_register.sr);
    XACTUAL(XACTUAL_register.sr);
    VSTART(VSTART_register.sr);
    A1(A1_register.sr);
    V1(V1_register.sr);
    AMAX(AMAX_register.sr);
    VMAX(VMAX_register.sr);
    DMAX(DMAX_register.sr);
    D1(D1_register.sr);
    VSTOP(VSTOP_register.sr);
    TZEROWAIT(TZEROWAIT_register.sr);
    SW_MODE(SW_MODE_register.sr);
    ENCMODE(ENCMODE_register.sr);
    ENC_CONST(ENC_CONST_register.sr);
}

///////////////////////////////////////////////////////////////////////////////////////
// R: IFCNT
uint8_t TMC5130Stepper::IFCNT() { return read(IFCNT_t::address); }
///////////////////////////////////////////////////////////////////////////////////////
// W: SLAVECONF
uint16_t TMC5130Stepper::SLAVECONF() { return SLAVECONF_register.sr; }
void TMC5130Stepper::SLAVECONF(uint16_t input) {
  SLAVECONF_register.sr = input;
  write(SLAVECONF_register.address, SLAVECONF_register.sr);
}
///////////////////////////////////////////////////////////////////////////////////////
// R: IOIN
uint32_t  TMC5130Stepper::IOIN() {
  return read(TMC5130_n::IOIN_t::address);
}
bool    TMC5130Stepper::refl_step()      { TMC5130_n::IOIN_t r{0}; r.sr = IOIN(); return r.refl_step; }
bool    TMC5130Stepper::refr_dir()       { TMC5130_n::IOIN_t r{0}; r.sr = IOIN(); return r.refr_dir; }
bool    TMC5130Stepper::encb_dcen_cfg4() { TMC5130_n::IOIN_t r{0}; r.sr = IOIN(); return r.encb_dcen_cfg4; }
bool    TMC5130Stepper::enca_dcin_cfg5() { TMC5130_n::IOIN_t r{0}; r.sr = IOIN(); return r.enca_dcin_cfg5; }
bool    TMC5130Stepper::drv_enn_cfg6()   { TMC5130_n::IOIN_t r{0}; r.sr = IOIN(); return r.drv_enn_cfg6; }
bool    TMC5130Stepper::enc_n_dco()      { TMC5130_n::IOIN_t r{0}; r.sr = IOIN(); return r.enc_n_dco; }
bool    TMC5130Stepper::sd_mode()        { TMC5130_n::IOIN_t r{0}; r.sr = IOIN(); return r.sd_mode; }
bool    TMC5130Stepper::swcomp_in()      { TMC5130_n::IOIN_t r{0}; r.sr = IOIN(); return r.swcomp_in; }
uint8_t   TMC5130Stepper::version()      { TMC5130_n::IOIN_t r{0}; r.sr = IOIN(); return r.version; }
///////////////////////////////////////////////////////////////////////////////////////
// W: OUTPUT
bool TMC5130Stepper::TMC_OUTPUT() { return OUTPUT_register.sr; }
void TMC5130Stepper::TMC_OUTPUT(bool input) {
  OUTPUT_register.sr = input;
  write(OUTPUT_register.address, OUTPUT_register.sr);
}
///////////////////////////////////////////////////////////////////////////////////////
// W: X_COMPARE
uint32_t TMC5130Stepper::X_COMPARE() { return X_COMPARE_register.sr; }
void TMC5130Stepper::X_COMPARE(uint32_t input) {
  X_COMPARE_register.sr = input;
  write(X_COMPARE_register.address, X_COMPARE_register.sr);
}
///////////////////////////////////////////////////////////////////////////////////////
// RW: RAMPMODE
uint8_t TMC5130Stepper::RAMPMODE() { return read(RAMPMODE_register.address); }
void TMC5130Stepper::RAMPMODE(uint8_t input) {
  RAMPMODE_register.sr = input;
  write(RAMPMODE_register.address, RAMPMODE_register.sr);
}
///////////////////////////////////////////////////////////////////////////////////////
// RW: XACTUAL
int32_t TMC5130Stepper::XACTUAL() { return read(XACTUAL_register.address); }
void TMC5130Stepper::XACTUAL(int32_t input) {
  write(XACTUAL_register.address, input);
}
///////////////////////////////////////////////////////////////////////////////////////
// R: VACTUAL
int32_t TMC5130Stepper::VACTUAL() {
  uint32_t int24 = read(VACTUAL_t::address);
  if((int24 >> 23) & 0x01) {
    int24 |= 0xFF000000;
  }
  return int24;
}
///////////////////////////////////////////////////////////////////////////////////////
// W: VSTART
uint32_t TMC5130Stepper::VSTART() { return VSTART_register.sr; }
void TMC5130Stepper::VSTART(uint32_t input) {
  VSTART_register.sr = input;
  write(VSTART_register.address, VSTART_register.sr);
}
///////////////////////////////////////////////////////////////////////////////////////
// W: A1
uint16_t TMC5130Stepper::A1() { return A1_register.sr; }
void TMC5130Stepper::A1(uint16_t input) {
  A1_register.sr = input;
  write(A1_register.address, A1_register.sr);
}
///////////////////////////////////////////////////////////////////////////////////////
// W: V1
uint32_t TMC5130Stepper::V1() { return V1_register.sr; }
void TMC5130Stepper::V1(uint32_t input) {
  V1_register.sr = input;
  write(V1_register.address, V1_register.sr);
}
///////////////////////////////////////////////////////////////////////////////////////
// W: AMAX
uint16_t TMC5130Stepper::AMAX() { return AMAX_register.sr; }
void TMC5130Stepper::AMAX(uint16_t input) {
  AMAX_register.sr = input;
  write(AMAX_register.address, AMAX_register.sr);
}
///////////////////////////////////////////////////////////////////////////////////////
// W: VMAX
uint32_t TMC5130Stepper::VMAX() { return VMAX_register.sr; }
void TMC5130Stepper::VMAX(uint32_t input) {
  VMAX_register.sr = input;
  write(VMAX_register.address, VMAX_register.sr);
}
///////////////////////////////////////////////////////////////////////////////////////
// W: DMAX
uint16_t TMC5130Stepper::DMAX() { return DMAX_register.sr; }
void TMC5130Stepper::DMAX(uint16_t input) {
  DMAX_register.sr = input;
  write(DMAX_register.address, DMAX_register.sr);
}
///////////////////////////////////////////////////////////////////////////////////////
// W: D1
uint16_t TMC5130Stepper::D1() { return D1_register.sr; }
void TMC5130Stepper::D1(uint16_t input) {
  D1_register.sr = input;
  write(D1_register.address, D1_register.sr);
}
///////////////////////////////////////////////////////////////////////////////////////
// W: VSTOP
uint32_t TMC5130Stepper::VSTOP() { return VSTOP_register.sr; }
void TMC5130Stepper::VSTOP(uint32_t input) {
  if (input == 0 && RAMPMODE() == 0) return;
  VSTOP_register.sr = input;
  write(VSTOP_register.address, VSTOP_register.sr);
}
///////////////////////////////////////////////////////////////////////////////////////
// W: TZEROWAIT
uint16_t TMC5130Stepper::TZEROWAIT() { return TZEROWAIT_register.sr; }
void TMC5130Stepper::TZEROWAIT(uint16_t input) {
  TZEROWAIT_register.sr = input;
  write(TZEROWAIT_register.address, TZEROWAIT_register.sr);
}
///////////////////////////////////////////////////////////////////////////////////////
// RW: XTARGET
int32_t TMC5130Stepper::XTARGET() { return read(XTARGET_t::address); }
void TMC5130Stepper::XTARGET(int32_t input) {
  write(XTARGET_t::address, input);
}
///////////////////////////////////////////////////////////////////////////////////////
// R: XLATCH
uint32_t TMC5130Stepper::XLATCH() { return read(XLATCH_t::address); }
///////////////////////////////////////////////////////////////////////////////////////
// RW: X_ENC
int32_t TMC5130Stepper::X_ENC() { return read(X_ENC_t::address); }
void TMC5130Stepper::X_ENC(int32_t input) {
  write(X_ENC_t::address, input);
}
///////////////////////////////////////////////////////////////////////////////////////
// W: ENC_CONST
uint32_t TMC5130Stepper::ENC_CONST() { return ENC_CONST_register.sr; }
void TMC5130Stepper::ENC_CONST(uint32_t input) {
  ENC_CONST_register.sr = input;
  write(ENC_CONST_register.address, ENC_CONST_register.sr);
}
///////////////////////////////////////////////////////////////////////////////////////
// R: ENC_STATUS
bool TMC5130Stepper::ENC_STATUS() { return read(ENC_STATUS_t::address); }
///////////////////////////////////////////////////////////////////////////////////////
// R: ENC_LATCH
uint32_t TMC5130Stepper::ENC_LATCH() { return read(ENC_LATCH_t::address); }
