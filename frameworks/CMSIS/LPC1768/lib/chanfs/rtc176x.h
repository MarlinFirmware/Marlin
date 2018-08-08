#ifndef _RTC_DEFINED
#define _RTC_DEFINE

#include "integer.h"
#include "LPC176x.h"

typedef struct {
	WORD	year;	/* 1..4095 */
	BYTE	month;	/* 1..12 */
	BYTE	mday;	/* 1.. 31 */
	BYTE	wday;	/* 1..7 */
	BYTE	hour;	/* 0..23 */
	BYTE	min;	/* 0..59 */
	BYTE	sec;	/* 0..59 */
} RTC;

int rtc_initialize (void);		/* Initialize RTC */
int rtc_gettime (RTC*);			/* Get time */
int rtc_settime (const RTC*);	/* Set time */
DWORD get_fattime (void);

#endif
