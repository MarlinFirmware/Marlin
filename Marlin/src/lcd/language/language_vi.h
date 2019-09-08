/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * Vietnamese
 *
 * LCD Menu Messages
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */

#define CHARSIZE 2

#define THIS_LANGUAGES_SPECIAL_SYMBOLS      _UxGT("àạậẵắấầđẻểếềìỉởộỗợúự")

#define WELCOME_MSG                         MACHINE_NAME _UxGT(" Sẵn sàng.")              // Ready
#define MSG_BACK                            _UxGT("Trở lại")                              // Back
#define MSG_SD_INSERTED                     _UxGT("Thẻ được cắm vào")                     // Card inserted
#define MSG_SD_REMOVED                      _UxGT("Thẻ được rút ra")
#define MSG_LCD_ENDSTOPS                    _UxGT("Công tắc")                             // Endstops - công tắc hành trình
#define MSG_LCD_SOFT_ENDSTOPS               _UxGT("Công tắc mềm")                         // soft Endstops
#define MSG_MAIN                            _UxGT("Chính")                                // Main
#define MSG_ADVANCED_SETTINGS               _UxGT("Thiết lập cấp cao")                    // Advanced Settings
#define MSG_CONFIGURATION                   _UxGT("Cấu hình")                             // Configuration
#define MSG_AUTOSTART                       _UxGT("Khởi chạy tự động")                    // Autostart
#define MSG_DISABLE_STEPPERS                _UxGT("Tắt động cơ bước")                     // Disable steppers
#define MSG_DEBUG_MENU                      _UxGT("Menu gỡ lỗi")                          // Debug Menu
#define MSG_PROGRESS_BAR_TEST               _UxGT("Kiểm tra tiến độ")                     // Progress bar test
#define MSG_AUTO_HOME                       _UxGT("Về nhà tự động")                       // Auto home
#define MSG_AUTO_HOME_X                     _UxGT("Về nhà X")                             // home x
#define MSG_AUTO_HOME_Y                     _UxGT("Về nhà Y")                             // home y
#define MSG_AUTO_HOME_Z                     _UxGT("Về nhà Z")
#define MSG_AUTO_Z_ALIGN                    _UxGT("Chỉnh canh Z tự động")
#define MSG_LEVEL_BED_HOMING                _UxGT("Đang về nhà XYZ")                      // Homing XYZ
#define MSG_LEVEL_BED_WAITING               _UxGT("Nhấn để bắt đầu")                      // Click to Begin
#define MSG_LEVEL_BED_NEXT_POINT            _UxGT("Điểm tiếp theo")                       // Next Point
#define MSG_LEVEL_BED_DONE                  _UxGT("San lấp được hoàn thành")              // Leveling Done!
#define MSG_Z_FADE_HEIGHT                   _UxGT("Chiều cao mờ dần")                     // Fade Height
#define MSG_SET_HOME_OFFSETS                _UxGT("Đặt bù đắp nhà")                       // Set home offsets
#define MSG_HOME_OFFSETS_APPLIED            _UxGT("Bù đắp được áp dụng")                  // Offsets applied
#define MSG_SET_ORIGIN                      _UxGT("Đặt nguồn gốc")                        // Set origin
#define MSG_PREHEAT_1                       _UxGT("Làm nóng " PREHEAT_1_LABEL " trước")   // Preheat
#define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 _UxGT(" ")
#define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1 _UxGT(" Tất cả")                // all
#define MSG_PREHEAT_1_END                   MSG_PREHEAT_1 _UxGT(" Đầu")
#define MSG_PREHEAT_1_BEDONLY               MSG_PREHEAT_1 _UxGT(" Bàn")                   // bed -- using vietnamese term for 'table' instead
#define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 _UxGT(" Cấu hình")              // conf
#define MSG_PREHEAT_2                       _UxGT("Làm nóng " PREHEAT_2_LABEL " trước")
#define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 _UxGT(" ")
#define MSG_PREHEAT_2_ALL                   MSG_PREHEAT_2 _UxGT(" Tất cả")
#define MSG_PREHEAT_2_END                   MSG_PREHEAT_2 _UxGT(" Đầu")
#define MSG_PREHEAT_2_BEDONLY               MSG_PREHEAT_2 _UxGT(" Bàn")                   // bed -- using vietnamese term for 'table' instead
#define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 _UxGT(" Cấu hình")
#define MSG_PREHEAT_CUSTOM                  _UxGT("Sự nóng trước tự chọn")                // Preheat Custom
#define MSG_COOLDOWN                        _UxGT("Nguội xuống")                          // Cooldown
#define MSG_SWITCH_PS_ON                    _UxGT("Bật nguồn")                            // Switch power on
#define MSG_SWITCH_PS_OFF                   _UxGT("Tắt nguồn")                            // Switch power off
#define MSG_EXTRUDE                         _UxGT("Ép đùn")                               // Extrude
#define MSG_RETRACT                         _UxGT("Rút lại")                              // Retract
#define MSG_MOVE_AXIS                       _UxGT("Di chuyển trục")                       // Move axis
#define MSG_BED_LEVELING                    _UxGT("San Lấp Bàn")                          // Bed Leveling
#define MSG_LEVEL_BED                       _UxGT("Làm bằng mặt bàn")                     // Level bed
#define MSG_LEVEL_CORNERS                   _UxGT("Làm bằng góc bàn")                     // Level corners
#define MSG_NEXT_CORNER                     _UxGT("Góc tiếp theo")                        // Next corner
#define MSG_EDITING_STOPPED                 _UxGT("Chỉnh lưới đã dừng")                   // Mesh Editing Stopped
#define MSG_MESH_X                          _UxGT("Mục lục X")                            // Index X
#define MSG_MESH_Y                          _UxGT("Mục lục Y")
#define MSG_MESH_EDIT_Z                     _UxGT("Giá trị Z")                            // Z Value
#define MSG_USER_MENU                       _UxGT("Các lệnh tự chọn")                     // Custom Commands
#define MSG_UBL_DOING_G29                   _UxGT("Đang chạy G29")                        // Doing G29
#define MSG_UBL_UNHOMED                     _UxGT("Về nhà XYZ trước")
#define MSG_UBL_TOOLS                       _UxGT("Công cụ UBL")                          // UBL tools
#define MSG_UBL_LEVEL_BED                   _UxGT("San Lấp Bàn Thống Nhất (UBL)")         // Unified Bed Leveling
#define MSG_IDEX_MENU                       _UxGT("chế độ IDEX")                          // IDEX Mode
#define MSG_IDEX_MODE_AUTOPARK              _UxGT("Đậu tự động")                          // Auto-Park
#define MSG_IDEX_MODE_DUPLICATE             _UxGT("Sự gấp đôi")                           // Duplication
#define MSG_IDEX_MODE_SCALED_COPY           _UxGT("Bản sao thu nhỏ")
#define MSG_IDEX_MODE_FULL_CTRL             _UxGT("Toàn quyền điều khiển")                // Full control
#define MSG_IDEX_X_OFFSET                   _UxGT("Đầu phun X nhì")                       // 2nd nozzle X
#define MSG_IDEX_Y_OFFSET                   _UxGT("Đầu phun Y nhì")
#define MSG_IDEX_Z_OFFSET                   _UxGT("Đầu phun Z nhì")
#define MSG_IDEX_SAVE_OFFSETS               _UxGT("Lưu bù đắp")                           // Save offsets
#define MSG_UBL_MANUAL_MESH                 _UxGT("Tự xây dựng lưới")                     // Manually Build Mesh
#define MSG_UBL_BC_INSERT                   _UxGT("Đặt chêm và đo")                       // Place shim & measure
#define MSG_UBL_BC_INSERT2                  _UxGT("Đo")                                   // Measure
#define MSG_UBL_BC_REMOVE                   _UxGT("Tháo và đo bàn")                       // Remove & measure bed
#define MSG_UBL_MOVING_TO_NEXT              _UxGT("Chuyển sang tiếp theo")                // moving to next
#define MSG_UBL_ACTIVATE_MESH               _UxGT("Bật UBL")
#define MSG_UBL_DEACTIVATE_MESH             _UxGT("Tắt UBL")
#define MSG_UBL_SET_TEMP_BED                _UxGT("Nhiệt độ bàn")                         // Bed Temp
#define MSG_UBL_BED_TEMP_CUSTOM             MSG_UBL_SET_TEMP_BED
#define MSG_UBL_SET_TEMP_HOTEND             _UxGT("Nhiệt độ đầu phun")                    // Hotend Temp
#define MSG_UBL_HOTEND_TEMP_CUSTOM          MSG_UBL_SET_TEMP_HOTEND
#define MSG_UBL_MESH_EDIT                   _UxGT("Chỉnh sửa lưới")                       // Mesh Edit
#define MSG_UBL_EDIT_CUSTOM_MESH            _UxGT("Chỉnh sửa lưới tự chọn")               // Edit Custom Mesh
#define MSG_UBL_FINE_TUNE_MESH              _UxGT("Chỉnh lưới chính xác")                 // Fine tuning mesh
#define MSG_UBL_DONE_EDITING_MESH           _UxGT("Chỉnh sửa xong lưới")                  // Done Editing Mesh
#define MSG_UBL_BUILD_CUSTOM_MESH           _UxGT("Xây dựng lưới tự chọn")                // Build Custom Mesh
#define MSG_UBL_BUILD_MESH_MENU             _UxGT("Xây dựng lưới")                        // Build Mesh
#define MSG_UBL_BUILD_MESH_M1               _UxGT("Xây dựng lưới (" PREHEAT_1_LABEL ")")
#define MSG_UBL_BUILD_MESH_M2               _UxGT("Xây dựng lưới (" PREHEAT_2_LABEL ")")
#define MSG_UBL_BUILD_COLD_MESH             _UxGT("Xây dựng lưới lạnh")                   // Build cold mesh
#define MSG_UBL_MESH_HEIGHT_ADJUST          _UxGT("Điều chỉnh chiều cao lưới")            // Adjust Mesh Height
#define MSG_UBL_MESH_HEIGHT_AMOUNT          _UxGT("Số lượng chiều cao")                   // Height Amount
#define MSG_UBL_VALIDATE_MESH_MENU          _UxGT("Thẩm tra lưới")                        // Validate Mesh
#define MSG_UBL_VALIDATE_MESH_M1            _UxGT("Thẩm tra lưới (" PREHEAT_1_LABEL ")")
#define MSG_UBL_VALIDATE_MESH_M2            _UxGT("Thẩm tra lưới (" PREHEAT_2_LABEL ")")
#define MSG_UBL_VALIDATE_CUSTOM_MESH        _UxGT("Thẩm tra lưới tự chọn")                // validate custom mesh
#define MSG_UBL_CONTINUE_MESH               _UxGT("Tiếp tục xây lưới bàn")                // Continue Bed Mesh
#define MSG_UBL_MESH_LEVELING               _UxGT("Đang san lấp lưới")                    // Mesh Leveling
#define MSG_UBL_3POINT_MESH_LEVELING        _UxGT("Đang san lấp 3-điểm")                  // 3-Point Leveling
#define MSG_UBL_GRID_MESH_LEVELING          _UxGT("Đang san lấp lưới phẳng")              // Grid (planar) Mesh Leveling
#define MSG_UBL_MESH_LEVEL                  _UxGT("Làm bằng lưới")                        // Level Mesh
#define MSG_UBL_SIDE_POINTS                 _UxGT("Điểm bên cạnh")                        // Side Points
#define MSG_UBL_MAP_TYPE                    _UxGT("Loại bản đồ")                          // Map Type
#define MSG_UBL_OUTPUT_MAP                  _UxGT("Đầu ra bản đồ lưới")                   // Output Mesh Map
#define MSG_UBL_OUTPUT_MAP_HOST             _UxGT("Đầu ra cho máy chủ")                   // Output for Host
#define MSG_UBL_OUTPUT_MAP_CSV              _UxGT("Đầu ra cho CSV")                       // Output for CSV
#define MSG_UBL_OUTPUT_MAP_BACKUP           _UxGT("Hỗ trợ lưới")                          // Off Printer Backup | backup mesh
#define MSG_UBL_INFO_UBL                    _UxGT("Đầu ra thông tin UBL")                 // Output UBL Info
#define MSG_EDIT_MESH                       _UxGT("Chỉnh sửa lưới")                       // Edit mesh
#define MSG_UBL_FILLIN_AMOUNT               _UxGT("Số lượng lấp đầy")                     // Fill-in Amount
#define MSG_UBL_MANUAL_FILLIN               _UxGT("Tự lấp đầy")                           // Manual Fill-in
#define MSG_UBL_SMART_FILLIN                _UxGT("Lấp đầy thông minh")                   // Smart Fill-in
#define MSG_UBL_FILLIN_MESH                 _UxGT("Lưới lấp đầy")                         // Fill-in Mesh
#define MSG_UBL_INVALIDATE_ALL              _UxGT("Bác bỏ tất cả")                       // Invalidate All
#define MSG_UBL_INVALIDATE_CLOSEST          _UxGT("Bác bỏ gần nhất")                     // Invalidate Closest
#define MSG_UBL_FINE_TUNE_ALL               _UxGT("Chỉnh chính xác tất cả")               // Fine Tune ALl
#define MSG_UBL_FINE_TUNE_CLOSEST           _UxGT("Chỉnh chính xác gần nhất")             // Fine Tune Closest
#define MSG_UBL_STORAGE_MESH_MENU           _UxGT("Lưu trữ lưới")                         // Mesh Storage
#define MSG_UBL_STORAGE_SLOT                _UxGT("Khe nhớ")                              // Memory Slot
#define MSG_UBL_LOAD_MESH                   _UxGT("Tải lưới bàn")                         // Load Bed Mesh
#define MSG_UBL_SAVE_MESH                   _UxGT("Lưu lưới bàn")                         // Save Bed Mesh
#define MSG_MESH_LOADED                     _UxGT("%i lưới được nạp")                     // Mesh %i loaded
#define MSG_MESH_SAVED                      _UxGT("%i lưới đã lưu")
#define MSG_NO_STORAGE                      _UxGT("Không lưu trữ")                        // No Storage
#define MSG_UBL_SAVE_ERROR                  _UxGT("Điều sai: Lưu UBL")                    // Err: UBL Save
#define MSG_UBL_RESTORE_ERROR               _UxGT("Điều Sai: Khôi Phục UBL")              // Err: UBL Restore
#define MSG_UBL_Z_OFFSET_STOPPED            _UxGT("Đầu Dò-Z Đã Ngừng")                    // Z-Offset Stopped
#define MSG_UBL_STEP_BY_STEP_MENU           _UxGT("Bước-Từng-Bước UBL")                   // Step-By-Step UBL

#define MSG_LED_CONTROL                     _UxGT("Điều khiển LED")                       // LED Control
#define MSG_LEDS                            _UxGT("Đèn")                                  // Lights
#define MSG_LED_PRESETS                     _UxGT("Đèn định sẵn")                         // Light Presets
#define MSG_SET_LEDS_RED                    _UxGT("Đỏ")                                   // Red
#define MSG_SET_LEDS_ORANGE                 _UxGT("Cam")                                  // Orange
#define MSG_SET_LEDS_YELLOW                 _UxGT("Vàng")                                 // Yellow
#define MSG_SET_LEDS_GREEN                  _UxGT("Xanh Lá")                              // Green
#define MSG_SET_LEDS_BLUE                   _UxGT("Xanh")                                 // Blue
#define MSG_SET_LEDS_INDIGO                 _UxGT("Xanh Đậm")                             // Indigo
#define MSG_SET_LEDS_VIOLET                 _UxGT("Tím")                                  // Violet
#define MSG_SET_LEDS_WHITE                  _UxGT("Trắng")                                // White
#define MSG_SET_LEDS_DEFAULT                _UxGT("Mặc định")                             // Default
#define MSG_CUSTOM_LEDS                     _UxGT("Đèn Tự Chọn")                          // Custom Lights
#define MSG_INTENSITY_R                     _UxGT("Cường Độ Đỏ")                          // Red Intensity
#define MSG_INTENSITY_G                     _UxGT("Cường Độ Xanh Lá")                     // Green Intensity
#define MSG_INTENSITY_B                     _UxGT("Cường Độ Xanh")                        // Blue Intensity
#define MSG_INTENSITY_W                     _UxGT("Cường Độ Trắng")                       // White Intensity
#define MSG_LED_BRIGHTNESS                  _UxGT("độ sáng")                              // Brightness

#define MSG_MOVING                          _UxGT("Di chuyển...")                         // Moving
#define MSG_FREE_XY                         _UxGT("Giải phóng XY")                        // Free XY
#define MSG_MOVE_X                          _UxGT("Di chuyển X")                          // Move X
#define MSG_MOVE_Y                          _UxGT("Di chuyển Y")
#define MSG_MOVE_Z                          _UxGT("Di chuyển Z")
#define MSG_MOVE_E                          _UxGT("Máy đùn")                              // Extruder
#define MSG_HOTEND_TOO_COLD                 _UxGT("Đầu nóng quá lạnh")                    // Hotend too cold
#define MSG_MOVE_01MM                       _UxGT("Di chuyển 0.1mm")                      // Move 0.1mm
#define MSG_MOVE_1MM                        _UxGT("Di chuyển 1mm")                        // Move 1mm
#define MSG_MOVE_10MM                       _UxGT("Di chuyển 10mm")                       // Move 10mm
#define MSG_SPEED                           _UxGT("Tốc độ")                               // Speed
#define MSG_BED_Z                           _UxGT("Z Bàn")
#define MSG_NOZZLE                          _UxGT("Đầu phun")                             // Nozzle
#define MSG_BED                             _UxGT("Bàn")                                  // bed
#define MSG_FAN_SPEED                       _UxGT("Tốc độ quạt")                          // fan speed
#define MSG_EXTRA_FAN_SPEED                 _UxGT("Tốc độ quạt phụ")                      // Extra fan speed
#define MSG_FLOW                            _UxGT("Lưu Lượng")
#define MSG_CONTROL                         _UxGT("Điều khiển")                           // Control
#define MSG_MIN                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Đa")   // min
#define MSG_MAX                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Thiểu")
#define MSG_FACTOR                          _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Hệ Số") // factor
#define MSG_AUTOTEMP                        _UxGT("Nhiệt độ tự động")                     // Autotemp
#define MSG_LCD_ON                          _UxGT("Bật")                                  // on
#define MSG_LCD_OFF                         _UxGT("Tắt")                                  // off
#define MSG_PID_P                           _UxGT("PID-P")
#define MSG_PID_I                           _UxGT("PID-I")
#define MSG_PID_D                           _UxGT("PID-D")
#define MSG_PID_C                           _UxGT("PID-C")
#define MSG_SELECT                          _UxGT("Lựa")                                  // Select
#define MSG_ACC                             _UxGT("Tăng Tốc")
#define MSG_JERK                            _UxGT("Giật")
#if IS_KINEMATIC
  #define MSG_VA_JERK                       _UxGT("Giật-Va")
  #define MSG_VB_JERK                       _UxGT("Giật-Vb")
  #define MSG_VC_JERK                       _UxGT("Giật-Vc")
#else
  #define MSG_VA_JERK                       _UxGT("Giật-Vx")
  #define MSG_VB_JERK                       _UxGT("Giật-Vy")
  #define MSG_VC_JERK                       _UxGT("Giật-Vz")
#endif
#define MSG_VE_JERK                         _UxGT("Giật-Ve")
#define MSG_JUNCTION_DEVIATION              _UxGT("Độ Lệch Chỗ Giao")                    // Junction Dev
#define MSG_VELOCITY                        _UxGT("Vận tốc")                              // velocity
#define MSG_VMAX                            _UxGT("Vđa")                                  // Vmax
#define MSG_VMIN                            _UxGT("Vthiểu")                               // Vmin
#define MSG_VTRAV_MIN                       _UxGT("Vchuyển thiểu")                        // VTrav min
#define MSG_ACCELERATION                    _UxGT("Sự tăng tốc")                          // Acceleration
#define MSG_AMAX                            _UxGT("Tăng tốc ca")                          // Amax
#define MSG_A_RETRACT                       _UxGT("TT-Rút")                               // A-retract
#define MSG_A_TRAVEL                        _UxGT("TT-Chuyển")                            // A-travel
#define MSG_STEPS_PER_MM                    _UxGT("Bước/mm")                              // Steps
#if IS_KINEMATIC
  #define MSG_ASTEPS                        _UxGT("BướcA/mm")                             // Asteps/mm
  #define MSG_BSTEPS                        _UxGT("BướcB/mm")
  #define MSG_CSTEPS                        _UxGT("BướcC/mm")
#else
  #define MSG_ASTEPS                        _UxGT("BướcX/mm")                             // Xsteps/mm
  #define MSG_BSTEPS                        _UxGT("BướcY/mm")
  #define MSG_CSTEPS                        _UxGT("BướcZ/mm")
#endif
#define MSG_ESTEPS                          _UxGT("BướcE/mm")
#define MSG_E1STEPS                         _UxGT("BướcE1/mm")
#define MSG_E2STEPS                         _UxGT("BướcE2/mm")
#define MSG_E3STEPS                         _UxGT("BướcE3/mm")
#define MSG_E4STEPS                         _UxGT("BướcE4/mm")
#define MSG_E5STEPS                         _UxGT("BướcE5/mm")
#define MSG_E6STEPS                         _UxGT("BướcE6/mm")
#define MSG_TEMPERATURE                     _UxGT("Nhiệt độ")                             // Temperature
#define MSG_MOTION                          _UxGT("Chuyển động")                          // Motion
#define MSG_FILAMENT                        _UxGT("Vật liệu in")                          // dây nhựa
#define MSG_VOLUMETRIC_ENABLED              _UxGT("E bằng mm") MSG_CUBED                  // E in mm
#define MSG_FILAMENT_DIAM                   _UxGT("Đường kính nhựa")                      // Fil. Dai.
#define MSG_FILAMENT_UNLOAD                 _UxGT("Dỡ mm")                                // unload mm
#define MSG_FILAMENT_LOAD                   _UxGT("Nạp mm")
#define MSG_ADVANCE_K                       _UxGT("K Cấp Cao")                            // Advance K
#define MSG_CONTRAST                        _UxGT("Độ tương phản LCD")                    // LCD contrast
#define MSG_STORE_EEPROM                    _UxGT("Lưu các thiết lập")                    // Store settings
#define MSG_LOAD_EEPROM                     _UxGT("Tải các cài đặt")                      // Load settings
#define MSG_RESTORE_FAILSAFE                _UxGT("Khôi phục phòng hư")                   // Restore failsafe
#define MSG_INIT_EEPROM                     _UxGT("Khởi Tạo EEPROM")                      // Initialize EEPROM
#define MSG_SD_UPDATE                       _UxGT("Cập Nhật SD")                          // SD Update
#define MSG_RESET_PRINTER                   _UxGT("Bặt Lại Máy In")
#define MSG_REFRESH                         _UxGT("Cập Nhật")                             // Refresh
#define MSG_WATCH                           _UxGT("Màn Hình Thông Tin")                   // Info screen
#define MSG_PREPARE                         _UxGT("Chuẩn bị")                             // Prepare
#define MSG_TUNE                            _UxGT("Điều Chỉnh")                           // Tune
#define MSG_PAUSE_PRINT                     _UxGT("Tạm dừng in")                          // Pause print
#define MSG_RESUME_PRINT                    _UxGT("Tiếp tục in")                          // Resume print
#define MSG_STOP_PRINT                      _UxGT("Ngừng in")                             // Stop print
#define MSG_OUTAGE_RECOVERY                 _UxGT("Phục Hồi Mất Điện")                    // Outage Recovery
#define MSG_CARD_MENU                       _UxGT("In từ SD")                             // Print from SD
#define MSG_NO_CARD                         _UxGT("Không có thẻ SD")                      // No SD card
#define MSG_DWELL                           _UxGT("Ngủ...")                               // Sleep
#define MSG_USERWAIT                        _UxGT("Nhấn để tiếp tục...")                  // Click to resume (same as 'continue')
#define MSG_PRINT_PAUSED                    _UxGT("In tạm dừng")                          // print paused
#define MSG_PRINTING                        _UxGT("Đang in...")                           // printing
#define MSG_PRINT_ABORTED                   _UxGT("In đã hủy bỏ")                         // Print aborted
#define MSG_NO_MOVE                         _UxGT("Không di chuyển.")                     // No move.
#define MSG_KILLED                          _UxGT("ĐÃ CHẾT. ")
#define MSG_STOPPED                         _UxGT("ĐÃ NGỪNG. ")
#define MSG_CONTROL_RETRACT                 _UxGT("Rút mm")                               // Retract mm
#define MSG_CONTROL_RETRACT_SWAP            _UxGT("Rút Trao.mm")                          // Swap Re.mm
#define MSG_CONTROL_RETRACTF                _UxGT("Rút V")
#define MSG_CONTROL_RETRACT_ZHOP            _UxGT("Nhảy mm")                              // hop
#define MSG_CONTROL_RETRACT_RECOVER         _UxGT("BỏRút mm")                             // UnRet mm
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    _UxGT("BỏRút T mm")                           // S UnRet mm
#define MSG_CONTROL_RETRACT_RECOVERF        _UxGT("BỏRút V")                              // UnRet V
#define MSG_CONTROL_RETRACT_RECOVER_SWAPF   _UxGT("BỏRút T V")                            // S UnRet V
#define MSG_AUTORETRACT                     _UxGT("RútTựĐộng")                            // AutoRetr.
#define MSG_FILAMENT_SWAP_LENGTH            _UxGT("Khoảng Cách Rút")                      // Retract Distance
#define MSG_TOOL_CHANGE                     _UxGT("Thay Đổi Công Cụ")                     // Tool Change
#define MSG_TOOL_CHANGE_ZLIFT               _UxGT("Đưa Lên Z")                            // Z Raise
#define MSG_SINGLENOZZLE_PRIME_SPD          _UxGT("Tốc Độ Tuôn Ra")                       // Prime Speed
#define MSG_SINGLENOZZLE_RETRACT_SPD        _UxGT("Tốc Độ Rút Lại")                       // Retract Speed
#define MSG_FILAMENTCHANGE                  _UxGT("Thay dây nhựa")                        // change filament
#define MSG_FILAMENTLOAD                    _UxGT("Nạp dây nhựa")                         // load filament
#define MSG_FILAMENTUNLOAD                  _UxGT("Dỡ dây nhựa")                          // unload filament
#define MSG_FILAMENTUNLOAD_ALL              _UxGT("Dỡ tất cả")                            // Unload All
#define MSG_INIT_SDCARD                     _UxGT("Khởi tạo thẻ SD ")                     // Init. SD card
#define MSG_CHANGE_SDCARD                   _UxGT("Thay thẻ SD")                          // Change SD card
#define MSG_ZPROBE_OUT                      _UxGT("Đầu Dò Z qua bàn")                     // Z Probe past bed
#define MSG_SKEW_FACTOR                     _UxGT("Hệ số nghiêng")                        // Skew Factor
#define MSG_BLTOUCH                         _UxGT("BLTOUCH")                              // BLTouch
#define MSG_BLTOUCH_SELFTEST                _UxGT("Tự kiểm tra BLTOUCH ")                 // BLTouch Self-Test
#define MSG_BLTOUCH_RESET                   _UxGT("Bặt lại BLTouch")                      // Reset BLTouch
#define MSG_BLTOUCH_DEPLOY                  _UxGT("Đem BLTouch")                          // Deploy BLTouch
#define MSG_BLTOUCH_STOW                    _UxGT("Cất BLTouch")                          // Stow BLTouch
#define MSG_MANUAL_DEPLOY                   _UxGT("Đem Đầu Dò-Z")                         // Deploy Z-Probe
#define MSG_MANUAL_STOW                     _UxGT("Cất Đầu Dò-Z")                         // Stow Z-Probe
#define MSG_HOME                            _UxGT("Về nhà") // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           _UxGT("Trước")
#define MSG_zprobe_zoffset                  _UxGT("Đầu Dò Bù Đắp Z")                      // Probe Z Offset
#define MSG_BABYSTEP_X                      _UxGT("Nhít X")                               // Babystep X
#define MSG_BABYSTEP_Y                      _UxGT("Nhít Y")
#define MSG_BABYSTEP_Z                      _UxGT("Nhít Z")
#define MSG_ENDSTOP_ABORT                   _UxGT("Hủy bỏ công tắc")                      // Endstop abort
#define MSG_HEATING_FAILED_LCD              _UxGT("Sưởi đầu phun không thành công")       // Heating failed
#define MSG_HEATING_FAILED_LCD_BED          _UxGT("Sưởi bàn không thành công")            // Bed heating failed
#define MSG_ERR_REDUNDANT_TEMP              _UxGT("Điều sai: nhiệt độ dư")                // Err: REDUNDANT TEMP
#define MSG_THERMAL_RUNAWAY                 _UxGT("Vấn đề nhiệt")                         // THERMAL RUNAWAY | problem
#define MSG_THERMAL_RUNAWAY_BED             _UxGT("Vấn đề nhiệt bàn")                     // BED THERMAL RUNAWAY
#define MSG_ERR_MAXTEMP                     _UxGT("Điều sai: nhiệt độ tối đa")            // Err: MAXTEMP
#define MSG_ERR_MINTEMP                     _UxGT("Điều sai: nhiệt độ tối thiểu")         // Err: MINTEMP
#define MSG_ERR_MAXTEMP_BED                 _UxGT("Điều sai: nhiệt độ bàn tối đa")        // Err: MAXTEMP BED
#define MSG_ERR_MINTEMP_BED                 _UxGT("Điều sai: nhiệt độ bàn tối thiểu")     // Err: MINTEMP BED
#define MSG_ERR_Z_HOMING                    MSG_HOME _UxGT(" ") MSG_X MSG_Y _UxGT(" ") MSG_FIRST
#define MSG_HALTED                          _UxGT("MÁY IN ĐÃ DỪNG LẠI")                   // PRINTER HALTED
#define MSG_PLEASE_RESET                    _UxGT("Xin bặt lại")                          // Please reset
#define MSG_SHORT_DAY                       _UxGT("n")                                    // d - ngày - One character only
#define MSG_SHORT_HOUR                      _UxGT("g")                                    // h - giờ  - One character only
#define MSG_SHORT_MINUTE                    _UxGT("p")                                    // m - phút - One character only
#define MSG_HEATING                         _UxGT("Đang sưởi nóng...")                    // heating
#define MSG_COOLING                         _UxGT("Đang làm nguội...")                    // cooling
#define MSG_BED_HEATING                     _UxGT("Đang sưởi nong bàn...")                // bed heating
#define MSG_BED_COOLING                     _UxGT("Đang làm nguội bàn...")                // bed cooling
#define MSG_DELTA_CALIBRATE                 _UxGT("Cân Chỉnh Delta")                      // Delta calibration
#define MSG_DELTA_CALIBRATE_X               _UxGT("Chỉnh X lại")
#define MSG_DELTA_CALIBRATE_Y               _UxGT("Chỉnh Y lại")
#define MSG_DELTA_CALIBRATE_Z               _UxGT("Chỉnh Z lại")
#define MSG_DELTA_CALIBRATE_CENTER          _UxGT("Chỉnh Z Center")                       // Calibrate Center
#define MSG_DELTA_SETTINGS                  _UxGT("Cài Đặt Delta")                        // Delta Settings
#define MSG_DELTA_AUTO_CALIBRATE            _UxGT("Cân Chỉnh Tự Động")                    // Auto Calibration
#define MSG_DELTA_HEIGHT_CALIBRATE          _UxGT("Đặt Chiều Cao Delta")                  // Set Delta Height
#define MSG_DELTA_Z_OFFSET_CALIBRATE        _UxGT("Đầu Dò Z-Bù Đắp")                      // Probe Z-offset
#define MSG_DELTA_DIAG_ROD                  _UxGT("Gậy Chéo")                             // Diag Rod
#define MSG_DELTA_HEIGHT                    _UxGT("Chiều Cao")                            // Height
#define MSG_DELTA_RADIUS                    _UxGT("Bán Kính")                             // Radius
#define MSG_INFO_MENU                       _UxGT("Về Máy In")
#define MSG_INFO_PRINTER_MENU               _UxGT("Thông Tin Máy In")                     // Printer Info
#define MSG_3POINT_LEVELING                 _UxGT("San lấp 3-Điểm")                       // 3-Point Leveling
#define MSG_LINEAR_LEVELING                 _UxGT("San Lấp Tuyến Tính")                   // Linear Leveling
#define MSG_BILINEAR_LEVELING               _UxGT("San Lấp Song Tuyến")                   // Bilinear Leveling
#define MSG_UBL_LEVELING                    _UxGT("San Lấp Bàn Thống Nhất")               // Unified Bed Leveling
#define MSG_MESH_LEVELING                   _UxGT("Lưới San Lấp")                         // Mesh Leveling
#define MSG_INFO_STATS_MENU                 _UxGT("Thống Kê Máy In")                      // Printer Stats
#define MSG_INFO_BOARD_MENU                 _UxGT("Thông Tin Bo Mạch")                    // Board Info
#define MSG_INFO_THERMISTOR_MENU            _UxGT("Điện Trở Nhiệt")                       // Thermistors
#define MSG_INFO_EXTRUDERS                  _UxGT("Máy đùn")                              // Extruders
#define MSG_INFO_BAUDRATE                   _UxGT("Baud")                                 // Baud
#define MSG_INFO_PROTOCOL                   _UxGT("Giao Thức")                            // Protocol
#define MSG_CASE_LIGHT                      _UxGT("Đèn Khuông")                           // Case light
#define MSG_CASE_LIGHT_BRIGHTNESS           _UxGT("Độ Sáng")                              // Light Brightness
#if LCD_WIDTH >= 20
  #define MSG_INFO_PRINT_COUNT              _UxGT("Số In")                                // Print Count
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Đã hoàn thành")
  #define MSG_INFO_PRINT_TIME               _UxGT("Tổng số thời gian in")                // Total print time
  #define MSG_INFO_PRINT_LONGEST            _UxGT("Thời gian việc lâu nhất")              // Longest job time
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("Tổng số đùn")                          // Extruded total
#else
  #define MSG_INFO_PRINT_COUNT              _UxGT("In")                                   // prints
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Đã hoàn thành")                        // Completed
  #define MSG_INFO_PRINT_TIME               _UxGT("Tổng số")                             // total
  #define MSG_INFO_PRINT_LONGEST            _UxGT("Dài nhất")                             // Longest
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("Đã ép đùn")
#endif
#define MSG_INFO_MIN_TEMP                   _UxGT("Nhiệt độ tối thiểu")                   // Min Temp
#define MSG_INFO_MAX_TEMP                   _UxGT("Nhiệt độ tối đa")                      // Max temp
#define MSG_INFO_PSU                        _UxGT("Bộ nguồn")                             // PSU
#define MSG_DRIVE_STRENGTH                  _UxGT("Sức mạnh ổ đĩa")                       // Drive Strength
#define MSG_DAC_PERCENT                     _UxGT("% trình điều khiển")                   // Driver %
#define MSG_DAC_EEPROM_WRITE                _UxGT("Ghi DAC EEPROM")                       // DAC EEPROM Write
#define MSG_FILAMENT_CHANGE_HEADER_PAUSE    _UxGT("In tạm dừng")                          // PRINT PAUSED
#define MSG_FILAMENT_CHANGE_HEADER_LOAD     _UxGT("Nạp dây nhựa")
#define MSG_FILAMENT_CHANGE_HEADER_UNLOAD   _UxGT("Dỡ dây nhựa")                          // unload filament
#define MSG_FILAMENT_CHANGE_OPTION_HEADER   _UxGT("Tùy chọn hồi phục:")                   // RESUME OPTIONS
#define MSG_FILAMENT_CHANGE_OPTION_PURGE    _UxGT("Xả thêm")                              // Purge more
#define MSG_FILAMENT_CHANGE_OPTION_RESUME   _UxGT("Tiếp tục")                             // continue
#define MSG_FILAMENT_CHANGE_NOZZLE          _UxGT("  Đầu Phun: ")                         // Nozzle
#define MSG_RUNOUT_SENSOR_ENABLE            _UxGT("Cảm Biến Hết")                         // Runout Sensor
#define MSG_ERR_HOMING_FAILED               _UxGT("Sự nhà không thành công")              // Homing failed
#define MSG_ERR_PROBING_FAILED              _UxGT(" không thành công")                    // Probing failed
#define MSG_M600_TOO_COLD                   _UxGT("M600: Quá lạnh")

//
// Filament Change screens show up to 3 lines on a 4-line display
//                        ...or up to 2 lines on a 3-line display
//
#if LCD_HEIGHT >= 4
  #define MSG_ADVANCED_PAUSE_WAITING_1      _UxGT("Nhấn nút")                             // Press button
  #define MSG_ADVANCED_PAUSE_WAITING_2      _UxGT("để tiếp tục in")                       // to resume print
  #define MSG_FILAMENT_CHANGE_INIT_1        _UxGT("Chờ cho sự")                           // wait for
  #define MSG_FILAMENT_CHANGE_INIT_2        _UxGT("thay đổi dây nhựa")                    // filament change
  #define MSG_FILAMENT_CHANGE_INIT_3        _UxGT("bắt đầu")                              // to start
  #define MSG_FILAMENT_CHANGE_INSERT_1      _UxGT("Đút dây nhựa vào")                     // insert filament
  #define MSG_FILAMENT_CHANGE_INSERT_2      _UxGT("và nhấn nút")                          // and press button
  #define MSG_FILAMENT_CHANGE_INSERT_3      _UxGT("để tiếp tục")                          // to continue
  #define MSG_FILAMENT_CHANGE_HEAT_1        _UxGT("Nhấn nút")                             // Press button
  #define MSG_FILAMENT_CHANGE_HEAT_2        _UxGT("để làm nóng đầu phun")                 // to heat nozzle
  #define MSG_FILAMENT_CHANGE_HEATING_1     _UxGT("Đầu phun đang nóng lên")               // Nozzle heating
  #define MSG_FILAMENT_CHANGE_HEATING_2     _UxGT("Xin chờ...")                           // Please wait
  #define MSG_FILAMENT_CHANGE_UNLOAD_1      _UxGT("Chờ tro")                              // Wait for
  #define MSG_FILAMENT_CHANGE_UNLOAD_2      _UxGT("dây nhựa ra")                          // filament unload
  #define MSG_FILAMENT_CHANGE_LOAD_1        _UxGT("Chờ tro")
  #define MSG_FILAMENT_CHANGE_LOAD_2        _UxGT("dây nhựa vào")
  #define MSG_FILAMENT_CHANGE_PURGE_1       _UxGT("Chờ tro")                              // wait for filament purge
  #define MSG_FILAMENT_CHANGE_PURGE_2       _UxGT("xả dây nhựa")
  #define MSG_FILAMENT_CHANGE_CONT_PURGE_1  _UxGT("Nhấn nút để kết thúc")                 // Click to finish
  #define MSG_FILAMENT_CHANGE_CONT_PURGE_2  _UxGT("xả dây nhựa")                          // dây nhựa purge
  #define MSG_FILAMENT_CHANGE_RESUME_1      _UxGT("Chờ tro in")                           // Wait for print to resume
  #define MSG_FILAMENT_CHANGE_RESUME_2      _UxGT("tiếp tục...")
#else // LCD_HEIGHT < 4
  #define MSG_ADVANCED_PAUSE_WAITING_1      _UxGT("Nhấn nút để tiếp tục")                 // Click to continue
  #define MSG_FILAMENT_CHANGE_INIT_1        _UxGT("Xin chờ...")
  #define MSG_FILAMENT_CHANGE_INSERT_1      _UxGT("Chèn và nhấn")                         // Insert and Click
  #define MSG_FILAMENT_CHANGE_HEAT_1        _UxGT("Nhấn để sưởi")                         // Click to heat
  #define MSG_FILAMENT_CHANGE_HEATING_1     _UxGT("Đang sưởi nóng")                       // Heating
  #define MSG_FILAMENT_CHANGE_UNLOAD_1      _UxGT("Đang dỡ ra...")                        // Ejecting
  #define MSG_FILAMENT_CHANGE_LOAD_1        _UxGT("Đang nạp...")                          // Loading
  #define MSG_FILAMENT_CHANGE_PURGE_1       _UxGT("Đang xả...")                           // Purging
  #define MSG_FILAMENT_CHANGE_CONT_PURGE_1  _UxGT("Nhấn nút để kết thúc")                 // Click to finish
  #define MSG_FILAMENT_CHANGE_RESUME_1      _UxGT("Đang tiếp tục...")                     // Resuming
#endif // LCD_HEIGHT < 4

#define MSG_TMC_DRIVERS                     _UxGT("Trình điều khiển TMC")                 // TMC drivers
#define MSG_TMC_CURRENT                     _UxGT("Dòng điện trình điều khiển")           // Driver current
#define MSG_TMC_HYBRID_THRS                 _UxGT("Ngưỡng Hỗn Hợp")                       // Hybrid threshold
#define MSG_TMC_HOMING_THRS                 _UxGT("Vô cảm biến")                          // Sensorless homing
#define MSG_TMC_STEPPING_MODE               _UxGT("Chế độ từng bước")                     // Stepping mode
#define MSG_TMC_STEALTH_ENABLED             _UxGT("CắtTàngHình được kích hoạt")           // StealthChop enabled
