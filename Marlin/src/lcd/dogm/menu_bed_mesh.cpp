#include "../../inc/MarlinConfig.h"

#if HAS_MARLINUI_U8GLIB && ENABLED(BED_MESH_VIEWER)

#include "../marlinui.h"
#include "marlinui_DOGM.h"

// Official header to expose the 'bedlevel' (mesh_bed_leveling) class instance
#include "../../feature/bedlevel/bedlevel.h"

#define MESH_MAP_COLS _MIN(GRID_MAX_POINTS_X, 7)
#define MESH_MAP_ROWS _MIN(GRID_MAX_POINTS_Y, 7)

static int16_t static_micron_values[7][7]; 
static bool view_mode_numeric = false;
static bool view_mode_help = false; // Flag to activate the full-screen exclusive Help Screen
static uint8_t selected_button = 1; // 1 = VIS, 2 = INFO, 3 = ESC

void menu_bed_mesh_draw();

// 1. UNIVERSAL INITIALIZATION COMPATIBLE WITH ALL LEVELING SYSTEMS
void menu_bed_mesh_init() {
  selected_button = 1;       
  view_mode_numeric = false;  
  view_mode_help = false;
  
  for (uint8_t x = 0; x < 7; x++) {
    for (uint8_t y = 0; y < 7; y++) {
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

    char step_buf[6];
    itoa(step_micron, step_buf, 10);

    u8g.setFont(u8g_font_5x7);
    u8g.drawStr(6, 60, "1 Px =");
    u8g.drawStr(44, 60, step_buf);

    // Pixel-by-pixel micro-drawing of the micro symbol 'µm' due to lack of character map support on basic LCD fonts
    int label_offset_x = 44 + (strlen(step_buf) * 6);
    u8g.drawVLine(label_offset_x, 56, 4);      // Left leg
    u8g.drawVLine(label_offset_x + 2, 56, 4);  // Right leg
    u8g.drawHLine(label_offset_x, 59, 3);      // Bottom join
    u8g.drawPixel(label_offset_x - 1, 59);     // Entry tick
    u8g.drawStr(label_offset_x + 4, 60, "m");  // Appends final 'm'

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
    
    // Uses full left screen width (100px) - grid spacing adjusts per column count
    const int num_x_spacing = 100 / MESH_MAP_COLS;  // Evaluates to 14px for 7x7/6x6, and 20px for 5x5
    const int num_y_spacing = 54 / MESH_MAP_ROWS;   
    
    const int start_num_x = (100 - (MESH_MAP_COLS * num_x_spacing)) / 2;
    const int start_num_y = (64 - (MESH_MAP_ROWS * num_y_spacing)) / 2;

    // Dynamic font assignment to prevent overlapping on high density meshes (6x6 and 7x7)
    int char_w = 5;
    int char_h = 7;
    if (MESH_MAP_COLS > 5) {
      u8g.setFont(u8g_font_4x6); // Ultra-compact 4px width font for 6x6 and 7x7 meshes
      char_w = 4;
      char_h = 6;
    } else {
      u8g.setFont(u8g_font_5x7); // Standard 5px width font for meshes up to 5x5
      char_w = 5;
      char_h = 7;
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
          
          int text_len = strlen(buf);
          int text_width = text_len * char_w + (text_len - 1); 
          int draw_x = cell_center_x - (text_width / 2);  

          // Draws white bounding box tailored to the currently active font geometry
          u8g.drawBox(draw_x - 1, draw_y - char_h, text_width + 2, char_h + 2);
          
          u8g.setColorIndex(0);
          u8g.drawStr(draw_x, draw_y, buf);
          u8g.setColorIndex(1); 
        } 
        else {
          // --- POSITIVE VALUE (Standard bright text centered layout) ---
          itoa(micron_val, buf, 10);
          
          int text_len = strlen(buf);
          int text_width = text_len * char_w + (text_len - 1);
          int draw_x = cell_center_x - (text_width / 2); 

          u8g.drawStr(draw_x, draw_y, buf);
        }
      }
    }
  } 
  else {
    // GRAPHICAL VIEW: BALANCED BARYCENTRIC ADAPTIVE GRID
    // EXPANDED AREA TO 56px: Fixes 7x7 mesh overlap from 2px down to exactly 1px maximum
    const int grid_y_spacing = 56 / MESH_MAP_ROWS;   
    const int grid_x_spacing = grid_y_spacing; // Enforces a perfect square cell boundary

    const int total_grid_w = MESH_MAP_COLS * grid_x_spacing;
    const int start_x = (100 - total_grid_w) / 2;          
    const int start_y = (64 - (MESH_MAP_ROWS * grid_y_spacing)) / 2;          

    // FINAL SIMMETRICAL POSITIVE VECTOR MAP (48 ELEMENTS FOR 7x7 EXTREMUMS)
    const int8_t path_pos_x[] = {  0, 1, 0, -1, 1, 1, -1, -1,  0, 2, 0, -2, 1, 2, 2, 1, -1, -2, -2, -1,  0, 2, 3, 2, 0, -2, -3, -2, 1, 3, 3, 1, -1, -3, -3, -1, 2, 3, 3, 2, -2, -3, -3, -2, 3, 3, -3, -3 };
    const int8_t path_pos_y[] = { -1, 0, 1,  0, -1, 1,  1, -1, -2, 0, 2,  0, -2, -1, 1, 2,  2, 1, -1, -2, -3, -2, 0, 2, 3, 2, 0, -2, -3, -1, 1, 3, 3, 1, -1, -3, -3, -1, 2, 3, 3, 2, -2, -3, -3, 3, 3, -3 };

    // USER VALIDATED INVERTED SPECULAR NEGATIVE VECTOR MAP (48 ELEMENTS FOR 7x7 EXTREMUMS)
    const int8_t path_neg_x[] = { -3, -3, 3, 3, -2, -3, -3, -2, 2, 3, 3, 2, -1, -3, -3, -1, 1, 3, 3, 1, -2, -3, -2, 0, 2, 3, 2, 0, -1, -2, -2, -1, 1, 2, 2, 1, -2, 0, 2, 0, -1, -1, 1, 1, -1, 0, 1, 0 };
    const int8_t path_neg_y[] = { -3,  3, 3, -3, -3, -2, 2, 3, 3, 2, -2, -3, -3, -1, 1, 3, 3, 1, -1, -3, -2, 0, 2, 3, 2, 0, -2, -3, -2, -1, 1, 2, 2, 1, -1, -2, 0, 2, 0, -2, -1, 1, 1, -1, 0, 1, 0, -1 };

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
        if (pixels_to_draw > 48) pixels_to_draw = 48; 

        if (pixels_to_draw == 0) {
          u8g.drawPixel(center_x, center_y); // Baseline unwarped flat reference pixel
        } 
        else {
          if (val > 0) {
            // HIGH: Barycentric cluster grows outward using bright pixels
            u8g.drawPixel(center_x, center_y); 
            for (int i = 0; i < pixels_to_draw; i++) {
              u8g.drawPixel(center_x + path_pos_x[i], center_y + path_pos_y[i]);
            }
          } 
          else {
            // LOW: 9x9 frame bounds an inward-collapsing specular matrix
            u8g.drawFrame(center_x - 4, center_y - 4, 9, 9);
            for (int i = 0; i < pixels_to_draw; i++) {
              u8g.drawPixel(center_x + path_neg_x[i], center_y + path_neg_y[i]);
            }
          }
        }
      }
    }
  }

  // ACTION SIDEBAR NAVIGATION CONTROLS (X: 102-128)
  u8g.setFont(u8g_font_6x10); 
  u8g.drawVLine(101, 0, 64); 

  // BUTTON 1: VIS (Pixel Y: 2-15)
  if (selected_button == 1) {
    u8g.drawBox(103, 2, 23, 13);       
    u8g.setColorIndex(0);              
    u8g.drawStr(106, 12, "VIS");
    u8g.setColorIndex(1);              
  } else {
    u8g.drawFrame(103, 2, 23, 13);     
    u8g.drawStr(106, 12, "VIS");
  }

  // BUTTON 2: INF (Pixel Y: 24-37)
  if (selected_button == 2) {
    u8g.drawBox(103, 24, 23, 13);      
    u8g.setColorIndex(0);              
    u8g.drawStr(106, 34, "INF");
    u8g.setColorIndex(1);              
  } else {
    u8g.drawFrame(103, 24, 23, 13);    
    u8g.drawStr(106, 34, "INF");
  }

  // BUTTON 3: ESC (Pixel Y: 46-59)
  if (selected_button == 3) {
    u8g.drawBox(103, 46, 23, 13);      
    u8g.setColorIndex(0);              
    u8g.drawStr(106, 56, "ESC");
    u8g.setColorIndex(1);              
  } else {
    u8g.drawFrame(103, 46, 23, 13);    
    u8g.drawStr(106, 56, "ESC");
  }
}

#endif // HAS_MARLINUI_U8GLIB && ENABLED(BED_MESH_VIEWER)