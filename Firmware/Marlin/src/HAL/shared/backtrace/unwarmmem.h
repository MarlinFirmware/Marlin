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
 * File Description: Interface to the memory tracking sub-system.
 **************************************************************************/

#pragma once

#include "unwarm.h"

bool UnwMemHashRead(MemData * const memData, uint32_t addr, uint32_t * const data, bool * const tracked);
bool UnwMemHashWrite(MemData * const memData, uint32_t addr, uint32_t val, bool valValid);
void UnwMemHashGC(UnwState * const state);
