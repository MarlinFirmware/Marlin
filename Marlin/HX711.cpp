#ifndef HX711_cpp
#define HX711_cpp

#include <Arduino.h>
#include <avr/interrupt.h>
#include "HX711.h"
#include "Marlin.h"

HX711::HX711(byte dout, byte pd_sck, byte gain) {
	PD_SCK 	= pd_sck;
	DOUT 	= dout;
	
	pinMode(PD_SCK, OUTPUT);
	pinMode(DOUT, INPUT);

	set_gain(gain);
}

HX711::~HX711() {

}

bool HX711::is_ready() {
	return digitalRead(DOUT) == LOW;
}

void HX711::set_gain(byte gain) {
	switch (gain) {
		case 128:		// channel A, gain factor 128
			GAIN = 1;
			break;
		case 64:		// channel A, gain factor 64
			GAIN = 3;
			break;
		case 32:		// channel B, gain factor 32
			GAIN = 2;
			break;
	}

	digitalWrite(PD_SCK, LOW);
	read();
}

bool HX711::try_read()
{
	if (!is_enable || !is_ready())
		return false;

	byte data[3];
	CRITICAL_SECTION_START;
	// pulse the clock pin 24 times to read the data
	for (byte j = 3; j--;) {
		for (char i = 8; i--;) {
			digitalWrite(PD_SCK, HIGH);
			bitWrite(data[j], i, digitalRead(DOUT));
			digitalWrite(PD_SCK, LOW);
		}
	}

	// set the channel and the gain factor for the next reading using the clock pin
	for (int i = 0; i < GAIN; i++) {
		digitalWrite(PD_SCK, HIGH);
		digitalWrite(PD_SCK, LOW);
	}
	data[2] ^= 0x80;
	current_raw_weight = ((uint32_t)data[2] << 16) | ((uint32_t)data[1] << 8) | (uint32_t)data[0];
	set_current_weight();
	CRITICAL_SECTION_END;

	return true;
}
void HX711::enable()
{
	is_enable = true;
}

void HX711::disable()
{
	is_enable = false;
}

bool HX711::stuff_is_detected()
{
	if (!is_enable)
		return false;
	if (STUFF_SENSIVITY != 0 && current_weight + STUFF_SENSIVITY <= 0)
		return true;
	return false;
}

long HX711::read() {
	if (!is_enable)
		return current_raw_weight;
	while (!try_read());
	return current_raw_weight;
}

long HX711::read_average(byte times) {
	long sum = 0;
	for (byte i = 0; i < times; i++) {
		sum += read();
	}
	return sum / times;
}

double HX711::get_value() {
	return current_weight;
}

float HX711::get_units(byte times) {
	long value = read_average(times);
	return value / SCALE;
}

void HX711::tare() {
	read();
	set_offset(current_raw_weight);
}

void HX711::set_scale(float scale) {
	SCALE = scale;
}

void HX711::set_offset(long offset) {
	OFFSET = offset;
}

void HX711::power_down() {
	digitalWrite(PD_SCK, LOW);
	digitalWrite(PD_SCK, HIGH);	
}

void HX711::power_up() {
	digitalWrite(PD_SCK, LOW);	
}

#endif //HX711_c