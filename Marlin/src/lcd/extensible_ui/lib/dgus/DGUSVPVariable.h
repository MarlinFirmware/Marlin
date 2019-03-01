/**
 * DGUSVPVariable.h
 *
 *  Created on: Feb 9, 2019
 *      Author: tobi
 */
#pragma once

struct DGUS_VP_Variable {
  uint16_t VP;
  void* memadr;  // when you have a nullptr here, the value cannot be uploaded to the display.
  uint8_t size;

  // Callback that will be called if the display modified the value.
  // nullptr makes it readonly for the display.
  void (*set_by_display_handler)(DGUS_VP_Variable &ref_to_this, void *ptr_to_new_value);
  void (*send_to_display_handler)(DGUS_VP_Variable &ref_to_this);

  template<typename T>
  DGUS_VP_Variable& operator =(T &o) {
    *(T*)memadr = o;  // warning this is not typesafe.
    // TODO: Call out the display or mark as dirty for the next update.
    return *this;
  }
};
