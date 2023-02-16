/**
 * write printf to Serial
 * printf implementation taken from https://gist.github.com/ridencww/4e5d10097fee0b0f7f6b
 */
#include "../src/inc/MarlinConfig.h"

#ifdef REDIRECT_PRINTF_TO_SERIAL
#define PRINTF_TARGET MYSERIAL1

void printChar(char c)
{
    // marlin serial does not have a way to print a single char ?!

    // seems like marlin's serial class does not
    // have a function to print a single char
    // so i'm creating a c string with the char that i want to print
    char str[2] = {c, '\0'};
    PRINTF_TARGET.print(str);
}

extern "C" int printf(const char *fmt, ...)
{
    va_list argv;
    va_start(argv, fmt);

    for (int i = 0; fmt[i] != '\0'; i++)
    {
        if (fmt[i] == '%')
        {
            // Look for specification of number of decimal places
            int places = 2;
            if (fmt[i + 1] == '.')
                i++; // alw1746: Allows %.4f precision like in stdio printf (%4f will still work).
            if (fmt[i + 1] >= '0' && fmt[i + 1] <= '9')
            {
                places = fmt[i + 1] - '0';
                i++;
            }

            switch (fmt[++i])
            {
            case 'B':
                PRINTF_TARGET.print("0b"); // Fall through intended
            case 'b':
                PRINTF_TARGET.print(va_arg(argv, int), PrintBase::Bin);
                break;
            case 'c':
                // pc
                printChar((char)va_arg(argv, int));
                // PRINTF_TARGET.print((char)va_arg(argv, int));
                break;
            case 'd':
            case 'i':
                PRINTF_TARGET.print(va_arg(argv, int), PrintBase::Dec);
                break;
            case 'f':
                PRINTF_TARGET.print(va_arg(argv, double), places);
                break;
            case 'l':
                PRINTF_TARGET.print(va_arg(argv, long), PrintBase::Dec);
                break;
            case 'o':
                PRINTF_TARGET.print(va_arg(argv, int) == 0 ? "off" : "on");
                break;
            case 's':
                PRINTF_TARGET.print(va_arg(argv, const char *));
                break;
            case 'X':
                PRINTF_TARGET.print("0x"); // Fall through intended
            case 'x':
                PRINTF_TARGET.print(va_arg(argv, int), PrintBase::Hex);
                break;
            case '%':
                PRINTF_TARGET.print("%");
                break;
            default:
                PRINTF_TARGET.print("?");
                break;
            }
        }
        else
        {
            printChar(fmt[i]);
        }
    }

    va_end(argv);
    return 0;
}
#endif
