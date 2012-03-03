#define __AVR_ATmega2560__
#define __cplusplus
#define __builtin_va_list int
#define __attribute__(x)
#define __inline__
#define __asm__(x)
#define ARDUINO 23
extern "C" void __cxa_pure_virtual() {}
#include "C:\arduino-0023\libraries\LiquidCrystal\LiquidCrystal.h"
#include "C:\arduino-0023\libraries\LiquidCrystal\LiquidCrystal.cpp"
void enquecommand(const char *cmd);
void setup_photpin();
void setup_powerhold();
void suicide();
void setup();
void loop();
void get_command();
float code_value();
long code_value_long();
bool code_seen(char code_string[]);
bool code_seen(char code);
void process_commands();
void process_commands();
void FlushSerialRequestResend();
void ClearToSend();
void get_coordinates();
void get_arc_coordinates();
void prepare_move();
void prepare_arc_move(char isclockwise);
void manage_inactivity(byte debug);
void kill();
void lcdProgMemprint(const char *str);
int intround(const float &x);
void lcd_status(const char* message);
void lcd_statuspgm(const char* message);
FORCE_INLINE void clear();
void lcd_init();
void beep();
void beepshort();
void lcd_status();
void buttons_init();
void buttons_check();
char *ftostr3(const float &x);
char *itostr2(const uint8_t &x);
char *ftostr31(const float &x);
char *ftostr32(const float &x);
char *itostr31(const int &xx);
char *itostr3(const int &xx);
char *itostr4(const int &xx);
char *ftostr51(const float &x);
char *ftostr52(const float &x);
void wd_init();
void wd_reset();

#include "C:\arduino-0023\hardware\arduino\cores\arduino\WProgram.h"
#include "C:\Users\Blair\Desktop\Marlin\Marlin\Marlin.pde" 
#include "C:\Users\Blair\Desktop\Marlin\Marlin\ultralcd.pde"
#include "C:\Users\Blair\Desktop\Marlin\Marlin\watchdog.pde"
