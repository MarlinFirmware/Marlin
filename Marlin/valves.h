/*
 *  Header file for valves.cpp
 *
 *  Author: Ricky Rininger
 *  Date:   25 July, 2017
 */

#include <avr/io.h>

#ifndef VALVES_H
#define VALVES_H

//===========================================================================
//  Defines
//===========================================================================

// Constant Definitions
#define MAX_VALVES              (4)

// Port Definitions
#define VALVE_0_PORT            PORTG
#define VALVE_1_PORT            PORTG
#define VALVE_2_PORT            PORTJ
#define VALVE_3_PORT            PORTJ

// DDR Definitions
#define VALVE_0_DDR             DDRG
#define VALVE_1_DDR             DDRG
#define VALVE_2_DDR             DDRJ
#define VALVE_3_DDR             DDRJ

// Pin Definitions
#define VALVE_0                 (1 << 4)
#define VALVE_1                 (1 << 3)
#define VALVE_2                 (1 << 2)
#define VALVE_3                 (1 << 3)


//===========================================================================
//  Function Prototypes
//===========================================================================

/*
 *  Init valves
 */
void valve_init();

/*
 *  Opens given valve
 */
void valve_open(uint8_t valve);

/*
 *  Closes given valve
 */
void valve_close(uint8_t valve);

#endif // VALVES_H

