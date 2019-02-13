/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifdef ARDUINO_ARCH_SAM

#include "../../core/macros.h"
#include "../../core/serial.h"
#include <stdarg.h>

#include "../shared/backtrace/unwinder.h"
#include "../shared/backtrace/unwmemaccess.h"

// Debug monitor that dumps to the Programming port all status when
// an exception or WDT timeout happens - And then resets the board

// All the Monitor routines must run with interrupts disabled and
// under an ISR execution context. That is why we cannot reuse the
// Serial interrupt routines or any C runtime, as we don't know the
// state we are when running them

// A SW memory barrier, to ensure GCC does not overoptimize loops
#define sw_barrier() __asm__ volatile("": : :"memory");

// (re)initialize UART0 as a monitor output to 250000,n,8,1
static void TXBegin(void) {

  // Disable UART interrupt in NVIC
  NVIC_DisableIRQ( UART_IRQn );

  // We NEED memory barriers to ensure Interrupts are actually disabled!
  // ( https://dzone.com/articles/nvic-disabling-interrupts-on-arm-cortex-m-and-the )
  __DSB();
  __ISB();

  // Disable clock
  pmc_disable_periph_clk( ID_UART );

  // Configure PMC
  pmc_enable_periph_clk( ID_UART );

  // Disable PDC channel
  UART->UART_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;

  // Reset and disable receiver and transmitter
  UART->UART_CR = UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RXDIS | UART_CR_TXDIS;

  // Configure mode: 8bit, No parity, 1 bit stop
  UART->UART_MR = UART_MR_CHMODE_NORMAL | US_MR_CHRL_8_BIT | US_MR_NBSTOP_1_BIT | UART_MR_PAR_NO;

  // Configure baudrate (asynchronous, no oversampling) to BAUDRATE bauds
  UART->UART_BRGR = (SystemCoreClock / (BAUDRATE << 4));

  // Enable receiver and transmitter
  UART->UART_CR = UART_CR_RXEN | UART_CR_TXEN;
}

// Send character through UART with no interrupts
static void TX(char c) {
  while (!(UART->UART_SR & UART_SR_TXRDY)) { WDT_Restart(WDT); sw_barrier(); };
  UART->UART_THR = c;
}

// Send String through UART
static void TX(const char* s) {
  while (*s) TX(*s++);
}

static void TXDigit(uint32_t d) {
  if (d < 10) TX((char)(d+'0'));
  else if (d < 16) TX((char)(d+'A'-10));
  else TX('?');
}

// Send Hex number thru UART
static void TXHex(uint32_t v) {
  TX("0x");
  for (uint8_t i = 0; i < 8; i++, v <<= 4)
    TXDigit((v >> 28) & 0xF);
}

// Send Decimal number thru UART
static void TXDec(uint32_t v) {
  if (!v) {
    TX('0');
    return;
  }

  char nbrs[14];
  char *p = &nbrs[0];
  while (v != 0) {
    *p++ = '0' + (v % 10);
    v /= 10;
  }
  do {
    p--;
    TX(*p);
  } while (p != &nbrs[0]);
}

// Dump a backtrace entry
static bool UnwReportOut(void* ctx, const UnwReport* bte) {
  int* p = (int*)ctx;

  (*p)++;
  TX('#'); TXDec(*p); TX(" : ");
  TX(bte->name?bte->name:"unknown"); TX('@'); TXHex(bte->function);
  TX('+'); TXDec(bte->address - bte->function);
  TX(" PC:");TXHex(bte->address); TX('\n');
  return true;
}

#ifdef UNW_DEBUG
  void UnwPrintf(const char* format, ...) {
    char dest[256];
    va_list argptr;
    va_start(argptr, format);
    vsprintf(dest, format, argptr);
    va_end(argptr);
    TX(&dest[0]);
  }
#endif

/* Table of function pointers for passing to the unwinder */
static const UnwindCallbacks UnwCallbacks = {
  UnwReportOut,
  UnwReadW,
  UnwReadH,
  UnwReadB
  #ifdef UNW_DEBUG
   , UnwPrintf
  #endif
};

/**
 * HardFaultHandler_C:
 * This is called from the HardFault_HandlerAsm with a pointer the Fault stack
 * as the parameter. We can then read the values from the stack and place them
 * into local variables for ease of reading.
 * We then read the various Fault Status and Address Registers to help decode
 * cause of the fault.
 * The function ends with a BKPT instruction to force control back into the debugger
 */
extern "C"
void HardFault_HandlerC(unsigned long *sp, unsigned long lr, unsigned long cause) {

  static const char* causestr[] = {
    "NMI","Hard","Mem","Bus","Usage","Debug","WDT","RSTC"
  };

  UnwindFrame btf;

  // Dump report to the Programming port (interrupts are DISABLED)
  TXBegin();
  TX("\n\n## Software Fault detected ##\n");
  TX("Cause: "); TX(causestr[cause]); TX('\n');

  TX("R0   : "); TXHex(((unsigned long)sp[0])); TX('\n');
  TX("R1   : "); TXHex(((unsigned long)sp[1])); TX('\n');
  TX("R2   : "); TXHex(((unsigned long)sp[2])); TX('\n');
  TX("R3   : "); TXHex(((unsigned long)sp[3])); TX('\n');
  TX("R12  : "); TXHex(((unsigned long)sp[4])); TX('\n');
  TX("LR   : "); TXHex(((unsigned long)sp[5])); TX('\n');
  TX("PC   : "); TXHex(((unsigned long)sp[6])); TX('\n');
  TX("PSR  : "); TXHex(((unsigned long)sp[7])); TX('\n');

  // Configurable Fault Status Register
  // Consists of MMSR, BFSR and UFSR
  TX("CFSR : "); TXHex((*((volatile unsigned long *)(0xE000ED28)))); TX('\n');

  // Hard Fault Status Register
  TX("HFSR : "); TXHex((*((volatile unsigned long *)(0xE000ED2C)))); TX('\n');

  // Debug Fault Status Register
  TX("DFSR : "); TXHex((*((volatile unsigned long *)(0xE000ED30)))); TX('\n');

  // Auxiliary Fault Status Register
  TX("AFSR : "); TXHex((*((volatile unsigned long *)(0xE000ED3C)))); TX('\n');

  // Read the Fault Address Registers. These may not contain valid values.
  // Check BFARVALID/MMARVALID to see if they are valid values
  // MemManage Fault Address Register
  TX("MMAR : "); TXHex((*((volatile unsigned long *)(0xE000ED34)))); TX('\n');

  // Bus Fault Address Register
  TX("BFAR : "); TXHex((*((volatile unsigned long *)(0xE000ED38)))); TX('\n');

  TX("ExcLR: "); TXHex(lr); TX('\n');
  TX("ExcSP: "); TXHex((unsigned long)sp); TX('\n');

  btf.sp = ((unsigned long)sp) + 8*4; // The original stack pointer
  btf.fp = btf.sp;
  btf.lr = ((unsigned long)sp[5]);
  btf.pc = ((unsigned long)sp[6]) | 1; // Force Thumb, as CORTEX only support it

  // Perform a backtrace
  TX("\nBacktrace:\n\n");
  int ctr = 0;
  UnwindStart(&btf, &UnwCallbacks, &ctr);

  // Disable all NVIC interrupts
  NVIC->ICER[0] = 0xFFFFFFFF;
  NVIC->ICER[1] = 0xFFFFFFFF;

  // Relocate VTOR table to default position
  SCB->VTOR = 0;

  // Disable USB
  otg_disable();

  // Restart watchdog
  WDT_Restart(WDT);

  // Reset controller
  NVIC_SystemReset();
  while(1) { WDT_Restart(WDT); }
}

__attribute__((naked)) void NMI_Handler(void) {
  __asm__ __volatile__ (
    ".syntax unified" "\n\t"
    A("tst lr, #4")
    A("ite eq")
    A("mrseq r0, msp")
    A("mrsne r0, psp")
    A("mov r1,lr")
    A("mov r2,#0")
    A("b HardFault_HandlerC")
  );
}

__attribute__((naked)) void HardFault_Handler(void) {
  __asm__ __volatile__ (
    ".syntax unified" "\n\t"
    A("tst lr, #4")
    A("ite eq")
    A("mrseq r0, msp")
    A("mrsne r0, psp")
    A("mov r1,lr")
    A("mov r2,#1")
    A("b HardFault_HandlerC")
  );
}

__attribute__((naked)) void MemManage_Handler(void) {
  __asm__ __volatile__ (
    ".syntax unified" "\n\t"
    A("tst lr, #4")
    A("ite eq")
    A("mrseq r0, msp")
    A("mrsne r0, psp")
    A("mov r1,lr")
    A("mov r2,#2")
    A("b HardFault_HandlerC")
  );
}

__attribute__((naked)) void BusFault_Handler(void) {
  __asm__ __volatile__ (
    ".syntax unified" "\n\t"
    A("tst lr, #4")
    A("ite eq")
    A("mrseq r0, msp")
    A("mrsne r0, psp")
    A("mov r1,lr")
    A("mov r2,#3")
    A("b HardFault_HandlerC")
  );
}

__attribute__((naked)) void UsageFault_Handler(void) {
  __asm__ __volatile__ (
    ".syntax unified" "\n\t"
    A("tst lr, #4")
    A("ite eq")
    A("mrseq r0, msp")
    A("mrsne r0, psp")
    A("mov r1,lr")
    A("mov r2,#4")
    A("b HardFault_HandlerC")
  );
}

__attribute__((naked)) void DebugMon_Handler(void) {
  __asm__ __volatile__ (
    ".syntax unified" "\n\t"
    A("tst lr, #4")
    A("ite eq")
    A("mrseq r0, msp")
    A("mrsne r0, psp")
    A("mov r1,lr")
    A("mov r2,#5")
    A("b HardFault_HandlerC")
  );
}

/* This is NOT an exception, it is an interrupt handler - Nevertheless, the framing is the same */
__attribute__((naked)) void WDT_Handler(void) {
  __asm__ __volatile__ (
    ".syntax unified" "\n\t"
    A("tst lr, #4")
    A("ite eq")
    A("mrseq r0, msp")
    A("mrsne r0, psp")
    A("mov r1,lr")
    A("mov r2,#6")
    A("b HardFault_HandlerC")
  );
}

__attribute__((naked)) void RSTC_Handler(void) {
  __asm__ __volatile__ (
    ".syntax unified" "\n\t"
    A("tst lr, #4")
    A("ite eq")
    A("mrseq r0, msp")
    A("mrsne r0, psp")
    A("mov r1,lr")
    A("mov r2,#7")
    A("b HardFault_HandlerC")
  );
}

#endif // ARDUINO_ARCH_SAM
