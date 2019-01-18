#pragma once

#if !defined(__SAM3X8E__)&&!defined(__AVR_ATmega1280__)&&!defined(__AVR_ATmega2560__)
  #error "Oops! Select 'Arduino Due or Mega' in 'Tools > Board.'"
#endif

/* CNC shield modifications:
FROM THE BOTTOM CUT THE 5V PIN THAT GOES TO ARDUINO!!!
On the top put jumper between 5V and 3V3 pins,
    jumper between D12 and A.STEP, jumper between D13 and A.DIR
*/

/* CNC shield 3D printer connections:
X,Y,Z steppers as normal
A stepper for E0 extruder
(X-)&(GND) - X limit
(Y-)&(GND) - Y limit
(Z-)&(GND) - Z limit
(Abort)&(GND) - Extruder thermistor (also require pullup resistor 4.7K between "Abort" and
                                      Vcc (now "5V" on the board but actual 3.3V because of jumper))
(Hold)&(GND) - Bed thermistor (also require pullup resistor 4.7K between "Hold" and
                                 Vcc (now "5V" on the board but actual 3.3V because of jumper))
(CoolEn) - 3.3v signal to controll extruder heater MOSFET
(Resume) - 3.3v signal to control heatbed MOSFET
(SDA) - 3.3v signal to controll extruder fan
(SCL) - 3.3v signal to controll extruder cooling fan
*/

/* CNC Shield pinout
"Name on the board": DUE pin
"Abort":    Analog pin 0 or Digital pin 54
"Hold":     Analog pin 1 or Digital pin 55
"Resume":   Analog pin 2 or Digital pin 56
"CoolEn":   Analog pin 3 or Digital pin 57
"SDA":      Analog pin 4 or Digital pin 58
"SCL":      Analog pin 5 or Digital pin 59
"E-STOP":   Reset pin
"RX":       Digital pin 0
"TX":       Digital pin 1
"X.STEP":   Digital pin 2
"Y.STEP":   Digital pin 3
"Z.STEP":   Digital pin 4
"X.DIR":    Digital pin 5
"Y.DIR":    Digital pin 6
"Z.DIR":    Digital pin 7
"EN":       Digital pin 8
"X+","X-":  Digital pin 9
"Y+","Y-":  Digital pin 10
"Z+","Z-":  Digital pin 11
"SpinEn":   Digital pin 12 -> will be connected to A.STEP with jumper
"SpinDir":  Digital pin 13 -> will be connected to A.DIR with jumper
*/

#define X_STEP_PIN          2
#define X_DIR_PIN           5
#define X_ENABLE_PIN        8
    
#define Y_STEP_PIN          3
#define Y_DIR_PIN           6
#define Y_ENABLE_PIN        8

#define Z_STEP_PIN          4
#define Z_DIR_PIN           7
#define Z_ENABLE_PIN        8

#define E0_STEP_PIN         12
#define E0_DIR_PIN          13
#define E0_ENABLE_PIN       8

#define X_MIN_PIN           9
#define Y_MIN_PIN           10
#define Z_MIN_PIN           11

#define FAN_PIN             54  // "Abort":  Analog pin 0, Digital pin 54

#define TEMP_0_PIN          4   // "SDA":    Analog pin 4, Digital pin 58

#if defined(E0_AUTO_FAN_PIN)
#undef E0_AUTO_FAN_PIN
#endif
#define E0_AUTO_FAN_PIN     56  // "Resume": Analog pin 2, Digital pin 56
#define HEATER_0_PIN        55  // "Hold":   Analog pin 1, Digital pin 55

#define TEMP_BED_PIN        5   // "SCL":    Analog pin 5, Digital pin 59
#define HEATER_BED_PIN      57  // "CoolEn": Analog pin 3, Digital pin 57

