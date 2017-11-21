/****************************************************************************
 *  Header file for solenoids.cpp
 *
 *  Author: Ricky Rininger
 *  Date:   25 July, 2017
 *
 *
 ***************************************************************************/

#ifndef SOLENOIDS_H_
#define SOLENOIDS_H_

//===========================================================================
// Pin Definitions
//===========================================================================
#define SOL_PORT_A      (PORTH)     // PORT for SOL_0 - SOL_2
#define SOL_DDR_A       (DDRH)      // DDR for SOL_0 - SOL_2

#define SOL_0           (_BV(6))    // PORT H PIN 6
#define SOL_1           (_BV(5))    // PORT H PIN 5
#define SOL_2           (_BV(4))    // PORT H PIN 4
#define SOL_3           (_BV(3))    // PORT H PIN 3

#define SOL_PORT_B      (PORTE)     // SOL 4 and 5 connectors uses PORTE
#define SOL_DDR_B       (DDRE)      // ^ uses DDRE

#define SOL_4           (_BV(5))    // PORT E PIN 5
#define SOL_5           (_BV(4))    // PORT E PIN 4

//===========================================================================
// Properties
//===========================================================================
#define NUM_SOLENOIDS   (6)

//===========================================================================
// Function Prototypes
//===========================================================================

/*
 *  Open given solenoid
 */
void solenoid_open(uint8_t solenoid);

/*
 *  Close given solenoid
 */
void solenoid_close(uint8_t solenoid);

#endif // SOLENOIDS_H

