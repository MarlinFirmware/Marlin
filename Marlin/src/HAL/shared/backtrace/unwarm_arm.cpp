/***************************************************************************
 * ARM Stack Unwinder, Michael.McTernan.2001@cs.bris.ac.uk
 * Updated, adapted and several bug fixes on 2018 by Eduardo José Tagle
 *
 * This program is PUBLIC DOMAIN.
 * This means that there is no copyright and anyone is able to take a copy
 * for free and use it as they wish, with or without modifications, and in
 * any context, commercially or otherwise. The only limitation is that I
 * don't guarantee that the software is fit for any purpose or accept any
 * liability for its use or misuse - this software is without warranty.
 ***************************************************************************
 * File Description: Abstract interpreter for ARM mode.
 **************************************************************************/

#if defined(__arm__) || defined(__thumb__)

#define MODULE_NAME "UNWARM_ARM"

#include <stdio.h>
#include "unwarm.h"

/** Check if some instruction is a data-processing instruction.
 * Decodes the passed instruction, checks if it is a data-processing and
 * verifies that the parameters and operation really indicate a data-
 * processing instruction.  This is needed because some parts of the
 * instruction space under this instruction can be extended or represent
 * other operations such as MRS, MSR.
 *
 * \param[in] inst  The instruction word.
 * \retval true  Further decoding of the instruction indicates that this is
 *                a valid data-processing instruction.
 * \retval false This is not a data-processing instruction,
 */
static bool isDataProc(uint32_t instr) {
  uint8_t opcode = (instr & 0x01E00000) >> 21;
  if ((instr & 0xFC000000) != 0xE0000000) return false;

  /* TST, TEQ, CMP and CMN all require S to be set */
  bool S = !!(instr & 0x00100000);
  if (!S && opcode >= 8 && opcode <= 11) return false;

  return true;
}

UnwResult UnwStartArm(UnwState * const state) {
  bool found = false;
  uint16_t   t = UNW_MAX_INSTR_COUNT;

  do {
    uint32_t instr;

    /* Attempt to read the instruction */
    if (!state->cb->readW(state->regData[15].v, &instr))
      return UNWIND_IREAD_W_FAIL;

    UnwPrintd4("A %x %x %08x:", state->regData[13].v, state->regData[15].v, instr);

    /* Check that the PC is still on Arm alignment */
    if (state->regData[15].v & 0x3) {
      UnwPrintd1("\nError: PC misalignment\n");
      return UNWIND_INCONSISTENT;
    }

    /* Check that the SP and PC have not been invalidated */
    if (!M_IsOriginValid(state->regData[13].o) || !M_IsOriginValid(state->regData[15].o)) {
      UnwPrintd1("\nError: PC or SP invalidated\n");
      return UNWIND_INCONSISTENT;
    }

    /* Branch and Exchange (BX)
     *  This is tested prior to data processing to prevent
     *  mis-interpretation as an invalid TEQ instruction.
     */
    if ((instr & 0xFFFFFFF0) == 0xE12FFF10) {
      uint8_t rn = instr & 0xF;

      UnwPrintd4("BX r%d\t ; r%d %s\n", rn, rn, M_Origin2Str(state->regData[rn].o));

      if (!M_IsOriginValid(state->regData[rn].o)) {
        UnwPrintd1("\nUnwind failure: BX to untracked register\n");
        return UNWIND_FAILURE;
      }

      /* Set the new PC value */
      state->regData[15].v = state->regData[rn].v;

      /* Check if the return value is from the stack */
      if (state->regData[rn].o == REG_VAL_FROM_STACK) {

        /* Now have the return address */
        UnwPrintd2(" Return PC=%x\n", state->regData[15].v & (~0x1));

        /* Report the return address */
        if (!UnwReportRetAddr(state, state->regData[rn].v))
          return UNWIND_TRUNCATED;
      }

      /* Determine the return mode */
      if (state->regData[rn].v & 0x1) /* Branching to THUMB */
        return UnwStartThumb(state);

      /* Branch to ARM */
      /* Account for the auto-increment which isn't needed */
      state->regData[15].v -= 4;
    }
    /* Branch */
    else if ((instr & 0xFF000000) == 0xEA000000) {

      int32_t offset = (instr & 0x00FFFFFF) << 2;

      /* Sign extend if needed */
      if (offset & 0x02000000) offset |= 0xFC000000;

      UnwPrintd2("B %d\n", offset);

      /* Adjust PC */
      state->regData[15].v += offset;

      /* Account for pre-fetch, where normally the PC is 8 bytes
       *  ahead of the instruction just executed.
       */
      state->regData[15].v += 4;
    }

    /* MRS */
    else if ((instr & 0xFFBF0FFF) == 0xE10F0000) {
      uint8_t rd = (instr & 0x0000F000) >> 12;
      #ifdef UNW_DEBUG
        const bool R = !!(instr & 0x00400000);
        UnwPrintd4("MRS r%d,%s\t; r%d invalidated", rd, R ? "SPSR" : "CPSR", rd);
      #endif

      /* Status registers untracked */
      state->regData[rd].o = REG_VAL_INVALID;
    }
    /* MSR */
    else if ((instr & 0xFFB0F000) == 0xE120F000) {
      #ifdef UNW_DEBUG
        UnwPrintd2("MSR %s_?, ???", (instr & 0x00400000) ? "SPSR" : "CPSR");
      #endif

      /* Status registers untracked.
       *  Potentially this could change processor mode and switch
       *  banked registers r8-r14.  Most likely is that r13 (sp) will
       *  be banked.  However, invalidating r13 will stop unwinding
       *  when potentially this write is being used to disable/enable
       *  interrupts (a common case).  Therefore no invalidation is
       *  performed.
       */
    }
    /* Data processing */
    else if (isDataProc(instr)) {
      bool            I = !!(instr & 0x02000000);
      uint8_t    opcode = (instr & 0x01E00000) >> 21;
      #ifdef UNW_DEBUG
        bool          S = !!(instr & 0x00100000);
      #endif
      uint8_t        rn = (instr & 0x000F0000) >> 16;
      uint8_t        rd = (instr & 0x0000F000) >> 12;
      uint16_t operand2 = (instr & 0x00000FFF);
      uint32_t        op2val;
      int             op2origin;

      switch (opcode) {
        case  0: UnwPrintd4("AND%s r%d,r%d,", S ? "S" : "", rd, rn); break;
        case  1: UnwPrintd4("EOR%s r%d,r%d,", S ? "S" : "", rd, rn); break;
        case  2: UnwPrintd4("SUB%s r%d,r%d,", S ? "S" : "", rd, rn); break;
        case  3: UnwPrintd4("RSB%s r%d,r%d,", S ? "S" : "", rd, rn); break;
        case  4: UnwPrintd4("ADD%s r%d,r%d,", S ? "S" : "", rd, rn); break;
        case  5: UnwPrintd4("ADC%s r%d,r%d,", S ? "S" : "", rd, rn); break;
        case  6: UnwPrintd4("SBC%s r%d,r%d,", S ? "S" : "", rd, rn); break;
        case  7: UnwPrintd4("RSC%s r%d,r%d,", S ? "S" : "", rd, rn); break;
        case  8: UnwPrintd3("TST%s r%d,", S ? "S" : "", rn); break;
        case  9: UnwPrintd3("TEQ%s r%d,", S ? "S" : "", rn); break;
        case 10: UnwPrintd3("CMP%s r%d,", S ? "S" : "", rn); break;
        case 11: UnwPrintd3("CMN%s r%d,", S ? "S" : "", rn); break;
        case 12: UnwPrintd3("ORR%s r%d,", S ? "S" : "", rn); break;
        case 13: UnwPrintd3("MOV%s r%d,", S ? "S" : "", rd); break;
        case 14: UnwPrintd4("BIC%s r%d,r%d", S ? "S" : "", rd, rn); break;
        case 15: UnwPrintd3("MVN%s r%d,", S ? "S" : "", rd); break;
      }

      /* Decode operand 2 */
      if (I) {
        uint8_t shiftDist  = (operand2 & 0x0F00) >> 8;
        uint8_t shiftConst = (operand2 & 0x00FF);

        /* rotate const right by 2 * shiftDist */
        shiftDist *= 2;
        op2val    = (shiftConst >> shiftDist) |
                    (shiftConst << (32 - shiftDist));
        op2origin = REG_VAL_FROM_CONST;

        UnwPrintd2("#0x%x", op2val);
      }
      else {

        /* Register and shift */
        uint8_t  rm        = (operand2 & 0x000F);
        uint8_t  regShift  = !!(operand2 & 0x0010);
        uint8_t  shiftType = (operand2 & 0x0060) >> 5;
        uint32_t shiftDist;
        #ifdef UNW_DEBUG
          const char * const shiftMnu[4] = { "LSL", "LSR", "ASR", "ROR" };
        #endif
        UnwPrintd2("r%d ", rm);

        /* Get the shift distance */
        if (regShift) {
          uint8_t rs = (operand2 & 0x0F00) >> 8;

          if (operand2 & 0x00800) {
            UnwPrintd1("\nError: Bit should be zero\n");
            return UNWIND_ILLEGAL_INSTR;
          }
          else if (rs == 15) {
            UnwPrintd1("\nError: Cannot use R15 with register shift\n");
            return UNWIND_ILLEGAL_INSTR;
          }

          /* Get shift distance */
          shiftDist = state->regData[rs].v;
          op2origin = state->regData[rs].o;

          UnwPrintd7("%s r%d\t; r%d %s r%d %s", shiftMnu[shiftType], rs, rm, M_Origin2Str(state->regData[rm].o), rs, M_Origin2Str(state->regData[rs].o));
        }
        else {
          shiftDist = (operand2 & 0x0F80) >> 7;
          op2origin = REG_VAL_FROM_CONST;
          if (shiftDist) UnwPrintd3("%s #%d", shiftMnu[shiftType], shiftDist);
          UnwPrintd3("\t; r%d %s", rm, M_Origin2Str(state->regData[rm].o));
        }

        /* Apply the shift type to the source register */
        switch (shiftType) {
          case 0: /* logical left */
            op2val = state->regData[rm].v << shiftDist;
            break;

          case 1: /* logical right */
            if (!regShift && shiftDist == 0) shiftDist = 32;
            op2val = state->regData[rm].v >> shiftDist;
            break;

          case 2: /* arithmetic right */
            if (!regShift && shiftDist == 0) shiftDist = 32;

            if (state->regData[rm].v & 0x80000000) {
              /* Register shifts maybe greater than 32 */
              if (shiftDist >= 32)
                op2val = 0xFFFFFFFF;
              else
                op2val = (state->regData[rm].v >> shiftDist) | (0xFFFFFFFF << (32 - shiftDist));
            }
            else
              op2val = state->regData[rm].v >> shiftDist;
            break;

          case 3: /* rotate right */

            if (!regShift && shiftDist == 0) {
              /* Rotate right with extend.
               *  This uses the carry bit and so always has an
               *  untracked result.
               */
              op2origin = REG_VAL_INVALID;
              op2val    = 0;
            }
            else {
              /* Limit shift distance to 0-31 incase of register shift */
              shiftDist &= 0x1F;

              op2val = (state->regData[rm].v >> shiftDist) |
                       (state->regData[rm].v << (32 - shiftDist));
            }
            break;

          default:
            UnwPrintd2("\nError: Invalid shift type: %d\n", shiftType);
            return UNWIND_FAILURE;
        }

        /* Decide the data origin */
        if (M_IsOriginValid(op2origin) && M_IsOriginValid(state->regData[rm].o))
          op2origin = REG_VAL_ARITHMETIC | state->regData[rm].o;
        else
          op2origin = REG_VAL_INVALID;
      }

      /* Propagate register validity */
      switch (opcode) {
        case  0: /* AND: Rd := Op1 AND Op2 */
        case  1: /* EOR: Rd := Op1 EOR Op2 */
        case  2: /* SUB: Rd:= Op1 - Op2 */
        case  3: /* RSB: Rd:= Op2 - Op1 */
        case  4: /* ADD: Rd:= Op1 + Op2 */
        case 12: /* ORR: Rd:= Op1 OR Op2 */
        case 14: /* BIC: Rd:= Op1 AND NOT Op2 */
          if (!M_IsOriginValid(state->regData[rn].o) ||
             !M_IsOriginValid(op2origin)) {
            state->regData[rd].o = REG_VAL_INVALID;
          }
          else {
            state->regData[rd].o = state->regData[rn].o;
            state->regData[rd].o = (RegValOrigin)(state->regData[rd].o | op2origin);
          }
          break;

        case  5: /* ADC: Rd:= Op1 + Op2 + C */
        case  6: /* SBC: Rd:= Op1 - Op2 + C */
        case  7: /* RSC: Rd:= Op2 - Op1 + C */
          /* CPSR is not tracked */
          state->regData[rd].o = REG_VAL_INVALID;
          break;

        case  8: /* TST: set condition codes on Op1 AND Op2 */
        case  9: /* TEQ: set condition codes on Op1 EOR Op2 */
        case 10: /* CMP: set condition codes on Op1 - Op2 */
        case 11: /* CMN: set condition codes on Op1 + Op2 */
          break;

        case 13: /* MOV: Rd:= Op2 */
        case 15: /* MVN: Rd:= NOT Op2 */
          state->regData[rd].o = (RegValOrigin) op2origin;
          break;
      }

      /* Account for pre-fetch by temporarily adjusting PC */
      if (rn == 15) {

        /* If the shift amount is specified in the instruction,
         *  the PC will be 8 bytes ahead. If a register is used
         *  to specify the shift amount the PC will be 12 bytes
         *  ahead.
         */
        state->regData[rn].v += ((!I && (operand2 & 0x0010)) ? 12 : 8);
      }

      /* Compute values */
      switch (opcode) {
        case  0: /* AND: Rd := Op1 AND Op2 */
          state->regData[rd].v = state->regData[rn].v & op2val;
          break;

        case  1: /* EOR: Rd := Op1 EOR Op2 */
          state->regData[rd].v = state->regData[rn].v ^ op2val;
          break;

        case  2: /* SUB: Rd:= Op1 - Op2 */
          state->regData[rd].v = state->regData[rn].v - op2val;
          break;
        case  3: /* RSB: Rd:= Op2 - Op1 */
          state->regData[rd].v = op2val - state->regData[rn].v;
          break;

        case  4: /* ADD: Rd:= Op1 + Op2 */
          state->regData[rd].v = state->regData[rn].v + op2val;
          break;

        case  5: /* ADC: Rd:= Op1 + Op2 + C */
        case  6: /* SBC: Rd:= Op1 - Op2 + C */
        case  7: /* RSC: Rd:= Op2 - Op1 + C */
        case  8: /* TST: set condition codes on Op1 AND Op2 */
        case  9: /* TEQ: set condition codes on Op1 EOR Op2 */
        case 10: /* CMP: set condition codes on Op1 - Op2 */
        case 11: /* CMN: set condition codes on Op1 + Op2 */
          UnwPrintd1("\t; ????");
          break;

        case 12: /* ORR: Rd:= Op1 OR Op2 */
          state->regData[rd].v = state->regData[rn].v | op2val;
          break;

        case 13: /* MOV: Rd:= Op2 */
          state->regData[rd].v = op2val;
          break;

        case 14: /* BIC: Rd:= Op1 AND NOT Op2 */
          state->regData[rd].v = state->regData[rn].v & (~op2val);
          break;

        case 15: /* MVN: Rd:= NOT Op2 */
          state->regData[rd].v = ~op2val;
          break;
      }

      /* Remove the prefetch offset from the PC */
      if (rd != 15 && rn == 15)
        state->regData[rn].v -= ((!I && (operand2 & 0x0010)) ? 12 : 8);
    }

    /* Block Data Transfer
     *  LDM, STM
     */
    else if ((instr & 0xFE000000) == 0xE8000000) {

      bool     P         = !!(instr & 0x01000000),
               U         = !!(instr & 0x00800000),
               S         = !!(instr & 0x00400000),
               W         = !!(instr & 0x00200000),
               L         = !!(instr & 0x00100000);
      uint16_t baseReg   = (instr & 0x000F0000) >> 16;
      uint16_t regList   = (instr & 0x0000FFFF);
      uint32_t addr      = state->regData[baseReg].v;
      bool     addrValid = M_IsOriginValid(state->regData[baseReg].o);
      int8_t r;

      #ifdef UNW_DEBUG
        /* Display the instruction */
        if (L)
          UnwPrintd6("LDM%c%c r%d%s, {reglist}%s\n", P ? 'E' : 'F', U ? 'D' : 'A', baseReg, W ? "!" : "", S ? "^" : "");
        else
          UnwPrintd6("STM%c%c r%d%s, {reglist}%s\n", !P ? 'E' : 'F', !U ? 'D' : 'A', baseReg, W ? "!" : "", S ? "^" : "");
      #endif

      /* S indicates that banked registers (untracked) are used, unless
       *  this is a load including the PC when the S-bit indicates that
       *  that CPSR is loaded from SPSR (also untracked, but ignored).
       */
      if (S && (!L || (regList & (0x01 << 15)) == 0)) {
        UnwPrintd1("\nError:S-bit set requiring banked registers\n");
        return UNWIND_FAILURE;
      }
      else if (baseReg == 15) {
        UnwPrintd1("\nError: r15 used as base register\n");
        return UNWIND_FAILURE;
      }
      else if (regList == 0) {
        UnwPrintd1("\nError: Register list empty\n");
        return UNWIND_FAILURE;
      }

      /* Check if ascending or descending.
       *  Registers are loaded/stored in order of address.
       *  i.e. r0 is at the lowest address, r15 at the highest.
       */
      r = U ? 0 : 15;
      do {

        /* Check if the register is to be transferred */
        if (regList & (0x01 << r)) {

          if (P) addr += U ? 4 : -4;

          if (L) {

            if (addrValid) {

              if (!UnwMemReadRegister(state, addr, &state->regData[r]))
                return UNWIND_DREAD_W_FAIL;

              /* Update the origin if read via the stack pointer */
              if (M_IsOriginValid(state->regData[r].o) && baseReg == 13)
                state->regData[r].o = REG_VAL_FROM_STACK;

              UnwPrintd5(" R%d = 0x%08x\t; r%d %s\n",r,state->regData[r].v,r, M_Origin2Str(state->regData[r].o));
            }
            else {
              /* Invalidate the register as the base reg was invalid */
              state->regData[r].o = REG_VAL_INVALID;
              UnwPrintd2(" R%d = ???\n", r);
            }
          }
          else {
            if (addrValid && !UnwMemWriteRegister(state, state->regData[13].v, &state->regData[r]))
              return UNWIND_DWRITE_W_FAIL;

            UnwPrintd2(" R%d = 0x%08x\n", r);
          }

          if (!P) addr += U ? 4 : -4;
        }

        /* Check the next register */
        r += U ? 1 : -1;

      } while (r >= 0 && r <= 15);

      /* Check the writeback bit */
      if (W) state->regData[baseReg].v = addr;

      /* Check if the PC was loaded */
      if (L && (regList & (0x01 << 15))) {
        if (!M_IsOriginValid(state->regData[15].o)) {
          /* Return address is not valid */
          UnwPrintd1("PC popped with invalid address\n");
          return UNWIND_FAILURE;
        }
        else {
          /* Store the return address */
          if (!UnwReportRetAddr(state, state->regData[15].v))
            return UNWIND_TRUNCATED;

          UnwPrintd2("  Return PC=0x%x", state->regData[15].v);

          /* Determine the return mode */
          if (state->regData[15].v & 0x1) {
            /* Branching to THUMB */
            return UnwStartThumb(state);
          }
          else {
            /* Branch to ARM */

            /* Account for the auto-increment which isn't needed */
            state->regData[15].v -= 4;
          }
        }
      }
    }
    else {
      UnwPrintd1("????");

      /* Unknown/undecoded.  May alter some register, so invalidate file */
      UnwInvalidateRegisterFile(state->regData);
    }

    UnwPrintd1("\n");

    /* Should never hit the reset vector */
    if (state->regData[15].v == 0) return UNWIND_RESET;

    /* Check next address */
    state->regData[15].v += 4;

    /* Garbage collect the memory hash (used only for the stack) */
    UnwMemHashGC(state);

    if (--t == 0) return UNWIND_EXHAUSTED;

  } while (!found);

  return UNWIND_UNSUPPORTED;
}

#endif // __arm__ || __thumb__
