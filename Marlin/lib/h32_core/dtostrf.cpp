/**
 * fixed dtostrf function. replaced the botched version in hdsc32core/lib/dtostrf.o.
 * Taken from https://github.com/alexqzd/Marlin-H32/blob/404a201487af58a959edf0af508211dc047c2f3b/firmware/Sources/framework/cores/dtostrf.cpp
 */

#include "dtostrf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *dtostrf(double val, signed char width, unsigned char prec, char *sout)
{
    // Handle negative numbers
    unsigned char negative = 0;
    if (val < 0.0)
    {
        negative = 1;
        val = -val;
    }

    // Round correctly so that print(1.999, 2) prints as "2.00"
    double rounding = 0.5;
    for (int i = 0; i < prec; ++i)
    {
        rounding /= 10.0;
    }

    val += rounding;

    // Extract the integer part of the number
    unsigned long int_part = (unsigned long)val;
    double remainder = val - (double)int_part;

    // Extract digits from the remainder
    unsigned long dec_part = 0;
    double decade = 1.0;
    for (int i = 0; i < prec; i++)
    {
        decade *= 10.0;
    }

    remainder *= decade;
    dec_part = (int)remainder;

    if (negative)
    {
        sprintf(sout, "-%ld.%0*ld", int_part, prec, dec_part);
    }
    else
    {
        sprintf(sout, "%ld.%0*ld", int_part, prec, dec_part);
    }

    // Handle minimum field width of the output string
    // width is signed value, negative for left adjustment.
    // Range -128,127
    char fmt[129] = "";
    unsigned int w = width;
    if (width < 0)
    {
        negative = 1;
        w = -width;
    }
    else
    {
        negative = 0;
    }

    if (strlen(sout) < w)
    {
        memset(fmt, ' ', 128);
        fmt[w - strlen(sout)] = '\0';
        if (negative == 0)
        {
            char *tmp = (char *)malloc(strlen(sout) + 1);
            strcpy(tmp, sout);
            strcpy(sout, fmt);
            strcat(sout, tmp);
            free(tmp);
        }
        else
        {
            // left adjustment
            strcat(sout, fmt);
        }
    }

    return sout;
}
