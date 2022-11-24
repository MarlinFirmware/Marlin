/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "../../../inc/MarlinConfig.h"
#include "HAL_NVIC.h"

#if defined(STM32F1xx)
  #ifdef STM32F1xx_CONNECTIVITY_LINE
    // See STM32F1xx reference manual page 198
    #define VECTOR_TABLE_SIZE 83
  #else
    // See STM32F1xx reference manual page 204
    #define VECTOR_TABLE_SIZE 75
  #endif
  #define IRQ_VECTOR_ALIGNMENT (1<<9)
  // Defined by GCC.
  // The vector is located in flash memory, thus there may be no memory bus for write access
  // wired into the CPU (hard fault at attempt).
  extern void (*const g_pfnVectors[VECTOR_TABLE_SIZE])();
  #define _NVIC_HAS_DEFAULT_VECTOR
#elif defined(STM32F4xx)
  // There are a few less entries specifically for "STM32F405xx/07xx and STM32F415xx/17xx" but w/e.
  #define VECTOR_TABLE_SIZE 107
  #define IRQ_VECTOR_ALIGNMENT (1<<9)
  // Defined by GCC.
  // The vector is located in flash memory, thus there may be no memory bus for write access
  // wired into the CPU (hard fault at attempt).
  extern void (*const g_pfnVectors[VECTOR_TABLE_SIZE])();
  #define _NVIC_HAS_DEFAULT_VECTOR
#elif defined(TARGET_LPC1768)
  // See NXP UM10360 page 75ff
  #define VECTOR_TABLE_SIZE 51
  #define IRQ_VECTOR_ALIGNMENT (1<<10)
#else
    #error Unknown NVIC constants for platform. Please inspect hardware reference manuals and update the code!
#endif
#define IRQ_START_IDX 16

// The interrupt table used by installment.
alignas(IRQ_VECTOR_ALIGNMENT) static void (*_isrv_redirect[VECTOR_TABLE_SIZE])();

// From the ARM Cortex M3 reference manual.
struct vtor_reg_t {
    uint32_t reserved1 : 9;
    uint32_t TBLOFF : 21;
    uint32_t reserved2 : 2;
};

// For debugging ;)
static bool _NVIC_IsVectorInCorrectAddressSpace(const void *vectaddr) {
    uint32_t valaddr = (uint32_t)vectaddr;
    return (valaddr >= (1<<9) && valaddr < (1<<30));
}

static void _NVIC_OnError() {
    for (;;) {
#if PIN_EXISTS(BEEPER)
        OUT_WRITE(BEEPER_PIN, HIGH);
        delay(500);
        OUT_WRITE(BEEPER_PIN, LOW);
        delay(500);
        OUT_WRITE(BEEPER_PIN, HIGH);
        delay(200);
        OUT_WRITE(BEEPER_PIN, LOW);
        delay(200);
        OUT_WRITE(BEEPER_PIN, HIGH);
        delay(600);
        OUT_WRITE(BEEPER_PIN, LOW);
        delay(600);
        OUT_WRITE(BEEPER_PIN, HIGH);
        delay(200);
        OUT_WRITE(BEEPER_PIN, LOW);
        delay(2000);
#endif
    }
}

static vtor_reg_t& _SCB_VTOR = *(vtor_reg_t*)0xE000ED08;

static unsigned int _vtor_refcount = 0;

static void (*const* _NVIC_GetCurrentVector())() {
    return (void (*const*)())( _SCB_VTOR.TBLOFF << 9 );
}

#ifndef _NVIC_HAS_DEFAULT_VECTOR
static void (*const*_nvic_default_vector)() = nullptr;
#endif

static void (*const*_NVIC_GetDefaultVector())() {
#ifdef STM32F1xx
    return g_pfnVectors;
#else
    return _nvic_default_vector;
#endif
}

static void _nvicFetchVectorEntriesFromDefault() {
    memcpy(_isrv_redirect, _NVIC_GetDefaultVector(), sizeof(void*)*VECTOR_TABLE_SIZE);
}

void nvicBegin() {
    if (_NVIC_IsVectorInCorrectAddressSpace(_isrv_redirect) == false)
        _NVIC_OnError();
#ifndef _NVIC_HAS_DEFAULT_VECTOR
    _nvic_default_vector = _NVIC_GetCurrentVector();
#endif
    _nvicFetchVectorEntriesFromDefault();
}

void nvicResetVector() {
    _nvicFetchVectorEntriesFromDefault();
}

void nvicUpdateVector() {
    if (_vtor_refcount == 0) {
        memcpy(_isrv_redirect, (const void*)_NVIC_GetCurrentVector(), sizeof(_isrv_redirect));
    }
}

void nvicSetHandler(uint32_t idx, void (*callback)()) {
    if (idx >= VECTOR_TABLE_SIZE)
        _NVIC_OnError();
    
    _isrv_redirect[idx] = callback;
    __DSB();    // handle CPU pipeline caching inconsistencies (non instruction encoding)
}

void nvicSetIRQHandler(IRQn_Type irqn, void (*callback)()) {
    nvicSetHandler(IRQ_START_IDX + (uint32_t)irqn, callback);
}

void (*nvicGetHandler(uint32_t idx))() {
    if (idx >= VECTOR_TABLE_SIZE)
        _NVIC_OnError();
    
    return _isrv_redirect[idx];
}

void nvicResetHandler(uint32_t idx) {
    if (idx >= VECTOR_TABLE_SIZE)
        _NVIC_OnError();

    _isrv_redirect[idx] = _NVIC_GetDefaultVector()[idx];
}

void nvicResetIRQHandler(IRQn_Type irqn) {
    nvicResetHandler(IRQ_START_IDX + (uint32_t)irqn);
}

uint32_t nvicGetMaxHandlerCount() {
    return VECTOR_TABLE_SIZE;
}

void nvicInstallRedirect() {
    _vtor_refcount++;
    if (_vtor_refcount == 1)
    {
        _SCB_VTOR.TBLOFF = ( (uint32_t)&_isrv_redirect[0] >> 9 );
        __DSB();    // handle CPU pipeline caching inconsistencies (non instruction encoding)
    }
}

void nvicUninstallRedirect() {
    if (_vtor_refcount == 1)
    {
        _SCB_VTOR.TBLOFF = ( (uint32_t)_NVIC_GetDefaultVector() >> 9 );
        __DSB();    // handle CPU pipeline caching inconsistencies (non instruction encoding)
    }
    _vtor_refcount--;
}