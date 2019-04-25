//
// structured configuration; see #13752
//

// see https://github.com/p3p/pio-framework-arduino-lpc176x

// 8 ADC channels: P0_02, P0_03, P0_23, P0_24, P0_25, P0_26, P1_30, P1_31
 
// H/W PWM
// Hardware channel   Attached pins
// 1   P1_18, P2_00
// 2   P1_20, P2_01, P3_25
// 3   P1_21, P2_02, P3_26
// 4   P1_23, P2_03
// 5   P1_24, P2_04
// 6   P1_26, P2_05

//#define LED_PIN            P1_18   // Status indicator; h/w pwm channel #1
//#define LED2_PIN           P1_19   // wired to EXP1#3
//#define LED3_PIN           P1_20   // wired to EXP1#4; h/w pwm channel #2
//#define LED4_PIN           P1_21   // wired to EXP1#5; h/w pwm channel #3

//
// Servo pin
//
#undef SERVO0_PIN
#undef SERVO1_PIN
#undef SERVO2_PIN
#undef SERVO3_PIN
#define SERVO0_PIN         P4_28 // BLTouch control signal "BL/PWM"; s/w pwm
#define SERVO1_PIN         P3_25 // "SRV1"; h/w pwm channel #2
#define SERVO2_PIN         P3_26 // "SRV2"; h/w pwm channel #3
#define SERVO3_PIN         P1_23 // TODO;   h/w pwm channel #4

// Case light PWM
#define CASE_LIGHT_PIN     P0_15   // s/w pwm "PWM1"

// BLTouch trigger signal "BL/S"
# define BLTOUCH_PIN       P1_22   // input

//
// Temperature Sensors
//
#undef TEMP_BED_PIN
#undef TEMP_0_PIN
#undef TEMP_1_PIN
#undef TEMP_2_PIN
#define TEMP_BED_PIN        0   // A0 (TH1)
#define TEMP_CHAMBER_PIN    1   // A1 (TH2)
#define TEMP_0_PIN          4   // A4: heater1-> pt100_e3d -> P1_30 
#define TEMP_1_PIN          5   // A5: heater2-> pt100_e3d -> P1_31 

//
// Heaters / Fans PWM
//
//#define HEATER_BED_PIN     P2_05  // h/w pwm channel #6
//#define HEATER_0_PIN       P2_07  // s/w pwm
//#define HEATER_1_PIN       P2_06  // s/w pwm
//#define FAN_PIN            P2_04  // h/w pwm channel #5

//
// Chamber heater PWM
//
#define HEATER_CHAMBER_PIN    P0_17 // s/w pwm "PWM2"

