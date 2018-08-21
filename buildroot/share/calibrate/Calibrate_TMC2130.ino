/*
 * Author: Teemu Mäntykallio
 */

#include <TMC2130Stepper.h>

#define AXIS 4

#if ( AXIS == 1 ) // X1-Axis
  #define SET_PINS { 46, 48, 62, 41, 17}; // {STEP, DIR, EN, CS, C17} > X1
  #define SET_PINPORT REG_PIOC_SODR // > X1
  //#define SET_SVAL { 900, 32, 28, 4, 24, 2, 0 }; // > X1
  #define SET_SVAL { 925, 32, 29, 3, 16, 1, -1 }; // > X1
#endif
#if ( AXIS == 2 ) // Y1-Axis
  #define SET_PINS { 60, 61, 56, 45, 3};  // {STEP, DIR, EN, CS, A3}  > Y1
  #define SET_PINPORT REG_PIOA_SODR // > Y1
  #define SET_SVAL { 900, 32, 28, 4, 24, 2, 1 }; // > Y
#endif
#if ( AXIS == 3 ) // Y2-Axis
  #define SET_PINS { 36, 34, 30, 43, 4};  // {STEP, DIR, EN, CS, C4}  > Y2 (E1)
  #define SET_PINPORT REG_PIOC_SODR // > Y2
  #define SET_SVAL { 925, 32, 29, 3, 16, 1, -1 }; // > Y
#endif
#if ( AXIS == 4 )// Z-Axis
  #define SET_PINS { 54, 55, 38, 32, 16}; // {STEP, DIR, EN, CS, A16} > Z1
  #define SET_PINPORT REG_PIOA_SODR // > Z1
  #define SET_SVAL { 925, 32, 29, 3, 16, 1, -1 }; // > Z
#endif
#if ( AXIS == 5 ) // Z-Axis
  #define SET_PINS { 26, 28, 24, 47, 1};  // {STEP, DIR, EN, CS, D1}  > Z2 (E2)
  #define SET_PINPORT REG_PIOD_SODR // > Z2
  #define SET_SVAL { 700, 32, 21, 4, 24, 1, 0 }; // > Z
#endif
#if ( AXIS == 6 ) // E-Axis
  #define SET_PINS { 40, 42, 65, 44, 8};  // {STEP, DIR, EN, CS, C8}  > E0
  #define SET_PINPORT REG_PIOC_SODR // > E0
  #define SET_SVAL { 500, 16, 15, 2, 16, 1, -2 }; // > E0
#endif

//SET_SVAL { Current, Microstepping, CS_RUN, off_time, blank_time, hysteresis_start, hysteresis_end };


// X   h_end: -3 h_start: 1 off_time: 3 blank_time:24 I: 900 Stepping: 32 Interpolation: 1 Mode: 0
// Y   h_end: -1 h_start: 1 off_time: 4 blank_time:24 I: 900 Stepping: 32 Interpolation: 0 Mode: 0
// Y2  h_end: -2 h_start: 2 off_time: 3 blank_time:24 I: 900 Stepping: 32 Interpolation: 0 Mode: 0
// Z   h_end: -3 h_start: 1 off_time: 4 blank_time:24 I: 700 Stepping: 32 Interpolation: 0 Mode: 0
// Z2  h_end: -3 h_start: 1 off_time: 4 blank_time:24 I: 700 Stepping: 32 Interpolation: 0 Mode: 0
// E0  h_end: 1 h_start: 1 off_time: 4 blank_time:36 I: 650 Stepping: 16 Interpolation: 0 Mode: 0

const uint32_t axpins[] = SET_PINS;
const int32_t axval[] = SET_SVAL;

#define EN_PIN       axpins[2]  // Nano v3:   16 Mega:    38  //enable (CFG6)
#define DIR_PIN      axpins[1]  //            19          55  //direction
#define STEP_PIN     axpins[0]  //            18          54  //step
#define CS_PIN       axpins[3]  //            17          64  //chip select
#define STEP_PORT    SET_PINPORT           // Register to match with STEP_PIN >> PIN46; 99; PC17
#define STEP_BIT     axpins[4]             // Bit in register to match STEP_PI

TMC2130Stepper driver = TMC2130Stepper(CS_PIN);

// You can define starting values here:
struct {
    uint8_t blank_time = (uint8_t)axval[4];         // [16, 24, 36, 54]
    uint8_t off_time = (uint8_t)axval[3];           // [1..15]
    uint8_t hysteresis_start = (uint8_t)axval[5];   // [0..7]  >> [1..8]    ;Hend-1
    int8_t hysteresis_end = (int8_t)axval[6];       // [0..15] >> [-3..12]  ;Hend+3
    uint16_t current = (uint16_t)axval[0];
    uint16_t mspepping = (uint16_t)axval[1];
    uint8_t cs_current = (uint8_t)axval[2];
    bool chopmode = false;
    bool interpolate = true;
    uint8_t stealth_amplitude = 255;
    uint8_t stealth_gradient = 2;
    uint8_t stealth_freq = 0;
    bool stealth_autoscale = 1;
} config;

void initPins();

ISR(TIMER1_COMPA_vect){
    STEP_PORT |= 1 << STEP_BIT;
    STEP_PORT &= ~(1 << STEP_BIT);
}

void reportCurrentSettings() {
    Serial.print(" h_start: ");
    Serial.print(config.hysteresis_start);
    Serial.print(" h_end: ");
    Serial.print(config.hysteresis_end);
    Serial.print(" off_time: ");
    Serial.print(config.off_time);
    Serial.print(" blank_time:");
    Serial.print(config.blank_time);
    Serial.print(" stealth_amplitude: ");
    Serial.print(config.stealth_amplitude);
    Serial.print(" stealth_gradient: ");
    Serial.print(config.stealth_gradient);
    Serial.print(" stealth_freq: ");
    Serial.print(config.stealth_freq);
    Serial.print(" stealth_autoscale: ");
    Serial.print(config.stealth_autoscale);
    Serial.print(" Interpolation: ");
    Serial.print(config.interpolate);
    Serial.print(" Mode: ");
    Serial.println(config.chopmode);
}

void setup() {
    initPins();
    Serial.begin(250000);
    Serial.println(F("Starting calibration of TMC spreadCycle parameters."));
    for (uint8_t i = 0; i < 30; i++) { Serial.print('.'); delay(100); }
    while(1) {
        Serial.println(F("\nStart? (yes)"));
        while(!Serial.available());
        String yn = Serial.readStringUntil('\n');
        Serial.println(yn);
        if (yn == "yes") {
            break;
        } else {
            Serial.println(F("...."));
        };
    }


    Serial.print(F("\nTesting connection..."));
    uint8_t result = driver.test_connection();
    if (result) {
        Serial.println(F("failed!"));
        Serial.print(F("Likely cause: "));
        switch(result) {
            case 1: Serial.println(F("loose connection")); break;
            case 2: Serial.println(F("Likely cause: no power")); break;
        }
        Serial.println(F("Fix the problem and reset board."));
        abort();
    }
    Serial.println(F("OK"));

    driver.push();
    driver.rms_current(config.current);
    driver.microsteps(config.mspepping);
    driver.stealthChop(config.chopmode);
    driver.interpolate(config.interpolate);
    driver.hold_current(config.cs_current);
    driver.run_current(config.cs_current);
    driver.blank_time(config.blank_time);
    driver.off_time(config.off_time);
    driver.hysteresis_end(config.hysteresis_end);
    driver.hysteresis_start(config.hysteresis_start);
    driver.stealth_freq(config.stealth_freq);
    driver.stealth_amplitude(config.stealth_amplitude);
    driver.stealth_gradient(config.stealth_gradient);
    driver.stealth_autoscale(config.stealth_autoscale);
    driver.sync_phases(3);
    
    Serial.print(F("Setings:"));
    reportCurrentSettings();
    
    Serial.print(F("Effective hysteresis = "));
    Serial.print(config.hysteresis_end);
    Serial.print(F(" + "));
    Serial.print(config.hysteresis_start);
    Serial.print(F(" = "));
    Serial.println(config.hysteresis_end + config.hysteresis_start);


    Serial.println(F("\nTuning:"));
    Serial.println(F("  hysteresis_start:  1 +/-"));
    Serial.println(F("  hysteresis_end:    2 +/-"));
    Serial.println(F("  off_time:          3 +/-"));
    Serial.println(F("  blank_time:        4 +/-"));
    Serial.println(F("  stealth_amplitude: 5 +/-"));
    Serial.println(F("  stealth_gradient:  6 +/-"));
    Serial.println(F("  stealth_freq:      7 +/-"));
    Serial.println(F("  stealth_autoscale: 8"));
    Serial.println(F("  interpolate:       9"));
    Serial.println(F("  chopmode:          0"));
    Serial.println(F("  Quit:              exit"));

    digitalWrite(EN_PIN, LOW);
    while (driver.cur_a() < 240) { // Use cur_b if measuring from B coil
        digitalWrite(STEP_PIN, HIGH);
        digitalWrite(STEP_PIN, LOW);
        delay(3);
    }

   SPI.begin();
   while(!Serial);
   Serial.println("Start...");
    pinMode(CS_PIN, OUTPUT);
    digitalWrite(CS_PIN, HIGH);
    driver.begin();             // Initiate pins and registeries
  
    while(1) {
        if (Serial.available() > 0) {
            //uint8_t c = Serial.read();
            String c = Serial.readStringUntil('\n');
            if (c == "1+") {
                if (config.hysteresis_start == 8) Serial.println(F("HSTRT_MAX=8"));
                else {
                    config.hysteresis_start++;
                    reportCurrentSettings();
                    driver.hysterisis_start(config.hysteresis_start);
                }
            }
            else if (c == "1-") {
                if (config.hysteresis_start == 1) Serial.println(F("HSTRT_MIN=1"));
                else {
                    config.hysteresis_start--;
                    reportCurrentSettings();
                    driver.hysterisis_start(config.hysteresis_start);
                }
            }
            else if (c == "2+") {
                if (config.hysteresis_end == 12) Serial.println(F("HEND_MAX=12"));
                else {
                    config.hysteresis_end++;
                    reportCurrentSettings();
                    driver.hysterisis_end(config.hysteresis_end);
                }
            } else if (c == "2-") {
                if (config.hysteresis_end == -3) Serial.println(F("HEND_MIN=-3"));
                else {
                    config.hysteresis_end--;
                    reportCurrentSettings();
                    driver.hysterisis_end(config.hysteresis_end);
                }
            }
             else if (c == "3+") {
                if (config.off_time == 15) Serial.println(F("TOFF_MAX=15"));
                else {
                    config.off_time++;
                    reportCurrentSettings();
                    driver.off_time(config.off_time);
                }
            }
            else if (c == "3-") {
                if (config.off_time == 1) Serial.println(F("TOFF_MIN=1"));
                else {
                    config.off_time--;
                    reportCurrentSettings();
                    driver.off_time(config.off_time);
                }
            }
             else if (c == "4+") {
                if (config.blank_time == 54) Serial.println(F("BLANK_MAX=54"));
                else if (config.blank_time == 36) {
                    config.blank_time = 54;
                    reportCurrentSettings();
                    driver.blank_time(config.blank_time);
                  }
                  else if (config.blank_time == 24) {
                    config.blank_time = 36;
                    reportCurrentSettings();
                    driver.blank_time(config.blank_time);
                  }
                  else if (config.blank_time == 16) {
                    config.blank_time = 24;
                    reportCurrentSettings();
                    driver.blank_time(config.blank_time);
                  }
                else Serial.println(F("ERROR"));
            }  
            else if (c == "4-") {
                if (config.blank_time == 16) Serial.println(F("BLANK_MIN=16"));
                else if (config.blank_time == 24) {
                    config.blank_time = 16;
                    reportCurrentSettings();
                    driver.blank_time(config.blank_time);
                  }
                  else if (config.blank_time == 36) {
                    config.blank_time = 24;
                    reportCurrentSettings();
                    driver.blank_time(config.blank_time);
                  }
                  else if (config.blank_time == 54) {
                    config.blank_time = 36;
                    reportCurrentSettings();
                    driver.blank_time(config.blank_time);
                  }
                else Serial.println(F("ERROR"));
            }
            else if (c == "5+") {
                if (config.stealth_amplitude == 255) Serial.println(F("Max stealth_amplitude=255"));
                else {
                    config.stealth_amplitude = config.stealth_amplitude + 5;
                    reportCurrentSettings();
                    driver.stealth_amplitude(config.stealth_amplitude);
                  }
            } 
            else if (c == "5-") {
                if (config.stealth_amplitude == 50) Serial.println(F("Min stealth_amplitude=50"));
                else {
                    config.stealth_amplitude = config.stealth_amplitude - 5;
                    reportCurrentSettings();
                    driver.stealth_amplitude(config.stealth_amplitude);
                  }
            } 
            else if (c == "6+") {
                if (config.stealth_gradient == 255) Serial.println(F("Max stealth_gradient=255"));
                else {
                    config.stealth_gradient = config.stealth_gradient + 1;
                    reportCurrentSettings();
                    driver.stealth_gradient(config.stealth_gradient);
                  }
            } 
            else if (c == "6-") {
                if (config.stealth_gradient == 0) Serial.println(F("Min stealth_gradient=0"));
                else {
                    config.stealth_gradient = config.stealth_gradient - 1;
                    reportCurrentSettings();
                    driver.stealth_gradient(config.stealth_gradient);
                  }
            } 
            else if (c == "7+") {
                if (config.stealth_freq == 3) Serial.println(F("Max sstealth_freq=3"));
                else {
                    config.stealth_freq = config.stealth_freq + 1;
                    reportCurrentSettings();
                    driver.stealth_freq(config.stealth_freq);
                  }
            } 
            else if (c == "7-") {
                if (config.stealth_freq == 0) Serial.println(F("Min stealth_freq=0"));
                else {
                    config.stealth_freq = config.stealth_freq - 1;
                    reportCurrentSettings();
                    driver.stealth_freq(config.stealth_freq);
                  }
            }
            else if (c == "8") {
                if (config.stealth_autoscale) {
                    config.stealth_autoscale = false;
                    reportCurrentSettings();
                    driver.stealth_autoscale(config.stealth_autoscale);
                   }
                else if (!config.stealth_autoscale) {
                    config.stealth_autoscale = true;
                    reportCurrentSettings();
                    driver.stealth_autoscale(config.stealth_autoscale);
                  }
            }       
            else if (c == "9") {
                if (config.interpolate) {
                    config.interpolate = false;
                    reportCurrentSettings();
                    driver.interpolate(config.interpolate);
                   }
                else if (!config.interpolate) {
                    config.interpolate = true;
                    reportCurrentSettings();
                    driver.interpolate(config.interpolate);
                  }
            }             
            else if (c == "0") {
                if (config.chopmode) {
                    config.chopmode = false;
                    reportCurrentSettings();
                    driver.stealthChop(config.chopmode);
                   }
                else if (!config.chopmode) {
                    config.chopmode = true;
                    reportCurrentSettings();
                    driver.stealthChop(config.chopmode);
                  }
            }
            else if (c == "exit") break;
        }
    }

    Serial.println(F("done..."));
    digitalWrite(EN_PIN, HIGH);
}

void initPins() {
    pinMode(EN_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);
    pinMode(STEP_PIN, OUTPUT);
    pinMode(CS_PIN, OUTPUT);
    digitalWrite(EN_PIN, HIGH); //deactivate driver (LOW active)
    digitalWrite(DIR_PIN, LOW); //LOW or HIGH
    digitalWrite(STEP_PIN, LOW);
    digitalWrite(CS_PIN, HIGH);
    SPI.begin();
    pinMode(MISO, INPUT_PULLUP);
}

void loop() {

}
