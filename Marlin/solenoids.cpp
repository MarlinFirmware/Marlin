/*
 *  Source file for solenoids.
 *
 *  Contains functions to open and close solenoids
 *
 *  Author: Ricky Rininger
 *  Date:   25 July, 2017
 */

#include <avr/io.h>
#include <stdint.h>

#include "solenoids.h"

//===========================================================================
//  Functions
//===========================================================================

/*
 *  Open given solenoid
 */
void solenoid_open(uint8_t solenoid) {
    switch (solenoid) {
        case 0:
            SOL_PORT_A |= SOL_0;
            break;
        case 1:
            SOL_PORT_A |= SOL_1;
            break;
        case 2:
            SOL_PORT_A |= SOL_2;
            break;
        case 3:
            SOL_PORT_A |= SOL_3;
            break;
        case 4:
            SOL_PORT_B |= SOL_4;
            break;
        case 5:
            SOL_PORT_B |= SOL_5;
            break;
        default:
            // Do nothing
            break;
    }
}

/*
 *  Close given solenoid
 */
void solenoid_close(uint8_t solenoid) {
    switch (solenoid) {
        case 0:
            SOL_PORT_A &= ~SOL_0;
            break;
        case 1:
            SOL_PORT_A &= ~SOL_1;
            break;
        case 2:
            SOL_PORT_A &= ~SOL_2;
            break;
        case 3:
            SOL_PORT_A &= ~SOL_3;
            break;
        case 4:
            SOL_PORT_B &= ~SOL_4;
            break;
        case 5:
            SOL_PORT_B &= ~SOL_5;
            break;
        default:
            // Do nothing
            break;
    }
}

