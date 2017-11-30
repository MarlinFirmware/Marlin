/****************************************************************************
 *  Header file for solenoids.cpp
 *
 *  Author: Ricky Rininger
 *  Date:   25 July, 2017
 *
 *
 ***************************************************************************/
#include <avr/io.h>

#ifndef SOLENOIDS_H_
#define SOLENOIDS_H_

//===========================================================================
// Pin Definitions
//===========================================================================

// Constant Definitions
#define MAX_SOLENOIDS           (4)

// Port Definitions
#define SOLENOID_0_PORT         PORTD
#define SOLENOID_1_PORT         PORTE
#define SOLENOID_2_PORT         PORTE
#define SOLENOID_3_PORT         PORTE

// DDR Definitions
#define SOLENOID_0_DDR          DDRD
#define SOLENOID_1_DDR          DDRE
#define SOLENOID_2_DDR          DDRE
#define SOLENOID_3_DDR          DDRE

// Pin Definitions
#define SOLENOID_0              (1 << 4)
#define SOLENOID_1              (1 << 7)
#define SOLENOID_2              (1 << 6)
#define SOLENOID_3              (1 << 2)


//===========================================================================
// Function Prototypes
//===========================================================================

/*
 *  Init solenoids
 */
void solenoid_init();

/*
 *  Activate given solenoid
 */
void solenoid_activate(uint8_t solenoid);

/*
 *  Deactivate given solenoid
 */
void solenoid_deactivate(uint8_t solenoid);

#endif // SOLENOIDS_H

