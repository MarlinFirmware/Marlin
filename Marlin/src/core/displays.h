/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * displays.h - Values for LCD_CONTROLLER
 *
 * Set LCD_CONTROLLER in Configuration.h to one of the names below.
 * Enabling a single controller option by name (e.g., "#define VIKI2") also still works.
 */

#define LCD_NONE                                    1  // No display

//=============================================================================
//======================== LCD / Controller Selection =========================
//========================   (Character-based LCDs)   =========================
//=============================================================================

//
// RepRapDiscount Smart Controller.
// https://reprap.org/wiki/RepRapDiscount_Smart_Controller
//
// Note: Usually sold with a white PCB.
//
#define LCD_REPRAP_DISCOUNT_SMART_CONTROLLER      101

//
// GT2560 (YHCB2004) LCD Display
//
// Requires Testato, Koepel softwarewire library and
// Andriy Golovnya's LiquidCrystal_AIP31068 library.
//
#define LCD_YHCB2004                              102

//
// Original RADDS LCD Display+Encoder+SDCardReader
// https://web.archive.org/web/20200719145306/doku.radds.org/dokumentation/lcd-display/
//
#define LCD_RADDS_DISPLAY                         103

//
// ULTIMAKER Controller.
//
#define LCD_ULTIMAKERCONTROLLER                   104

//
// ULTIPANEL as seen on Thingiverse.
//
#define LCD_ULTIPANEL                             105

//
// PanelOne from T3P3 (via RAMPS 1.4 AUX2/AUX3)
// https://reprap.org/wiki/PanelOne
//
#define LCD_PANEL_ONE                             106

//
// GADGETS3D G3D LCD/SD Controller
// https://reprap.org/wiki/RAMPS_1.3/1.4_GADGETS3D_Shield_with_Panel
//
// Note: Usually sold with a blue PCB.
//
#define LCD_G3D_PANEL                             107

//
// RigidBot Panel V1.0
//
#define LCD_RIGIDBOT_PANEL                        108

//
// Makeboard 3D Printer Parts 3D Printer Mini Display 1602 Mini Controller
// https://www.aliexpress.com/item/32765887917.html
//
#define LCD_MAKEBOARD_MINI_2_LINE_DISPLAY_1602    109

/**
 * ANET and Tronxy 20x4 Controller
 * LCD2004 display with 5 analog buttons.
 *
 * NOTE: Requires ADC_KEYPAD_PIN to be assigned to an analog pin.
 * This LCD is known to be susceptible to electrical interference which
 * scrambles the display. Press any button to clear it up.
 */
#define LCD_ZONESTAR_LCD                          110

//
// Generic 16x2, 16x4, 20x2, or 20x4 character-based LCD.
//
#define LCD_ULTRA_LCD                             111

//=============================================================================
//======================== LCD / Controller Selection =========================
//=====================   (I2C and Shift-Register LCDs)   =====================
//=============================================================================

//
// CONTROLLER TYPE: I2C
//
// Note: These controllers require the installation of Arduino's LiquidCrystal_I2C
// library. For more info: https://github.com/kiyoshigawa/LiquidCrystal_I2C
//

//
// Elefu RA Board Control Panel
// https://web.archive.org/web/20140823033947/www.elefu.com/index.php?route=product/product&product_id=53
//
#define LCD_RA_CONTROL_PANEL                      201

//
// Sainsmart (YwRobot) LCD Displays
//
// These require LiquidCrystal_I2C library:
//   https://github.com/MarlinFirmware/New-LiquidCrystal
//   https://github.com/fmalpartida/New-LiquidCrystal/wiki
//
#define LCD_LCD_SAINSMART_I2C_1602                202
#define LCD_LCD_SAINSMART_I2C_2004                203

//
// Generic LCM1602 LCD adapter
//
#define LCD_LCM1602                               204

//
// PANELOLU2 LCD with status LEDs,
// separate encoder and click inputs.
//
// Note: This controller requires Arduino's LiquidTWI2 library v1.2.3 or later.
// For more info: https://github.com/lincomatic/LiquidTWI2
//
// Note: The PANELOLU2 encoder click input can either be directly connected to
// a pin (if BTN_ENC defined to != -1) or read through I2C (when BTN_ENC == -1).
//
#define LCD_LCD_I2C_PANELOLU2                     205

//
// Panucatt VIKI LCD with status LEDs,
// integrated click & L/R/U/D buttons, separate encoder inputs.
//
#define LCD_LCD_I2C_VIKI                          206

//
// CONTROLLER TYPE: Shift register panels
//

//
// 2-wire Non-latching LCD SR from https://github.com/fmalpartida/New-LiquidCrystal/wiki/schematics#user-content-ShiftRegister_connection
// LCD configuration: https://reprap.org/wiki/SAV_3D_LCD
//
#define LCD_SAV_3DLCD                             207

//
// 3-wire SR LCD with strobe using 74HC4094
// https://github.com/mikeshub/SailfishLCD
// Uses the code directly from Sailfish
//
#define LCD_FF_INTERFACEBOARD                     208

//
// MightyBoard LCD and Interface
//
#define LCD_MIGHTYBOARD_LCD                       209

//
// TFT GLCD Panel with Marlin UI
// Panel connected to main board by SPI or I2C interface.
// See https://github.com/Serhiy-K/TFTGLCDAdapter
//
#define LCD_TFTGLCD_PANEL_SPI                     210
#define LCD_TFTGLCD_PANEL_I2C                     211

//=============================================================================
//=======================   LCD / Controller Selection  =======================
//=========================      (Graphical LCDs)      ========================
//=============================================================================

//
// CONTROLLER TYPE: Graphical 128x64 (DOGM)
//
// IMPORTANT: The U8glib library is required for Graphical Display!
//            https://github.com/olikraus/U8glib_Arduino
//
// NOTE: If the LCD is unresponsive you may need to reverse the plugs.
//

//
// RepRapDiscount FULL GRAPHIC Smart Controller
// https://reprap.org/wiki/RepRapDiscount_Full_Graphic_Smart_Controller
//
#define LCD_REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER  301

//
// K.3D Full Graphic Smart Controller
//
#define LCD_K3D_FULL_GRAPHIC_SMART_CONTROLLER     302

//
// ReprapWorld Graphical LCD
// https://reprapworld.com/electronics/3d-printer-modules/autonomous-printing/graphical-lcd-screen-v1-0/
//
#define LCD_REPRAPWORLD_GRAPHICAL_LCD             303

//
// Activate one of these if you have a Panucatt Devices
// Viki 2.0 or mini Viki with Graphic LCD
// https://www.panucatt.com
//
#define LCD_VIKI2                                 304
#define LCD_miniVIKI                              305

//
// Alfawise Ex8 printer LCD marked as WYH L12864 COG
//
#define LCD_WYH_L12864                            306

//
// MakerLab Mini Panel with graphic
// controller and SD support - https://reprap.org/wiki/Mini_panel
//
#define LCD_MINIPANEL                             307

//
// MaKr3d Makr-Panel with graphic controller and SD support.
// https://reprap.org/wiki/MaKrPanel
//
#define LCD_MAKRPANEL                             308

//
// Adafruit ST7565 Full Graphic Controller.
// https://github.com/eboston/Adafruit-ST7565-Full-Graphic-Controller/
//
#define LCD_ELB_FULL_GRAPHIC_CONTROLLER           309

//
// BQ LCD Smart Controller shipped by
// default with the BQ Hephestos 2 and Witbox 2.
//
#define LCD_BQ_LCD_SMART_CONTROLLER               310

//
// Cartesio UI
// https://web.archive.org/web/20180605050442/mauk.cc/webshop/cartesio-shop/electronics/user-interface
//
#define LCD_CARTESIO_UI                           311

//
// LCD for Melzi Card with Graphical LCD
//
#define LCD_LCD_FOR_MELZI                         312

//
// Original Ulticontroller from Ultimaker 2 printer with SSD1309 I2C display and encoder
// https://github.com/Ultimaker/Ultimaker2/tree/master/1249_Ulticontroller_Board_(x1)
//
#define LCD_ULTI_CONTROLLER                       313

//
// MKS MINI12864 with graphic controller and SD support
// https://reprap.org/wiki/MKS_MINI_12864
//
#define LCD_MKS_MINI_12864                        314

//
// MKS MINI12864 V3 is an alias for FYSETC_MINI_12864_2_1. Type A/B. NeoPixel RGB Backlight.
//
#define LCD_MKS_MINI_12864_V3                     315

//
// MKS LCD12864A/B with graphic controller and SD support. Follows MKS_MINI_12864 pinout.
// https://www.aliexpress.com/item/33018110072.html
//
#define LCD_MKS_LCD12864A                         316
#define LCD_MKS_LCD12864B                         317

//
// FYSETC variant of the MINI12864 graphic controller with SD support
// https://wiki.fysetc.com/docs/Mini12864Panel
//
#define LCD_FYSETC_MINI_12864_X_X                 318  // Type C/D/E/F. No tunable RGB Backlight by default
#define LCD_FYSETC_MINI_12864_1_2                 319  // Type C/D/E/F. Simple RGB Backlight (always on)
#define LCD_FYSETC_MINI_12864_2_0                 320  // Type A/B. Discreet RGB Backlight
#define LCD_FYSETC_MINI_12864_2_1                 321  // Type A/B. NeoPixel RGB Backlight
#define LCD_FYSETC_GENERIC_12864_1_1              322  // Larger display with basic ON/OFF backlight.

//
// BigTreeTech Mini 12864 V1.0 / V2.0 is an alias for FYSETC_MINI_12864_2_1. Type A/B. NeoPixel RGB Backlight.
// https://github.com/bigtreetech/MINI-12864
//
#define LCD_BTT_MINI_12864                        323

//
// BEEZ MINI 12864 is an alias for FYSETC_MINI_12864_2_1. Type A/B. NeoPixel RGB Backlight.
//
#define LCD_BEEZ_MINI_12864                       324

//
// Factory display for Creality CR-10 / CR-7 / Ender-3
// https://marlinfw.org/docs/hardware/controllers.html#cr10_stockdisplay
//
// Connect to EXP1 on RAMPS and compatible boards.
//
#define LCD_CR10_STOCKDISPLAY                     325

//
// Ender-2 OEM display, a variant of the MKS_MINI_12864
//
#define LCD_ENDER2_STOCKDISPLAY                   326

//
// ANET and Tronxy 128×64 Full Graphics Controller as used on Anet A6
//
#define LCD_ANET_FULL_GRAPHICS_LCD                327

//
// GUCOCO CTC 128×64 Full Graphics Controller as used on GUCOCO CTC A10S
//
#define LCD_CTC_A10S_A13                          328

//
// AZSMZ 12864 LCD with SD
// https://www.aliexpress.com/item/32837222770.html
//
#define LCD_AZSMZ_12864                           329

//
// Silvergate GLCD controller
// https://github.com/android444/Silvergate
//
#define LCD_SILVER_GATE_GLCD_CONTROLLER           330

//
// eMotion Tech LCD with SD
// https://www.reprap-france.com/produit/1234568748-ecran-graphique-128-x-64-points-2-1
//
#define LCD_EMOTION_TECH_LCD                      331

//=============================================================================
//==============================  OLED Displays  ==============================
//=============================================================================

//
// SSD1306 OLED full graphics generic display
//
#define LCD_U8GLIB_SSD1306                        401

//
// SAV OLED LCD module with an SSD1306 or SH1106 controller
//
#define LCD_SAV_3DGLCD_SSD1306                    402
#define LCD_SAV_3DGLCD_SH1106                     403

//
// TinyBoy2 128x64 OLED / Encoder Panel
//
#define LCD_OLED_PANEL_TINYBOY2                   404

//
// MKS OLED 1.3" 128×64 Full Graphics Controller
// https://reprap.org/wiki/MKS_12864OLED
//
// Tiny, but very sharp OLED display
//
#define LCD_MKS_12864OLED                         405  // Uses the SH1106 controller
#define LCD_MKS_12864OLED_SSD1306                 406  // Uses the SSD1306 controller

//
// Zonestar OLED 128×64 Full Graphics Controller
//
#define LCD_ZONESTAR_12864LCD                     407  // Graphical (DOGM) with ST7920 controller
#define LCD_ZONESTAR_12864OLED                    408  // 1.3" OLED with SH1106 controller
#define LCD_ZONESTAR_12864OLED_SSD1306            409  // 0.96" OLED with SSD1306 controller

//
// Einstart S OLED SSD1306
//
#define LCD_U8GLIB_SH1106_EINSTART                410

//
// Overlord OLED display/controller with i2c buzzer and LEDs
//
#define LCD_OVERLORD_OLED                         411

//
// FYSETC OLED 2.42" 128×64 Full Graphics Controller with WS2812 RGB
// Where to find : https://www.aliexpress.com/item/4000345255731.html
#define LCD_FYSETC_242_OLED_12864                 412  // Uses the SSD1309 controller

//
// K.3D SSD1309 OLED 2.42" 128×64 Full Graphics Controller
//
#define LCD_K3D_242_OLED_CONTROLLER               413  // Software SPI

//=============================================================================
//========================== Extensible UI Displays ===========================
//=============================================================================

/**
 * DGUS Touch Display with DWIN OS.
 *
 * ORIGIN (Marlin DWIN_SET)
 *  - Download https://github.com/coldtobi/Marlin_DGUS_Resources
 *  - Copy the downloaded DWIN_SET folder to the SD card.
 *  - Product: https://www.aliexpress.com/item/32993409517.html
 *
 * FYSETC (Supplier default)
 *  - Download https://github.com/FYSETC/FYSTLCD-2.0
 *  - Copy the downloaded SCREEN folder to the SD card.
 *  - Product: https://www.aliexpress.com/item/32961471929.html
 *
 * HIPRECY (Supplier default)
 *  - Download https://github.com/HiPrecy/Touch-Lcd-LEO
 *  - Copy the downloaded DWIN_SET folder to the SD card.
 *
 * MKS (MKS-H43) (Supplier default)
 *  - Download https://github.com/makerbase-mks/MKS-H43
 *  - Copy the downloaded DWIN_SET folder to the SD card.
 *  - Product: https://www.aliexpress.com/item/1005002008179262.html
 *
 * RELOADED (T5UID1)
 *  - Download https://github.com/Neo2003/DGUS-reloaded/releases
 *  - Copy the downloaded DWIN_SET folder to the SD card.
 *
 * IA_CREALITY (T5UID1)
 *  - Download https://github.com/InsanityAutomation/Marlin/raw/CrealityDwin_2.0/TM3D_Combined480272_Landscape_V7.7z
 *  - Copy the downloaded DWIN_SET folder to the SD card.
 *
 * E3S1PRO (T5L)
 *  - Download https://github.com/CrealityOfficial/Ender-3S1/archive/3S1_Plus_Screen.zip
 *  - Copy the downloaded DWIN_SET folder to the SD card.
 *
 * Flash display with DGUS Displays for Marlin:
 *  - Format the SD card to FAT32 with an allocation size of 4kb.
 *  - Download files as specified for your type of display.
 *  - Plug the microSD card into the back of the display.
 *  - Boot the display and wait for the update to complete.
 */
#define LCD_DGUS_ORIGIN                           501
#define LCD_DGUS_FYSETC                           502
#define LCD_DGUS_HIPRECY                          503
#define LCD_DGUS_MKS                              504
#define LCD_DGUS_RELOADED                         505
#define LCD_DGUS_IA_CREALITY                      506
#define LCD_DGUS_E3S1PRO                          507

//
// LCD for Malyan M200/M300 printers
//
#define LCD_MALYAN_LCD                            508

//
// Touch UI for FTDI EVE (FT800/FT810) displays
// See Configuration_adv.h for all configuration options.
//
#define LCD_TOUCH_UI_FTDI_EVE                     509

//
// Touch-screen LCD for Anycubic Chiron
//
#define LCD_ANYCUBIC_LCD_CHIRON                   510

//
// Touch-screen LCD for Anycubic i3 Mega
//
#define LCD_ANYCUBIC_LCD_I3MEGA                   511

//
// Touch-screen LCD for Anycubic Vyper
//
#define LCD_ANYCUBIC_LCD_VYPER                    512

//
// Sovol SV-06 Resistive Touch Screen
//
#define LCD_SOVOL_SV06_RTS                        513

//
// 320x240 Nextion 2.8" serial TFT Resistive Touch Screen NX3224T028
//
#define LCD_NEXTION_TFT                           514

//
// Third-party or vendor-customized controller interfaces.
// Sources should be installed in 'src/lcd/extui'.
//
#define LCD_EXTENSIBLE_UI                         515

//=============================================================================
//=============================== Graphical TFTs ==============================
//=============================================================================

/**
 * Specific TFT Model Presets. Select one of the following,
 * or select LCD_TFT_GENERIC and set its options in Configuration.h.
 */

//
// 480x320, 3.5", SPI Display with Rotary Encoder from MKS
// Usually paired with MKS Robin Nano V2 & V3
// https://github.com/makerbase-mks/MKS-TFT-Hardware/tree/master/MKS%20TS35
//
#define LCD_MKS_TS35_V2_0                         601

//
// MKS TS24-R V2.1 (2.4" 320x240 ST7789V) as shipped with the MKS DLC32
//
#define LCD_MKS_TS24_R_V2_1                       602

//
// 320x240, 2.4", FSMC Display From MKS
// Usually paired with MKS Robin Nano V1.2
//
#define LCD_MKS_ROBIN_TFT24                       603

//
// 320x240, 2.8", FSMC Display From MKS
// Usually paired with MKS Robin Nano V1.2
//
#define LCD_MKS_ROBIN_TFT28                       604

//
// 320x240, 3.2", FSMC Display From MKS
// Usually paired with MKS Robin Nano V1.2
//
#define LCD_MKS_ROBIN_TFT32                       605

//
// 480x320, 3.5", FSMC Display From MKS
// Usually paired with MKS Robin Nano V1.2
//
#define LCD_MKS_ROBIN_TFT35                       606

//
// 480x272, 4.3", FSMC Display From MKS
//
#define LCD_MKS_ROBIN_TFT43                       607

//
// 320x240, 3.2", FSMC Display From MKS
// Usually paired with MKS Robin
//
#define LCD_MKS_ROBIN_TFT_V1_1R                   608

//
// 480x320, 3.5", FSMC Stock Display from Tronxy
//
#define LCD_TFT_TRONXY_X5SA                       609

//
// 480x320, 3.5", FSMC Stock Display from AnyCubic
//
#define LCD_ANYCUBIC_TFT35                        610

//
// 320x240, 2.8", FSMC Stock Display from Longer/Alfawise
//
#define LCD_LONGER_LK_TFT28                       611

//
// 320x240, 2.8", FSMC Stock Display from ET4
//
#define LCD_ANET_ET4_TFT28                        612

//
// 480x320, 3.5", FSMC Stock Display from ET5
//
#define LCD_ANET_ET5_TFT35                        613

//
// 1024x600, 7", RGB Stock Display with Rotary Encoder from BIQU BX
// https://github.com/bigtreetech/BIQU-BX/tree/master/Hardware
//
#define LCD_BIQU_BX_TFT70                         614

//
// 480x320, 3.5", SPI Stock Display with Rotary Encoder from BIQU B1 SE Series
// https://github.com/bigtreetech/TFT35-SPI/tree/master/v1
//
#define LCD_BTT_TFT35_SPI_V1_0                    615

//
// Generic TFT with detailed options
//
#define LCD_TFT_GENERIC                           616

//=============================================================================
//============================  Other Controllers  ============================
//=============================================================================

//
// Ender-3 v2 OEM display. A DWIN display with Rotary Encoder.
//
#define LCD_DWIN_CREALITY_LCD                     701  // Creality UI
#define LCD_DWIN_LCD_PROUI                        702  // Pro UI by MRiscoC
#define LCD_DWIN_CREALITY_LCD_JYERSUI             703  // Jyers UI by Jacob Myers
#define LCD_DWIN_MARLINUI_PORTRAIT                704  // MarlinUI (portrait orientation)
#define LCD_DWIN_MARLINUI_LANDSCAPE               705  // MarlinUI (landscape orientation)

//
// Test the selected display with LCD_IS(NAME). Also true when NAME itself is enabled.
//
#define _LCD_IS_1(N) (_ENA_1(N) || (defined(LCD_##N) && LCD_CONTROLLER == LCD_##N))
#define LCD_IS(V...) DO(LCD_IS,||,V)
