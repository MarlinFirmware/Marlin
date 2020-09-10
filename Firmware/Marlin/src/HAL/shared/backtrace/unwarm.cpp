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
 * File Description: Utility functions and glue for ARM unwinding sub-modules.
 **************************************************************************/

#if defined(__arm__) || defined(__thumb__)

#define MODULE_NAME "UNWARM"

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "unwarm.h"
#include "unwarmmem.h"

#ifdef UNW_DEBUG

/**
 * Printf wrapper.
 * This is used such that alternative outputs for any output can be selected
 * by modification of this wrapper function.
 */
void UnwPrintf(const char *format, ...) {
  va_list args;

  va_start( args, format );
  vprintf(format, args );
}
#endif

/**
 * Invalidate all general purpose registers.
 */
void UnwInvalidateRegisterFile(RegData *regFile) {
  uint8_t t = 0;
  do {
    regFile[t].o = REG_VAL_INVALID;
    t++;
  } while (t < 13);
}


/**
 * Initialize the data used for unwinding.
 */
void UnwInitState(UnwState * const state,     /**< Pointer to structure to fill. */
                  const UnwindCallbacks *cb,  /**< Callbacks. */
                  void *rptData,              /**< Data to pass to report function. */
                  uint32_t pcValue,           /**< PC at which to start unwinding. */
                  uint32_t spValue) {         /**< SP at which to start unwinding. */

  UnwInvalidateRegisterFile(state->regData);

  /* Store the pointer to the callbacks */
  state->cb = cb;
  state->reportData = rptData;

  /* Setup the SP and PC */
  state->regData[13].v = spValue;
  state->regData[13].o = REG_VAL_FROM_CONST;
  state->regData[15].v = pcValue;
  state->regData[15].o = REG_VAL_FROM_CONST;

  UnwPrintd3("\nInitial: PC=0x%08x SP=0x%08x\n", pcValue, spValue);

  /* Invalidate all memory addresses */
  memset(state->memData.used, 0, sizeof(state->memData.used));
}

// Detect if function names are available
static int __attribute__ ((noinline)) has_function_names() {
  uint32_t flag_word = ((uint32_t*)(((uint32_t)(&has_function_names)) & (-4))) [-1];
  return ((flag_word & 0xFF000000) == 0xFF000000) ? 1 : 0;
}

/**
 * Call the report function to indicate some return address.
 * This returns the value of the report function, which if true
 * indicates that unwinding may continue.
 */
bool UnwReportRetAddr(UnwState * const state, uint32_t addr) {

  UnwReport entry;

  // We found two acceptable values.
  entry.name = nullptr;
  entry.address = addr & 0xFFFFFFFE; // Remove Thumb bit
  entry.function = 0;

  // If there are function names, try to solve name
  if (has_function_names()) {

    // Lets find the function name, if possible

    // Align address to 4 bytes
    uint32_t pf = addr & (-4);

    // Scan backwards until we find the function name
    uint32_t v;
    while (state->cb->readW(pf-4,&v)) {

      // Check if name descriptor is valid
      if ((v & 0xFFFFFF00) == 0xFF000000 && (v & 0xFF) > 1) {
        // Assume the name was found!
        entry.name = ((const char*)pf) - 4 - (v & 0xFF);
        entry.function = pf;
        break;
      }

      // Go backwards to the previous word
      pf -= 4;
    }
  }

  /* Cast away const from reportData.
   *  The const is only to prevent the unw module modifying the data.
   */
  return state->cb->report((void *)state->reportData, &entry);
}


/**
 * Write some register to memory.
 * This will store some register and meta data onto the virtual stack.
 * The address for the write
 * \param state [in/out]  The unwinding state.
 * \param wAddr [in]  The address at which to write the data.
 * \param reg   [in]  The register to store.
 * \return true if the write was successful, false otherwise.
 */
bool UnwMemWriteRegister(UnwState * const state, const uint32_t addr, const RegData * const reg) {
  return UnwMemHashWrite(&state->memData, addr, reg->v, M_IsOriginValid(reg->o));
}

/**
 * Read a register from memory.
 * This will read a register from memory, and setup the meta data.
 * If the register has been previously written to memory using
 * UnwMemWriteRegister, the local hash will be used to return the
 * value while respecting whether the data was valid or not.  If the
 * register was previously written and was invalid at that point,
 * REG_VAL_INVALID will be returned in *reg.
 * \param state [in]  The unwinding state.
 * \param addr  [in]  The address to read.
 * \param reg   [out] The result, containing the data value and the origin
 *                     which will be REG_VAL_FROM_MEMORY, or REG_VAL_INVALID.
 * \return true if the address could be read and *reg has been filled in.
 *         false is the data could not be read.
 */
bool UnwMemReadRegister(UnwState * const state, const uint32_t addr, RegData * const reg) {
  bool tracked;

  // Check if the value can be found in the hash
  if (UnwMemHashRead(&state->memData, addr, &reg->v, &tracked)) {
    reg->o = tracked ? REG_VAL_FROM_MEMORY : REG_VAL_INVALID;
    return true;
  }
  else if (state->cb->readW(addr, &reg->v)) {   // Not in the hash, so read from real memory
    reg->o = REG_VAL_FROM_MEMORY;
    return true;
  }
  else return false;                            // Not in the hash, and failed to read from memory
}

#endif // __arm__ || __thumb__
