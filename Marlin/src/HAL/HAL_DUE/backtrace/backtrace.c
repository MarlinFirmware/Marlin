/*
 * Libbacktrace
 * Copyright 2015 Stephen Street <stephen@redrocketcomputing.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This library was modified and adapted to be used in Marlin 3D printer
 * firmware as backtracer for exceptions for debugging purposes in 2018
 * by Eduardo José Tagle.
 */

#ifdef ARDUINO_ARCH_SAM

#include "backtrace.h"

#include <stdint.h>
#include <string.h>

typedef struct unwind_control_block {
  uint32_t vrs[16];
  const uint32_t *current;
  int remaining;
  int byte;
} unwind_control_block_t;

typedef struct unwind_index {
  uint32_t addr_offset;
  uint32_t insn;
} unwind_index_t;

/* These symbols point to the unwind index and should be provide by the linker script */
extern const unwind_index_t __exidx_start[];
extern const unwind_index_t __exidx_end[];

/* This prevents the linking of libgcc unwinder code */
void __aeabi_unwind_cpp_pr0(void) {};
void __aeabi_unwind_cpp_pr1(void) {};
void __aeabi_unwind_cpp_pr2(void) {};

/* These symbols point to the start and end of stack */
extern const int _sstack;
extern const int _estack;

/* Validate stack pointer */
static inline __attribute__((always_inline)) int validate_sp(const void* sp) {
  if ((uint32_t)sp < (uint32_t)&_sstack || (uint32_t)sp > (uint32_t)&_estack)
    return -1;
  return 0;
}

static inline __attribute__((always_inline)) uint32_t prel31_to_addr(const uint32_t *prel31) {
  int32_t offset = (((int32_t)(*prel31)) << 1) >> 1;
  return ((uint32_t)prel31 + offset) & 0x7fffffff;
}

static const struct unwind_index *unwind_search_index(const unwind_index_t *start, const unwind_index_t *end, uint32_t ip) {
  const struct unwind_index *middle;

  /* Perform a binary search of the unwind index */
  while (start < end - 1) {
    middle = start + ((end - start + 1) >> 1);
    if (ip < prel31_to_addr(&middle->addr_offset))
      end = middle;
    else
      start = middle;
  }
  return start;
}

static const char *unwind_get_function_name(void *address) {
  uint32_t flag_word = *(uint32_t *)(address - 4);
  if ((flag_word & 0xff000000) == 0xff000000) {
    return (const char *)(address - 4 - (flag_word & 0x00ffffff));
  }
  return "unknown";
}

static int unwind_get_next_byte(unwind_control_block_t *ucb) {
  int instruction;

  /* Are there more instructions */
  if (ucb->remaining == 0)
    return -1;

  /* Extract the current instruction */
  instruction = ((*ucb->current) >> (ucb->byte << 3)) & 0xff;

  /* Move the next byte */
  --ucb->byte;
  if (ucb->byte < 0) {
    ++ucb->current;
    ucb->byte = 3;
  }
  --ucb->remaining;

  return instruction;
}

static int unwind_control_block_init(unwind_control_block_t *ucb, const uint32_t *instructions, const backtrace_frame_t *frame) {
  /* Initialize control block */
  memset(ucb, 0, sizeof(unwind_control_block_t));
  ucb->current = instructions;

  /* Is the a short unwind description */
  if ((*instructions & 0xff000000) == 0x80000000) {
    ucb->remaining = 3;
    ucb->byte = 2;
  /* Is the a long unwind description */
  } else if ((*instructions & 0xff000000) == 0x81000000) {
    ucb->remaining = ((*instructions & 0x00ff0000) >> 14) + 2;
    ucb->byte = 1;
  } else
    return -1;

  /* Initialize the virtual register set */
  if (frame) {
    ucb->vrs[7] = frame->fp;
    ucb->vrs[13] = frame->sp;
    ucb->vrs[14] = frame->lr;
    ucb->vrs[15] = 0;
  }

  /* All good */
  return 0;
}

static int unwind_execute_instruction(unwind_control_block_t *ucb) {

  int instruction;
  uint32_t mask;
  uint32_t reg;
  uint32_t *vsp;

  /* Consume all instruction byte */
  while ((instruction = unwind_get_next_byte(ucb)) != -1) {

    if ((instruction & 0xc0) == 0x00) {
      /* vsp = vsp + (xxxxxx << 2) + 4 */
      ucb->vrs[13] += ((instruction & 0x3f) << 2) + 4;

    } else if ((instruction & 0xc0) == 0x40) {
      /* vsp = vsp - (xxxxxx << 2) - 4 */
      ucb->vrs[13] -= ((instruction & 0x3f) << 2) - 4;

    } else if ((instruction & 0xf0) == 0x80) {
      /* pop under mask {r15-r12},{r11-r4} or refuse to unwind */
      instruction = instruction << 8 | unwind_get_next_byte(ucb);

      /* Check for refuse to unwind */
      if (instruction == 0x8000)
        return 0;

      /* Pop registers using mask */
      vsp = (uint32_t *)ucb->vrs[13];
      mask = instruction & 0xfff;

      reg = 4;
      while (mask != 0) {
        if ((mask & 0x001) != 0) {
          if (validate_sp(vsp))
            return -1;
          ucb->vrs[reg] = *vsp++;
        }
        mask = mask >> 1;
        ++reg;
      }

      /* Patch up the vrs sp if it was in the mask */
      if ((mask & (1 << (13 - 4))) != 0)
        ucb->vrs[13] = (uint32_t)vsp;

    } else if ((instruction & 0xf0) == 0x90 && instruction != 0x9d && instruction != 0x9f) {
      /* vsp = r[nnnn] */
      ucb->vrs[13] = ucb->vrs[instruction & 0x0f];

    } else if ((instruction & 0xf0) == 0xa0) {
      /* pop r4-r[4+nnn] or pop r4-r[4+nnn], r14*/
      vsp = (uint32_t *)ucb->vrs[13];

      for (reg = 4; reg <= (instruction & 0x07) + 4; ++reg) {
        if (validate_sp(vsp))
          return -1;
        ucb->vrs[reg] = *vsp++;
      }

      if (instruction & 0x80) {
        if (validate_sp(vsp))
          return -1;
        ucb->vrs[14] = *vsp++;
      }

      ucb->vrs[13] = (uint32_t)vsp;

    } else if (instruction == 0xb0) {
      /* finished */
      if (ucb->vrs[15] == 0)
        ucb->vrs[15] = ucb->vrs[14];

      /* All done unwinding */
      return 0;

    } else if (instruction == 0xb1) {
      /* pop register under mask {r3,r2,r1,r0} */
      vsp = (uint32_t *)ucb->vrs[13];
      mask = unwind_get_next_byte(ucb);

      reg = 0;
      while (mask != 0) {
        if ((mask & 0x01) != 0) {
          if (validate_sp(vsp))
            return -1;
          ucb->vrs[reg] = *vsp++;
        }
        mask = mask >> 1;
        ++reg;
      }
      ucb->vrs[13] = (uint32_t)vsp;

    } else if (instruction == 0xb2) {
      /* vps = vsp + 0x204 + (uleb128 << 2) */
      ucb->vrs[13] += 0x204 + (unwind_get_next_byte(ucb) << 2);

    } else if (instruction == 0xb3 || instruction == 0xc8 || instruction == 0xc9) {
      /* pop VFP double-precision registers */
      vsp = (uint32_t *)ucb->vrs[13];

      /* D[ssss]-D[ssss+cccc] */
      if (validate_sp(vsp))
        return -1;
      ucb->vrs[14] = *vsp++;

      if (instruction == 0xc8) {
        /* D[16+sssss]-D[16+ssss+cccc] */
        ucb->vrs[14] |= 1 << 16;
      }

      if (instruction != 0xb3) {
        /* D[sssss]-D[ssss+cccc] */
        ucb->vrs[14] |= 1 << 17;
      }

      ucb->vrs[13] = (uint32_t)vsp;

    } else if ((instruction & 0xf8) == 0xb8 || (instruction & 0xf8) == 0xd0) {
      /* Pop VFP double precision registers D[8]-D[8+nnn] */
      ucb->vrs[14] = 0x80 | (instruction & 0x07);

      if ((instruction & 0xf8) == 0xd0) {
        ucb->vrs[14] = 1 << 17;
      }

    } else
      return -1;
  }

  return instruction != -1;
}

static inline __attribute__((always_inline)) uint32_t *read_psp(void) {
  /* Read the current PSP and return its value as a pointer */
  uint32_t psp;

  __asm volatile (
    "   mrs %0, psp \n"
    : "=r" (psp) : :
  );

  return (uint32_t*)psp;
}

static int unwind_frame(backtrace_frame_t *frame) {

  unwind_control_block_t ucb;
  const unwind_index_t *index;
  const uint32_t *instructions;
  int execution_result;

  /* Search the unwind index for the matching unwind table */
  index = unwind_search_index(__exidx_start, __exidx_end, frame->pc);
  if (index == NULL)
    return -1;

  /* Make sure we can unwind this frame */
  if (index->insn == 0x00000001)
    return 0;

  /* Get the pointer to the first unwind instruction */
  if (index->insn & 0x80000000)
    instructions = &index->insn;
  else
    instructions = (uint32_t *)prel31_to_addr(&index->insn);

  /* Initialize the unwind control block */
  if (unwind_control_block_init(&ucb, instructions, frame) < 0)
    return -1;

  /* Execute the unwind instructions TODO range check the stack pointer */
  while ((execution_result = unwind_execute_instruction(&ucb)) > 0);
  if (execution_result == -1)
    return -1;

  /* Set the virtual pc to the virtual lr if this is the first unwind */
  if (ucb.vrs[15] == 0)
    ucb.vrs[15] = ucb.vrs[14];

  /* Check for exception return */
  /* TODO Test with other ARM processors to verify this method. */
  if ((ucb.vrs[15] & 0xf0000000) == 0xf0000000) {
    /* According to the Cortex Programming Manual (p.44), the stack address is always 8-byte aligned (Cortex-M7).
       Depending on where the exception came from (MSP or PSP), we need the right SP value to work with.

       ucb.vrs[7] contains the right value, so take it and align it by 8 bytes, store it as the current
       SP to work with (ucb.vrs[13]) which is then saved as the current (virtual) frame's SP.
    */
    uint32_t *stack;
    ucb.vrs[13] = (ucb.vrs[7] & ~7);

    /* If we need to start from the MSP, we need to go down X words to find the PC, where:
        X=2  if it was a non-floating-point exception
        X=20 if it was a floating-point (VFP) exception

       If we need to start from the PSP, we need to go up exactly 6 words to find the PC.
       See the ARMv7-M Architecture Reference Manual p.594 and Cortex-M7 Processor Programming Manual p.44/p.45 for details.
    */
    if ((ucb.vrs[15] & 0xc) == 0) {
      /* Return to Handler Mode: MSP (0xffffff-1) */
      stack = (uint32_t*)(ucb.vrs[13]);

      /* The PC is always 2 words down from the MSP, if it was a non-floating-point exception */
      stack -= 2;

      /* If there was a VFP exception (0xffffffe1), the PC is located another 18 words down */
      if ((ucb.vrs[15] & 0xf0) == 0xe0)
      {
        stack -= 18;
      }
    }
    else {
      /* Return to Thread Mode: PSP (0xffffff-d) */
      stack = read_psp();

      /* The PC is always 6 words up from the PSP */
      stack += 6;
    }

    /* Store the PC */
    ucb.vrs[15] = *stack--;

    /* Store the LR */
    ucb.vrs[14] = *stack--;
  }

  /* We are done if current frame pc is equal to the virtual pc, prevent infinite loop */
  if (frame->pc == ucb.vrs[15])
    return 0;

  /* Update the frame */
  frame->fp = ucb.vrs[7];
  frame->sp = ucb.vrs[13];
  frame->lr = ucb.vrs[14];
  frame->pc = ucb.vrs[15];

  /* All good */
  return 1;
}

int backtrace_dump(backtrace_frame_t *frame, backtrace_dump_fn_t dump_entry, void* ctx )
{
  backtrace_t entry;
  int count = 1;

  /* Unwind all frames */
  do {
    if (frame->pc == 0) {
      /* Reached __exidx_end. */
      entry.name = "<reached end of unwind table>";
      entry.address = 0;
      entry.function = 0;
      dump_entry(count, &entry, ctx);
      break;
    }

    if (frame->pc == 0x00000001) {
      /* Reached .cantunwind instruction. */
      entry.name = "<reached .cantunwind>";
      entry.address = 0;
      entry.function = 0;
      dump_entry(count, &entry, ctx);
      break;
    }

    /* Find the unwind index of the current frame pc */
    const unwind_index_t *index = unwind_search_index(__exidx_start, __exidx_end, frame->pc);

    /* Clear last bit (Thumb indicator) */
    frame->pc &= 0xfffffffeU;

    /* Generate the backtrace information */
    entry.address = (void *)frame->pc;
    entry.function = (void *)prel31_to_addr(&index->addr_offset);
    entry.name = unwind_get_function_name(entry.function);
    dump_entry(count, &entry, ctx);

    /* Next backtrace frame */
    ++count;

  } while (unwind_frame(frame) == 1);

  /* All done */
  return count;
}

#endif
