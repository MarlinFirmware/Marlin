/* *****************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 Perry Hung.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * ****************************************************************************/

# On an exception, push a fake stack thread mode stack frame and redirect
# thread execution to a thread mode error handler

# From RM008:
# The SP is decremented by eight words by the completion of the stack push.
# Figure 5-1 shows the contents of the stack after an exception pre-empts the
# current program flow.
#
# Old SP--> <previous>
#           xPSR
#           PC
#           LR
#           r12
#           r3
#           r2
#           r1
#    SP-->  r0

.text
.globl __exc_nmi
.weak  __exc_nmi
.globl __exc_hardfault
.weak  __exc_hardfault
.globl __exc_memmanage
.weak  __exc_memmanage
.globl __exc_busfault
.weak  __exc_busfault
.globl __exc_usagefault
.weak  __exc_usagefault

.code 16
.thumb_func
__exc_nmi:
    mov r0, #1
    b __default_exc

.thumb_func
__exc_hardfault:
    mov r0, #2
    b __default_exc

.thumb_func
__exc_memmanage:
    mov r0, #3
    b __default_exc

.thumb_func
__exc_busfault:
    mov r0, #4
    b __default_exc

.thumb_func
__exc_usagefault:
    mov r0, #5
    b __default_exc

.thumb_func
__default_exc:
    ldr r2, NVIC_CCR            @ Enable returning to thread mode even if there are
    mov r1 ,#1                  @ pending exceptions. See flag NONEBASETHRDENA.
    str r1, [r2]
    cpsid i                     @ Disable global interrupts
    ldr r2, SYSTICK_CSR         @ Disable systick handler
    mov r1, #0
    str r1, [r2]
    ldr r1, CPSR_MASK           @ Set default CPSR
    push {r1}
    ldr r1, TARGET_PC           @ Set target pc
    push {r1}
    sub sp, sp, #24             @ Don't care
    ldr r1, EXC_RETURN          @ Return to thread mode
    mov lr, r1
    bx lr                       @ Exception exit

.align 4
CPSR_MASK:     .word 0x61000000
EXC_RETURN:    .word 0xFFFFFFF9
TARGET_PC:     .word __error
NVIC_CCR:      .word 0xE000ED14    @ NVIC configuration control register
SYSTICK_CSR:   .word 0xE000E010    @ Systick control register
