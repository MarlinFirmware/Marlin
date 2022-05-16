
#include "../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_TFT)
#include "bttSerial.h"
#include "../../../src/core/serial.h"


/*
uint8_t marlin_debug_flags = MARLIN_DEBUG_NONE;

// Commonly-used strings in btt_serial output
PGMSTR(NUL_STR,   "");   PGMSTR(SP_P_STR, " P");  PGMSTR(SP_T_STR, " T");
PGMSTR(X_STR,     "X");  PGMSTR(Y_STR,     "Y");  PGMSTR(Z_STR,     "Z");  PGMSTR(E_STR,     "E");
PGMSTR(X_LBL,     "X:"); PGMSTR(Y_LBL,     "Y:"); PGMSTR(Z_LBL,     "Z:"); PGMSTR(E_LBL,     "E:");
PGMSTR(SP_A_STR, " A");  PGMSTR(SP_B_STR, " B");  PGMSTR(SP_C_STR, " C");
PGMSTR(SP_X_STR, " X");  PGMSTR(SP_Y_STR, " Y");  PGMSTR(SP_Z_STR, " Z");  PGMSTR(SP_E_STR, " E");
PGMSTR(SP_X_LBL, " X:"); PGMSTR(SP_Y_LBL, " Y:"); PGMSTR(SP_Z_LBL, " Z:"); PGMSTR(SP_E_LBL, " E:");
PGMSTR(I_STR, AXIS4_STR);     PGMSTR(J_STR, AXIS5_STR);     PGMSTR(K_STR, AXIS6_STR);
PGMSTR(I_LBL, AXIS4_STR ":"); PGMSTR(J_LBL, AXIS5_STR ":"); PGMSTR(K_LBL, AXIS6_STR ":");
PGMSTR(SP_I_STR, " " AXIS4_STR);     PGMSTR(SP_J_STR, " " AXIS5_STR);     PGMSTR(SP_K_STR, " " AXIS6_STR);
PGMSTR(SP_I_LBL, " " AXIS4_STR ":"); PGMSTR(SP_J_LBL, " " AXIS5_STR ":"); PGMSTR(SP_K_LBL, " " AXIS6_STR ":");

*/
void btt_serial_print_P(PGM_P str) {
  while (const char c = pgm_read_byte(str++)) BTT_SERIAL_CHAR(c);
}

void btt_serial_echo_start()  { static PGMSTR(echomagic, "echo:"); btt_serial_print_P(echomagic); }
void btt_serial_error_start() { static PGMSTR(errormagic, "Error:"); btt_serial_print_P(errormagic); }

void btt_serial_spaces(uint8_t count) { count *= (PROPORTIONAL_FONT_RATIO); while (count--) BTT_SERIAL_CHAR(' '); }

void btt_serial_ternary(const bool onoff, FSTR_P const pre, FSTR_P const on, FSTR_P const off, FSTR_P const post/*=nullptr*/) {
  if (pre) btt_serial_print(pre);
  btt_serial_print(onoff ? on : off);
  if (post) btt_serial_print(post);
}
void btt_serialprint_onoff(const bool onoff) { btt_serial_print(onoff ? F(STR_ON) : F(STR_OFF)); }
void btt_serialprintln_onoff(const bool onoff) { btt_serialprint_onoff(onoff); BTT_SERIAL_EOL(); }
void btt_serialprint_truefalse(const bool tf) { btt_serial_print(tf ? F("true") : F("false")); }

void btt_print_bin(uint16_t val) {
  for (uint8_t i = 16; i--;) {
    BTT_SERIAL_CHAR('0' + TEST(val, i));
    if (!(i & 0x3) && i) BTT_SERIAL_CHAR(' ');
  }
}

void btt_print_pos(LINEAR_AXIS_ARGS(const_float_t), FSTR_P const prefix/*=nullptr*/, FSTR_P const suffix/*=nullptr*/) {
  if (prefix) btt_serial_print(prefix);
  BTT_SERIAL_ECHOPGM_P(
    LIST_N(DOUBLE(LINEAR_AXES), SP_X_STR, x, SP_Y_STR, y, SP_Z_STR, z, SP_I_STR, i, SP_J_STR, j, SP_K_STR, k)
  );
  if (suffix) btt_serial_print(suffix); else BTT_SERIAL_EOL();
}

#endif