
#ifndef _WIRISH_WIRISH_MATH_H_
#define _WIRISH_WIRISH_MATH_H_

#include <math.h>
#include <stdint.h>

void randomSeed(unsigned int seed);
long random(long max);
long random(long min, long max);

static inline int map(int value, int fromStart, int fromEnd,
                      int toStart, int toEnd)
{
  return ((value - fromStart) * (toEnd - toStart)) / (fromEnd - fromStart) + toStart;
}

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105

#define EULER 2.718281828459045235360287471352
#define SERIAL 0x0
#define DISPLAY 0x1

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))
#define round(x) ((x) >= 0 ? (long)((x) + 0.5) : (long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x) * (x))

#define abs(x) (((x) > 0) ? (x) : -(x))

#ifdef math
double cos(double x);
double sin(double x);
double tan(double x);
double sqrt(double x);
double pow(double x, double y);
#endif
extern uint16_t makeWord(uint16_t w);
extern uint16_t makeWord(uint8_t h, uint8_t l);

#define word(...) makeWord(__VA_ARGS__)

#endif
