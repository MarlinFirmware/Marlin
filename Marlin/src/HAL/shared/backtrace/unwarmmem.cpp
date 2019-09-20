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
 ***************************************************************************
 * File Description: Implementation of the memory tracking sub-system.
 **************************************************************************/

#if defined(__arm__) || defined(__thumb__)
#define MODULE_NAME "UNWARMMEM"

#include <stdio.h>
#include "unwarmmem.h"
#include "unwarm.h"

#define M_IsIdxUsed(a, v) !!((a)[v >> 3] & (1 << (v & 0x7)))
#define M_SetIdxUsed(a, v) ((a)[v >> 3] |= (1 << (v & 0x7)))
#define M_ClrIdxUsed(a, v) ((a)[v >> 3] &= ~(1 << (v & 0x7)))

/** Search the memory hash to see if an entry is stored in the hash already.
 * This will search the hash and either return the index where the item is
 * stored, or -1 if the item was not found.
 */
static int16_t memHashIndex(MemData * const memData, const uint32_t addr) {

  const uint16_t v = addr % MEM_HASH_SIZE;
  uint16_t       s = v;

  do {
    /* Check if the element is occupied */
    if (M_IsIdxUsed(memData->used, s)) {
      /* Check if it is occupied with the sought data */
      if (memData->a[s] == addr) return s;
    }
    else {
      /* Item is free, this is where the item should be stored */
      return s;
    }

    /* Search the next entry */
    s++;
    if (s > MEM_HASH_SIZE) s = 0;
  } while (s != v);

  /* Search failed, hash is full and the address not stored */
  return -1;
}

bool UnwMemHashRead(MemData * const memData, uint32_t addr,uint32_t * const data, bool * const tracked) {

  const int16_t i = memHashIndex(memData, addr);

  if (i >= 0 && M_IsIdxUsed(memData->used, i) && memData->a[i] == addr) {
    *data    = memData->v[i];
    *tracked = M_IsIdxUsed(memData->tracked, i);
    return true;
  }
  else {
    /* Address not found in the hash */
    return false;
  }
}

bool UnwMemHashWrite(MemData * const memData, uint32_t addr, uint32_t val, bool valValid) {
  const int16_t i = memHashIndex(memData, addr);
  if (i < 0) return false; /* Hash full */

  /* Store the item */
  memData->a[i] = addr;
  M_SetIdxUsed(memData->used, i);

  if (valValid) {
    memData->v[i] = val;
    M_SetIdxUsed(memData->tracked, i);
  }
  else {
    #ifdef UNW_DEBUG
      memData->v[i] = 0xDEADBEEF;
    #endif
    M_ClrIdxUsed(memData->tracked, i);
  }

  return true;
}

void UnwMemHashGC(UnwState * const state) {

  const uint32_t minValidAddr = state->regData[13].v;
  MemData * const memData  = &state->memData;
  uint16_t t;

  for (t = 0; t < MEM_HASH_SIZE; t++) {
    if (M_IsIdxUsed(memData->used, t) && (memData->a[t] < minValidAddr)) {
      UnwPrintd3("MemHashGC: Free elem %d, addr 0x%08x\n", t, memData->a[t]);
      M_ClrIdxUsed(memData->used, t);
    }
  }
}

#endif // __arm__ || __thumb__
