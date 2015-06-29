#ifndef __HX711_h
#define __HX711_h
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

//===========================================================================
//============================= public variables ============================
//===========================================================================
//volatile long current_weight;  // current extruder weight 

class HX711
{
	private:
		byte PD_SCK;			// Power Down and Serial Clock Input Pin
		byte DOUT;				// Serial Data Output Pin
		byte GAIN;				// amplification factor
		long OFFSET;			// used for tare weight
		float SCALE;			// used to return weight in grams, kg, ounces, whatever
		bool is_enable;

	public:
		long STUFF_SENSIVITY;	// Sensivity for check material on the weights
		// Current value of this scale
		float current_weight;
		float current_raw_weight;
#define set_current_weight() {current_weight = (current_raw_weight-OFFSET)/SCALE;}
#define set_stuff_sensivity(sensivity) {STUFF_SENSIVITY = sensivity;}
		void enable();
		void disable();

		// define clock and data pin, channel, and gain factor
		// channel selection is made by passing the appropriate gain: 128 or 64 for channel A, 32 for channel B
		// gain: 128 or 64 for channel A; channel B works with 32 gain factor only
		HX711(byte dout, byte pd_sck, byte gain = 128);

		virtual ~HX711();

		// check if HX711 is ready
		// from the datasheet: When output data is not ready for retrieval, digital output pin DOUT is high. Serial clock
		// input PD_SCK should be low. When DOUT goes to low, it indicates data is ready for retrieval.
		bool is_ready();

		// set the gain factor; takes effect only after a call to read()
		// channel A can be set for a 128 or 64 gain; channel B has a fixed 32 gain
		// depending on the parameter, the channel is also set to either A or B
		void set_gain(byte gain = 128);
		
		bool try_read();
		
		// waits for the chip to be ready and returns a reading
		long read();

		// returns an average reading; times = how many times to read
		long read_average(byte times = 10);

		// current value without the tare weight
		double get_value();

		// returns get_value() divided by SCALE, that is the raw value divided by a value obtained via calibration
		// times = how many readings to do
		float get_units(byte times = 1);

		// set the OFFSET value for tare weight; times = how many times to read the tare value
		void tare();

		// set the SCALE value; this value is used to convert the raw data to "human readable" data (measure units)
		void set_scale(float scale = 1.f);

		// set OFFSET, the value that's subtracted from the actual reading (tare weight)
		void set_offset(long offset = 0);
		
		// check stuff on the weight sensor
		bool stuff_is_detected();
		
		// puts the chip into power down mode
		void power_down();

		// wakes up the chip after power down mode
		void power_up();
};
#ifdef ELECTRONIC_SCALE_PROBE
	extern HX711 scale;
#endif
#endif /* HX711_h */