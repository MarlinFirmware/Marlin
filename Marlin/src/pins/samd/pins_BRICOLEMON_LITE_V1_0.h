/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * BRICOLEMON LITE Board. Based on atsamd51 (AGCM4), bootloader and credits by ADAFRUIT. 
 * This board its a 3.3V LOGIC Board, following the ADAFRUIT example, all of the board is open source.
 * Schematic: Refer to the Bricolemon
 * 3DSTEP: https://github.com/bricogeek/bricolemon/blob/master/Documentacion/Bricolemon%20Lite/LC_BG_002_PCB_V1I4.step
 * PinDemux: https://github.com/bricogeek/bricolemon/blob/master/Documentacion/Bricolemon/PinDEMUX.xlsx 
 * 
 * NOTE: We need the Serial port on the -1 to make it work!!. Remember to change it on configuration.h #define SERIAL_PORT -1
 */





#if NOT_TARGET(ARDUINO_GRAND_CENTRAL_M4)
  #error "Oops! Select 'Adafruit Grand Central M4' in 'Tools > Board.'"
#endif


#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "BRICOLEMON LITE V1.0, Lemoncrest & BricoGeek colaboration."
#endif

/*
* EEPROM EMULATION: Work with some bugs already, but the board have solder an I2C EEPROM memory. 
*/
//#define FLASH_EEPROM_EMULATION
#define I2C_EEPROM                                // EEPROM on I2C-0
#define MARLIN_EEPROM_SIZE 0x70000                 // 512K (CAT24C512)


//This its another option to emulate an EEPROM, but its more efficient to dont loose the data the first One.
//#define SDCARD_EEPROM_EMULATION


//
// BLTOUCH PIN: This pin is the signal pin for the BLTOUCH sensor.
//

#define SERVO0_PIN         33 // Pin for the pwm of the bltouch



/************************************/
/******    Limit Switches     ******/
/************************************/

//Section X
#ifdef USE_XMIN_PLUG
#define X_MIN_PIN                             10
#endif

#ifdef USE_XMAX_PLUG
#define X_MAX_PIN                             10
#endif

//Section Y
#ifdef USE_YMIN_PLUG
#define Y_MIN_PIN                             11
#endif

#ifdef USE_YMAX_PLUG
#define Y_MAX_PIN                             11
#endif

//Section Z

#ifdef USE_ZMIN_PLUG
#define Z_MIN_PIN                             12
#endif

#ifdef USE_ZMAX_PLUG
#define Z_MAX_PIN                             12
#endif


//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                     12
#endif











/************************************/
/******    Steppers TMC2209    ******/
/************************************/


//Stepper X axis
#define X_STEP_PIN                             3  
#define X_DIR_PIN                             22  
#define X_ENABLE_PIN                          26


//Stepper Y axis
#define Y_STEP_PIN                             4  
#define Y_DIR_PIN                             23  
#define Y_ENABLE_PIN                          27  


//Stepper Z axis
#define Z_STEP_PIN                            5
#define Z_DIR_PIN                             24
#define Z_ENABLE_PIN                          28  


//Stepper Extruder
#define E0_STEP_PIN                            2
#define E0_DIR_PIN                            25
#define E0_ENABLE_PIN                         29

// Extruder runout filament, in case you want to use this pin for any other purpose, its up to you. Its a normal GPIO that could be configured as I/O.
// A example could be, filament runout, switch to detect any kind of behaivour, Power supply pin .... etc.
#define FIL_RUNOUT_PIN                        32



/*
* Address for the UART Configuration of the TMC2209. This could be commet with "//" if you configure this on the configuration_adv.h file. 
* If you have configured in both files, configuration_adv.h and pins_BRICOLEMON_LITE_V1_0.h, you will receive a warning by the compiler.
* For test the TMC2209 Steppers active the TMC_DEBUG on the configuration_adv.h and, with voltage on the steppers test the M122 command. 
*/

#define  X_SLAVE_ADDRESS 0b00
#define  Y_SLAVE_ADDRESS 0b01
#define  Z_SLAVE_ADDRESS 0b10
#define E0_SLAVE_ADDRESS 0b11

#if HAS_TMC_UART
  /**
   * TMC2208/TMC2209 stepper drivers
   *  It seems to work perfectly fine on Software Serial, if an advance user want to test, you could use the Serial1 and Serial 2 of the Samd51. Be carefull with the Sercom configurations.
   */

  //#define X_HARDWARE_SERIAL  Serial1
  //#define Y_HARDWARE_SERIAL  Serial1
  //#define Z_HARDWARE_SERIAL  Serial1
  //#define E0_HARDWARE_SERIAL Serial1

  
  // This is the stable default value after testing, but, higher UART rates could be configured, remeber to test the Steppers with the M122 command to check if everything works.
  #define TMC_BAUD_RATE 250000
  //
  // Software serial
  //


    #define X_SERIAL_TX_PIN   0
    #define X_SERIAL_RX_PIN   1

    #define Y_SERIAL_TX_PIN   0
    #define Y_SERIAL_RX_PIN   1

    #define Z_SERIAL_TX_PIN   0
    #define Z_SERIAL_RX_PIN   1

    #define E0_SERIAL_TX_PIN  0
    #define E0_SERIAL_RX_PIN  1

#endif



/************************************/
/******      Extruder/BED      ******/
/************************************/

// Temperature Sensors
#define TEMP_0_PIN                            1
#define TEMP_BED_PIN                          3

// Heaters / Fans
#define HEATER_0_PIN                           6
#define HEATER_BED_PIN                         7

//Fan Pins (By default with Marlin, one will be the layer fan and other the barrel fan)
#define FAN_PIN                                8 
#define FAN1_PIN                               9















/************************************/
/******  LCD and Controllers   ******/
/************************************/
/*

- Visual pinout of the Bricolemon Expansion connectors.


                EXP1
               --------
        VCC   | .    . | GND
        LCD7  | .    . | LCDD6
        LCD5  | .    .   LCDD4
        LCDRS | .    . | LCDDE
  BTN_ENCODER | .    . | BEEPER
               --------

                EXP2
               --------
        KILL  | .    . | GND
        RESET | .    . | SD_DETECT
        MOSI  | .    .   BTN_EN1
        SS    | .    . | BTN_EN2
        SCLK  | .    . | MISO
               --------

- This extra connector its for connect a serial displays like the TFT offer by MKS.
                MKS TFT
               --------
        RX0   | .    . | NC
        TX0   | .    .   NC
        GND   | .    .   GND
        5B    | .    . | 5V
               --------



- Especial configuration to adapt the EXP1 to the EXP3. This its done to work with the Ender Displays. 

                Connect the EXP1 to the EXP3 if you configure the CR10_STOCKDISPLAY (~ 2475 of configuration.h) display, as this configuration will be activated.
               --------
        VCC   | .    . | GND
        LCDDE | .    . | LCDRS
        LCDD4 | .    .   BTN_EN2
        RESET | .    . | BTN_EN1
  BTN_ENCODER | .    . | BEEPER
               --------



- Digital pinout reference of the Bricolemon for advance users/configurations.

                EXP1
               --------
        VCC   | .    . | GND
        D39   | .    . | D38
        D37   | .    .   D36
        D34   | .    . | D35
        D40   | .    . | D41
               --------


               
                EXP2
               --------
        D49   | .    . | GND
        RST   | .    . | D44
        D51   | .    .   D42
        D53   | .    . | D43
        D52   | .    . | D50
               --------


- Pin table to a fast and easy view of the total.
    LCD---
      LCD4  36
      LCD5  37
      LCD6  38
      LCD7  39
      LCDRS 34
      LCDDE 35
    ENCODER---
      BTN_EN1     42
      BTN_EN2     43
      BTN_ENCODER 40
    SPI---
      MISO  50
      SS    53
      SCKL  52
      MOSI  51
    BEEPER
      BEEP  41
    SD-CARD---
      SD_DETECT 44
    KILL_PIN 49

*/


/************************************/
/***** Configurations Section  ******/
/************************************/

/*
* This sections starts with the pins_RAMPS_144.h as example, after if you need any new display, you could use normal dupons and connect it with with the scheme showed before.
* Tested:
*   - Ender 3 Old display (Character LCD)
*   - Ender 3 New Serial DWING Display
*   - Reprap Display
*   - Ender 5 New Serial Display
*   - Any Reprap character display like
*/


#if HAS_WIRED_LCD



  //
  // LCD Display output pins
  //

  #if (HAS_DWIN_E3V2 || IS_DWIN_MARLINUI)

  //We need tu undef the LCD_SERIAL_PORT defined by Creality on the conditionals_LCD.h ~lane 509, and put our Serial correctly.
  
  #ifndef LCD_SERIAL_PORT
  #define LCD_SERIAL_PORT 1
  #else
  #undef LCD_SERIAL_PORT
  #define LCD_SERIAL_PORT 1
  #endif

  // DWIN ENCODER LCD
  #define BTN_ENC                           40
  #define BTN_EN1                           35
  #define BTN_EN2                           34

  //#define LCD_LED_PIN                     PB2
  #ifndef BEEPER_PIN
    #define BEEPER_PIN                      41
    #undef SPEAKER
  #endif



  #elif ENABLED(REPRAPWORLD_GRAPHICAL_LCD)

    // TO TEST
    //#define LCD_PINS_RS                     49  // CS chip select /SS chip slave select
    //#define LCD_PINS_ENABLE                 51  // SID (MOSI)
    //#define LCD_PINS_D4                     52  // SCK (CLK) clock

  #elif BOTH(IS_NEWPANEL, PANEL_ONE)

    // TO TEST
    //#define LCD_PINS_RS                     40
    //#define LCD_PINS_ENABLE                 42
    //#define LCD_PINS_D4                     57  // Mega/Due:65 - AGCM4:57
    //#define LCD_PINS_D5                     58  // Mega/Due:66 - AGCM4:58
    //#define LCD_PINS_D6                     44
    //#define LCD_PINS_D7                     56  // Mega/Due:64 - AGCM4:56

  #else

    #if ENABLED(CR10_STOCKDISPLAY)

      // TO TEST
      #define LCD_PINS_RS       38
      #define LCD_PINS_ENABLE   39
      #define LCD_PINS_D4       37

      #if !IS_NEWPANEL
        // TO TEST
        //#define BEEPER_PIN                  37
      #endif

    #elif ENABLED(ZONESTAR_LCD)

      // TO TEST
      //#define LCD_PINS_RS                   56  // Mega/Due:64 - AGCM4:56
      //#define LCD_PINS_ENABLE               44
      //#define LCD_PINS_D4                   55  // Mega/Due:63 - AGCM4:55
      //#define LCD_PINS_D5                   40
      //#define LCD_PINS_D6                   42
      //#define LCD_PINS_D7                   57  // Mega/Due:65 - AGCM4:57

    #else

      #if EITHER(MKS_12864OLED, MKS_12864OLED_SSD1306)
        // TO TEST
        //#define LCD_PINS_DC                 25  // Set as output on init
        //#define LCD_PINS_RS                 27  // Pull low for 1s to init
        // DOGM SPI LCD Support
        //#define DOGLCD_CS                   16
        //#define DOGLCD_MOSI                 17
        //#define DOGLCD_SCK                  23
        //#define DOGLCD_A0          LCD_PINS_DC
      
      
//Definition to any standar Display
      #else
        #define LCD_PINS_RS                   34
        #define LCD_PINS_ENABLE               35
        #define LCD_PINS_D4                   36
        #define LCD_PINS_D5                   37
        #define LCD_PINS_D6                   38
      #endif

      #define LCD_PINS_D7                     39

      #if !IS_NEWPANEL
        #define BEEPER_PIN                    41
      #endif

    #endif

    #if !IS_NEWPANEL
      // Buttons attached to a shift register
      // Not wired yet
      //#define SHIFT_CLK_PIN                 38
      //#define SHIFT_LD_PIN                  42
      //#define SHIFT_OUT_PIN                 40
      //#define SHIFT_EN_PIN                  17
    #endif

  #endif

  //
  // LCD Display input pins
  //
  #if IS_NEWPANEL

    #if IS_RRD_SC

      #define BEEPER_PIN                      41

      #if ENABLED(CR10_STOCKDISPLAY)
        // TO TEST
         #define BTN_EN1         35
         #define BTN_EN2         36
        
//Definition to any standar Display
      #else
        #define BTN_EN1                       42
        #define BTN_EN2                       43
        #if ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)
          #define BTN_ENC_EN         LCD_PINS_D7  // Detect the presence of the encoder
        #endif
      #endif

      #define BTN_ENC                         40
      #ifndef SD_DETECT_PIN
        #define SD_DETECT_PIN                 44
      #endif
      #define KILL_PIN                        49

      #if ENABLED(BQ_LCD_SMART_CONTROLLER)
        //#define LCD_BACKLIGHT_PIN           39  // TO TEST
      #endif

    #elif ENABLED(REPRAPWORLD_GRAPHICAL_LCD)

      // TO TEST
      //#define BTN_EN1                       56  // Mega/Due:64 - AGCM4:56
      //#define BTN_EN2                       72  // Mega/Due:59 - AGCM4:72
      //#define BTN_ENC                       55
      //#define SD_DETECT_PIN                 42

    #elif ENABLED(LCD_I2C_PANELOLU2)

      // TO TEST
      //#define BTN_EN1                       47
      //#define BTN_EN2                       43
      //#define BTN_ENC                       32
      //#define LCD_SDSS                    SDSS
      //#define KILL_PIN                      41

    #elif ENABLED(LCD_I2C_VIKI)

      // TO TEST
      //#define BTN_EN1                       40  // https://files.panucatt.com/datasheets/viki_wiring_diagram.pdf explains 40/42.
      //#define BTN_EN2                       42
      //#define BTN_ENC                       -1

      //#define LCD_SDSS                    SDSS
      //#define SD_DETECT_PIN                 49

    #elif ANY(VIKI2, miniVIKI)

      // TO TEST
      //#define DOGLCD_CS                     45
      //#define DOGLCD_A0                     44
      //#define LCD_SCREEN_ROT_180

      //#define BEEPER_PIN                    33
      //#define STAT_LED_RED_PIN              32
      //#define STAT_LED_BLUE_PIN             35

      //#define BTN_EN1                       22
      //#define BTN_EN2                        7
      //#define BTN_ENC                       39

      //#define SD_DETECT_PIN                 -1  // Pin 49 for display SD interface, 72 for easy adapter board
      //#define KILL_PIN                      31

    #elif ENABLED(ELB_FULL_GRAPHIC_CONTROLLER)

      // TO TEST
      //#define DOGLCD_CS                     29
      //#define DOGLCD_A0                     27

      //#define BEEPER_PIN                    23
      //#define LCD_BACKLIGHT_PIN             33

      //#define BTN_EN1                       35
      //#define BTN_EN2                       37
      //#define BTN_ENC                       31

      //#define LCD_SDSS                    SDSS
      //#define SD_DETECT_PIN                 49
      //#define KILL_PIN                      41

    #elif EITHER(MKS_MINI_12864, FYSETC_MINI_12864)

      // TO TEST
      //#define BEEPER_PIN                    37
      //#define BTN_ENC                       35
      //#define SD_DETECT_PIN                 49

      //#ifndef KILL_PIN
      //  #define KILL_PIN         41
      //#endif

      #if ENABLED(MKS_MINI_12864)

        // TO TEST
        //#define DOGLCD_A0                   27
        //#define DOGLCD_CS                   25

        // GLCD features
        // Uncomment screen orientation
        //#define LCD_SCREEN_ROT_90
        //#define LCD_SCREEN_ROT_180
        //#define LCD_SCREEN_ROT_270

        // not connected to a pin
        //#define LCD_BACKLIGHT_PIN           57  // backlight LED on A11/D? (Mega/Due:65 - AGCM4:57)

        //#define BTN_EN1                     31
        //#define BTN_EN2                     33

      #elif ENABLED(FYSETC_MINI_12864)

        // From https://wiki.fysetc.com/Mini12864_Panel/?fbclid=IwAR1FyjuNdVOOy9_xzky3qqo_WeM5h-4gpRnnWhQr_O1Ef3h0AFnFXmCehK8

        // TO TEST
        //#define DOGLCD_A0                   16
        //#define DOGLCD_CS                   17

        //#define BTN_EN1                     33
        //#define BTN_EN2                     31

        //#define FORCE_SOFT_SPI                  // Use this if default of hardware SPI causes display problems
                                                  //   results in LCD soft SPI mode 3, SD soft SPI mode 0

        //#define LCD_RESET_PIN               23  // Must be high or open for LCD to operate normally.

        #if EITHER(FYSETC_MINI_12864_1_2, FYSETC_MINI_12864_2_0)
          #ifndef RGB_LED_R_PIN
            // TO TEST
            //#define RGB_LED_R_PIN           25
          #endif
          #ifndef RGB_LED_G_PIN
            // TO TEST
            //#define RGB_LED_G_PIN           27
          #endif
          #ifndef RGB_LED_B_PIN
            // TO TEST
            //#define RGB_LED_B_PIN           29
          #endif
        #elif ENABLED(FYSETC_MINI_12864_2_1)
          // TO TEST
          //#define NEOPIXEL_PIN              25
        #endif

      #endif

    #elif ENABLED(MINIPANEL)

      // TO TEST
      //#define BEEPER_PIN                    42
      // not connected to a pin
      //#define LCD_BACKLIGHT_PIN             57  // backlight LED on A11/D? (Mega/Due:65 - AGCM4:57)

      //#define DOGLCD_A0                     44
      //#define DOGLCD_CS                     58  // Mega/Due:66 - AGCM4:58

      // GLCD features
      // Uncomment screen orientation
      //#define LCD_SCREEN_ROT_90
      //#define LCD_SCREEN_ROT_180
      //#define LCD_SCREEN_ROT_270

      //#define BTN_EN1                       40
      //#define BTN_EN2                       55  // Mega/Due:63 - AGCM4:55
      //#define BTN_ENC                       72  // Mega/Due:59 - AGCM4:72

      //#define SD_DETECT_PIN                 49
      //#define KILL_PIN                      56  // Mega/Due:64 - AGCM4:56

    #elif ENABLED(ZONESTAR_LCD)

      // TO TEST
      //#define ADC_KEYPAD_PIN                12

    #elif ENABLED(AZSMZ_12864)

      // TO TEST

    #else

      // Beeper on AUX-4
      //#define BEEPER_PIN                    33

      // Buttons are directly attached to AUX-2
      #if IS_RRW_KEYPAD
        // TO TEST
        //#define SHIFT_OUT_PIN               40
        //#define SHIFT_CLK_PIN               44
        //#define SHIFT_LD_PIN                42
        //#define BTN_EN1                     56  // Mega/Due:64 - AGCM4:56
        //#define BTN_EN2                     72  // Mega/Due:59 - AGCM4:72
        //#define BTN_ENC                     55  // Mega/Due:63 - AGCM4:55
      #elif ENABLED(PANEL_ONE)
        // TO TEST
        //#define BTN_EN1                     72  // AUX2 PIN 3 (Mega/Due:59 - AGCM4:72)
        //#define BTN_EN2                     55  // AUX2 PIN 4 (Mega/Due:63 - AGCM4:55)
        //#define BTN_ENC                     49  // AUX3 PIN 7
      #else
        // TO TEST
        //#define BTN_EN1                     37
        //#define BTN_EN2                     35
        //#define BTN_ENC                     31
      #endif

      #if ENABLED(G3D_PANEL)
        // TO TEST
        //#define SD_DETECT_PIN               49
        //#define KILL_PIN                    41
      #endif

    #endif
  #endif // IS_NEWPANEL

#endif // HAS_WIRED_LCD

/************************************/
/******     SD Support         ******/
/************************************/

/**
   * The Bricolemon board have a SD slot, but if you want to change it to your LCD or Custom set the option to one of the following:
   *
   *           LCD - Use the SD drive in the external LCD controller.
   *       ONBOARD - Use the SD drive on the control board. 
   *  CUSTOM_CABLE - Use a custom cable to access the SD (as defined in a pins file).
   *
   * :[ 'LCD', 'ONBOARD', 'CUSTOM_CABLE' ]
   */

#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION              ONBOARD
#endif

#if SD_CONNECTION_IS(ONBOARD)
  #define SDSS                                83
  #undef SD_DETECT_PIN
  #define SD_DETECT_PIN                       95
#else
  #define SDSS                                53
#endif


