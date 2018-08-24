/***************************************************************************
 * ARM Stack Unwinder, Michael.McTernan.2001@cs.bris.ac.uk
 * Updated, adapted and several bug fixes on 2018 by Eduardo José Tagle
 *
 * This program is PUBLIC DOMAIN.
 * This means that there is no copyright and anyone is able to take a copy
 * for free and use it as they wish, with or without modifications, and in
 * any context, commerically or otherwise. The only limitation is that I
 * don't guarantee that the software is fit for any purpose or accept any
 * liablity for it's use or misuse - this software is without warranty.
 **************************************************************************/
/** \file
 * Interface to the ARM stack unwinding module.
 **************************************************************************/

#ifndef UNWINDER_H
#define UNWINDER_H

#include <stdint.h>

/** \def UNW_DEBUG
 * If this define is set, additional information will be produced while
 * unwinding the stack to allow debug of the unwind module itself.
 */
/* #define UNW_DEBUG 1 */

/***************************************************************************
 * Type Definitions
 **************************************************************************/

/** Possible results for UnwindStart to return.
 */
typedef enum {
  /** Unwinding was successful and complete. */
  UNWIND_SUCCESS = 0,

  /** Not an error: More frames are available. */
  UNWIND_MORE_AVAILABLE = 1,

  /** Unsupported DWARF unwind personality. */
  UNWIND_UNSUPPORTED_DWARF_PERSONALITY = -1,

  /** Refused to perform unwind. */
  UNWIND_REFUSED = -2,

  /** Reached an invalid SP. */
  UNWIND_INVALID_SP = -3,

  /** Reached an invalid PC */
  UNWIND_INVALID_PC = -4,

  /** Unsupported DWARF instruction */
  UNWIND_UNSUPPORTED_DWARF_INSTR = -5,

  /** More than UNW_MAX_INSTR_COUNT instructions were interpreted. */
  UNWIND_EXHAUSTED = -6,

  /** Unwinding stopped because the reporting func returned false. */
  UNWIND_TRUNCATED = -7,

  /** Read data was found to be inconsistent. */
  UNWIND_INCONSISTENT = -8,

  /** Unsupported instruction or data found. */
  UNWIND_UNSUPPORTED = -9,

  /** General failure. */
  UNWIND_FAILURE = -10,

  /** Illegal instruction. */
  UNWIND_ILLEGAL_INSTR = -11,

  /** Unwinding hit the reset vector. */
  UNWIND_RESET = -12,

  /** Failed read for an instruction word. */
  UNWIND_IREAD_W_FAIL = -13,

  /** Failed read for an instruction half-word. */
  UNWIND_IREAD_H_FAIL = -14,

  /** Failed read for an instruction byte. */
  UNWIND_IREAD_B_FAIL = -15,

  /** Failed read for a data word. */
  UNWIND_DREAD_W_FAIL = -16,

  /** Failed read for a data half-word. */
  UNWIND_DREAD_H_FAIL = -17,

  /** Failed read for a data byte. */
  UNWIND_DREAD_B_FAIL = -18,

  /** Failed write for a data word. */
  UNWIND_DWRITE_W_FAIL = -19

} UnwResult;

/** A backtrace report */
typedef struct {
  uint32_t function;  /** Starts address of function */
  const char *name;   /** Function name, or null if not available */
  uint32_t address;   /** PC on that function */
} UnwReport;

/** Type for function pointer for result callback.
 * The function is passed two parameters, the first is a void * pointer,
 * and the second is the return address of the function.  The bottom bit
 * of the passed address indicates the execution mode; if it is set,
 * the execution mode at the return address is Thumb, otherwise it is
 * ARM.
 *
 * The return value of this function determines whether unwinding should
 * continue or not.  If true is returned, unwinding will continue and the
 * report function maybe called again in future.  If false is returned,
 * unwinding will stop with UnwindStart() returning UNWIND_TRUNCATED.
 */
typedef bool (*UnwindReportFunc)(void* data, const UnwReport* bte);

/** Structure that holds memory callback function pointers.
 */
typedef struct {

  /** Report an unwind result. */
  UnwindReportFunc report;

  /** Read a 32 bit word from memory.
   * The memory address to be read is passed as \a address, and
   * \a *val is expected to be populated with the read value.
   * If the address cannot or should not be read, false can be
   * returned to indicate that unwinding should stop.  If true
   * is returned, \a *val is assumed to be valid and unwinding
   * will continue.
   */
  bool (*readW)(const uint32_t address, uint32_t *val);

  /** Read a 16 bit half-word from memory.
   * This function has the same usage as for readW, but is expected
   * to read only a 16 bit value.
   */
  bool (*readH)(const uint32_t address, uint16_t *val);

  /** Read a byte from memory.
   * This function has the same usage as for readW, but is expected
   * to read only an 8 bit value.
   */
  bool (*readB)(const uint32_t address, uint8_t  *val);

  #ifdef UNW_DEBUG
    /** Print a formatted line for debug. */
    void (*printf)(const char *format, ...);
  #endif
} UnwindCallbacks;

/* A frame */
typedef struct {
  uint32_t fp;
  uint32_t sp;
  uint32_t lr;
  uint32_t pc;
} UnwindFrame;

/** Start unwinding the current stack.
 * This will unwind the stack starting at the PC value supplied to in the
 * link register (i.e. not a normal register) and the stack pointer value
 * supplied.
 *
 * -If the program was compiled with -funwind-tables , it will use them to
 * perform the traceback. Otherwise, brute force will be employed
 * -If the program was compiled with -mpoke-function-name, then you will
 * get function names in the traceback. Otherwise, you will not.
 */
UnwResult UnwindStart(UnwindFrame* frame, const UnwindCallbacks *cb, void *data);

#endif /* UNWINDER_H */
