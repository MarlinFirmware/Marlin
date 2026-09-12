#ifndef __DSHOTAT32_H
#define __DSHOTAT32_H

#include "at32f403a_407.h"


#define DSHOT600 600000
#define MOTOR_COUNT 4
#define DSHOT_BITS 16
#define DSHOT_BUFFER_LEN (DSHOT_BITS + 2)

uint16_t dshot_preparee(uint16_t value);
void dshot_initt(void);
void dshot_build_framee(uint16_t *buf, uint16_t throttle);
void dshot_send_all(uint16_t m1, uint16_t m2, uint16_t m3, uint16_t m4);




#endif