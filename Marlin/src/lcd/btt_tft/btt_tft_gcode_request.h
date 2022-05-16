#pragma once

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(BTT_TFT)

#include "bttSerial.h"
#include "../../module/temperature.h"
#include "bttSerial.h"
#include "bttSerial_STM32.h"



class btt_tft_gcode_request{
public:
void ok_send();
void M105_report(){
    btt_tft_gcode_request::ok_send();
    btt_tft_gcode_request::temp_report();
    }
void temp_report();
//void postion();
bool M503_report(const char* message);
void M92_report();
/*void M114_report(){
    btt_tft_gcode_request::postion();
    btt_tft_gcode_request::ok_send();
    }
void M211_report();
void M569_report();
void M115_report();
*/

#ifdef test
void report_heading(const bool forReplay, FSTR_P const fstr, const bool eol/*=true*/);
void report_echo_start(const bool forReplay);
void say_units();
void M149_report(const bool forReplay/*=true*/);
void report(const bool forReplay);
void M92_report(const bool echo=true, const int8_t e=-1);
void M200_report(const bool forReplay/*=true*/);
#endif


};
#endif


/* M503 
Recv: echo:; Linear Units:
Recv: echo:  G21 ; (mm)
Recv: echo:; Temperature Units:
Recv: echo:  M149 C ; Units in Celsius
Recv: echo:; Filament settings (Disabled):
Recv: echo:  M200 S0 D1.7500
Recv: echo:; Steps per unit:
Recv: echo:  M92 X160.0000 Y160.0000 Z800.0000 E411.8000
Recv: echo:; Max feedrates (units/s):
Recv: echo:  M203 X200.0000 Y200.0000 Z18.0000 E25.0000
Recv: echo:; Max Acceleration (units/s2):
Recv: echo:  M201 X3000.0000 Y3000.0000 Z600.0000 E40000.0000
Recv: echo:; Acceleration (units/s2) (P<print-accel> R<retract-accel> T<travel-accel>):
Recv: echo:  M204 P1500.0000 R5000.0000 T3000.0000
Recv: echo:; Advanced (B<min_segment_time_us> S<min_feedrate> T<min_travel_feedrate> J<junc_dev>):
Recv: echo:  M205 B20000.0000 S0.0000 T0.0000 J0.0130
Recv: echo:; Home offset:
Recv: echo:  M206 X0.0000 Y0.0000 Z0.0000
Recv: echo:; Unified Bed Leveling:
Recv: echo:  M420 S0 Z10.0000 ; Leveling OFF
Recv: 
Recv: Unified Bed Leveling System v1.01 inactive
Recv: echo:Active Mesh Slot -1
Recv: echo:EEPROM can hold 8 meshes.
Recv: 
Recv: echo:; Z Auto-Align:
Recv: echo:  M422 S1 X15.0000 Y200.0000
Recv: echo:  M422 S2 X376.0000 Y200.0000
Recv: echo:; Hotend PID:
Recv: echo:  M301 P24.6318 I2.4149 D62.8112
Recv: echo:; Bed PID:
Recv: echo:  M304 P153.4584 I30.2084 D519.7126
Recv: echo:; Power-loss recovery:
Recv: echo:  M413 S0 ; OFF
Recv: echo:; Retract (S<length> F<feedrate> Z<lift>):
Recv: echo:  M207 S60.0000 W13.0000 F2700.0000 Z0.0000
Recv: echo:; Recover (S<length> F<feedrate>):
Recv: echo:  M208 S0.0000 W0.0000 F480.0000
Recv: echo:; Auto-Retract (S<enable>):
Recv: echo:  M209 S0
Recv: echo:; Z-Probe Offset:
Recv: echo:  M851 X-41.0000 Y-13.0000 Z-1.0000 ; (mm)
Recv: echo:; Stepper driver current:
Recv: echo:  M906 X650 Y650 Z400
Recv: echo:  M906 I1 Z400
Recv: echo:  M906 T0 E650
Recv: 
Recv: echo:; Driver stepping mode:
Recv: echo:  M569 S1 X Y Z
Recv: echo:  M569 S1 I1 Z
Recv: echo:; Linear Advance:
Recv: echo:  M900 K1.3000
Recv: echo:; Filament load/unload:
Recv: echo:  M603 L0.0000 U100.0000 ; (mm)
Recv: echo:; Filament runout sensor:
Recv: echo:  M412 S1 ; Sensor ON
Recv: ok

*/




