/***************************************************************************
 * ARM Stack Unwinder, Michael.McTernan.2001@cs.bris.ac.uk
 * Updated, adapted and several bug fixes on 2018 by Eduardo José Tagle
 *
 * This program is PUBLIC DOMAIN.
 * This means that there is no copyright and anyone is able to take a copy
 * for free and use it as they wish, with or without modifications, and in
 * any context, commerically or otherwise. The only limitation is that I
 * don't guarantee that the software is fit for any purpose or accept any
 * liability for its use or misuse - this software is without warranty.
 ***************************************************************************
 * File Description: Utility functions to access memory
 **************************************************************************/

#pragma once

#include "unwarm.h"
#include <stdint.h>

bool UnwReadW(const uint32_t a, uint32_t *v);
bool UnwReadH(const uint32_t a, uint16_t *v);
bool UnwReadB(const uint32_t a, uint8_t *v);
