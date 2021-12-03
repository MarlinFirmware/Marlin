#include "../../inc/MarlinConfigPre.h"

#if ENABLED(X_AXIS_TWIST_COMPENSATION)

#include "menu_item.h"
#include "menu_addon.h"
#include "../../module/planner.h"
#include "../../feature/bedlevel/bedlevel.h"
#include "../../module/motion.h"
#include "../../gcode/queue.h"
#include "../../module/probe.h"

#ifndef XATC_Y_POSITION
  #define XATC_Y_POSITION ((probe.max_y() - probe.min_y())/2)
#endif

static uint8_t manual_probe_index;
float measured_z;
float z_offset;
bool was_leveling_active;

//
// Step 9: X Axis Twist Compensation Wizard is finished.
//
void xatc_wizard_done()
{
  if (!ui.wait_for_move)
  {
    print_xatc_points();
    set_bed_leveling_enabled(was_leveling_active);
    SET_SOFT_ENDSTOP_LOOSE(false);
    ui.goto_screen(menu_advanced_settings);
  }
  if (ui.should_draw())
    MenuItem_static::draw(LCD_HEIGHT >= 4, GET_TEXT(MSG_XATC_DONE));
  ui.refresh(LCDVIEW_CALL_REDRAW_NEXT);
}

void xatc_wizard_goto_next_point();

#if ENABLED(PROBE_OFFSET_WIZARD)
  void _goto_manual_move_z(const_float_t);
#else
  void _goto_manual_move_z(const_float_t scale) {
    ui.manual_move.menu_scale = scale;
    ui.goto_screen(lcd_move_z);
  }
#endif

//
// Step 8: Ask the user if he wants to update the z-offset of the probe
//
void xatc_wizard_update_z_offset(){

  MenuItem_confirm::select_screen(
          GET_TEXT(MSG_YES), GET_TEXT(MSG_NO)
        , []{
            probe.offset.z = z_offset;
            ui.goto_screen(xatc_wizard_done);
          }
        , []{
            ui.goto_screen(xatc_wizard_done);
          }
        , GET_TEXT(MSG_XATC_UPDATE_Z_OFFSET)
        , ftostr42_52(z_offset), PSTR("?")
      );
}

//
// Step 7: Set the Z-offset for this point and go to the next one.
//
void xatc_wizard_set_offset_and_go_to_next_point() {
  // Set Z-offset at probed point
  xatc_z_values[manual_probe_index] = probe.offset.z + current_position.z - measured_z;

  // Go to next point
  manual_probe_index ++;
  ui.goto_screen(xatc_wizard_goto_next_point);
}


//
// Step 6: Wizard Menu. Move the nozzle down until it touches the bed.
//
void xatc_wizard_menu() {
  START_MENU();
  float calculated_z_offset = probe.offset.z + current_position.z - measured_z;

  if (LCD_HEIGHT >= 4)
    STATIC_ITEM(MSG_MOVE_NOZZLE_TO_BED, SS_CENTER|SS_INVERT);

  STATIC_ITEM_P(PSTR("Z="), SS_CENTER, ftostr42_52(current_position.z));
  STATIC_ITEM(MSG_ZPROBE_ZOFFSET, SS_LEFT, ftostr42_52(calculated_z_offset));

  SUBMENU(MSG_MOVE_1MM,  []{ _goto_manual_move_z( 1);    });
  SUBMENU(MSG_MOVE_01MM, []{ _goto_manual_move_z( 0.1f); });

  if ((FINE_MANUAL_MOVE) > 0.0f && (FINE_MANUAL_MOVE) < 0.1f) {
    char tmp[20], numstr[10];
    // Determine digits needed right of decimal
    const uint8_t digs = !UNEAR_ZERO((FINE_MANUAL_MOVE) * 1000 - int((FINE_MANUAL_MOVE) * 1000)) ? 4 :
                         !UNEAR_ZERO((FINE_MANUAL_MOVE) *  100 - int((FINE_MANUAL_MOVE) *  100)) ? 3 : 2;
    sprintf_P(tmp, GET_TEXT(MSG_MOVE_N_MM), dtostrf(FINE_MANUAL_MOVE, 1, digs, numstr));
    #if DISABLED(HAS_GRAPHICAL_TFT)
      SUBMENU_P(NUL_STR, []{ _goto_manual_move_z(float(FINE_MANUAL_MOVE)); });
      MENU_ITEM_ADDON_START(0 + ENABLED(HAS_MARLINUI_HD44780));
      lcd_put_u8str(tmp);
      MENU_ITEM_ADDON_END();
    #else
      SUBMENU_P(tmp, []{ _goto_manual_move_z(float(FINE_MANUAL_MOVE)); });
    #endif
  }

  ACTION_ITEM(MSG_BUTTON_DONE, xatc_wizard_set_offset_and_go_to_next_point);

  END_MENU();
}

//
// Step 5: Display "Next point: 1 / 9" while waiting for move to finish
//
void xatc_wizard_moving()
{
  if (ui.should_draw())
  {
    char msg[10];
    sprintf_P(msg, PSTR("%i / %u"), manual_probe_index + 1, XATC_MAX_POINTS);
    MenuEditItemBase::draw_edit_screen(GET_TEXT(MSG_LEVEL_BED_NEXT_POINT), msg);
  }
  ui.refresh(LCDVIEW_CALL_NO_REDRAW);
  if (!ui.wait_for_move){
    ui.goto_screen(xatc_wizard_menu);
  }
}

//
// Step 4: Initiate a move to the next point
//
void xatc_wizard_goto_next_point()
{
  if (manual_probe_index < XATC_MAX_POINTS){

    float x = xatc_start + manual_probe_index * xatc_spacing;

    // Avoid probing outside the round or hexagonal area
    if (!TERN0(IS_KINEMATIC, !probe.can_reach(x, XATC_Y_POSITION))){
      ui.wait_for_move = true;
      ui.goto_screen(xatc_wizard_moving);

      // Deploy certain probes before starting probing
      #if HAS_BED_PROBE
        if (ENABLED(BLTOUCH))
          do_z_clearance(Z_CLEARANCE_DEPLOY_PROBE);
      #endif

      measured_z = probe.probe_at_point(x, XATC_Y_POSITION, PROBE_PT_STOW);
      current_position += probe.offset_xy;
      current_position.z = XATC_START_Z - probe.offset.z + measured_z;
      line_to_current_position(MMM_TO_MMS(XY_PROBE_FEEDRATE));
      ui.wait_for_move = false;
    } else {
      //Go to next point
      manual_probe_index++;
    }
  } else {

    // Compute the z-offset by averaging the values found with this wizard
    z_offset = 0;
    for(int i = 0; i < XATC_MAX_POINTS; i++){
      z_offset += xatc_z_values[i];
    }
    z_offset /= XATC_MAX_POINTS;

    // Subtract the average from the values found with this wizard.
    // This way they are indipendent from the z-offset
    for(int i = 0; i < XATC_MAX_POINTS; i++){
      xatc_z_values[i] -= z_offset;
    }
    
    ui.goto_screen(xatc_wizard_update_z_offset);
  }
}

//
// Step 3: Display "Click to Begin", wait for click
//         Move to the first probe position
//
void xatc_wizard_homing_done() {
  if (ui.should_draw())
  {
    MenuItem_static::draw(1, GET_TEXT(MSG_LEVEL_BED_WAITING));

  // Color UI needs a control to detect a touch
  #if BOTH(TOUCH_SCREEN, HAS_GRAPHICAL_TFT)
      touch.add_control(CLICK, 0, 0, TFT_WIDTH, TFT_HEIGHT);
  #endif
  }

  if (ui.use_click())
  {   
    xatc_spacing = (probe.max_x() - probe.min_x()) / (XATC_MAX_POINTS - 1);
    xatc_start = probe.min_x();                              

    SET_SOFT_ENDSTOP_LOOSE(true); // Disable soft endstops for free Z movement

    ui.goto_screen(xatc_wizard_goto_next_point);
  }
}

//
// Step 2: Display "Homing XYZ" - Wait for homing to finish
//
void xatc_wizard_homing() {
  _lcd_draw_homing();
  if (all_axes_homed())
    ui.goto_screen(xatc_wizard_homing_done);
}

//
// Step 1: Prepare for the wizard...
//
void xatc_wizard_continue() {

  // Store Bed-Leveling-State and disable
  #if HAS_LEVELING
    was_leveling_active = planner.leveling_active;
    set_bed_leveling_enabled(false);
  #endif

  // Home all axes
  ui.defer_status_screen();
  set_all_unhomed();
  ui.goto_screen(xatc_wizard_homing);
  queue.inject_P(G28_STR);
}

#endif // PROBE_OFFSET_MESH
