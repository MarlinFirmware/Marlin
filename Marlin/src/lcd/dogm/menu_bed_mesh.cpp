#include "../../inc/MarlinConfig.h"

#if HAS_MARLINUI_U8GLIB && ENABLED(BED_MESH_VIEWER)

#include "../marlinui.h"
#include "marlinui_DOGM.h"

// Official header to expose the 'bedlevel' (mesh_bed_leveling) class instance
#include "../../feature/bedlevel/bedlevel.h"

#define MESH_MAP_COLS _MIN(GRID_MAX_POINTS_X, 10)
#define MESH_MAP_ROWS _MIN(GRID_MAX_POINTS_Y, 10)

static int16_t static_micron_values[GRID_MAX_POINTS_X][GRID_MAX_POINTS_Y];
static bool view_mode_numeric = false;
static bool view_mode_help = false; // Flag to activate the full-screen exclusive Help Screen
static uint8_t selected_button = 1; // 1 = VIS, 2 = INFO, 3 = ESC

void menu_bed_mesh_draw();

// 1. UNIVERSAL INITIALIZATION COMPATIBLE WITH ALL LEVELING SYSTEMS
void menu_bed_mesh_init() {
  selected_button = 1;       
  view_mode_numeric = false;  
  view_mode_help = false;
  
  for (uint8_t x = 0; x < GRID_MAX_POINTS_X; x++) {
    for (uint8_t y = 0; y < GRID_MAX_POINTS_Y; y++) {
      static_micron_values[x][y] = 0;
    }
  }

  // Safe data extraction using official Marlin leveling structures
  for (uint8_t x = 0; x < MESH_MAP_COLS; x++) {
    for (uint8_t y = 0; y < MESH_MAP_ROWS; y++) {
      float z = 0.0;
      
      #if ENABLED(MESH_BED_LEVELING)
        z = bedlevel.z_values[x][y]; 
      #elif ENABLED(AUTO_BED_LEVELING_BILINEAR)
        #if defined(bilinear_grid_values)
          z = bilinear_grid_values[x][y];
        #elif HAS_LEVELING
          z = bedlevel.z_values[x][y];
        #endif
      #elif ENABLED(AUTO_BED_LEVELING_UBL)
        z = ubl.z_values[x][y];
      #endif
      
      if (!isnan(z) && z > -10.0 && z < 10.0) {
        static_micron_values[x][y] = (int16_t)(z * 1000.0);
      }
    }
  }

  ui.encoderPosition = 0;

  // EXCLUSIVE SCREEN LOOP WITH RELATIVE ENCODER TRACKING
  bool in_mesh_screen = true;
  int16_t last_encoder_pos = 0;
  
  while (in_mesh_screen) {
    ui.update(); // Poll hardware pins on the mainboard

    const int16_t current_encoder_pos = ui.encoderPosition;
    if (current_encoder_pos != last_encoder_pos) {
      int16_t diff = current_encoder_pos - last_encoder_pos;

      // If inside the Help Screen, any knob rotation closes it
      if (view_mode_help) {
        view_mode_help = false;
        ui.encoderPosition = 0;
        last_encoder_pos = 0;
      } 
      else {
        if (diff > 0) {
          selected_button = (selected_button % 3) + 1;
        } 
        else if (diff < 0) {
          selected_button = (selected_button == 1) ? 3 : selected_button - 1;
        }
        last_encoder_pos = current_encoder_pos;
      }
      ui.refresh(); 
    }

    if (ui.use_click()) {
      // If clicked while inside the Help Screen, close it immediately
      if (view_mode_help) {
        view_mode_help = false;
      } 
      else {
        if (selected_button == 1) {
          view_mode_numeric = !view_mode_numeric;
        } 
        else if (selected_button == 2) {
          view_mode_help = true; // Activate full-screen textless infographic help mode
        }
        else if (selected_button == 3) {
          in_mesh_screen = false; // Exit back to Marlin menu structure
        }
      }
      ui.refresh();
    }

    u8g.firstPage();
    do {
      menu_bed_mesh_draw();
    } while (u8g.nextPage());

    safe_delay(20); 
  }

  ui.goto_previous_screen();
}

// 2. GRAPHICAL RENDERING FUNCTION
void menu_bed_mesh_draw() {
  
  if (view_mode_help) {
    // =========================================================================
    // UNIVERSAL INFOGRAPHIC HELP SCREEN (TEXTLESS / FULL SCREEN)
    // =========================================================================
    
    // 1. DRAW BED LATERAL SECTION VIEW (LEFT SIDE)
    // Dashed baseline representing Z = 0 (Ideal flat bed)
    for (int x = 6; x < 50; x += 3) u8g.drawHLine(x, 26, 1);
    
    // Warped bed profile: positive peak (convex) and negative lake (concave) using smooth segments
    u8g.drawLine(6, 26, 10, 18);
    u8g.drawLine(10, 18, 16, 14);
    u8g.drawLine(16, 14, 22, 18);
    u8g.drawLine(22, 18, 26, 26);
    
    u8g.drawLine(26, 26, 30, 34);
    u8g.drawLine(30, 34, 36, 38);
    u8g.drawLine(36, 38, 42, 34);
    u8g.drawLine(42, 34, 46, 26);

    // Connecting arrows linking bed profile anomalies to the right-side layout examples
    u8g.drawLine(18, 10, 75, 10); u8g.drawPixel(18, 11); u8g.drawPixel(18, 12); // High indicator
    u8g.drawLine(38, 42, 75, 42); u8g.drawPixel(38, 41); u8g.drawPixel(38, 40); // Low indicator

    // 2. GRAPHICAL MESH PATTERN EXAMPLES (RIGHT SIDE)
    const int ex_pos_center_x = 80;
    const int ex_neg_center_x = 80;
    const int ex_pos_center_y = 14;
    const int ex_neg_center_y = 42;

    // High Example (Top): Convex point -> expansion from stational center (8-pixel sample)
    u8g.drawPixel(ex_pos_center_x, ex_pos_center_y);
    const int8_t ex_path_pos_x[] = {  0, 1, 0, -1, 1, 1, -1, -1 };
    const int8_t ex_path_pos_y[] = { -1, 0, 1,  0, -1, 1,  1, -1 };
    for (int i = 0; i < 8; i++) u8g.drawPixel(ex_pos_center_x + ex_path_pos_x[i], ex_pos_center_y + ex_path_pos_y[i]);

    // Low Example (Bottom): Concave point -> 9x9 frame bounding a symmetrical collapse (8-pixel sample)
    u8g.drawFrame(ex_neg_center_x - 4, ex_neg_center_y - 4, 9, 9);
    const int8_t ex_path_neg_x[] = { -3, -3, 3, 3, -2, -3, -3, -2 };
    const int8_t ex_path_neg_y[] = { -3,  3, 3, -3, -3, -2, 2, 3 };
    for (int i = 0; i < 8; i++) u8g.drawPixel(ex_neg_center_x + ex_path_neg_x[i], ex_neg_center_y + ex_path_neg_y[i]);

    // 3. DYNAMIC MATHEMATICAL SCALE FORMULA DISPLAY (BOTTOM LEFT)
    #ifdef MESH_EDIT_Z_STEP
      const int16_t step_micron = (int16_t)(MESH_EDIT_Z_STEP * 1000.0);
    #else
      const int16_t step_micron = 25;
    #endif

    // Buffer to store the formatted scale string
    char step_buf[20]; 
    
    // Uses the strict hex escape \xB5 for the 'µ' symbol to prevent UTF-8 dual-byte encoding artifacts
    sprintf_P(step_buf, PSTR("1 Px = %d \xB5m"), step_micron);

    // Switch to the official U8glib 7x14 font which includes full Latin-1 character map support
    u8g.setFont(u8g_font_7x14);
    
    // Draw the unified scale string at a calibrated Y-axis baseline to prevent bottom bezel clipping
    u8g.drawStr(6, 60, step_buf);

    // 4. INTERNATIONAL VERTICAL EXIT COMBINED ICON STRUCTURE (BOTTOM RIGHT)
    const int exit_x = 112; 

    // Encoder knob glyph
    u8g.drawCircle(exit_x, 6, 4); 
    u8g.drawPixel(exit_x, 6);

    // Press arrow glyph
    u8g.drawVLine(exit_x, 14, 5); 
    u8g.drawPixel(exit_x - 1, 17); u8g.drawPixel(exit_x + 1, 17);
    u8g.drawPixel(exit_x - 2, 16); u8g.drawPixel(exit_x + 2, 16);

    u8g.setFont(u8g_font_5x7);
    u8g.drawStr(exit_x - 14, 29, "CLICK");

    // Open door exit glyph
    const int door_x = exit_x - 8; 
    const int door_y = 38;

    u8g.drawVLine(door_x, door_y, 16);          
    u8g.drawHLine(door_x, door_y, 12);          
    u8g.drawVLine(door_x + 12, door_y, 16);     
    
    u8g.drawLine(door_x, door_y, door_x - 4, door_y + 4);
    u8g.drawVLine(door_x - 4, door_y + 4, 16);
    u8g.drawLine(door_x - 4, door_y + 20, door_x, door_y + 16);

    u8g.drawHLine(door_x + 3, door_y + 9, 13);   
    u8g.drawPixel(door_x + 14, door_y + 8);      
    u8g.drawPixel(door_x + 14, door_y + 10);     
    u8g.drawPixel(door_x + 13, door_y + 7);
    u8g.drawPixel(door_x + 13, door_y + 11);

    return; // Fast return to block standard view layers
  }

  // =========================================================================
  // STANDARD LAYOUT LAYER (GRID + ACTION SIDEBAR)
  // =========================================================================
  if (view_mode_numeric) {
    // =========================================================================
    // MODAL NUMERIC VIEW WITH ADAPTIVE ANTI-OVERLAP BOUNDS (FULL WIDTH)
    // =========================================================================
    const int max_w = (MESH_MAP_COLS > 9) ? 120 : 108;
    const int num_x_spacing = max_w / MESH_MAP_COLS;  
    const int num_y_spacing = 64 / MESH_MAP_ROWS;   

    const int start_num_x = (max_w - (MESH_MAP_COLS * num_x_spacing)) / 2;
    const int start_num_y = (64 - (MESH_MAP_ROWS * num_y_spacing)) / 2;

    // Hardcoded bounding box widths for strict non-proportional evaluation
    int char_w = 5, char_h = 7;
    if (MESH_MAP_COLS > 7 || MESH_MAP_ROWS > 7) {
      u8g.setFont(u8g_font_micro); char_w = 3; char_h = 5;
    } else if (MESH_MAP_COLS > 5 || MESH_MAP_ROWS > 5) {
      u8g.setFont(u8g_font_4x6); char_w = 4; char_h = 6;
    } else {
      u8g.setFont(u8g_font_5x7); char_w = 5; char_h = 7;
    }

    for (uint8_t x = 0; x < MESH_MAP_COLS; x++) {
      for (uint8_t y = 0; y < MESH_MAP_ROWS; y++) {
        int16_t micron_val = static_micron_values[x][y];
        
        int cell_center_x = start_num_x + (x * num_x_spacing) + (num_x_spacing / 2);
        int draw_y = start_num_y + ((MESH_MAP_ROWS - 1 - y) * num_y_spacing) + char_h + 1; 
        
        char buf[6]; 

        if (micron_val == 0) {
          u8g.drawStr(cell_center_x - (char_w / 2), draw_y, "0");
        }
        else if (micron_val < 0) {
          // --- NEGATIVE VALUE (Dark text printed over tight white solid box) ---
          micron_val = -micron_val; 
          itoa(micron_val, buf, 10);
          
          // Static math calculation with dynamic tracking and 1px subtraction
          int text_len = strlen(buf);
          
          // Extracts the condition to understand if we are in 8x8/9x9 (Tier 3) or lower
          const bool is_dense = (MESH_MAP_COLS > 7 || MESH_MAP_ROWS > 7);

          int text_width = is_dense 
            ? ((text_len * char_w) + (text_len - 1)) - 1
            : u8g.getStrWidth(buf) - 1;
            
          int draw_x = cell_center_x - (text_width / 2);  

          // For dense meshes (Tier 3), we shift the box start 1px to the left (draw_x - 2) and 1px up (draw_y - char_h - 1)
          // while expanding width (+3) and height (+2) to inject the missing 1px border on top and left.
          if (is_dense) {
            u8g.drawBox(draw_x - 1, draw_y - char_h - 1, text_width + 3, char_h + 2);
          } else {
            u8g.drawBox(draw_x - 1, draw_y - char_h, text_width + 2, char_h + 1); // Retains 7x7 perfection
          }
          
          u8g.setColorIndex(0);
          u8g.drawStr(draw_x, draw_y, buf);
          u8g.setColorIndex(1); 
        } 
        else {
          // --- POSITIVE VALUE (Standard bright text centered layout) ---
          itoa(micron_val, buf, 10);
          
          // Static non-proportional grid width math calculation
          int text_len = strlen(buf);
          int text_width = (text_len * char_w) + (text_len - 1); 
          int draw_x = cell_center_x - (text_width / 2); 

          u8g.drawStr(draw_x, draw_y, buf);
        }
      }
    }
  }
  else {
    // =========================================================================
    // GRAPHICAL VIEW: BALANCED BARYCENTRIC ADAPTIVE GRID
    // =========================================================================
    const int max_possible_x_space = 108 / MESH_MAP_COLS;
    const int max_possible_y_space = 64 / MESH_MAP_ROWS;

    // Enforces a perfect square cell using the smallest available boundary as the master spacing
    const int grid_y_spacing = (max_possible_x_space < max_possible_y_space) ? max_possible_x_space : max_possible_y_space;   
    const int grid_x_spacing = grid_y_spacing; 

    const int total_grid_w = MESH_MAP_COLS * grid_x_spacing;
    const int start_x = (108 - total_grid_w) / 2;          
    const int start_y = (64 - (MESH_MAP_ROWS * grid_y_spacing)) / 2;          

    // FINAL SIMMETRICAL POSITIVE VECTOR MAP (48 ELEMENTS FOR 7x7 EXTREMUMS)
    const int8_t path_pos_x[] = {  0, 1, 0, -1, 1, 1, -1, -1,  0, 2, 0, -2, 1, 2, 2, 1, -1, -2, -2, -1,  0, 2, 3, 2, 0, -2, -3, -2, 1, 3, 3, 1, -1, -3, -3, -1, 2, 3, 3, 2, -2, -3, -3, -2, 3, 3, -3, -3 };
    const int8_t path_pos_y[] = { -1, 0, 1,  0, -1, 1,  1, -1, -2, 0, 2,  0, -2, -1, 1, 2,  2, 1, -1, -2, -3, -2, 0, 2, 3, 2, 0, -2, -3, -1, 1, 3, 3, 1, -1, -3, -3, -1, 2, 3, 3, 2, -2, -3, -3, 3, 3, -3 };

    // USER VALIDATED INVERTED SPECULAR NEGATIVE VECTOR MAP (48 ELEMENTS FOR 7x7 EXTREMUMS)
    const int8_t path_neg_x[] = { -3, -3, 3, 3, -2, -3, -3, -2, 2, 3, 3, 2, -1, -3, -3, -1, 1, 3, 3, 1, -2, -3, -2, 0, 2, 3, 2, 0, -1, -2, -2, -1, 1, 2, 2, 1, -2, 0, 2, 0, -1, -1, 1, 1, -1, 0, 1, 0 };
    const int8_t path_neg_y[] = { -3,  3, 3, -3, -3, -2, 2, 3, 3, 2, -2, -3, -3, -1, 1, 3, 3, 1, -1, -3, -2, 0, 2, 3, 2, 0, -2, -3, -2, -1, 1, 2, 2, 1, -1, -2, 0, 2, 0, -2, -1, 1, 1, -1, 0, 1, 0, -1 };

    // >>> SPECIFIC SEQUENCES FOR 8x8 AND 9x9 DENSE MESHES <<<
    const int8_t path_dense_pos_x[] = {  0, 1, 0, -1, 1, 1, -1, -1,  0, 2, 0, -2, 1, 2, 2, 1, -1, -2, -2, -1, 2, 2, -2, -2}; 
    const int8_t path_dense_pos_y[] = { -1, 0, 1,  0, -1, 1,  1, -1, -2, 0, 2,  0, -2, -1, 1, 2,  2, 1, -1, -2, -2, 2, 2, -2};
    
    const int8_t path_dense_neg_x[] = { -2, -2, 2, 2, -1, -2, -2, -1, 1, 2, 2, 1, -2, 0, 2, 0, -1, -1, 1, 1, -1, 0, 1, 0 };
    const int8_t path_dense_neg_y[] = { -2, 2, 2, -2, -2, -1, 1, 2, 2, 1, -1, -2, 0, 2, 0, -2, -1, 1, 1, -1, 0, 1, 0, -1 };

    // Multi-tier structural clearance heuristic based on critical threshold boundaries
    const int8_t *current_pos_x = path_pos_x, *current_pos_y = path_pos_y;
    const int8_t *current_neg_x = path_neg_x, *current_neg_y = path_neg_y;
    int max_allowed_pixels = 48, frame_offset = 4, frame_size = 9;

    // Trigger Tier 2 if rows exceed 7 OR if columns exceed the 11-point horizontal threshold
    if (MESH_MAP_ROWS > 7 || MESH_MAP_COLS > 11) {
      current_pos_x = path_dense_pos_x; current_pos_y = path_dense_pos_y;
      current_neg_x = path_dense_neg_x; current_neg_y = path_dense_neg_y;
      max_allowed_pixels = 24; 
      frame_offset = 3; 
      frame_size = 7; // Forces the safer, tight 7x7 frame layout
    }
    
    for (uint8_t x = 0; x < MESH_MAP_COLS; x++) {
      for (uint8_t y = 0; y < MESH_MAP_ROWS; y++) {
        int16_t val = static_micron_values[x][y]; 
        
        int center_x = start_x + (x * grid_x_spacing) + (grid_x_spacing / 2);
        int center_y = start_y + ((MESH_MAP_ROWS - 1 - y) * grid_y_spacing) + (grid_y_spacing / 2);

        // Adaptive scaling calculations synchronized with firmware MESH_EDIT_Z_STEP
        #ifdef MESH_EDIT_Z_STEP
          const int16_t step_micron = (int16_t)(MESH_EDIT_Z_STEP * 1000.0);
        #else
          const int16_t step_micron = 25;
        #endif

        int pixels_to_draw = (int)(abs(val) / (step_micron > 0 ? step_micron : 25));
        
        // Clamped using the dynamic variable to prevent out-of-bounds memory read
        if (pixels_to_draw > max_allowed_pixels) pixels_to_draw = max_allowed_pixels; 

        if (pixels_to_draw == 0) {
          u8g.drawPixel(center_x, center_y); // Baseline unwarped flat reference pixel
        } 
        else {
          if (val > 0) {
            // HIGH: Barycentric cluster grows outward using bright pixels
            u8g.drawPixel(center_x, center_y); 
            for (int i = 0; i < pixels_to_draw; i++) {
              // Uses active pointer tracking instead of hardcoded 7x7 maps
              u8g.drawPixel(center_x + current_pos_x[i], center_y + current_pos_y[i]);
            }
          } 
          else {
            // LOW: Draws frame using dynamic pre-calculated sizing parameters
            u8g.drawFrame(center_x - frame_offset, center_y - frame_offset, frame_size, frame_size);
            
            for (int i = 0; i < pixels_to_draw; i++) {
              // Uses active pointer tracking instead of hardcoded 7x7 maps
              u8g.drawPixel(center_x + current_neg_x[i], center_y + current_neg_y[i]);
            }
          }
        }
      }
    }
  }

  // ACTION SIDEBAR NAVIGATION CONTROLS (X: 109-128)
  u8g.setFont(u8g_font_6x10); 

  // Dynamic sidebar boundary assignment: 120px for ultra-dense meshes, 110px for standard ones
  const bool is_ultra_dense = (view_mode_numeric && MESH_MAP_COLS > 9);
  const int btn_x = is_ultra_dense ? 121 : 109;
  const int btn_w = is_ultra_dense ? 10 : 21; 
  
  // Dynamic vertical spacing to achieve absolute equidistance in both configurations
  const int inf_y = is_ultra_dense ? 19 : 21; 
  const int esc_y = is_ultra_dense ? 37 : 41; 
  const int esc_h = is_ultra_dense ? 27 : 13; // Handwired 27px tall box to fit 7px uppercase letters perfectly

  // --- BUTTON 1: VIS / V ---
  if (selected_button == 1) {
    u8g.drawBox(btn_x, 1, btn_w, 13);
    u8g.setColorIndex(0);
    u8g.drawStr(btn_x + 2, 11, is_ultra_dense ? "V" : "VIS");
    u8g.setColorIndex(1);
  } else {
    u8g.drawFrame(btn_x, 1, btn_w, 13);
    u8g.drawStr(btn_x + 2, 11, is_ultra_dense ? "V" : "VIS");
  }

  // --- BUTTON 2: INF / I ---
  if (selected_button == 2) {
    u8g.drawBox(btn_x, inf_y, btn_w, 13);
    u8g.setColorIndex(0);
    u8g.drawStr(btn_x + 2, inf_y + 10, is_ultra_dense ? "I" : "INF"); 
    u8g.setColorIndex(1);
  } else {
    u8g.drawFrame(btn_x, inf_y, btn_w, 13);
    u8g.drawStr(btn_x + 2, inf_y + 10, is_ultra_dense ? "I" : "INF");
  }

  // --- BUTTON 3: ESC / VERTICAL ESC ---
  if (selected_button == 3) {
    u8g.drawBox(btn_x, esc_y, btn_w, esc_h); 
    u8g.setColorIndex(0);
    if (is_ultra_dense) {
      // Perfectly balanced 1px margins with 7px tall characters over 27px box bounds
      u8g.drawStr(btn_x + 2, esc_y + 9,  "E"); // Top letter (baseline at +8)
      u8g.drawStr(btn_x + 2, esc_y + 17, "S"); // Middle letter (baseline at +16)
      u8g.drawStr(btn_x + 2, esc_y + 25, "C"); // Bottom letter (baseline at +24)
    } else {
      u8g.drawStr(btn_x + 2, esc_y + 10, "ESC"); 
    }
    u8g.setColorIndex(1);
  } else {
    u8g.drawFrame(btn_x, esc_y, btn_w, esc_h); 
    if (is_ultra_dense) {
      u8g.drawStr(btn_x + 2, esc_y + 9,  "E");
      u8g.drawStr(btn_x + 2, esc_y + 17, "S");
      u8g.drawStr(btn_x + 2, esc_y + 25, "C");
    } else {
      u8g.drawStr(btn_x + 2, esc_y + 10, "ESC");
    }
  }
}

#endif // HAS_MARLINUI_U8GLIB && ENABLED(BED_MESH_VIEWER)
