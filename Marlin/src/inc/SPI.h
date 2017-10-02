#include <stdint.h>


#ifndef SPI_FULL_SPEED

/**
 * SPI speed where 0 <= index <= 6
 *
 * Approximate rates :
 *
 *  0 :  8 - 10 MHz
 *  1 :  4 - 5 MHz
 *  2 :  2 - 2.5 MHz
 *  3 :  1 - 1.25 MHz
 *  4 :  500 - 625 kHz
 *  5 :  250 - 312 kHz
 *  6 :  125 - 156 kHz
 *
 *  On AVR, actual speed is F_CPU/2^(1 + index).
 *  On other platforms, speed should be in range given above where possible.
 */

/** Set SCK to max rate */
#define SPI_FULL_SPEED 0
/** Set SCK rate to half max rate. */
#define SPI_HALF_SPEED 1
/** Set SCK rate to quarter max rate. */
#define SPI_QUARTER_SPEED 2
/** Set SCK rate to 1/8 max rate. */
#define SPI_EIGHTH_SPEED 3
/** Set SCK rate to 1/16 of max rate. */
#define SPI_SIXTEENTH_SPEED 4
/** Set SCK rate to 1/32 of max rate. */
#define SPI_SPEED_5 5
/** Set SCK rate to 1/64 of max rate. */
#define SPI_SPEED_6 6

// Standard SPI functions
/** Initialise SPI bus */
void spiBegin(void);
/** Configure SPI for specified SPI speed */
void spiInit(uint8_t spiRate);
/** Write single byte to SPI */
void spiSend(uint8_t b);
/** Read single byte from SPI */
uint8_t spiRec(void);
/** Read from SPI into buffer */
void spiRead(uint8_t* buf, uint16_t nbyte);
/** Write token and then write from 512 byte buffer to SPI (for SD card) */
void spiSendBlock(uint8_t token, const uint8_t* buf);

#endif
