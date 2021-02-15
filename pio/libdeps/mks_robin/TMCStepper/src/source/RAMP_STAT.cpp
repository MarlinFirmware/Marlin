#include "TMCStepper.h"
#include "TMC_MACROS.h"

#define GET_REG(SETTING) RAMP_STAT_t r{0}; r.sr = RAMP_STAT(); return r.SETTING

uint32_t TMC5130Stepper::RAMP_STAT() {
	return read(RAMP_STAT_t::address);
}

bool TMC5130Stepper::status_stop_l()		{ GET_REG(status_stop_l);		}
bool TMC5130Stepper::status_stop_r()		{ GET_REG(status_stop_r);		}
bool TMC5130Stepper::status_latch_l()		{ GET_REG(status_latch_l);		}
bool TMC5130Stepper::status_latch_r()		{ GET_REG(status_latch_r);		}
bool TMC5130Stepper::event_stop_l()			{ GET_REG(event_stop_l);		}
bool TMC5130Stepper::event_stop_r()			{ GET_REG(event_stop_r);		}
bool TMC5130Stepper::event_stop_sg()		{ GET_REG(event_stop_sg);		}
bool TMC5130Stepper::event_pos_reached()	{ GET_REG(event_pos_reached);	}
bool TMC5130Stepper::velocity_reached()		{ GET_REG(velocity_reached);	}
bool TMC5130Stepper::position_reached()		{ GET_REG(position_reached);	}
bool TMC5130Stepper::vzero()				{ GET_REG(vzero);	 			}
bool TMC5130Stepper::t_zerowait_active()	{ GET_REG(t_zerowait_active);	}
bool TMC5130Stepper::second_move()			{ GET_REG(second_move);			}
bool TMC5130Stepper::status_sg()			{ GET_REG(status_sg);	 		}
