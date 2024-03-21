#pragma once

#define BOARD_INFO_NAME   "MKS OWL"
#define BOARD_WEBSITE_URL "github.com/makerbase-mks/MKS OWL"

#define MKS_TEST
// MKS TEST
#if ENABLED(MKS_TEST)
  #define MKS_TEST_POWER_LOSS_PIN           P1_00   // PW_DET
  #define MKS_TEST_PS_ON_PIN                P0_23   // PW_OFF
#endif

//
// Servos
//
#define SERVO0_PIN                          P0_10 // Enable BLTOUCH

//
// EEPROM
//
#if NO_EEPROM_SELECTED
   #define I2C_EEPROM      // EEPROM on I2C-0
#endif

#if ANY(NO_EEPROM_SELECTED, I2C_EEPROM)
  #define I2C_EEPROM
  #define USE_SHARED_EEPROM
  #define MARLIN_EEPROM_SIZE 0x1000 // 4KB
  #define I2C_SCL_PIN P1_27
  #define I2C_SDA_PIN P1_26
#endif

//
// Limit Switches
//
#define X_STOP_PIN                          P0_13
#define Y_STOP_PIN                          P0_24
#define Z_MIN_PIN                           P1_23
#define Z_MAX_PIN                           P1_17

//
// Steppers
//
#define X_ENABLE_PIN                        P1_20
#define X_STEP_PIN                          P1_24
#define X_DIR_PIN                           P1_13

#define Y_ENABLE_PIN                        P1_20
#define Y_STEP_PIN                          P1_11
#define Y_DIR_PIN                           P1_31

#define Z_ENABLE_PIN                        P0_06
#define Z_STEP_PIN                          P0_05
#define Z_DIR_PIN                           P1_16

#define E0_ENABLE_PIN                       P0_02
#define E0_STEP_PIN                         P1_29
#define E0_DIR_PIN                          P0_25

#define E1_ENABLE_PIN                       P0_02
#define E1_STEP_PIN                         P0_21
#define E1_DIR_PIN                          P0_20

//
// Temperature Sensors
//
#define TEMP_0_PIN                          P0_15           /* TH1  */
#define TEMP_1_PIN                          P0_31           /* TH2  */
#define TEMP_BED_PIN                        P1_08           /* TB   */

//
// Heaters / Fans
//
#ifndef HEATER_0_PIN
  #define HEATER_0_PIN                      P1_07
#endif
#if HOTENDS == 1 && DISABLED(HEATERS_PARALLEL)
  #ifndef FAN1_PIN
    #define FAN1_PIN                        P1_06
  #endif
#else
  #ifndef HEATER_1_PIN
    #define HEATER_1_PIN                    P0_17
  #endif
#endif
#ifndef FAN0_PIN
  #define FAN0_PIN                          P0_01   // FAN
#endif
#ifndef HEATER_BED_PIN
  #define HEATER_BED_PIN                    P1_09
#endif

//
// Power Supply Control
//
// #define MKS_PWC
#if ENABLED(MKS_PWC)
  #if ENABLED(TFT_LVGL_UI)
    #undef PSU_CONTROL
    #undef MKS_PWC
    #define SUICIDE_PIN                     P0_23
    #define SUICIDE_PIN_STATE               LOW
  #else
    #define PS_ON_PIN                       P1_00
  #endif
//   #define KILL_PIN                          P0_23
//   #define KILL_PIN_STATE                    HIGH
#endif

//
// Misc. Functions
//
#if HAS_TFT_LVGL_UI
  #define MT_DET_1_PIN                       P0_27
//   #define MT_DET_2_PIN                       P0_08
  #define MT_DET_PIN_STATE                   LOW
#endif

//
// SD Card(U disk)
//
#define HAS_OTG_USB_HOST_SUPPORT                // USB Flash Drive support
#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION                     ONBOARD
#endif

//
// LCD / Controller
//
  #define TFT_BUFFER_SIZE                       14400

  /**
 *                 ------                                         ------
 * (BEEPER) P1_12 |1    2| P1_14 (BTN_ENC)     (HS_SPI_MISO)P1_03 |1    2| P1_02 (HS_SPI_SCK)
 * (LCD_EN) P0_18 |3    4| P0_09 (LCD_RS)          (BTN_EN1)P1_10 |3    4| P1_01 (HS_SPI_CS)
 * (LCD_D4) P1_30 |5    6   P1_18 (LCD_D5)         (BTN_EN2) P1_22|5    6  P0_26 (HS_SPI_MOSI)
 * (LCD_D6) P0_00 |7    8| P1_21 (LCD_D7)         (SPI1_RS) P1_05 |7    8| RESET
 *           GND  |9   10| 5V                                GND  |9   10| 3.3V
 *                 ------                                         ￣￣￣
 *                 EXP1                                            EXP2
 */
#define EXP1_01_PIN                         P1_12
#define EXP1_02_PIN                         P1_14
#define EXP1_03_PIN                         P0_18
#define EXP1_04_PIN                         P0_09
#define EXP1_05_PIN                         P1_30
#define EXP1_06_PIN                         P1_18
#define EXP1_07_PIN                         P0_00
#define EXP1_08_PIN                         P1_21

#define EXP2_01_PIN                         P1_03
#define EXP2_02_PIN                         P1_02
#define EXP2_03_PIN                         P1_10
#define EXP2_04_PIN                         P1_01
#define EXP2_05_PIN                         P1_22
#define EXP2_06_PIN                         P0_26
#define EXP2_07_PIN                         P1_05
#define EXP2_08_PIN                         -1


#if ANY(TFT_COLOR_UI, TFT_LVGL_UI, TFT_CLASSIC_UI)
  #define TFT_CS_PIN                            EXP1_07_PIN            /* LCD_D6 */
  #define TFT_A0_PIN                            EXP1_08_PIN            /* LCD_D7 */
  #define TFT_DC_PIN                            EXP1_08_PIN            /* LCD_D7 */
  #define TFT_MISO_PIN                          EXP2_01_PIN            /* LCD MISO */
  #define TFT_MOSI_PIN                          EXP2_06_PIN            /* LCD MOSI */
  #define TFT_SCK_PIN                           EXP2_02_PIN            /* LCD SCK */
  #define TFT_BACKLIGHT_PIN                     EXP1_03_PIN            /* LCD_EN */
  #define TFT_RESET_PIN                         EXP1_04_PIN            /* LCD_RS */

  #define LCD_USE_DMA_SPI
  #define TOUCH_CS_PIN                          EXP1_05_PIN              /* LCD_D4 */
  #define TOUCH_MISO_PIN                        EXP2_01_PIN
  #define TOUCH_MOSI_PIN                        EXP2_06_PIN
  #define TOUCH_SCK_PIN                         EXP2_02_PIN

  #ifndef TFT_WIDTH
  #define TFT_WIDTH                             480
  #endif
  #ifndef TFT_HEIGHT
    #define TFT_HEIGHT                          320
  #endif

  #define TOUCH_BUTTONS_HW_SPI
  #define TOUCH_BUTTONS_HW_SPI_DEVICE           5

  #ifndef TOUCH_CALIBRATION_X
    #define TOUCH_CALIBRATION_X           -17253
  #endif
  #ifndef TOUCH_CALIBRATION_Y
    #define TOUCH_CALIBRATION_Y            11579
  #endif
  #ifndef TOUCH_OFFSET_X
    #define TOUCH_OFFSET_X                   514
  #endif
  #ifndef TOUCH_OFFSET_Y
    #define TOUCH_OFFSET_Y                   -24
  #endif
  #ifndef TOUCH_ORIENTATION
    #define TOUCH_ORIENTATION    TOUCH_LANDSCAPE
  #endif

#else
  #if ENABLED(MKS_MINI_12864_V3)
    #define LCD_PINS_ENABLE                   EXP1_03_PIN
    #define LCD_PINS_RS                       EXP1_04_PIN
    #define BTN_EN1                           EXP2_03_PIN
    #define BTN_EN2                           EXP2_05_PIN
    #define LCD_BACKLIGHT_PIN                 -1
    #define DOGLCD_A0                         EXP1_07_PIN
    #define DOGLCD_CS                         EXP1_06_PIN
    #define NEOPIXEL_PIN                      EXP1_05_PIN
  #elif ENABLED(MKS_MINI_12864)
    #define LCD_PINS_D4                     EXP1_05_PIN
    #if ENABLED(ULTIPANEL)
      #define LCD_PINS_D5                   EXP1_06_PIN
      #define LCD_PINS_D6                   EXP1_07_PIN
      #define LCD_PINS_D7                   EXP1_08_PIN
    #endif

    #define BOARD_ST7920_DELAY_1    DELAY_NS(200)
    #define BOARD_ST7920_DELAY_2    DELAY_NS(400)
    #define BOARD_ST7920_DELAY_3    DELAY_NS(600)

  #endif
#endif

//
// SPI Flash
//
#define HAS_SPI_FLASH                             1
#if HAS_SPI_FLASH
  #define SPI_FLASH_SIZE                          0x1000000  // 16MB
  #define SPI_FLASH_CS_PIN                        P1_19
  #define SPI_FLASH_MOSI_PIN                      P0_29
  #define SPI_FLASH_MISO_PIN                      P0_30
  #define SPI_FLASH_SCK_PIN                       P0_28
#endif

#if ANY(TFT_COLOR_UI, TFT_LVGL_UI, TFT_CLASSIC_UI, HAS_WIRED_LCD)
    #define BEEPER_PIN                         P1_12
//   #define BTN_ENC                           P1_14
//   #define BTN_EN1                           P1_10
//   #define BTN_EN2                           P1_22
#endif

#if HAS_TMC_UART
  /**
   * TMC2208/TMC2209 stepper drivers
   *
   * Hardware serial communication ports.
   * If undefined software serial is used according to the pins below
   */
  #define X_SERIAL_TX_PIN                  P1_04
  #define X_SERIAL_RX_PIN        X_SERIAL_TX_PIN

  #define Y_SERIAL_TX_PIN                  P0_19
  #define Y_SERIAL_RX_PIN        Y_SERIAL_TX_PIN

  #define Z_SERIAL_TX_PIN                  P0_04
  #define Z_SERIAL_RX_PIN        Z_SERIAL_TX_PIN

  #define E0_SERIAL_TX_PIN                 P1_25
  #define E0_SERIAL_RX_PIN      E0_SERIAL_TX_PIN

  #define E1_SERIAL_TX_PIN                 P1_28
  #define E1_SERIAL_RX_PIN      E1_SERIAL_TX_PIN

  // Reduce baud rate to improve software serial reliability
  #define TMC_BAUD_RATE                    19200
#endif // HAS_TMC_UART
