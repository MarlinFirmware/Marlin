/*
 * Libbacktrace
 * Copyright 2015 Stephen Street <stephen@redrocketcomputing.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This library was modified and adapted to be used in Marlin 3D printer
 * firmware as backtracer for exceptions for debugging purposes in 2018
 * by Eduardo José Tagle.
 */

/*
 * For this library to work, you need to compile with the following options
 * -funwind-tables => So we will have unwind information to perform the stack trace
 * -mpoke-function-name => So we will have function names in the trace
 */

#ifndef _BACKTRACE_H_
#define _BACKTRACE_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* A frame */
typedef struct backtrace_frame {
  uint32_t fp;
  uint32_t sp;
  uint32_t lr;
  uint32_t pc;
} backtrace_frame_t;

/* A backtrace */
typedef struct backtrace {
  void *function;
  void *address;
  const char *name;
} backtrace_t;

typedef void (*backtrace_dump_fn_t)(int idx, const backtrace_t* bte, void* ctx);

/* Perform a backtrace, given the specified stack start frame */
int backtrace_dump(backtrace_frame_t *startframe, backtrace_dump_fn_t fn, void* ctx );

#ifdef __cplusplus
}
#endif

#endif // _BACKTRACE_H_
