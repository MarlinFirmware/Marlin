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
 * File Description: Abstract interpretation for Thumb mode.
 **************************************************************************/

#if defined(__arm__) || defined(__thumb__)

#define MODULE_NAME "UNWARM_THUMB"

#include <stdio.h>
#include "unwarm.h"

/** Sign extend an 11 bit value.
 * This function simply inspects bit 11 of the input \a value, and if
 * set, the top 5 bits are set to give a 2's compliment signed value.
 * \param value   The value to sign extend.
 * \return The signed-11 bit value stored in a 16bit data type.
 */
static int32_t signExtend11(const uint16_t value) {
  return (value & 0x400) ? value | 0xFFFFF800 : value;
}

UnwResult UnwStartThumb(UnwState * const state) {
  bool found = false;
  uint16_t t = UNW_MAX_INSTR_COUNT;
  uint32_t lastJumpAddr = 0;  // Last JUMP address, to try to detect infinite loops
  bool loopDetected = false;  // If a loop was detected

  do {
    uint16_t instr;

    /* Attempt to read the instruction */
    if (!state->cb->readH(state->regData[15].v & (~0x1), &instr))
      return UNWIND_IREAD_H_FAIL;

    UnwPrintd4("T %x %x %04x:", state->regData[13].v, state->regData[15].v, instr);

    /* Check that the PC is still on Thumb alignment */
    if (!(state->regData[15].v & 0x1)) {
      UnwPrintd1("\nError: PC misalignment\n");
      return UNWIND_INCONSISTENT;
    }

    /* Check that the SP and PC have not been invalidated */
    if (!M_IsOriginValid(state->regData[13].o) || !M_IsOriginValid(state->regData[15].o)) {
      UnwPrintd1("\nError: PC or SP invalidated\n");
      return UNWIND_INCONSISTENT;
    }

    /*
     * Detect 32bit thumb instructions
     */
    if ((instr & 0xE000) == 0xE000 && (instr & 0x1800) != 0) {
      uint16_t instr2;

      /* Check next address */
      state->regData[15].v += 2;

      /* Attempt to read the 2nd part of the instruction */
      if (!state->cb->readH(state->regData[15].v & (~0x1), &instr2))
        return UNWIND_IREAD_H_FAIL;

      UnwPrintd3(" %x %04x:", state->regData[15].v, instr2);

      /*
       * Load/Store multiple: Only interpret
       *  PUSH and POP
       */
      if ((instr & 0xFE6F) == 0xE82D) {
        bool     L     = !!(instr &  0x10);
        uint16_t rList = instr2;

        if (L) {
          uint8_t r;

          /* Load from memory: POP */
          UnwPrintd1("POP {Rlist}\n");

          /* Load registers from stack */
          for (r = 0; r < 16; r++) {
            if (rList & (0x1 << r)) {

              /* Read the word */
              if (!UnwMemReadRegister(state, state->regData[13].v, &state->regData[r]))
                return UNWIND_DREAD_W_FAIL;

              /* Alter the origin to be from the stack if it was valid */
              if (M_IsOriginValid(state->regData[r].o)) {

                state->regData[r].o = REG_VAL_FROM_STACK;

                /* If restoring the PC */
                if (r == 15) {

                  /* The bottom bit should have been set to indicate that
                   *  the caller was from Thumb.  This would allow return
                   *  by BX for interworking APCS.
                   */
                  if ((state->regData[15].v & 0x1) == 0) {
                    UnwPrintd2("Warning: Return address not to Thumb: 0x%08x\n", state->regData[15].v);

                    /* Pop into the PC will not switch mode */
                    return UNWIND_INCONSISTENT;
                  }

                  /* Store the return address */
                  if (!UnwReportRetAddr(state, state->regData[15].v))
                    return UNWIND_TRUNCATED;

                  /* Now have the return address */
                  UnwPrintd2(" Return PC=%x\n", state->regData[15].v);

                  /* Compensate for the auto-increment, which isn't needed here */
                  state->regData[15].v -= 2;

                }

              } else {

                if (r == 15) {
                  /* Return address is not valid */
                  UnwPrintd1("PC popped with invalid address\n");
                  return UNWIND_FAILURE;
                }
              }

              state->regData[13].v += 4;

              UnwPrintd3("  r%d = 0x%08x\n", r, state->regData[r].v);
            }
          }
        }
        else {
          int8_t r;

          /* Store to memory: PUSH */
          UnwPrintd1("PUSH {Rlist}");

          for (r = 15; r >= 0; r--) {
            if (rList & (0x1 << r)) {
              UnwPrintd4("\n  r%d = 0x%08x\t; %s", r, state->regData[r].v, M_Origin2Str(state->regData[r].o));

              state->regData[13].v -= 4;

              if (!UnwMemWriteRegister(state, state->regData[13].v, &state->regData[r]))
                return UNWIND_DWRITE_W_FAIL;
            }
          }
        }
      }
      /*
       * PUSH register
       */
      else if (instr == 0xF84D && (instr2 & 0x0FFF) == 0x0D04) {
        uint8_t r = instr2 >> 12;

        /* Store to memory: PUSH */
        UnwPrintd2("PUSH {R%d}\n", r);
        UnwPrintd4("\n  r%d = 0x%08x\t; %s", r, state->regData[r].v, M_Origin2Str(state->regData[r].o));

        state->regData[13].v -= 4;

        if (!UnwMemWriteRegister(state, state->regData[13].v, &state->regData[r]))
          return UNWIND_DWRITE_W_FAIL;
      }
      /*
       * POP register
       */
      else if (instr == 0xF85D && (instr2 & 0x0FFF) == 0x0B04) {
        uint8_t r = instr2 >> 12;

        /* Load from memory: POP */
        UnwPrintd2("POP {R%d}\n", r);

        /* Read the word */
        if (!UnwMemReadRegister(state, state->regData[13].v, &state->regData[r]))
          return UNWIND_DREAD_W_FAIL;

        /* Alter the origin to be from the stack if it was valid */
        if (M_IsOriginValid(state->regData[r].o)) {

          state->regData[r].o = REG_VAL_FROM_STACK;

          /* If restoring the PC */
          if (r == 15) {

            /* The bottom bit should have been set to indicate that
             *  the caller was from Thumb.  This would allow return
             *  by BX for interworking APCS.
             */
            if ((state->regData[15].v & 0x1) == 0) {
              UnwPrintd2("Warning: Return address not to Thumb: 0x%08x\n", state->regData[15].v);

              /* Pop into the PC will not switch mode */
              return UNWIND_INCONSISTENT;
            }

            /* Store the return address */
            if (!UnwReportRetAddr(state, state->regData[15].v))
              return UNWIND_TRUNCATED;

            /* Now have the return address */
            UnwPrintd2(" Return PC=%x\n", state->regData[15].v);

            /* Compensate for the auto-increment, which isn't needed here */
            state->regData[15].v -= 2;

          }

        } else {

          if (r == 15) {
            /* Return address is not valid */
            UnwPrintd1("PC popped with invalid address\n");
            return UNWIND_FAILURE;
          }
        }

        state->regData[13].v += 4;

        UnwPrintd3("  r%d = 0x%08x\n", r, state->regData[r].v);
      }
      /*
       * TBB / TBH
       */
      else if ((instr & 0xFFF0) == 0xE8D0 && (instr2 & 0xFFE0) == 0xF000) {
        /* We are only interested in
         * the forms
         *  TBB [PC, ...]
         *  TBH [PC, ..., LSL #1]
         * as those are used by the C compiler to implement
         * the switch clauses
         */
        uint8_t rn = instr & 0xF;
        bool H = !!(instr2 & 0x10);

        UnwPrintd5("TB%c [r%d,r%d%s]\n", H ? 'H' : 'B', rn, (instr2 & 0xF), H ? ",LSL #1" : "");

        // We are only interested if the RN is the PC. Let's choose the 1st destination
        if (rn == 15) {
          if (H) {
            uint16_t rv;
            if (!state->cb->readH((state->regData[15].v & (~1)) + 2, &rv))
              return UNWIND_DREAD_H_FAIL;
            state->regData[15].v += rv * 2;
          }
          else {
            uint8_t rv;
            if (!state->cb->readB((state->regData[15].v & (~1)) + 2, &rv))
              return UNWIND_DREAD_B_FAIL;
            state->regData[15].v += rv * 2;
          }
        }
      }
      /*
       * Unconditional branch
       */
      else if ((instr & 0xF800) == 0xF000 && (instr2 & 0xD000) == 0x9000) {
        uint32_t v;

        uint8_t      S = (instr & 0x400) >> 10;
        uint16_t imm10 = (instr & 0x3FF);
        uint8_t     J1 = (instr2 & 0x2000) >> 13;
        uint8_t     J2 = (instr2 & 0x0800) >> 11;
        uint16_t imm11 = (instr2 & 0x7FF);

        uint8_t I1 = J1 ^ S ^ 1;
        uint8_t I2 = J2 ^ S ^ 1;
        uint32_t imm32 = (S << 24) | (I1 << 23) | (I2 << 22) |(imm10 << 12) | (imm11 << 1);
        if (S) imm32 |= 0xFE000000;

        UnwPrintd2("B %d \n", imm32);

        /* Update PC */
        state->regData[15].v += imm32;

        /* Need to advance by a word to account for pre-fetch.
         *  Advance by a half word here, allowing the normal address
         *  advance to account for the other half word.
         */
        state->regData[15].v += 2;

        /* Compute the jump address */
        v = state->regData[15].v + 2;

        /* Display PC of next instruction */
        UnwPrintd2(" New PC=%x", v);

        /* Did we detect an infinite loop ? */
        loopDetected = lastJumpAddr == v;

        /* Remember the last address we jumped to  */
        lastJumpAddr = v;
      }

      /*
       * Branch with link
       */
      else if ((instr & 0xF800) == 0xF000 && (instr2 & 0xD000) == 0xD000) {

        uint8_t      S = (instr & 0x400) >> 10;
        uint16_t imm10 = (instr & 0x3FF);
        uint8_t     J1 = (instr2 & 0x2000) >> 13;
        uint8_t     J2 = (instr2 & 0x0800) >> 11;
        uint16_t imm11 = (instr2 & 0x7FF);

        uint8_t I1 = J1 ^ S ^ 1;
        uint8_t I2 = J2 ^ S ^ 1;
        uint32_t imm32 = (S << 24) | (I1 << 23) | (I2 << 22) |(imm10 << 12) | (imm11 << 1);
        if (S) imm32 |= 0xFE000000;

        UnwPrintd2("BL %d \n", imm32);

        /* Never taken, as we are unwinding the stack */
        if (0) {

          /* Store return address in LR register */
          state->regData[14].v = state->regData[15].v + 2;
          state->regData[14].o = REG_VAL_FROM_CONST;

          /* Update PC */
          state->regData[15].v += imm32;

          /* Need to advance by a word to account for pre-fetch.
           *  Advance by a half word here, allowing the normal address
           *  advance to account for the other half word.
           */
          state->regData[15].v += 2;

          /* Display PC of next instruction */
          UnwPrintd2(" Return PC=%x", state->regData[15].v);

          /* Report the return address, including mode bit */
          if (!UnwReportRetAddr(state, state->regData[15].v))
            return UNWIND_TRUNCATED;

          /* Determine the new mode */
          if (state->regData[15].v & 0x1) {
            /* Branching to THUMB */

            /* Account for the auto-increment which isn't needed */
            state->regData[15].v -= 2;
          }
          else {
            /* Branch to ARM */
            return UnwStartArm(state);
          }
        }
      }

      /*
       * Conditional branches. Usually not taken, unless infinite loop is detected
       */
      else if ((instr & 0xF800) == 0xF000 && (instr2 & 0xD000) == 0x8000) {

        uint8_t      S = (instr & 0x400) >> 10;
        uint16_t  imm6 = (instr &  0x3F);
        uint8_t     J1 = (instr2 & 0x2000) >> 13;
        uint8_t     J2 = (instr2 & 0x0800) >> 11;
        uint16_t imm11 = (instr2 & 0x7FF);

        uint8_t I1 = J1 ^ S ^ 1;
        uint8_t I2 = J2 ^ S ^ 1;
        uint32_t imm32 = (S << 20) | (I1 << 19) | (I2 << 18) |(imm6 << 12) | (imm11 << 1);
        if (S) imm32 |= 0xFFE00000;

        UnwPrintd2("Bcond %d\n", imm32);

        /* Take the jump only if a loop is detected */
        if (loopDetected) {

          /* Update PC */
          state->regData[15].v += imm32;

          /* Need to advance by a word to account for pre-fetch.
           *  Advance by a half word here, allowing the normal address
           *  advance to account for the other half word.
           */
          state->regData[15].v += 2;

          /* Display PC of next instruction */
          UnwPrintd2(" New PC=%x", state->regData[15].v + 2);
        }
      }
      /*
       * PC-relative load
       *  LDR Rd,[PC, #+/-imm]
       */
      else if ((instr & 0xFF7F) == 0xF85F) {
        uint8_t  rt    = (instr2 & 0xF000) >> 12;
        uint8_t  imm12 = (instr2 & 0x0FFF);
        bool     A     = !!(instr  & 0x80);
        uint32_t address;

        /* Compute load address, adding a word to account for prefetch */
        address = (state->regData[15].v & (~0x3)) + 4;
        if (A) address += imm12;
        else address -= imm12;

        UnwPrintd4("LDR r%d,[PC #%c0x%08x]", rt, A?'+':'-', address);

        if (!UnwMemReadRegister(state, address, &state->regData[rt]))
          return UNWIND_DREAD_W_FAIL;
      }
      /*
       * LDR immediate.
       *  We are only interested when destination is PC.
       *  LDR Rt,[Rn , #n]
       */
      else if ((instr & 0xFFF0) == 0xF8D0) {
        uint8_t     rn = (instr  & 0xF);
        uint8_t     rt = (instr2 & 0xF000) >> 12;
        uint16_t imm12 = (instr2 & 0xFFF);

        /* If destination is PC and we don't know the source value, then fail */
        if (!M_IsOriginValid(state->regData[rn].o)) {
          state->regData[rt].o = state->regData[rn].o;
        }
        else {
          uint32_t address = state->regData[rn].v + imm12;
          if (!UnwMemReadRegister(state, address, &state->regData[rt]))
            return UNWIND_DREAD_W_FAIL;
        }
      }
      /*
       * LDR immediate
       *  We are only interested when destination is PC.
       *  LDR Rt,[Rn , #-n]
       *  LDR Rt,[Rn], #+/-n]
       *  LDR Rt,[Rn, #+/-n]!
       */
      else if ((instr & 0xFFF0) == 0xF850 && (instr2 & 0x0800) == 0x0800) {
        uint8_t     rn = (instr  & 0xF);
        uint8_t     rt = (instr2 & 0xF000) >> 12;
        uint16_t  imm8 = (instr2 & 0xFF);
        bool         P = !!(instr2 & 0x400);
        bool         U = !!(instr2 & 0x200);
        bool         W = !!(instr2 & 0x100);

        if (!M_IsOriginValid(state->regData[rn].o))
          state->regData[rt].o = state->regData[rn].o;
        else {
          uint32_t offaddress = state->regData[rn].v + (U ? imm8 + imm8 : 0),
                   address = P ? offaddress : state->regData[rn].v;

          if (!UnwMemReadRegister(state, address, &state->regData[rt]))
            return UNWIND_DREAD_W_FAIL;

          if (W) state->regData[rn].v = offaddress;
        }
      }
      /*
       * LDR (register).
       *  We are interested in the form
       *   ldr  Rt, [Rn, Rm, lsl #x]
       *  Where Rt is PC, Rn value is known, Rm is not known or unknown
       */
      else if ((instr & 0xFFF0) == 0xF850 && (instr2 & 0x0FC0) == 0x0000) {
        const uint8_t rn = (instr  & 0xF),
                      rt = (instr2 & 0xF000) >> 12,
                      rm = (instr2 & 0xF),
                    imm2 = (instr2 & 0x30) >> 4;

        if (!M_IsOriginValid(state->regData[rn].o) || !M_IsOriginValid(state->regData[rm].o)) {

          /* If Rt is PC, and Rn is known, then do an exception and assume
             Rm equals 0 => This takes the first case in a switch() */
          if (rt == 15 && M_IsOriginValid(state->regData[rn].o)) {
            uint32_t address = state->regData[rn].v;
            if (!UnwMemReadRegister(state, address, &state->regData[rt]))
              return UNWIND_DREAD_W_FAIL;
          }
          else /* Propagate unknown value */
            state->regData[rt].o = state->regData[rn].o;

        }
        else {
          uint32_t address = state->regData[rn].v + (state->regData[rm].v << imm2);
          if (!UnwMemReadRegister(state, address, &state->regData[rt]))
            return UNWIND_DREAD_W_FAIL;
        }
      }
      else {
        UnwPrintd1("???? (32)");

        /* Unknown/undecoded.  May alter some register, so invalidate file */
        UnwInvalidateRegisterFile(state->regData);
      }
      /* End of thumb 32bit code */

    }
    /* Format 1: Move shifted register
     *  LSL Rd, Rs, #Offset5
     *  LSR Rd, Rs, #Offset5
     *  ASR Rd, Rs, #Offset5
     */
    else if ((instr & 0xE000) == 0x0000 && (instr & 0x1800) != 0x1800) {
      bool signExtend;
      const uint8_t op      = (instr & 0x1800) >> 11,
                    offset5 = (instr & 0x07C0) >>  6,
                    rs      = (instr & 0x0038) >>  3,
                    rd      = (instr & 0x0007);

      switch (op) {
        case 0: /* LSL */
          UnwPrintd6("LSL r%d, r%d, #%d\t; r%d %s", rd, rs, offset5, rs, M_Origin2Str(state->regData[rs].o));
          state->regData[rd].v = state->regData[rs].v << offset5;
          state->regData[rd].o = state->regData[rs].o;
          state->regData[rd].o |= REG_VAL_ARITHMETIC;
          break;

        case 1: /* LSR */
          UnwPrintd6("LSR r%d, r%d, #%d\t; r%d %s", rd, rs, offset5, rs, M_Origin2Str(state->regData[rs].o));
          state->regData[rd].v = state->regData[rs].v >> offset5;
          state->regData[rd].o = state->regData[rs].o;
          state->regData[rd].o |= REG_VAL_ARITHMETIC;
          break;

        case 2: /* ASR */
          UnwPrintd6("ASL r%d, r%d, #%d\t; r%d %s", rd, rs, offset5, rs, M_Origin2Str(state->regData[rs].o));

          signExtend = !!(state->regData[rs].v & 0x8000);
          state->regData[rd].v = state->regData[rs].v >> offset5;
          if (signExtend) state->regData[rd].v |= 0xFFFFFFFF << (32 - offset5);
          state->regData[rd].o = state->regData[rs].o;
          state->regData[rd].o |= REG_VAL_ARITHMETIC;
          break;
      }
    }
    /* Format 2: add/subtract
     *  ADD Rd, Rs, Rn
     *  ADD Rd, Rs, #Offset3
     *  SUB Rd, Rs, Rn
     *  SUB Rd, Rs, #Offset3
     */
    else if ((instr & 0xF800) == 0x1800) {
      bool    I  = !!(instr & 0x0400);
      bool    op = !!(instr & 0x0200);
      uint8_t rn = (instr & 0x01C0) >> 6;
      uint8_t rs = (instr & 0x0038) >> 3;
      uint8_t rd = (instr & 0x0007);

      /* Print decoding */
      UnwPrintd6("%s r%d, r%d, %c%d\t;",op ? "SUB" : "ADD",rd, rs,I ? '#' : 'r',rn);
      UnwPrintd5("r%d %s, r%d %s",rd, M_Origin2Str(state->regData[rd].o),rs, M_Origin2Str(state->regData[rs].o));
      if (!I) {

        UnwPrintd3(", r%d %s", rn, M_Origin2Str(state->regData[rn].o));

        /* Perform calculation */
        state->regData[rd].v = state->regData[rs].v + (op ? -state->regData[rn].v : state->regData[rn].v);

        /* Propagate the origin */
        if (M_IsOriginValid(state->regData[rs].o) && M_IsOriginValid(state->regData[rn].o)) {
          state->regData[rd].o = state->regData[rs].o;
          state->regData[rd].o |= REG_VAL_ARITHMETIC;
        }
        else
          state->regData[rd].o = REG_VAL_INVALID;
      }
      else {
        /* Perform calculation */
        state->regData[rd].v = state->regData[rs].v + (op ? -rn : rn);

        /* Propagate the origin */
        state->regData[rd].o = state->regData[rs].o;
        state->regData[rd].o |= REG_VAL_ARITHMETIC;
      }
    }
    /* Format 3: move/compare/add/subtract immediate
     *  MOV Rd, #Offset8
     *  CMP Rd, #Offset8
     *  ADD Rd, #Offset8
     *  SUB Rd, #Offset8
     */
    else if ((instr & 0xE000) == 0x2000) {

      uint8_t op      = (instr & 0x1800) >> 11;
      uint8_t rd      = (instr & 0x0700) >>  8;
      uint8_t offset8 = (instr & 0x00FF);

      switch (op) {
        case 0: /* MOV */
          UnwPrintd3("MOV r%d, #0x%x", rd, offset8);
          state->regData[rd].v = offset8;
          state->regData[rd].o = REG_VAL_FROM_CONST;
          break;

        case 1: /* CMP */
          /* Irrelevant to unwinding */
          UnwPrintd1("CMP ???");
          break;

        case 2: /* ADD */
          UnwPrintd5("ADD r%d, #0x%x\t; r%d %s", rd, offset8, rd, M_Origin2Str(state->regData[rd].o));
          state->regData[rd].v += offset8;
          state->regData[rd].o |= REG_VAL_ARITHMETIC;
          break;

        case 3: /* SUB */
          UnwPrintd5("SUB r%d, #0x%d\t; r%d %s", rd, offset8, rd, M_Origin2Str(state->regData[rd].o));
          state->regData[rd].v -= offset8;
          state->regData[rd].o |= REG_VAL_ARITHMETIC;
          break;
      }
    }
    /* Format 4: ALU operations
     *  AND Rd, Rs
     *  EOR Rd, Rs
     *  LSL Rd, Rs
     *  LSR Rd, Rs
     *  ASR Rd, Rs
     *  ADC Rd, Rs
     *  SBC Rd, Rs
     *  ROR Rd, Rs
     *  TST Rd, Rs
     *  NEG Rd, Rs
     *  CMP Rd, Rs
     *  CMN Rd, Rs
     *  ORR Rd, Rs
     *  MUL Rd, Rs
     *  BIC Rd, Rs
     *  MVN Rd, Rs
     */
    else if ((instr & 0xFC00) == 0x4000) {
      uint8_t op = (instr & 0x03C0) >> 6;
      uint8_t rs = (instr & 0x0038) >> 3;
      uint8_t rd = (instr & 0x0007);

#ifdef UNW_DEBUG
      static const char * const mnu[16] = {
        "AND", "EOR", "LSL", "LSR",
        "ASR", "ADC", "SBC", "ROR",
        "TST", "NEG", "CMP", "CMN",
        "ORR", "MUL", "BIC", "MVN" };
#endif
      /* Print the mnemonic and registers */
      switch (op) {
        case 0: /* AND */
        case 1: /* EOR */
        case 2: /* LSL */
        case 3: /* LSR */
        case 4: /* ASR */
        case 7: /* ROR */
        case 9: /* NEG */
        case 12: /* ORR */
        case 13: /* MUL */
        case 15: /* MVN */
          UnwPrintd8("%s r%d ,r%d\t; r%d %s, r%d %s",mnu[op],rd, rs, rd, M_Origin2Str(state->regData[rd].o), rs, M_Origin2Str(state->regData[rs].o));
          break;

        case 5: /* ADC */
        case 6: /* SBC */
          UnwPrintd4("%s r%d, r%d", mnu[op], rd, rs);
          break;

        case 8: /* TST */
        case 10: /* CMP */
        case 11: /* CMN */
          /* Irrelevant to unwinding */
          UnwPrintd2("%s ???", mnu[op]);
          break;

        case 14: /* BIC */
          UnwPrintd5("r%d ,r%d\t; r%d %s", rd, rs, rs, M_Origin2Str(state->regData[rs].o));
          break;
      }

      /* Perform operation */
      switch (op) {
        case 0: /* AND */
          state->regData[rd].v &= state->regData[rs].v;
          break;

        case 1: /* EOR */
          state->regData[rd].v ^= state->regData[rs].v;
          break;

        case 2: /* LSL */
          state->regData[rd].v <<= state->regData[rs].v;
          break;

        case 3: /* LSR */
          state->regData[rd].v >>= state->regData[rs].v;
          break;

        case 4: /* ASR */
          if (state->regData[rd].v & 0x80000000) {
            state->regData[rd].v >>= state->regData[rs].v;
            state->regData[rd].v |= 0xFFFFFFFF << (32 - state->regData[rs].v);
          }
          else {
            state->regData[rd].v >>= state->regData[rs].v;
          }

          break;

        case 5: /* ADC */
        case 6: /* SBC */
        case 8: /* TST */
        case 10: /* CMP */
        case 11: /* CMN */
          break;

        case 7: /* ROR */
          state->regData[rd].v = (state->regData[rd].v >> state->regData[rs].v) |
                          (state->regData[rd].v << (32 - state->regData[rs].v));
          break;

        case 9: /* NEG */
          state->regData[rd].v = -state->regData[rs].v;
          break;

        case 12: /* ORR */
          state->regData[rd].v |= state->regData[rs].v;
          break;

        case 13: /* MUL */
          state->regData[rd].v *= state->regData[rs].v;
          break;

        case 14: /* BIC */
          state->regData[rd].v &= ~state->regData[rs].v;
          break;

        case 15: /* MVN */
          state->regData[rd].v = ~state->regData[rs].v;
          break;
      }

      /* Propagate data origins */
      switch (op) {
        case 0: /* AND */
        case 1: /* EOR */
        case 2: /* LSL */
        case 3: /* LSR */
        case 4: /* ASR */
        case 7: /* ROR */
        case 12: /* ORR */
        case 13: /* MUL */
        case 14: /* BIC */
          if (M_IsOriginValid(state->regData[rd].o) && M_IsOriginValid(state->regData[rs].o)) {
            state->regData[rd].o = state->regData[rs].o;
            state->regData[rd].o |= REG_VAL_ARITHMETIC;
          }
          else
            state->regData[rd].o = REG_VAL_INVALID;
          break;

        case 5: /* ADC */
        case 6: /* SBC */
          /* C-bit not tracked */
          state->regData[rd].o = REG_VAL_INVALID;
          break;

        case 8: /* TST */
        case 10: /* CMP */
        case 11: /* CMN */
          /* Nothing propagated */
          break;

        case 9: /* NEG */
        case 15: /* MVN */
          state->regData[rd].o = state->regData[rs].o;
          state->regData[rd].o |= REG_VAL_ARITHMETIC;
          break;
      }
    }
    /* Format 5: Hi register operations/branch exchange
     *  ADD Rd, Hs
     *  CMP Hd, Rs
     *  MOV Hd, Hs
     */
    else if ((instr & 0xFC00) == 0x4400) {
      uint8_t op  = (instr & 0x0300) >> 8;
      bool    h1  = (instr & 0x0080) ? true: false;
      bool    h2  = (instr & 0x0040) ? true: false;
      uint8_t rhs = (instr & 0x0038) >> 3;
      uint8_t rhd = (instr & 0x0007);

      /* Adjust the register numbers */
      if (h2) rhs += 8;
      if (h1) rhd += 8;

      switch (op) {
        case 0: /* ADD */
          UnwPrintd5("ADD r%d, r%d\t; r%d %s", rhd, rhs, rhs, M_Origin2Str(state->regData[rhs].o));
          state->regData[rhd].v += state->regData[rhs].v;
          state->regData[rhd].o  =  state->regData[rhs].o;
          state->regData[rhd].o |= REG_VAL_ARITHMETIC;
          break;

        case 1: /* CMP */
          /* Irrelevant to unwinding */
          UnwPrintd1("CMP ???");
          break;

        case 2: /* MOV */
          UnwPrintd5("MOV r%d, r%d\t; r%d %s", rhd, rhs, rhd, M_Origin2Str(state->regData[rhs].o));
          state->regData[rhd].v  = state->regData[rhs].v;
          state->regData[rhd].o  = state->regData[rhs].o;
          break;

        case 3: /* BX */
          UnwPrintd4("BX r%d\t; r%d %s\n", rhs, rhs, M_Origin2Str(state->regData[rhs].o));

          /* Only follow BX if the data was from the stack or BX LR */
          if (rhs == 14 || state->regData[rhs].o == REG_VAL_FROM_STACK) {
            UnwPrintd2(" Return PC=0x%x\n", state->regData[rhs].v & (~0x1));

            /* Report the return address, including mode bit */
            if (!UnwReportRetAddr(state, state->regData[rhs].v))
              return UNWIND_TRUNCATED;

            /* Update the PC */
            state->regData[15].v = state->regData[rhs].v;

            /* Determine the new mode */
            if (state->regData[rhs].v & 0x1) {
              /* Branching to THUMB */

              /* Account for the auto-increment which isn't needed */
              state->regData[15].v -= 2;
            }
            else /* Branch to ARM */
              return UnwStartArm(state);
          }
          else {
            UnwPrintd4("\nError: BX to invalid register: r%d = 0x%x (%s)\n", rhs, state->regData[rhs].o, M_Origin2Str(state->regData[rhs].o));
            return UNWIND_FAILURE;
          }
      }
    }
    /* Format 9: PC-relative load
     *  LDR Rd,[PC, #imm]
     */
    else if ((instr & 0xF800) == 0x4800) {
      uint8_t  rd    = (instr & 0x0700) >> 8;
      uint8_t  word8 = (instr & 0x00FF);
      uint32_t address;

      /* Compute load address, adding a word to account for prefetch */
      address = (state->regData[15].v & (~0x3)) + 4 + (word8 << 2);

      UnwPrintd3("LDR r%d, 0x%08x", rd, address);

      if (!UnwMemReadRegister(state, address, &state->regData[rd]))
        return UNWIND_DREAD_W_FAIL;
    }
    /* Format 13: add offset to Stack Pointer
     *  ADD sp,#+imm
     *  ADD sp,#-imm
     */
    else if ((instr & 0xFF00) == 0xB000) {
      uint8_t value = (instr & 0x7F) * 4;

      /* Check the negative bit */
      if ((instr & 0x80) != 0) {
        UnwPrintd2("SUB sp,#0x%x", value);
        state->regData[13].v -= value;
      }
      else {
        UnwPrintd2("ADD sp,#0x%x", value);
        state->regData[13].v += value;
      }
    }
    /* Format 14: push/pop registers
     *  PUSH {Rlist}
     *  PUSH {Rlist, LR}
     *  POP {Rlist}
     *  POP {Rlist, PC}
     */
    else if ((instr & 0xF600) == 0xB400) {
      bool    L     = !!(instr & 0x0800);
      bool    R     = !!(instr & 0x0100);
      uint8_t rList = (instr & 0x00FF);

      if (L) {
        uint8_t r;

        /* Load from memory: POP */
        UnwPrintd2("POP {Rlist%s}\n", R ? ", PC" : "");

        for (r = 0; r < 8; r++) {
          if (rList & (0x1 << r)) {

            /* Read the word */
            if (!UnwMemReadRegister(state, state->regData[13].v, &state->regData[r]))
              return UNWIND_DREAD_W_FAIL;

            /* Alter the origin to be from the stack if it was valid */
            if (M_IsOriginValid(state->regData[r].o))
              state->regData[r].o = REG_VAL_FROM_STACK;

            state->regData[13].v += 4;

            UnwPrintd3("  r%d = 0x%08x\n", r, state->regData[r].v);
          }
        }

        /* Check if the PC is to be popped */
        if (R) {
          /* Get the return address */
          if (!UnwMemReadRegister(state, state->regData[13].v, &state->regData[15]))
            return UNWIND_DREAD_W_FAIL;

          /* Alter the origin to be from the stack if it was valid */
          if (!M_IsOriginValid(state->regData[15].o)) {
            /* Return address is not valid */
            UnwPrintd1("PC popped with invalid address\n");
            return UNWIND_FAILURE;
          }
          else {
            /* The bottom bit should have been set to indicate that
             *  the caller was from Thumb.  This would allow return
             *  by BX for interworking APCS.
             */
            if ((state->regData[15].v & 0x1) == 0) {
              UnwPrintd2("Warning: Return address not to Thumb: 0x%08x\n", state->regData[15].v);

              /* Pop into the PC will not switch mode */
              return UNWIND_INCONSISTENT;
            }

            /* Store the return address */
            if (!UnwReportRetAddr(state, state->regData[15].v))
              return UNWIND_TRUNCATED;

            /* Now have the return address */
            UnwPrintd2(" Return PC=%x\n", state->regData[15].v);

            /* Update the pc */
            state->regData[13].v += 4;

            /* Compensate for the auto-increment, which isn't needed here */
            state->regData[15].v -= 2;
          }
        }
      }
      else {
        int8_t r;

        /* Store to memory: PUSH */
        UnwPrintd2("PUSH {Rlist%s}", R ? ", LR" : "");

        /* Check if the LR is to be pushed */
        if (R) {
          UnwPrintd3("\n  lr = 0x%08x\t; %s", state->regData[14].v, M_Origin2Str(state->regData[14].o));

          state->regData[13].v -= 4;

          /* Write the register value to memory */
          if (!UnwMemWriteRegister(state, state->regData[13].v, &state->regData[14]))
            return UNWIND_DWRITE_W_FAIL;
        }

        for (r = 7; r >= 0; r--) {
          if (rList & (0x1 << r)) {
            UnwPrintd4("\n  r%d = 0x%08x\t; %s", r, state->regData[r].v, M_Origin2Str(state->regData[r].o));

            state->regData[13].v -= 4;

            if (!UnwMemWriteRegister(state, state->regData[13].v, &state->regData[r]))
              return UNWIND_DWRITE_W_FAIL;
          }
        }
      }
    }

    /*
     * Conditional branches
     * Bcond
     */
    else if ((instr & 0xF000) == 0xD000) {
      int32_t branchValue = (instr & 0xFF);
      if (branchValue & 0x80) branchValue |= 0xFFFFFF00;

      /* Branch distance is twice that specified in the instruction. */
      branchValue *= 2;

      UnwPrintd2("Bcond %d \n", branchValue);

      /* Only take the branch if a loop was detected */
      if (loopDetected) {

        /* Update PC */
        state->regData[15].v += branchValue;

        /* Need to advance by a word to account for pre-fetch.
         *  Advance by a half word here, allowing the normal address
         *  advance to account for the other half word.
         */
        state->regData[15].v += 2;

        /* Display PC of next instruction */
        UnwPrintd2(" New PC=%x", state->regData[15].v + 2);
      }
    }

    /* Format 18: unconditional branch
     *  B label
     */
    else if ((instr & 0xF800) == 0xE000) {
      uint32_t v;
      int32_t branchValue = signExtend11(instr & 0x07FF);

      /* Branch distance is twice that specified in the instruction. */
      branchValue *= 2;

      UnwPrintd2("B %d \n", branchValue);

      /* Update PC */
      state->regData[15].v += branchValue;

      /* Need to advance by a word to account for pre-fetch.
       *  Advance by a half word here, allowing the normal address
       *  advance to account for the other half word.
       */
      state->regData[15].v += 2;

      /* Compute the jump address */
      v = state->regData[15].v + 2;

      /* Display PC of next instruction */
      UnwPrintd2(" New PC=%x", v);

      /* Did we detect an infinite loop ? */
      loopDetected = lastJumpAddr == v;

      /* Remember the last address we jumped to  */
      lastJumpAddr = v;
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
    state->regData[15].v += 2;

    /* Garbage collect the memory hash (used only for the stack) */
    UnwMemHashGC(state);

    if (--t == 0) return UNWIND_EXHAUSTED;

  } while (!found);

  return UNWIND_SUCCESS;
}

#endif // __arm__ || __thumb__
