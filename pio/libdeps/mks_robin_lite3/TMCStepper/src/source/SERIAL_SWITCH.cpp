#include "SERIAL_SWITCH.h"

SSwitch::SSwitch( const uint16_t pin1, const uint16_t pin2, const uint8_t address) :
  p1(pin1),
  p2(pin2),
  addr(address)
	{
		pinMode(pin1, OUTPUT);		
    pinMode(pin2, OUTPUT);
	}

void SSwitch::active() {
  digitalWrite(p1, addr & 0b01 ? HIGH : LOW);
  digitalWrite(p2, addr & 0b10 ? HIGH : LOW);
}
