

#include "../../inc/MarlinConfigPre.h"


#if ENABLED(BTT_TFT)
#include "btt_tft_gcode_request.h"
#include "../../../src/libs/numtostr.h"
#include "../extui/ui_api.h"

using namespace ExtUI;


int8_t tempBuf[100] = {0};

void btt_tft_gcode_request::ok_send() { BTT_TFT_SERIAL.println("ok"); }

void btt_tft_gcode_request::temp_report(){
  
  ZERO(tempBuf);
  char *outBuf = (char *)tempBuf;
  char tbuf[34];

  sprintf_P(tbuf, PSTR("%d /%d"), thermalManager.wholeDegHotend(0), thermalManager.degTargetHotend(0));
  const int tlen = strlen(tbuf);
  sprintf_P(outBuf, PSTR("T:%s"), tbuf);
  outBuf += 2 + tlen;

  strcpy_P(outBuf, PSTR(" B:"));
  outBuf += 3;
#if HAS_HEATED_BED
  sprintf_P(outBuf, PSTR("%d /%d"), thermalManager.wholeDegBed(), thermalManager.degTargetBed());
#else
  strcpy_P(outBuf, PSTR("0 /0"));
#endif
  outBuf += 4;
  #if HAS_MULTI_HOTEND
  strcat_P(outBuf, PSTR(" T0:"));
  strcat(outBuf, tbuf);
  outBuf += 4 + tlen;

  strcat_P(outBuf, PSTR(" T1:"));
  outBuf += 4;

    sprintf_P(outBuf, PSTR("%d /%d"), thermalManager.wholeDegHotend(1), thermalManager.degTargetHotend(1));

  outBuf += 4;
  #endif
  strcat_P(outBuf, PSTR(" @:0 B@:0\r\n"));

  BTT_TFT_SERIAL.print((char *)tempBuf);
}
void btt_tft_gcode_request::M92_report(){
  //   M92 X160.0000 Y160.0000 Z800.0000 E411.8000
  // dont work 
  char buf[100];
  int i = ExtUI::extruderCount;
  switch (i)
  {
  case 1:
    sprintf(buf,"   M92 X%f Y%f Z%f E%f",
    ExtUI::getAxisSteps_per_mm(X),
    ExtUI::getAxisSteps_per_mm(Y),
    ExtUI::getAxisSteps_per_mm(Z),
    ExtUI::getAxisSteps_per_mm(E0));
    break;
    case 2:
    sprintf(buf,"   M92 X%f Y%f Z%f E0%f E1%f",
    ExtUI::getAxisSteps_per_mm(X),
    ExtUI::getAxisSteps_per_mm(Y),
    ExtUI::getAxisSteps_per_mm(Z),
    ExtUI::getAxisSteps_per_mm(E0),
    ExtUI::getAxisSteps_per_mm(E1));
    break;
    case 3:
    sprintf(buf,"   M92 X%f Y%f Z%f E0%f E1%f E2%f",
    ExtUI::getAxisSteps_per_mm(X),
    ExtUI::getAxisSteps_per_mm(Y),
    ExtUI::getAxisSteps_per_mm(Z),
    ExtUI::getAxisSteps_per_mm(E0),
    ExtUI::getAxisSteps_per_mm(E1),
    ExtUI::getAxisSteps_per_mm(E2));
    break;
    case 4:
    sprintf(buf,"   M92 X%f Y%f Z%f E0%f E1%f E2%f E3%f",
    ExtUI::getAxisSteps_per_mm(X),
    ExtUI::getAxisSteps_per_mm(Y),
    ExtUI::getAxisSteps_per_mm(Z),
    ExtUI::getAxisSteps_per_mm(E0),
    ExtUI::getAxisSteps_per_mm(E1),
    ExtUI::getAxisSteps_per_mm(E2),
    ExtUI::getAxisSteps_per_mm(E3));
    break;
    case 5:
    sprintf(buf,"   M92 X%f Y%f Z%f E0%f E1%f E2%f E3%f E4%f",
    ExtUI::getAxisSteps_per_mm(X),
    ExtUI::getAxisSteps_per_mm(Y),
    ExtUI::getAxisSteps_per_mm(Z),
    ExtUI::getAxisSteps_per_mm(E0),
    ExtUI::getAxisSteps_per_mm(E1),
    ExtUI::getAxisSteps_per_mm(E2),
    ExtUI::getAxisSteps_per_mm(E3),
    ExtUI::getAxisSteps_per_mm(E4));
    break;
    case 6:
    sprintf(buf,"   M92 X%f Y%f Z%f E0%f E1%f E2%f E3%f E4%f E5%f",
    ExtUI::getAxisSteps_per_mm(X),
    ExtUI::getAxisSteps_per_mm(Y),
    ExtUI::getAxisSteps_per_mm(Z),
    ExtUI::getAxisSteps_per_mm(E0),
    ExtUI::getAxisSteps_per_mm(E1),
    ExtUI::getAxisSteps_per_mm(E2),
    ExtUI::getAxisSteps_per_mm(E3),
    ExtUI::getAxisSteps_per_mm(E4),
    ExtUI::getAxisSteps_per_mm(E5));
    break;
    case 7:
    sprintf(buf,"   M92 X%f Y%f Z%f E0%f E1%f E2%f E3%f E4%f E5%f E6%f",
    ExtUI::getAxisSteps_per_mm(X),
    ExtUI::getAxisSteps_per_mm(Y),
    ExtUI::getAxisSteps_per_mm(Z),
    ExtUI::getAxisSteps_per_mm(E0),
    ExtUI::getAxisSteps_per_mm(E1),
    ExtUI::getAxisSteps_per_mm(E2),
    ExtUI::getAxisSteps_per_mm(E3),
    ExtUI::getAxisSteps_per_mm(E4),
    ExtUI::getAxisSteps_per_mm(E5),
    ExtUI::getAxisSteps_per_mm(E6));
    break;
    case 8:
    sprintf(buf,"   M92 X%f Y%f Z%f E0%f E1%f E2%f E3%f E4%f E5%f E6%f E7%f",
    ExtUI::getAxisSteps_per_mm(X),
    ExtUI::getAxisSteps_per_mm(Y),
    ExtUI::getAxisSteps_per_mm(Z),
    ExtUI::getAxisSteps_per_mm(E0),
    ExtUI::getAxisSteps_per_mm(E1),
    ExtUI::getAxisSteps_per_mm(E2),
    ExtUI::getAxisSteps_per_mm(E3),
    ExtUI::getAxisSteps_per_mm(E4),
    ExtUI::getAxisSteps_per_mm(E5),
    ExtUI::getAxisSteps_per_mm(E6),
    ExtUI::getAxisSteps_per_mm(E7));
    break;
  default:
    strcpy(buf,"error");
    break;
  }
  
  
  
}

bool btt_tft_gcode_request::M503_report(const char* message){

  if(strcmp(message,"M503 S0")==0){

BTT_TFT_SERIAL.println("   G21 ; (mm)");
BTT_TFT_SERIAL.println("   M149 C ; Units in Celsius");
BTT_TFT_SERIAL.println("   M200 S0 D1.7500");
BTT_TFT_SERIAL.println("   M92 X160.0000 Y160.0000 Z800.0000 E411.8000");
BTT_TFT_SERIAL.println("   M203 X200.0000 Y200.0000 Z18.0000 E25.0000");
BTT_TFT_SERIAL.println("   M201 X3000.0000 Y3000.0000 Z600.0000 E40000.0000");
BTT_TFT_SERIAL.println("   M204 P1500.0000 R5000.0000 T3000.0000");
BTT_TFT_SERIAL.println("   M205 B20000.0000 S0.0000 T0.0000 J0.0130");
BTT_TFT_SERIAL.println("   M206 X0.0000 Y0.0000 Z0.0000");
BTT_TFT_SERIAL.println("   M420 S0 Z10.0000 ; Leveling OFF");
BTT_TFT_SERIAL.println("   M422 S1 X15.0000 Y200.0000");
BTT_TFT_SERIAL.println("   M422 S2 X376.0000 Y200.0000");
BTT_TFT_SERIAL.println("   M301 P24.6318 I2.4149 D62.8112");
BTT_TFT_SERIAL.println("   M304 P153.4584 I30.2084 D519.7126");
BTT_TFT_SERIAL.println("   M413 S0 ; OFF");
BTT_TFT_SERIAL.println("   M207 S60.0000 W13.0000 F2700.0000 Z0.0000");
BTT_TFT_SERIAL.println("   M208 S0.0000 W0.0000 F480.0000");
BTT_TFT_SERIAL.println("   M209 S0");
BTT_TFT_SERIAL.println("   M851 X-41.0000 Y-13.0000 Z-1.0000 ; (mm)");
BTT_TFT_SERIAL.println("   M906 X650 Y650 Z400");
BTT_TFT_SERIAL.println("   M906 I1 Z400");
BTT_TFT_SERIAL.println("   M906 T0 E650");
BTT_TFT_SERIAL.println("");
BTT_TFT_SERIAL.println("   M569 S1 X Y Z");
BTT_TFT_SERIAL.println("   M569 S1 I1 Z");
BTT_TFT_SERIAL.println("   M900 K1.3000");
BTT_TFT_SERIAL.println("   M603 L0.0000 U100.0000 ; (mm)");
BTT_TFT_SERIAL.println("   M412 S1 ; Sensor ON");

/*
Recv:   G21 ; (mm)
Recv:   M149 C ; Units in Celsius
Recv:   M200 S0 D1.7500
Recv:   M92 X160.0000 Y160.0000 Z800.0000 E411.8000
Recv:   M203 X200.0000 Y200.0000 Z18.0000 E25.0000
Recv:   M201 X3000.0000 Y3000.0000 Z600.0000 E40000.0000
Recv:   M204 P1500.0000 R5000.0000 T3000.0000
Recv:   M205 B20000.0000 S0.0000 T0.0000 J0.0130
Recv:   M206 X0.0000 Y0.0000 Z0.0000
Recv:   M420 S0 Z10.0000 ; Leveling OFF
Recv:   M422 S1 X15.0000 Y200.0000
Recv:   M422 S2 X376.0000 Y200.0000
Recv:   M301 P24.6318 I2.4149 D62.8112
Recv:   M304 P153.4584 I30.2084 D519.7126
Recv:   M413 S0 ; OFF
Recv:   M207 S60.0000 W13.0000 F2700.0000 Z0.0000
Recv:   M208 S0.0000 W0.0000 F480.0000
Recv:   M209 S0
Recv:   M851 X-41.0000 Y-13.0000 Z-1.0000 ; (mm)
Recv:   M906 X650 Y650 Z400
Recv:   M906 I1 Z400
Recv:   M906 T0 E650
Recv: 
Recv:   M569 S1 X Y Z
Recv:   M569 S1 I1 Z
Recv:   M900 K1.3000
Recv:   M603 L0.0000 U100.0000 ; (mm)
Recv:   M412 S1 ; Sensor ON
Recv: ok
    */
  }

// dummy report of M503
// M503 
  else if(strcmp(message,"M503")==0){

BTT_SERIAL_ECHOLN("; Linear Units:");
BTT_SERIAL_ECHOLN("  G21 ; (mm)");


BTT_SERIAL_ECHOLN("; Temperature Units:");
BTT_SERIAL_ECHOLN("  M149 C ; Units in Celsius");

BTT_SERIAL_ECHOLN("; Filament settings (Disabled):");
BTT_SERIAL_ECHOLN("  M200 S0 D1.7500");

BTT_SERIAL_ECHOLN("; Steps per unit:");
BTT_SERIAL_ECHOLN("  M92 X160.0000 Y160.0000 Z800.0000 E411.8000");

BTT_SERIAL_ECHOLN("; Max feedrates (units/s):");
BTT_SERIAL_ECHOLN("  M203 X200.0000 Y200.0000 Z18.0000 E25.0000");

BTT_SERIAL_ECHOLN("; Max Acceleration (units/s2):");
BTT_SERIAL_ECHOLN("  M201 X3000.0000 Y3000.0000 Z600.0000 E40000.0000");

BTT_SERIAL_ECHOLN("; Acceleration (units/s2) (P<print-accel> R<retract-accel> T<travel-accel>):");
BTT_SERIAL_ECHOLN("  M204 P1500.0000 R5000.0000 T3000.0000");

BTT_SERIAL_ECHOLN("; Advanced (B<min_segment_time_us> S<min_feedrate> T<min_travel_feedrate> J<junc_dev>):");
BTT_SERIAL_ECHOLN("  M205 B20000.0000 S0.0000 T0.0000 J0.0130");

BTT_SERIAL_ECHOLN("; Home offset:");
BTT_SERIAL_ECHOLN("  M206 X0.0000 Y0.0000 Z0.0000");

BTT_SERIAL_ECHOLN("; Unified Bed Leveling:");
BTT_SERIAL_ECHOLN("  M420 S0 Z10.0000 ; Leveling OFF");
BTT_SERIAL_EOL(); 

BTT_TFT_SERIAL.println("Unified Bed Leveling System v1.01 inactive");
BTT_SERIAL_ECHOLN("Active Mesh Slot -1");
BTT_SERIAL_ECHOLN("EEPROM can hold 8 meshes.");
BTT_SERIAL_EOL(); 

BTT_SERIAL_ECHOLN("; Z Auto-Align:");
BTT_SERIAL_ECHOLN("  M422 S1 X15.0000 Y200.0000");
BTT_SERIAL_ECHOLN("  M422 S2 X376.0000 Y200.0000");

BTT_SERIAL_ECHOLN("; Hotend PID:");
BTT_SERIAL_ECHOLN("  M301 P24.6318 I2.4149 D62.8112");

BTT_SERIAL_ECHOLN("; Bed PID:");
BTT_SERIAL_ECHOLN("  M304 P153.4584 I30.2084 D519.7126");

BTT_SERIAL_ECHOLN("; Power-loss recovery:");
BTT_SERIAL_ECHOLN("  M413 S0 ; OFF");

BTT_SERIAL_ECHOLN("; Retract (S<length> F<feedrate> Z<lift>):");
BTT_SERIAL_ECHOLN("  M207 S60.0000 W13.0000 F2700.0000 Z0.0000");

BTT_SERIAL_ECHOLN("; Recover (S<length> F<feedrate>):");
BTT_SERIAL_ECHOLN("  M208 S0.0000 W0.0000 F480.0000");

BTT_SERIAL_ECHOLN("; Auto-Retract (S<enable>):");
BTT_SERIAL_ECHOLN("  M209 S0");

BTT_SERIAL_ECHOLN("; Z-Probe Offset:");
BTT_SERIAL_ECHOLN("  M851 X-41.0000 Y-13.0000 Z-1.0000 ; (mm)");

BTT_SERIAL_ECHOLN("; Stepper driver current:");
BTT_SERIAL_ECHOLN("  M906 X650 Y650 Z400");
BTT_SERIAL_ECHOLN("  M906 I1 Z400");
BTT_SERIAL_ECHOLN("  M906 T0 E650");
BTT_SERIAL_EOL(); 

BTT_SERIAL_ECHOLN("; Driver stepping mode:");
BTT_SERIAL_ECHOLN("  M569 S1 X Y Z");
BTT_SERIAL_ECHOLN("  M569 S1 I1 Z");

BTT_SERIAL_ECHOLN("; Linear Advance:");
BTT_SERIAL_ECHOLN("  M900 K1.3000");

BTT_SERIAL_ECHOLN("; Filament load/unload:");
BTT_SERIAL_ECHOLN("  M603 L0.0000 U100.0000 ; (mm)");

BTT_SERIAL_ECHOLN("; Filament runout sensor:");
BTT_SERIAL_ECHOLN("  M412 S1 ; Sensor ON");

/*
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
  }
  
  btt_tft_gcode_request::ok_send();
  return false;

}

#ifdef test

void BTTgcode::report_echo_start(const bool forReplay) { if (!forReplay) BTT_SERIAL_ECHO_START();}

void BTTgcode::report_heading(const bool forReplay, FSTR_P const fstr, const bool eol/*=true*/) {
  if (forReplay) return;
  if (fstr) {
    BTT_SERIAL_ECHO_START();
    BTT_SERIAL_ECHOPGM("; ");
    BTT_SERIAL_ECHOF(fstr);
  }
  if (eol) { BTT_SERIAL_CHAR(':'); BTT_SERIAL_EOL(); }
}


void BTTgcode::say_units() {
  BTT_SERIAL_ECHOLNPGM_P(
    TERN_(INCH_MODE_SUPPORT, parser.linear_unit_factor != 1.0 ? PSTR(" (in)") :)
    PSTR(" (mm)")
  );
}
#if ENABLED(TEMPERATURE_UNITS_SUPPORT)

#include "../gcode.h"


void BTTgcode::M149_report(const bool forReplay/*=true*/) {
  report_heading_etc(forReplay, F(STR_TEMPERATURE_UNITS));
  SERIAL_ECHOPGM("  M149 ", AS_CHAR(parser.temp_units_code()), " ; Units in ");
  SERIAL_ECHOLNF(parser.temp_units_name());
}

#endif // TEMPERATURE_UNITS_SUPPORT
#if DISABLED(NO_VOLUMETRICS)


  void BTTgcode::M200_report(const bool forReplay/*=true*/) {
    if (!forReplay) {
      BTTgcode::report_heading(forReplay, F(STR_FILAMENT_SETTINGS), false);
      if (!parser.volumetric_enabled) BTT_SERIAL_ECHOPGM(" (Disabled):");
      BTT_SERIAL_EOL();
      BTTgcode::report_echo_start(forReplay);
    }
  }
#endif // NO_VOLUMETRICS

#if DISABLED(DISABLE_M503)
//#if DISABLED(1)
  #define BTT_CONFIG_ECHO_START()       BTTgcode::report_echo_start(forReplay)
  #define BTT_CONFIG_ECHO_MSG(V...)     do{ BTT_CONFIG_ECHO_START(); BTT_SERIAL_ECHOLNPGM(V); }while(0)
  #define BTT_CONFIG_ECHO_MSG_P(V...)   do{ BTT_CONFIG_ECHO_START(); BTT_SERIAL_ECHOLNPGM_P(V); }while(0)
  #define BTT_CONFIG_ECHO_HEADING(STR)  BTTgcode::report_heading(forReplay, F(STR))

  //void BTTgcode::M92_report(const bool echo=true, const int8_t e=-1);

  /**
   * M503 - Report current settings in RAM
   *
   * Unless specifically disabled, M503 is available even without EEPROM
   */
 
  
  void BTTgcode::report(const bool forReplay) {
    //
    // Announce current units, in case inches are being displayed
    //
    BTT_CONFIG_ECHO_HEADING("Linear Units");
    BTT_CONFIG_ECHO_START();
    #if ENABLED(INCH_MODE_SUPPORT)
      BTT_SERIAL_ECHOPGM("  G2", AS_DIGIT(parser.linear_unit_factor == 1.0), " ;");
    #else
      BTT_SERIAL_ECHOPGM("  G21 ;");
    #endif
    BTTgcode::say_units(); // " (in/mm)"

    //
    // M149 Temperature units
    //
    #if ENABLED(TEMPERATURE_UNITS_SUPPORT)
      BTTgcode.M149_report(forReplay);
    #else
      BTT_CONFIG_ECHO_HEADING(STR_TEMPERATURE_UNITS);
      BTT_CONFIG_ECHO_MSG("  M149 C ; Units in Celsius");
    #endif

    //
    // M200 Volumetric Extrusion
    //
    IF_DISABLED(NO_VOLUMETRICS, BTTgcode::M200_report(forReplay));

    //
    // M92 Steps per Unit
    //
    BTTgcode::M92_report(forReplay);

    //
    // M203 Maximum feedrates (units/s)
    //
    BTTgcode::M203_report(forReplay);

    //
    // M201 Maximum Acceleration (units/s2)
    //
    BTTgcode::M201_report(forReplay);

    //
    // M204 Acceleration (units/s2)
    //
    BTTgcode::M204_report(forReplay);

    //
    // M205 "Advanced" Settings
    //
    BTTgcode::M205_report(forReplay);

    //
    // M206 Home Offset
    //
    TERN_(HAS_M206_COMMAND, BTTgcode::M206_report(forReplay));

    //
    // M218 Hotend offsets
    //
    TERN_(HAS_HOTEND_OFFSET, BTTgcode::M218_report(forReplay));

    //
    // Bed Leveling
    //
    #if HAS_LEVELING

      BTTgcode::M420_report(forReplay);

      #if ENABLED(MESH_BED_LEVELING)

        if (leveling_is_valid()) {
          LOOP_L_N(py, GRID_MAX_POINTS_Y) {
            LOOP_L_N(px, GRID_MAX_POINTS_X) {
              CONFIG_ECHO_START();
              SERIAL_ECHOPGM("  G29 S3 I", px, " J", py);
              SERIAL_ECHOLNPAIR_F_P(SP_Z_STR, LINEAR_UNIT(mbl.z_values[px][py]), 5);
            }
          }
          CONFIG_ECHO_START();
          SERIAL_ECHOLNPAIR_F("  G29 S4 Z", LINEAR_UNIT(mbl.z_offset), 5);
        }

      #elif ENABLED(AUTO_BED_LEVELING_UBL)

        if (!forReplay) {
          BTT_SERIAL_EOL();
          ubl.report_state();
          BTT_SERIAL_ECHO_MSG("Active Mesh Slot ", ubl.storage_slot);
          BTT_SERIAL_ECHO_MSG("EEPROM can hold ", calc_num_meshes(), " meshes.\n");
        }

       //ubl.report_current_mesh();   // This is too verbose for large meshes. A better (more terse)
                                                  // solution needs to be found.
      #elif ENABLED(AUTO_BED_LEVELING_BILINEAR)

        if (leveling_is_valid()) {
          LOOP_L_N(py, GRID_MAX_POINTS_Y) {
            LOOP_L_N(px, GRID_MAX_POINTS_X) {
              CONFIG_ECHO_START();
              SERIAL_ECHOPGM("  G29 W I", px, " J", py);
              SERIAL_ECHOLNPAIR_F_P(SP_Z_STR, LINEAR_UNIT(z_values[px][py]), 5);
            }
          }
        }

        // TODO: Create G-code for settings
        //#if ENABLED(X_AXIS_TWIST_COMPENSATION)
        //  CONFIG_ECHO_START();
        //  xatc.print_points();
        //#endif

      #endif

    #endif // HAS_LEVELING

    //
    // Editable Servo Angles
    //
    TERN_(EDITABLE_SERVO_ANGLES, BTTgcode.M281_report(forReplay));

    //
    // Kinematic Settings
    //
    TERN_(IS_KINEMATIC, BTTgcode.M665_report(forReplay));

    //
    // M666 Endstops Adjustment
    //
    #if EITHER(DELTA, HAS_EXTRA_ENDSTOPS)
      gcode.M666_report(forReplay);
    #endif

    //
    // Z Auto-Align
    //
    TERN_(Z_STEPPER_AUTO_ALIGN, gcode.M422_report(forReplay));

    //
    // LCD Preheat Settings
    //
    #if HAS_PREHEAT
      gcode.M145_report(forReplay);
    #endif

    //
    // PID
    //
    TERN_(PIDTEMP,        gcode.M301_report(forReplay));
    TERN_(PIDTEMPBED,     gcode.M304_report(forReplay));
    TERN_(PIDTEMPCHAMBER, gcode.M309_report(forReplay));

    #if HAS_USER_THERMISTORS
      LOOP_L_N(i, USER_THERMISTORS)
        thermalManager.M305_report(i, forReplay);
    #endif

    //
    // LCD Contrast
    //
    TERN_(HAS_LCD_CONTRAST, gcode.M250_report(forReplay));

    //
    // LCD Brightness
    //
    TERN_(HAS_LCD_BRIGHTNESS, gcode.M256_report(forReplay));

    //
    // Controller Fan
    //
    TERN_(CONTROLLER_FAN_EDITABLE, gcode.M710_report(forReplay));

    //
    // Power-Loss Recovery
    //
    TERN_(POWER_LOSS_RECOVERY, gcode.M413_report(forReplay));

    //
    // Firmware Retraction
    //
    #if ENABLED(FWRETRACT)
      gcode.M207_report(forReplay);
      gcode.M208_report(forReplay);
      TERN_(FWRETRACT_AUTORETRACT, gcode.M209_report(forReplay));
    #endif

    //
    // Probe Offset
    //
    TERN_(HAS_BED_PROBE, gcode.M851_report(forReplay));

    //
    // Bed Skew Correction
    //
    TERN_(SKEW_CORRECTION_GCODE, gcode.M852_report(forReplay));

    #if HAS_TRINAMIC_CONFIG
      //
      // TMC Stepper driver current
      //
      gcode.M906_report(forReplay);

      //
      // TMC Hybrid Threshold
      //
      TERN_(HYBRID_THRESHOLD, gcode.M913_report(forReplay));

      //
      // TMC Sensorless homing thresholds
      //
      TERN_(USE_SENSORLESS, gcode.M914_report(forReplay));
    #endif

    //
    // TMC stepping mode
    //
    TERN_(HAS_STEALTHCHOP, gcode.M569_report(forReplay));

    //
    // Linear Advance
    //
    TERN_(LIN_ADVANCE, gcode.M900_report(forReplay));

    //
    // Motor Current (SPI or PWM)
    //
    #if HAS_MOTOR_CURRENT_SPI || HAS_MOTOR_CURRENT_PWM
      gcode.M907_report(forReplay);
    #endif

    //
    // Advanced Pause filament load & unload lengths
    //
    TERN_(ADVANCED_PAUSE_FEATURE, gcode.M603_report(forReplay));

    //
    // Tool-changing Parameters
    //
    E_TERN_(gcode.M217_report(forReplay));

    //
    // Backlash Compensation
    //
    TERN_(BACKLASH_GCODE, gcode.M425_report(forReplay));

    //
    // Filament Runout Sensor
    //
    TERN_(HAS_FILAMENT_SENSOR, gcode.M412_report(forReplay));

    #if HAS_ETHERNET
      CONFIG_ECHO_HEADING("Ethernet");
      if (!forReplay) ETH0_report();
      CONFIG_ECHO_START(); SERIAL_ECHO_SP(2); MAC_report();
      CONFIG_ECHO_START(); SERIAL_ECHO_SP(2); gcode.M552_report();
      CONFIG_ECHO_START(); SERIAL_ECHO_SP(2); gcode.M553_report();
      CONFIG_ECHO_START(); SERIAL_ECHO_SP(2); gcode.M554_report();
    #endif

    TERN_(HAS_MULTI_LANGUAGE, gcode.M414_report(forReplay));
  }
#endif
#endif // !DISABLE_M503



#endif


