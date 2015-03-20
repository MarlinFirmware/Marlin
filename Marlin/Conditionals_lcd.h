/**
 * Conditionals_lcd.h
 * LCD-oriented defines that depend on configuration.
 */
#ifndef CONDITIONALS_LCD_H
  #define CONDITIONALS_LCD_H

  #if defined(MAKRPANEL)
    #define DOGLCD
    #define SDSUPPORT
    #define DEFAULT_LCD_CONTRAST 17
    #define ULTIPANEL
    #define NEWPANEL
  #endif

  #if defined(miniVIKI) || defined(VIKI2)
    #define ULTRA_LCD  //general LCD support, also 16x2
    #define DOGLCD  // Support for SPI LCD 128x64 (Controller ST7565R graphic Display Family)
    #define ULTIMAKERCONTROLLER //as available from the Ultimaker online store.

    #ifdef miniVIKI
      #define DEFAULT_LCD_CONTRAST 95
    #else
      #define DEFAULT_LCD_CONTRAST 40
    #endif

    #define ENCODER_PULSES_PER_STEP 4
    #define ENCODER_STEPS_PER_MENU_ITEM 1
  #endif

  #ifdef PANEL_ONE
    #define SDSUPPORT
    #define ULTIMAKERCONTROLLER
  #endif

  #ifdef REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER
    #define DOGLCD
    #define U8GLIB_ST7920
    #define REPRAP_DISCOUNT_SMART_CONTROLLER
  #endif

  #if defined(ULTIMAKERCONTROLLER) || defined(REPRAP_DISCOUNT_SMART_CONTROLLER) || defined(G3D_PANEL)
    #define ULTIPANEL
    #define NEWPANEL
  #endif

  #ifdef REPRAPWORLD_KEYPAD
    #define ULTIPANEL
    #define NEWPANEL
  #endif

  #ifdef RA_CONTROL_PANEL
    #define LCD_I2C_TYPE_PCA8574
    #define LCD_I2C_ADDRESS 0x27   // I2C Address of the port expander
    #define ULTIPANEL
    #define NEWPANEL
  #endif

  /**
   * I2C PANELS
   */

  #ifdef LCD_I2C_SAINSMART_YWROBOT
    // This uses the LiquidCrystal_I2C library ( https://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/Home )
    // Make sure it is placed in the Arduino libraries directory.
    #define LCD_I2C_TYPE_PCF8575
    #define LCD_I2C_ADDRESS 0x27   // I2C Address of the port expander
    #define ULTIPANEL
    #define NEWPANEL
  #endif

  // PANELOLU2 LCD with status LEDs, separate encoder and click inputs
  #ifdef LCD_I2C_PANELOLU2
    // This uses the LiquidTWI2 library v1.2.3 or later ( https://github.com/lincomatic/LiquidTWI2 )
    // Make sure the LiquidTWI2 directory is placed in the Arduino or Sketchbook libraries subdirectory.
    // (v1.2.3 no longer requires you to define PANELOLU in the LiquidTWI2.h library header file)
    // Note: The PANELOLU2 encoder click input can either be directly connected to a pin
    //       (if BTN_ENC defined to != -1) or read through I2C (when BTN_ENC == -1).
    #define LCD_I2C_TYPE_MCP23017
    #define LCD_I2C_ADDRESS 0x20 // I2C Address of the port expander
    #define LCD_USE_I2C_BUZZER //comment out to disable buzzer on LCD

    #ifndef ENCODER_PULSES_PER_STEP
      #define ENCODER_PULSES_PER_STEP 4
    #endif

    #ifndef ENCODER_STEPS_PER_MENU_ITEM
      #define ENCODER_STEPS_PER_MENU_ITEM 1
    #endif

    #ifdef LCD_USE_I2C_BUZZER
      #define LCD_FEEDBACK_FREQUENCY_HZ 1000
      #define LCD_FEEDBACK_FREQUENCY_DURATION_MS 100
    #endif

    #define ULTIPANEL
    #define NEWPANEL
  #endif

  // Panucatt VIKI LCD with status LEDs, integrated click & L/R/U/P buttons, separate encoder inputs
  #ifdef LCD_I2C_VIKI
    // This uses the LiquidTWI2 library v1.2.3 or later ( https://github.com/lincomatic/LiquidTWI2 )
    // Make sure the LiquidTWI2 directory is placed in the Arduino or Sketchbook libraries subdirectory.
    // Note: The pause/stop/resume LCD button pin should be connected to the Arduino
    //       BTN_ENC pin (or set BTN_ENC to -1 if not used)
    #define LCD_I2C_TYPE_MCP23017
    #define LCD_I2C_ADDRESS 0x20 // I2C Address of the port expander
    #define LCD_USE_I2C_BUZZER //comment out to disable buzzer on LCD (requires LiquidTWI2 v1.2.3 or later)
    #define ULTIPANEL
    #define NEWPANEL
  #endif

  // Shift register panels
  // ---------------------
  // 2 wire Non-latching LCD SR from:
  // https://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/schematics#!shiftregister-connection

  #ifdef SAV_3DLCD
     #define SR_LCD_2W_NL    // Non latching 2 wire shiftregister
     #define ULTIPANEL
     #define NEWPANEL
  #endif


  #ifdef ULTIPANEL
    #define NEWPANEL  //enable this if you have a click-encoder panel
    #define SDSUPPORT
    #define ULTRA_LCD
    #ifdef DOGLCD // Change number of lines to match the DOG graphic display
      #define LCD_WIDTH 22
      #define LCD_HEIGHT 5
    #else
      #define LCD_WIDTH 20
      #define LCD_HEIGHT 4
    #endif
  #else //no panel but just LCD
    #ifdef ULTRA_LCD
    #ifdef DOGLCD // Change number of lines to match the 128x64 graphics display
      #define LCD_WIDTH 22
      #define LCD_HEIGHT 5
    #else
      #define LCD_WIDTH 16
      #define LCD_HEIGHT 2
    #endif
    #endif
  #endif

  /**
   * Default LCD contrast for dogm-like LCD displays
   */
  #if defined(DOGLCD) && !defined(DEFAULT_LCD_CONTRAST)
    #define DEFAULT_LCD_CONTRAST 32
  #endif

#endif //CONDITIONALS_LCD_H
