///////////////////////////////////////////////////////////////////////////////
/// \file GuiAction.h
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza
///         Joaquin Herrero
///
/// \brief UI dependent common functions.
///
/// Copyright (c) 2015 BQ - Mundo Reader S.L.
/// http://www.bq.com
///
/// This file is free software; you can redistribute it and/or modify
/// it under the terms of either the GNU General Public License version 2 or
/// later or the GNU Lesser General Public License version 2.1 or later, both
/// as published by the Free Software Foundation.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
/// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
/// DEALINGS IN THE SOFTWARE.
///////////////////////////////////////////////////////////////////////////////

#ifndef GUI_ACTION_H
#define GUI_ACTION_H

#include <stdint.h>

#include "Action.h"

extern void action_set_temperature(uint16_t degrees);
extern void action_preheat();
extern void action_cooldown();

extern void action_filament_unload();
extern void action_filament_load();

extern uint8_t level_plate_step;
extern void action_level_plate();

extern void gui_action_homing();
extern void gui_action_z_homing();
extern void action_get_plane();

extern void action_correct_movement(float &x_pos, float &y_pos, float &z_pos);
extern void action_move_axis_to(uint8_t axis, float position);
extern void action_move_to_rest();
extern void action_move_to_filament_change();
extern float action_get_height();

extern void action_start_print();
extern void action_stop_print();
extern void action_finish_print();
extern void action_pause_print();
extern void action_resume_print();
extern void action_set_feedrate_multiply(uint16_t value);
extern uint16_t action_get_feedrate_multiply();

extern void action_offset_homing();
extern void action_offset();
extern void action_offset_rest();
extern void action_set_offset(uint8_t axis, float value);
extern void action_save_offset();

extern void action_wizard_init();
extern void action_wizard_finish();
extern bool action_check_preheat_temp();
extern bool action_check_cooling();

extern void action_close_inactivity();
extern void action_erase_EEPROM();

extern bool action_check_wizard();
extern void action_reset_wizard();

extern bool action_check_auto_gcode();

extern bool action_check_pause_state();

#endif // GUI_ACTION_H
