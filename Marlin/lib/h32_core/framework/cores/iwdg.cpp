/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 Michael Hope.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

/**
 * @file libmaple/iwdg.c
 * @brief Independent watchdog (IWDG) support
 */

#include "../board/startup.h"
#include "iwdg.h"

/* WDT count cycle definition */
#define WDT_COUNT_CYCLE (16384u)

void iwdg_init(void)
{
    stc_wdt_init_t stcWdtInit;
    RMU_ClrResetFlag();
    /* WDT configure */
    MEM_ZERO_STRUCT(stcWdtInit);

    stcWdtInit.enClkDiv = WdtPclk3Div2048;
    stcWdtInit.enCountCycle = WdtCountCycle65536;
    stcWdtInit.enRefreshRange = WdtRefresh0To100Pct;
    stcWdtInit.enSleepModeCountEn = Disable;
    stcWdtInit.enRequsetType = WdtTriggerResetRequest;
    WDT_Init(&stcWdtInit);
    /* First refresh to start WDT */
    WDT_RefreshCounter();
    /* Wait for WDT module to complete initial load */
    Ddl_Delay1ms(200u);
}

void iwdg_feed(void)
{
    WDT_RefreshCounter();
}
