#ifndef _ITOA_
#define _ITOA_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    extern char *itoa(int value, char *string, int radix);
    extern char *ltoa(long value, char *string, int radix);
    extern char *utoa(unsigned value, char *string, int radix);
    extern char *ultoa(unsigned long value, char *string, int radix);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // _ITOA_
