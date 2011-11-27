#ifndef __MARLINH
#define __MARLINH

// Tonokip RepRap firmware rewrite based off of Hydra-mmm firmware.
// Licence: GPL
#include <WProgram.h>
#include "fastio.h"
#include <avr/pgmspace.h>
#include "Configuration.h"


#define  FORCE_INLINE __attribute__((always_inline)) inline
//#define SERIAL_ECHO(x) Serial << "echo: " << x;
//#define SERIAL_ECHOLN(x) Serial << "echo: "<<x<<endl;
//#define SERIAL_ERROR(x) Serial << "Error: " << x;
//#define SERIAL_ERRORLN(x) Serial << "Error: " << x<<endl;
//#define SERIAL_PROTOCOL(x) Serial << x;
//#define SERIAL_PROTOCOLLN(x) Serial << x<<endl;



#define SERIAL_PROTOCOL(x) Serial.print(x);
#define SERIAL_PROTOCOLPGM(x) serialprintPGM(PSTR(x));
#define SERIAL_PROTOCOLLN(x) {Serial.print(x);Serial.write('\n');}
#define SERIAL_PROTOCOLLNPGM(x) {serialprintPGM(PSTR(x));Serial.write('\n');}

const char errormagic[] PROGMEM ="Error:";
const char echomagic[] PROGMEM ="echo:";
#define SERIAL_ERROR_START serialprintPGM(errormagic);
#define SERIAL_ERROR(x) SERIAL_PROTOCOL(x)
#define SERIAL_ERRORPGM(x) SERIAL_PROTOCOLPGM(x)
#define SERIAL_ERRORLN(x) SERIAL_PROTOCOLLN(x)
#define SERIAL_ERRORLNPGM(x) SERIAL_PROTOCOLLNPGM(x)

#define SERIAL_ECHO_START serialprintPGM(echomagic);
#define SERIAL_ECHO(x) SERIAL_PROTOCOL(x)
#define SERIAL_ECHOPGM(x) SERIAL_PROTOCOLPGM(x)
#define SERIAL_ECHOLN(x) SERIAL_PROTOCOLLN(x)
#define SERIAL_ECHOLNPGM(x) SERIAL_PROTOCOLLNPGM(x)

#define SERIAL_ECHOPAIR(name,value) {SERIAL_ECHOPGM(name);SERIAL_ECHO(value);}


//things to write to serial from Programmemory. saves 400 to 2k of RAM.
#define SerialprintPGM(x) serialprintPGM(PSTR(x))
FORCE_INLINE void serialprintPGM(const char *str)
{
  char ch=pgm_read_byte(str);
  while(ch)
  {
    Serial.write(ch);
    ch=pgm_read_byte(++str);
  }
}


void get_command();
void process_commands();

void manage_inactivity(byte debug);

#if X_ENABLE_PIN > -1
  #define  enable_x() WRITE(X_ENABLE_PIN, X_ENABLE_ON)
  #define disable_x() WRITE(X_ENABLE_PIN,!X_ENABLE_ON)
#else
  #define enable_x() ;
  #define disable_x() ;
#endif

#if Y_ENABLE_PIN > -1
  #define  enable_y() WRITE(Y_ENABLE_PIN, Y_ENABLE_ON)
  #define disable_y() WRITE(Y_ENABLE_PIN,!Y_ENABLE_ON)
#else
  #define enable_y() ;
  #define disable_y() ;
#endif

#if Z_ENABLE_PIN > -1
  #define  enable_z() WRITE(Z_ENABLE_PIN, Z_ENABLE_ON)
  #define disable_z() WRITE(Z_ENABLE_PIN,!Z_ENABLE_ON)
#else
  #define enable_z() ;
  #define disable_z() ;
#endif

#if E_ENABLE_PIN > -1
  #define  enable_e() WRITE(E_ENABLE_PIN, E_ENABLE_ON)
  #define disable_e() WRITE(E_ENABLE_PIN,!E_ENABLE_ON)
#else
  #define enable_e() ;
  #define disable_e() ;
#endif

enum AxisEnum {X_AXIS=0, Y_AXIS=1, Z_AXIS=2, E_AXIS=3};


void FlushSerialRequestResend();
void ClearToSend();

void get_coordinates();
void prepare_move();
void kill();

void enquecommand(const char *cmd); //put an ascii command at the end of the current buffer.
void prepare_arc_move(char isclockwise);

#ifndef CRITICAL_SECTION_START
  #define CRITICAL_SECTION_START  unsigned char _sreg = SREG; cli();
  #define CRITICAL_SECTION_END    SREG = _sreg;
#endif //CRITICAL_SECTION_START

extern float homing_feedrate[];
extern bool axis_relative_modes[];
extern float current_position[NUM_AXIS] ;
extern float add_homeing[3];

#endif
