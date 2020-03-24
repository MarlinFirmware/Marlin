/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../../inc/MarlinConfig.h"

#if ENABLED(CNC_5X)

#include "../../feature/bedlevel/bedlevel.h"

#include "../../MarlinCore.h"
#include "../../module/planner.h"
#include "../../module/stepper.h"
#include "../../module/motion.h"
#include "../../module/tool_change.h"
#include "../../module/temperature.h"
#include "../../lcd/ultralcd.h"
#include "../../lcd/menu/menu.h"
#include "../../HAL/shared/Delay.h"
#include "../../module/stepper.h"
#include "../../module/endstops.h"
#include "../gcode.h"


/**
 *   M168 - 5 Axis Simple Convertion (Non Modal)
 *
 *  X Y Z - position of cartesians axis
 *  A B C - position of rotary axis
 * 
 */

// External references

extern xyze_pos_t destination;

extern bool fast_move;/*=false*/


// Private functions

int FLG5X,
    HomeDir5x,
    Home5x;

double X_5,
      Y_5,
      Z_5,
      IX_5,
      IY_5,
      IZ_5,
      OX_5,
      OY_5,
      OZ_5,
      A_5,
      B_5,
      C_5,
      IA_5,
      IB_5,
      IC_5,
      OA_5,
      OB_5,
      OC_5,
      E_5,
      I_5,
      J_5 = 0;




inline void M167_single_probe() {
  endstops.enable(true);\
  M167_move = 2;
  prepare_line_to_destination();
  planner.synchronize();
  M167_move = 0;
  endstops.hit_on_purpose();
  set_current_from_steppers_for_axis(ALL_AXES);
  sync_plan_position();
}

inline bool M167_run_probe() {

  bool M167_pass_fail = false;

  planner.synchronize();  // wait until the machine is idle

  M167_did_trigger = false;

  // Move until destination reached or target hit
  M167_single_probe();

  if (M167_did_trigger) {

    M167_pass_fail = true;
  }

  endstops.not_homing();
  return M167_pass_fail;
}


inline void M167_Probe() {

  remember_feedrate_scaling_off();

  const bool error_on_fail = 2;

  // If any axis has enough movement, do the move
  LOOP_XYZE(i)
    if (ABS(destination[i] - current_position[i]) >= G38_MINIMUM_MOVE) {
      if (!parser.seenval('F')) feedrate_mm_s = homing_feedrate((AxisEnum)i);
      // If fails throw an error
      if (!M167_run_probe() && error_on_fail) SERIAL_ERROR_MSG("Failed to reach target");
      break;
    }

  restore_feedrate_and_scaling();
}







/**
 * M167 - Home Rotary
 */

void GcodeSuite::M167()
{
  
    // Get X Y Z E F
    get_destination_from_command();

    SERIAL_ECHOLN("Entrou no M167");

    #if ENABLED (A_5x) && ENABLED(B_5x)

      // A e B

    #endif

    #if ENABLED (A_5x) && ENABLED(C_5x)

    // A e C
      HomeDir5x = 0;
      if(A_HOME_DIR>=0) HomeDir5x += 0; else HomeDir5x +=1;
      if(C_HOME_DIR>=0) HomeDir5x += 0; else HomeDir5x +=2;

    //Set base Directions of Home
      if (HomeDir5x==0 || HomeDir5x==2) {OUT_WRITE(EA_DIR_PIN,!INVERT_EA_DIR);} else {OUT_WRITE(EA_DIR_PIN,INVERT_EA_DIR);}
      if (HomeDir5x==0 || HomeDir5x==1) {OUT_WRITE(EB_DIR_PIN,!INVERT_EB_DIR);} else {OUT_WRITE(EB_DIR_PIN,INVERT_EB_DIR);}

      Home5x = 1;

      SERIAL_ECHO("Calculou HomeDir5X: ");
      SERIAL_ECHOLN(HomeDir5x);

    // Zero A
      mixer.set_collector(0, 1);
      mixer.set_collector(1, 0);
      mixer.normalize(0);

      SERIAL_ECHOLN("Fez o mix A");

      
      
      M167_Probe();

      //homeaxis(E_AXIS);

      SERIAL_ECHOLN("Fez o Home ao A?");

      OA_5 = 0;

      SERIAL_ECHOLN("Zerou o A");
      
    // Zero C

      mixer.set_collector(0, 0);
      mixer.set_collector(1, 1);
      mixer.normalize(0);

      SERIAL_ECHOLN("Fez o mix C");

      M167_Probe();

      //homeaxis(E_AXIS);

      SERIAL_ECHOLN("Fez o Home ao C?");

      OC_5 = 0;

      SERIAL_ECHOLN("Zerou o C");

      Home5x = 0;
      
    #endif


    #if ENABLED (B_5x) && ENABLED(C_5x)

      if (IB_5>=0) {FLG5X += 0; I_5=IB_5;} else {FLG5X += 1; I_5=-IB_5;}

      if (IC_5>=0) {FLG5X += 0; J_5=IC_5;} else {FLG5X += 2; J_5=-IC_5;} 

    #endif

    SERIAL_ECHOLN("Sai do M167");

}
/**
 * M168
 */
void GcodeSuite::M168()
{
  
    if (parser.seen('X')) {
      
      X_5 = parser.value_float();

      //SERIAL_ECHO("X");
      //SERIAL_ECHOLN(X_5);
      }
      else
      {
        X_5 = current_position.x;
      }
      

    if (parser.seen('Y')) {
      
      Y_5 = parser.value_float();

      //SERIAL_ECHO("Y");
      //SERIAL_ECHOLN(Y_5);
      }
      else
      {
        Y_5 = current_position.y;
      }
    
    if (parser.seen('Z')) {
      
      Z_5 = parser.value_float();

      //SERIAL_ECHO("Z");
      //SERIAL_ECHOLN(Z_5);
      }
      else
      {
        Z_5 = current_position.z;
      }

    if (parser.seen('A')) {
      
      A_5 = parser.value_float();
    
      //SERIAL_ECHO("A");
      //SERIAL_ECHOLN(A_5);
      }

    if (parser.seen('B')) {
      
      B_5 = parser.value_float();
   
      //SERIAL_ECHO("B");
      //SERIAL_ECHOLN(B_5);
      }

    if (parser.seen('C')) {
      
      C_5 = parser.value_float();
   
      //SERIAL_ECHO("C");
      //SERIAL_ECHOLN(C_5);
      }

    if (parser.linearval('F') > 0)
    feedrate_mm_s = parser.value_feedrate();

    IA_5=A_5-OA_5;
    IB_5=B_5-OB_5;
    IC_5=C_5-OC_5;

    FLG5X = 0;

    #if ENABLED (A_5x) && ENABLED(B_5x)

      if (IA_5>=0) {FLG5X += 0; I_5=IA_5;} else {FLG5X += 1; I_5=-IA_5;}

      if (IB_5>=0) {FLG5X += 0; J_5=IB_5;} else {FLG5X += 2; J_5=-IB_5;} 

    #endif

    #if ENABLED (A_5x) && ENABLED(C_5x)

      if (IA_5>=0) {FLG5X += 0; I_5=IA_5;} else {FLG5X += 1; I_5=-IA_5;}

      if (IC_5>=0) {FLG5X += 0; J_5=IC_5;} else {FLG5X += 2; J_5=-IC_5;}

    #endif

    #if ENABLED (B_5x) && ENABLED(C_5x)

      if (IB_5>=0) {FLG5X += 0; I_5=IB_5;} else {FLG5X += 1; I_5=-IB_5;}

      if (IC_5>=0) {FLG5X += 0; J_5=IC_5;} else {FLG5X += 2; J_5=-IC_5;} 

    #endif

//try to change E steppers directions from M168

  #if MINIMUM_STEPPER_PRE_DIR_DELAY > 0
    DELAY_NS(MINIMUM_STEPPER_PRE_DIR_DELAY);
  #endif
  
    if (FLG5X==0 || FLG5X==2) {OUT_WRITE(EA_DIR_PIN,!INVERT_EA_DIR);} else {OUT_WRITE(EA_DIR_PIN,INVERT_EA_DIR);}
    
    if (FLG5X==0 || FLG5X==1) {OUT_WRITE(EB_DIR_PIN,!INVERT_EB_DIR);} else {OUT_WRITE(EB_DIR_PIN,INVERT_EB_DIR);}


    OA_5=A_5;
    OB_5=B_5;
    OC_5=C_5;

    SERIAL_ECHOLN("X"&&X_5&&" Y"&&Y_5&&" Z"&&Z_5&&" A"&&OA_5&&" B"&&OB_5&&" C"&&OC_5);

    E_5=(I_5+J_5);
       
    mixer.set_collector(0, I_5);
    mixer.set_collector(1, J_5);
    mixer.normalize(0);
    
    destination.x =  X_5;
    destination.y =  Y_5;
    destination.z =  Z_5;
    destination.e =  (current_position.e + E_5);
  
    #if MINIMUM_STEPPER_POST_DIR_DELAY > 0
      DELAY_NS(MINIMUM_STEPPER_POST_DIR_DELAY);
    #endif
    
      prepare_internal_move_to_destination();
      
      planner.synchronize();   

}

#endif

