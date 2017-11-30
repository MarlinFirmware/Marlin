/*
 *  Source file for solenoids.
 *
 *  Contains functions to activate and deactivate solenoids
 *
 *  Author: Ricky Rininger
 *  Date:   25 July, 2017
 */

#include "solenoids.h"

//===========================================================================
//  Functions
//===========================================================================

/*
 *  Initialize solenoids
 */
void solenoid_init(void) {
    // Set all outputs low
    SOLENOID_0_PORT &= ~SOLENOID_0;
    SOLENOID_1_PORT &= ~SOLENOID_1;
    SOLENOID_2_PORT &= ~SOLENOID_2;
    SOLENOID_3_PORT &= ~SOLENOID_3;

    // Set pin direction as output
    SOLENOID_0_DDR |= SOLENOID_0;
    SOLENOID_1_DDR |= SOLENOID_1;
    SOLENOID_2_DDR |= SOLENOID_2;
    SOLENOID_3_DDR |= SOLENOID_3;

    // Set all solenoids to "inactive"
    // (remember, index starts at 1 and not 0)
    for (uint8_t solenoid = 1; solenoid <= MAX_SOLENOIDS; solenoid++) {
        solenoid_deactivate(solenoid);
    }
}

/*
 *  Activate given solenoid
 */
void solenoid_activate(uint8_t solenoid) {
    switch (solenoid - 1) {
        case 0:
            SOLENOID_0_PORT |= SOLENOID_0;
            break;
        case 1:
            SOLENOID_1_PORT |= SOLENOID_1;
            break;
        case 2:
            SOLENOID_2_PORT |= SOLENOID_2;
            break;
        case 3:
            SOLENOID_3_PORT |= SOLENOID_3;
            break;
        default:
            // Do nothing
            break;
    }
}

/*
 *  Deactivate given solenoid
 */
void solenoid_deactivate(uint8_t solenoid) {
    switch (solenoid - 1) {
        case 0:
            SOLENOID_0_PORT &= ~SOLENOID_0;
            break;
        case 1:
            SOLENOID_1_PORT &= ~SOLENOID_1;
            break;
        case 2:
            SOLENOID_2_PORT &= ~SOLENOID_2;
            break;
        case 3:
            SOLENOID_3_PORT &= ~SOLENOID_3;
            break;
        default:
            // Do nothing
            break;
    }
}

