/*
 *  Source file for valve operation.
 *
 *  This file refers to material valves. Each material valve consists of one
 *  normall open solenoid and one normally closed valve connected to the same
 *  relay output.
 *
 *  Previous version of this file used two normally closed solenoids on
 *  different relay outputs to achieve the same effect.
 *
 *  Author: Ricky Rininger
 *  Date:   25 July, 2017
 */

#include "valves.h"

//===========================================================================
//  Functions
//===========================================================================

/*
 *  Initialize valves
 */
void valve_init(void) {
    // Set all outputs low
    VALVE_0_PORT &= ~VALVE_0;
    VALVE_1_PORT &= ~VALVE_1;
    VALVE_2_PORT &= ~VALVE_2;
    VALVE_3_PORT &= ~VALVE_3;

    // Set pin direction as output
    VALVE_0_DDR |= VALVE_0;
    VALVE_1_DDR |= VALVE_1;
    VALVE_2_DDR |= VALVE_2;
    VALVE_3_DDR |= VALVE_3;

    // Set all valves to "closed"
    // (remember, numbering starts at 1, not 0)
    for (uint8_t valve = 1; valve <= MAX_VALVES; valve++) {
        valve_close(valve);
    }
}

/*
 *  Opens given valve
 */
void valve_open(uint8_t valve) {
    switch (valve - 1) {
        case 0:
            VALVE_0_PORT |= VALVE_0;
            break;
        case 1:
            VALVE_1_PORT |= VALVE_1;
            break;
        case 2:
            VALVE_2_PORT |= VALVE_2;
            break;
        case 3:
            VALVE_3_PORT |= VALVE_3;
            break;
        default:
            // Do nothing
            break;
    }
}   // End valve_open

void valve_close(uint8_t valve) {
    switch (valve - 1) {
        case 0:
            VALVE_0_PORT &= ~VALVE_0;
            break;
        case 1:
            VALVE_1_PORT &= ~VALVE_1;
            break;
        case 2:
            VALVE_2_PORT &= ~VALVE_2;
            break;
        case 3:
            VALVE_3_PORT &= ~VALVE_3;
            break;
        default:
            // Do nothing
            break;
    }
}   // End valve_close

