/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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

#if HAS_CASE_LIGHT

  #ifndef INVERT_CASE_LIGHT
    #define INVERT_CASE_LIGHT false
  #endif
  int case_light_brightness;  // LCD routine wants INT
  bool case_light_on;

  void update_case_light() {
    pinMode(CASE_LIGHT_PIN, OUTPUT); // digitalWrite doesn't set the port mode
    uint8_t case_light_bright = (uint8_t)case_light_brightness;
    if (case_light_on) {
      if (USEABLE_HARDWARE_PWM(CASE_LIGHT_PIN)) {
        analogWrite(CASE_LIGHT_PIN, INVERT_CASE_LIGHT ? 255 - case_light_brightness : case_light_brightness );
      }
      else WRITE(CASE_LIGHT_PIN, INVERT_CASE_LIGHT ? LOW : HIGH);
    }
    else WRITE(CASE_LIGHT_PIN, INVERT_CASE_LIGHT ? HIGH : LOW);
  }

#endif // HAS_CASE_LIGHT

/**
 * M355: Turn case light on/off and set brightness
 *
 *   P<byte>  Set case light brightness (PWM pin required - ignored otherwise)
 *
 *   S<bool>  Set case light on/off
 *
 *   When S turns on the light on a PWM pin then the current brightness level is used/restored
 *
 *   M355 P200 S0 turns off the light & sets the brightness level
 *   M355 S1 turns on the light with a brightness of 200 (assuming a PWM pin)
 */
void gcode_M355() {
  #if HAS_CASE_LIGHT
    uint8_t args = 0;
    if (parser.seenval('P')) ++args, case_light_brightness = parser.value_byte();
    if (parser.seenval('S')) ++args, case_light_on = parser.value_bool();
    if (args) update_case_light();

    // always report case light status
    SERIAL_ECHO_START();
    if (!case_light_on) {
      SERIAL_ECHOLN("Case light: off");
    }
    else {
      if (!USEABLE_HARDWARE_PWM(CASE_LIGHT_PIN)) SERIAL_ECHOLN("Case light: on");
      else SERIAL_ECHOLNPAIR("Case light: ", case_light_brightness);
    }
  #else
    SERIAL_ERROR_START();
    SERIAL_ERRORLNPGM(MSG_ERR_M355_NONE);
  #endif
}
