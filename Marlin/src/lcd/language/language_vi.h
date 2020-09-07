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
 * Vietnamese
 *
 * LCD Menu Messages
 * See also https://marlinfw.org/docs/development/lcd_language.html
 *
 */
namespace Language_vi {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t    CHARSIZE                            = 2;
  PROGMEM Language_Str LANGUAGE                            = _UxGT("Vietnamese");

  PROGMEM Language_Str WELCOME_MSG                         = MACHINE_NAME _UxGT(" Sẵn sàng.");              // Ready
  PROGMEM Language_Str MSG_BACK                            = _UxGT("Trở lại");                              // Back
  PROGMEM Language_Str MSG_MEDIA_ABORTING                  = _UxGT("Đang hủy bỏ...");
  PROGMEM Language_Str MSG_MEDIA_INSERTED                  = _UxGT("Phương tiện được cắm vào");             // Media inserted
  PROGMEM Language_Str MSG_MEDIA_REMOVED                   = _UxGT("Phương tiện được rút ra");
  PROGMEM Language_Str MSG_MEDIA_WAITING                   = _UxGT("Chờ đợi phương tiện");
  PROGMEM Language_Str MSG_MEDIA_READ_ERROR                = _UxGT("Lỗi đọc phương tiện");
  PROGMEM Language_Str MSG_MEDIA_USB_REMOVED               = _UxGT("USB được rút ra");
  PROGMEM Language_Str MSG_MEDIA_USB_FAILED                = _UxGT("USB khởi thất bại");
  PROGMEM Language_Str MSG_LCD_ENDSTOPS                    = _UxGT("Công tắc");                             // Endstops - công tắc hành trình
  PROGMEM Language_Str MSG_LCD_SOFT_ENDSTOPS               = _UxGT("Công tắc mềm");                         // soft Endstops
  PROGMEM Language_Str MSG_MAIN                            = _UxGT("Chính");                                // Main
  PROGMEM Language_Str MSG_ADVANCED_SETTINGS               = _UxGT("Thiết lập cấp cao");                    // Advanced Settings
  PROGMEM Language_Str MSG_CONFIGURATION                   = _UxGT("Cấu hình");                             // Configuration
  PROGMEM Language_Str MSG_AUTOSTART                       = _UxGT("Khởi chạy tự động");                    // Autostart
  PROGMEM Language_Str MSG_DISABLE_STEPPERS                = _UxGT("Tắt động cơ bước");                     // Disable steppers
  PROGMEM Language_Str MSG_DEBUG_MENU                      = _UxGT("Menu gỡ lỗi");                          // Debug Menu
  PROGMEM Language_Str MSG_PROGRESS_BAR_TEST               = _UxGT("Kiểm tra tiến độ");                     // Progress bar test
  PROGMEM Language_Str MSG_AUTO_HOME                       = _UxGT("Về nhà tự động");                       // Auto home
  PROGMEM Language_Str MSG_AUTO_HOME_X                     = _UxGT("Về nhà X");                             // home x
  PROGMEM Language_Str MSG_AUTO_HOME_Y                     = _UxGT("Về nhà Y");                             // home y
  PROGMEM Language_Str MSG_AUTO_HOME_Z                     = _UxGT("Về nhà Z");
  PROGMEM Language_Str MSG_AUTO_Z_ALIGN                    = _UxGT("Chỉnh canh Z tự động");
  PROGMEM Language_Str MSG_LEVEL_BED_HOMING                = _UxGT("Đang về nhà XYZ");                      // Homing XYZ
  PROGMEM Language_Str MSG_LEVEL_BED_WAITING               = _UxGT("Nhấn để bắt đầu");                      // Click to Begin
  PROGMEM Language_Str MSG_LEVEL_BED_NEXT_POINT            = _UxGT("Điểm tiếp theo");                       // Next Point
  PROGMEM Language_Str MSG_LEVEL_BED_DONE                  = _UxGT("San lấp được hoàn thành");              // Leveling Done!
  PROGMEM Language_Str MSG_Z_FADE_HEIGHT                   = _UxGT("Chiều cao mờ dần");                     // Fade Height
  PROGMEM Language_Str MSG_SET_HOME_OFFSETS                = _UxGT("Đặt bù đắp nhà");                       // Set home offsets
  PROGMEM Language_Str MSG_HOME_OFFSETS_APPLIED            = _UxGT("Bù đắp được áp dụng");                  // Offsets applied
  PROGMEM Language_Str MSG_SET_ORIGIN                      = _UxGT("Đặt nguồn gốc");                        // Set origin
  #if PREHEAT_COUNT
    PROGMEM Language_Str MSG_PREHEAT_1                     = _UxGT("Làm nóng ") PREHEAT_1_LABEL _UxGT(" trước");      // Preheat
    PROGMEM Language_Str MSG_PREHEAT_1_H                   = _UxGT("Làm nóng ") PREHEAT_1_LABEL _UxGT(" trước ~");    // Preheat
    PROGMEM Language_Str MSG_PREHEAT_1_END                 = _UxGT("Làm nóng ") PREHEAT_1_LABEL _UxGT(" Đầu");
    PROGMEM Language_Str MSG_PREHEAT_1_END_E               = _UxGT("Làm nóng ") PREHEAT_1_LABEL _UxGT(" Đầu ~");
    PROGMEM Language_Str MSG_PREHEAT_1_ALL                 = _UxGT("Làm nóng ") PREHEAT_1_LABEL _UxGT(" Tất cả");     // all
    PROGMEM Language_Str MSG_PREHEAT_1_BEDONLY             = _UxGT("Làm nóng ") PREHEAT_1_LABEL _UxGT(" Bàn");        // bed -- using vietnamese term for 'table' instead
    PROGMEM Language_Str MSG_PREHEAT_1_SETTINGS            = _UxGT("Làm nóng ") PREHEAT_1_LABEL _UxGT(" Cấu hình");   // conf

    PROGMEM Language_Str MSG_PREHEAT_M                     = _UxGT("Làm nóng $ trước");      // Preheat
    PROGMEM Language_Str MSG_PREHEAT_M_H                   = _UxGT("Làm nóng $ trước ~");    // Preheat
    PROGMEM Language_Str MSG_PREHEAT_M_END                 = _UxGT("Làm nóng $ Đầu");
    PROGMEM Language_Str MSG_PREHEAT_M_END_E               = _UxGT("Làm nóng $ Đầu ~");
    PROGMEM Language_Str MSG_PREHEAT_M_ALL                 = _UxGT("Làm nóng $ Tất cả");     // all
    PROGMEM Language_Str MSG_PREHEAT_M_BEDONLY             = _UxGT("Làm nóng $ Bàn");        // bed -- using vietnamese term for 'table' instead
    PROGMEM Language_Str MSG_PREHEAT_M_SETTINGS            = _UxGT("Làm nóng $ Cấu hình");   // conf
  #endif
  PROGMEM Language_Str MSG_PREHEAT_CUSTOM                  = _UxGT("Sự nóng trước tự chọn");                // Preheat Custom
  PROGMEM Language_Str MSG_COOLDOWN                        = _UxGT("Nguội xuống");                          // Cooldown
  PROGMEM Language_Str MSG_SWITCH_PS_ON                    = _UxGT("Bật nguồn");                            // Switch power on
  PROGMEM Language_Str MSG_SWITCH_PS_OFF                   = _UxGT("Tắt nguồn");                            // Switch power off
  PROGMEM Language_Str MSG_EXTRUDE                         = _UxGT("Ép đùn");                               // Extrude
  PROGMEM Language_Str MSG_RETRACT                         = _UxGT("Rút lại");                              // Retract
  PROGMEM Language_Str MSG_MOVE_AXIS                       = _UxGT("Di chuyển trục");                       // Move axis
  PROGMEM Language_Str MSG_BED_LEVELING                    = _UxGT("San Lấp Bàn");                          // Bed Leveling
  PROGMEM Language_Str MSG_LEVEL_BED                       = _UxGT("Làm bằng mặt bàn");                     // Level bed
  PROGMEM Language_Str MSG_LEVEL_CORNERS                   = _UxGT("Làm bằng góc bàn");                     // Level corners
  PROGMEM Language_Str MSG_NEXT_CORNER                     = _UxGT("Góc tiếp theo");                        // Next corner
  PROGMEM Language_Str MSG_EDITING_STOPPED                 = _UxGT("Chỉnh lưới đã dừng");                   // Mesh Editing Stopped
  PROGMEM Language_Str MSG_MESH_X                          = _UxGT("Mục lục X");                            // Index X
  PROGMEM Language_Str MSG_MESH_Y                          = _UxGT("Mục lục Y");
  PROGMEM Language_Str MSG_MESH_EDIT_Z                     = _UxGT("Giá trị Z");                            // Z Value
  PROGMEM Language_Str MSG_USER_MENU                       = _UxGT("Các lệnh tự chọn");                     // Custom Commands
  PROGMEM Language_Str MSG_UBL_DOING_G29                   = _UxGT("Đang chạy G29");                        // Doing G29
  PROGMEM Language_Str MSG_UBL_TOOLS                       = _UxGT("Công cụ UBL");                          // UBL tools
  PROGMEM Language_Str MSG_UBL_LEVEL_BED                   = _UxGT("San Lấp Bàn Thống Nhất (UBL)");         // Unified Bed Leveling
  PROGMEM Language_Str MSG_IDEX_MENU                       = _UxGT("chế độ IDEX");                          // IDEX Mode
  PROGMEM Language_Str MSG_IDEX_MODE_AUTOPARK              = _UxGT("Đậu tự động");                          // Auto-Park
  PROGMEM Language_Str MSG_IDEX_MODE_DUPLICATE             = _UxGT("Sự gấp đôi");                           // Duplication
  PROGMEM Language_Str MSG_IDEX_MODE_SCALED_COPY           = _UxGT("Bản sao thu nhỏ");
  PROGMEM Language_Str MSG_IDEX_MODE_FULL_CTRL             = _UxGT("Toàn quyền điều khiển");                // Full control
  PROGMEM Language_Str MSG_IDEX_X_OFFSET                   = _UxGT("Đầu phun X nhì");                       // 2nd nozzle X
  PROGMEM Language_Str MSG_IDEX_Y_OFFSET                   = _UxGT("Đầu phun Y nhì");
  PROGMEM Language_Str MSG_IDEX_Z_OFFSET                   = _UxGT("Đầu phun Z nhì");
  PROGMEM Language_Str MSG_IDEX_SAVE_OFFSETS               = _UxGT("Lưu bù đắp");                           // Save offsets
  PROGMEM Language_Str MSG_UBL_MANUAL_MESH                 = _UxGT("Tự xây dựng lưới");                     // Manually Build Mesh
  PROGMEM Language_Str MSG_UBL_BC_INSERT                   = _UxGT("Đặt chêm và đo");                       // Place shim & measure
  PROGMEM Language_Str MSG_UBL_BC_INSERT2                  = _UxGT("Đo");                                   // Measure
  PROGMEM Language_Str MSG_UBL_BC_REMOVE                   = _UxGT("Tháo và đo bàn");                       // Remove & measure bed
  PROGMEM Language_Str MSG_UBL_MOVING_TO_NEXT              = _UxGT("Chuyển sang tiếp theo");                // moving to next
  PROGMEM Language_Str MSG_UBL_ACTIVATE_MESH               = _UxGT("Bật UBL");
  PROGMEM Language_Str MSG_UBL_DEACTIVATE_MESH             = _UxGT("Tắt UBL");
  PROGMEM Language_Str MSG_UBL_SET_TEMP_BED                = _UxGT("Nhiệt độ bàn");                         // Bed Temp
  PROGMEM Language_Str MSG_UBL_BED_TEMP_CUSTOM             = _UxGT("Bed Temp");
  PROGMEM Language_Str MSG_UBL_SET_TEMP_HOTEND             = _UxGT("Nhiệt độ đầu phun");                    // Hotend Temp
  PROGMEM Language_Str MSG_UBL_HOTEND_TEMP_CUSTOM          = _UxGT("Hotend Temp");
  PROGMEM Language_Str MSG_UBL_MESH_EDIT                   = _UxGT("Chỉnh sửa lưới");                       // Mesh Edit
  PROGMEM Language_Str MSG_UBL_EDIT_CUSTOM_MESH            = _UxGT("Chỉnh sửa lưới tự chọn");               // Edit Custom Mesh
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_MESH              = _UxGT("Chỉnh lưới chính xác");                 // Fine tuning mesh
  PROGMEM Language_Str MSG_UBL_DONE_EDITING_MESH           = _UxGT("Chỉnh sửa xong lưới");                  // Done Editing Mesh
  PROGMEM Language_Str MSG_UBL_BUILD_CUSTOM_MESH           = _UxGT("Xây dựng lưới tự chọn");                // Build Custom Mesh
  PROGMEM Language_Str MSG_UBL_BUILD_MESH_MENU             = _UxGT("Xây dựng lưới");                        // Build Mesh
  #if PREHEAT_COUNT
    PROGMEM Language_Str MSG_UBL_BUILD_MESH_M              = _UxGT("Xây dựng lưới ($)");
    PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_M           = _UxGT("Thẩm tra lưới ($)");
  #endif
  PROGMEM Language_Str MSG_UBL_BUILD_COLD_MESH             = _UxGT("Xây dựng lưới lạnh");                   // Build cold mesh
  PROGMEM Language_Str MSG_UBL_MESH_HEIGHT_ADJUST          = _UxGT("Điều chỉnh chiều cao lưới");            // Adjust Mesh Height
  PROGMEM Language_Str MSG_UBL_MESH_HEIGHT_AMOUNT          = _UxGT("Số lượng chiều cao");                   // Height Amount
  PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_MENU          = _UxGT("Thẩm tra lưới");                        // Validate Mesh
  PROGMEM Language_Str MSG_UBL_VALIDATE_CUSTOM_MESH        = _UxGT("Thẩm tra lưới tự chọn");                // validate custom mesh
  PROGMEM Language_Str MSG_UBL_CONTINUE_MESH               = _UxGT("Tiếp tục xây lưới bàn");                // Continue Bed Mesh
  PROGMEM Language_Str MSG_UBL_MESH_LEVELING               = _UxGT("Đang san lấp lưới");                    // Mesh Leveling
  PROGMEM Language_Str MSG_UBL_3POINT_MESH_LEVELING        = _UxGT("Đang san lấp 3-điểm");                  // 3-Point Leveling
  PROGMEM Language_Str MSG_UBL_GRID_MESH_LEVELING          = _UxGT("Đang san lấp lưới phẳng");              // Grid (planar) Mesh Leveling
  PROGMEM Language_Str MSG_UBL_MESH_LEVEL                  = _UxGT("Làm bằng lưới");                        // Level Mesh
  PROGMEM Language_Str MSG_UBL_SIDE_POINTS                 = _UxGT("Điểm bên cạnh");                        // Side Points
  PROGMEM Language_Str MSG_UBL_MAP_TYPE                    = _UxGT("Loại bản đồ");                          // Map Type
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP                  = _UxGT("Đầu ra bản đồ lưới");                   // Output Mesh Map
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_HOST             = _UxGT("Đầu ra cho máy chủ");                   // Output for Host
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_CSV              = _UxGT("Đầu ra cho CSV");                       // Output for CSV
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_BACKUP           = _UxGT("Hỗ trợ lưới");                          // Off Printer Backup | backup mesh
  PROGMEM Language_Str MSG_UBL_INFO_UBL                    = _UxGT("Đầu ra thông tin UBL");                 // Output UBL Info
  PROGMEM Language_Str MSG_EDIT_MESH                       = _UxGT("Chỉnh sửa lưới");                       // Edit mesh
  PROGMEM Language_Str MSG_UBL_FILLIN_AMOUNT               = _UxGT("Số lượng lấp đầy");                     // Fill-in Amount
  PROGMEM Language_Str MSG_UBL_MANUAL_FILLIN               = _UxGT("Tự lấp đầy");                           // Manual Fill-in
  PROGMEM Language_Str MSG_UBL_SMART_FILLIN                = _UxGT("Lấp đầy thông minh");                   // Smart Fill-in
  PROGMEM Language_Str MSG_UBL_FILLIN_MESH                 = _UxGT("Lưới lấp đầy");                         // Fill-in Mesh
  PROGMEM Language_Str MSG_UBL_INVALIDATE_ALL              = _UxGT("Bác bỏ tất cả");                       // Invalidate All
  PROGMEM Language_Str MSG_UBL_INVALIDATE_CLOSEST          = _UxGT("Bác bỏ gần nhất");                     // Invalidate Closest
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_ALL               = _UxGT("Chỉnh chính xác tất cả");               // Fine Tune ALl
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_CLOSEST           = _UxGT("Chỉnh chính xác gần nhất");             // Fine Tune Closest
  PROGMEM Language_Str MSG_UBL_STORAGE_MESH_MENU           = _UxGT("Lưu trữ lưới");                         // Mesh Storage
  PROGMEM Language_Str MSG_UBL_STORAGE_SLOT                = _UxGT("Khe nhớ");                              // Memory Slot
  PROGMEM Language_Str MSG_UBL_LOAD_MESH                   = _UxGT("Tải lưới bàn");                         // Load Bed Mesh
  PROGMEM Language_Str MSG_UBL_SAVE_MESH                   = _UxGT("Lưu lưới bàn");                         // Save Bed Mesh
  PROGMEM Language_Str MSG_MESH_LOADED                     = _UxGT("%i lưới được nạp");                // Mesh %i loaded
  PROGMEM Language_Str MSG_MESH_SAVED                      = _UxGT("%i lưới đã lưu");
  PROGMEM Language_Str MSG_NO_STORAGE                      = _UxGT("Không lưu trữ");                        // No Storage
  PROGMEM Language_Str MSG_UBL_SAVE_ERROR                  = _UxGT("Điều sai: Lưu UBL");                    // Err: UBL Save
  PROGMEM Language_Str MSG_UBL_RESTORE_ERROR               = _UxGT("Điều Sai: Khôi Phục UBL");              // Err: UBL Restore
  PROGMEM Language_Str MSG_UBL_Z_OFFSET_STOPPED            = _UxGT("Đầu Dò-Z Đã Ngừng");                    // Z-Offset Stopped
  PROGMEM Language_Str MSG_UBL_STEP_BY_STEP_MENU           = _UxGT("Bước-Từng-Bước UBL");                   // Step-By-Step UBL
  PROGMEM Language_Str MSG_UBL_1_BUILD_COLD_MESH           = _UxGT("1.Xây dựng lưới lạnh");
  PROGMEM Language_Str MSG_UBL_2_SMART_FILLIN              = _UxGT("2.Lấp đầy thông minh");
  PROGMEM Language_Str MSG_UBL_3_VALIDATE_MESH_MENU        = _UxGT("3.Thẩm tra lưới");
  PROGMEM Language_Str MSG_UBL_4_FINE_TUNE_ALL             = _UxGT("4.Chỉnh chính xác tất cả");
  PROGMEM Language_Str MSG_UBL_5_VALIDATE_MESH_MENU        = _UxGT("5.Thẩm tra lưới");
  PROGMEM Language_Str MSG_UBL_6_FINE_TUNE_ALL             = _UxGT("6.Chỉnh chính xác tất cả");
  PROGMEM Language_Str MSG_UBL_7_SAVE_MESH                 = _UxGT("7.Lưu lưới bàn");

  PROGMEM Language_Str MSG_LED_CONTROL                     = _UxGT("Điều khiển LED");                       // LED Control
  PROGMEM Language_Str MSG_LEDS                            = _UxGT("Đèn");                                  // Lights
  PROGMEM Language_Str MSG_LED_PRESETS                     = _UxGT("Đèn định sẵn");                         // Light Presets
  PROGMEM Language_Str MSG_SET_LEDS_RED                    = _UxGT("Đỏ");                                   // Red
  PROGMEM Language_Str MSG_SET_LEDS_ORANGE                 = _UxGT("Cam");                                  // Orange
  PROGMEM Language_Str MSG_SET_LEDS_YELLOW                 = _UxGT("Vàng");                                 // Yellow
  PROGMEM Language_Str MSG_SET_LEDS_GREEN                  = _UxGT("Xanh Lá");                              // Green
  PROGMEM Language_Str MSG_SET_LEDS_BLUE                   = _UxGT("Xanh");                                 // Blue
  PROGMEM Language_Str MSG_SET_LEDS_INDIGO                 = _UxGT("Xanh Đậm");                             // Indigo
  PROGMEM Language_Str MSG_SET_LEDS_VIOLET                 = _UxGT("Tím");                                  // Violet
  PROGMEM Language_Str MSG_SET_LEDS_WHITE                  = _UxGT("Trắng");                                // White
  PROGMEM Language_Str MSG_SET_LEDS_DEFAULT                = _UxGT("Mặc định");                             // Default
  PROGMEM Language_Str MSG_CUSTOM_LEDS                     = _UxGT("Đèn Tự Chọn");                          // Custom Lights
  PROGMEM Language_Str MSG_INTENSITY_R                     = _UxGT("Cường Độ Đỏ");                          // Red Intensity
  PROGMEM Language_Str MSG_INTENSITY_G                     = _UxGT("Cường Độ Xanh Lá");                     // Green Intensity
  PROGMEM Language_Str MSG_INTENSITY_B                     = _UxGT("Cường Độ Xanh");                        // Blue Intensity
  PROGMEM Language_Str MSG_INTENSITY_W                     = _UxGT("Cường Độ Trắng");                       // White Intensity
  PROGMEM Language_Str MSG_LED_BRIGHTNESS                  = _UxGT("độ sáng");                              // Brightness

  PROGMEM Language_Str MSG_MOVING                          = _UxGT("Di chuyển...");                         // Moving
  PROGMEM Language_Str MSG_FREE_XY                         = _UxGT("Giải phóng XY");                        // Free XY
  PROGMEM Language_Str MSG_MOVE_X                          = _UxGT("Di chuyển X");                          // Move X
  PROGMEM Language_Str MSG_MOVE_Y                          = _UxGT("Di chuyển Y");
  PROGMEM Language_Str MSG_MOVE_Z                          = _UxGT("Di chuyển Z");
  PROGMEM Language_Str MSG_MOVE_E                          = _UxGT("Máy đùn");                              // Extruder
  PROGMEM Language_Str MSG_MOVE_EN                         = _UxGT("Máy đùn *");                            // Extruder
  PROGMEM Language_Str MSG_HOTEND_TOO_COLD                 = _UxGT("Đầu nóng quá lạnh");                    // Hotend too cold
  PROGMEM Language_Str MSG_MOVE_01MM                       = _UxGT("Di chuyển 0.1mm");                      // Move 0.1mm
  PROGMEM Language_Str MSG_MOVE_1MM                        = _UxGT("Di chuyển 1mm");                        // Move 1mm
  PROGMEM Language_Str MSG_MOVE_10MM                       = _UxGT("Di chuyển 10mm");                       // Move 10mm
  PROGMEM Language_Str MSG_SPEED                           = _UxGT("Tốc độ");                               // Speed
  PROGMEM Language_Str MSG_BED_Z                           = _UxGT("Z Bàn");
  PROGMEM Language_Str MSG_NOZZLE                          = _UxGT("Đầu phun");                             // Nozzle
  PROGMEM Language_Str MSG_NOZZLE_N                        = _UxGT("Đầu phun ~");                           // Nozzle
  PROGMEM Language_Str MSG_BED                             = _UxGT("Bàn");                                  // bed
  PROGMEM Language_Str MSG_FAN_SPEED                       = _UxGT("Tốc độ quạt");                          // fan speed
  PROGMEM Language_Str MSG_FAN_SPEED_N                     = _UxGT("Tốc độ quạt ~");                        // fan speed
  PROGMEM Language_Str MSG_EXTRA_FAN_SPEED                 = _UxGT("Tốc độ quạt phụ");                      // Extra fan speed
  PROGMEM Language_Str MSG_EXTRA_FAN_SPEED_N               = _UxGT("Tốc độ quạt phụ ~");                    // Extra fan speed
  PROGMEM Language_Str MSG_FLOW                            = _UxGT("Lưu Lượng");
  PROGMEM Language_Str MSG_FLOW_N                          = _UxGT("Lưu Lượng ~");
  PROGMEM Language_Str MSG_CONTROL                         = _UxGT("Điều khiển");                           // Control
  PROGMEM Language_Str MSG_MIN                             = " " LCD_STR_THERMOMETER _UxGT(" Đa");          // min
  PROGMEM Language_Str MSG_MAX                             = " " LCD_STR_THERMOMETER _UxGT(" Thiểu");
  PROGMEM Language_Str MSG_FACTOR                          = " " LCD_STR_THERMOMETER _UxGT(" Hệ Số");       // factor
  PROGMEM Language_Str MSG_AUTOTEMP                        = _UxGT("Nhiệt độ tự động");                     // Autotemp
  PROGMEM Language_Str MSG_LCD_ON                          = _UxGT("Bật");                                  // on
  PROGMEM Language_Str MSG_LCD_OFF                         = _UxGT("Tắt");                                  // off
  PROGMEM Language_Str MSG_SELECT                          = _UxGT("Lựa");                                  // Select
  PROGMEM Language_Str MSG_SELECT_E                        = _UxGT("Lựa *");
  PROGMEM Language_Str MSG_ACC                             = _UxGT("Tăng Tốc");
  PROGMEM Language_Str MSG_JERK                            = _UxGT("Giật");
  PROGMEM Language_Str MSG_VA_JERK                         = _UxGT("Giật-V") LCD_STR_A;
  PROGMEM Language_Str MSG_VB_JERK                         = _UxGT("Giật-V") LCD_STR_B;
  PROGMEM Language_Str MSG_VC_JERK                         = _UxGT("Giật-V") LCD_STR_C;
  PROGMEM Language_Str MSG_VE_JERK                         = _UxGT("Giật-Ve");
  PROGMEM Language_Str MSG_JUNCTION_DEVIATION              = _UxGT("Độ Lệch Chỗ Giao");                    // Junction Dev
  PROGMEM Language_Str MSG_VELOCITY                        = _UxGT("Vận tốc");                              // velocity
  PROGMEM Language_Str MSG_VMAX_A                          = _UxGT("Vđa") LCD_STR_A;                        // Vmax
  PROGMEM Language_Str MSG_VMAX_B                          = _UxGT("Vđa") LCD_STR_B;                        // Vmax
  PROGMEM Language_Str MSG_VMAX_C                          = _UxGT("Vđa") LCD_STR_C;                        // Vmax
  PROGMEM Language_Str MSG_VMAX_E                          = _UxGT("Vđa") LCD_STR_E;                        // Vmax
  PROGMEM Language_Str MSG_VMAX_EN                         = _UxGT("Vđa *");                                // Vmax
  PROGMEM Language_Str MSG_VMIN                            = _UxGT("Vthiểu");                               // Vmin
  PROGMEM Language_Str MSG_VTRAV_MIN                       = _UxGT("Vchuyển thiểu");                        // VTrav min
  PROGMEM Language_Str MSG_ACCELERATION                    = _UxGT("Sự tăng tốc");                          // Acceleration
  PROGMEM Language_Str MSG_AMAX_A                          = _UxGT("Tăng tốc ca") LCD_STR_A;                // Amax
  PROGMEM Language_Str MSG_AMAX_B                          = _UxGT("Tăng tốc ca") LCD_STR_B;                // Amax
  PROGMEM Language_Str MSG_AMAX_C                          = _UxGT("Tăng tốc ca") LCD_STR_C;                // Amax
  PROGMEM Language_Str MSG_AMAX_E                          = _UxGT("Tăng tốc ca") LCD_STR_E;                // Amax
  PROGMEM Language_Str MSG_AMAX_EN                         = _UxGT("Tăng tốc ca *");                        // Amax
  PROGMEM Language_Str MSG_A_RETRACT                       = _UxGT("TT-Rút");                               // A-retract
  PROGMEM Language_Str MSG_A_TRAVEL                        = _UxGT("TT-Chuyển");                            // A-travel
  PROGMEM Language_Str MSG_STEPS_PER_MM                    = _UxGT("Bước/mm");                              // Steps
  PROGMEM Language_Str MSG_A_STEPS                         = _UxGT("Bước") LCD_STR_A _UxGT("/mm");               // Asteps/mm
  PROGMEM Language_Str MSG_B_STEPS                         = _UxGT("Bước") LCD_STR_B _UxGT("/mm");
  PROGMEM Language_Str MSG_C_STEPS                         = _UxGT("Bước") LCD_STR_C _UxGT("/mm");
  PROGMEM Language_Str MSG_E_STEPS                         = _UxGT("BướcE/mm");
  PROGMEM Language_Str MSG_EN_STEPS                        = _UxGT("Bước */mm");
  PROGMEM Language_Str MSG_TEMPERATURE                     = _UxGT("Nhiệt độ");                             // Temperature
  PROGMEM Language_Str MSG_MOTION                          = _UxGT("Chuyển động");                          // Motion
  PROGMEM Language_Str MSG_FILAMENT                        = _UxGT("Vật liệu in");                          // dây nhựa
  PROGMEM Language_Str MSG_VOLUMETRIC_ENABLED              = _UxGT("E bằng mm³");                           // E in mm
  PROGMEM Language_Str MSG_FILAMENT_DIAM                   = _UxGT("Đường kính nhựa");                      // Fil. Dai.
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E                 = _UxGT("Đường kính nhựa *");
  PROGMEM Language_Str MSG_FILAMENT_UNLOAD                 = _UxGT("Dỡ mm");                                // unload mm
  PROGMEM Language_Str MSG_FILAMENT_LOAD                   = _UxGT("Nạp mm");
  PROGMEM Language_Str MSG_ADVANCE_K                       = _UxGT("K Cấp Cao");                            // Advance K
  PROGMEM Language_Str MSG_ADVANCE_K_E                     = _UxGT("K Cấp Cao *");                          // Advance K
  PROGMEM Language_Str MSG_CONTRAST                        = _UxGT("Độ tương phản LCD");                    // LCD contrast
  PROGMEM Language_Str MSG_STORE_EEPROM                    = _UxGT("Lưu các thiết lập");                    // Store settings
  PROGMEM Language_Str MSG_LOAD_EEPROM                     = _UxGT("Tải các cài đặt");                      // Load settings
  PROGMEM Language_Str MSG_RESTORE_DEFAULTS                = _UxGT("Khôi phục phòng hư");                   // Restore Defaults
  PROGMEM Language_Str MSG_INIT_EEPROM                     = _UxGT("Khởi Tạo EEPROM");                      // Initialize EEPROM
  PROGMEM Language_Str MSG_MEDIA_UPDATE                    = _UxGT("Cập Nhật phương tiện");                 // Update media
  PROGMEM Language_Str MSG_RESET_PRINTER                   = _UxGT("Bặt Lại Máy In");
  PROGMEM Language_Str MSG_REFRESH                         = LCD_STR_REFRESH  _UxGT("Cập Nhật");            // Refresh
  PROGMEM Language_Str MSG_INFO_SCREEN                     = _UxGT("Màn Hình Thông Tin");                   // Info screen
  PROGMEM Language_Str MSG_PREPARE                         = _UxGT("Chuẩn bị");                             // Prepare
  PROGMEM Language_Str MSG_TUNE                            = _UxGT("Điều Chỉnh");                           // Tune
  PROGMEM Language_Str MSG_PAUSE_PRINT                     = _UxGT("Tạm dừng in");                          // Pause print
  PROGMEM Language_Str MSG_RESUME_PRINT                    = _UxGT("Tiếp tục in");                          // Resume print
  PROGMEM Language_Str MSG_STOP_PRINT                      = _UxGT("Ngừng in");                             // Stop print
  PROGMEM Language_Str MSG_OUTAGE_RECOVERY                 = _UxGT("Phục Hồi Mất Điện");                    // Outage Recovery
  PROGMEM Language_Str MSG_MEDIA_MENU                      = _UxGT("In từ phương tiện");                    // Print from media
  PROGMEM Language_Str MSG_NO_MEDIA                        = _UxGT("Không có phương tiện");                 // No media
  PROGMEM Language_Str MSG_DWELL                           = _UxGT("Ngủ...");                               // Sleep
  PROGMEM Language_Str MSG_USERWAIT                        = _UxGT("Nhấn để tiếp tục...");                  // Click to resume (same as 'continue')
  PROGMEM Language_Str MSG_PRINT_PAUSED                    = _UxGT("In tạm dừng");                          // print paused
  PROGMEM Language_Str MSG_PRINTING                        = _UxGT("Đang in...");                           // printing
  PROGMEM Language_Str MSG_PRINT_ABORTED                   = _UxGT("In đã hủy bỏ");                         // Print aborted
  PROGMEM Language_Str MSG_NO_MOVE                         = _UxGT("Không di chuyển.");                     // No move.
  PROGMEM Language_Str MSG_KILLED                          = _UxGT("ĐÃ CHẾT. ");
  PROGMEM Language_Str MSG_STOPPED                         = _UxGT("ĐÃ NGỪNG. ");
  PROGMEM Language_Str MSG_CONTROL_RETRACT                 = _UxGT("Rút mm");                               // Retract mm
  PROGMEM Language_Str MSG_CONTROL_RETRACT_SWAP            = _UxGT("Rút Trao.mm");                          // Swap Re.mm
  PROGMEM Language_Str MSG_CONTROL_RETRACTF                = _UxGT("Rút V");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_ZHOP            = _UxGT("Nhảy mm");                              // hop
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER         = _UxGT("BỏRút mm");                             // UnRet mm
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAP    = _UxGT("BỏRút T mm");                           // S UnRet mm
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVERF        = _UxGT("BỏRút V");                              // UnRet V
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAPF   = _UxGT("BỏRút T V");                            // S UnRet V
  PROGMEM Language_Str MSG_AUTORETRACT                     = _UxGT("RútTựĐộng");                            // AutoRetr.
  PROGMEM Language_Str MSG_FILAMENT_SWAP_LENGTH            = _UxGT("Khoảng Cách Rút");                      // Retract Distance
  PROGMEM Language_Str MSG_TOOL_CHANGE                     = _UxGT("Thay Đổi Công Cụ");                     // Tool Change
  PROGMEM Language_Str MSG_TOOL_CHANGE_ZLIFT               = _UxGT("Đưa Lên Z");                            // Z Raise
  PROGMEM Language_Str MSG_SINGLENOZZLE_PRIME_SPEED        = _UxGT("Tốc Độ Tuôn Ra");                       // Prime Speed
  PROGMEM Language_Str MSG_SINGLENOZZLE_RETRACT_SPEED      = _UxGT("Tốc Độ Rút Lại");                       // Retract Speed
  PROGMEM Language_Str MSG_FILAMENTCHANGE                  = _UxGT("Thay dây nhựa");                        // change filament
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E                = _UxGT("Thay dây nhựa *");                      // change filament
  PROGMEM Language_Str MSG_FILAMENTLOAD                    = _UxGT("Nạp dây nhựa");                         // load filament
  PROGMEM Language_Str MSG_FILAMENTLOAD_E                  = _UxGT("Nạp dây nhựa *");                       // load filament
  PROGMEM Language_Str MSG_FILAMENTUNLOAD                  = _UxGT("Dỡ dây nhựa");                          // unload filament
  PROGMEM Language_Str MSG_FILAMENTUNLOAD_E                = _UxGT("Dỡ dây nhựa *");                        // unload filament
  PROGMEM Language_Str MSG_FILAMENTUNLOAD_ALL              = _UxGT("Dỡ tất cả");                            // Unload All
  PROGMEM Language_Str MSG_ATTACH_MEDIA                    = _UxGT("Khởi tạo phương tiện");                 // Attach media
  PROGMEM Language_Str MSG_CHANGE_MEDIA                    = _UxGT("Thay phương tiện");                     // Change midea
  PROGMEM Language_Str MSG_RELEASE_MEDIA                   = _UxGT("Phát hành phương tiện");
  PROGMEM Language_Str MSG_ZPROBE_OUT                      = _UxGT("Đầu Dò Z qua bàn");                     // Z Probe past bed
  PROGMEM Language_Str MSG_SKEW_FACTOR                     = _UxGT("Hệ số nghiêng");                        // Skew Factor
  PROGMEM Language_Str MSG_BLTOUCH                         = _UxGT("BLTOUCH");                              // BLTouch
  PROGMEM Language_Str MSG_BLTOUCH_SELFTEST                = _UxGT("Tự kiểm tra BLTOUCH ");                 // BLTouch Self-Test
  PROGMEM Language_Str MSG_BLTOUCH_RESET                   = _UxGT("Bặt lại BLTouch");                      // Reset BLTouch
  PROGMEM Language_Str MSG_BLTOUCH_DEPLOY                  = _UxGT("Đem BLTouch");                          // Deploy BLTouch
  PROGMEM Language_Str MSG_BLTOUCH_STOW                    = _UxGT("Cất BLTouch");                          // Stow BLTouch
  PROGMEM Language_Str MSG_MANUAL_DEPLOY                   = _UxGT("Đem Đầu Dò-Z");                         // Deploy Z-Probe
  PROGMEM Language_Str MSG_MANUAL_STOW                     = _UxGT("Cất Đầu Dò-Z");                         // Stow Z-Probe
  PROGMEM Language_Str MSG_HOME_FIRST                      = _UxGT("Về nhà %s%s%s Trước");
  PROGMEM Language_Str MSG_ZPROBE_ZOFFSET                  = _UxGT("Đầu Dò Bù Đắp Z");                      // Probe Z Offset
  PROGMEM Language_Str MSG_BABYSTEP_X                      = _UxGT("Nhít X");                               // Babystep X
  PROGMEM Language_Str MSG_BABYSTEP_Y                      = _UxGT("Nhít Y");
  PROGMEM Language_Str MSG_BABYSTEP_Z                      = _UxGT("Nhít Z");
  PROGMEM Language_Str MSG_ENDSTOP_ABORT                   = _UxGT("Hủy bỏ công tắc");                      // Endstop abort
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD              = _UxGT("Sưởi đầu phun không thành công");       // Heating failed
  PROGMEM Language_Str MSG_ERR_REDUNDANT_TEMP              = _UxGT("Điều sai: nhiệt độ dư");                // Err: REDUNDANT TEMP
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY                 = _UxGT("Vấn đề nhiệt");                         // THERMAL RUNAWAY | problem
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY_BED             = _UxGT("Vấn đề nhiệt bàn");                     // BED THERMAL RUNAWAY
  PROGMEM Language_Str MSG_ERR_MAXTEMP                     = _UxGT("Điều sai: nhiệt độ tối đa");            // Err: MAXTEMP
  PROGMEM Language_Str MSG_ERR_MINTEMP                     = _UxGT("Điều sai: nhiệt độ tối thiểu");         // Err: MINTEMP
  PROGMEM Language_Str MSG_ERR_MAXTEMP_BED                 = _UxGT("Điều sai: nhiệt độ bàn tối đa");        // Err: MAXTEMP BED
  PROGMEM Language_Str MSG_ERR_MINTEMP_BED                 = _UxGT("Điều sai: nhiệt độ bàn tối thiểu");     // Err: MINTEMP BED
  PROGMEM Language_Str MSG_HALTED                          = _UxGT("MÁY IN ĐÃ DỪNG LẠI");                   // PRINTER HALTED
  PROGMEM Language_Str MSG_PLEASE_RESET                    = _UxGT("Xin bặt lại");                          // Please reset
  PROGMEM Language_Str MSG_SHORT_DAY                       = _UxGT("n");                                    // d - ngày - One character only
  PROGMEM Language_Str MSG_SHORT_HOUR                      = _UxGT("g");                                    // h - giờ  - One character only
  PROGMEM Language_Str MSG_SHORT_MINUTE                    = _UxGT("p");                                    // m - phút - One character only
  PROGMEM Language_Str MSG_HEATING                         = _UxGT("Đang sưởi nóng...");                    // heating
  PROGMEM Language_Str MSG_COOLING                         = _UxGT("Đang làm nguội...");                    // cooling
  PROGMEM Language_Str MSG_BED_HEATING                     = _UxGT("Đang sưởi nong bàn...");                // bed heating
  PROGMEM Language_Str MSG_BED_COOLING                     = _UxGT("Đang làm nguội bàn...");                // bed cooling
  PROGMEM Language_Str MSG_DELTA_CALIBRATE                 = _UxGT("Cân Chỉnh Delta");                      // Delta calibration
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_X               = _UxGT("Chỉnh X lại");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Y               = _UxGT("Chỉnh Y lại");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Z               = _UxGT("Chỉnh Z lại");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_CENTER          = _UxGT("Chỉnh Z Center");                       // Calibrate Center
  PROGMEM Language_Str MSG_DELTA_SETTINGS                  = _UxGT("Cài Đặt Delta");                        // Delta Settings
  PROGMEM Language_Str MSG_DELTA_AUTO_CALIBRATE            = _UxGT("Cân Chỉnh Tự Động");                    // Auto Calibration
  PROGMEM Language_Str MSG_DELTA_HEIGHT_CALIBRATE          = _UxGT("Đặt Chiều Cao Delta");                  // Set Delta Height
  PROGMEM Language_Str MSG_DELTA_Z_OFFSET_CALIBRATE        = _UxGT("Đầu Dò Z-Bù Đắp");                      // Probe Z-offset
  PROGMEM Language_Str MSG_DELTA_DIAG_ROD                  = _UxGT("Gậy Chéo");                             // Diag Rod
  PROGMEM Language_Str MSG_DELTA_HEIGHT                    = _UxGT("Chiều Cao");                            // Height
  PROGMEM Language_Str MSG_DELTA_RADIUS                    = _UxGT("Bán Kính");                             // Radius
  PROGMEM Language_Str MSG_INFO_MENU                       = _UxGT("Về Máy In");
  PROGMEM Language_Str MSG_INFO_PRINTER_MENU               = _UxGT("Thông Tin Máy In");                     // Printer Info
  PROGMEM Language_Str MSG_3POINT_LEVELING                 = _UxGT("San lấp 3-Điểm");                       // 3-Point Leveling
  PROGMEM Language_Str MSG_LINEAR_LEVELING                 = _UxGT("San Lấp Tuyến Tính");                   // Linear Leveling
  PROGMEM Language_Str MSG_BILINEAR_LEVELING               = _UxGT("San Lấp Song Tuyến");                   // Bilinear Leveling
  PROGMEM Language_Str MSG_UBL_LEVELING                    = _UxGT("San Lấp Bàn Thống Nhất");               // Unified Bed Leveling
  PROGMEM Language_Str MSG_MESH_LEVELING                   = _UxGT("Lưới San Lấp");                         // Mesh Leveling
  PROGMEM Language_Str MSG_INFO_STATS_MENU                 = _UxGT("Thống Kê Máy In");                      // Printer Stats
  PROGMEM Language_Str MSG_INFO_BOARD_MENU                 = _UxGT("Thông Tin Bo Mạch");                    // Board Info
  PROGMEM Language_Str MSG_INFO_THERMISTOR_MENU            = _UxGT("Điện Trở Nhiệt");                       // Thermistors
  PROGMEM Language_Str MSG_INFO_EXTRUDERS                  = _UxGT("Máy đùn");                              // Extruders
  PROGMEM Language_Str MSG_INFO_BAUDRATE                   = _UxGT("Baud");                                 // Baud
  PROGMEM Language_Str MSG_INFO_PROTOCOL                   = _UxGT("Giao Thức");                            // Protocol
  PROGMEM Language_Str MSG_CASE_LIGHT                      = _UxGT("Đèn Khuông");                           // Case light
  PROGMEM Language_Str MSG_CASE_LIGHT_BRIGHTNESS           = _UxGT("Độ Sáng");                              // Light Brightness
  #if LCD_WIDTH >= 20
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("Số In");                                // Print Count
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("Đã hoàn thành");
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Tổng số thời gian in");                // Total print time
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("Thời gian việc lâu nhất");              // Longest job time
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("Tổng số đùn");                          // Extruded total
  #else
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("In");                                   // prints
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("Đã hoàn thành");                        // Completed
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Tổng số");                             // total
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("Dài nhất");                             // Longest
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("Đã ép đùn");
  #endif
  PROGMEM Language_Str MSG_INFO_MIN_TEMP                   = _UxGT("Nhiệt độ tối thiểu");                   // Min Temp
  PROGMEM Language_Str MSG_INFO_MAX_TEMP                   = _UxGT("Nhiệt độ tối đa");                      // Max temp
  PROGMEM Language_Str MSG_INFO_PSU                        = _UxGT("Bộ nguồn");                             // PSU
  PROGMEM Language_Str MSG_DRIVE_STRENGTH                  = _UxGT("Sức mạnh ổ đĩa");                       // Drive Strength
  PROGMEM Language_Str MSG_DAC_PERCENT_X                   = _UxGT("X % trình điều khiển");                 // X Driver %
  PROGMEM Language_Str MSG_DAC_PERCENT_Y                   = _UxGT("Y % trình điều khiển");                 // Y Driver %
  PROGMEM Language_Str MSG_DAC_PERCENT_Z                   = _UxGT("Z % trình điều khiển");                 // Z Driver %
  PROGMEM Language_Str MSG_DAC_PERCENT_E                   = _UxGT("E % trình điều khiển");                 // E Driver %
  PROGMEM Language_Str MSG_DAC_EEPROM_WRITE                = _UxGT("Ghi DAC EEPROM");                       // DAC EEPROM Write
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_PAUSE    = _UxGT("In tạm dừng");                          // PRINT PAUSED
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_LOAD     = _UxGT("Nạp dây nhựa");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_UNLOAD   = _UxGT("Dỡ dây nhựa");                          // unload filament
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_HEADER   = _UxGT("Tùy chọn hồi phục:");                   // RESUME OPTIONS
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_PURGE    = _UxGT("Xả thêm");                              // Purge more
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_RESUME   = _UxGT("Tiếp tục");                             // continue
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_NOZZLE          = _UxGT("  Đầu Phun: ");                         // Nozzle
  PROGMEM Language_Str MSG_RUNOUT_SENSOR_ENABLE            = _UxGT("Cảm Biến Hết");                         // Runout Sensor
  PROGMEM Language_Str MSG_KILL_HOMING_FAILED              = _UxGT("Sự nhà không thành công");              // Homing failed
  PROGMEM Language_Str MSG_LCD_PROBING_FAILED              = _UxGT(" không thành công");                    // Probing failed
  PROGMEM Language_Str MSG_M600_TOO_COLD                   = _UxGT("M600: Quá lạnh");

  //
  // Filament Change screens show up to 3 lines on a 4-line display
  //                        ...or up to 2 lines on a 3-line display
  //
  #if LCD_HEIGHT >= 4
    PROGMEM Language_Str MSG_ADVANCED_PAUSE_WAITING        = _UxGT(MSG_2_LINE("Nhấn nút", "để tiếp tục in")); // Press button to resume print
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_3_LINE("Chờ cho sự", "thay đổi dây nhựa", "bắt đầu")); // wait for filament change to start
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_3_LINE("Đút dây nhựa vào", "và nhấn nút", "để tiếp tục")); // insert filament and press button to continue                       //
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEAT          = _UxGT(MSG_2_LINE("Nhấn nút", "để làm nóng đầu phun")); // Press button to heat nozzle
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEATING       = _UxGT(MSG_2_LINE("Đầu phun đang nóng lên", "Xin chờ...")); // Nozzle heating Please wait
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_2_LINE("Chờ tro", "dây nhựa ra"));   // Wait for filament unload
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_2_LINE("Chờ tro", "dây nhựa vào"));  // Wait for filament load
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_PURGE         = _UxGT(MSG_2_LINE("Chờ tro", "xả dây nhựa"));   // wait for filament purge
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_CONT_PURGE    = _UxGT(MSG_2_LINE("Nhấn nút để kết thúc", "xả dây nhựa")); // Click to finish dây nhựa purge
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_2_LINE("Chờ tro in", "tiếp tục...")); // Wait for print to resume
  #else // LCD_HEIGHT < 4
    PROGMEM Language_Str MSG_ADVANCED_PAUSE_WAITING        = _UxGT(MSG_1_LINE("Nhấn nút để tiếp tục"));     // Click to continue
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_1_LINE("Xin chờ..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_1_LINE("Chèn và nhấn"));             // Insert and Click
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEAT          = _UxGT(MSG_1_LINE("Nhấn để sưởi"));             // Click to heat
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEATING       = _UxGT(MSG_1_LINE("Đang sưởi nóng"));           // Heating
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_1_LINE("Đang dỡ ra..."));            // Ejecting
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_1_LINE("Đang nạp..."));              // Loading
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_PURGE         = _UxGT(MSG_1_LINE("Đang xả..."));               // Purging
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_CONT_PURGE    = _UxGT(MSG_1_LINE("Nhấn nút để kết thúc"));     // Click to finish
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_1_LINE("Đang tiếp tục..."));         // Resuming
  #endif // LCD_HEIGHT < 4

  PROGMEM Language_Str MSG_TMC_DRIVERS                     = _UxGT("Trình điều khiển TMC");                 // TMC drivers
  PROGMEM Language_Str MSG_TMC_CURRENT                     = _UxGT("Dòng điện trình điều khiển");           // Driver current
  PROGMEM Language_Str MSG_TMC_HYBRID_THRS                 = _UxGT("Ngưỡng Hỗn Hợp");                       // Hybrid threshold
  PROGMEM Language_Str MSG_TMC_HOMING_THRS                 = _UxGT("Vô cảm biến");                          // Sensorless homing
  PROGMEM Language_Str MSG_TMC_STEPPING_MODE               = _UxGT("Chế độ từng bước");                     // Stepping mode
  PROGMEM Language_Str MSG_TMC_STEALTH_ENABLED             = _UxGT("CắtTàngHình được kích hoạt");           // StealthChop enabled
}
