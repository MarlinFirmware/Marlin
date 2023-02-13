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
 * @file libmaple/include/libmaple/iwdg.h
 * @author Michael Hope, Marti Bolivar <mbolivar@leaflabs.com>
 * @brief Independent watchdog support.
 *
 * To use the independent watchdog, first call iwdg_init() with the
 * appropriate prescaler and IWDG counter reload values for your
 * application.  Afterwards, you must periodically call iwdg_feed()
 * before the IWDG counter reaches 0 to reset the counter to its
 * reload value.  If you do not, the chip will reset.
 *
 * Once started, the independent watchdog cannot be turned off.
 */

#ifndef _LIBMAPLE_IWDG_H_
#define _LIBMAPLE_IWDG_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "libmaple_types.h"

    void iwdg_init(void);
    void iwdg_feed(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
