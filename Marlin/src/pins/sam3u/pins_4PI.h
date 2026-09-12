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
 * 4pi (AT91SAM3U4E) pin assignments
 *
 * Sources: the board's own pinmap and EAGLE schematic (reup), cross-checked
 * against the original 4pi/Sprinter firmware.
 *
 * Pins are named natively - PA0..PC31 - matching the schematic. See
 * HAL/SAM3U/include/pinmapping.h for the numbering.
 *
 * Notable board facts:
 *  - Five A4982-class drivers, microstepping set by MS1/MS2 pins per axis
 *  - Motor current set by an AD5206 digital pot on the SPI bus (PA14/15/16)
 *  - Thermistors on the 12-bit ADC12B
 *  - The SD card is on HSMCI (PA3..PA8), not SPI - see the note below
 *  - Host serial is native USB CDC (SERIAL_PORT -1), which is how the board is
 *    normally used. The chip's UART on PA11/PA12 is also available as
 *    SERIAL_PORT 0, on pins 13/14 of the expansion header
 */

#define BOARD_INFO_NAME "4pi"
#define DEFAULT_MACHINE_NAME BOARD_INFO_NAME

#define MAX_EXTRUDERS 2

/**
 * Stepper drivers
 *
 * The 4pi carries five Allegro A4982 drivers soldered directly to the board -
 * they are not sockets, so there is nothing to swap. Marlin has no A4982 type,
 * and none is needed: for firmware purposes the A4982 is an A4988 with the
 * microstep table capped at 1/16 (see MICROSTEP_MODES below), and both are
 * driven identically over STEP/DIR/ENABLE plus MS1/MS2.
 *
 * One caveat that is not checkable here: MS1/MS2 select 1, 1/2, 1/4 or 1/16
 * only - the A4982 has no 1/8 step. A MICROSTEP_MODES entry of 8 would be
 * programmed as the 1/16 pattern and that axis would move at half the rate
 * Marlin expects.
 *
 * Selecting any other driver type would silently produce wrong behavior -
 * a TMC type would have Marlin talking UART/SPI to pins that go nowhere, and
 * a DRV8825 would use a microstep table this hardware cannot reach - so reject
 * it at compile time rather than at the printer.
 */
#if !(AXIS_DRIVER_TYPE_X(A4988) && AXIS_DRIVER_TYPE_Y(A4988) && AXIS_DRIVER_TYPE_Z(A4988) && AXIS_DRIVER_TYPE_E0(A4988))
  #error "4pi has soldered-on A4982 drivers. Set ([XYZ]|E0)_DRIVER_TYPE to A4988 in Configuration.h."
#elif E_STEPPERS > 1 && !AXIS_DRIVER_TYPE_E1(A4988)
  #error "4pi has soldered-on A4982 drivers. Set E1_DRIVER_TYPE to A4988 in Configuration.h."
#endif

//
// Limit Switches
//
#define X_MIN_PIN                           PB16
#define Y_MIN_PIN                           PA17
#define Z_MIN_PIN                           PC12

#define X_MAX_PIN                           PC15  // Also ADC12B4
#define Y_MAX_PIN                           PC17  // Also ADC12B6
#define Z_MAX_PIN                           PC18  // Also ADC12B7

//
// Steppers
//
#define X_STEP_PIN                          PC28
#define X_DIR_PIN                           PB8
#define X_ENABLE_PIN                        PC31
#define X_MS1_PIN                           PC30
#define X_MS2_PIN                           PC29

#define Y_STEP_PIN                          PC23
#define Y_DIR_PIN                           PA31
#define Y_ENABLE_PIN                        PA22
#define Y_MS1_PIN                           PC11
#define Y_MS2_PIN                           PC10

#define Z_STEP_PIN                          PA27
#define Z_DIR_PIN                           PC27
#define Z_ENABLE_PIN                        PB7
#define Z_MS1_PIN                           PB6
#define Z_MS2_PIN                           PB5

#define E0_STEP_PIN                         PC26
#define E0_DIR_PIN                          PB2
#define E0_ENABLE_PIN                       PB13
#define E0_MS1_PIN                          PB12
#define E0_MS2_PIN                          PB11

#define E1_STEP_PIN                         PB1
#define E1_DIR_PIN                          PC25
#define E1_ENABLE_PIN                       PC13
#define E1_MS1_PIN                          PB0
#define E1_MS2_PIN                          PC24

//
// Temperature Sensors
//
// The original firmware wires hotend 1 to the "TEMP1" input, hotend 2 to
// "TEMP2" and the bed to "TEMP0"; that assignment is kept here.
//
#define TEMP_0_PIN                          PB4   // TEMP1 / ADC12B3
#define TEMP_1_PIN                          PA30  // TEMP2 / ADC12B1
#define TEMP_BED_PIN                        PC16  // TEMP0 / ADC12B5

// Spare thermistor input, free for a chamber or probe sensor
#define TEMP_2_PIN                          PB3   // TEMP3 / ADC12B2

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PA21  // HOTEND1
#define HEATER_1_PIN                        PA23  // HOTEND2
#define HEATER_BED_PIN                      PA20  // BEDFET

#define FAN0_PIN                            PA25  // FET1
#define FAN1_PIN                            PA24  // FET2

//
// Motor current - AD5206 6-channel digital potentiometer on the SPI bus
//
// AD5206 channel order on this board is Z, Y, E0, X, AUX, E1, so the
// X/Y/Z/E ordering Marlin expects maps to channels 3, 1, 0, 2.
//
#define DIGIPOTSS_PIN                       PA16  // SPI NPCS0
#define DIGIPOT_CHANNELS           { 3, 1, 0, 2, 5 }  // X Y Z E0 E1

// The AD5206 wiper setting maps to roughly (count * 7.43) mA, so the original
// firmware's default of 128 is about 950mA per driver. Override in
// Configuration_adv.h to suit the motors actually fitted.
#ifndef DIGIPOT_MOTOR_CURRENT
  #define DIGIPOT_MOTOR_CURRENT   { 128, 128, 128, 128, 128 }
#endif

//
// Misc. board I/O
//
#define SD_DETECT_PIN                       PA1
#define POWER_MONITOR_VOLTAGE_PIN           PA18  // VMOTDET - motor supply present

//
// Indicator LEDs - three RGB LEDs, nine pins
//
/**
 * What the schematic calls LED1..LED9 is not nine separate LEDs: it is three
 * common-cathode RGB packages, each with its three anodes driven by a MCU pin
 * through a series resistor and all cathodes tied to ground. So they are plain
 * active-high outputs, and each group of three is one physical LED that can be
 * mixed to any colour.
 *
 * Package pinout (from the BOM part):
 *   Pin 1 anode blue    Pin 6 cathode blue
 *   Pin 2 anode green   Pin 5 cathode green
 *   Pin 3 anode red     Pin 4 cathode red
 *
 * Which gives, per package - note the schematic's net numbering runs blue,
 * green, red within each group, not red-first:
 *
 *   RGB LED 1 (U$33)   blue LED1 PC22   green LED2 PA29   red LED3 PA28
 *   RGB LED 2 (U$35)   blue LED4 PA2    green LED5 PC1    red LED6 PA0
 *   RGB LED 3 (U$34)   blue LED7 PA26   green LED8 PC20   red LED9 PC0
 *
 * Two further LEDs on the board are hardwired power indicators - one on VUSB
 * and one on the V+ motor supply - and cannot be driven by firmware.
 */

// By schematic net name, for M42 and for matching the board documentation
#define LED1_PIN                            PC22  // RGB 1 blue
#define LED2_PIN                            PA29  // RGB 1 green
#define LED3_PIN                            PA28  // RGB 1 red
#define LED4_PIN                            PA2   // RGB 2 blue
#define LED5_PIN                            PC1   // RGB 2 green
#define LED6_PIN                            PA0   // RGB 2 red
#define LED7_PIN                            PA26  // RGB 3 blue
#define LED8_PIN                            PC20  // RGB 3 green
#define LED9_PIN                            PC0   // RGB 3 red

/**
 * Marlin's status LED. LED1 is the *blue* channel of the first RGB package, so
 * the stock status LED lights blue; point LED_PIN at LED2_PIN (green) or
 * LED3_PIN (red) for a different colour.
 */
#ifndef LED_PIN
  #define LED_PIN                       LED1_PIN
#endif

/**
 * Marlin's RGB_LED / RGBW_LED feature drives one colour-mixed LED. Enable
 * RGB_LED in Configuration.h and the first package is used; for the second or
 * third, define RGB_LED_R/G/B_PIN yourself from the table above.
 *
 * Note the feature wants PWM for smooth mixing and this HAL has none, so
 * colours are limited to the eight on/off combinations - leds.cpp falls back
 * to WRITE() whenever PWM_PIN() is false. See HAL/SAM3U/AGENTS.md.
 */
#if ENABLED(RGB_LED) && !defined(RGB_LED_R_PIN)
  #define RGB_LED_R_PIN                 LED6_PIN
  #define RGB_LED_G_PIN                 LED5_PIN
  #define RGB_LED_B_PIN                 LED4_PIN
#endif

//
// Host communication
//
// Pins for documentation and sanity checks only.
// Changing these will not change the pin they are on.
//

// Native USB (SERIAL_PORT -1)
//
// The 4pi has no USB-serial chip - the host link is the SAM3U's own UDPHS
// device controller. Its data lines are dedicated analog pins, not PIO, so
// they have no pin number and cannot be reassigned:
//
//   DHSDP  D+  -> USB connector pin 3, through a 39R series resistor (R35)
//   DHSDM  D-  -> USB connector pin 2, through a 39R series resistor (R34)
//   VBG        -> bandgap reference resistor (R36); required by the transceiver
//   VDDUTMI    -> UTMI transceiver supply
//
// The pull-up on D+ is internal to the SAM3U (BOARD_USB_PULLUP_INTERNAL in the
// original firmware), so there is no pull-up control pin either.
#define USB_VBUS_DETECT_PIN                 PC19  // USBDET - VBUS present, via R37/R38 divider

// Hardware UART pins, numbered as Marlin's SERIAL_PORT values
//
// Only SERIAL_PORT 0 is usable on this board: it is the only one whose pins
// reach a header. Every USART shares pins with a board function, and
// HAL/SAM3U/inc/SanityCheck.h rejects or warns on them accordingly.
#define UART0_RX_PIN                        PA11  // UART   URXD - J_PERIPH pin 13
#define UART0_TX_PIN                        PA12  // UART   UTXD - J_PERIPH pin 14
#define UART1_RX_PIN                        PA19  // USART0 RXD0 - also J_PERIPH CS3 (SPI NPCS3)
#define UART1_TX_PIN                        PA18  // USART0 TXD0 - also POWER_MONITOR_VOLTAGE_PIN
#define UART2_RX_PIN                        PA21  // USART1 RXD1 - also HEATER_0_PIN
#define UART2_TX_PIN                        PA20  // USART1 TXD1 - also HEATER_BED_PIN
#define UART3_RX_PIN                        PA23  // USART2 RXD2 - also HEATER_1_PIN
#define UART3_TX_PIN                        PA22  // USART2 TXD2 - also Y_ENABLE_PIN
#define UART4_RX_PIN                        PC13  // USART3 RXD3 - also E1_ENABLE_PIN
#define UART4_TX_PIN                        PC12  // USART3 TXD3 - also Z_MIN_PIN

/**
 * ============================ Expansion headers ============================
 *
 * Traced from the board's EAGLE schematic (referance material/reup). Header
 * designators are the schematic's; the silkscreen may differ.
 *
 * "GND" below is MOTGND, the board's single ground net. "VCC" is the 3.3V
 * rail; "V+" is the unregulated motor supply.
 *
 * --- J_PERIPH (U$43) - 14-pin SPI / I2C / serial header -------------------
 *
 *   Pin  Signal    MCU pin  Peripheral function
 *    1   V+         -       Motor supply
 *    2   GND        -
 *    3   GND        -
 *    4   VCC        -       3.3V
 *    5   MISO      PA13     SPI MISO      (peripheral A)
 *    6   MOSI      PA14     SPI MOSI      (peripheral A)
 *    7   SCK       PA15     SPI SPCK      (peripheral A)
 *    8   CS3       PA19     SPI NPCS3     (peripheral B)
 *    9   CS1       PC4      SPI NPCS2     (peripheral B)
 *   10   CS2       PC3      SPI NPCS1     (peripheral B)
 *   11   I2CDATA   PA9      TWI0 TWD0     (peripheral A)
 *   12   I2CCLK    PA10     TWI0 TWCK0    (peripheral A)
 *   13   RX        PA11     UART URXD     (peripheral A)
 *   14   TX        PA12     UART UTXD     (peripheral A)
 *
 *   Watch the chip-select labels: the silkscreen numbering does not match the
 *   silicon's. CS1 is NPCS2, CS2 is NPCS1, and only CS3 is NPCS3.
 *
 *   This header carries the same SPI bus as the onboard AD5206 digipot, which
 *   sits on NPCS0 (PA16). Anything added here shares SCK/MOSI/MISO with it.
 *
 * --- J_ENDSTOP (U$17) - 20-pin endstop and thermistor header --------------
 *
 *   Pin  Signal  MCU pin      Pin  Signal  MCU pin
 *    1   GND      -           11   Y-      PA17
 *    2   T0      PC16         12   GND      -
 *    3   GND      -           13   Z-      PC12
 *    4   T1      PB4          14   GND      -
 *    5   GND      -           15   X+      PC15
 *    6   T2      PA30         16   GND      -
 *    7   GND      -           17   Y+      PC17
 *    8   T3      PB3          18   GND      -
 *    9   X-      PB16         19   Z+      PC18
 *   10   GND      -           20   PB15    PB15
 *
 *   T0..T3 are the four thermistor inputs (see TEMP_*_PIN above). The MAX
 *   endstop inputs double as ADC12B channels 4, 6 and 7. Pin 20 (PB15) is a
 *   plain GPIO the board labels Z-MAX-GND.
 *
 * --- J_GPIO (U$3) - 20-pin general purpose I/O header ---------------------
 *
 *   Pin  MCU     Pin  MCU     Pin  MCU     Pin  MCU
 *    1   PB27     6   PB22    11   PB18    16   PB9
 *    2   PB26     7   PB21    12   PB17    17   VUSB
 *    3   PB25     8   PC21    13   PC14    18   GND
 *    4   PB24     9   PB20    14   PB14    19   GND
 *    5   PB23    10   PB19    15   PB10    20   VCC
 *
 *   Sixteen uncommitted PIO lines - the obvious place to attach an LCD,
 *   controller buttons, extra endstops or a probe. Note pin 17 is VUSB (bus
 *   power), not the regulated 3.3V rail on pin 20.
 *
 * --- J_GPIO4 (U$28) - 4-pin GPIO header -----------------------------------
 *
 *   Pin 1 = PB31, pin 2 = PB30, pin 3 = PB29, pin 4 = PB28
 *
 * --- J_FET (U$20) - 4-pin MOSFET output header ----------------------------
 *
 *   Pin 1 = V+, pin 2 = FET2 output (gate PA24), pin 3 = V+, pin 4 = FET1
 *   output (gate PA25). Note the outputs are in the order FET2 then FET1.
 *   These are the two general-purpose low-side switches Marlin uses as
 *   FAN0_PIN (PA25) and FAN1_PIN (PA24).
 *
 * --- J_JTAG (U$18) - 6-pin JTAG / SWD header ------------------------------
 *
 *   Pin 1 = TDI, pin 2 = NRST, pin 3 = TDO, pin 4 = TCK, pin 5 = GND,
 *   pin 6 = TMS
 *
 *   Usable as 2-wire SWD: TMS is SWDIO and TCK is SWCLK.
 */

//
// Expansion header resources
//
// Named here so features configured to use them do not have to hard-code pin
// numbers. None of these are claimed by the board itself.
//

// TWI0 on J_PERIPH pins 11/12 - for an I2C EEPROM, accelerometer, etc.
#define I2C_SDA_PIN                         PA9   // TWD0
#define I2C_SCL_PIN                         PA10  // TWCK0

// Spare hardware SPI chip selects on J_PERIPH (see the label warning above)
#define EXP_CS1_PIN                         PC4   // "CS1" - SPI NPCS2
#define EXP_CS2_PIN                         PC3   // "CS2" - SPI NPCS1
#define EXP_CS3_PIN                         PA19  // "CS3" - SPI NPCS3

//
// Onboard SD card - HSMCI, not SPI
//
/**
 * The microSD socket is wired to the High Speed Multimedia Card Interface in
 * 4-bit mode. Marlin's stock Sd2Card driver speaks SPI and cannot reach it, so
 * ONBOARD_SDIO routes the SD layer through the native host driver in
 * HAL/SAM3U/sdio.cpp instead.
 *
 * ONBOARD_SDIO is Marlin's generic "onboard card on a native SD host
 * controller rather than SPI" flag - not a reference to the SDIO standard for
 * WiFi/GPS cards. Every HAL implements it with whatever controller it has;
 * HSMCI is the SAM3U's.
 *
 *   MCCK   PA3   Card clock
 *   MCCDA  PA4   Command / response
 *   MCDA0  PA5   Data 0
 *   MCDA1  PA6   Data 1
 *   MCDA2  PA7   Data 2
 *   MCDA3  PA8   Data 3
 *
 * All six are on PIOA peripheral A and are driven by the controller, so they
 * are documented here rather than defined as assignable pins.
 *
 * To use an external SPI card reader on the expansion header instead, comment
 * out ONBOARD_SDIO and set SD_SS_PIN to one of EXP_CS1/2/3_PIN. Do not point
 * SD_SS_PIN at PA16 - that is the AD5206 digipot's select, and SanityCheck.h
 * rejects it.
 */
#ifndef ONBOARD_SDIO
  #define ONBOARD_SDIO                                // SD socket is on HSMCI
#endif

#if ENABLED(ONBOARD_SDIO)
  #define ONBOARD_SD_MCCK_PIN               PA3   // Documentation only -
  #define ONBOARD_SD_MCCDA_PIN              PA4   // these pins belong to
  #define ONBOARD_SD_MCDA0_PIN              PA5   // the HSMCI controller and
  #define ONBOARD_SD_MCDA1_PIN              PA6   // cannot be reassigned
  #define ONBOARD_SD_MCDA2_PIN              PA7
  #define ONBOARD_SD_MCDA3_PIN              PA8
#endif
