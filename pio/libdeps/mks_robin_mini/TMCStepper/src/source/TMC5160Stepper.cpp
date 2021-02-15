#include "TMCStepper.h"
#include "TMC_MACROS.h"

TMC5160Stepper::TMC5160Stepper(uint16_t pinCS, float RS, int8_t link) : TMC5130Stepper(pinCS, RS, link)
  { defaults(); }
TMC5160Stepper::TMC5160Stepper(uint16_t pinCS, float RS, uint16_t pinMOSI, uint16_t pinMISO, uint16_t pinSCK, int8_t link) :
  TMC5130Stepper(pinCS, RS, pinMOSI, pinMISO, pinSCK, link)
  { defaults(); }
TMC5160Stepper::TMC5160Stepper(uint16_t pinCS, uint16_t pinMOSI, uint16_t pinMISO, uint16_t pinSCK, int8_t link) :
  TMC5130Stepper(pinCS, default_RS, pinMOSI, pinMISO, pinSCK, link)
  { defaults(); }

void TMC5160Stepper::defaults() {
  SHORT_CONF_register.s2vs_level = 6;
  SHORT_CONF_register.s2g_level = 6;
  SHORT_CONF_register.shortfilter = 0b01;
  SHORT_CONF_register.shortdelay = 0;
  DRV_CONF_register.bbmtime = 0;
  DRV_CONF_register.bbmclks = 4;
  DRV_CONF_register.otselect = 0b00;
  DRV_CONF_register.drvstrength = 0b10;
  DRV_CONF_register.filt_isense = 0b00;
  TPOWERDOWN_register.sr = 10;
  VSTOP_register.sr = 1;
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
  CHOPCONF_register.sr = 0x10410150;
  PWMCONF_register.sr = 0xC40C001E;
}

void TMC5160Stepper::push() {
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
    SHORT_CONF(SHORT_CONF_register.sr);
    DRV_CONF(DRV_CONF_register.sr);
    GLOBAL_SCALER(GLOBAL_SCALER_register.sr);
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
	ENC_DEVIATION(ENC_DEVIATION_register.sr);
}

// R+WC: ENC_STATUS
uint8_t TMC5160Stepper::ENC_STATUS() { return read(ENC_STATUS_t::address); }
void TMC5160Stepper::ENC_STATUS(uint8_t input) {
	write(ENC_STATUS_t::address, input & 0x3);
}

// W: ENC_DEVIATION
uint32_t TMC5160Stepper::ENC_DEVIATION() { return ENC_DEVIATION_register.sr; }
void TMC5160Stepper::ENC_DEVIATION(uint32_t input) {
	ENC_DEVIATION_register.sr = input;
	write(ENC_DEVIATION_register.address, ENC_DEVIATION_register.sr);
}

// R: PWM_AUTO
uint32_t TMC5160Stepper::PWM_AUTO() {
	return read(PWM_AUTO_t::address);
}
uint8_t TMC5160Stepper::pwm_ofs_auto()  { PWM_AUTO_t r{0}; r.sr = PWM_AUTO(); return r.pwm_ofs_auto; }
uint8_t TMC5160Stepper::pwm_grad_auto() { PWM_AUTO_t r{0}; r.sr = PWM_AUTO(); return r.pwm_grad_auto; }
