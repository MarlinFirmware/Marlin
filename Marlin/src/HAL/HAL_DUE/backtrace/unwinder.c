/***************************************************************************
 * ARM Stack Unwinder, Michael.McTernan.2001@cs.bris.ac.uk
 * Updated, adapted and several bug fixes on 2018 by Eduardo José Tagle
 *
 * This program is PUBLIC DOMAIN.
 * This means that there is no copyright and anyone is able to take a copy
 * for free and use it as they wish, with or without modifications, and in
 * any context, commercially or otherwise. The only limitation is that I
 * don't guarantee that the software is fit for any purpose or accept any
 * liability for it's use or misuse - this software is without warranty.
 ***************************************************************************
 * File Description: Implementation of the interface into the ARM unwinder.
 **************************************************************************/

#ifdef ARDUINO_ARCH_SAM

#define MODULE_NAME "UNWINDER"

#include <stdio.h>
#include <string.h>
#include "unwinder.h"
#include "unwarm.h"
#include "unwarmbytab.h"


/* These symbols point to the start and end of stack */
extern const int _sstack;
extern const int _estack;

/* These symbols point to the start and end of the code section */
extern const int _sfixed;
extern const int _efixed;

/* These symbols point to the start and end of initialized data (could be SRAM functions!) */
extern const int _srelocate;
extern const int _erelocate;


/* Validate stack pointer (SP): It must be in the stack area */
static inline __attribute__((always_inline)) UnwResult validate_sp(const void* sp) {

  // SP must point into the allocated stack area
  if ((uint32_t)sp >= (uint32_t)&_sstack && (uint32_t)sp <= (uint32_t)&_estack)
    return UNWIND_SUCCESS;

  return UNWIND_INVALID_SP;
}

/* Validate code pointer (PC): It must be either in TEXT or in SRAM */
static inline __attribute__((always_inline)) UnwResult validate_pc(const void* pc) {

  // PC must point into the text (CODE) area
  if ((uint32_t)pc >= (uint32_t)&_sfixed && (uint32_t)pc <= (uint32_t)&_efixed)
    return UNWIND_SUCCESS;

  // Or into the SRAM function area
  if ((uint32_t)pc >= (uint32_t)&_srelocate && (uint32_t)pc <= (uint32_t)&_erelocate)
    return UNWIND_SUCCESS;

  return UNWIND_INVALID_PC;
}

/* These symbols point to the unwind index and should be provide by the linker script */
extern const UnwTabEntry __exidx_start[];
extern const UnwTabEntry __exidx_end[];

// Detect if unwind information is present or not
static int HasUnwindTableInfo(void) {
  return ((char*)(&__exidx_end) - (char*)(&__exidx_start)) > 16 ? 1 : 0; // 16 because there are default entries we can´t supress
}

UnwResult UnwindStart(UnwindFrame* frame, const UnwindCallbacks *cb, void *data)
{
  if (HasUnwindTableInfo()) {

    /* We have unwind information tables */
    return UnwindByTableStart(frame, cb, data);

  } else {

    /* We don't have unwind information tables */
    UnwState state;

    /* Initialise the unwinding state */
    UnwInitState(&state, cb, data, frame->pc, frame->sp);

    /* Check the Thumb bit */
    if(frame->pc & 0x1) {
      return UnwStartThumb(&state);
    }
    else {
      return UnwStartArm(&state);
    }
  }
}
#endif


