/***************************************************************************
 * ARM Stack Unwinder, Michael.McTernan.2001@cs.bris.ac.uk
 *
 * This program is PUBLIC DOMAIN.
 * This means that there is no copyright and anyone is able to take a copy
 * for free and use it as they wish, with or without modifications, and in
 * any context, commercially or otherwise. The only limitation is that I
 * don't guarantee that the software is fit for any purpose or accept any
 * liability for its use or misuse - this software is without warranty.
 ***************************************************************************
 * File Description: Internal interface between the ARM unwinding sub-modules.
 **************************************************************************/

#pragma once

#include "unwinder.h"

/** The maximum number of instructions to interpet in a function.
 * Unwinding will be unconditionally stopped and UNWIND_EXHAUSTED returned
 * if more than this number of instructions are interpreted in a single
 * function without unwinding a stack frame.  This prevents infinite loops
 * or corrupted program memory from preventing unwinding from progressing.
 */
#define UNW_MAX_INSTR_COUNT 500

/** The size of the hash used to track reads and writes to memory.
 * This should be a prime value for efficiency.
 */
#define MEM_HASH_SIZE       31

/***************************************************************************
 * Type Definitions
 **************************************************************************/

typedef enum {
  /** Invalid value. */
  REG_VAL_INVALID     = 0x00,
  REG_VAL_FROM_STACK  = 0x01,
  REG_VAL_FROM_MEMORY = 0x02,
  REG_VAL_FROM_CONST  = 0x04,
  REG_VAL_ARITHMETIC  = 0x80
} RegValOrigin;


/** Type for tracking information about a register.
 * This stores the register value, as well as other data that helps unwinding.
 */
typedef struct {

  /** The value held in the register. */
  uint32_t v;

  /** The origin of the register value.
   * This is used to track how the value in the register was loaded.
   */
  int o; /* (RegValOrigin) */
} RegData;


/** Structure used to track reads and writes to memory.
 * This structure is used as a hash to store a small number of writes
 * to memory.
 */
typedef struct {
  /** Memory contents. */
  uint32_t v[MEM_HASH_SIZE];

  /** Address at which v[n] represents. */
  uint32_t a[MEM_HASH_SIZE];

  /** Indicates whether the data in v[n] and a[n] is occupied.
   * Each bit represents one hash value.
   */
  uint8_t used[(MEM_HASH_SIZE + 7) / 8];

  /** Indicates whether the data in v[n] is valid.
   * This allows a[n] to be set, but for v[n] to be marked as invalid.
   * Specifically this is needed for when an untracked register value
   * is written to memory.
   */
  uint8_t tracked[(MEM_HASH_SIZE + 7) / 8];
} MemData;


/** Structure that is used to keep track of unwinding meta-data.
 * This data is passed between all the unwinding functions.
 */
typedef struct {
  /** The register values and meta-data. */
  RegData regData[16];

  /** Memory tracking data. */
  MemData memData;

  /** Pointer to the callback functions */
  const UnwindCallbacks *cb;

  /** Pointer to pass to the report function. */
  const void *reportData;
} UnwState;

/***************************************************************************
 *  Macros
 **************************************************************************/

#define M_IsOriginValid(v) !!((v) & 0x7F)
#define M_Origin2Str(v)    ((v) ? "VALID" : "INVALID")

#ifdef UNW_DEBUG
#define UnwPrintd1(a)               state->cb->printf(a)
#define UnwPrintd2(a,b)             state->cb->printf(a,b)
#define UnwPrintd3(a,b,c)           state->cb->printf(a,b,c)
#define UnwPrintd4(a,b,c,d)         state->cb->printf(a,b,c,d)
#define UnwPrintd5(a,b,c,d,e)       state->cb->printf(a,b,c,d,e)
#define UnwPrintd6(a,b,c,d,e,f)     state->cb->printf(a,b,c,d,e,f)
#define UnwPrintd7(a,b,c,d,e,f,g)   state->cb->printf(a,b,c,d,e,f,g)
#define UnwPrintd8(a,b,c,d,e,f,g,h) state->cb->printf(a,b,c,d,e,f,g,h)
#else
#define UnwPrintd1(a)
#define UnwPrintd2(a,b)
#define UnwPrintd3(a,b,c)
#define UnwPrintd4(a,b,c,d)
#define UnwPrintd5(a,b,c,d,e)
#define UnwPrintd6(a,b,c,d,e,f)
#define UnwPrintd7(a,b,c,d,e,f,g)
#define UnwPrintd8(a,b,c,d,e,f,g,h)
#endif

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/

UnwResult UnwStartArm(UnwState * const state);
UnwResult UnwStartThumb(UnwState * const state);
void UnwInvalidateRegisterFile(RegData *regFile);
void UnwInitState(UnwState * const state, const UnwindCallbacks *cb, void *rptData, uint32_t pcValue, uint32_t spValue);
bool UnwReportRetAddr(UnwState * const state, uint32_t addr);
bool UnwMemWriteRegister(UnwState * const state, const uint32_t addr, const RegData * const reg);
bool UnwMemReadRegister(UnwState * const state, const uint32_t addr, RegData * const reg);
void UnwMemHashGC(UnwState * const state);
