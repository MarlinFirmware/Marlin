/*
  stepper_indirection.c - stepper motor driver indirection 
  to allow some stepper functions to be done via SPI/I2c instead of direct pin manipulation
  Part of Marlin

  Copyright (c) 2015 Dominik Wenger

  Marlin is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Marlin is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Marlin.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "stepper_indirection.h"
#include "Configuration.h"

#ifdef HAVE_TMCDRIVER
#include <SPI.h>
#include <TMC26XStepper.h>
#endif

// Stepper objects of TMC steppers used
#ifdef X_IS_TMC
	TMC26XStepper stepperX(200,X_ENABLE_PIN,X_STEP_PIN,X_DIR_PIN,X_MAX_CURRENT,X_SENSE_RESISTOR);
#endif
#ifdef X2_IS_TMC
	TMC26XStepper stepperX2(200,X2_ENABLE_PIN,X2_STEP_PIN,X2_DIR_PIN,X2_MAX_CURRENT,X2_SENSE_RESISTOR);
#endif
#ifdef Y_IS_TMC
	TMC26XStepper stepperY(200,Y_ENABLE_PIN,Y_STEP_PIN,Y_DIR_PIN,Y_MAX_CURRENT,Y_SENSE_RESISTOR);
#endif
#ifdef Y2_IS_TMC
	TMC26XStepper stepperY2(200,Y2_ENABLE_PIN,Y2_STEP_PIN,Y2_DIR_PIN,Y2_MAX_CURRENT,Y2_SENSE_RESISTOR);
#endif
#ifdef Z_IS_TMC
	TMC26XStepper stepperZ(200,Z_ENABLE_PIN,Z_STEP_PIN,Z_DIR_PIN,Z_MAX_CURRENT,Z_SENSE_RESISTOR);
#endif
#ifdef Z2_IS_TMC
	TMC26XStepper stepperZ2(200,Z2_ENABLE_PIN,Z2_STEP_PIN,Z2_DIR_PIN,Z2_MAX_CURRENT,Z2_SENSE_RESISTOR);
#endif
#ifdef E0_IS_TMC
	TMC26XStepper stepperE0(200,E0_ENABLE_PIN,E0_STEP_PIN,E0_DIR_PIN,E0_MAX_CURRENT,E0_SENSE_RESISTOR);
#endif
#ifdef E1_IS_TMC
	TMC26XStepper stepperE1(200,E1_ENABLE_PIN,E1_STEP_PIN,E1_DIR_PIN,E1_MAX_CURRENT,E1_SENSE_RESISTOR);
#endif
#ifdef E2_IS_TMC
	TMC26XStepper stepperE2(200,E2_ENABLE_PIN,E2_STEP_PIN,E2_DIR_PIN,E2_MAX_CURRENT,E2_SENSE_RESISTOR);
#endif
#ifdef E3_IS_TMC
	TMC26XStepper stepperE3(200,E3_ENABLE_PIN,E3_STEP_PIN,E3_DIR_PIN,E3_MAX_CURRENT,E3_SENSE_RESISTOR);
#endif	

#ifdef HAVE_TMCDRIVER
void tmc_init()
{
#ifdef X_IS_TMC
	stepperX.setMicrosteps(X_MICROSTEPS);
	stepperX.start();
#endif
#ifdef X2_IS_TMC
	stepperX2.setMicrosteps(X2_MICROSTEPS);
	stepperX2.start();
#endif
#ifdef Y_IS_TMC
	stepperY.setMicrosteps(Y_MICROSTEPS);
	stepperY.start();
#endif
#ifdef Y2_IS_TMC
	stepperY2.setMicrosteps(Y2_MICROSTEPS);
	stepperY2.start();
#endif
#ifdef Z_IS_TMC
	stepperZ.setMicrosteps(Z_MICROSTEPS);
	stepperZ.start();
#endif
#ifdef Z2_IS_TMC
	stepperZ2.setMicrosteps(Z2_MICROSTEPS);
	stepperZ2.start();
#endif
#ifdef E0_IS_TMC
	stepperE0.setMicrosteps(E0_MICROSTEPS);
	stepperE0.start();
#endif
#ifdef E1_IS_TMC
	stepperE1.setMicrosteps(E1_MICROSTEPS);
	stepperE1.start();
#endif
#ifdef E2_IS_TMC
	stepperE2.setMicrosteps(E2_MICROSTEPS);
	stepperE2.start();
#endif
#ifdef E3_IS_TMC
	stepperE3.setMicrosteps(E3_MICROSTEPS);
	stepperE3.start();
#endif
}
#endif