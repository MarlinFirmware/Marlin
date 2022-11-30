/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
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

#pragma once

// OVERVIEW OF PREPROCESSOR DEFINITIONS:
// __AVR_ATmega2560__
// __AVR_ATmega1284P__
// __AVR_ATmega1280__
// __AVR_ATmega644__
// __AVR_ATmega644P__
// __AVR_ATmega2561__

// Contributed by Martin Turski, company owner of EirDev, on the 29th of November, 2022
// Contact E-Mail: turningtides@outlook.de
// Created specifically for the Marlin FW for AVR backwards-compatibility.
// Please expand this file with details of every supported AVR implementation.
// 1) download the latest technical reference manual
// 2) add the new technical reference manual below using a set of __AVR_*__ preprocessor definitions and a new __AVR_TRM*__ incrementing define
// 3) check which of the existing AVR registers exist on the new implementation and enable them
// 4) add any new register definitions
// 5) add the register memory layout below the definitions
// 6) extend the _ATmega_resetperipherals functions
// 7) extend the _ATmega_savePinAlternate function
// 8) copy the extension idea to _ATmega_restorePinAlternate and finish implementing it
// You need to adjust the eATmegaPort enumeration aswell.

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__)
#error Fatal error: __AVR_TRMn__ already defined! (n: 01|02|03|04)
#endif

#if defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || defined(__AVR_ATmega640__)
// ATmega2560 technical reference manual date: 28th of November, 2022
// ATmega640-1280-1281-2560-2561-Datasheet-DS40002211A.pdf
#define __AVR_TRM01__
#elif defined(__AVR_ATmega164A__) || defined(__AVR_ATmega164PA__) || defined(__AVR_ATmega324A__) || defined(__AVR_ATmega324PA__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644PA__) || defined(__AVR_ATmega1284__) || defined(__AVT_ATmega1284P__)
// ATmega1284 technical reference manual date: 29th of November, 2022
// ATmega164A_PA-324A_PA-644A_PA-1284_P_Data-Sheet-40002070B.pdf
#define __AVR_TRM02__
#elif defined(__AVR_ATmega48A__) || defined(__AVR_ATmega48PA__) || defined(__AVR_ATmega88A__) || defined(__AVR_ATmega88PA__) || defined(__AVR_ATmega168A__) || defined(__AVR_ATmega168PA__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)
// ATmega328 technical reference manual date: 29th of November, 2022
// ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061B.pdf
#define __AVR_TRM03__
#elif defined(__AVR_AT90USB1287__) || defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB1286P__) || defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB646P__) || defined(__AVR_AT90USB647__)
// AT90USB1287 technical reference manual ID: 7593D–AVR–07/06
// Preliminary.
#define __AVR_TRM04__
#endif

// As old as the ATmega series of CPU is, the worse the actual libraries making
// use of the MCU likely are.

// These registers as references do not take program space since they are purely references.

struct _bit_reg_t {
  uint8_t val;

  bool getValue(uint8_t idx) {
    return ( val & (1<<idx) );
  }
  void setValue(uint8_t idx, bool value) {
    if (value)
      val |= (1<<idx);
    else
      val &= ~(1<<idx);
  }
};

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)

typedef _bit_reg_t PIN_reg_t;
typedef _bit_reg_t DDR_reg_t;
typedef _bit_reg_t PORT_reg_t;

struct PORT_dev_t {
  PIN_reg_t _PIN;
  DDR_reg_t _DDR;
  PORT_reg_t _PORT;
};
static_assert(sizeof(PORT_dev_t) == 3, "invalid size of ATmega2560 GPIO_dev_t");

#endif

#if defined(__AVR_TRM01__)

struct _bitG_reg_t {
  uint8_t val : 6;
  uint8_t reserved1 : 2;
  
  bool getValue(uint8_t idx) {
    return ( val & (1<<idx) );
  }
  void setValue(uint8_t idx, bool value) {
    if (value)
      val |= (1<<idx);
    else
      val &= ~(1<<idx);
  }
};
typedef _bitG_reg_t PING_reg_t;
typedef _bitG_reg_t DDRG_reg_t;
typedef _bitG_reg_t PORTG_reg_t;

struct PORTG_dev_t {
  PING_reg_t _PIN;
  DDRG_reg_t _DDR;
  PORTG_reg_t _PORT;
};

#endif

#if defined(__AVR_TRM03__)

struct _bitC_reg_t {
  uint8_t val : 7;
  uint8_t reserved1 : 1;
  
  bool getValue(uint8_t idx) {
    return ( val & (1<<idx) );
  }
  void setValue(uint8_t idx, bool value) {
    if (value)
      val |= (1<<idx);
    else
      val &= ~(1<<idx);
  }
};
typedef _bitC_reg_t PINC_reg_t;
typedef _bitC_reg_t DDRC_reg_t;
typedef _bitC_reg_t PORTC_reg_t;

struct PORTC_dev_t {
  PINC_reg_t _PIN;
  DDRC_reg_t _DDR;
  PORTC_reg_t _PORT;
};

#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)

struct TIFR0_reg_t {
  uint8_t _TOV0 : 1;
  uint8_t _OCF0A : 1;
  uint8_t _OCF0B : 1;
  uint8_t reserved1 : 5;
};
static_assert(sizeof(TIFR0_reg_t) == 1, "invalid size of ATmega2560 TIFR0_reg_t");

struct TIFR1_reg_t {
#ifdef __AVR_TRM01__
  uint8_t _TOV1 : 1;
  uint8_t _OCF1A : 1;
  uint8_t _OCF1B : 1;
  uint8_t _OCF1C : 1;
  uint8_t reserved1 : 1;
  uint8_t _ICF1 : 1;
  uint8_t reserved2 : 2;
#elif defined(__AVR_TRM02__) || defined(__AVR_TRM03__)
  uint8_t _TOV1 : 1;
  uint8_t _OCF1A : 1;
  uint8_t _OCF1B : 1;
  uint8_t reserved1 : 2;
  uint8_t _ICF1 : 1;
  uint8_t reserved2 : 2;
#endif
};
static_assert(sizeof(TIFR1_reg_t) == 1, "invalid size of ATmega2560 TIFR1_reg_t");

struct TIFR2_reg_t {
  uint8_t _TOV2 : 1;
  uint8_t _OCF2A : 1;
  uint8_t _OCF2B : 1;
  uint8_t reserved1 : 5;
};
static_assert(sizeof(TIFR2_reg_t) == 1, "invalid size of ATmega2560 TIFR2_reg_t");

#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__)

struct TIFR3_reg_t {
#ifdef __AVR_TRM01__
  uint8_t _TOV3 : 1;
  uint8_t _OCF3A : 1;
  uint8_t _OCF3B : 1;
  uint8_t _OCF3C : 1;
  uint8_t reserved1 : 1;
  uint8_t _ICF3 : 1;
  uint8_t reserved2 : 2;
#elif defined(__AVR_TRM02__)
  uint8_t _TOV3 : 1;
  uint8_t _OCF3A : 1;
  uint8_t _OCF3B : 1;
  uint8_t reserved1 : 2;
  uint8_t _ICF3 : 1;
  uint8_t reserved2 : 2;
#endif
};
static_assert(sizeof(TIFR3_reg_t) == 1, "invalid size of ATmega2560 TIFR3_reg_t");

#endif

#if defined(__AVR_TRM01__)

struct TIFR4_reg_t {
  uint8_t _TOV4 : 1;
  uint8_t _OCF4A : 1;
  uint8_t _OCF4B : 1;
  uint8_t _OCF4C : 1;
  uint8_t reserved1 : 1;
  uint8_t _ICF4 : 1;
  uint8_t reserved2 : 2;
};
static_assert(sizeof(TIFR4_reg_t) == 1, "invalid size of ATmega2560 TIFR4_reg_t");

struct TIFR5_reg_t {
  uint8_t _TOV5 : 1;
  uint8_t _OCF5A : 1;
  uint8_t _OCF5B : 1;
  uint8_t _OCF5C : 1;
  uint8_t reserved1 : 1;
  uint8_t _ICF5 : 1;
  uint8_t reserved2 : 2;
};
static_assert(sizeof(TIFR5_reg_t) == 1, "invalid size of ATmega2560 TIFR5_reg_t");

#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)

struct PCIFR_reg_t {
#if defined(__AVR_TRM01__) || defined(__AVR_TRM03__)
  uint8_t _PCIF0 : 1;
  uint8_t _PCIF1 : 1;
  uint8_t _PCIF2 : 1;
  uint8_t reserved1 : 5;
#elif defined(__AVR_TRM02__)
  uint8_t _PCIF0 : 1;
  uint8_t _PCIF1 : 1;
  uint8_t _PCIF2 : 1;
  uint8_t _PCIF3 : 1;
  uint8_t reserved1 : 4;
#endif
};
static_assert(sizeof(PCIFR_reg_t) == 1, "invalid size of ATmega2560 PCIFR_reg_t");

#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)

struct EIFR_reg_t {
#if defined(__AVR_TRM01__)
  uint8_t _INTF0 : 1;
  uint8_t _INTF1 : 1;
  uint8_t _INTF2 : 1;
  uint8_t _INTF3 : 1;
  uint8_t _INTF4 : 1;
  uint8_t _INTF5 : 1;
  uint8_t _INTF6 : 1;
  uint8_t _INTF7 : 1;
#elif defined(__AVR_TRM02__)
  uint8_t _INTF0 : 1;
  uint8_t _INTF1 : 1;
  uint8_t _INTF2 : 1;
  uint8_t reserved1 : 5;
#elif defined(__AVR_TRM03__)
  uint8_t _INTF0 : 1;
  uint8_t _INTF1 : 1;
  uint8_t reserved1 : 6;
#endif
};
static_assert(sizeof(EIFR_reg_t) == 1, "invalid size of ATmega2560 EIFR_reg_t");

struct EIMSK_reg_t {
#if defined(__AVR_TRM01__)
  uint8_t _INT0 : 1;
  uint8_t _INT1 : 1;
  uint8_t _INT2 : 1;
  uint8_t _INT3 : 1;
  uint8_t _INT4 : 1;
  uint8_t _INT5 : 1;
  uint8_t _INT6 : 1;
  uint8_t _INT7 : 1;
#elif defined(__AVR_TRM02__)
  uint8_t _INT0 : 1;
  uint8_t _INT1 : 1;
  uint8_t _INT2 : 1;
  uint8_t reserved1 : 5;
#elif defined(__AVR_TRM03__)
  uint8_t _INT0 : 1;
  uint8_t _INT1 : 1;
  uint8_t reserved1 : 6;
#endif
};
static_assert(sizeof(EIMSK_reg_t) == 1, "invalid size of ATmega2560 EIMSK_reg_t");

#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)

struct EECR_reg_t {
  uint8_t _EERE : 1;
  uint8_t _EEPE : 1;
  uint8_t _EEMPE : 1;
  uint8_t _EERIE : 1;
  uint8_t _EEPM0 : 1;
  uint8_t _EEPM1 : 1;
  uint8_t reserved1 : 2;
};
static_assert(sizeof(EECR_reg_t) == 1, "invalid size of ATmega2560 EECR_reg_t");

struct EEAR_reg_t {
#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__)
  uint16_t _EEAR : 12;
  uint16_t reserved1 : 4;
#elif defined(__AVR_TRM03__)
#if defined(__AVR_ATmega88A__) || defined(__AVR_ATmega88PA__) || defined(__AVR_ATmega168A__) || defined(__AVR_ATmega168PA__) || defined(__AVR_ATmega328P__)
  uint16_t _EEAR : 16;
#else
  uint8_t _EEAR : 8;
  uint8_t reserved1 : 8;
#endif
#endif
};
static_assert(sizeof(EEAR_reg_t) == 2, "invalid size of ATmega2560 EEAR_reg_t");

struct GTCCR_reg_t {
  uint8_t _PSRSYNC : 1;
  uint8_t _PSRASY : 1;
  uint8_t reserved1 : 5;
  uint8_t _TSM : 1;
};
static_assert(sizeof(GTCCR_reg_t) == 1, "invalid size of ATmega2560 GTCCR_reg_t");

#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)

struct SPCR_reg_t {
  uint8_t _SPR : 2;
  uint8_t _CPHA : 1;
  uint8_t _CPOL : 1;
  uint8_t _MSTR : 1;
  uint8_t _DORD : 1;
  uint8_t _SPE : 1;
  uint8_t _SPIE : 1;
};
static_assert(sizeof(SPCR_reg_t) == 1, "invalid size of ATmega2560 SPCR_reg_t");

struct SPSR_reg_t {
  uint8_t _SPI2X : 1;
  uint8_t reserved1 : 5;
  uint8_t _WCOL : 1;
  uint8_t _SPIF : 1;
};
static_assert(sizeof(SPSR_reg_t) == 1, "invalid size of ATmega2560 SPSR_reg_t");

#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)

struct ACSR_reg_t {
  uint8_t _ACIS : 2;
  uint8_t _ACIC : 1;
  uint8_t _ACIE : 1;
  uint8_t _ACI : 1;
  uint8_t _ACO : 1;
  uint8_t _ACBG : 1;
  uint8_t _ACD : 1;
};
static_assert(sizeof(ACSR_reg_t) == 1, "invalid size of ATmega2560 ACSR_reg_t");

#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)

struct SMCR_reg_t {
  uint8_t _SE : 1;
  uint8_t _SM : 3;
  uint8_t reserved1 : 4;
};
static_assert(sizeof(SMCR_reg_t) == 1, "invalid size of ATmega2560 SMCR_reg_t");

struct MCUSR_reg_t {
#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__)
  uint8_t _PORF : 1;
  uint8_t _EXTRF : 1;
  uint8_t _BORF : 1;
  uint8_t _WDRF : 1;
  uint8_t _JTRF : 1;
  uint8_t reserved1 : 3;
#elif defined(__AVR_TRM03__)
  uint8_t _PORF : 1;
  uint8_t _EXTRF : 1;
  uint8_t _BORF : 1;
  uint8_t _WDRF : 1;
  uint8_t reserved1 : 4;
#endif
};
static_assert(sizeof(MCUSR_reg_t) == 1, "invalid size of ATmega2560 MCUSR_reg_t");

struct MCUCR_reg_t {
#if defined(__AVR_TRM01__)
  uint8_t _IVCE : 1;
  uint8_t _IVSEL : 1;
  uint8_t reserved1 : 2;
  uint8_t _PUD : 1;
  uint8_t reserved2 : 2;
  uint8_t _JTD : 1;
#elif defined(__AVR_TRM02__)
  uint8_t _IVCE : 1;
  uint8_t _IVSEL : 1;
  uint8_t reserved1 : 2;
  uint8_t _PUD : 1;
  uint8_t _BODSE : 1;
  uint8_t _BODS : 1;
  uint8_t _JTD : 1;
#elif defined(__AVR_TRM03__)
  uint8_t _IVCE : 1;
  uint8_t _IVSEL : 1;
  uint8_t reserved1 : 2;
  uint8_t _PUD : 1;
  uint8_t _BODSE : 1;
  uint8_t _BODS : 1;
  uint8_t reserved2 : 1;
#endif
};
static_assert(sizeof(MCUCR_reg_t) == 1, "invalid size of ATmega2560 MCUCR_reg_t");

struct SPMCSR_reg_t {
#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__)
  uint8_t _SPMEN : 1;
  uint8_t _PGERS : 1;
  uint8_t _PGWRT : 1;
  uint8_t _BLBSET : 1;
  uint8_t _RWWSRE : 1;
  uint8_t _SIGRD : 1;
  uint8_t _RWWSB : 1;
  uint8_t _SPMIE : 1;
#elif defined(__AVR_TRM03__)
#if defined(__AVR_ATmega88A__) || defined(__AVR_ATmega88PA__) || defined(__AVR_ATmega168A__) || defined(__AVR_ATmega168PA__) || defined(__AVR_ATmega328P__)
  uint8_t _SPMEN : 1;
  uint8_t _PGERS : 1;
  uint8_t _PGWRT : 1;
  uint8_t _BLBSET : 1;
  uint8_t _RWWSRE : 1;
  uint8_t _SIGRD : 1;
  uint8_t _RWWSB : 1;
  uint8_t _SPMIE : 1;
#else
  uint8_t _SPMEN : 1;
  uint8_t _PGERS : 1;
  uint8_t _PGWRT : 1;
  uint8_t _BLBSET : 1;
  uint8_t reserved1 : 1;
  uint8_t _SIGRD : 1;
  uint8_t reserved2 : 1;
  uint8_t _SPMIE : 1;
#endif
#endif
};
static_assert(sizeof(SPMCSR_reg_t) == 1, "invalid size of ATmega2560 SPMCSR_reg_t");

#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__)

struct RAMPZ_reg_t {
  uint8_t _RAMPZ : 2;
  uint8_t reserved1 : 6;
};
static_assert(sizeof(RAMPZ_reg_t) == 1, "invalid size of ATmega2560 RAMPZ_reg_t");

#endif

#if defined(__AVR_TRM01__)

struct EIND_reg_t {
  uint8_t _EIND0 : 1;
  uint8_t reserved1 : 7;
};
static_assert(sizeof(EIND_reg_t) == 1, "invalid size of ATmega2560 EIND_reg_t");

#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)

struct SP_reg_t {
#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__)
  uint16_t _SP;
#elif defined(__AVR_TRM03__)
#if defined(__AVR_ATmega88A__) || defined(__AVR_ATmega88PA__) || defined(__AVR_ATmega168A__) || defined(__AVR_ATmega168PA__) || defined(__AVR_ATmega328P__)
  uint16_t _SP : 11;
  uint16_t reserved1 : 5;
#else
  uint16_t _SP : 10;
  uint16_t reserved1 : 6;
#endif
#endif
};
static_assert(sizeof(SP_reg_t) == 2, "invalid size of ATmega2560 SP_reg_t");

#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)

struct SREG_reg_t {
  uint8_t _C : 1;
  uint8_t _Z : 1;
  uint8_t _N : 1;
  uint8_t _V : 1;
  uint8_t _S : 1;
  uint8_t _H : 1;
  uint8_t _T : 1;
  uint8_t _I : 1;
};
static_assert(sizeof(SREG_reg_t) == 1, "invalid size of ATmega2560 SREG_reg_t");

struct WDTCSR_reg_t {
  uint8_t _WDP0 : 1;
  uint8_t _WDP1 : 1;
  uint8_t _WDP2 : 1;
  uint8_t _WDE : 1;
  uint8_t _WDCE : 1;
  uint8_t _WDP3 : 1;
  uint8_t _WDIE : 1;
  uint8_t _WDIF : 1;
};
static_assert(sizeof(WDTCSR_reg_t) == 1, "invalid size of ATmega2560 WDTCSR_reg_t");

struct CLKPR_reg_t {
  uint8_t _CLKPS : 4;
  uint8_t reserved1 : 3;
  uint8_t _CLKPCE : 1;
};
static_assert(sizeof(CLKPR_reg_t) == 1, "invalid size of ATmega2560 CLKPR_reg_t");

#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)

struct PRR0_reg_t {
#if defined(__AVR_TRM01__)
  uint8_t _PRADC : 1;
  uint8_t _PRUSART0 : 1;
  uint8_t _PRSPI : 1;
  uint8_t _PRTIM1 : 1;
  uint8_t reserved1 : 1;
  uint8_t _PRTIM0 : 1;
  uint8_t _PRTIM2 : 1;
  uint8_t _PRTWI : 1;
#elif defined(__AVR_TRM02__)
  uint8_t _PRADC : 1;
  uint8_t _PRUSART0 : 1;
  uint8_t _PRSPI : 1;
  uint8_t _PRTIM1 : 1;
  uint8_t _PRUSART1 : 1;
  uint8_t _PRTIM0 : 1;
  uint8_t _PRTIM2 : 1;
  uint8_t _PRTWI : 1;
#elif defined(__AVR_TRM03__)
  uint8_t _PRADC : 1;
  uint8_t _PRUSART0 : 1;
  uint8_t _PRSPI : 1;
  uint8_t _PRTIM1 : 1;
  uint8_t reserved1 : 1;
  uint8_t _PRTIM0 : 1;
  uint8_t _PRTIM2 : 1;
  uint8_t _PRTWI : 1;
#endif
};
static_assert(sizeof(PRR0_reg_t) == 1, "invalid size of ATmega2560 PRR0_reg_t");

#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__)

struct PRR1_reg_t {
#if defined(__AVR_TRM01__)
  uint8_t _PRUSART1 : 1;
  uint8_t _PRUSART2 : 1;
  uint8_t _PRUSART3 : 1;
  uint8_t _PRTIM3 : 1;
  uint8_t _PRTIM4 : 1;
  uint8_t _PRTIM5 : 1;
  uint8_t reserved1 : 2;
#elif defined(__AVR_TRM02__)
  uint8_t _PRTIM3 : 1;
  uint8_t reserved1 : 7;
#endif
};
static_assert(sizeof(PRR1_reg_t) == 1, "invalid size of ATmega2560 PRR1_reg_t");

#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)

struct PCICR_reg_t {
#if defined(__AVR_TRM01__) || defined(__AVR_TRM03__)
  uint8_t _PCIE0 : 1;
  uint8_t _PCIE1 : 1;
  uint8_t _PCIE2 : 1;
  uint8_t reserved1 : 5;
#elif defined(__AVR_TRM02__)
  uint8_t _PCIE0 : 1;
  uint8_t _PCIE1 : 1;
  uint8_t _PCIE2 : 1;
  uint8_t _PCIE3 : 1;
  uint8_t reserved1 : 4;
#endif
};
static_assert(sizeof(PCICR_reg_t) == 1, "invalid size of ATmega2560 PCICR_reg_t");

struct EICRA_reg_t {
#if defined(__AVR_TRM01__)
  uint8_t _ISC0 : 2;
  uint8_t _ISC1 : 2;
  uint8_t _ISC2 : 2;
  uint8_t _ISC3 : 2;
#elif defined(__AVR_TRM02__)
  uint8_t _ISC0 : 2;
  uint8_t _ISC1 : 2;
  uint8_t _ISC2 : 2;
  uint8_t reserved1 : 2;
#elif defined(__AVR_TRM03__)
  uint8_t _ISC0 : 2;
  uint8_t _ISC1 : 2;
  uint8_t reserved1 : 4;
#endif
};
static_assert(sizeof(EICRA_reg_t) == 1, "invalid size of ATmega2560 EICRA_reg_t");

#endif

#if defined(__AVR_TRM01__)

struct EICRB_reg_t {
  uint8_t _ISC4 : 2;
  uint8_t _ISC5 : 2;
  uint8_t _ISC6 : 2;
  uint8_t _ISC7 : 2;
};
static_assert(sizeof(EICRB_reg_t) == 1, "invalid size of ATmega2560 EICRB_reg_t");

#endif

#if defined(__AVR_TRM03__)

struct _bitPCMSK1_reg_t {
  uint8_t val : 7;
  uint8_t reserved1 : 1;
  
  bool getValue(uint8_t idx) {
    return ( val & (1<<idx) );
  }
  void setValue(uint8_t idx, bool value) {
    if (value)
      val |= (1<<idx);
    else
      val &= ~(1<<idx);
  }
};

#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)

struct TIMSK0_reg_t {
#if defined(__AVR_TRM01__)
  uint8_t _TOIE0 : 1;
  uint8_t _OCIE0A : 1;
  uint8_t _OCIE0B : 1;
  uint8_t _OCIE0C : 1;
  uint8_t reserved1 : 1;
  uint8_t _ICIE0 : 1;
  uint8_t reserved2 : 2;
#elif defined(__AVR_TRM02__) || defined(__AVR_TRM03__)
  uint8_t _TOIE0 : 1;
  uint8_t _OCIE0A : 1;
  uint8_t _OCIE0B : 1;
  uint8_t reserved1 : 5;
#endif
};
static_assert(sizeof(TIMSK0_reg_t) == 1, "invalid size of ATmega2560 TIMSK0_reg_t");

struct TIMSK1_reg_t {
#if defined(__AVR_TRM01__)
  uint8_t _TOIE1 : 1;
  uint8_t _OCIE1A : 1;
  uint8_t _OCIE1B : 1;
  uint8_t _OCIE1C : 1;
  uint8_t reserved1 : 1;
  uint8_t _ICIE1: 1;
  uint8_t reserved2 : 2;
#elif defined(__AVR_TRM02__) || defined(__AVR_TRM03__)
  uint8_t _TOIE1 : 1;
  uint8_t _OCIE1A : 1;
  uint8_t _OCIE1B : 1;
  uint8_t reserved1 : 2;
  uint8_t _ICIE1: 1;
  uint8_t reserved2 : 2;
#endif
};
static_assert(sizeof(TIMSK1_reg_t) == 1, "invalid size of ATmega2560 TIMSK1_reg_t");

struct TIMSK2_reg_t {
  uint8_t _TOIE2 : 1;
  uint8_t _OCIE2A : 1;
  uint8_t _OCIE2B : 1;
  uint8_t reserved1 : 5;
};
static_assert(sizeof(TIMSK2_reg_t) == 1, "invalid size of ATmega2560 TIMSK2_reg_t");

#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__)

struct TIMSK3_reg_t {
#if defined(__AVR_TRM01__)
  uint8_t _TOIE3 : 1;
  uint8_t _OCIE3A : 1;
  uint8_t _OCIE3B : 1;
  uint8_t _OCIE3C : 1;
  uint8_t reserved1 : 1;
  uint8_t _ICIE3 : 1;
  uint8_t reserved2 : 2;
#elif defined(__AVR_TRM02__)
  uint8_t _TOIE3 : 1;
  uint8_t _OCIE3A : 1;
  uint8_t _OCIE3B : 1;
  uint8_t reserved1 : 2;
  uint8_t _ICIE3 : 1;
  uint8_t reserved2 : 2;
#endif
};
static_assert(sizeof(TIMSK3_reg_t) == 1, "invalid size of ATmega2560 TIMSK3_reg_t");

#endif

#if defined(__AVR_TRM01__)

struct TIMSK4_reg_t {
  uint8_t _TOIE4 : 1;
  uint8_t _OCIE4A : 1;
  uint8_t _OCIE4B : 1;
  uint8_t _OCIE4C : 1;
  uint8_t reserved1 : 1;
  uint8_t _ICIE4 : 1;
  uint8_t reserved2 : 2;
};
static_assert(sizeof(TIMSK4_reg_t) == 1, "invalid size of ATmega2560 TIMSK4_reg_t");

struct TIMSK5_reg_t {
  uint8_t _TOIE5 : 1;
  uint8_t _OCIE5A : 1;
  uint8_t _OCIE5B : 1;
  uint8_t _OCIE5C : 1;
  uint8_t reserved1 : 1;
  uint8_t _ICIE5 : 1;
  uint8_t reserved2 : 2;
};
static_assert(sizeof(TIMSK5_reg_t) == 1, "invalid size of ATmega2560 TIMSK5_reg_t");

#endif

#if defined(__AVR_TRM01__)

struct XMCRA_reg_t {
  uint8_t _SRW0 : 2;
  uint8_t _SRW1 : 2;
  uint8_t _SRL : 3;
  uint8_t _SRE : 1;
};
static_assert(sizeof(XMCRA_reg_t) == 1, "invalid size of ATmega2560 XMCRA_reg_t");

struct XMCRB_reg_t {
  uint8_t _XMM : 3;
  uint8_t reserved1 : 4;
  uint8_t _XMBK : 1;
};
static_assert(sizeof(XMCRB_reg_t) == 1, "invalid size of ATmega2560 XMCRB_reg_t");

#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)

struct ADCSRA_reg_t {
  uint8_t _ADPS : 3;
  uint8_t _ADIE : 1;
  uint8_t _ADIF : 1;
  uint8_t _ADATE : 1;
  uint8_t _ADSC : 1;
  uint8_t _ADEN : 1;
};
static_assert(sizeof(ADCSRA_reg_t) == 1, "invalid size of ATmega2560 ADCSRA_reg_t");

struct ADCSRB_reg_t {
#if defined(__AVR_TRM01__)
  uint8_t _ADTS : 3;
  uint8_t _MUX5 : 1;
  uint8_t reserved1 : 2;
  uint8_t _ACME : 1;
  uint8_t reserved2 : 1;
#elif defined(__AVR_TRM02__) || defined(__AVR_TRM03__)
  uint8_t _ADTS : 3;
  uint8_t reserved1 : 3;
  uint8_t _ACME : 1;
  uint8_t reserved2 : 1;
#endif  
};
static_assert(sizeof(ADCSRB_reg_t) == 1, "invalid size of ATmega2560 ADCSRB_reg_t");

struct ADMUX_reg_t {
#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__)
  uint8_t _MUX0 : 1;
  uint8_t _MUX1 : 1;
  uint8_t _MUX2 : 1;
  uint8_t _MUX3 : 1;
  uint8_t _MUX4 : 1;
  uint8_t _ADLAR : 1;
  uint8_t _REFS0 : 1;
  uint8_t _REFS1 : 1;
#elif defined(__AVR_TRM03__)
  uint8_t _MUX0 : 1;
  uint8_t _MUX1 : 1;
  uint8_t _MUX2 : 1;
  uint8_t _MUX3 : 1;
  uint8_t reserved1 : 1;
  uint8_t _ADLAR : 1;
  uint8_t _REFS0 : 1;
  uint8_t _REFS1 : 1;
#endif
};
static_assert(sizeof(ADMUX_reg_t) == 1, "invalid size of ATmega2560 ADMUX_reg_t");

#endif

#if defined(__AVR_TRM01__)

struct DIDR2_reg_t {
  uint8_t _ADC8D : 1;
  uint8_t _ADC9D : 1;
  uint8_t _ADC10D : 1;
  uint8_t _ADC11D : 1;
  uint8_t _ADC12D : 1;
  uint8_t _ADC13D : 1;
  uint8_t _ADC14D : 1;
  uint8_t _ADC15D : 1;
};
static_assert(sizeof(DIDR2_reg_t) == 1, "invalid size of ATmega2560 DIDR2_reg_t");

#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)

struct DIDR0_reg_t {
#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__)
  uint8_t _ADC0D : 1;
  uint8_t _ADC1D : 1;
  uint8_t _ADC2D : 1;
  uint8_t _ADC3D : 1;
  uint8_t _ADC4D : 1;
  uint8_t _ADC5D : 1;
  uint8_t _ADC6D : 1;
  uint8_t _ADC7D : 1;
#elif defined(__AVR_TRM03__)
  uint8_t _ADC0D : 1;
  uint8_t _ADC1D : 1;
  uint8_t _ADC2D : 1;
  uint8_t _ADC3D : 1;
  uint8_t _ADC4D : 1;
  uint8_t _ADC5D : 1;
  uint8_t reserved1 : 2;
#endif
};
static_assert(sizeof(DIDR0_reg_t) == 1, "invalid size of ATmega2560 DIDR0_reg_t");

struct DIDR1_reg_t {
  uint8_t _AIN0D : 1;
  uint8_t _AIN1D : 1;
  uint8_t reserved1 : 6;
};
static_assert(sizeof(DIDR1_reg_t) == 1, "invalid size of ATmega2560 DIDR1_reg_t");

#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)

struct TCCRnA_reg_t {
#if defined(__AVR_TRM01__)
  uint8_t _WGMn0 : 1;
  uint8_t _WGMn1 : 1;
  uint8_t _COMnC : 2;
  uint8_t _COMnB : 2;
  uint8_t _COMnA : 2;
#elif defined(__AVR_TRM02__) || defined(__AVR_TRM03__)
  uint8_t _WGMn0 : 1;
  uint8_t _WGMn1 : 1;
  uint8_t reserved1 : 2;
  uint8_t _COMnB : 2;
  uint8_t _COMnA : 2;
#endif
};
static_assert(sizeof(TCCRnA_reg_t) == 1, "invalid size of ATmega2560 TCCRnA_reg_t");

struct TCCRnB_reg_t {
  uint8_t _CSn : 3;
  uint8_t _WGMn2 : 1;
  uint8_t _WGMn3 : 1;
  uint8_t reserved1 : 1;
  uint8_t _ICESn : 1;
  uint8_t _ICNCn : 1;
};
static_assert(sizeof(TCCRnB_reg_t) == 1, "invalid size of ATmega2560 TCCRnB_reg_t");

struct TCCRnC_reg_t {
#if defined(__AVR_TRM01__)
  uint8_t reserved1 : 5;
  uint8_t _FOCnC : 1;
  uint8_t _FOCnB : 1;
  uint8_t _FOCnA : 1;
#elif defined(__AVR_TRM02__) || defined(__AVR_TRM03__)
  uint8_t reserved1 : 6;
  uint8_t _FOCnB : 1;
  uint8_t _FOCnA : 1;
#endif
};
static_assert(sizeof(TCCRnC_reg_t) == 1, "invalid size of ATmega2560 TCCRnC_reg_t");

struct TIMER_dev_t {
  TCCRnA_reg_t _TCCRnA;
  TCCRnB_reg_t _TCCRnB;
  TCCRnC_reg_t _TCCRnC;
  uint8_t reserved1;
  uint16_t _TCNTn;
  uint16_t _ICRn;
  uint16_t _OCRnA;
  uint16_t _OCRnB;
#if defined(__AVR_TRM01__)
  uint16_t _OCRnC;
#endif
};
#if defined(__AVR_TRM01__)
static_assert(sizeof(TIMER_dev_t) == 14, "invalid size of ATmega2560 TIMER_dev_t");
#elif defined(__AVR_TRM02__) || defined(__AVR_TRM03__)
static_assert(sizeof(TIMER_dev_t) == 12, "invalid size of ATmega1284 TIMER_dev_t");
#endif

#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)

struct TCCRnA_8bit_reg_t {
  uint8_t _WGMn0 : 1;
  uint8_t _WGMn1 : 1;
  uint8_t reserved1 : 2;
  uint8_t _COMnB : 2;
  uint8_t _COMnA : 2;
};
static_assert(sizeof(TCCRnA_8bit_reg_t) == 1, "invalid size of ATmega2560 TCCRnA_8bit_reg_t");

struct TCCRnB_8bit_reg_t {
  uint8_t _CSn : 3;
  uint8_t _WGMn2 : 1;
  uint8_t reserved1 : 2;
  uint8_t _FOCnB : 1;
  uint8_t _FOCnA : 1;
};
static_assert(sizeof(TCCRnB_8bit_reg_t) == 1, "invalid size of ATmega2560 TCCRnB_8bit_reg_t");

struct TIMER_8bit_dev_t {
  TCCRnA_8bit_reg_t _TCCRnA;
  TCCRnB_8bit_reg_t _TCCRnB;
  uint8_t _TCNTn;
  uint8_t _OCRnA;
  uint8_t _OCRnB;
};
static_assert(sizeof(TIMER_8bit_dev_t) == 5, "invalid size of ATmega2560 TIMER_8bit_dev_t");

#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)

struct ASSR_reg_t {
  uint8_t _TCR2BUB : 1;
  uint8_t _TCR2AUB : 1;
  uint8_t _OCR2BUB : 1;
  uint8_t _OCR2AUB : 1;
  uint8_t _TCN2UB : 1;
  uint8_t _AS2 : 1;
  uint8_t _EXCLK : 1;
  uint8_t reserved1 : 1;
};
static_assert(sizeof(ASSR_reg_t) == 1, "invalid size of ATmega2560 ASSR_reg_t");

#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)

struct TWSR_reg_t {
  uint8_t _TWPS0 : 1;
  uint8_t _TWPS1 : 1;
  uint8_t reserved1 : 1;
  uint8_t _TWS3 : 1;
  uint8_t _TWS4 : 1;
  uint8_t _TWS5 : 1;
  uint8_t _TWS6 : 1;
  uint8_t _TWS7 : 1;
};
static_assert(sizeof(TWSR_reg_t) == 1, "invalid size of ATmega2560 TWSR_reg_t");

struct TWAR_reg_t {
  uint8_t _TWGCE : 1;
  uint8_t _TWA : 7;
};
static_assert(sizeof(TWAR_reg_t) == 1, "invalid size of ATmega2560 TWAR_reg_t");

struct TWCR_reg_t {
  uint8_t _TWIE : 1;
  uint8_t reserved1 : 1;
  uint8_t _TWEN : 1;
  uint8_t _TWWC : 1;
  uint8_t _TWSTO : 1;
  uint8_t _TWSTA : 1;
  uint8_t _TWEA : 1;
  uint8_t _TWINT : 1;
};
static_assert(sizeof(TWCR_reg_t) == 1, "invalid size of ATmega2560 TWCR_reg_t");

struct TWAMR_reg_t {
  uint8_t reserved1 : 1;
  uint8_t _TWAM : 7;
};
static_assert(sizeof(TWAMR_reg_t) == 1, "invalid size of ATmega2560 TWAMR_reg_t");

#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)

struct UBRRn_reg_t {
  uint16_t _UBRR : 12;
  uint16_t reserved1 : 4;
}; 
static_assert(sizeof(UBRRn_reg_t) == 2, "invalid size of ATmega2560 UBRRn_reg_t)");

struct UCSRnC_reg_t {
  uint8_t _UCPOL : 1;
  uint8_t _UCSZn0 : 1;
  uint8_t _UCSZn1 : 1;
  uint8_t _USBS : 1;
  uint8_t _UPM : 2;
  uint8_t _UMSEL : 2; 
};
static_assert(sizeof(UCSRnC_reg_t) == 1, "invalid size of ATmega2560 UCSRnC_reg_t");

struct UCSRnB_reg_t {
  uint8_t _TXB8 : 1;
  uint8_t _RXB8 : 1;
  uint8_t _UCSZn2 : 1;
  uint8_t _TXEN : 1;
  uint8_t _RXEN : 1;
  uint8_t _UDRIE : 1;
  uint8_t _TXCIE : 1;
  uint8_t _RXCIE : 1;
};
static_assert(sizeof(UCSRnB_reg_t) == 1, "invalid size of ATmega2560 UCSRnB_reg_t");

struct UCSRnA_reg_t {
  uint8_t _MPCM : 1;
  uint8_t _U2X : 1;
  uint8_t _UPE : 1;
  uint8_t _DOR : 1;
  uint8_t _FE : 1;
  uint8_t _UDRE : 1;
  uint8_t _TXC : 1;
  uint8_t _RXC : 1;
};
static_assert(sizeof(UCSRnA_reg_t) == 1, "invalid size of ATmega2560 UCSRnA_reg_t");

struct USART_dev_t {
  UCSRnA_reg_t _UCSRnA;
  UCSRnB_reg_t _UCSRnB;
  UCSRnC_reg_t _UCSRnC;
  uint8_t reserved1;
  UBRRn_reg_t _UBRRn;
  uint8_t _UDRn;
};
static_assert(sizeof(USART_dev_t) == 7, "invalid size of ATmega2560 USART_dev_t");

#endif

#if defined(__AVR_TRM01__)
// page 399ff of ATmega640-1280-1281-2560-2561-Datasheet-DS40002211A.pdf
static PORT_dev_t& _PORTA = *(PORT_dev_t*)0x20;
static PORT_dev_t& _PORTB = *(PORT_dev_t*)0x23;
static PORT_dev_t& _PORTC = *(PORT_dev_t*)0x26;
static PORT_dev_t& _PORTD = *(PORT_dev_t*)0x29;
static PORT_dev_t& _PORTE = *(PORT_dev_t*)0x2C;
static PORT_dev_t& _PORTF = *(PORT_dev_t*)0x2F;
static PORTG_dev_t& _PORTG = *(PORTG_dev_t*)0x32;
static PORT_dev_t& _PORTH = *(PORT_dev_t*)0x100;
static PORT_dev_t& _PORTJ = *(PORT_dev_t*)0x103;
static PORT_dev_t& _PORTK = *(PORT_dev_t*)0x106;
static PORT_dev_t& _PORTL = *(PORT_dev_t*)0x109;
static TIFR0_reg_t& _TIFR0 = *(TIFR0_reg_t*)0x35;
static TIFR1_reg_t& _TIFR1 = *(TIFR1_reg_t*)0x36;
static TIFR2_reg_t& _TIFR2 = *(TIFR2_reg_t*)0x37;
static TIFR3_reg_t& _TIFR3 = *(TIFR3_reg_t*)0x38;
static TIFR4_reg_t& _TIFR4 = *(TIFR4_reg_t*)0x39;
static TIFR5_reg_t& _TIFR5 = *(TIFR5_reg_t*)0x3A;
static PCIFR_reg_t& _PCIFR = *(PCIFR_reg_t*)0x3B;
static EIFR_reg_t& _EIFR = *(EIFR_reg_t*)0x3C;
static EIMSK_reg_t& _EIMSK = *(EIMSK_reg_t*)0x3D;
static _bit_reg_t& _GPIOR0 = *(_bit_reg_t*)0x3E;
static EECR_reg_t& _EECR = *(EECR_reg_t*)0x3F;
static uint8_t& _EEDR = *(uint8_t*)0x40;
static EEAR_reg_t& _EEAR = *(EEAR_reg_t*)0x41;
static GTCCR_reg_t& _GTCCR = *(GTCCR_reg_t*)0x43;
static TIMER_8bit_dev_t& TIMER0 = *(TIMER_8bit_dev_t*)0x44;
static _bit_reg_t& _GPIOR1 = *(_bit_reg_t*)0x4A;
static _bit_reg_t& _GPIOR2 = *(_bit_reg_t*)0x4B;
static SPCR_reg_t& _SPCR = *(SPCR_reg_t*)0x4C;
static SPSR_reg_t& _SPSR = *(SPSR_reg_t*)0x4D;
static uint8_t& _SPDR = *(uint8_t*)0x4E;
static ACSR_reg_t& _ACSR = *(ACSR_reg_t*)0x50;
static _bit_reg_t& _OCDR = *(_bit_reg_t*)0x51;
static SMCR_reg_t& _SMCR = *(SMCR_reg_t*)0x53;
static MCUSR_reg_t& _MCUSR = *(MCUSR_reg_t*)0x54;
static MCUCR_reg_t& _MCUCR = *(MCUCR_reg_t*)0x55;
static SPMCSR_reg_t& _SPMCSR = *(SPMCSR_reg_t*)0x57;
static RAMPZ_reg_t& _RAMPZ = *(RAMPZ_reg_t*)0x5B;
static EIND_reg_t& _EIND = *(EIND_reg_t*)0x5C;
static SP_reg_t& _SP = *(SP_reg_t*)0x5D;
static SREG_reg_t& _SREG = *(SREG_reg_t*)0x5F;
static WDTCSR_reg_t& _WDTCSR = *(WDTCSR_reg_t*)0x60;
static CLKPR_reg_t& _CLKPR = *(CLKPR_reg_t*)0x61;
static PRR0_reg_t& _PRR0 = *(PRR0_reg_t*)0x64;
static PRR1_reg_t& _PRR1 = *(PRR1_reg_t*)0x65;
static uint8_t& _OSCCAL = *(uint8_t*)0x66;
static PCICR_reg_t& _PCICR = *(PCICR_reg_t*)0x68;
static EICRA_reg_t& _EICRA = *(EICRA_reg_t*)0x69;
static EICRB_reg_t& _EICRB = *(EICRB_reg_t*)0x6A;
static _bit_reg_t& _PCMSK0 = *(_bit_reg_t*)0x6B;
static _bit_reg_t& _PCMSK1 = *(_bit_reg_t*)0x6C;
static _bit_reg_t& _PCMSK2 = *(_bit_reg_t*)0x6D;
static TIMSK0_reg_t& _TIMSK0 = *(TIMSK0_reg_t*)0x6E;
static TIMSK1_reg_t& _TIMSK1 = *(TIMSK1_reg_t*)0x6F;
static TIMSK2_reg_t& _TIMSK2 = *(TIMSK2_reg_t*)0x70;
static TIMSK3_reg_t& _TIMSK3 = *(TIMSK3_reg_t*)0x71;
static TIMSK4_reg_t& _TIMSK4 = *(TIMSK4_reg_t*)0x72;
static TIMSK5_reg_t& _TIMSK5 = *(TIMSK5_reg_t*)0x73;
static XMCRA_reg_t& _XMCRA = *(XMCRA_reg_t*)0x74;
static XMCRB_reg_t& _XMCRB = *(XMCRB_reg_t*)0x75;
static uint16_t& _ADC = *(uint16_t*)0x78;
static ADCSRA_reg_t& _ADCSRA = *(ADCSRA_reg_t*)0x7A;
static ADCSRB_reg_t& _ADCSRB = *(ADCSRB_reg_t*)0x7B;
static ADMUX_reg_t& _ADMUX = *(ADMUX_reg_t*)0x7C;
static DIDR2_reg_t& _DIDR2 = *(DIDR2_reg_t*)0x7D;
static DIDR0_reg_t& _DIDR0 = *(DIDR0_reg_t*)0x7E;
static DIDR1_reg_t& _DIDR1 = *(DIDR1_reg_t*)0x7F;
static TIMER_dev_t& TIMER1 = *(TIMER_dev_t*)0x80;
static TIMER_dev_t& TIMER3 = *(TIMER_dev_t*)0x90;
static TIMER_dev_t& TIMER4 = *(TIMER_dev_t*)0xA0;
static TIMER_dev_t& TIMER5 = *(TIMER_dev_t*)0x120;
static TIMER_8bit_dev_t& _TIMER2 = *(TIMER_8bit_dev_t*)0xB0;
static ASSR_reg_t& _ASSR = *(ASSR_reg_t*)0xB6;
static uint8_t& _TWBR = *(uint8_t*)0xB8;
static TWSR_reg_t& _TWSR = *(TWSR_reg_t*)0xB9;
static TWAR_reg_t& _TWAR = *(TWAR_reg_t*)0xBA;
static uint8_t& _TWDR = *(uint8_t*)0xBB;
static TWCR_reg_t& _TWCR = *(TWCR_reg_t*)0xBC;
static TWAMR_reg_t& _TWAMR = *(TWAMR_reg_t*)0xBD;
static USART_dev_t& USART0 = *(USART_dev_t*)0xC0;
static USART_dev_t& USART1 = *(USART_dev_t*)0xC8;
static USART_dev_t& USART2 = *(USART_dev_t*)0xD0;
static USART_dev_t& USART3 = *(USART_dev_t*)0x130;

#elif defined(__AVR_TRM02__)
// page 637ff of ATmega164A_PA-324A_PA-644A_PA-1284_P_Data-Sheet-40002070B.pdf
static PORT_dev_t& _PORTA = *(PORT_dev_t*)0x20;
static PORT_dev_t& _PORTB = *(PORT_dev_t*)0x23;
static PORT_dev_t& _PORTC = *(PORT_dev_t*)0x26;
static PORT_dev_t& _PORTD = *(PORT_dev_t*)0x29;
static TIFR0_reg_t& _TIFR0 = *(TIFR0_reg_t*)0x35;
static TIFR1_reg_t& _TIFR1 = *(TIFR1_reg_t*)0x36;
static TIFR2_reg_t& _TIFR2 = *(TIFR2_reg_t*)0x37;
static TIFR3_reg_t& _TIFR3 = *(TIFR3_reg_t*)0x38;
static PCIFR_reg_t& _PCIFR = *(PCIFR_reg_t*)0x3B;
static EIFR_reg_t& _EIFR = *(EIFR_reg_t*)0x3C;
static EIMSK_reg_t& _EIMSK = *(EIMSK_reg_t*)0x3D;
static _bit_reg_t& _GPIOR0 = *(_bit_reg_t*)0x3E;
static EECR_reg_t& _EECR = *(EECR_reg_t*)0x3F;
static uint8_t& _EEDR = *(uint8_t*)0x40;
static EEAR_reg_t& _EEAR = *(EEAR_reg_t*)0x41;
static GTCCR_reg_t& _GTCCR = *(GTCCR_reg_t*)0x43;
static TIMER_8bit_dev_t& TIMER0 = *(TIMER_8bit_dev_t*)0x44;
static _bit_reg_t& _GPIOR1 = *(_bit_reg_t*)0x4A;
static _bit_reg_t& _GPIOR2 = *(_bit_reg_t*)0x4B;
static SPCR_reg_t& _SPCR = *(SPCR_reg_t*)0x4C;
static SPSR_reg_t& _SPSR = *(SPSR_reg_t*)0x4D;
static uint8_t& _SPDR = *(uint8_t*)0x4E;
static ACSR_reg_t& _ACSR = *(ACSR_reg_t*)0x50;
static SMCR_reg_t& _SMCR = *(SMCR_reg_t*)0x53;
static MCUSR_reg_t& _MCUSR = *(MCUSR_reg_t*)0x54;
static MCUCR_reg_t& _MCUCR = *(MCUCR_reg_t*)0x55;
static SPMCSR_reg_t& _SPMCSR = *(SPMCSR_reg_t*)0x57;
static SP_reg_t& _SP = *(SP_reg_t*)0x5D;
static SREG_reg_t& _SREG = *(SREG_reg_t*)0x5F;
static WDTCSR_reg_t& _WDTCSR = *(WDTCSR_reg_t*)0x60;
static CLKPR_reg_t& _CLKPR = *(CLKPR_reg_t*)0x61;
static PRR0_reg_t& _PRR0 = *(PRR0_reg_t*)0x64;
static PRR1_reg_t& _PRR1 = *(PRR1_reg_t*)0x65;
static uint8_t& _OSCCAL = *(uint8_t*)0x66;
static PCICR_reg_t& _PCICR = *(PCICR_reg_t*)0x68;
static EICRA_reg_t& _EICRA = *(EICRA_reg_t*)0x69;
static _bit_reg_t& _PCMSK0 = *(_bit_reg_t*)0x6B;
static _bit_reg_t& _PCMSK1 = *(_bit_reg_t*)0x6C;
static _bit_reg_t& _PCMSK2 = *(_bit_reg_t*)0x6D;
static TIMSK0_reg_t& _TIMSK0 = *(TIMSK0_reg_t*)0x6E;
static TIMSK1_reg_t& _TIMSK1 = *(TIMSK1_reg_t*)0x6F;
static TIMSK2_reg_t& _TIMSK2 = *(TIMSK2_reg_t*)0x70;
static TIMSK3_reg_t& _TIMSK3 = *(TIMSK3_reg_t*)0x71;
static _bit_reg_t& _PCMSK3 = *(_bit_reg_t*)0x73;
static uint16_t& _ADC = *(uint16_t*)0x78;
static ADCSRA_reg_t& _ADCSRA = *(ADCSRA_reg_t*)0x7A;
static ADCSRB_reg_t& _ADCSRB = *(ADCSRB_reg_t*)0x7B;
static ADMUX_reg_t& _ADMUX = *(ADMUX_reg_t*)0x7C;
static DIDR0_reg_t& _DIDR0 = *(DIDR0_reg_t*)0x7E;
static DIDR1_reg_t& _DIDR1 = *(DIDR1_reg_t*)0x7F;
static TIMER_dev_t& TIMER1 = *(TIMER_dev_t*)0x70;
static TIMER_dev_t& TIMER3 = *(TIMER_dev_t*)0x90;
static TIMER_8bit_dev_t& _TIMER2 = *(TIMER_8bit_dev_t*)0xB0;
static ASSR_reg_t& _ASSR = *(ASSR_reg_t*)0xB6;
static uint8_t& _TWBR = *(uint8_t*)0xB8;
static TWSR_reg_t& _TWSR = *(TWSR_reg_t*)0xB9;
static TWAR_reg_t& _TWAR = *(TWAR_reg_t*)0xBA;
static uint8_t& _TWDR = *(uint8_t*)0xBB;
static TWCR_reg_t& _TWCR = *(TWCR_reg_t*)0xBC;
static TWAMR_reg_t& _TWAMR = *(TWAMR_reg_t*)0xBD;
static USART_dev_t& USART0 = *(USART_dev_t*)0xC0;
static USART_dev_t& USART1 = *(USART_dev_t*)0xC8;

#elif defined(__AVR_TRM03__)
// page 621ff of ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061B.pdf
static PORT_dev_t& _PORTB = *(PORT_dev_t*)0x23;
static PORTC_dev_t& _PORTC = *(PORTC_dev_t*)0x26;
static PORT_dev_t& _PORTD = *(PORT_dev_t*)0x29;
static TIFR0_reg_t& _TIFR0 = *(TIFR0_reg_t*)0x35;
static TIFR1_reg_t& _TIFR1 = *(TIFR1_reg_t*)0x36;
static TIFR2_reg_t& _TIFR2 = *(TIFR2_reg_t*)0x37;
static PCIFR_reg_t& _PCIFR = *(PCIFR_reg_t*)0x3B;
static EIFR_reg_t& _EIFR = *(EIFR_reg_t*)0x3C;
static EIMSK_reg_t& _EIMSK = *(EIMSK_reg_t*)0x3D;
static _bit_reg_t& _GPIOR0 = *(_bit_reg_t*)0x3E;
static EECR_reg_t& _EECR = *(EECR_reg_t*)0x3F;
static uint8_t& _EEDR = *(uint8_t*)0x40;
static EEAR_reg_t& _EEAR = *(EEAR_reg_t*)0x41;
static GTCCR_reg_t& _GTCCR = *(GTCCR_reg_t*)0x43;
static TIMER_8bit_dev_t& TIMER0 = *(TIMER_8bit_dev_t*)0x44;
static _bit_reg_t& _GPIOR1 = *(_bit_reg_t*)0x4A;
static _bit_reg_t& _GPIOR2 = *(_bit_reg_t*)0x4B;
static SPCR_reg_t& _SPCR = *(SPCR_reg_t*)0x4C;
static SPSR_reg_t& _SPSR = *(SPSR_reg_t*)0x4D;
static ACSR_reg_t& _ACSR = *(ACSR_reg_t*)0x50;
static SMCR_reg_t& _SMCR = *(SMCR_reg_t*)0x53;
static MCUSR_reg_t& _MCUSR = *(MCUSR_reg_t*)0x54;
static MCUCR_reg_t& _MCUCR = *(MCUCR_reg_t*)0x55;
static SPMCSR_reg_t& _SPMCSR = *(SPMCSR_reg_t*)0x57;
static SP_reg_t& _SP = *(SP_reg_t*)0x5D;
static SREG_reg_t& _SREG = *(SREG_reg_t*)0x5F;
static WDTCSR_reg_t& _WDTCSR = *(WDTCSR_reg_t*)0x60;
static CLKPR_reg_t& _CLKPR = *(CLKPR_reg_t*)0x61;
static PRR0_reg_t& _PRR0 = *(PRR0_reg_t*)0x64;
static uint8_t& _OSCCAL = *(uint8_t*)0x66;
static PCICR_reg_t& _PCICR = *(PCICR_reg_t*)0x68;
static EICRA_reg_t& _EICRA = *(EICRA_reg_t*)0x69;
static _bit_reg_t& _PCMSK0 = *(_bit_reg_t*)0x6B;
static _bitPCMSK1_reg_t& _PCMSK1 = *(_bitPCMSK1_reg_t*)0x6C;
static _bit_reg_t& _PCMSK2 = *(_bit_reg_t*)0x6D;
static TIMSK0_reg_t& _TIMSK0 = *(TIMSK0_reg_t*)0x6E;
static TIMSK1_reg_t& _TIMSK1 = *(TIMSK1_reg_t*)0x6F;
static TIMSK2_reg_t& _TIMSK2 = *(TIMSK2_reg_t*)0x70;
static uint16_t& _ADC = *(uint16_t*)0x78;
static ADCSRA_reg_t& _ADCSRA = *(ADCSRA_reg_t*)0x7A;
static ADCSRB_reg_t& _ADCSRB = *(ADCSRB_reg_t*)0x7B;
static ADMUX_reg_t& _ADMUX = *(ADMUX_reg_t*)0x7C;
static DIDR0_reg_t& _DIDR0 = *(DIDR0_reg_t*)0x7E;
static DIDR1_reg_t& _DIDR1 = *(DIDR1_reg_t*)0x7F;
static TIMER_dev_t& TIMER1 = *(TIMER_dev_t*)0x80;
static TIMER_8bit_dev_t& _TIMER2 = *(TIMER_8bit_dev_t*)0xB0;
static ASSR_reg_t& _ASSR = *(ASSR_reg_t*)0xB6;
static uint8_t& _TWBR = *(uint8_t*)0xB8;
static TWSR_reg_t& _TWSR = *(TWSR_reg_t*)0xB9;
static TWAR_reg_t& _TWAR = *(TWAR_reg_t*)0xBA;
static uint8_t& _TWDR = *(uint8_t*)0xBB;
static TWCR_reg_t& _TWCR = *(TWCR_reg_t*)0xBC;
static TWAMR_reg_t& _TWAMR = *(TWAMR_reg_t*)0xBD;
static USART_dev_t& USART0 = *(USART_dev_t*)0xC0;
#endif

/*
  HELPER FUNCTIONS
*/

inline void _ATmega_resetperipherals() {
  // Due to BOOTLOADER or other board inconsistencies we could get launched into Marlin FW
  // with configuration that does not match the reset state in the documentation. That is why
  // we should clean-reset the entire device.
#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)
  SREG_reg_t __SREG;
  __SREG._C = false;
  __SREG._Z = false;
  __SREG._N = false;
  __SREG._V = false;
  __SREG._S = false;
  __SREG._H = false;
  __SREG._T = false;
  __SREG._I = false;
  _SREG = __SREG;
#endif

#if defined(__AVR_TRM01__)
  _RAMPZ._RAMPZ = 0;
  _EIND._EIND0 = false;
#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)
  _EEAR._EEAR = 0;
  _EEDR = 0;
#endif
  
#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)
  EECR_reg_t __EECR;
  __EECR._EERE = false;
  __EECR._EEPE = false;
  __EECR._EEMPE = false;
  __EECR._EERIE = false;
  __EECR._EEPM0 = 0;
  __EECR._EEPM1 = 0;
  __EECR.reserved1 = 0;
  _EECR = __EECR;
#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)
  _GPIOR2.val = 0;
  _GPIOR1.val = 0;
  _GPIOR0.val = 0;
#endif

#if defined(__AVR_TRM01__)
  XMCRA_reg_t __XMCRA;
  __XMCRA._SRW0 = 0;
  __XMCRA._SRW1 = 0;
  __XMCRA._SRL = 0;
  __XMCRA._SRE = 0;
  _XMCRA = __XMCRA;

  XMCRB_reg_t __XMCRB;
  __XMCRB._XMM = 0;
  __XMCRB.reserved1 = 0;
  __XMCRB._XMBK = false;
  _XMCRB = __XMCRB;
#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)
  SMCR_reg_t __SMCR;
  __SMCR._SE = false;
  __SMCR._SM = 0;
  __SMCR.reserved1 = 0;
  _SMCR = __SMCR;
#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)
  PRR0_reg_t __PRR0;
#if defined(__AVR_TRM01__) || defined(__AVR_TRM03__)
  __PRR0._PRADC = false;
  __PRR0._PRUSART0 = false;
  __PRR0._PRSPI = false;
  __PRR0._PRTIM1 = false;
  __PRR0.reserved1 = false;
  __PRR0._PRTIM0 = false;
  __PRR0._PRTIM2 = false;
  __PRR0._PRTWI = false;
#elif defined(__AVR_TRM02__)
  __PRR0._PRADC = false;
  __PRR0._PRUSART0 = false;
  __PRR0._PRSPI = false;
  __PRR0._PRTIM1 = false;
  __PRR0._PRUSART1 = false;
  __PRR0._PRTIM0 = false;
  __PRR0._PRTIM2 = false;
  __PRR0._PRTWI = false;
#endif
  _PRR0 = __PRR0;
#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__)
  PRR1_reg_t __PRR1;
#if defined(__AVR_TRM01__)
  __PRR1._PRUSART1 = false;
  __PRR1._PRUSART2 = false;
  __PRR1._PRUSART3 = false;
  __PRR1._PRTIM3 = false;
  __PRR1._PRTIM4 = false;
  __PRR1._PRTIM5 = false;
  __PRR1.reserved1 = 0;
#elif defined(__AVR_TRM02__)
  __PRR1._PRTIM3 = false;
  __PRR1.reserved1 = 0;
#endif
  _PRR1 = __PRR1;
#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)
  WDTCSR_reg_t __WDTCSR;
  __WDTCSR._WDP0 = 0;
  __WDTCSR._WDP1 = 0;
  __WDTCSR._WDP2 = 0;
  __WDTCSR._WDE = false;
  __WDTCSR._WDCE = false;
  __WDTCSR._WDP3 = 0;
  __WDTCSR._WDIE = false;
  __WDTCSR._WDIF = false;
  _WDTCSR = __WDTCSR;
#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)
  _MCUCR._PUD = false;
#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)
  PORT_dev_t __PORT;
  __PORT._PIN.val = 0;
  __PORT._DDR.val = 0;
  __PORT._PORT.val = 0;
#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__)
  _PORTA = __PORT;
  _PORTC = __PORT;
#endif
#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)
  _PORTB = __PORT;
  _PORTD = __PORT;
#endif
#if defined(__AVR_TRM01__)
  _PORTE = __PORT;
  _PORTF = __PORT;
#endif

#if defined(__AVR_TRM01__)
  PORTG_dev_t __PORTG;
  __PORTG._PIN.val = 0;
  __PORTG._PIN.reserved1 = 0;
  __PORTG._DDR.val = 0;
  __PORTG._DDR.reserved1 = 0;
  __PORTG._PORT.val = 0;
  __PORTG._PORT.reserved1 = 0;
  _PORTG = __PORTG;
#endif

#if defined(__AVR_TRM03__)
  PORTC_dev_t __PORTC;
  __PORTC._PIN.val = 0;
  __PORTC._PIN.reserved1 = 0;
  __PORTC._DDR.val = 0;
  __PORTC._DDR.reserved1 = 0;
  __PORTC._PORT.val = 0;
  __PORTC._PORT.reserved1 = 0;
  _PORTC = __PORTC;
#endif

#if defined(__AVR_TRM01__)
  _PORTH = __PORT;
  _PORTJ = __PORT;
  _PORTK = __PORT;
  _PORTL = __PORT;
#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)
  EICRA_reg_t __EICRA;
#if defined(__AVR_TRM01__)
  __EICRA._ISC0 = 0;
  __EICRA._ISC1 = 0;
  __EICRA._ISC2 = 0;
  __EICRA._ISC3 = 0;
#elif defined(__AVR_TRM02__)
  __EICRA._ISC0 = 0;
  __EICRA._ISC1 = 0;
  __EICRA._ISC2 = 0;
  __EICRA.reserved1 = 0;
#elif defined(__AVR_TRM03__)
  __EICRA._ISC0 = 0;
  __EICRA._ISC1 = 0;
  __EICRA.reserved1 = 0;
#endif
  _EICRA = __EICRA;
#endif

#if defined(__AVR_TRM01__)
  EICRB_reg_t __EICRB;
  __EICRB._ISC4 = 0;
  __EICRB._ISC5 = 0;
  __EICRB._ISC6 = 0;
  __EICRB._ISC7 = 0;
  _EICRB = __EICRB;
#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)
  EIMSK_reg_t __EIMSK;
#if defined(__AVR_TRM01__)
  __EIMSK._INT0 = false;
  __EIMSK._INT1 = false;
  __EIMSK._INT2 = false;
  __EIMSK._INT3 = false;
  __EIMSK._INT4 = false;
  __EIMSK._INT5 = false;
  __EIMSK._INT6 = false;
  __EIMSK._INT7 = false;
#elif defined(__AVR_TRM02__)
  __EIMSK._INT0 = false;
  __EIMSK._INT1 = false;
  __EIMSK._INT2 = false;
  __EIMSK.reserved1 = 0;
#elif defined(__AVR_TRM03__)
  __EIMSK._INT0 = false;
  __EIMSK._INT1 = false;
  __EIMSK.reserved1 = 0;
#endif
  _EIMSK = __EIMSK;
#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)
  EIFR_reg_t __EIFR;
#if defined(__AVR_TRM01__)
  __EIFR._INTF0 = false;
  __EIFR._INTF1 = false;
  __EIFR._INTF2 = false;
  __EIFR._INTF3 = false;
  __EIFR._INTF4 = false;
  __EIFR._INTF5 = false;
  __EIFR._INTF6 = false;
  __EIFR._INTF7 = false;
#elif defined(__AVR_TRM02__)
  __EIFR._INTF0 = false;
  __EIFR._INTF1 = false;
  __EIFR._INTF2 = false;
  __EIFR.reserved1 = 0;
#elif defined(__AVR_TRM03__)
  __EIFR._INTF0 = false;
  __EIFR._INTF1 = false;
  __EIFR.reserved1 = 0;
#endif
  _EIFR = __EIFR;
#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)
  PCICR_reg_t __PCICR;
#if defined(__AVR_TRM01__) || defined(__AVR_TRM03__)
  __PCICR._PCIE0 = false;
  __PCICR._PCIE1 = false;
  __PCICR._PCIE2 = false;
  __PCICR.reserved1 = 0;
#elif defined(__AVR_TRM02__)
  __PCICR._PCIE0 = false;
  __PCICR._PCIE1 = false;
  __PCICR._PCIE2 = false;
  __PCICR._PCIE3 = false;
  __PCICR.reserved1 = 0;
#endif
  _PCICR = __PCICR;
#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)
  PCIFR_reg_t __PCIFR;
#if defined(__AVR_TRM01__) || defined(__AVR_TRM03__)
  __PCIFR._PCIF0 = false;
  __PCIFR._PCIF1 = false;
  __PCIFR._PCIF2 = false;
  __PCIFR.reserved1 = 0;
#elif defined(__AVR_TRM02__)
  __PCIFR._PCIF0 = false;
  __PCIFR._PCIF1 = false;
  __PCIFR._PCIF2 = false;
  __PCIFR._PCIF3 = false;
  __PCIFR.reserved1 = 0;
#endif
  _PCIFR = __PCIFR;
#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)
  _PCMSK2.val = 0;
  _PCMSK1.val = 0;
  _PCMSK0.val = 0;
#endif
#if defined(__AVR_TRM02__)
  _PCMSK3.val = 0;
#endif
#if defined(__AVR_TRM03__)
  _PCMSK1.reserved1 = 0;
#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)
  TIMER_8bit_dev_t __TIMER_8bit;
  __TIMER_8bit._TCCRnA._WGMn0 = 0;
  __TIMER_8bit._TCCRnA._WGMn1 = 0;
  __TIMER_8bit._TCCRnA.reserved1 = 0;
  __TIMER_8bit._TCCRnA._COMnB = 0;
  __TIMER_8bit._TCCRnA._COMnA = 0;
  __TIMER_8bit._TCCRnB._CSn = 0;
  __TIMER_8bit._TCCRnB._WGMn2 = 0;
  __TIMER_8bit._TCCRnB.reserved1 = 0;
  __TIMER_8bit._TCCRnB._FOCnB = false;
  __TIMER_8bit._TCCRnB._FOCnA = false,
  __TIMER_8bit._TCNTn = 0;
  __TIMER_8bit._OCRnA = 0;
  __TIMER_8bit._OCRnB = 0;
  TIMER0 = __TIMER_8bit;
#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)
  TIMSK0_reg_t __TIMSK0;
  __TIMSK0._TOIE0 = false;
  __TIMSK0._OCIE0A = false;
  __TIMSK0._OCIE0B = false;
  __TIMSK0.reserved1 = 0;
  _TIMSK0 = __TIMSK0;

  TIFR0_reg_t __TIFR0;
  __TIFR0._TOV0 = false;
  __TIFR0._OCF0A = false;
  __TIFR0._OCF0B = false;
  __TIFR0.reserved1 = 0;
  _TIFR0 = __TIFR0;
#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)
  TIMER_dev_t TIMER;
  TIMER._TCCRnA._WGMn0 = 0;
  TIMER._TCCRnA._WGMn1 = 0;
#if defined(__AVR_TRM01__)
  TIMER._TCCRnA._COMnC = 0;
#endif
  TIMER._TCCRnA._COMnB = 0;
  TIMER._TCCRnA._COMnA = 0;
  TIMER._TCCRnB._CSn = 0;
  TIMER._TCCRnB._WGMn2 = 0;
  TIMER._TCCRnB.reserved1 = 0;
  TIMER._TCCRnB._ICESn = 0;
  TIMER._TCCRnB._ICNCn = 0;
  TIMER._TCCRnC.reserved1 = 0;
#if defined(__AVR_TRM01__)
  TIMER._TCCRnC._FOCnC = false;
#endif
  TIMER._TCCRnC._FOCnB = false;
  TIMER._TCCRnC._FOCnA = false;
  TIMER._TCNTn = 0;
  TIMER._OCRnA = 0;
  TIMER._OCRnB = 0;
#if defined(__AVR_TRM01__)
  TIMER._OCRnC = 0;
#endif
  TIMER._ICRn = 0;
  TIMER1 = TIMER;
#endif
#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__)
  TIMER3 = TIMER;
#endif
#if defined(__AVR_TRM01__)
  TIMER4 = TIMER;
  TIMER5 = TIMER;
#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)
  TIMSK1_reg_t __TIMSK1;
  __TIMSK1._TOIE1 = false;
  __TIMSK1._OCIE1A = false;
  __TIMSK1._OCIE1B = false;
#if defined(__AVR_TRM01__)
  __TIMSK1._OCIE1C = false;
#endif
  __TIMSK1.reserved1 = 0;
  __TIMSK1._ICIE1 = false;
  __TIMSK1.reserved2 = 0;
  _TIMSK1 = __TIMSK1;
#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__)
  TIMSK3_reg_t __TIMSK3;
  __TIMSK3._TOIE3 = false;
  __TIMSK3._OCIE3A = false;
  __TIMSK3._OCIE3B = false;
#if defined(__AVR_TRM01__)
  __TIMSK3._OCIE3C = false;
#endif
  __TIMSK3.reserved1 = 0;
  __TIMSK3._ICIE3 = false;
  __TIMSK3.reserved2 = 0;
  _TIMSK3 = __TIMSK3;
#endif

#if defined(__AVR_TRM01__)
  TIMSK4_reg_t __TIMSK4;
  __TIMSK4._TOIE4 = false;
  __TIMSK4._OCIE4A = false;
  __TIMSK4._OCIE4B = false;
  __TIMSK4._OCIE4C = false;
  __TIMSK4.reserved1 = false;
  __TIMSK4._ICIE4 = false;
  __TIMSK4.reserved2 = false;
  _TIMSK4 = __TIMSK4;

  TIMSK5_reg_t __TIMSK5;
  __TIMSK5._TOIE5 = false;
  __TIMSK5._OCIE5A = false;
  __TIMSK5._OCIE5B = false; 
  __TIMSK5._OCIE5C = false;
  __TIMSK5.reserved1 = 0;
  __TIMSK5._ICIE5 = false;
  __TIMSK5.reserved2 = 0;
  _TIMSK5 = __TIMSK5;
#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)
  TIFR1_reg_t __TIFR1;
  __TIFR1._TOV1 = false;
  __TIFR1._OCF1A = false;
  __TIFR1._OCF1B = false;
#if defined(__AVR_TRM01__)
  __TIFR1._OCF1C = false;
#endif
  __TIFR1.reserved1 = 0;
  __TIFR1._ICF1 = false;
  __TIFR1.reserved2 = 0;
  _TIFR1 = __TIFR1;
#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__)
  TIFR3_reg_t __TIFR3;
  __TIFR3._TOV3 = false;
  __TIFR3._OCF3A = false;
  __TIFR3._OCF3B = false;
#if defined(__AVR_TRM01__)
  __TIFR3._OCF3C = false;
#endif
  __TIFR3.reserved1 = 0;
  __TIFR3._ICF3 = false;
  __TIFR3.reserved2 = 0;
  _TIFR3 = __TIFR3;
#endif

#if defined(__AVR_TRM01__)
  TIFR4_reg_t __TIFR4;
  __TIFR4._TOV4 = false;
  __TIFR4._OCF4A = false;
  __TIFR4._OCF4B = false;
  __TIFR4._OCF4C = false;
  __TIFR4.reserved1 = 0;
  __TIFR4._ICF4 = false;
  __TIFR4.reserved2 = 0;
  _TIFR4 = __TIFR4;

  TIFR5_reg_t __TIFR5;
  __TIFR5._TOV5 = false;
  __TIFR5._OCF5A = false;
  __TIFR5._OCF5B = false;
  __TIFR5._OCF5C = false;
  __TIFR5.reserved1 = 0;
  __TIFR5._ICF5 = false;
  __TIFR5.reserved2 = 0;
  _TIFR5 = __TIFR5;
#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)
  _TIMER2 = __TIMER_8bit;
#endif

#if defined(__AV_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)
  ASSR_reg_t __ASSR;
  __ASSR._TCR2BUB = false;
  __ASSR._TCR2AUB = false;
  __ASSR._OCR2BUB = false;
  __ASSR._OCR2AUB = false;
  __ASSR._TCN2UB = false;
  __ASSR._AS2 = false;
  __ASSR._EXCLK = false;
  __ASSR.reserved1 = 0;
  _ASSR = __ASSR;
#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)
  TIMSK2_reg_t __TIMSK2;
  __TIMSK2._TOIE2 = false;
  __TIMSK2._OCIE2A = false;
  __TIMSK2._OCIE2B = false;
  __TIMSK2.reserved1 = 0;
  _TIMSK2 = __TIMSK2;

  TIFR2_reg_t __TIFR2;
  __TIFR2._TOV2 = false;
  __TIFR2._OCF2A = false;
  __TIFR2._OCF2B = false;
  __TIFR2.reserved1 = 0;
  _TIFR2 = __TIFR2;
#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)
  SPCR_reg_t __SPCR;
  __SPCR._SPR = 0;
  __SPCR._CPHA = 0;
  __SPCR._CPOL = 0;
  __SPCR._MSTR = 0;
  __SPCR._DORD = 0;
  __SPCR._SPE = false;
  __SPCR._SPIE = false;
  _SPCR = __SPCR;

  SPSR_reg_t __SPSR;
  __SPSR._SPI2X = false;
  __SPSR.reserved1 = 0;
  __SPSR._WCOL = false;
  __SPSR._SPIF = false;
  _SPSR = __SPSR;
#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)
  USART_dev_t USART;
  USART._UDRn = 0;
  USART._UCSRnA._MPCM = false;
  USART._UCSRnA._U2X = false;
  USART._UCSRnA._UPE = false;
  USART._UCSRnA._DOR = false;
  USART._UCSRnA._FE = false;
  USART._UCSRnA._UDRE = true;
  USART._UCSRnA._TXC = false;
  USART._UCSRnA._RXC = false;
  USART._UCSRnB._TXB8 = false;
  USART._UCSRnB._RXB8 = false;
  USART._UCSRnB._UCSZn2 = false;
  USART._UCSRnB._TXEN = false;
  USART._UCSRnB._RXEN = false;
  USART._UCSRnB._UDRIE = false;
  USART._UCSRnB._TXCIE = false;
  USART._UCSRnB._RXCIE = false;
  USART._UCSRnC._UCPOL = false;
  USART._UCSRnC._UCSZn0 = 1;
  USART._UCSRnC._UCSZn1 = 1;
  USART._UCSRnC._USBS = false;
  USART._UCSRnC._UPM = 0;
  USART._UCSRnC._UPM = 0;
  USART._UCSRnC._UMSEL = 0;
  USART._UBRRn._UBRR = 0;
  USART._UBRRn.reserved1 = 0;
  USART0 = USART;
#endif
#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__)
  USART1 = USART;
#endif
#if defined(__AVR_TRM01__)
  USART2 = USART;
  USART3 = USART;
#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)
  _TWBR = 0;

  TWCR_reg_t __TWCR;
  __TWCR._TWIE = false;
  __TWCR.reserved1 = 0;
  __TWCR._TWEN = false;
  __TWCR._TWWC = false;
  __TWCR._TWSTO = false;
  __TWCR._TWSTA = false;
  __TWCR._TWEA = false;
  __TWCR._TWINT = false;
  _TWCR = __TWCR;

  TWSR_reg_t __TWSR;
  __TWSR._TWPS0 = false;
  __TWSR._TWPS1 = false;
  __TWSR.reserved1 = 0;
  __TWSR._TWS3 = 1;
  __TWSR._TWS4 = 1;
  __TWSR._TWS5 = 1;
  __TWSR._TWS6 = 1;
  __TWSR._TWS7 = 1;
  _TWSR = __TWSR;

  _TWDR = 0xFF;

  TWAR_reg_t __TWAR;
  __TWAR._TWGCE = false;
  __TWAR._TWA = 0x7F;
  _TWAR = __TWAR;

  TWAMR_reg_t __TWAMR;
  __TWAMR.reserved1 = false;
  __TWAMR._TWAM = 0;
  _TWAMR = __TWAMR;
#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)
  ADCSRB_reg_t __ADCSRB;
  __ADCSRB._ADTS = 0;
#if defined(__AVR_TRM01__)
  __ADCSRB._MUX5 = 0;
#endif
  __ADCSRB.reserved1 = 0;
  __ADCSRB._ACME = false;
  __ADCSRB.reserved2 = 0;
  _ADCSRB = __ADCSRB;

  ACSR_reg_t __ACSR;
  __ACSR._ACIS = 0;
  __ACSR._ACIC = false;
  __ACSR._ACIE = false;
  __ACSR._ACI = false;
  __ACSR._ACO = false;
  __ACSR._ACBG = false;
  __ACSR._ACD = false;
  _ACSR = __ACSR;
#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)
  DIDR1_reg_t __DIDR1;
  __DIDR1._AIN0D = false;
  __DIDR1._AIN1D = false;
  __DIDR1.reserved1 = false;
  _DIDR1 = __DIDR1;
#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)
  ADMUX_reg_t __ADMUX;
  __ADMUX._MUX0 = 0;
  __ADMUX._MUX1 = 0;
  __ADMUX._MUX2 = 0;
  __ADMUX._MUX3 = 0;
#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__)
  __ADMUX._MUX4 = 0;
#elif defined(__AVR_TRM03__)
  __ADMUX.reserved1 = 0;
#endif
  __ADMUX._ADLAR = 0;
  __ADMUX._REFS0 = 0;
  __ADMUX._REFS1 = 0;
  _ADMUX = __ADMUX;

  ADCSRA_reg_t __ADCSRA;
  __ADCSRA._ADPS = 0;
  __ADCSRA._ADIE = false;
  __ADCSRA._ADIF = false;
  __ADCSRA._ADATE = false;
  __ADCSRA._ADSC = false;
  __ADCSRA._ADEN = false;
  _ADCSRA = __ADCSRA;

  _ADC = 0;
#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)
  SPMCSR_reg_t __SPMCSR;
#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__)
  __SPMCSR._SPMEN = false;
  __SPMCSR._PGERS = false;
  __SPMCSR._PGWRT = false;
  __SPMCSR._BLBSET = false;
  __SPMCSR._RWWSRE = false;
  __SPMCSR._SIGRD = false;
  __SPMCSR._RWWSB = false;
  __SPMCSR._SPMIE = false;
#elif defined(__AVR_TRM03__)
#if defined(__AVR_ATmega88A__) || defined(__AVR_ATmega88PA__) || defined(__AVR_ATmega168A__) || defined(__AVR_ATmega168PA__) || defined(__AVR_ATmega328P__)
  __SPMCSR._SPMEN = false;
  __SPMCSR._PGERS = false;
  __SPMCSR._PGWRT = false;
  __SPMCSR._BLBSET = false;
  __SPMCSR._RWWSRE = false;
  __SPMCSR._SIGRD = false;
  __SPMCSR._RWWSB = false;
  __SPMCSR._SPMIE = false;
#else
  __SPMCSR._SPMEN = false;
  __SPMCSR._PGERS = false;
  __SPMCSR._PGWRT = false;
  __SPMCSR._BLBSET = false;
  __SPMCSR.reserved1 = false;
  __SPMCSR._SIGRD = false;
  __SPMCSR.reserved2 = false;
  __SPMCSR._SPMIE = false;
#endif
#endif
  _SPMCSR = __SPMCSR;
#endif
}

struct pin_dev_state_t {
#if defined(__AVR_TRM01__)
  uint8_t _SRE : 1;   // port A
  uint8_t _COM0B : 2;
  uint8_t _COM1A : 2;
  uint8_t _COM1B : 2;
  uint8_t _COM1C : 2;
  uint8_t _COM2A : 2;
  uint8_t _COM2B : 2;
  uint8_t _COM3B : 2;
  uint8_t _COM3C : 2;
  uint8_t _COM4A : 2;
  uint8_t _COM4B : 2;
  uint8_t _COM4C : 2;
  uint8_t _COM5A : 2;
  uint8_t _COM5B : 2;
  uint8_t _COM5C : 2;
  uint8_t _PCIE0 : 1;
  uint8_t _PCIE1 : 1; // INTn
  uint8_t _PCIE2 : 1;
  uint8_t _SPE : 1;
  uint8_t _USART0_RXEN : 1;
  uint8_t _USART0_TXEN : 1;
  uint8_t _USART1_RXEN : 1;
  uint8_t _USART1_TXEN : 1;
  uint8_t _USART2_RXEN : 1;
  uint8_t _USART2_TXEN : 1;
  uint8_t _USART3_RXEN : 1;
  uint8_t _USART3_TXEN : 1;
  //uint8_t _JTAGEN : 1;
  uint8_t _AS2 : 1;
#elif defined(__AVR_TRM02__)
  uint8_t _PCIE0 : 1;
  uint8_t _PCIE1 : 1;
  uint8_t _PCIE2 : 1;
  uint8_t _PCIE3 : 1;
  uint8_t _ADCnD : 1;
  uint8_t _SPE : 1;
  uint8_t _COM0A : 2;
  uint8_t _COM0B : 2;
  uint8_t _COM2A : 2;
  uint8_t _COM2B : 2;
  uint8_t _COM1A : 2;
  uint8_t _COM1B : 2;
  //uint8_t _JTAGEN : 1;
  uint8_t _AS2 : 1;
  uint8_t _TWEN : 1;
  uint8_t _USART1_TXEN : 1;
  uint8_t _USART1_RXEN : 1;
  uint8_t _USART0_TXEN : 1;
#elif defined(__AVR_TRM03__)
  uint8_t _AS2 : 1;
  uint8_t _PCIE0 : 1;
  uint8_t _PCIE1 : 1;
  uint8_t _PCIE2 : 1;
  uint8_t _SPE : 1;
  uint8_t _COM2B : 2;
  uint8_t _COM2A : 2;
  uint8_t _COM1B : 2;
  uint8_t _COM1A : 2;
  uint8_t _COM0A : 2;
  uint8_t _COM0B : 2;
  uint8_t _TWEN : 1;
  uint8_t _ADCnD : 1;
  uint8_t _UMSEL : 2;
  uint8_t _USART0_TXEN : 1;
  uint8_t _USART0_RXEN : 1;
#endif
};

// AVR ArduinoCore is written like a hack-job (random peripherals enabled all-the-time).

enum class eATmegaPort {
#if defined(__AVR_TRM01__)
  PORT_A, PORT_B, PORT_C, PORT_D, PORT_E, PORT_F, PORT_G, PORT_H, PORT_J, PORT_K, PORT_L
#elif defined(__AVR_TRM02__)
  PORT_A, PORT_B, PORT_C, PORT_D
#elif defined(__AVR_TRM03__)
  PORT_B, PORT_C, PORT_D
#endif
};

struct ATmegaPinInfo {
  eATmegaPort port;
  uint8_t pinidx;
};

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__)
#define _SPA_DIO_DDRA (eATmegaPort::PORT_A)
#endif
#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__)
#define _SPA_DIO_DDRB (eATmegaPort::PORT_B)
#define _SPA_DIO_DDRC (eATmegaPort::PORT_C)
#define _SPA_DIO_DDRD (eATmegaPort::PORT_D)
#endif
#if defined(__AVR_TRM01__)
#define _SPA_DIO_DDRE (eATmegaPort::PORT_E)
#define _SPA_DIO_DDRF (eATmegaPort::PORT_F)
#define _SPA_DIO_DDRG (eATmegaPort::PORT_G)
#define _SPA_DIO_DDRH (eATmegaPort::PORT_H)
#define _SPA_DIO_DDRJ (eATmegaPort::PORT_J)
#define _SPA_DIO_DDRK (eATmegaPort::PORT_K)
#define _SPA_DIO_DDRL (eATmegaPort::PORT_L)
#endif

#define __SPA_IFPORT_STMT(dr) if (ddrp == &D##dr) port = _SPA_DIO_D##dr;

#ifdef _SPA_DIO_DDRA
#define _SPA_IFPORT_PORTA __SPA_IFPORT_STMT(DRA)
#else
#define _SPA_IFPORT_PORTA
#endif
#ifdef _SPA_DIO_DDRB
#define _SPA_IFPORT_PORTB __SPA_IFPORT_STMT(DRB)
#else
#define _SPA_IFPORT_PORTB
#endif
#ifdef _SPA_DIO_DDRC
#define _SPA_IFPORT_PORTC __SPA_IFPORT_STMT(DRC)
#else
#define _SPA_IFPORT_PORTC
#endif
#ifdef _SPA_DIO_DDRD
#define _SPA_IFPORT_PORTD __SPA_IFPORT_STMT(DRD)
#else
#define _SPA_IFPORT_PORTD
#endif
#ifdef _SPA_DIO_DDRE
#define _SPA_IFPORT_PORTE __SPA_IFPORT_STMT(DRE)
#else
#define _SPA_IFPORT_PORTE
#endif
#ifdef _SPA_DIO_DDRF
#define _SPA_IFPORT_PORTF __SPA_IFPORT_STMT(DRF)
#else
#define _SPA_IFPORT_PORTF
#endif
#ifdef _SPA_DIO_DDRG
#define _SPA_IFPORT_PORTG __SPA_IFPORT_STMT(DRG)
#else
#define _SPA_IFPORT_PORTG
#endif
#ifdef _SPA_DIO_DDRH
#define _SPA_IFPORT_PORTH __SPA_IFPORT_STMT(DRH)
#else
#define _SPA_IFPORT_PORTH
#endif
#ifdef _SPA_DIO_DDRJ
#define _SPA_IFPORT_PORTJ __SPA_IFPORT_STMT(DRJ)
#else
#define _SPA_IFPORT_PORTJ
#endif
#ifdef _SPA_DIO_DDRK
#define _SPA_IFPORT_PORTK __SPA_IFPORT_STMT(DRK)
#else
#define _SPA_IFPORT_PORTK
#endif
#ifdef _SPA_DIO_DDRL
#define _SPA_IFPORT_PORTL __SPA_IFPORT_STMT(DRL)
#else
#define _SPA_IFPORT_PORTL
#endif

#define _SPA_RESOLVE_DIO(ddr) _SPA_DIO_##ddr
#define _SPA_DIOn_PORTRET(val, n) if (val == n) { \
    auto *ddrp = &DIO##n##_DDR; \
    eATmegaPort port; \
    _SPA_IFPORT_PORTA \
    _SPA_IFPORT_PORTB \
    _SPA_IFPORT_PORTC \
    _SPA_IFPORT_PORTD \
    _SPA_IFPORT_PORTE \
    _SPA_IFPORT_PORTF \
    _SPA_IFPORT_PORTG \
    _SPA_IFPORT_PORTH \
    _SPA_IFPORT_PORTJ \
    _SPA_IFPORT_PORTK \
    _SPA_IFPORT_PORTL \
    return { port, DIO##n##_PIN }; \
  }

inline ATmegaPinInfo _ATmega_getPinInfo(uint8_t pin) {
#if DIO_NUM > 0
  _SPA_DIOn_PORTRET(pin, 0)
#endif
#if DIO_NUM > 1
  _SPA_DIOn_PORTRET(pin, 1)
#endif
#if DIO_NUM > 2
  _SPA_DIOn_PORTRET(pin, 2)
#endif
#if DIO_NUM > 3
  _SPA_DIOn_PORTRET(pin, 3)
#endif
#if DIO_NUM > 4
  _SPA_DIOn_PORTRET(pin, 4)
#endif
#if DIO_NUM > 5
  _SPA_DIOn_PORTRET(pin, 5)
#endif
#if DIO_NUM > 6
  _SPA_DIOn_PORTRET(pin, 6)
#endif
#if DIO_NUM > 7
  _SPA_DIOn_PORTRET(pin, 7)
#endif
#if DIO_NUM > 8
  _SPA_DIOn_PORTRET(pin, 8)
#endif
#if DIO_NUM > 9
  _SPA_DIOn_PORTRET(pin, 9)
#endif

#if DIO_NUM > 10
  _SPA_DIOn_PORTRET(pin, 10)
#endif
#if DIO_NUM > 11
  _SPA_DIOn_PORTRET(pin, 11)
#endif
#if DIO_NUM > 12
  _SPA_DIOn_PORTRET(pin, 12)
#endif
#if DIO_NUM > 13
  _SPA_DIOn_PORTRET(pin, 13)
#endif
#if DIO_NUM > 14
  _SPA_DIOn_PORTRET(pin, 14)
#endif
#if DIO_NUM > 15
  _SPA_DIOn_PORTRET(pin, 15)
#endif
#if DIO_NUM > 16
  _SPA_DIOn_PORTRET(pin, 16)
#endif
#if DIO_NUM > 17
  _SPA_DIOn_PORTRET(pin, 17)
#endif
#if DIO_NUM > 18
  _SPA_DIOn_PORTRET(pin, 18)
#endif
#if DIO_NUM > 19
  _SPA_DIOn_PORTRET(pin, 19)
#endif

#if DIO_NUM > 20
  _SPA_DIOn_PORTRET(pin, 20)
#endif
#if DIO_NUM > 21
  _SPA_DIOn_PORTRET(pin, 21)
#endif
#if DIO_NUM > 22
  _SPA_DIOn_PORTRET(pin, 22)
#endif
#if DIO_NUM > 23
  _SPA_DIOn_PORTRET(pin, 23)
#endif
#if DIO_NUM > 24
  _SPA_DIOn_PORTRET(pin, 24)
#endif
#if DIO_NUM > 25
  _SPA_DIOn_PORTRET(pin, 25)
#endif
#if DIO_NUM > 26
  _SPA_DIOn_PORTRET(pin, 26)
#endif
#if DIO_NUM > 27
  _SPA_DIOn_PORTRET(pin, 27)
#endif
#if DIO_NUM > 28
  _SPA_DIOn_PORTRET(pin, 28)
#endif
#if DIO_NUM > 29
  _SPA_DIOn_PORTRET(pin, 29)
#endif

#if DIO_NUM > 30
  _SPA_DIOn_PORTRET(pin, 30)
#endif
#if DIO_NUM > 31
  _SPA_DIOn_PORTRET(pin, 31)
#endif
#if DIO_NUM > 32
  _SPA_DIOn_PORTRET(pin, 32)
#endif
#if DIO_NUM > 33
  _SPA_DIOn_PORTRET(pin, 33)
#endif
#if DIO_NUM > 34
  _SPA_DIOn_PORTRET(pin, 34)
#endif
#if DIO_NUM > 35
  _SPA_DIOn_PORTRET(pin, 35)
#endif
#if DIO_NUM > 36
  _SPA_DIOn_PORTRET(pin, 36)
#endif
#if DIO_NUM > 37
  _SPA_DIOn_PORTRET(pin, 37)
#endif
#if DIO_NUM > 38
  _SPA_DIOn_PORTRET(pin, 38)
#endif
#if DIO_NUM > 39
  _SPA_DIOn_PORTRET(pin, 39)
#endif

#if DIO_NUM > 40
  _SPA_DIOn_PORTRET(pin, 40)
#endif
#if DIO_NUM > 41
  _SPA_DIOn_PORTRET(pin, 41)
#endif
#if DIO_NUM > 42
  _SPA_DIOn_PORTRET(pin, 42)
#endif
#if DIO_NUM > 43
  _SPA_DIOn_PORTRET(pin, 43)
#endif
#if DIO_NUM > 44
  _SPA_DIOn_PORTRET(pin, 44)
#endif
#if DIO_NUM > 45
  _SPA_DIOn_PORTRET(pin, 45)
#endif
#if DIO_NUM > 46
  _SPA_DIOn_PORTRET(pin, 46)
#endif
#if DIO_NUM > 47
  _SPA_DIOn_PORTRET(pin, 47)
#endif
#if DIO_NUM > 48
  _SPA_DIOn_PORTRET(pin, 48)
#endif
#if DIO_NUM > 49
  _SPA_DIOn_PORTRET(pin, 49)
#endif

#if DIO_NUM > 50
  _SPA_DIOn_PORTRET(pin, 50)
#endif
#if DIO_NUM > 51
  _SPA_DIOn_PORTRET(pin, 51)
#endif
#if DIO_NUM > 52
  _SPA_DIOn_PORTRET(pin, 52)
#endif
#if DIO_NUM > 53
  _SPA_DIOn_PORTRET(pin, 53)
#endif
#if DIO_NUM > 54
  _SPA_DIOn_PORTRET(pin, 54)
#endif
#if DIO_NUM > 55
  _SPA_DIOn_PORTRET(pin, 55)
#endif
#if DIO_NUM > 56
  _SPA_DIOn_PORTRET(pin, 56)
#endif
#if DIO_NUM > 57
  _SPA_DIOn_PORTRET(pin, 57)
#endif
#if DIO_NUM > 58
  _SPA_DIOn_PORTRET(pin, 58)
#endif
#if DIO_NUM > 59
  _SPA_DIOn_PORTRET(pin, 59)
#endif

#if DIO_NUM > 60
  _SPA_DIOn_PORTRET(pin, 60)
#endif
#if DIO_NUM > 61
  _SPA_DIOn_PORTRET(pin, 61)
#endif
#if DIO_NUM > 62
  _SPA_DIOn_PORTRET(pin, 62)
#endif
#if DIO_NUM > 63
  _SPA_DIOn_PORTRET(pin, 63)
#endif
#if DIO_NUM > 64
  _SPA_DIOn_PORTRET(pin, 64)
#endif
#if DIO_NUM > 65
  _SPA_DIOn_PORTRET(pin, 65)
#endif
#if DIO_NUM > 66
  _SPA_DIOn_PORTRET(pin, 66)
#endif
#if DIO_NUM > 67
  _SPA_DIOn_PORTRET(pin, 67)
#endif
#if DIO_NUM > 68
  _SPA_DIOn_PORTRET(pin, 68)
#endif
#if DIO_NUM > 69
  _SPA_DIOn_PORTRET(pin, 69)
#endif

#if DIO_NUM > 70
  _SPA_DIOn_PORTRET(pin, 70)
#endif
#if DIO_NUM > 71
  _SPA_DIOn_PORTRET(pin, 71)
#endif
#if DIO_NUM > 72
  _SPA_DIOn_PORTRET(pin, 72)
#endif
#if DIO_NUM > 73
  _SPA_DIOn_PORTRET(pin, 73)
#endif
#if DIO_NUM > 74
  _SPA_DIOn_PORTRET(pin, 74)
#endif
#if DIO_NUM > 75
  _SPA_DIOn_PORTRET(pin, 75)
#endif
#if DIO_NUM > 76
  _SPA_DIOn_PORTRET(pin, 76)
#endif
#if DIO_NUM > 77
  _SPA_DIOn_PORTRET(pin, 77)
#endif
#if DIO_NUM > 78
  _SPA_DIOn_PORTRET(pin, 78)
#endif
#if DIO_NUM > 79
  _SPA_DIOn_PORTRET(pin, 79)
#endif

#if DIO_NUM > 80
  _SPA_DIOn_PORTRET(pin, 80)
#endif
#if DIO_NUM > 81
  _SPA_DIOn_PORTRET(pin, 81)
#endif
#if DIO_NUM > 82
  _SPA_DIOn_PORTRET(pin, 82)
#endif
#if DIO_NUM > 83
  _SPA_DIOn_PORTRET(pin, 83)
#endif
#if DIO_NUM > 84
  _SPA_DIOn_PORTRET(pin, 84)
#endif
#if DIO_NUM > 85
  _SPA_DIOn_PORTRET(pin, 85)
#endif
#if DIO_NUM > 86
  _SPA_DIOn_PORTRET(pin, 86)
#endif
#if DIO_NUM > 87
  _SPA_DIOn_PORTRET(pin, 87)
#endif
#if DIO_NUM > 88
  _SPA_DIOn_PORTRET(pin, 88)
#endif
#if DIO_NUM > 89
  _SPA_DIOn_PORTRET(pin, 89)
#endif

  // Default.
#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__)
  return { eATmegaPort::PORT_A, 0 };
#elif defined(__AVR_TRM03__)
  return { eATmegaPort::PORT_B, 0 };
#endif
}

inline pin_dev_state_t _ATmega_savePinAlternate(uint8_t pin) {
  ATmegaPinInfo info = _ATmega_getPinInfo(pin);

  pin_dev_state_t state;

#if defined(__AVR_TRM01__)
  // See page 75ff of ATmega2560 technical reference manual.
  if (info.port == eATmegaPort::PORT_A) {
    state._SRE = _XMCRA._SRE;

    _XMCRA._SRE = false;
  }
  else if (info.port == eATmegaPort::PORT_B) {
    state._PCIE0 = _PCICR._PCIE0;

    _PCICR._PCIE0 = false;

    if (info.pinidx == 7) {
      state._COM1C = TIMER1._TCCRnA._COMnC;

      TIMER1._TCCRnA._COMnC = 0;
    }
    else if (info.pinidx == 6) {
      state._COM1B = TIMER1._TCCRnA._COMnB;

      TIMER1._TCCRnA._COMnB = 0;
    }
    else if (info.pinidx == 5) {
      state._COM1A = TIMER1._TCCRnA._COMnA;

      TIMER1._TCCRnA._COMnA = 0;
    }
    else if (info.pinidx == 4) {
      state._COM2A = _TIMER2._TCCRnA._COMnA;

      _TIMER2._TCCRnA._COMnA = 0;
    }
    else if (info.pinidx <= 3) {
      state._SPE = _SPCR._SPE;

      _SPCR._SPE = false;
    }
  }
  else if (info.port == eATmegaPort::PORT_C) {
    state._SRE = _XMCRA._SRE;

    _XMCRA._SRE = false;
  }
  else if (info.port == eATmegaPort::PORT_D) {
    if (info.pinidx == 5 || info.pinidx == 3) {
      state._USART1_TXEN = USART1._UCSRnB._TXEN;

      USART1._UCSRnB._TXEN = false;
    }
    if (info.pinidx == 5 || info.pinidx == 2) {
      state._USART1_RXEN = USART1._UCSRnB._RXEN;

      USART1._UCSRnB._RXEN = false;
    }
    if (info.pinidx <= 3) {
      state._PCIE0 = _PCICR._PCIE0;

      _PCICR._PCIE0 = false;
    }
  }
  else if (info.port == eATmegaPort::PORT_E) {
    if (info.pinidx >= 4 && info.pinidx <= 7) {
      state._PCIE0 = _PCICR._PCIE0;

      _PCICR._PCIE0 = false;
    }
    if (info.pinidx == 5) {
      state._COM3C = TIMER3._TCCRnA._COMnC;

      TIMER3._TCCRnA._COMnC = 0;
    }
    if (info.pinidx == 4 || info.pinidx == 3) {
      state._COM3B = TIMER3._TCCRnA._COMnB;

      TIMER3._TCCRnA._COMnB = 0;
    }
    if (info.pinidx == 2 || info.pinidx == 0) {
      state._USART0_RXEN = USART0._UCSRnB._RXEN;

      USART0._UCSRnB._RXEN = false;
    }
    if (info.pinidx == 2 || info.pinidx == 1) {
      state._USART0_TXEN = USART0._UCSRnB._TXEN;

      USART0._UCSRnB._TXEN = false;
    }
    if (info.pinidx == 0) {
      state._PCIE1 = _PCICR._PCIE1;

      _PCICR._PCIE1 = false;
    }
  }
  // Port F ignored.
  else if (info.port == eATmegaPort::PORT_G) {
    if (info.pinidx == 5) {
      state._COM0B = TIMER0._TCCRnA._COMnB;

      TIMER0._TCCRnA._COMnB = 0;
    }
    if (info.pinidx == 4 || info.pinidx == 3) {
      state._AS2 = _ASSR._AS2;

      _ASSR._AS2 = false;
    }
    if (info.pinidx <= 2) {
      state._SRE = _XMCRA._SRE;

      _XMCRA._SRE = false;
    }
  }
  else if (info.port == eATmegaPort::PORT_H) {
    if (info.pinidx == 6) {
      state._COM2B = _TIMER2._TCCRnA._COMnB;

      _TIMER2._TCCRnA._COMnB = 0;
    }
    if (info.pinidx == 5) {
      state._COM4C = TIMER4._TCCRnA._COMnC;

      TIMER4._TCCRnA._COMnC = 0;
    }
    if (info.pinidx == 4) {
      state._COM4B = TIMER4._TCCRnA._COMnB;

      TIMER4._TCCRnA._COMnB = 0;
    }
    if (info.pinidx == 3) {
      state._COM4A = TIMER4._TCCRnA._COMnA;

      TIMER4._TCCRnA._COMnA = 0;
    }
    if (info.pinidx == 2 || info.pinidx == 0) {
      state._USART2_RXEN = USART2._UCSRnB._RXEN;

      USART2._UCSRnB._RXEN = false;
    }
    if (info.pinidx == 2 || info.pinidx == 1) {
      state._USART2_TXEN = USART2._UCSRnB._TXEN;

      USART2._UCSRnB._TXEN = false;
    }
  }
  else if (info.port == eATmegaPort::PORT_J) {
    if (info.pinidx <= 6) {
      state._PCIE1 = _PCICR._PCIE1;

      _PCICR._PCIE1 = false;
    }
    if (info.pinidx == 2 || info.pinidx == 0) {
      state._USART3_RXEN = USART3._UCSRnB._RXEN;

      USART3._UCSRnB._RXEN = false;
    }
    if (info.pinidx == 2 || info.pinidx == 1) {
      state._USART3_TXEN = USART3._UCSRnB._TXEN;

      USART3._UCSRnB._TXEN = false;
    }
  }
  else if (info.port == eATmegaPort::PORT_K) {
    state._PCIE2 = _PCICR._PCIE2;

    _PCICR._PCIE2 = false;
  }
  else if (info.port == eATmegaPort::PORT_L) {
    if (info.pinidx == 5) {
      state._COM5C = TIMER5._TCCRnA._COMnC;

      TIMER5._TCCRnA._COMnC = 0;
    }
    if (info.pinidx == 4) {
      state._COM5B = TIMER5._TCCRnA._COMnB;

      TIMER5._TCCRnA._COMnB = 0;
    }
    if (info.pinidx == 3) {
      state._COM5A = TIMER5._TCCRnA._COMnA;

      TIMER5._TCCRnA._COMnA = 0;
    }
  }
#elif defined(__AVR_TRM02__)
  if (info.port == eATmegaPort::PORT_A) {
    state._PCIE0 = _PCICR._PCIE0;

    _PCICR._PCIE0 = false;

    if (info.pinidx == 7) {
      state._ADCnD = _DIDR0._ADC7D;

      _DIDR0._ADC7D = false;
    }
    else if (info.pinidx == 6) {
      state._ADCnD = _DIDR0._ADC6D;

      _DIDR0._ADC6D = false;
    }
    else if (info.pinidx == 5) {
      state._ADCnD = _DIDR0._ADC5D;

      _DIDR0._ADC5D = false;
    }
    else if (info.pinidx == 4) {
      state._ADCnD = _DIDR0._ADC4D;

      _DIDR0._ADC4D = false;
    }
    else if (info.pinidx == 3) {
      state._ADCnD = _DIDR0._ADC3D;

      _DIDR0._ADC3D = false;
    }
    else if (info.pinidx == 2) {
      state._ADCnD = _DIDR0._ADC2D;

      _DIDR0._ADC2D = false;
    }
    else if (info.pinidx == 1) {
      state._ADCnD = _DIDR0._ADC1D;

      _DIDR0._ADC1D = false;
    }
    else if (info.pinidx == 0) {
      state._ADCnD = _DIDR0._ADC0D;

      _DIDR0._ADC0D = false;
    }
  }
  else if (info.port == eATmegaPort::PORT_B) {
    state._PCIE1 = _PCICR._PCIE1;

    _PCICR._PCIE1 = false;

    if (info.pinidx <= 7 && info.pinidx >= 4) {
      state._SPE = _SPCR._SPE;

      _SPCR._SPE = false;
    }
    
    if (info.pinidx == 4) {
      state._COM0A = TIMER0._TCCRnA._COMnA;

      TIMER0._TCCRnA._COMnA = 0;
    }
    else if (info.pinidx == 5) {
      state._COM0B = TIMER0._TCCRnA._COMnB;

      TIMER0._TCCRnA._COMnB = 0;
    }
  }
  else if (info.port == eATmegaPort::PORT_C) {
    state._AS2 = _ASSR._AS2;
    state._PCIE2 = _PCICR._PCIE2;

    _ASSR._AS2 = false;
    _PCICR._PCIE2 = false;
  }
  else if (info.port == eATmegaPort::PORT_D) {
    state._PCIE3 = _PCICR._PCIE3;

    _PCICR._PCIE3 = false;

    if (info.pinidx == 7) {
      state._COM2A = _TIMER2._TCCRnA._COMnA;

      _TIMER2._TCCRnA._COMnA = 0;
    }
    else if (info.pinidx == 6) {
      state._COM2B = _TIMER2._TCCRnA._COMnB;

      _TIMER2._TCCRnA._COMnB = 0;
    }
    else if (info.pinidx == 5) {
      state._COM1A = TIMER1._TCCRnA._COMnA;

      TIMER1._TCCRnA._COMnA = 0;
    }
    else if (info.pinidx == 4) {
      state._COM1B = TIMER1._TCCRnA._COMnB;

      TIMER1._TCCRnA._COMnB = 0;
    }
    else if (info.pinidx == 3) {
      state._USART1_TXEN = USART1._UCSRnB._TXEN;

      USART1._UCSRnB._TXEN = false;
    }
    else if (info.pinidx == 2 || info.pinidx == 0) {
      state._USART1_RXEN = USART1._UCSRnB._RXEN;

      USART1._UCSRnB._RXEN = false;
    }
    else if (info.pinidx == 1) {
      state._USART0_TXEN = USART0._UCSRnB._TXEN;

      USART0._UCSRnB._TXEN = false;
    }
  }
#elif defined(__AVR_TRM03__)
  if (info.port == eATmegaPort::PORT_B) {
    state._PCIE0 = _PCICR._PCIE0;

    _PCICR._PCIE0 = false;

    if (info.pinidx <= 7 && info.pinidx <= 6) {
      state._AS2 = _ASSR._AS2;

      _ASSR._AS2 = false;
    }
    else if (info.pinidx <= 5 && info.pinidx >= 2) {
      state._SPE = _SPCR._SPE;

      _SPCR._SPE = false;
    }

    if (info.pinidx == 3) {
      state._COM2A = _TIMER2._TCCRnA._COMnA;

      _TIMER2._TCCRnA._COMnA = 0;
    }
    else if (info.pinidx == 2) {
      state._COM1B = TIMER1._TCCRnA._COMnB;

      TIMER1._TCCRnA._COMnB = 0;
    }
    else if (info.pinidx == 1) {
      state._COM1A = TIMER1._TCCRnA._COMnA;

      TIMER1._TCCRnA._COMnA = 0;
    }
  }
  else if (info.port == eATmegaPort::PORT_C) {
    state._PCIE1 = _PCICR._PCIE1;

    _PCICR._PCIE1 = false;

    if (info.pinidx <= 5 && info.pinidx >= 4) {
      state._TWEN = _TWCR._TWEN;

      _TWCR._TWEN = false;
    }

    if (info.pinidx == 5) {
      state._ADCnD = _DIDR0._ADC5D;

      _DIDR0._ADC5D = false;
    }
    else if (info.pinidx == 4) {
      state._ADCnD = _DIDR0._ADC4D;

      _DIDR0._ADC4D = false;
    }
    if (info.pinidx == 3) {
      state._ADCnD = _DIDR0._ADC3D;

      _DIDR0._ADC3D = false;
    }
    if (info.pinidx == 2) {
      state._ADCnD = _DIDR0._ADC2D;

      _DIDR0._ADC2D = false;
    }
    if (info.pinidx == 1) {
      state._ADCnD = _DIDR0._ADC1D;

      _DIDR0._ADC1D = false;
    }
    if (info.pinidx == 0) {
      state._ADCnD = _DIDR0._ADC0D;

      _DIDR0._ADC0D = false;
    }
  }
  else if (info.port == eATmegaPort::PORT_D) {
    if (info.pinidx == 2) {
      state._PCIE1 = _PCICR._PCIE1;

      _PCICR._PCIE1 = false;
    }
    else {
      state._PCIE2 = _PCICR._PCIE2;

      _PCICR._PCIE2 = false;
    }

    if (info.pinidx == 6) {
      state._COM0A = TIMER0._TCCRnA._COMnA;

      TIMER0._TCCRnA._COMnA = 0;
    }
    else if (info.pinidx == 5) {
      state._COM0B = TIMER0._TCCRnA._COMnB;

      TIMER0._TCCRnA._COMnB = 0;
    }
    else if (info.pinidx == 4) {
      state._UMSEL = USART0._UCSRnC._UMSEL;

      USART0._UCSRnC._UMSEL = 0;
    }
    else if (info.pinidx == 3) {
      state._COM2B = _TIMER2._TCCRnA._COMnB;

      _TIMER2._TCCRnA._COMnB = 0;
    }
    else if (info.pinidx == 1) {
      state._USART0_TXEN = USART0._UCSRnB._TXEN;

      USART0._UCSRnB._TXEN = false;
    }
    else if (info.pinidx == 0) {
      state._USART0_RXEN = USART0._UCSRnB._RXEN;

      USART0._UCSRnB._RXEN = false;
    }
  }
#endif

  return state;
}

inline void _ATmega_restorePinAlternate(uint8_t pin, const pin_dev_state_t& state) {
  ATmegaPinInfo info = _ATmega_getPinInfo(pin);

#if defined(__AVR_TRM01__)
  // See page 75ff of ATmega2560 technical reference manual.
  if (info.port == eATmegaPort::PORT_A) {
    _XMCRA._SRE = state._SRE;
  }
  else if (info.port == eATmegaPort::PORT_B) {
    _PCICR._PCIE0 = state._PCIE0;

    if (info.pinidx == 7) {
      TIMER1._TCCRnA._COMnC = state._COM1C;
    }
    else if (info.pinidx == 6) {
      TIMER1._TCCRnA._COMnB = state._COM1B;
    }
    else if (info.pinidx == 5) {
      TIMER1._TCCRnA._COMnA = state._COM1A;
    }
    else if (info.pinidx == 4) {
      _TIMER2._TCCRnA._COMnA = state._COM2A;
    }
    else if (info.pinidx <= 3) {
      _SPCR._SPE = state._SPE;
    }
  }
  else if (info.port == eATmegaPort::PORT_C) {
    _XMCRA._SRE = state._SRE;
  }
  else if (info.port == eATmegaPort::PORT_D) {
    if (info.pinidx == 5 || info.pinidx == 3) {
      USART1._UCSRnB._TXEN = state._USART1_TXEN;
    }
    if (info.pinidx == 5 || info.pinidx == 2) {
      USART1._UCSRnB._RXEN = state._USART1_RXEN;
    }
    if (info.pinidx <= 3) {
      _PCICR._PCIE0 = state._PCIE0;
    }
  }
  else if (info.port == eATmegaPort::PORT_E) {
    if (info.pinidx >= 4 && info.pinidx <= 7) {
      _PCICR._PCIE0 = state._PCIE0;
    }
    if (info.pinidx == 5) {
      TIMER3._TCCRnA._COMnC = state._COM3C;
    }
    if (info.pinidx == 4 || info.pinidx == 3) {
      TIMER3._TCCRnA._COMnB = state._COM3B;
    }
    if (info.pinidx == 2 || info.pinidx == 0) {
      USART0._UCSRnB._RXEN = state._USART0_RXEN;
    }
    if (info.pinidx == 2 || info.pinidx == 1) {
      USART0._UCSRnB._TXEN = state._USART0_TXEN;
    }
    if (info.pinidx == 0) {
      _PCICR._PCIE1 = state._PCIE1;
    }
  }
  // Port F ignored.
  else if (info.port == eATmegaPort::PORT_G) {
    if (info.pinidx == 5) {
      TIMER0._TCCRnA._COMnB = state._COM0B;
    }
    if (info.pinidx == 4 || info.pinidx == 3) {
      _ASSR._AS2 = state._AS2;
    }
    if (info.pinidx <= 2) {
      _XMCRA._SRE = state._SRE;
    }
  }
  else if (info.port == eATmegaPort::PORT_H) {
    if (info.pinidx == 6) {
      _TIMER2._TCCRnA._COMnB = state._COM2B;
    }
    if (info.pinidx == 5) {
      TIMER4._TCCRnA._COMnC = state._COM4C;
    }
    if (info.pinidx == 4) {
      TIMER4._TCCRnA._COMnB = state._COM4B;
    }
    if (info.pinidx == 3) {
      TIMER4._TCCRnA._COMnA = state._COM4A;
    }
    if (info.pinidx == 2 || info.pinidx == 0) {
      USART2._UCSRnB._RXEN = state._USART2_RXEN;
    }
    if (info.pinidx == 2 || info.pinidx == 1) {
      USART2._UCSRnB._TXEN = state._USART2_TXEN;
    }
  }
  else if (info.port == eATmegaPort::PORT_J) {
    if (info.pinidx <= 6) {
      _PCICR._PCIE1 = state._PCIE1;
    }
    if (info.pinidx == 2 || info.pinidx == 0) {
      USART3._UCSRnB._RXEN = state._USART3_RXEN;
    }
    if (info.pinidx == 2 || info.pinidx == 1) {
      USART3._UCSRnB._TXEN = state._USART3_TXEN;
    }
  }
  else if (info.port == eATmegaPort::PORT_K) {
    _PCICR._PCIE2 = state._PCIE2;
  }
  else if (info.port == eATmegaPort::PORT_L) {
    if (info.pinidx == 5) {
      TIMER5._TCCRnA._COMnC = state._COM5C;
    }
    if (info.pinidx == 4) {
      TIMER5._TCCRnA._COMnB = state._COM5B;
    }
    if (info.pinidx == 3) {
      TIMER5._TCCRnA._COMnA = state._COM5A;
    }
  }
#elif defined(__AVR_TRM02__)
  if (info.port == eATmegaPort::PORT_A) {
    _PCICR._PCIE0 = state._PCIE0;

    if (info.pinidx == 7) {
      _DIDR0._ADC7D = state._ADCnD;
    }
    else if (info.pinidx == 6) {
      _DIDR0._ADC6D = state._ADCnD;
    }
    else if (info.pinidx == 5) {
      _DIDR0._ADC5D = state._ADCnD;
    }
    else if (info.pinidx == 4) {
      _DIDR0._ADC4D = state._ADCnD;
    }
    else if (info.pinidx == 3) {
      _DIDR0._ADC3D = state._ADCnD;
    }
    else if (info.pinidx == 2) {
      _DIDR0._ADC2D = state._ADCnD;
    }
    else if (info.pinidx == 1) {
      _DIDR0._ADC1D = state._ADCnD;
    }
    else if (info.pinidx == 0) {
      _DIDR0._ADC0D = state._ADCnD;
    }
  }
  else if (info.port == eATmegaPort::PORT_B) {
    _PCICR._PCIE1 = state._PCIE1;

    if (info.pinidx <= 7 && info.pinidx >= 4) {
      _SPCR._SPE = state._SPE;
    }
    
    if (info.pinidx == 4) {
      TIMER0._TCCRnA._COMnA = state._COM0A;
    }
    else if (info.pinidx == 5) {
      TIMER0._TCCRnA._COMnB = state._COM0B;
    }
  }
  else if (info.port == eATmegaPort::PORT_C) {
    _ASSR._AS2 = state._AS2;
    _PCICR._PCIE2 = state._PCIE2;
  }
  else if (info.port == eATmegaPort::PORT_D) {
    _PCICR._PCIE3 = state._PCIE3;

    if (info.pinidx == 7) {
      _TIMER2._TCCRnA._COMnA = state._COM2A;
    }
    else if (info.pinidx == 6) {
      _TIMER2._TCCRnA._COMnB = state._COM2B;
    }
    else if (info.pinidx == 5) {
      TIMER1._TCCRnA._COMnA = state._COM1A;
    }
    else if (info.pinidx == 4) {
      TIMER1._TCCRnA._COMnB = state._COM1B;
    }
    else if (info.pinidx == 3) {
      USART1._UCSRnB._TXEN = state._USART1_TXEN;
    }
    else if (info.pinidx == 2 || info.pinidx == 0) {
      USART1._UCSRnB._RXEN = state._USART1_RXEN;
    }
    else if (info.pinidx == 1) {
      USART0._UCSRnB._TXEN = state._USART0_TXEN;
    }
  }
#elif defined(__AVR_TRM03__)
  if (info.port == eATmegaPort::PORT_B) {
    _PCICR._PCIE0 = state._PCIE0;

    if (info.pinidx <= 7 && info.pinidx <= 6) {
      _ASSR._AS2 = state._AS2;
    }
    else if (info.pinidx <= 5 && info.pinidx >= 2) {
      _SPCR._SPE = state._SPE;
    }

    if (info.pinidx == 3) {
      _TIMER2._TCCRnA._COMnA = state._COM2A;
    }
    else if (info.pinidx == 2) {
      TIMER1._TCCRnA._COMnB = state._COM1B;
    }
    else if (info.pinidx == 1) {
      TIMER1._TCCRnA._COMnA = state._COM1A;
    }
  }
  else if (info.port == eATmegaPort::PORT_C) {
    _PCICR._PCIE1 = state._PCIE1;

    if (info.pinidx <= 5 && info.pinidx >= 4) {
      _TWCR._TWEN = state._TWEN;
    }

    if (info.pinidx == 5) {
      _DIDR0._ADC5D = state._ADCnD;
    }
    else if (info.pinidx == 4) {
      _DIDR0._ADC4D = state._ADCnD;
    }
    if (info.pinidx == 3) {
      _DIDR0._ADC3D = state._ADCnD;
    }
    if (info.pinidx == 2) {
      _DIDR0._ADC2D = state._ADCnD;
    }
    if (info.pinidx == 1) {
      _DIDR0._ADC1D = state._ADCnD;
    }
    if (info.pinidx == 0) {
      _DIDR0._ADC0D = state._ADCnD;
    }
  }
  else if (info.port == eATmegaPort::PORT_D) {
    if (info.pinidx == 2) {
      _PCICR._PCIE1 = state._PCIE1;
    }
    else {
      _PCICR._PCIE2 = state._PCIE2;
    }

    if (info.pinidx == 6) {
      TIMER0._TCCRnA._COMnA = state._COM0A;
    }
    else if (info.pinidx == 5) {
      TIMER0._TCCRnA._COMnB = state._COM0B;
    }
    else if (info.pinidx == 4) {
      USART0._UCSRnC._UMSEL = state._UMSEL;
    }
    else if (info.pinidx == 3) {
      _TIMER2._TCCRnA._COMnB = state._COM2B;
    }
    else if (info.pinidx == 1) {
      USART0._UCSRnB._TXEN = state._USART0_TXEN;
    }
    else if (info.pinidx == 0) {
      USART0._UCSRnB._RXEN = state._USART0_RXEN;
    }
  }
#endif
}