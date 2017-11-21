/*
 *  Header file for valves.cpp
 *
 *  Author: Ricky Rininger
 *  Date:   25 July, 2017
 */

#ifndef VALVES_H
#define VALVES_H

//===========================================================================
//  Defines
//===========================================================================

// Constant Definitions
#define MAX_VALVES              (4)

// Port Definitions
#define VALVE
#define VALVE
#define VALVE
#define VALVE
//===========================================================================
//  Function Prototypes
//===========================================================================

/*
 *  Opens given valve
 */
void valve_open(uint8_t valve);

/*
 *  Closes given valve
 */
void valve_close(uint8_t valve);

#endif // VALVES_H

