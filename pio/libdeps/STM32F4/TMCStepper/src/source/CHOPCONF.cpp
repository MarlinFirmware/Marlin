#include "TMCStepper.h"
#include "TMC_MACROS.h"

#define SET_REG(SETTING) CHOPCONF_register.SETTING = B; write(CHOPCONF_register.address, CHOPCONF_register.sr)

// CHOPCONF
uint32_t TMC2130Stepper::CHOPCONF() {
	return read(CHOPCONF_register.address);
}
void TMC2130Stepper::CHOPCONF(uint32_t input) {
	CHOPCONF_register.sr = input;
	write(CHOPCONF_register.address, CHOPCONF_register.sr);
}

void TMC2130Stepper::toff(		uint8_t B )	{ SET_REG(toff);	}
void TMC2130Stepper::hstrt(		uint8_t B )	{ SET_REG(hstrt);	}
void TMC2130Stepper::hend(		uint8_t B )	{ SET_REG(hend);	}
//void TMC2130Stepper::fd(		uint8_t B )	{ SET_REG(fd);		}
void TMC2130Stepper::disfdcc(	bool 	B )	{ SET_REG(disfdcc);	}
void TMC2130Stepper::rndtf(		bool 	B )	{ SET_REG(rndtf);	}
void TMC2130Stepper::chm(		bool 	B )	{ SET_REG(chm);		}
void TMC2130Stepper::tbl(		uint8_t B )	{ SET_REG(tbl);		}
void TMC2130Stepper::vsense(	bool 	B )	{ SET_REG(vsense);	}
void TMC2130Stepper::vhighfs(	bool 	B )	{ SET_REG(vhighfs);	}
void TMC2130Stepper::vhighchm(	bool 	B )	{ SET_REG(vhighchm);}
void TMC2130Stepper::sync(		uint8_t B )	{ SET_REG(sync);	}
void TMC2130Stepper::mres(		uint8_t B )	{ SET_REG(mres);	}
void TMC2130Stepper::intpol(	bool 	B )	{ SET_REG(intpol);	}
void TMC2130Stepper::dedge(		bool 	B )	{ SET_REG(dedge);	}
void TMC2130Stepper::diss2g(	bool 	B )	{ SET_REG(diss2g);	}

uint8_t TMC2130Stepper::toff()		{ CHOPCONF_t r{0}; r.sr = CHOPCONF(); return r.toff;	}
uint8_t TMC2130Stepper::hstrt()		{ CHOPCONF_t r{0}; r.sr = CHOPCONF(); return r.hstrt;	}
uint8_t TMC2130Stepper::hend()		{ CHOPCONF_t r{0}; r.sr = CHOPCONF(); return r.hend;	}
//uint8_t TMC2130Stepper::fd()		{ CHOPCONF_t r{0}; r.sr = CHOPCONF(); return r.fd;		}
bool 	TMC2130Stepper::disfdcc()	{ CHOPCONF_t r{0}; r.sr = CHOPCONF(); return r.disfdcc;	}
bool 	TMC2130Stepper::rndtf()		{ CHOPCONF_t r{0}; r.sr = CHOPCONF(); return r.rndtf;	}
bool 	TMC2130Stepper::chm()		{ CHOPCONF_t r{0}; r.sr = CHOPCONF(); return r.chm;		}
uint8_t TMC2130Stepper::tbl()		{ CHOPCONF_t r{0}; r.sr = CHOPCONF(); return r.tbl;		}
bool 	TMC2130Stepper::vsense()	{ CHOPCONF_t r{0}; r.sr = CHOPCONF(); return r.vsense;	}
bool 	TMC2130Stepper::vhighfs()	{ CHOPCONF_t r{0}; r.sr = CHOPCONF(); return r.vhighfs;	}
bool 	TMC2130Stepper::vhighchm()	{ CHOPCONF_t r{0}; r.sr = CHOPCONF(); return r.vhighchm;}
uint8_t TMC2130Stepper::sync()		{ CHOPCONF_t r{0}; r.sr = CHOPCONF(); return r.sync;	}
uint8_t TMC2130Stepper::mres()		{ CHOPCONF_t r{0}; r.sr = CHOPCONF(); return r.mres;	}
bool 	TMC2130Stepper::intpol()	{ CHOPCONF_t r{0}; r.sr = CHOPCONF(); return r.intpol;	}
bool 	TMC2130Stepper::dedge()		{ CHOPCONF_t r{0}; r.sr = CHOPCONF(); return r.dedge;	}
bool 	TMC2130Stepper::diss2g()	{ CHOPCONF_t r{0}; r.sr = CHOPCONF(); return r.diss2g;	}

void TMC5160Stepper::diss2vs(bool B){ SET_REG(diss2vs); }
void TMC5160Stepper::tpfd(uint8_t B){ SET_REG(tpfd);	}
bool TMC5160Stepper::diss2vs()		{ CHOPCONF_t r{0}; r.sr = CHOPCONF(); return r.diss2vs; }
uint8_t TMC5160Stepper::tpfd()		{ CHOPCONF_t r{0}; r.sr = CHOPCONF(); return r.tpfd;	}

void TMC2208Stepper::CHOPCONF(uint32_t input) {
	CHOPCONF_register.sr = input;
	write(CHOPCONF_register.address, CHOPCONF_register.sr);
}
uint32_t TMC2208Stepper::CHOPCONF() {
	return read(CHOPCONF_register.address);
}
void TMC2208Stepper::toff	( uint8_t  B )	{ SET_REG(toff); 	}
void TMC2208Stepper::hstrt	( uint8_t  B )	{ SET_REG(hstrt); 	}
void TMC2208Stepper::hend	( uint8_t  B )	{ SET_REG(hend); 	}
void TMC2208Stepper::tbl	( uint8_t  B )	{ SET_REG(tbl); 	}
void TMC2208Stepper::vsense	( bool     B )	{ SET_REG(vsense); 	}
void TMC2208Stepper::mres	( uint8_t  B )	{ SET_REG(mres); 	}
void TMC2208Stepper::intpol	( bool     B )	{ SET_REG(intpol); 	}
void TMC2208Stepper::dedge	( bool     B )	{ SET_REG(dedge); 	}
void TMC2208Stepper::diss2g	( bool     B )	{ SET_REG(diss2g); 	}
void TMC2208Stepper::diss2vs( bool     B )	{ SET_REG(diss2vs); }

uint8_t TMC2208Stepper::toff()		{ TMC2208_n::CHOPCONF_t r{0}; r.sr = CHOPCONF(); return r.toff; 	}
uint8_t TMC2208Stepper::hstrt()		{ TMC2208_n::CHOPCONF_t r{0}; r.sr = CHOPCONF(); return r.hstrt; 	}
uint8_t TMC2208Stepper::hend()		{ TMC2208_n::CHOPCONF_t r{0}; r.sr = CHOPCONF(); return r.hend; 	}
uint8_t TMC2208Stepper::tbl()		{ TMC2208_n::CHOPCONF_t r{0}; r.sr = CHOPCONF(); return r.tbl;	 	}
bool 	TMC2208Stepper::vsense()	{ TMC2208_n::CHOPCONF_t r{0}; r.sr = CHOPCONF(); return r.vsense; 	}
uint8_t TMC2208Stepper::mres()		{ TMC2208_n::CHOPCONF_t r{0}; r.sr = CHOPCONF(); return r.mres; 	}
bool 	TMC2208Stepper::intpol()	{ TMC2208_n::CHOPCONF_t r{0}; r.sr = CHOPCONF(); return r.intpol; 	}
bool 	TMC2208Stepper::dedge()		{ TMC2208_n::CHOPCONF_t r{0}; r.sr = CHOPCONF(); return r.dedge; 	}
bool 	TMC2208Stepper::diss2g()	{ TMC2208_n::CHOPCONF_t r{0}; r.sr = CHOPCONF(); return r.diss2g; 	}
bool 	TMC2208Stepper::diss2vs()	{ TMC2208_n::CHOPCONF_t r{0}; r.sr = CHOPCONF(); return r.diss2vs; 	}

#define GET_REG_2660(SETTING) return CHOPCONF_register.SETTING;

uint32_t TMC2660Stepper::CHOPCONF() { return CHOPCONF_register.sr; }
void TMC2660Stepper::CHOPCONF(uint32_t data) {
  CHOPCONF_register.sr = data;
  write(CHOPCONF_register.address, CHOPCONF_register.sr);
}

void TMC2660Stepper::toff(uint8_t B) 	{
	SET_REG(toff);
	if (B>0) _savedToff = B;
}
void TMC2660Stepper::hstrt(uint8_t B) 	{ SET_REG(hstrt); 	}
void TMC2660Stepper::hend(uint8_t B) 	{ SET_REG(hend);	}
void TMC2660Stepper::hdec(uint8_t B) 	{ SET_REG(hdec);	}
void TMC2660Stepper::rndtf(bool B) 	{ SET_REG(rndtf);	}
void TMC2660Stepper::chm(bool B) 	{ SET_REG(chm);	}
void TMC2660Stepper::tbl(uint8_t B) 	{ SET_REG(tbl);	}

uint8_t TMC2660Stepper::toff() 	{ GET_REG_2660(toff);	}
uint8_t TMC2660Stepper::hstrt() 	{ GET_REG_2660(hstrt);	}
uint8_t TMC2660Stepper::hend() { GET_REG_2660(hend);	}
uint8_t TMC2660Stepper::hdec() 	{ GET_REG_2660(hdec);	}
bool TMC2660Stepper::rndtf() { GET_REG_2660(rndtf);	}
bool TMC2660Stepper::chm() 	{ GET_REG_2660(chm);	}
uint8_t TMC2660Stepper::tbl() { GET_REG_2660(tbl);	}
