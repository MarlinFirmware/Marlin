/*------------------------------------------------------------------------/
/  LPC176x RTC control module
/-------------------------------------------------------------------------/
/
/  Copyright (C) 2011, ChaN, all right reserved.
/
/ * This software is a free software and there is NO WARRANTY.
/ * No restriction on use. You can use, modify and redistribute it for
/   personal, non-profit or commercial products UNDER YOUR RESPONSIBILITY.
/ * Redistributions of source code must retain the above copyright notice.
/
/-------------------------------------------------------------------------*/

#include "rtc176x.h"


int rtc_initialize (void)
{
	/* Enable PCLK to the RTC */
	__set_PCONP(PCRTC, 1);

	/* Start RTC with external XTAL */
	RTC_CCR = 0x11;

	return 1;
}



int rtc_gettime (RTC *rtc)	/* 1:RTC valid, 0:RTC volatiled */
{
	DWORD d, t;


	do {
		t = RTC_CTIME0;
		d = RTC_CTIME1;
	} while (t != RTC_CTIME0 || d != RTC_CTIME1);

	if (RTC_AUX & _BV(4)) {	/* If power fail has been detected, return default time. */
		rtc->sec = 0; rtc->min = 0; rtc->hour = 0;
		rtc->wday = 0; rtc->mday = 1; rtc->month = 1; rtc->year = 2014;
		return 0;
	}

	rtc->sec = t & 63;
	rtc->min = (t >> 8) & 63;
	rtc->hour = (t >> 16) & 31;
	rtc->wday = (t >> 24) & 7;
	rtc->mday = d & 31;
	rtc->month = (d >> 8) & 15;
	rtc->year = (d >> 16) & 4095;
	return 1;
}




int rtc_settime (const RTC *rtc)
{
	RTC_CCR = 0x12;		/* Stop RTC */

	/* Update RTC registers */
	RTC_SEC = rtc->sec;
	RTC_MIN = rtc->min;
	RTC_HOUR = rtc->hour;
	RTC_DOW = rtc->wday;
	RTC_DOM = rtc->mday;
	RTC_MONTH = rtc->month;
	RTC_YEAR = rtc->year;

	RTC_AUX = _BV(4);	/* Clear power fail flag */
	RTC_CCR = 0x11;		/* Restart RTC, Disable calibration feature */

	return 1;
}

DWORD get_fattime (void) {
  RTC rtc;

  /* Get local time */
  rtc_gettime(&rtc);

  /* Pack date and time into a DWORD variable */
  return    ((DWORD)(rtc.year - 1980) << 25)
      | ((DWORD)rtc.month << 21)
      | ((DWORD)rtc.mday << 16)
      | ((DWORD)rtc.hour << 11)
      | ((DWORD)rtc.min << 5)
      | ((DWORD)rtc.sec >> 1);
}
