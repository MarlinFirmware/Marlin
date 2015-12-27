#include "Configuration.h"

#ifdef DIGIPOT_I2C
#include "Stream.h"
#include "utility/twi.h"
#include "Wire.h"

// Settings for the I2C based DIGIPOT (MCP4451) on Azteeg X3 Pro
#if MOTHERBOARD == 88
#define DIGIPOT_I2C_FACTOR 117.96
#define DIGIPOT_I2C_MAX_CURRENT 1.736
#else
#define DIGIPOT_I2C_FACTOR 106.7
#define DIGIPOT_I2C_MAX_CURRENT 2.5
#endif

static byte current_to_wiper( float current ){
    return byte(ceil(float((DIGIPOT_I2C_FACTOR*current))));
}

static void i2c_send(byte addr, byte a, byte b)
{
	Wire.beginTransmission(addr);
    Wire.write(a);
    Wire.write(b);
    Wire.endTransmission();
}

// This is for the MCP4451 I2C based digipot
void digipot_i2c_set_current( int channel, float current )
{
    current = min( (float) max( current, 0.0f ), DIGIPOT_I2C_MAX_CURRENT);
    // these addresses are specific to Azteeg X3 Pro, can be set to others,
    // In this case first digipot is at address A0=0, A1= 0, second one is at A0=0, A1= 1
    byte addr= 0x2C; // channel 0-3
    if(channel >= 4) {
    	addr= 0x2E; // channel 4-7
    	channel-= 4;
    }

    // Initial setup
    i2c_send( addr, 0x40, 0xff );
    i2c_send( addr, 0xA0, 0xff );

    // Set actual wiper value
    byte addresses[4] = { 0x00, 0x10, 0x60, 0x70 };
    i2c_send( addr, addresses[channel], current_to_wiper(current) );
}

void digipot_i2c_init()
{
    const float digipot_motor_current[] = DIGIPOT_I2C_MOTOR_CURRENTS;
    Wire.begin();
    // setup initial currents as defined in Configuration_adv.h
    for(int i=0;i<=sizeof(digipot_motor_current)/sizeof(float);i++) {
        digipot_i2c_set_current(i, digipot_motor_current[i]);
    }
}
#endif
