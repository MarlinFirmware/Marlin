/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * File: pins/pins_lcd.h
 *
 * Include pins for enabled LCD / Encoders
 *
 * - EXP1/2/3 headers are defined in the board pins files.
 *   with consideration for standard display adapters and mappings.
 *
 * - LCD pins are defined in terms of their connection to EXP1/2/3
 *   headers, so AUX / alternative pins must also be EXP-mapped.
 *
 */

#ifdef LCD_MODEL

  #define LCD_PATH(M) STRINGIFY_(lcd/M.h)
  #include LCD_PATH(LCD_MODEL)

#elif ENABLED(REPRAP_DISCOUNT_SMART_CONTROLLER)
  #include "lcd/REPRAP_DISCOUNT_SMART_CONTROLLER.h"
#elif ENABLED(YHCB2004)
  #include "lcd/YHCB2004.h"
#elif ENABLED(RADDS_DISPLAY)
  #include "lcd/RADDS_DISPLAY.h"
#elif ENABLED(ULTIMAKERCONTROLLER)
  #include "lcd/ULTIMAKERCONTROLLER.h"
#elif ENABLED(ULTIPANEL)
  #include "lcd/ULTIPANEL.h"
#elif ENABLED(PANEL_ONE)
  #include "lcd/PANEL_ONE.h"
#elif ENABLED(G3D_PANEL)
  #include "lcd/G3D_PANEL.h"
#elif ENABLED(RIGIDBOT_PANEL)
  #include "lcd/RIGIDBOT_PANEL.h"
#elif ENABLED(MAKEBOARD_MINI_2_LINE_DISPLAY_1602)
  #include "lcd/MAKEBOARD_MINI_2_LINE_DISPLAY_1602.h"
#elif ENABLED(ZONESTAR_LCD)
  #include "lcd/ZONESTAR_LCD.h"
#elif ENABLED(ULTRA_LCD)
  #include "lcd/ULTRA_LCD.h"
#elif ENABLED(RA_CONTROL_PANEL)
  #include "lcd/RA_CONTROL_PANEL.h"
#elif ENABLED(LCD_SAINSMART_I2C_1602)
  #include "lcd/LCD_SAINSMART_I2C_1602.h"
#elif ENABLED(LCD_SAINSMART_I2C_2004)
  #include "lcd/LCD_SAINSMART_I2C_2004.h"
#elif ENABLED(LCM1602)
  #include "lcd/LCM1602.h"
#elif ENABLED(LCD_I2C_PANELOLU2)
  #include "lcd/LCD_I2C_PANELOLU2.h"
#elif ENABLED(LCD_I2C_VIKI)
  #include "lcd/LCD_I2C_VIKI.h"
#elif ENABLED(SAV_3DLCD)
  #include "lcd/SAV_3DLCD.h"
#elif ENABLED(FF_INTERFACEBOARD)
  #include "lcd/FF_INTERFACEBOARD.h"
#elif ENABLED(TFTGLCD_PANEL_SPI)
  #include "lcd/TFTGLCD_PANEL_SPI.h"
#elif ENABLED(TFTGLCD_PANEL_I2C)
  #include "lcd/TFTGLCD_PANEL_I2C.h"
#elif ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)
  #include "lcd/REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER.h"
#elif ENABLED(K3D_FULL_GRAPHIC_SMART_CONTROLLER)
  #include "lcd/K3D_FULL_GRAPHIC_SMART_CONTROLLER.h"
#elif ENABLED(REPRAPWORLD_GRAPHICAL_LCD)
  #include "lcd/REPRAPWORLD_GRAPHICAL_LCD.h"
#elif ENABLED(VIKI2)
  #include "lcd/VIKI2.h"
#elif ENABLED(miniVIKI)
  #include "lcd/miniVIKI.h"
#elif ENABLED(WYH_L12864)
  #include "lcd/WYH_L12864.h"
#elif ENABLED(MINIPANEL)
  #include "lcd/MINIPANEL.h"
#elif ENABLED(MAKRPANEL)
  #include "lcd/MAKRPANEL.h"
#elif ENABLED(ELB_FULL_GRAPHIC_CONTROLLER)
  #include "lcd/ELB_FULL_GRAPHIC_CONTROLLER.h"
#elif ENABLED(BQ_LCD_SMART_CONTROLLER)
  #include "lcd/BQ_LCD_SMART_CONTROLLER.h"
#elif ENABLED(CARTESIO_UI)
  #include "lcd/CARTESIO_UI.h"
#elif ENABLED(LCD_FOR_MELZI)
  #include "lcd/LCD_FOR_MELZI.h"
#elif ENABLED(ULTI_CONTROLLER)
  #include "lcd/ULTI_CONTROLLER.h"
#elif ENABLED(MKS_MINI_12864)
  #include "lcd/MKS_MINI_12864.h"
#elif ENABLED(MKS_MINI_12864_V3)
  #include "lcd/MKS_MINI_12864_V3.h"
#elif ENABLED(MKS_LCD12864A)
  #include "lcd/MKS_LCD12864A.h"
#elif ENABLED(MKS_LCD12864B)
  #include "lcd/MKS_LCD12864B.h"
#elif ENABLED(FYSETC_MINI_12864_X_X)
  #include "lcd/FYSETC_MINI_12864_X_X.h"
#elif ENABLED(FYSETC_MINI_12864_1_2)
  #include "lcd/FYSETC_MINI_12864_1_2.h"
#elif ENABLED(FYSETC_MINI_12864_2_0)
  #include "lcd/FYSETC_MINI_12864_2_0.h"
#elif ENABLED(FYSETC_MINI_12864_2_1)
  #include "lcd/FYSETC_MINI_12864_2_1.h"
#elif ENABLED(FYSETC_GENERIC_12864_1_1)
  #include "lcd/FYSETC_GENERIC_12864_1_1.h"
#elif ENABLED(BTT_MINI_12864_V1)
  #include "lcd/BTT_MINI_12864_V1.h"
#elif ENABLED(CR10_STOCKDISPLAY)
  #include "lcd/CR10_STOCKDISPLAY.h"
#elif ENABLED(ENDER2_STOCKDISPLAY)
  #include "lcd/ENDER2_STOCKDISPLAY.h"
#elif ENABLED(ANET_FULL_GRAPHICS_LCD)
  #include "lcd/ANET_FULL_GRAPHICS_LCD.h"
#elif ENABLED(ANET_FULL_GRAPHICS_LCD_ALT_WIRING)
  #include "lcd/ANET_FULL_GRAPHICS_LCD_ALT_WIRING.h"
#elif ENABLED(AZSMZ_12864)
  #include "lcd/AZSMZ_12864.h"
#elif ENABLED(SILVER_GATE_GLCD_CONTROLLER)
  #include "lcd/SILVER_GATE_GLCD_CONTROLLER.h"
#elif ENABLED(EMOTION_TECH_LCD)
  #include "lcd/EMOTION_TECH_LCD.h"
#elif ENABLED(LCD_U8GLIB_SSD1306)
  #include "lcd/LCD_U8GLIB_SSD1306.h"
#elif ENABLED(SAV_3DGLCD)
  #include "lcd/SAV_3DGLCD.h"
#elif ENABLED(OLED_PANEL_TINYBOY2)
  #include "lcd/OLED_PANEL_TINYBOY2.h"
#elif ENABLED(MKS_12864OLED)
  #include "lcd/MKS_12864OLED.h"
#elif ENABLED(MKS_12864OLED_SSD1306)
  #include "lcd/MKS_12864OLED_SSD1306.h"
#elif ENABLED(ZONESTAR_12864LCD)
  #include "lcd/ZONESTAR_12864LCD.h"
#elif ENABLED(ZONESTAR_12864OLED)
  #include "lcd/ZONESTAR_12864OLED.h"
#elif ENABLED(ZONESTAR_12864OLED_SSD1306)
  #include "lcd/ZONESTAR_12864OLED_SSD1306.h"
#elif ENABLED(U8GLIB_SH1106_EINSTART)
  #include "lcd/U8GLIB_SH1106_EINSTART.h"
#elif ENABLED(OVERLORD_OLED)
  #include "lcd/OVERLORD_OLED.h"
#elif ENABLED(FYSETC_242_OLED_12864)
  #include "lcd/FYSETC_242_OLED_12864.h"
#elif ENABLED(K3D_242_OLED_CONTROLLER)
  #include "lcd/K3D_242_OLED_CONTROLLER.h"
#elif ENABLED(DGUS_LCD_UI_ORIGIN)
  #include "lcd/DGUS_LCD_UI_ORIGIN.h"
#elif ENABLED(DGUS_LCD_UI_FYSETC)
  #include "lcd/DGUS_LCD_UI_FYSETC.h"
#elif ENABLED(DGUS_LCD_UI_HIPRECY)
  #include "lcd/DGUS_LCD_UI_HIPRECY.h"
#elif ENABLED(DGUS_LCD_UI_MKS)
  #include "lcd/DGUS_LCD_UI_MKS.h"
#elif ENABLED(DGUS_LCD_UI_RELOADED)
  #include "lcd/DGUS_LCD_UI_RELOADED.h"
#elif ENABLED(DWIN_CREALITY_TOUCHLCD)
  #include "lcd/DWIN_CREALITY_TOUCHLCD.h"
#elif ENABLED(MALYAN_LCD)
  #include "lcd/MALYAN_LCD.h"
#elif ENABLED(TOUCH_UI_FTDI_EVE)
  #include "lcd/TOUCH_UI_FTDI_EVE.h"
#elif ENABLED(ANYCUBIC_LCD_I3MEGA)
  #include "lcd/ANYCUBIC_LCD_I3MEGA.h"
#elif ENABLED(ANYCUBIC_LCD_CHIRON)
  #include "lcd/ANYCUBIC_LCD_CHIRON.h"
#elif ENABLED(NEXTION_TFT)
  #include "lcd/NEXTION_TFT.h"
#elif ENABLED(PANELDUE)
  #include "lcd/PANELDUE.h"
#elif ENABLED(EXTENSIBLE_UI)
  #include "lcd/EXTENSIBLE_UI.h"
#elif ENABLED(MKS_TS35_V2_0)
  #include "lcd/MKS_TS35_V2_0.h"
#elif ENABLED(MKS_ROBIN_TFT24)
  #include "lcd/MKS_ROBIN_TFT24.h"
#elif ENABLED(MKS_ROBIN_TFT28)
  #include "lcd/MKS_ROBIN_TFT28.h"
#elif ENABLED(MKS_ROBIN_TFT32)
  #include "lcd/MKS_ROBIN_TFT32.h"
#elif ENABLED(MKS_ROBIN_TFT35)
  #include "lcd/MKS_ROBIN_TFT35.h"
#elif ENABLED(MKS_ROBIN_TFT43)
  #include "lcd/MKS_ROBIN_TFT43.h"
#elif ENABLED(MKS_ROBIN_TFT_V1_1R)
  #include "lcd/MKS_ROBIN_TFT_V1_1R.h"
#elif ENABLED(TFT_TRONXY_X5SA)
  #include "lcd/TFT_TRONXY_X5SA.h"
#elif ENABLED(ANYCUBIC_TFT35)
  #include "lcd/ANYCUBIC_TFT35.h"
#elif ENABLED(LONGER_LK_TFT28)
  #include "lcd/LONGER_LK_TFT28.h"
#elif ENABLED(ANET_ET4_TFT28)
  #include "lcd/ANET_ET4_TFT28.h"
#elif ENABLED(ANET_ET5_TFT35)
  #include "lcd/ANET_ET5_TFT35.h"
#elif ENABLED(BIQU_BX_TFT70)
  #include "lcd/BIQU_BX_TFT70.h"
#elif ENABLED(BTT_TFT35_SPI_V1_0)
  #include "lcd/BTT_TFT35_SPI_V1_0.h"
#elif ENABLED(TFT_GENERIC)
  #include "lcd/TFT_GENERIC.h"
#elif ENABLED(TFT_CLASSIC_UI)
  #include "lcd/TFT_CLASSIC_UI.h"
#elif ENABLED(TFT_COLOR_UI)
  #include "lcd/TFT_COLOR_UI.h"
#elif ENABLED(TFT_LVGL_UI)
  #include "lcd/TFT_LVGL_UI.h"
#elif ANY(DWIN_CREALITY_LCD, DWIN_LCD_PROUI, DWIN_MARLINUI_PORTRAIT, DWIN_MARLINUI_LANDSCAPE)
  #include "lcd/DWIN_CREALITY_LCD.h"
#endif

// Button panels
#if ENABLED(REPRAPWORLD_KEYPAD)
  #include "lcd/REPRAPWORLD_KEYPAD.h"
#elif ENABLED(EASYTHREED_UI)
  #include "lcd/EASYTHREED_UI.h"
#endif
