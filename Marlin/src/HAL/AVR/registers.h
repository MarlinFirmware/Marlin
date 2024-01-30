/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include <stdint.h>

// This volatile-nonsense has to be done due to the C++ platform language specialization that specifies, for it's own compiler ideology,
// that memory writes and reads can be optimized across easily-reachable code spaces. This is in contrast to MSVC which specifies that
// memory writes and reads are holy.

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

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
  #error "Fatal error: __AVR_TRMn__ already defined! (n: 01|02|03|04|05)"
#endif

#if defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || defined(__AVR_ATmega640__)
  // ATmega2560 technical reference manual date: 28th of November, 2022
  // ATmega640-1280-1281-2560-2561-Datasheet-DS40002211A.pdf
  #define __AVR_TRM01__
#elif defined(__AVR_ATmega164A__) || defined(__AVR_ATmega164PA__) || defined(__AVR_ATmega324A__) || defined(__AVR_ATmega324PA__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644PA__) || defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__)
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
#elif defined(__AVR_ATmega164P__) || defined(__AVR_ATmega164V__) || defined(__AVR_ATmega324P__) || defined(__AVR_ATmega324V__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644V__)
  // ATmega644P technical reference manual date: 14th of February, 2023
  // ATmega164P-324P-644P-Data-Sheet-40002071A.pdf
  #define __AVR_TRM05__
#endif

/**
 * HELPER FUNCTIONS
 */
namespace AVRHelpers {

  template <typename T>
  struct no_volatile {
    typedef T type;
  };

  template <typename T>
  struct no_volatile <volatile T> : public no_volatile <T> {};

  template <typename T, unsigned int>
  struct voltype {
    typedef T type;
  };
  template <typename T>
  struct voltype <T, 1u> {
    typedef uint8_t type;
  };
  template <typename T>
  struct voltype <T, 2u> {
    typedef uint16_t type;
  };
  template <typename T>
  struct voltype <T, 4u> {
    typedef uint32_t type;
  };

  template <typename T>
  inline void dwrite(volatile T& v, const T& V) noexcept {
    (volatile typename voltype <T, sizeof(T)>::type&)v = (const typename voltype <T, sizeof(T)>::type&)V;
  }

} // namespace AVRHelpers

// As old as the ATmega series of CPU is, the worse the actual libraries making
// use of the MCU likely are.

// These registers as references do not take program space since they are purely references.

// It would be great if the old AVR definitions could be wasted in favor of these
// and code be rewritten to use the following more robust definitions.

struct _bit_reg_t {
  uint8_t val;

  bool getValue(uint8_t idx) const volatile {
    return ( val & (1 << idx) );
  }
  void setValue(uint8_t idx, bool value) volatile {
    if (value)
      val |= (1 << idx);
    else
      val &= ~(1 << idx);
  }
};

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)

  typedef _bit_reg_t PIN_reg_t;
  typedef _bit_reg_t DDR_reg_t;
  typedef _bit_reg_t PORT_reg_t;

  struct PORT_dev_t {
    PIN_reg_t _PIN;
    DDR_reg_t _DDR;
    PORT_reg_t _PORT;

    inline void operator = ( const PORT_dev_t& r ) volatile {
      using namespace AVRHelpers;
      dwrite(this->_PIN, r._PIN);
      dwrite(this->_DDR, r._DDR);
      dwrite(this->_PORT, r._PORT);
    }
  };
  static_assert(sizeof(PORT_dev_t) == 3, "invalid size of ATmega2560 GPIO_dev_t");

#endif // __AVR_TRM01__ || __AVR_TRM02__ || __AVR_TRM03__ || __AVR_TRM04__ || __AVR_TRM05__

#ifdef __AVR_TRM01__

  struct _bitG_reg_t {
    uint8_t val : 6;
    uint8_t reserved1 : 2;

    bool getValue(uint8_t idx) const volatile {
      return val & (1 << idx);
    }
    void setValue(uint8_t idx, bool value) volatile {
      if (value)
        val |= (1 << idx);
      else
        val &= ~(1 << idx);
    }
  };
  typedef _bitG_reg_t PING_reg_t;
  typedef _bitG_reg_t DDRG_reg_t;
  typedef _bitG_reg_t PORTG_reg_t;

  struct PORTG_dev_t {
    PING_reg_t _PIN;
    DDRG_reg_t _DDR;
    PORTG_reg_t _PORT;

    inline void operator = ( const PORTG_dev_t& r ) volatile {
      using namespace AVRHelpers;
      dwrite(this->_PIN, r._PIN);
      dwrite(this->_DDR, r._DDR);
      dwrite(this->_PORT, r._PORT);
    }
  };

#endif

#ifdef __AVR_TRM03__

  struct _bitC_reg_t {
    uint8_t val : 7;
    uint8_t reserved1 : 1;

    bool getValue(uint8_t idx) const volatile {
      return ( val & (1 << idx) );
    }
    void setValue(uint8_t idx, bool value) volatile {
      if (value)
        val |= (1 << idx);
      else
        val &= ~(1 << idx);
    }
  };
  typedef _bitC_reg_t PINC_reg_t;
  typedef _bitC_reg_t DDRC_reg_t;
  typedef _bitC_reg_t PORTC_reg_t;

  struct PORTC_dev_t {
    PINC_reg_t _PIN;
    DDRC_reg_t _DDR;
    PORTC_reg_t _PORT;

    inline void operator = ( const PORTC_dev_t& r ) volatile {
      this->_PIN = r._PIN;
      this->_DDR = r._DDR;
      this->_PORT = r._PORT;
    }
  };

#endif

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)

  struct TIFR0_reg_t {
    uint8_t _TOV0 : 1;
    uint8_t _OCF0A : 1;
    uint8_t _OCF0B : 1;
    uint8_t reserved1 : 5;
  };
  static_assert(sizeof(TIFR0_reg_t) == 1, "invalid size of ATmega2560 TIFR0_reg_t");

  struct TIFR1_reg_t {
    #if defined(__AVR_TRM01__) || defined(__AVR_TRM04__)
      uint8_t _TOV1 : 1;
      uint8_t _OCF1A : 1;
      uint8_t _OCF1B : 1;
      uint8_t _OCF1C : 1;
      uint8_t reserved1 : 1;
      uint8_t _ICF1 : 1;
      uint8_t reserved2 : 2;
    #elif defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM05__)
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

#endif // __AVR_TRM01__ || __AVR_TRM02__ || __AVR_TRM03__ || __AVR_TRM04__ || __AVR_TRM05__

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM04__)

  struct TIFR3_reg_t {
    #if defined(__AVR_TRM01__) || defined(__AVR_TRM04__)
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

#endif // __AVR_TRM01__ || __AVR_TRM02__ || __AVR_TRM04__

#ifdef __AVR_TRM01__

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

#endif // __AVR_TRM01__

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)

  struct PCIFR_reg_t {
    #if defined(__AVR_TRM01__) || defined(__AVR_TRM03__)
      uint8_t _PCIF0 : 1;
      uint8_t _PCIF1 : 1;
      uint8_t _PCIF2 : 1;
      uint8_t reserved1 : 5;
    #elif defined(__AVR_TRM02__) || defined(__AVR_TRM05__)
      uint8_t _PCIF0 : 1;
      uint8_t _PCIF1 : 1;
      uint8_t _PCIF2 : 1;
      uint8_t _PCIF3 : 1;
      uint8_t reserved1 : 4;
    #elif defined(__AVR_TRM04__)
      uint8_t _PCIF0 : 1;
      uint8_t reserved1 : 7;
    #endif
  };
  static_assert(sizeof(PCIFR_reg_t) == 1, "invalid size of ATmega2560 PCIFR_reg_t");

  struct EIFR_reg_t {
    #if defined(__AVR_TRM01__) || defined(__AVR_TRM04__)
      uint8_t _INTF0 : 1;
      uint8_t _INTF1 : 1;
      uint8_t _INTF2 : 1;
      uint8_t _INTF3 : 1;
      uint8_t _INTF4 : 1;
      uint8_t _INTF5 : 1;
      uint8_t _INTF6 : 1;
      uint8_t _INTF7 : 1;
    #elif defined(__AVR_TRM02__) || defined(__AVR_TRM05__)
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
    #if defined(__AVR_TRM01__) || defined(__AVR_TRM04__)
      uint8_t _INT0 : 1;
      uint8_t _INT1 : 1;
      uint8_t _INT2 : 1;
      uint8_t _INT3 : 1;
      uint8_t _INT4 : 1;
      uint8_t _INT5 : 1;
      uint8_t _INT6 : 1;
      uint8_t _INT7 : 1;
    #elif defined(__AVR_TRM02__) || defined(__AVR_TRM05__)
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
    #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
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

  struct SMCR_reg_t {
    uint8_t _SE : 1;
    uint8_t _SM : 3;
    uint8_t reserved1 : 4;
  };
  static_assert(sizeof(SMCR_reg_t) == 1, "invalid size of ATmega2560 SMCR_reg_t");

  struct MCUSR_reg_t {
    #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
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
    #if defined(__AVR_TRM01__) || defined(__AVR_TRM04__)
      uint8_t _IVCE : 1;
      uint8_t _IVSEL : 1;
      uint8_t reserved1 : 2;
      uint8_t _PUD : 1;
      uint8_t reserved2 : 2;
      uint8_t _JTD : 1;
    #elif defined(__AVR_TRM02__) || defined(__AVR_TRM05__)
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
    #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
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

#endif // __AVR_TRM01__ || __AVR_TRM02__ || __AVR_TRM03__ || __AVR_TRM04__

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)

  struct RAMPZ_reg_t {
    #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM04__)
      uint8_t _RAMPZ : 2;
      uint8_t reserved1 : 6;
    #elif defined(__AVR_TRM05__)
      uint8_t _RAMPZ : 1;
      uint8_t reserved1 : 7;
    #endif
  };
  static_assert(sizeof(RAMPZ_reg_t) == 1, "invalid size of ATmega2560 RAMPZ_reg_t");

#endif // __AVR_TRM01__ || __AVR_TRM02__ || __AVR_TRM04__ || __AVR_TRM05__

#ifdef __AVR_TRM01__

  struct EIND_reg_t {
    uint8_t _EIND0 : 1;
    uint8_t reserved1 : 7;
  };
  static_assert(sizeof(EIND_reg_t) == 1, "invalid size of ATmega2560 EIND_reg_t");

#endif // __AVR_TRM01__

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)

  struct SP_reg_t {
    #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
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

  struct PRR0_reg_t {
    #ifdef __AVR_TRM01__
      uint8_t _PRADC : 1;
      uint8_t _PRUSART0 : 1;
      uint8_t _PRSPI : 1;
      uint8_t _PRTIM1 : 1;
      uint8_t reserved1 : 1;
      uint8_t _PRTIM0 : 1;
      uint8_t _PRTIM2 : 1;
      uint8_t _PRTWI : 1;
    #elif defined(__AVR_TRM02__) || defined(__AVR_TRM05__)
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
    #elif defined(__AVR_TRM04__)
      uint8_t _PRADC : 1;
      uint8_t reserved1 : 1;
      uint8_t _PRSPI : 1;
      uint8_t _PRTIM1 : 1;
      uint8_t reserved2 : 1;
      uint8_t _PRTIM0 : 1;
      uint8_t _PRTIM2 : 1;
      uint8_t _PRTWI : 1;
    #endif
  };
  static_assert(sizeof(PRR0_reg_t) == 1, "invalid size of ATmega2560 PRR0_reg_t");

#endif // __AVR_TRM01__ || __AVR_TRM02__ || __AVR_TRM03__ || __AVR_TRM04__ || __AVR_TRM05__

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM04__)

  struct PRR1_reg_t {
    #ifdef __AVR_TRM01__
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
    #elif defined(__AVR_TRM04__)
      uint8_t _PRUSART1 : 1;
      uint8_t reserved1 : 2;
      uint8_t _PRTIM3 : 1;
      uint8_t reserved2 : 3;
      uint8_t _PRUSB : 1;
    #endif
  };
  static_assert(sizeof(PRR1_reg_t) == 1, "invalid size of ATmega2560 PRR1_reg_t");

#endif // __AVR_TRM01__ || __AVR_TRM02__ || __AVR_TRM04__

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)

  struct PCICR_reg_t {
    #if defined(__AVR_TRM01__) || defined(__AVR_TRM03__)
      uint8_t _PCIE0 : 1;
      uint8_t _PCIE1 : 1;
      uint8_t _PCIE2 : 1;
      uint8_t reserved1 : 5;
    #elif defined(__AVR_TRM02__) || defined(__AVR_TRM05__)
      uint8_t _PCIE0 : 1;
      uint8_t _PCIE1 : 1;
      uint8_t _PCIE2 : 1;
      uint8_t _PCIE3 : 1;
      uint8_t reserved1 : 4;
    #elif defined(__AVR_TRM04__)
      uint8_t _PCIE0 : 1;
      uint8_t reserved1 : 7;
    #endif
  };
  static_assert(sizeof(PCICR_reg_t) == 1, "invalid size of ATmega2560 PCICR_reg_t");

  struct EICRA_reg_t {
    #if defined(__AVR_TRM01__) || defined(__AVR_TRM04__)
      uint8_t _ISC0 : 2;
      uint8_t _ISC1 : 2;
      uint8_t _ISC2 : 2;
      uint8_t _ISC3 : 2;
    #elif defined(__AVR_TRM02__) || defined(__AVR_TRM05__)
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

#endif // __AVR_TRM01__ || __AVR_TRM02__ || __AVR_TRM03__ || __AVR_TRM04__ || __AVR_TRM05__

#if defined(__AVR_TRM01__) || defined(__AVR_TRM04__)

  struct EICRB_reg_t {
    uint8_t _ISC4 : 2;
    uint8_t _ISC5 : 2;
    uint8_t _ISC6 : 2;
    uint8_t _ISC7 : 2;
  };
  static_assert(sizeof(EICRB_reg_t) == 1, "invalid size of ATmega2560 EICRB_reg_t");

#endif // __AVR_TRM01__ || __AVR_TRM04__

#if defined(__AVR_TRM03__)

  struct _bitPCMSK1_reg_t {
    uint8_t val : 7;
    uint8_t reserved1 : 1;

    bool getValue(uint8_t idx) { return val & (1 << idx); }
    void setValue(uint8_t idx, bool value) {
      if (value)
        val |= (1 << idx);
      else
        val &= ~(1 << idx);
    }
  };

#endif // __AVR_TRM03__

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)

  struct TIMSK0_reg_t {
    #ifdef __AVR_TRM01__
      uint8_t _TOIE0 : 1;
      uint8_t _OCIE0A : 1;
      uint8_t _OCIE0B : 1;
      uint8_t _OCIE0C : 1;
      uint8_t reserved1 : 1;
      uint8_t _ICIE0 : 1;
      uint8_t reserved2 : 2;
    #elif defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
      uint8_t _TOIE0 : 1;
      uint8_t _OCIE0A : 1;
      uint8_t _OCIE0B : 1;
      uint8_t reserved1 : 5;
    #endif
  };
  static_assert(sizeof(TIMSK0_reg_t) == 1, "invalid size of ATmega2560 TIMSK0_reg_t");

  struct TIMSK1_reg_t {
    #if defined(__AVR_TRM01__) || defined(__AVR_TRM04__)
      uint8_t _TOIE1 : 1;
      uint8_t _OCIE1A : 1;
      uint8_t _OCIE1B : 1;
      uint8_t _OCIE1C : 1;
      uint8_t reserved1 : 1;
      uint8_t _ICIE1: 1;
      uint8_t reserved2 : 2;
    #elif defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM05__)
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

#endif // __AVR_TRM01__ || __AVR_TRM02__ || __AVR_TRM03__ || __AVR_TRM04__ || __AVR_TRM05__

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM04__)

  struct TIMSK3_reg_t {
    #if defined(__AVR_TRM01__) || defined(__AVR_TRM04__)
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

#endif // __AVR_TRM01__ || __AVR_TRM02__ || __AVR_TRM04__

#ifdef __AVR_TRM01__

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

#endif // __AVR_TRM01__

#if defined(__AVR_TRM01__) || defined(__AVR_TRM04__)

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

#endif // __AVR_TRM01__ || __AVR_TRM04__

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)

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
    #if defined(__AVR_TRM01__) || defined(__AVR_TRM04__)
      uint8_t _ADTS : 3;
      uint8_t _MUX5 : 1;
      uint8_t reserved1 : 2;
      uint8_t _ACME : 1;
      uint8_t reserved2 : 1;
    #elif defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM05__)
      uint8_t _ADTS : 3;
      uint8_t reserved1 : 3;
      uint8_t _ACME : 1;
      uint8_t reserved2 : 1;
    #endif
  };
  static_assert(sizeof(ADCSRB_reg_t) == 1, "invalid size of ATmega2560 ADCSRB_reg_t");

  struct ADMUX_reg_t {
    #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
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

#endif // __AVR_TRM01__ || __AVR_TRM02__ || __AVR_TRM03__ || __AVR_TRM04__

#ifdef __AVR_TRM01__

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

#endif // __AVR_TRM01__

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)

  struct DIDR0_reg_t {
    #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
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

  struct TCCRnA_reg_t {
    #if defined(__AVR_TRM01__) || defined(__AVR_TRM04__)
      uint8_t _WGMn0 : 1;
      uint8_t _WGMn1 : 1;
      uint8_t _COMnC : 2;
      uint8_t _COMnB : 2;
      uint8_t _COMnA : 2;
    #elif defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM05__)
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
    #if defined(__AVR_TRM01__) || defined(__AVR_TRM04__)
      uint8_t reserved1 : 5;
      uint8_t _FOCnC : 1;
      uint8_t _FOCnB : 1;
      uint8_t _FOCnA : 1;
    #elif defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM05__)
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
    #if defined(__AVR_TRM01__) || defined(__AVR_TRM04__)
      uint16_t _OCRnC;
    #endif

    inline void operator = ( const TIMER_dev_t& r ) volatile {
      using namespace AVRHelpers;
      dwrite(this->_TCCRnA, r._TCCRnA);
      dwrite(this->_TCCRnB, r._TCCRnB);
      dwrite(this->_TCCRnC, r._TCCRnC);
      this->reserved1 = r.reserved1;
      this->_TCNTn = r._TCNTn;
      this->_ICRn = r._ICRn;
      this->_OCRnA = r._OCRnA;
      this->_OCRnB = r._OCRnB;
      #if defined(__AVR_TRM01__) || defined(__AVR_TRM04__)
        this->_OCRnC = r._OCRnC;
      #endif
    }
  };
  #if defined(__AVR_TRM01__) || defined(__AVR_TRM04__)
    static_assert(sizeof(TIMER_dev_t) == 14, "invalid size of ATmega2560 TIMER_dev_t");
  #elif defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM05__)
    static_assert(sizeof(TIMER_dev_t) == 12, "invalid size of ATmega1284 TIMER_dev_t");
  #endif

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

    inline void operator = ( const TIMER_8bit_dev_t& r ) volatile {
      using namespace AVRHelpers;
      dwrite(this->_TCCRnA, r._TCCRnA);
      dwrite(this->_TCCRnB, r._TCCRnB);
      this->_TCNTn = r._TCNTn;
      this->_OCRnA = r._OCRnA;
      this->_OCRnB = r._OCRnB;
    }
  };
  static_assert(sizeof(TIMER_8bit_dev_t) == 5, "invalid size of ATmega2560 TIMER_8bit_dev_t");

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

  struct UBRRn_reg_t {
    uint16_t _UBRR : 12;
    uint16_t reserved1 : 4;
  };
  static_assert(sizeof(UBRRn_reg_t) == 2, "invalid size of ATmega2560 UBRRn_reg_t)");

  struct UCSRnC_reg_t {
    #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__)
      uint8_t _UCPOL : 1;
      uint8_t _UCSZn0 : 1;
      uint8_t _UCSZn1 : 1;
      uint8_t _USBS : 1;
      uint8_t _UPM : 2;
      uint8_t _UMSEL : 2;
    #elif defined(__AVR_TRM05__)
      uint8_t _UCPOL : 1;
      uint8_t _UCPHA : 1;
      uint8_t _UDORD : 1;
      uint8_t reserved1 : 3;
      uint8_t _UMSEL : 2;
    #endif
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

    inline void operator = ( const USART_dev_t& r ) volatile {
      using namespace AVRHelpers;
      dwrite(this->_UCSRnA, r._UCSRnA);
      dwrite(this->_UCSRnB, r._UCSRnB);
      dwrite(this->_UCSRnC, r._UCSRnC);
      dwrite(this->reserved1, r.reserved1);
      dwrite(this->_UBRRn, r._UBRRn);
      dwrite(this->_UDRn, r._UDRn);
    }
  };
  static_assert(sizeof(USART_dev_t) == 7, "invalid size of ATmega2560 USART_dev_t");

#endif // __AVR_TRM01__ || __AVR_TRM02__ || __AVR_TRM03__ || __AVR_TRM04__

#ifdef __AVR_TRM04__

  struct UHCON_reg_t {
    uint8_t _SOFEN : 1;
    uint8_t _RESET : 1;
    uint8_t _RESUME : 1;
    uint8_t reserved1 : 5;
  };
  static_assert(sizeof(UHCON_reg_t) == 1, "invalid size of ATUSB90 UHCON_reg_t");

  struct UHINT_reg_t {
    uint8_t _DCONNI : 1;
    uint8_t _DDISCI : 1;
    uint8_t _RSTI : 1;
    uint8_t _RSMEDI : 1;
    uint8_t _RXRSMI : 1;
    uint8_t _HSOFI : 1;
    uint8_t _HWUPI : 1;
    uint8_t reserved1 : 1;
  };
  static_assert(sizeof(UHINT_reg_t) == 1, "invalid size of ATUSB90 UHINT_reg_t");

  struct UHIEN_reg_t {
    uint8_t _SUSPE : 1;
    uint8_t _MSOFE : 1;
    uint8_t _SOFE : 1;
    uint8_t _EORSTE : 1;
    uint8_t _WAKEUPE : 1;
    uint8_t _EORSME : 1;
    uint8_t _UPRSME : 1;
    uint8_t reserved1 : 1;
  };
  static_assert(sizeof(UHIEN_reg_t) == 1, "invalid size of ATUSB90 UHIEN_reg_t");

  struct UHADDR_reg_t {
    uint8_t _HADD : 7;
    uint8_t reserved1 : 1;
  };
  static_assert(sizeof(UHADDR_reg_t) == 1, "invalid size of ATUSB90 UHADDR_reg_t");

  struct UHFNUM_reg_t {
    uint16_t _FNUM : 11;
    uint16_t reserved1 : 5;
  };
  static_assert(sizeof(UHFNUM_reg_t) == 2, "invalid size of ATUSB90 UHFNUM_reg_t");

  struct UPINTX_reg_t {
    uint8_t _RXINI : 1;
    uint8_t _RXSTALLI : 1;
    uint8_t _TXOUTI : 1;
    uint8_t _TXSTPI : 1;
    uint8_t _PERRI : 1;
    uint8_t _RWAL : 1;
    uint8_t _NAKEDI : 1;
    uint8_t _FIFOCON : 1;
  };
  static_assert(sizeof(UPINTX_reg_t) == 1, "invalid size of ATUSB90 UPINTX_reg_t");

  struct UPNUM_reg_t {
    uint8_t _PNUM : 3;
    uint8_t reserved1 : 5;
  };
  static_assert(sizeof(UPNUM_reg_t) == 1, "invalid size of ATUSB90 UPNUM_reg_t");

  struct UPRST_reg_t {
    uint8_t _PRST : 7;
    uint8_t reserved1 : 1;
  };
  static_assert(sizeof(UPRST_reg_t) == 1, "invalid size of ATUSB90 UPRST_reg_t");

  struct UPCONX_reg_t {
    uint8_t _PEN : 1;
    uint8_t reserved1 : 2;
    uint8_t _RSTDT : 1;
    uint8_t _AUTOSW : 1;
    uint8_t _INMODE : 1;
    uint8_t _PFREEZE : 1;
    uint8_t reserved2 : 1;
  };
  static_assert(sizeof(UPCONX_reg_t) == 1, "invalid size of ATUSB90 UPCONX_reg_t");

  struct UPCFG0X_reg_t {
    uint8_t _PEPNUM : 4;
    uint8_t _PTOKEN : 2;
    uint8_t _PTYPE : 2;
  };
  static_assert(sizeof(UPCFG0X_reg_t) == 1, "invalid size of ATUSB90 UPCFG0_reg_t");

  struct UPCFG1X_reg_t {
    uint8_t reserved1 : 1;
    uint8_t _ALLOC : 1;
    uint8_t _PBK : 2;
    uint8_t _PSIZE : 3;
    uint8_t reserved2 : 1;
  };
  static_assert(sizeof(UPCFG1X_reg_t) == 1, "invalid size of ATUSB90 UPCFG1X_reg_t");

  struct UPSTAX_reg_t {
    uint8_t _NBUSYBK : 2;
    uint8_t _DTSEQ : 2;
    uint8_t reserved1 : 1;
    uint8_t _UNDERFI : 1;
    uint8_t _OVERFI : 1;
    uint8_t _CFGOK : 1;
  };
  static_assert(sizeof(UPSTAX_reg_t) == 1, "invalid size of ATUSB90 UPSTAX_reg_t");

  struct UPIENX_reg_t {
    uint8_t _RXINE : 1;
    uint8_t _RXSTALLE : 1;
    uint8_t _TXOUTE : 1;
    uint8_t _TXSTPE : 1;
    uint8_t _PERRE : 1;
    uint8_t reserved1 : 1;
    uint8_t _NAKEDE : 1;
    uint8_t _FLERRE : 1;
  };
  static_assert(sizeof(UPIENX_reg_t) == 1, "invalid size of ATUSB90 UPIENX_reg_t");

  struct UHWCON_reg_t {
    uint8_t _UVREGE : 1;
    uint8_t reserved1 : 3;
    uint8_t _UVCONE : 1;
    uint8_t reserved2 : 1;
    uint8_t _UIDE : 1;
    uint8_t _UIMOD : 1;
  };
  static_assert(sizeof(UHWCON_reg_t) == 1, "invalid size of ATUSB90 UHWCON_reg_t");

  struct USBCON_reg_t {
    uint8_t _VBUSTE : 1;
    uint8_t _IDTE : 1;
    uint8_t reserved1 : 2;
    uint8_t _OTGPADE : 1;
    uint8_t _FRZCLK : 1;
    uint8_t _HOST : 1;
    uint8_t _USBE : 1;
  };
  static_assert(sizeof(USBCON_reg_t) == 1, "invalid size of ATUSB90 USBCON_reg_t");

  struct USBSTA_reg_t {
    uint8_t _VBUS : 1;
    uint8_t _ID : 1;
    uint8_t reserved1 : 1;
    uint8_t _SPEED : 1;
    uint8_t reserved2 : 4;
  };
  static_assert(sizeof(USBSTA_reg_t) == 1, "invalid size of ATUSB90 USBSTA_reg_t");

  struct USBINT_reg_t {
    uint8_t _VBUSTI : 1;
    uint8_t _IDTI : 1;
    uint8_t reserved1 : 6;
  };
  static_assert(sizeof(USBINT_reg_t) == 1, "invalid size of ATUSB90 USBINT_reg_t");

  struct UDPADD_reg_t {
    uint16_t _DPADD : 11;
    uint16_t reserved1 : 4;
    uint16_t _DPACC : 1;
  };
  static_assert(sizeof(UDPADD_reg_t) == 2, "invalid size of ATUSB90 UDPADD_reg_t");

  struct OTGCON_reg_t {
    uint8_t _VBUSRQC : 1;
    uint8_t _VBUSREQ : 1;
    uint8_t _VBUSHWC : 1;
    uint8_t _SRPSEL : 1;
    uint8_t _SRPREQ : 1;
    uint8_t _HNPREQ : 1;
    uint8_t reserved1 : 1;
    uint8_t _zero : 1;
  };
  static_assert(sizeof(OTGCON_reg_t) == 1, "invalid size of ATUSB90 OTGCON_reg_t");

  struct OTGIEN_reg_t {
    uint8_t _SRPE : 1;
    uint8_t _VBERRE : 1;
    uint8_t _BCERRE : 1;
    uint8_t _ROLEEXE : 1;
    uint8_t _HNPERRE : 1;
    uint8_t _STOE : 1;
    uint8_t reserved1 : 2;
  };
  static_assert(sizeof(OTGIEN_reg_t) == 1, "invalid size of ATUSB90 OTGIEN_reg_t");

  struct OTGINT_reg_t {
    uint8_t _SRPI : 1;
    uint8_t _VBERRI : 1;
    uint8_t _BCERRI : 1;
    uint8_t _ROLEEXI : 1;
    uint8_t _HNPERRI : 1;
    uint8_t _STOI : 1;
    uint8_t reserved1 : 2;
  };
  static_assert(sizeof(OTGINT_reg_t) == 1, "invalid size of ATUSB90 OTGINT_reg_t");

  struct UDCON_reg_t {
    uint8_t _DETACH : 1;
    uint8_t _RMWKUP : 1;
    uint8_t _LSM : 1;
    uint8_t reserved1 : 5;
  };
  static_assert(sizeof(UDCON_reg_t) == 1, "invalid size of ATUSB90 UDCON_reg_t");

  struct UDINT_reg_t {
    uint8_t _SUSPI : 1;
    uint8_t _MSOFI : 1;
    uint8_t _SOFI : 1;
    uint8_t _EORSTI : 1;
    uint8_t _WAKEUPI : 1;
    uint8_t _EORSMI : 1;
    uint8_t _UPRSMI : 1;
    uint8_t reserved1 : 1;
  };
  static_assert(sizeof(UDINT_reg_t) == 1, "invalid size of ATUSB90 UDINT_reg_t");

  struct UDIEN_reg_t {
    uint8_t _SUSPE : 1;
    uint8_t _MSOFE : 1;
    uint8_t _SOFE : 1;
    uint8_t _EORSTE : 1;
    uint8_t _WAKEUPE : 1;
    uint8_t _EORSME : 1;
    uint8_t _UPRSME : 1;
    uint8_t reserved1 : 1;
  };
  static_assert(sizeof(UDIEN_reg_t) == 1, "invalid size of ATUSB90 UDIEN_reg_t");

  struct UDADDR_reg_t {
    uint8_t _UADD : 7;
    uint8_t _ADDEN : 1;
  };
  static_assert(sizeof(UDADDR_reg_t) == 1, "invalid size of ATUSB90 UADDR_reg_t");

  struct UDFNUM_reg_t {
    uint16_t _FNUM : 11;
    uint16_t reserved1 : 5;
  };
  static_assert(sizeof(UDFNUM_reg_t) == 2, "invalid size of ATUSB90 UDFNUM_reg_t");

  struct UDMFN_reg_t {
    uint8_t reserved1 : 4;
    uint8_t _FNCERR : 1;
    uint8_t reserved2 : 3;
  };
  static_assert(sizeof(UDMFN_reg_t) == 1, "invalid size of ATUSB90 UDMFN_reg_t");

  struct UDTST_reg_t {
    uint8_t reserved1 : 2;
    uint8_t _TSTJ : 1;
    uint8_t _TSTK : 1;
    uint8_t _TSTPCKT : 1;
    uint8_t _OPMODE2 : 1;
    uint8_t reserved2 : 2;
  };
  static_assert(sizeof(UDTST_reg_t) == 1, "invalid size of ATUSB90 UDTST_reg_t");

  struct UEINTX_reg_t {
    uint8_t _TXINI : 1;
    uint8_t _STALLEDI : 1;
    uint8_t _RXOUTI : 1;
    uint8_t _RXSTPI : 1;
    uint8_t _NAKOUTI : 1;
    uint8_t _RWAL : 1;
    uint8_t _NAKINI : 1;
    uint8_t _FIFOCON : 1;
  };
  static_assert(sizeof(UEINTX_reg_t) == 1, "invalid size of ATUSB90 UEINTX_reg_t");

  struct UENUM_reg_t {
    uint8_t _EPNUM : 3;
    uint8_t reserved1 : 5;
  };
  static_assert(sizeof(UENUM_reg_t) == 1, "invalid size of ATUSB90 UENUM_reg_t");

  struct UERST_reg_t {
    uint8_t _EPRST : 7;
    uint8_t reserved1 : 1;
  };
  static_assert(sizeof(UERST_reg_t) == 1, "invalid size of ATUSB90 UERST_reg_t");

  struct UECONX_reg_t {
    uint8_t _EPEN : 1;
    uint8_t reserved1 : 2;
    uint8_t _RSTDT : 1;
    uint8_t _STALLRQC : 1;
    uint8_t _STALLRQ : 1;
    uint8_t reserved2 : 2;
  };
  static_assert(sizeof(UECONX_reg_t) == 1, "invalid size of ATUSB90 UECONX_reg_t");

  struct UECFG0X_reg_t {
    uint8_t _EPDIR : 1;
    uint8_t _NYETSDIS : 1;
    uint8_t _AUTOSW : 1;
    uint8_t _ISOSW : 1;
    uint8_t reserved1 : 2;
    uint8_t _EPTYPE : 2;
  };
  static_assert(sizeof(UECFG0X_reg_t) == 1, "invalid size of ATUSB90 UECFG0X_reg_t");

  struct UECFG1X_reg_t {
    uint8_t reserved1 : 1;
    uint8_t _ALLOC : 1;
    uint8_t _EPBK : 2;
    uint8_t _EPSIZE : 3;
    uint8_t reserved2 : 1;
  };
  static_assert(sizeof(UECFG1X_reg_t) == 1, "invalid size of ATUSB90 UECFG1X_reg_t");

  struct UESTA0X_reg_t {
    uint8_t _NBUSYBK : 2;
    uint8_t _DTSEQ : 2;
    uint8_t _ZLPSEEN : 1;
    uint8_t _UNDERFI : 1;
    uint8_t _OVERFI : 1;
    uint8_t _CFGOK : 1;
  };
  static_assert(sizeof(UESTA0X_reg_t) == 1, "invalid size of ATUSB90 UESTA0X_reg_t");

  struct UESTA1X_reg_t {
    uint8_t _CURRBK : 2;
    uint8_t _CTRLDIR : 1;
    uint8_t reserved1 : 5;
  };
  static_assert(sizeof(UESTA1X_reg_t) == 1, "invalid size of ATUSB90 UESTA1X_reg_t");

  struct UEIENX_reg_t {
    uint8_t _TXINE : 1;
    uint8_t _STALLEDE : 1;
    uint8_t _RXOUTE : 1;
    uint8_t _RXSTPE : 1;
    uint8_t _NAKOUTE : 1;
    uint8_t reserved1 : 1;
    uint8_t _NAKINE : 1;
    uint8_t _FLERRE : 1;
  };
  static_assert(sizeof(UEIENX_reg_t) == 1, "invalid size of ATUSB90 UEIENX_reg_t");

  struct UEBCX_reg_t {
    uint16_t _BYCT : 11;
    uint16_t reserved1 : 5;
  };
  static_assert(sizeof(UEBCX_reg_t) == 2, "invalid size of ATUSB90 UEBCX_reg_t");

  struct UEINT_reg_t {
    uint8_t _EPINT : 7;
    uint8_t reserved1 : 1;
  };
  static_assert(sizeof(UEINT_reg_t) == 1, "invalid size of ATUSB90 UEINT_reg_t");

  struct UPERRX_reg_t {
    uint8_t _DATATGL : 1;
    uint8_t _DATAPID : 1;
    uint8_t _PID : 1;
    uint8_t _TIMEOUT : 1;
    uint8_t _CRC16 : 1;
    uint8_t _COUNTER : 2;
    uint8_t reserved1 : 1;
  };
  static_assert(sizeof(UPERRX_reg_t) == 1, "invalid size of ATUSB90 UPERRX_reg_t");

  struct UPBCX_reg_t {
    uint16_t _PBYCT : 11;
    uint16_t reserved1 : 5;
  };
  static_assert(sizeof(UPBCX_reg_t) == 2, "invalid size of ATUSB90 UPBCX_reg_t");

  struct OTGTCON_reg_t {
    uint8_t _VALUE : 2;
    uint8_t reserved1 : 3;
    uint8_t _PAGE : 2;
    uint8_t _one : 1;
  };
  static_assert(sizeof(OTGTCON_reg_t) == 1, "invalid size of ATUSB90 OTGTCON_reg_t");

  struct PLLCSR_reg_t {
    uint8_t _PLOCK : 1;
    uint8_t _PLLE : 1;
    uint8_t _PLLP : 3;
    uint8_t reserved1 : 3;
  };
  static_assert(sizeof(PLLCSR_reg_t) == 1, "invalid size of ATUSB90 PLLCSR_reg_t");

#endif // __AVR_TRM04__

/**
 * REGISTER MEMORY MAP
 */

#define __AVR_DEFREG(tn,n,a) static volatile tn& n = *(tn*)a
#define _AVR_DEFREG(n,a) __AVR_DEFREG(n##_reg_t, _##n, a)

#ifdef __AVR_TRM01__
  // page 399ff of ATmega640-1280-1281-2560-2561-Datasheet-DS40002211A.pdf

  __AVR_DEFREG(PORT_dev_t, _PORTA, 0x20);
  __AVR_DEFREG(PORT_dev_t, _PORTB, 0x23);
  __AVR_DEFREG(PORT_dev_t, _PORTC, 0x26);
  __AVR_DEFREG(PORT_dev_t, _PORTD, 0x29);
  __AVR_DEFREG(PORT_dev_t, _PORTE, 0x2C);
  __AVR_DEFREG(PORT_dev_t, _PORTF, 0x2F);
  __AVR_DEFREG(PORTG_dev_t, _PORTG, 0x32);
  __AVR_DEFREG(PORT_dev_t, _PORTH, 0x100);
  __AVR_DEFREG(PORT_dev_t, _PORTJ, 0x103);
  __AVR_DEFREG(PORT_dev_t, _PORTK, 0x106);
  __AVR_DEFREG(PORT_dev_t, _PORTL, 0x109);
  __AVR_DEFREG(TIFR0_reg_t, _TIFR0, 0x35);
  __AVR_DEFREG(TIFR1_reg_t, _TIFR1, 0x36);
  __AVR_DEFREG(TIFR2_reg_t, _TIFR2, 0x37);
  __AVR_DEFREG(TIFR3_reg_t, _TIFR3, 0x38);
  __AVR_DEFREG(TIFR4_reg_t, _TIFR4, 0x39);
  __AVR_DEFREG(TIFR5_reg_t, _TIFR5, 0x3A);
  __AVR_DEFREG(PCIFR_reg_t, _PCIFR, 0x3B);
  __AVR_DEFREG(EIFR_reg_t, _EIFR, 0x3C);
  __AVR_DEFREG(EIMSK_reg_t, _EIMSK, 0x3D);
  __AVR_DEFREG(_bit_reg_t, _GPIOR0, 0x3E);
  __AVR_DEFREG(EECR_reg_t, _EECR, 0x3F);
  __AVR_DEFREG(uint8_t, _EEDR, 0x40);
  __AVR_DEFREG(EEAR_reg_t, _EEAR, 0x41);
  __AVR_DEFREG(GTCCR_reg_t, _GTCCR, 0x43);
  __AVR_DEFREG(TIMER_8bit_dev_t, TIMER0, 0x44);
  __AVR_DEFREG(_bit_reg_t, _GPIOR1, 0x4A);
  __AVR_DEFREG(_bit_reg_t, _GPIOR2, 0x4B);
  __AVR_DEFREG(SPCR_reg_t, _SPCR, 0x4C);
  __AVR_DEFREG(SPSR_reg_t, _SPSR, 0x4D);
  __AVR_DEFREG(uint8_t, _SPDR, 0x4E);
  __AVR_DEFREG(ACSR_reg_t, _ACSR, 0x50);
  __AVR_DEFREG(_bit_reg_t, _OCDR, 0x51);
  __AVR_DEFREG(SMCR_reg_t, _SMCR, 0x53);
  __AVR_DEFREG(MCUSR_reg_t, _MCUSR, 0x54);
  __AVR_DEFREG(MCUCR_reg_t, _MCUCR, 0x55);
  __AVR_DEFREG(SPMCSR_reg_t, _SPMCSR, 0x57);
  __AVR_DEFREG(RAMPZ_reg_t, _RAMPZ, 0x5B);
  __AVR_DEFREG(EIND_reg_t, _EIND, 0x5C);
  __AVR_DEFREG(SP_reg_t, _SP, 0x5D);
  __AVR_DEFREG(SREG_reg_t, _SREG, 0x5F);
  __AVR_DEFREG(WDTCSR_reg_t, _WDTCSR, 0x60);
  __AVR_DEFREG(CLKPR_reg_t, _CLKPR, 0x61);
  __AVR_DEFREG(PRR0_reg_t, _PRR0, 0x64);
  __AVR_DEFREG(PRR1_reg_t, _PRR1, 0x65);
  __AVR_DEFREG(uint8_t, _OSCCAL, 0x66);
  __AVR_DEFREG(PCICR_reg_t, _PCICR, 0x68);
  __AVR_DEFREG(EICRA_reg_t, _EICRA, 0x69);
  __AVR_DEFREG(EICRB_reg_t, _EICRB, 0x6A);
  __AVR_DEFREG(_bit_reg_t, _PCMSK0, 0x6B);
  __AVR_DEFREG(_bit_reg_t, _PCMSK1, 0x6C);
  __AVR_DEFREG(_bit_reg_t, _PCMSK2, 0x6D);
  __AVR_DEFREG(TIMSK0_reg_t, _TIMSK0, 0x6E);
  __AVR_DEFREG(TIMSK1_reg_t, _TIMSK1, 0x6F);
  __AVR_DEFREG(TIMSK2_reg_t, _TIMSK2, 0x70);
  __AVR_DEFREG(TIMSK3_reg_t, _TIMSK3, 0x71);
  __AVR_DEFREG(TIMSK4_reg_t, _TIMSK4, 0x72);
  __AVR_DEFREG(TIMSK5_reg_t, _TIMSK5, 0x73);
  __AVR_DEFREG(XMCRA_reg_t, _XMCRA, 0x74);
  __AVR_DEFREG(XMCRB_reg_t, _XMCRB, 0x75);
  __AVR_DEFREG(uint16_t, _ADC, 0x78);
  __AVR_DEFREG(ADCSRA_reg_t, _ADCSRA, 0x7A);
  __AVR_DEFREG(ADCSRB_reg_t, _ADCSRB, 0x7B);
  __AVR_DEFREG(ADMUX_reg_t, _ADMUX, 0x7C);
  __AVR_DEFREG(DIDR2_reg_t, _DIDR2, 0x7D);
  __AVR_DEFREG(DIDR0_reg_t, _DIDR0, 0x7E);
  __AVR_DEFREG(DIDR1_reg_t, _DIDR1, 0x7F);
  __AVR_DEFREG(TIMER_dev_t, TIMER1, 0x80);
  __AVR_DEFREG(TIMER_dev_t, TIMER3, 0x90);
  __AVR_DEFREG(TIMER_dev_t, TIMER4, 0xA0);
  __AVR_DEFREG(TIMER_dev_t, TIMER5, 0x120);
  __AVR_DEFREG(TIMER_8bit_dev_t, _TIMER2, 0xB0);
  __AVR_DEFREG(ASSR_reg_t, _ASSR, 0xB6);
  __AVR_DEFREG(uint8_t, _TWBR, 0xB8);
  __AVR_DEFREG(TWSR_reg_t, _TWSR, 0xB9);
  __AVR_DEFREG(TWAR_reg_t, _TWAR, 0xBA);
  __AVR_DEFREG(uint8_t, _TWDR, 0xBB);
  __AVR_DEFREG(TWCR_reg_t, _TWCR, 0xBC);
  __AVR_DEFREG(TWAMR_reg_t, _TWAMR, 0xBD);
  __AVR_DEFREG(USART_dev_t, USART0, 0xC0);
  __AVR_DEFREG(USART_dev_t, USART1, 0xC8);
  __AVR_DEFREG(USART_dev_t, USART2, 0xD0);
  __AVR_DEFREG(USART_dev_t, USART3, 0x130);

#elif defined(__AVR_TRM02__)
  // page 637ff of ATmega164A_PA-324A_PA-644A_PA-1284_P_Data-Sheet-40002070B.pdf
  __AVR_DEFREG(PORT_dev_t, _PORTA, 0x20);
  __AVR_DEFREG(PORT_dev_t, _PORTB, 0x23);
  __AVR_DEFREG(PORT_dev_t, _PORTC, 0x26);
  __AVR_DEFREG(PORT_dev_t, _PORTD, 0x29);
  __AVR_DEFREG(TIFR0_reg_t, _TIFR0, 0x35);
  __AVR_DEFREG(TIFR1_reg_t, _TIFR1, 0x36);
  __AVR_DEFREG(TIFR2_reg_t, _TIFR2, 0x37);
  __AVR_DEFREG(TIFR3_reg_t, _TIFR3, 0x38);
  __AVR_DEFREG(PCIFR_reg_t, _PCIFR, 0x3B);
  __AVR_DEFREG(EIFR_reg_t, _EIFR, 0x3C);
  __AVR_DEFREG(EIMSK_reg_t, _EIMSK, 0x3D);
  __AVR_DEFREG(_bit_reg_t, _GPIOR0, 0x3E);
  __AVR_DEFREG(EECR_reg_t, _EECR, 0x3F);
  __AVR_DEFREG(uint8_t, _EEDR, 0x40);
  __AVR_DEFREG(EEAR_reg_t, _EEAR, 0x41);
  __AVR_DEFREG(GTCCR_reg_t, _GTCCR, 0x43);
  __AVR_DEFREG(TIMER_8bit_dev_t, TIMER0, 0x44);
  __AVR_DEFREG(_bit_reg_t, _GPIOR1, 0x4A);
  __AVR_DEFREG(_bit_reg_t, _GPIOR2, 0x4B);
  __AVR_DEFREG(SPCR_reg_t, _SPCR, 0x4C);
  __AVR_DEFREG(SPSR_reg_t, _SPSR, 0x4D);
  __AVR_DEFREG(uint8_t, _SPDR, 0x4E);
  __AVR_DEFREG(ACSR_reg_t, _ACSR, 0x50);
  __AVR_DEFREG(SMCR_reg_t, _SMCR, 0x53);
  __AVR_DEFREG(MCUSR_reg_t, _MSUSR, 0x54);
  __AVR_DEFREG(MCUCR_reg_t, _MCUCR, 0x55);
  __AVR_DEFREG(SPMCSR_reg_t, _SPMCSR, 0x57);
  __AVR_DEFREG(SP_reg_t, _SP, 0x5D);
  __AVR_DEFREG(SREG_reg_t, _SREG, 0x5F);
  __AVR_DEFREG(WDTCSR_reg_t, _WDTCSR, 0x60);
  __AVR_DEFREG(CLKPR_reg_t, _CLKPR, 0x61);
  __AVR_DEFREG(PRR0_reg_t, _PRR0, 0x64);
  __AVR_DEFREG(PRR1_reg_t, _PRR1, 0x65);
  __AVR_DEFREG(uint8_t, _OSCCAL, 0x66);
  __AVR_DEFREG(PCICR_reg_t, _PCICR, 0x68);
  __AVR_DEFREG(EICRA_reg_t, _EICRA, 0x69);
  __AVR_DEFREG(_bit_reg_t, _PCMSK0, 0x6B);
  __AVR_DEFREG(_bit_reg_t, _PCMSK1, 0x6C);
  __AVR_DEFREG(_bit_reg_t, _PCMSK2, 0x6D);
  __AVR_DEFREG(TIMSK0_reg_t, _TIMSK0, 0x6E);
  __AVR_DEFREG(TIMSK1_reg_t, _TIMSK1, 0x6F);
  __AVR_DEFREG(TIMSK2_reg_t, _TIMSK2, 0x70);
  __AVR_DEFREG(TIMSK3_reg_t, _TIMSK3, 0x71);
  __AVR_DEFREG(_bit_reg_t, _PCMSK3, 0x73);
  __AVR_DEFREG(uint16_t, _ADC, 0x78);
  __AVR_DEFREG(ADCSRA_reg_t, _ADCSRA, 0x7A);
  __AVR_DEFREG(ADCSRB_reg_t, _ADCSRB, 0x7B);
  __AVR_DEFREG(ADMUX_reg_t, _ADMUX, 0x7C);
  __AVR_DEFREG(DIDR0_reg_t, _DIDR0, 0x7E);
  __AVR_DEFREG(DIDR1_reg_t, _DIDR1, 0x7F);
  __AVR_DEFREG(TIMER_dev_t, TIMER1, 0x80);
  __AVR_DEFREG(TIMER_dev_t, TIMER3, 0x90);
  __AVR_DEFREG(TIMER_8bit_dev_t, _TIMER2, 0xB0);
  __AVR_DEFREG(ASSR_reg_t, _ASSR, 0xB6);
  __AVR_DEFREG(uint8_t, _TWBR, 0xB8);
  __AVR_DEFREG(TWSR_reg_t, _TWSR, 0xB9);
  __AVR_DEFREG(TWAR_reg_t, _TWAR, 0xBA);
  __AVR_DEFREG(uint8_t, _TWDR, 0xBB);
  __AVR_DEFREG(TWCR_reg_t, _TWCR, 0xBC);
  __AVR_DEFREG(TWAMR_reg_t, _TWAMR, 0xBD);
  __AVR_DEFREG(USART_dev_t, USART0, 0xC0);
  __AVR_DEFREG(USART_dev_t, USART1, 0xC8);

#elif defined(__AVR_TRM03__)
  // page 621ff of ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061B.pdf
  __AVR_DEFREG(PORT_dev_t, _PORTB, 0x23);
  __AVR_DEFREG(PORTC_dev_t, _PORTC, 0x26);
  __AVR_DEFREG(PORT_dev_t, _PORTD, 0x29);
  __AVR_DEFREG(TIFR0_reg_t, _TIFR0, 0x35);
  __AVR_DEFREG(TIFR1_reg_t, _TIFR1, 0x36);
  __AVR_DEFREG(TIFR2_reg_t, _TIFR2, 0x37);
  __AVR_DEFREG(PCIFR_reg_t, _PCIFR, 0x3B);
  __AVR_DEFREG(EIFR_reg_t, _EIFR, 0x3C);
  __AVR_DEFREG(EIMSK_reg_t, _EIMSK, 0x3D);
  __AVR_DEFREG(_bit_reg_t, _GPIOR0, 0x3E);
  __AVR_DEFREG(EECR_reg_t, _EECR, 0x3F);
  __AVR_DEFREG(uint8_t, _EEDR, 0x40);
  __AVR_DEFREG(EEAR_reg_t, _EEAR, 0x41);
  __AVR_DEFREG(GTCCR_reg_t, _GTCCR, 0x43);
  __AVR_DEFREG(TIMER_8bit_dev_t, TIMER0, 0x44);
  __AVR_DEFREG(_bit_reg_t, _GPIOR1, 0x4A);
  __AVR_DEFREG(_bit_reg_t, _GPIOR2, 0x4B);
  __AVR_DEFREG(SPCR_reg_t, _SPCR, 0x4C);
  __AVR_DEFREG(SPSR_reg_t, _SPSR, 0x4D);
  __AVR_DEFREG(uint8_t, _SPDR, 0x4E);
  __AVR_DEFREG(ACSR_reg_t, _ACSR, 0x50);
  __AVR_DEFREG(SMCR_reg_t, _SMCR, 0x53);
  __AVR_DEFREG(MCUSR_reg_t, _MSUCR, 0x54);
  __AVR_DEFREG(MCUCR_reg_t, _MCUCR, 0x55);
  __AVR_DEFREG(SPMCSR_reg_t, _SPMCSR, 0x57);
  __AVR_DEFREG(SP_reg_t, _SP, 0x5D);
  __AVR_DEFREG(SREG_reg_t, _SREG, 0x5F);
  __AVR_DEFREG(WDTCSR_reg_t, _WDTCSR, 0x60);
  __AVR_DEFREG(CLKPR_reg_t, _CLKPR, 0x61);
  __AVR_DEFREG(PRR0_reg_t, _PRR0, 0x64);
  __AVR_DEFREG(uint8_t, _OSCCAL, 0x66);
  __AVR_DEFREG(PCICR_reg_t, _PCICR, 0x68);
  __AVR_DEFREG(EICRA_reg_t, _EICRA, 0x69);
  __AVR_DEFREG(_bit_reg_t, _PCMSK0, 0x6B);
  __AVR_DEFREG(_bitPCMSK1_reg_t, _PCMSK1, 0x6C);
  __AVR_DEFREG(_bit_reg_t, _PCMSK2, 0x6D);
  __AVR_DEFREG(TIMSK0_reg_t, _TIMSK0, 0x6E);
  __AVR_DEFREG(TIMSK1_reg_t, _TIMSK1, 0x6F);
  __AVR_DEFREG(TIMSK2_reg_t, _TIMSK2, 0x70);
  __AVR_DEFREG(uint16_t, _ADC, 0x78);
  __AVR_DEFREG(ADCSRA_reg_t, _ADCSRA, 0x7A);
  __AVR_DEFREG(ADCSRB_reg_t, _ADCSRB, 0x7B);
  __AVR_DEFREG(ADMUX_reg_t, _ADMUX, 0x7C);
  __AVR_DEFREG(DIDR0_reg_t, _DIDR0, 0x7E);
  __AVR_DEFREG(DIDR1_reg_t, _DIDR1, 0x7F);
  __AVR_DEFREG(TIMER_dev_t, TIMER1, 0x80);
  __AVR_DEFREG(TIMER_8bit_dev_t, _TIMER2, 0xB0);
  __AVR_DEFREG(ASSR_reg_t, _ASSR, 0xB6);
  __AVR_DEFREG(uint8_t, _TWBR, 0xB8);
  __AVR_DEFREG(TWSR_reg_t, _TWSR, 0xB9);
  __AVR_DEFREG(TWAR_reg_t, _TWAR, 0xBA);
  __AVR_DEFREG(uint8_t, _TWDR, 0xBB);
  __AVR_DEFREG(TWCR_reg_t, _TWCR, 0xBC);
  __AVR_DEFREG(TWAMR_reg_t, _TWAMR, 0xBD);
  __AVR_DEFREG(USART_dev_t, USART0, 0xC0);

#elif defined(__AVR_TRM04__)
  __AVR_DEFREG(PORT_dev_t, _PORTA, 0x20);
  __AVR_DEFREG(PORT_dev_t, _PORTB, 0x23);
  __AVR_DEFREG(PORT_dev_t, _PORTC, 0x26);
  __AVR_DEFREG(PORT_dev_t, _PORTD, 0x29);
  __AVR_DEFREG(PORT_dev_t, _PORTE, 0x2C);
  __AVR_DEFREG(PORT_dev_t, _PORTF, 0x2F);
  __AVR_DEFREG(TIFR0_reg_t, _TIFR0, 0x35);
  __AVR_DEFREG(TIFR1_reg_t, _TIFR1, 0x36);
  __AVR_DEFREG(TIFR2_reg_t, _TIFR2, 0x37);
  __AVR_DEFREG(TIFR3_reg_t, _TIFR3, 0x38);
  __AVR_DEFREG(PCIFR_reg_t, _PCIFR, 0x3B);
  __AVR_DEFREG(EIFR_reg_t, _EIFR, 0x3C);
  __AVR_DEFREG(EIMSK_reg_t, _EIMSK, 0x3D);
  __AVR_DEFREG(_bit_reg_t, _GPIOR0, 0x3E);
  __AVR_DEFREG(EECR_reg_t, _EECR, 0x3F);
  __AVR_DEFREG(uint8_t, _EEDR, 0x40);
  __AVR_DEFREG(EEAR_reg_t, _EEAR, 0x41);
  __AVR_DEFREG(GTCCR_reg_t, _GTCCR, 0x43);
  __AVR_DEFREG(TIMER_8bit_dev_t, TIMER0, 0x44);
  __AVR_DEFREG(PLLCSR_reg_t, _PLLCSR, 0x49);
  __AVR_DEFREG(_bit_reg_t, _GPIOR1, 0x4A);
  __AVR_DEFREG(_bit_reg_t, _GPIOR2, 0x4B);
  __AVR_DEFREG(SPCR_reg_t, _SPCR, 0x4C);
  __AVR_DEFREG(SPSR_reg_t, _SPSR, 0x4D);
  __AVR_DEFREG(uint8_t, _SPDR, 0x4E);
  __AVR_DEFREG(ACSR_reg_t, _ACSR, 0x50);
  __AVR_DEFREG(uint8_t, _OCDR, 0x51);
  __AVR_DEFREG(SMCR_reg_t, _SMCR, 0x53);
  __AVR_DEFREG(MCUSR_reg_t, _MCUSR, 0x54);
  __AVR_DEFREG(MCUCR_reg_t, _MCUCR, 0x55);
  __AVR_DEFREG(SPMCSR_reg_t, _SPMCSR, 0x57);
  __AVR_DEFREG(RAMPZ_reg_t, _RAMPZ, 0x5B);
  __AVR_DEFREG(SP_reg_t, _SP, 0x5D);
  __AVR_DEFREG(SREG_reg_t, _SREG, 0x5F);
  __AVR_DEFREG(WDTCSR_reg_t, _WDTCSR, 0x60);
  __AVR_DEFREG(CLKPR_reg_t, _CLKPR, 0x61);
  __AVR_DEFREG(PRR0_reg_t, _PRR0, 0x64);
  __AVR_DEFREG(PRR1_reg_t, _PRR1, 0x65);
  __AVR_DEFREG(uint8_t, _OSCCAL, 0x66);
  __AVR_DEFREG(PCICR_reg_t, _PCICR, 0x68);
  __AVR_DEFREG(EICRA_reg_t, _EICRA, 0x69);
  __AVR_DEFREG(EICRB_reg_t, _EICRB, 0x6A);
  __AVR_DEFREG(_bit_reg_t, _PCMSK0, 0x6B);
  __AVR_DEFREG(TIMSK0_reg_t, _TIMSK0, 0x6E);
  __AVR_DEFREG(TIMSK1_reg_t, _TIMSK1, 0x6F);
  __AVR_DEFREG(TIMSK2_reg_t, _TIMSK2, 0x70);
  __AVR_DEFREG(TIMSK3_reg_t, _TIMSK3, 0x71);
  __AVR_DEFREG(XMCRA_reg_t, _XMCRA, 0x74);
  __AVR_DEFREG(XMCRB_reg_t, _XMCRB, 0x75);
  __AVR_DEFREG(uint16_t, _ADC, 0x78);
  __AVR_DEFREG(ADCSRA_reg_t, _ADCSRA, 0x7A);
  __AVR_DEFREG(ADCSRB_reg_t, _ADCSRB, 0x7B);
  __AVR_DEFREG(ADMUX_reg_t, _ADMUX, 0x7C);
  __AVR_DEFREG(DIDR0_reg_t, _DIDR0, 0x7E);
  __AVR_DEFREG(DIDR1_reg_t, _DIDR1, 0x7F);
  __AVR_DEFREG(TIMER_dev_t, TIMER1, 0x80);
  __AVR_DEFREG(TIMER_dev_t, TIMER3, 0x90);
  __AVR_DEFREG(UHCON_reg_t, _UHCON, 0x9E);
  __AVR_DEFREG(UHINT_reg_t, _UHINT, 0x9F);
  __AVR_DEFREG(UHIEN_reg_t, _UHIEN, 0xA0);
  __AVR_DEFREG(UHADDR_reg_t, _UHADDR, 0xA1);
  __AVR_DEFREG(UHFNUM_reg_t, _UHFNUM, 0xA2);
  __AVR_DEFREG(uint8_t, _UHFLEN, 0xA4);
  __AVR_DEFREG(uint8_t, _UPINRQX, 0xA5);
  __AVR_DEFREG(UPINTX_reg_t, _UPINTX, 0xA6);
  __AVR_DEFREG(UPNUM_reg_t, _UPNUM, 0xA7);
  __AVR_DEFREG(UPRST_reg_t, _UPRST, 0xA8);
  __AVR_DEFREG(UPCONX_reg_t, _UPCONX, 0xA9);
  _AVR_DEFREG(UPCFG0X, 0xAA);
  _AVR_DEFREG(UPCFG1X, 0xAB);
  _AVR_DEFREG(UPSTAX, 0xAC);
  __AVR_DEFREG(uint8_t, _UPCFG2X, 0xAD);
  _AVR_DEFREG(UPIENX, 0xAE);
  __AVR_DEFREG(uint8_t, _UPDATX, 0xAF);
  __AVR_DEFREG(TIMER_8bit_dev_t, _TIMER2, 0xB0);
  __AVR_DEFREG(ASSR_reg_t, _ASSR, 0xB6);
  __AVR_DEFREG(uint8_t, _TWBR, 0xB8);
  __AVR_DEFREG(TWSR_reg_t, _TWSR, 0xB9);
  __AVR_DEFREG(TWAR_reg_t, _TWAR, 0xBA);
  __AVR_DEFREG(uint8_t, _TWDR, 0xBB);
  __AVR_DEFREG(TWCR_reg_t, _TWCR, 0xBC);
  __AVR_DEFREG(TWAMR_reg_t, _TWAMR, 0xBD);
  __AVR_DEFREG(USART_dev_t, USART1, 0xC8);
  _AVR_DEFREG(UHWCON, 0xD7);
  _AVR_DEFREG(USBCON, 0xD8);
  _AVR_DEFREG(USBSTA, 0xD9);
  _AVR_DEFREG(USBINT, 0xDA);
  _AVR_DEFREG(UDPADD, 0xDB);
  _AVR_DEFREG(OTGCON, 0xDD);
  _AVR_DEFREG(OTGIEN, 0xDE);
  _AVR_DEFREG(OTGINT, 0xDF);
  _AVR_DEFREG(UDCON, 0xE0);
  _AVR_DEFREG(UDINT, 0xE1);
  _AVR_DEFREG(UDIEN, 0xE2);
  _AVR_DEFREG(UDADDR, 0xE3);
  _AVR_DEFREG(UDFNUM, 0xE4);
  _AVR_DEFREG(UDMFN, 0xE6);
  _AVR_DEFREG(UDTST, 0xE7);
  _AVR_DEFREG(UEINTX, 0xE8);
  _AVR_DEFREG(UENUM, 0xE9);
  _AVR_DEFREG(UERST, 0xEA);
  _AVR_DEFREG(UECONX, 0xEB);
  _AVR_DEFREG(UECFG0X, 0xEC);
  _AVR_DEFREG(UECFG1X, 0xED);
  _AVR_DEFREG(UESTA0X, 0xEE);
  _AVR_DEFREG(UESTA1X, 0xEF);
  _AVR_DEFREG(UEIENX, 0xF0);
  __AVR_DEFREG(uint8_t, _UEDATx, 0xF1);
  _AVR_DEFREG(UEBCX, 0xF2);
  _AVR_DEFREG(UEINT, 0xF4);
  _AVR_DEFREG(UPERRX, 0xF5);
  _AVR_DEFREG(UPBCX, 0xF6);
  __AVR_DEFREG(uint8_t, _UPINT, 0xF8);
  _AVR_DEFREG(OTGTCON, 0xF9);
#elif defined(__AVR_TRM05__)
  // page 476ff. of ATmega164P-324P-644P-Data-Sheet-40002071A.pdf
  __AVR_DEFREG(PORT_dev_t, _PORTA, 0x20);
  __AVR_DEFREG(PORT_dev_t, _PORTB, 0x23);
  __AVR_DEFREG(PORT_dev_t, _PORTC, 0x26);
  __AVR_DEFREG(PORT_dev_t, _PORTD, 0x29);
  __AVR_DEFREG(TIFR0_reg_t, _TIFR0, 0x35);
  __AVR_DEFREG(TIFR1_reg_t, _TIFR1, 0x36);
  __AVR_DEFREG(TIFR2_reg_t, _TIFR2, 0x37);
  __AVR_DEFREG(PCIFR_reg_t, _PCIFR, 0x3B);
  __AVR_DEFREG(EIFR_reg_t, _EIFR, 0x3C);
  __AVR_DEFREG(EIMSK_reg_t, _EIMSK, 0x3D);
  __AVR_DEFREG(_bit_reg_t, _GPIOR0, 0x3E);
  __AVR_DEFREG(EECR_reg_t, _EECR, 0x3F);
  __AVR_DEFREG(uint8_t, _EEDR, 0x40);
  __AVR_DEFREG(EEAR_reg_t, _EEAR, 0x41);
  __AVR_DEFREG(GTCCR_reg_t, _GTCCR, 0x43);
  __AVR_DEFREG(TIMER_8bit_dev_t, TIMER0, 0x44);
  __AVR_DEFREG(_bit_reg_t, _GPIOR1, 0x4A);
  __AVR_DEFREG(_bit_reg_t, _GPIOR2, 0x4B);
  __AVR_DEFREG(SPCR_reg_t, _SPCR, 0x4C);
  __AVR_DEFREG(SPSR_reg_t, _SPSR, 0x4D);
  __AVR_DEFREG(uint8_t, _SPDR, 0x4E);
  __AVR_DEFREG(ACSR_reg_t, _ACSR, 0x50);
  __AVR_DEFREG(uint8_t, _OCDR, 0x51);
  __AVR_DEFREG(SMCR_reg_t, _SMCR, 0x53);
  __AVR_DEFREG(MCUSR_reg_t, _MCUSR, 0x54);
  __AVR_DEFREG(MCUCR_reg_t, _MCUCR, 0x55);
  __AVR_DEFREG(SPMCSR_reg_t, _SPMCSR, 0x57);
  __AVR_DEFREG(RAMPZ_reg_t, _RAMPZ, 0x5B);
  __AVR_DEFREG(SP_reg_t, _SP, 0x5D);
  __AVR_DEFREG(SREG_reg_t, _SREG, 0x5F);
  __AVR_DEFREG(WDTCSR_reg_t, _WDTCSR, 0x60);
  __AVR_DEFREG(CLKPR_reg_t, _CLKPR, 0x61);
  __AVR_DEFREG(PRR0_reg_t, _PRR0, 0x64);
  __AVR_DEFREG(uint8_t, _OSCCAL, 0x66);
  __AVR_DEFREG(PCICR_reg_t, _PCICR, 0x68);
  __AVR_DEFREG(EICRA_reg_t, _EICRA, 0x69);
  __AVR_DEFREG(_bit_reg_t, _PCMSK0, 0x6B);
  __AVR_DEFREG(_bit_reg_t, _PCMSK1, 0x6C);
  __AVR_DEFREG(_bit_reg_t, _PCMSK2, 0x6D);
  __AVR_DEFREG(TIMSK0_reg_t, _TIMSK0, 0x6E);
  __AVR_DEFREG(TIMSK1_reg_t, _TIMSK1, 0x6F);
  __AVR_DEFREG(TIMSK2_reg_t, _TIMSK2, 0x70);
  __AVR_DEFREG(_bit_reg_t, _PCMKS3, 0x73);
  __AVR_DEFREG(uint16_t, _ADC, 0x78);
  __AVR_DEFREG(ADCSRA_reg_t, _ADCSRA, 0x7A);
  __AVR_DEFREG(ADCSRB_reg_t, _ADCSRB, 0x7B);
  __AVR_DEFREG(ADMUX_reg_t, _ADMUX, 0x7C);
  __AVR_DEFREG(DIDR0_reg_t, _DIDR0, 0x7E);
  __AVR_DEFREG(DIDR1_reg_t, _DIDR1, 0x7F);
  __AVR_DEFREG(TIMER_dev_t, TIMER1, 0x80);
  __AVR_DEFREG(TIMER_8bit_dev_t, _TIMER2, 0xB0);
  __AVR_DEFREG(ASSR_reg_t, _ASSR, 0xB6);
  __AVR_DEFREG(uint8_t, _TWBR, 0xB8);
  __AVR_DEFREG(TWSR_reg_t, _TWSR, 0xB8);
  __AVR_DEFREG(TWAR_reg_t, _TWAR, 0xBA);
  __AVR_DEFREG(uint8_t, _TWDR, 0xBB);
  __AVR_DEFREG(TWCR_reg_t, _TWCR, 0xBC);
  __AVR_DEFREG(TWAMR_reg_t, _TWAMR, 0xBD);
  __AVR_DEFREG(USART_dev_t, USART0, 0xC0);
  __AVR_DEFREG(USART_dev_t, USART1, 0xC8);
#endif

inline void _ATmega_resetperipherals() {
  using namespace AVRHelpers;

    // Due to BOOTLOADER or other board inconsistencies we could get launched into Marlin FW
    // with configuration that does not match the reset state in the documentation. That is why
    // we should clean-reset the entire device.
  #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
    SREG_reg_t __SREG;
    __SREG._C = false;
    __SREG._Z = false;
    __SREG._N = false;
    __SREG._V = false;
    __SREG._S = false;
    __SREG._H = false;
    __SREG._T = false;
    __SREG._I = false;
    dwrite(_SREG, __SREG);
  #endif

  #if defined(__AVR_TRM01__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
    _RAMPZ._RAMPZ = 0;
  #endif
  #ifdef __AVR_TRM01__
    _EIND._EIND0 = false;
  #endif

  #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM05__)
    _EEAR._EEAR = 0;
    dwrite(_EEDR, (uint8_t)0u);
  #endif

  #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
    EECR_reg_t __EECR;
    __EECR._EERE = false;
    __EECR._EEPE = false;
    __EECR._EEMPE = false;
    __EECR._EERIE = false;
    __EECR._EEPM0 = 0;
    __EECR._EEPM1 = 0;
    __EECR.reserved1 = 0;
    dwrite(_EECR, __EECR);
  #endif

  #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
    _GPIOR2.val = 0;
    _GPIOR1.val = 0;
    _GPIOR0.val = 0;
  #endif

  #if defined(__AVR_TRM01__) || defined(__AVR_TRM04__)
    XMCRA_reg_t __XMCRA;
    __XMCRA._SRW0 = 0;
    __XMCRA._SRW1 = 0;
    __XMCRA._SRL = 0;
    __XMCRA._SRE = 0;
    dwrite(_XMCRA, __XMCRA);

    XMCRB_reg_t __XMCRB;
    __XMCRB._XMM = 0;
    __XMCRB.reserved1 = 0;
    __XMCRB._XMBK = false;
    dwrite(_XMCRB, __XMCRB);
  #endif

  #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
    SMCR_reg_t __SMCR;
    __SMCR._SE = false;
    __SMCR._SM = 0;
    __SMCR.reserved1 = 0;
    dwrite(_SMCR, __SMCR);
  #endif

  #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
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
    #elif defined(__AVR_TRM04__)
      __PRR0._PRADC = false;
      __PRR0.reserved1 = false;
      __PRR0._PRSPI = false;
      __PRR0._PRTIM1 = false;
      __PRR0.reserved2 = false;
      __PRR0._PRTIM0 = false;
      __PRR0._PRTIM2 = false;
      __PRR0._PRTWI = false;
    #endif
    dwrite(_PRR0, __PRR0);
  #endif

  #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM04__)
    PRR1_reg_t __PRR1;
    #ifdef __AVR_TRM01__
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
    #elif defined(__AVR_TRM04__)
      __PRR1._PRUSART1 = false;
      __PRR1.reserved1 = 0;
    #endif
    dwrite(_PRR1, __PRR1);
  #endif

  #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
    WDTCSR_reg_t __WDTCSR;
    __WDTCSR._WDP0 = 0;
    __WDTCSR._WDP1 = 0;
    __WDTCSR._WDP2 = 0;
    __WDTCSR._WDE = false;
    __WDTCSR._WDCE = false;
    __WDTCSR._WDP3 = 0;
    __WDTCSR._WDIE = false;
    __WDTCSR._WDIF = false;
    dwrite(_WDTCSR, __WDTCSR);
  #endif

  #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
    _MCUCR._PUD = false;
  #endif

  #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
    PORT_dev_t __PORT;
    __PORT._PIN.val = 0;
    __PORT._DDR.val = 0;
    __PORT._PORT.val = 0;
  #endif

  #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
    dwrite(_PORTA, __PORT);
    dwrite(_PORTC, __PORT);
  #endif
  #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
    dwrite(_PORTB, __PORT);
    dwrite(_PORTD, __PORT);
  #endif
  #if defined(__AVR_TRM01__) || defined(__AVR_TRM04__)
    dwrite(_PORTE, __PORT);
    dwrite(_PORTF, __PORT);
  #endif

  #ifdef __AVR_TRM01__
    PORTG_dev_t __PORTG;
    __PORTG._PIN.val = 0;
    __PORTG._PIN.reserved1 = 0;
    __PORTG._DDR.val = 0;
    __PORTG._DDR.reserved1 = 0;
    __PORTG._PORT.val = 0;
    __PORTG._PORT.reserved1 = 0;
    dwrite(_PORTG, __PORTG);
  #endif

  #ifdef __AVR_TRM03__
    PORTC_dev_t __PORTC;
    __PORTC._PIN.val = 0;
    __PORTC._PIN.reserved1 = 0;
    __PORTC._DDR.val = 0;
    __PORTC._DDR.reserved1 = 0;
    __PORTC._PORT.val = 0;
    __PORTC._PORT.reserved1 = 0;
    dwrite(_PORTC, __PORTC);
  #endif

  #ifdef __AVR_TRM01__
    dwrite(_PORTH, __PORT);
    dwrite(_PORTJ, __PORT);
    dwrite(_PORTK, __PORT);
    dwrite(_PORTL, __PORT);
  #endif

  #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
    EICRA_reg_t __EICRA;
    #if defined(__AVR_TRM01__) || defined(__AVR_TRM04__)
      __EICRA._ISC0 = 0;
      __EICRA._ISC1 = 0;
      __EICRA._ISC2 = 0;
      __EICRA._ISC3 = 0;
    #elif defined(__AVR_TRM02__) || defined(__AVR_TRM05__)
      __EICRA._ISC0 = 0;
      __EICRA._ISC1 = 0;
      __EICRA._ISC2 = 0;
      __EICRA.reserved1 = 0;
    #elif defined(__AVR_TRM03__)
      __EICRA._ISC0 = 0;
      __EICRA._ISC1 = 0;
      __EICRA.reserved1 = 0;
    #endif
    dwrite(_EICRA, __EICRA);
  #endif

  #if defined(__AVR_TRM01__) || defined(__AVR_TRM04__)
    EICRB_reg_t __EICRB;
    __EICRB._ISC4 = 0;
    __EICRB._ISC5 = 0;
    __EICRB._ISC6 = 0;
    __EICRB._ISC7 = 0;
    dwrite(_EICRB, __EICRB);
  #endif

  #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
    EIMSK_reg_t __EIMSK;
  #if defined(__AVR_TRM01__) || defined(__AVR_TRM04__)
    __EIMSK._INT0 = false;
    __EIMSK._INT1 = false;
    __EIMSK._INT2 = false;
    __EIMSK._INT3 = false;
    __EIMSK._INT4 = false;
    __EIMSK._INT5 = false;
    __EIMSK._INT6 = false;
    __EIMSK._INT7 = false;
  #elif defined(__AVR_TRM02__) || defined(__AVR_TRM05__)
    __EIMSK._INT0 = false;
    __EIMSK._INT1 = false;
    __EIMSK._INT2 = false;
    __EIMSK.reserved1 = 0;
  #elif defined(__AVR_TRM03__)
    __EIMSK._INT0 = false;
    __EIMSK._INT1 = false;
    __EIMSK.reserved1 = 0;
  #endif
    dwrite(_EIMSK, __EIMSK);
  #endif

  #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
    EIFR_reg_t __EIFR;
  #if defined(__AVR_TRM01__) || defined(__AVR_TRM04__)
    __EIFR._INTF0 = false;
    __EIFR._INTF1 = false;
    __EIFR._INTF2 = false;
    __EIFR._INTF3 = false;
    __EIFR._INTF4 = false;
    __EIFR._INTF5 = false;
    __EIFR._INTF6 = false;
    __EIFR._INTF7 = false;
  #elif defined(__AVR_TRM02__) || defined(__AVR_TRM05__)
    __EIFR._INTF0 = false;
    __EIFR._INTF1 = false;
    __EIFR._INTF2 = false;
    __EIFR.reserved1 = 0;
  #elif defined(__AVR_TRM03__)
    __EIFR._INTF0 = false;
    __EIFR._INTF1 = false;
    __EIFR.reserved1 = 0;
  #endif
    dwrite(_EIFR, __EIFR);
  #endif

  #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
    PCICR_reg_t __PCICR;
  #if defined(__AVR_TRM01__) || defined(__AVR_TRM03__)
    __PCICR._PCIE0 = false;
    __PCICR._PCIE1 = false;
    __PCICR._PCIE2 = false;
    __PCICR.reserved1 = 0;
  #elif defined(__AVR_TRM02__) || defined(__AVR_TRM05__)
    __PCICR._PCIE0 = false;
    __PCICR._PCIE1 = false;
    __PCICR._PCIE2 = false;
    __PCICR._PCIE3 = false;
    __PCICR.reserved1 = 0;
  #elif defined(__AVR_TRM04__)
    __PCICR._PCIE0 = false;
    __PCICR.reserved1 = 0;
  #endif
    dwrite(_PCICR, __PCICR);
  #endif

  #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
    PCIFR_reg_t __PCIFR;
    #if defined(__AVR_TRM01__) || defined(__AVR_TRM03__)
      __PCIFR._PCIF0 = false;
      __PCIFR._PCIF1 = false;
      __PCIFR._PCIF2 = false;
      __PCIFR.reserved1 = 0;
    #elif defined(__AVR_TRM02__) || defined(__AVR_TRM05__)
      __PCIFR._PCIF0 = false;
      __PCIFR._PCIF1 = false;
      __PCIFR._PCIF2 = false;
      __PCIFR._PCIF3 = false;
      __PCIFR.reserved1 = 0;
    #elif defined(__AVR_TRM04__)
      __PCIFR._PCIF0 = false;
      __PCIFR.reserved1 = 0;
    #endif
    dwrite(_PCIFR, __PCIFR);
  #endif

  #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
    _PCMSK0.val = 0;
  #endif
  #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM05__)
    _PCMSK1.val = 0;
    _PCMSK2.val = 0;
  #endif
  #if defined(__AVR_TRM03__)
    _PCMSK1.reserved1 = 0;
  #endif
  #if defined(__AVR_TRM02__)
    _PCMSK3.val = 0;
  #endif

  #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
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
    dwrite(TIMER0, __TIMER_8bit);
  #endif

  #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
    TIMSK0_reg_t __TIMSK0;
    __TIMSK0._TOIE0 = false;
    __TIMSK0._OCIE0A = false;
    __TIMSK0._OCIE0B = false;
    __TIMSK0.reserved1 = 0;
    dwrite(_TIMSK0, __TIMSK0);

    TIFR0_reg_t __TIFR0;
    __TIFR0._TOV0 = false;
    __TIFR0._OCF0A = false;
    __TIFR0._OCF0B = false;
    __TIFR0.reserved1 = 0;
    dwrite(_TIFR0, __TIFR0);
  #endif

  #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
    TIMER_dev_t TIMER;
    TIMER._TCCRnA._WGMn0 = 0;
    TIMER._TCCRnA._WGMn1 = 0;
    #if defined(__AVR_TRM01__) || defined(__AVR_TRM04__)
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
    #if defined(__AVR_TRM01__) || defined(__AVR_TRM04__)
      TIMER._TCCRnC._FOCnC = false;
    #endif
    TIMER._TCCRnC._FOCnB = false;
    TIMER._TCCRnC._FOCnA = false;
    TIMER._TCNTn = 0;
    TIMER._OCRnA = 0;
    TIMER._OCRnB = 0;
    #if defined(__AVR_TRM01__) || defined(__AVR_TRM04__)
      TIMER._OCRnC = 0;
    #endif
    TIMER._ICRn = 0;
    dwrite(TIMER1, TIMER);
  #endif
  #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM04__)
    dwrite(TIMER3, TIMER);
  #endif
  #ifdef __AVR_TRM01__
    dwrite(TIMER4, TIMER);
    dwrite(TIMER5, TIMER);
  #endif

  #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
    TIMSK1_reg_t __TIMSK1;
    __TIMSK1._TOIE1 = false;
    __TIMSK1._OCIE1A = false;
    __TIMSK1._OCIE1B = false;
    #if defined(__AVR_TRM01__) || defined(__AVR_TRM04__)
      __TIMSK1._OCIE1C = false;
    #endif
    __TIMSK1.reserved1 = 0;
    __TIMSK1._ICIE1 = false;
    __TIMSK1.reserved2 = 0;
    dwrite(_TIMSK1, __TIMSK1);
  #endif

  #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM04__)
    TIMSK3_reg_t __TIMSK3;
    __TIMSK3._TOIE3 = false;
    __TIMSK3._OCIE3A = false;
    __TIMSK3._OCIE3B = false;
    #if defined(__AVR_TRM01__) || defined(__AVR_TRM04__)
      __TIMSK3._OCIE3C = false;
    #endif
    __TIMSK3.reserved1 = 0;
    __TIMSK3._ICIE3 = false;
    __TIMSK3.reserved2 = 0;
    dwrite(_TIMSK3, __TIMSK3);
  #endif

  #ifdef __AVR_TRM01__
    TIMSK4_reg_t __TIMSK4;
    __TIMSK4._TOIE4 = false;
    __TIMSK4._OCIE4A = false;
    __TIMSK4._OCIE4B = false;
    __TIMSK4._OCIE4C = false;
    __TIMSK4.reserved1 = false;
    __TIMSK4._ICIE4 = false;
    __TIMSK4.reserved2 = false;
    dwrite(_TIMSK4, __TIMSK4);

    TIMSK5_reg_t __TIMSK5;
    __TIMSK5._TOIE5 = false;
    __TIMSK5._OCIE5A = false;
    __TIMSK5._OCIE5B = false;
    __TIMSK5._OCIE5C = false;
    __TIMSK5.reserved1 = 0;
    __TIMSK5._ICIE5 = false;
    __TIMSK5.reserved2 = 0;
    dwrite(_TIMSK5, __TIMSK5);
  #endif

  #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
    TIFR1_reg_t __TIFR1;
    __TIFR1._TOV1 = false;
    __TIFR1._OCF1A = false;
    __TIFR1._OCF1B = false;
    #if defined(__AVR_TRM01__) || defined(__AVR_TRM04__)
      __TIFR1._OCF1C = false;
    #endif
    __TIFR1.reserved1 = 0;
    __TIFR1._ICF1 = false;
    __TIFR1.reserved2 = 0;
    dwrite(_TIFR1, __TIFR1);
  #endif

  #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM04__)
    TIFR3_reg_t __TIFR3;
    __TIFR3._TOV3 = false;
    __TIFR3._OCF3A = false;
    __TIFR3._OCF3B = false;
    #if defined(__AVR_TRM01__) || defined(__AVR_TRM04__)
      __TIFR3._OCF3C = false;
    #endif
    __TIFR3.reserved1 = 0;
    __TIFR3._ICF3 = false;
    __TIFR3.reserved2 = 0;
    dwrite(_TIFR3, __TIFR3);
  #endif

  #ifdef __AVR_TRM01__
    TIFR4_reg_t __TIFR4;
    __TIFR4._TOV4 = false;
    __TIFR4._OCF4A = false;
    __TIFR4._OCF4B = false;
    __TIFR4._OCF4C = false;
    __TIFR4.reserved1 = 0;
    __TIFR4._ICF4 = false;
    __TIFR4.reserved2 = 0;
    dwrite(_TIFR4, __TIFR4);

    TIFR5_reg_t __TIFR5;
    __TIFR5._TOV5 = false;
    __TIFR5._OCF5A = false;
    __TIFR5._OCF5B = false;
    __TIFR5._OCF5C = false;
    __TIFR5.reserved1 = 0;
    __TIFR5._ICF5 = false;
    __TIFR5.reserved2 = 0;
    dwrite(_TIFR5, __TIFR5);
  #endif

  #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
    dwrite(_TIMER2, __TIMER_8bit);
  #endif

  #if defined(__AV_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
    ASSR_reg_t __ASSR;
    __ASSR._TCR2BUB = false;
    __ASSR._TCR2AUB = false;
    __ASSR._OCR2BUB = false;
    __ASSR._OCR2AUB = false;
    __ASSR._TCN2UB = false;
    __ASSR._AS2 = false;
    __ASSR._EXCLK = false;
    __ASSR.reserved1 = 0;
    dwrite(_ASSR, __ASSR);
  #endif

  #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
    TIMSK2_reg_t __TIMSK2;
    __TIMSK2._TOIE2 = false;
    __TIMSK2._OCIE2A = false;
    __TIMSK2._OCIE2B = false;
    __TIMSK2.reserved1 = 0;
    dwrite(_TIMSK2, __TIMSK2);

    TIFR2_reg_t __TIFR2;
    __TIFR2._TOV2 = false;
    __TIFR2._OCF2A = false;
    __TIFR2._OCF2B = false;
    __TIFR2.reserved1 = 0;
    dwrite(_TIFR2, __TIFR2);
  #endif

  #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
    SPCR_reg_t __SPCR;
    __SPCR._SPR = 0;
    __SPCR._CPHA = 0;
    __SPCR._CPOL = 0;
    __SPCR._MSTR = 0;
    __SPCR._DORD = 0;
    __SPCR._SPE = false;
    __SPCR._SPIE = false;
    dwrite(_SPCR, __SPCR);

    SPSR_reg_t __SPSR;
    __SPSR._SPI2X = false;
    __SPSR.reserved1 = 0;
    __SPSR._WCOL = false;
    __SPSR._SPIF = false;
    dwrite(_SPSR, __SPSR);
  #endif

  #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
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
  #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__)
    USART._UCSRnC._UCSZn0 = 1;
    USART._UCSRnC._UCSZn1 = 1;
    USART._UCSRnC._USBS = false;
    USART._UCSRnC._UPM = 0;
    USART._UCSRnC._UPM = 0;
    USART._UCSRnC._UMSEL = 0;
  #elif defined(__AVR_TRM05__)
    USART._UCSRnC._UCPOL = 0;
    USART._UCSRnC._UCPHA = 0;
    USART._UCSRnC._UDORD = 0;
    USART._UCSRnC.reserved1 = 0;
    USART._UCSRnC._UMSEL = 0;
  #endif
    USART._UBRRn._UBRR = 0;
    USART._UBRRn.reserved1 = 0;
  #endif
  #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM05__)
    dwrite(USART0, USART);
  #endif
  #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
    dwrite(USART1, USART);
  #endif
  #ifdef __AVR_TRM01__
    dwrite(USART2, USART);
    dwrite(USART3, USART);
  #endif

  #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
    dwrite(_TWBR, (uint8_t)0);

    TWCR_reg_t __TWCR;
    __TWCR._TWIE = false;
    __TWCR.reserved1 = 0;
    __TWCR._TWEN = false;
    __TWCR._TWWC = false;
    __TWCR._TWSTO = false;
    __TWCR._TWSTA = false;
    __TWCR._TWEA = false;
    __TWCR._TWINT = false;
    dwrite(_TWCR, __TWCR);

    TWSR_reg_t __TWSR;
    __TWSR._TWPS0 = false;
    __TWSR._TWPS1 = false;
    __TWSR.reserved1 = 0;
    __TWSR._TWS3 = 1;
    __TWSR._TWS4 = 1;
    __TWSR._TWS5 = 1;
    __TWSR._TWS6 = 1;
    __TWSR._TWS7 = 1;
    dwrite(_TWSR, __TWSR);

    dwrite(_TWDR, (uint8_t)0xFF);

    TWAR_reg_t __TWAR;
    __TWAR._TWGCE = false;
    __TWAR._TWA = 0x7F;
    dwrite(_TWAR, __TWAR);

    TWAMR_reg_t __TWAMR;
    __TWAMR.reserved1 = false;
    __TWAMR._TWAM = 0;
    dwrite(_TWAMR, __TWAMR);
  #endif

  #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
    ADCSRB_reg_t __ADCSRB;
    __ADCSRB._ADTS = 0;
    #ifdef __AVR_TRM01__
      __ADCSRB._MUX5 = 0;
    #endif
    __ADCSRB.reserved1 = 0;
    __ADCSRB._ACME = false;
    __ADCSRB.reserved2 = 0;
    dwrite(_ADCSRB, __ADCSRB);

    ACSR_reg_t __ACSR;
    __ACSR._ACIS = 0;
    __ACSR._ACIC = false;
    __ACSR._ACIE = false;
    __ACSR._ACI = false;
    __ACSR._ACO = false;
    __ACSR._ACBG = false;
    __ACSR._ACD = false;
    dwrite(_ACSR, __ACSR);
  #endif

  #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
    DIDR1_reg_t __DIDR1;
    __DIDR1._AIN0D = false;
    __DIDR1._AIN1D = false;
    __DIDR1.reserved1 = false;
    dwrite(_DIDR1, __DIDR1);
  #endif

  #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
    ADMUX_reg_t __ADMUX;
    __ADMUX._MUX0 = 0;
    __ADMUX._MUX1 = 0;
    __ADMUX._MUX2 = 0;
    __ADMUX._MUX3 = 0;
    #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
      __ADMUX._MUX4 = 0;
    #elif defined(__AVR_TRM03__)
      __ADMUX.reserved1 = 0;
    #endif
    __ADMUX._ADLAR = 0;
    __ADMUX._REFS0 = 0;
    __ADMUX._REFS1 = 0;
    dwrite(_ADMUX, __ADMUX);

    ADCSRA_reg_t __ADCSRA;
    __ADCSRA._ADPS = 0;
    __ADCSRA._ADIE = false;
    __ADCSRA._ADIF = false;
    __ADCSRA._ADATE = false;
    __ADCSRA._ADSC = false;
    __ADCSRA._ADEN = false;
    dwrite(_ADCSRA, __ADCSRA);

    dwrite(_ADC, (uint16_t)0);
  #endif

  #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
    SPMCSR_reg_t __SPMCSR;
    #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
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
    dwrite(_SPMCSR, __SPMCSR);
  #endif

  // TODO: add the __AVR_TRM04__ initializations, if required (mostly USB related)
}

struct pin_dev_state_t {
  #ifdef __AVR_TRM01__
    uint8_t _SRE : 1;   // port A
    uint8_t _COM0B : 2;
    uint8_t _COM1A : 2;
    uint8_t _COM1B : 2;
    uint8_t _COM1C : 2;
    uint8_t _COM2A : 2;
    uint8_t _COM2B : 2;
    uint8_t _COM3A : 2;
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
    uint8_t _ADC7D : 1;
    uint8_t _ADC6D : 1;
    uint8_t _ADC5D : 1;
    uint8_t _ADC4D : 1;
    uint8_t _ADC3D : 1;
    uint8_t _ADC2D : 1;
    uint8_t _ADC1D : 1;
    uint8_t _ADC0D : 1;
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
    uint8_t _USART0_RXEN : 1;
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
    uint8_t _ADC7D : 1;
    uint8_t _ADC6D : 1;
    uint8_t _ADC5D : 1;
    uint8_t _ADC4D : 1;
    uint8_t _ADC3D : 1;
    uint8_t _ADC2D : 1;
    uint8_t _ADC1D : 1;
    uint8_t _ADC0D : 1;
    uint8_t _UMSEL : 2;
    uint8_t _USART0_TXEN : 1;
    uint8_t _USART0_RXEN : 1;
  #elif defined(__AVR_TRM04__)
    uint8_t _SRE : 1;
    uint8_t _SPE : 1;
    uint8_t _COM0B : 2;
    uint8_t _COM1C : 2;
    uint8_t _COM1B : 2;
    uint8_t _COM1A : 2;
    uint8_t _COM2A : 2;
    uint8_t _COM2B : 2;
    uint8_t _PCIE0 : 1;
    uint8_t _USART1_RXEN : 1;
    uint8_t _USART1_TXEN : 1;
    uint8_t _TWEN : 1;
    uint8_t _INT7 : 1;
    uint8_t _INT6 : 1;
    uint8_t _INT5 : 1;
    uint8_t _INT4 : 1;
    uint8_t _INT3 : 1;
    uint8_t _INT2 : 1;
    uint8_t _INT1 : 1;
    uint8_t _INT0;
    uint8_t _UVCONE : 1;
    uint8_t _UIDE : 1;
    //uint8_t _JTAGEN : 1;
  #elif defined(__AVR_TRM05__)
    uint8_t _ADC7D : 1;
    uint8_t _ADC6D : 1;
    uint8_t _ADC5D : 1;
    uint8_t _ADC4D : 1;
    uint8_t _ADC3D : 1;
    uint8_t _ADC2D : 1;
    uint8_t _ADC1D : 1;
    uint8_t _ADC0D : 1;
    uint8_t _PCIE0 : 1;
    uint8_t _PCIE1 : 1;
    uint8_t _PCIE2 : 1;
    uint8_t _PCIE3 : 1;
    uint8_t _SPE : 1;
    uint8_t _COM0A : 2;
    uint8_t _COM0B : 2;
    uint8_t _COM2A : 2;
    uint8_t _COM2B : 2;
    uint8_t _COM1A : 2;
    uint8_t _COM1B : 2;
    uint8_t _AS2 : 1;
    uint8_t _TWEN : 1;
    uint8_t _TXEN1 : 1;
    uint8_t _RXEN1 : 1;
    uint8_t _TXEN0 : 1;
    uint8_t _RXEN0 : 1;
    uint8_t _INT2 : 1;
    uint8_t _INT1 : 1;
    uint8_t _INT0 : 1;
    //uint8_t _JTAGEN : 1;
  #endif
};

// AVR ArduinoCore is written like a hack-job (random peripherals enabled all-the-time).

enum class eATmegaPort {
  #ifdef __AVR_TRM01__
    PORT_A, PORT_B, PORT_C, PORT_D, PORT_E, PORT_F, PORT_G, PORT_H, PORT_J, PORT_K, PORT_L
  #elif defined(__AVR_TRM02__) || defined(__AVR_TRM05__)
    PORT_A, PORT_B, PORT_C, PORT_D
  #elif defined(__AVR_TRM03__)
    PORT_B, PORT_C, PORT_D
  #elif defined(__AVR_TRM04__)
    PORT_A, PORT_B, PORT_C, PORT_D, PORT_E, PORT_F
  #endif
};

struct ATmegaPinInfo {
  eATmegaPort port;
  uint8_t pinidx;
};

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
  #define _SPA_DIO_DDRA (eATmegaPort::PORT_A)
#endif
#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM03__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
  #define _SPA_DIO_DDRB (eATmegaPort::PORT_B)
  #define _SPA_DIO_DDRC (eATmegaPort::PORT_C)
  #define _SPA_DIO_DDRD (eATmegaPort::PORT_D)
#endif
#if defined(__AVR_TRM01__) || defined(__AVR_TRM04__)
  #define _SPA_DIO_DDRE (eATmegaPort::PORT_E)
  #define _SPA_DIO_DDRF (eATmegaPort::PORT_F)
#endif
#ifdef __AVR_TRM01__
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
  #if defined(__AVR_TRM01__) || defined(__AVR_TRM02__) || defined(__AVR_TRM04__) || defined(__AVR_TRM05__)
    return { eATmegaPort::PORT_A, 0 };
  #elif defined(__AVR_TRM03__)
    return { eATmegaPort::PORT_B, 0 };
  #endif
}

enum class eATmegaPeripheral {
  UNDEFINED,
  #ifdef __AVR_TRM01__
    PADC, PUSART0, PSPI, PTIM1, PTIM0, PTIM2, PTWI, PUSART1, PUSART2, PUSART3, PTIM3, PTIM4, PTIM5
  #elif defined(__AVR_TRM02__)
    PADC, PUSART0, PSPI, PTIM1, PUSART1, PTIM0, PTIM2, PTWI, PTIM3
  #elif defined(__AVR_TRM03__)
    PADC, PUSART0, PSPI, PTIM1, PTIM0, PTIM2, PTWI
  #elif defined(__AVR_TRM04__)
    PADC, PSPI, PTIM1, PTIM0, PTIM2, PTWI, PUSART1, PTIM3, PUSB
  #elif defined(__AVR_TRM05__)
    PADC, PUSART0, PSPI, PTIM1, PUSART1, PTIM0, PTIM2, PTWI
  #endif
  , NUM_PERIPHERALS
};

enum class eATmegaPinFunc : uint8_t {
  #ifdef __AVR_TRM01__
    EXTMEM_AD15, EXTMEM_AD14, EXTMEM_AD13, EXTMEM_AD12, EXTMEM_AD11, EXTMEM_AD10, EXTMEM_AD9, EXTMEM_AD8,
    EXTMEM_AD7, EXTMEM_AD6, EXTMEM_AD5, EXTMEM_AD4, EXTMEM_AD3, EXTMEM_AD2, EXTMEM_AD1, EXTMEM_AD0,
    EXTMEM_ALE, EXTMEM_RD, EXTMEM_WR,
    TOC0A, TOC0B, TOC1A, TOC1B, TOC1C, TOC2A, TOC2B, TOC3C, TOC3B, TOC3A, TOC4C, TOC4B, TOC4A, TOC5C, TOC5B, TOC5A,
    EINT7, EINT6, EINT5, EINT4, EINT3, EINT2, EINT1, EINT0,
    PCI0, PCI1, PCI2, PCI3, PCI4, PCI5, PCI6, PCI7,
    PCI8, PCI9, PCI10, PCI11, PCI12, PCI13, PCI14, PCI15,
    PCI16, PCI17, PCI18, PCI19, PCI20, PCI21, PCI22, PCI23,
    SPI_MISO, SPI_MOSI, SPI_SCK, SPI_CS,
    TOSC1, TOSC2,
    TIMER0_CLKI, TIMER1_CLKI, TIMER3_CLKI, TIMER4_CLKI, TIMER5_CLKI,
    TIMER1_ICP, TIMER3_ICP, TIMER5_ICP, TIMER4_ICP,
    USART0_CLK, USART1_CLK, USART2_CLK, USART3_CLK,
    USART0_TXD, USART0_RXD, USART1_TXD, USART1_RXD, USART2_TXD, USART2_RXD, USART3_TXD, USART3_RXD,
    TWI_SDA, TWI_CLK,
    CLKO, PDO, PDI,
    AIN0, AIN1,
    ADC15, ADC14, ADC13, ADC12, ADC11, ADC10, ADC9, ADC8,
    ADC7, ADC6, ADC5, ADC4, ADC3, ADC2, ADC1, ADC0
  #elif defined(__AVR_TRM02__)
    ADC7, ADC6, ADC5, ADC4, ADC3, ADC2, ADC1, ADC0,
    SPI_SCK, SPI_MISO, SPI_MOSI, SPI_CS,
    PCI31, PCI30, PCI29, PCI28, PCI27, PCI26, PCI25, PCI24,
    PCI23, PCI22, PCI21, PCI20, PCI19, PCI18, PCI17, PCI16,
    PCI15, PCI14, PCI13, PCI12, PCI11, PCI10, PCI9, PCI8,
    PCI7, PCI6, PCI5, PCI4, PCI3, PCI2, PCI1, PCI0,
    EINT2, EINT1, EINT0,
    TIMER3_ICP,
    TIMER3_ECI, TIMER1_ECI, TIMER0_ECI,
    TIMER1_ICP,
    TOC3B, TOC3A, TOC2A, TOC2B, TOC1A, TOC1B, TOC0B, TOC0A,
    AIN1, AIN0,
    USART0_CLK, USART1_CLK,
    USART0_TXD, USART0_RXD, USART1_TXD, USART1_RXD,
    CLKO,
    TOSC2, TOSC1,
    TWI_SDA, TWI_CLK
  #elif defined(__AVR_TRM03__)
    ADC5, ADC4, ADC3, ADC2, ADC1, ADC0,
    XTAL2, XTAL1,
    TOSC2, TOSC1,
    SPI_SCK, SPI_MISO, SPI_MOSI, SPI_CS,
    TOC2B, TOC2A, TOC1B, TOC1A, TOC0A, TOC0B,
    TIMER1_ICP,
    TIMER1_ECI, TIMER0_ECI,
    TWI_CLK, TWI_SDA,
    PCI23, PCI22, PCI21, PCI20, PCI19, PCI18, PCI17, PCI16,
    PCI14, PCI13, PCI12, PCI11, PCI10, PCI9, PCI8,
    PCI7, PCI6, PCI5, PCI4, PCI3, PCI2, PCI1, PCI0,
    CLKO,
    AIN1, AIN0,
    USART_CLK,
    USART_TXD, USART_RXD,
    EINT1, EINT0
  #elif defined(__AVR_TRM04__)
    EXTMEM_AD15, EXTMEM_AD14, EXTMEM_AD13, EXTMEM_AD12, EXTMEM_AD11, EXTMEM_AD10, EXTMEM_AD9, EXTMEM_AD8,
    EXTMEM_AD7, EXTMEM_AD6, EXTMEM_AD5, EXTMEM_AD4, EXTMEM_AD3, EXTMEM_AD2, EXTMEM_AD1, EXTMEM_AD0,
    EXTMEM_ALE, EXTMEM_RD, EXTMEM_WR,
    TOC0B, TOC0A, TOC1C, TOC1B, TOC1A, TOC2B, TOC2A, TOC3A, TOC3B, TOC3C,
    CLKO, PDO, PDI,
    SPI_MISO, SPI_MOSI, SPI_SCK, SPI_CS,
    TIMER3_ICP, TIMER1_ICP,
    TIMER3_CLKI, TIMER0_CLKI, TIMER1_CLKI,
    USART1_CLK, USART1_TXD, USART1_RXD,
    EINT7, EINT6, EINT5, EINT4, EINT3, EINT2, EINT1, EINT0,
    PCI7, PCI6, PCI5, PCI4, PCI3, PCI2, PCI1, PCI0,
    TWI_SDA, TWI_CLK,
    AIN1, AIN0,
    TOSC2,
    UID, UVCON,
    ADC7, ADC6, ADC5, ADC4, ADC3, ADC2, ADC1, ADC0
  #elif defined(__AVR_TRM05__)
    ADC7, ADC6, ADC5, ADC4, ADC3, ADC2, ADC1, ADC0,
    PCI31, PCI30, PCI29, PCI28, PCI27, PCI26, PCI25, PCI24,
    PCI23, PCI22, PCI21, PCI20, PCI19, PCI18, PCI17, PCI16,
    PCI15, PCI14, PCI13, PCI12, PCI11, PCI10, PCI9, PCI8,
    PCI7, PCI6, PCI5, PCI4, PCI3, PCI2, PCI1, PCI0,
    SPI_SCK, SPI_MISO, SPI_MOSI, SPI_CS,
    AIN1, AIN0,
    TIMER1_ICP, TIMER0_ICP,
    TIMER1_ECI, TIMER0_ECI,
    TOC0B, TOC0A, TOC2A, TOC2B, TOC1A, TOC1B,
    TOSC2, TOSC1,
    //JTAG_TDI, JTAG_TDO, JTAG_TMS, JTAG_TCK,
    TWI_CLK, TWI_SDA,
    EINT2, EINT1, EINT0,
    CLKO,
    USART0_CLK, USART0_TXD, USART0_RXD,
    USART1_CLK, USART1_TXD, USART1_RXD
  #endif
  , NUM_FUNCS
};

#ifndef countof
  #define countof(x) (sizeof(x) / sizeof(*x))
#endif

struct ATmegaPinFunctions {
  inline ATmegaPinFunctions(const eATmegaPinFunc *funcs, uint8_t cnt) noexcept : funcs(funcs), cnt(cnt) {}
  inline ATmegaPinFunctions() = default;
  inline ATmegaPinFunctions(const ATmegaPinFunctions&) = default;

  const eATmegaPinFunc *funcs = nullptr;
  uint8_t cnt = 0;

  inline bool hasFunc(eATmegaPinFunc query) const {
    for (uint8_t n = 0; n < this->cnt; n++) {
      eATmegaPinFunc func = this->funcs[n];
      if (func == query) return true;
    }
    return false;
  }
  template <typename... otherItemType>
  inline bool hasFunc(eATmegaPinFunc func, otherItemType&&... items) const {
    return hasFunc(func) || hasFunc(((otherItemType&&)items)...);
  }

  template <typename callbackType>
  inline void iterate(callbackType&& cb) const {
    for (uint8_t n = 0; n < this->cnt; n++) {
      eATmegaPinFunc func = this->funcs[n];
      cb(func);
    }
  }
};

ATmegaPinFunctions _ATmega_getPinFunctions(int pin);

struct ATmegaPinFuncSet {
  inline ATmegaPinFuncSet() noexcept {
    for (bool& f : this->funcs) f = false;
  }
  template <typename... funcItemType>
  inline ATmegaPinFuncSet(eATmegaPinFunc func, funcItemType&&... items) noexcept : ATmegaPinFuncSet() {
    add(func, ((funcItemType&&)items)...);
  }
  template <typename... funcItemType>
  inline ATmegaPinFuncSet(int pin, funcItemType&&... items) noexcept : ATmegaPinFuncSet() {
    addFromPin(pin, ((funcItemType&&)items)...);
  }
  inline ATmegaPinFuncSet(const ATmegaPinFuncSet&) = default;

  inline void add(eATmegaPinFunc value) noexcept {
    this->funcs[(uint8_t)value] = true;
  }
  template <typename... funcItemType>
  inline void add(eATmegaPinFunc value, funcItemType&&... items) {
    add(value);
    add(((eATmegaPinFunc&&)items)...);
  }

  inline void addFromPin(int pin) noexcept {
    ATmegaPinFunctions funcs = _ATmega_getPinFunctions(pin);
    funcs.iterate(
      [this]( eATmegaPinFunc func ) noexcept { this->add(func); }
    );
  }
  template <typename... itemType>
  inline void addFromPin(int pin, itemType&&... items) noexcept {
    addFromPin(pin);
    addFromPin(((itemType&&)items)...);
  }

  inline bool hasFunc(eATmegaPinFunc value) const noexcept {
    return this->funcs[(uint8_t)value];
  }

  inline bool hasAnyFunc() const noexcept { return false; }
  template <typename funcItem, typename... otherFuncItem>
  inline bool hasAnyFunc(funcItem&& item, otherFuncItem&&... funcs) const noexcept {
    return hasFunc(item) || hasAnyFunc(((otherFuncItem&&)funcs)...);
  }

  template <typename callbackType>
  inline void iterate(callbackType&& cb) const {
    for (uint8_t n = 1; n < countof(this->funcs); n++) {
      const bool& f = this->funcs[n];
      if (f) cb((eATmegaPinFunc)n);
    }
  }

private:
  bool funcs[(uint8_t)eATmegaPinFunc::NUM_FUNCS];
};

inline void _ATmega_setPeripheralPower(eATmegaPeripheral peri, bool fullPower) {
  bool reducePower = (fullPower == false);
  switch(peri) {
    #ifdef __AVR_TRM01__
      case eATmegaPeripheral::PADC: _PRR0._PRADC = reducePower; break;
      case eATmegaPeripheral::PUSART0: _PRR0._PRUSART0 = reducePower; break;
      case eATmegaPeripheral::PSPI: _PRR0._PRSPI = reducePower; break;
      case eATmegaPeripheral::PTIM1: _PRR0._PRTIM1 = reducePower; break;
      case eATmegaPeripheral::PTIM0: _PRR0._PRTIM0 = reducePower; break;
      case eATmegaPeripheral::PTIM2: _PRR0._PRTIM2 = reducePower; break;
      case eATmegaPeripheral::PTWI: _PRR0._PRTWI = reducePower; break;
      case eATmegaPeripheral::PUSART1: _PRR1._PRUSART1 = reducePower; break;
      case eATmegaPeripheral::PUSART2: _PRR1._PRUSART2 = reducePower; break;
      case eATmegaPeripheral::PUSART3: _PRR1._PRUSART3 = reducePower; break;
      case eATmegaPeripheral::PTIM3: _PRR1._PRTIM3 = reducePower; break;
      case eATmegaPeripheral::PTIM4: _PRR1._PRTIM4 = reducePower; break;
      case eATmegaPeripheral::PTIM5: _PRR1._PRTIM5 = reducePower; break;
    #elif defined(__AVR_TRM02__)
      case eATmegaPeripheral::PADC: _PRR0._PRADC = reducePower; break;
      case eATmegaPeripheral::PUSART0: _PRR0._PRUSART0 = reducePower; break;
      case eATmegaPeripheral::PSPI: _PRR0._PRSPI = reducePower; break;
      case eATmegaPeripheral::PTIM1: _PRR0._PRTIM1 = reducePower; break;
      case eATmegaPeripheral::PUSART1: _PRR0._PRUSART1 = reducePower; break;
      case eATmegaPeripheral::PTIM0: _PRR0._PRTIM0 = reducePower; break;
      case eATmegaPeripheral::PTIM2: _PRR0._PRTIM2 = reducePower; break;
      case eATmegaPeripheral::PTWI: _PRR0._PRTWI = reducePower; break;
      case eATmegaPeripheral::PTIM3: _PRR1._PRTIM3 = reducePower; break;
    #elif defined(__AVR_TRM03__)
      case eATmegaPeripheral::PADC: _PRR0._PRADC = reducePower; break;
      case eATmegaPeripheral::PUSART0: _PRR0._PRUSART0 = reducePower; break;
      case eATmegaPeripheral::PSPI: _PRR0._PRSPI = reducePower; break;
      case eATmegaPeripheral::PTIM1: _PRR0._PRTIM1 = reducePower; break;
      case eATmegaPeripheral::PTIM0: _PRR0._PRTIM0 = reducePower; break;
      case eATmegaPeripheral::PTIM2: _PRR0._PRTIM2 = reducePower; break;
      case eATmegaPeripheral::PTWI: _PRR0._PRTWI = reducePower; break;
    #elif defined(__AVR_TRM04__)
      case eATmegaPeripheral::PADC: _PRR0._PRADC = reducePower; break;
      case eATmegaPeripheral::PSPI: _PRR0._PRSPI = reducePower; break;
      case eATmegaPeripheral::PTIM1: _PRR0._PRTIM1 = reducePower; break;
      case eATmegaPeripheral::PTIM0: _PRR0._PRTIM0 = reducePower; break;
      case eATmegaPeripheral::PTIM2: _PRR0._PRTIM2 = reducePower; break;
      case eATmegaPeripheral::PTWI: _PRR0._PRTWI = reducePower; break;
      case eATmegaPeripheral::PUSART1: _PRR1._PRUSART1 = reducePower; break;
      case eATmegaPeripheral::PTIM3: _PRR1._PRTIM3 = reducePower; break;
      case eATmegaPeripheral::PUSB: _PRR1._PRUSB = reducePower; break;
    #elif defined(__AVR_TRM05__)
      case eATmegaPeripheral::PADC: _PRR0._PRADC = reducePower; break;
      case eATmegaPeripheral::PUSART0: _PRR0._PRUSART0 = reducePower; break;
      case eATmegaPeripheral::PSPI: _PRR0._PRSPI = reducePower; break;
      case eATmegaPeripheral::PTIM1: _PRR0._PRTIM1 = reducePower; break;
      case eATmegaPeripheral::PUSART1: _PRR0._PRUSART1 = reducePower; break;
      case eATmegaPeripheral::PTIM0: _PRR0._PRTIM0 = reducePower; break;
      case eATmegaPeripheral::PTIM2: _PRR0._PRTIM2 = reducePower; break;
      case eATmegaPeripheral::PTWI: _PRR0._PRTWI = reducePower; break;
    #endif
  case eATmegaPeripheral::UNDEFINED: case eATmegaPeripheral::NUM_PERIPHERALS: break;
  }
}

inline bool _ATmega_getPeripheralPower(eATmegaPeripheral peri) {
  switch(peri) {
    #ifdef __AVR_TRM01__
      case eATmegaPeripheral::PADC: return _PRR0._PRADC == false;
      case eATmegaPeripheral::PUSART0: return _PRR0._PRUSART0 == false;
      case eATmegaPeripheral::PSPI: return _PRR0._PRSPI == false;
      case eATmegaPeripheral::PTIM1: return _PRR0._PRTIM1 == false;
      case eATmegaPeripheral::PTIM0: return _PRR0._PRTIM0 == false;
      case eATmegaPeripheral::PTIM2: return _PRR0._PRTIM2 == false;
      case eATmegaPeripheral::PTWI: return _PRR0._PRTWI == false;
      case eATmegaPeripheral::PUSART1: return _PRR1._PRUSART1 == false;
      case eATmegaPeripheral::PUSART2: return _PRR1._PRUSART2 == false;
      case eATmegaPeripheral::PUSART3: return _PRR1._PRUSART3 == false;
      case eATmegaPeripheral::PTIM3: return _PRR1._PRTIM3 == false;
      case eATmegaPeripheral::PTIM4: return _PRR1._PRTIM4 == false;
      case eATmegaPeripheral::PTIM5: return _PRR1._PRTIM5 == false;
    #elif defined(__AVR_TRM02__)
      case eATmegaPeripheral::PADC: return _PRR0._PRADC == false;
      case eATmegaPeripheral::PUSART0: return _PRR0._PRUSART0 == false;
      case eATmegaPeripheral::PSPI: return _PRR0._PRSPI == false;
      case eATmegaPeripheral::PTIM1: return _PRR0._PRTIM1 == false;
      case eATmegaPeripheral::PUSART1: return _PRR0._PRUSART1 == false;
      case eATmegaPeripheral::PTIM0: return _PRR0._PRTIM0 == false;
      case eATmegaPeripheral::PTIM2: return _PRR0._PRTIM2 == false;
      case eATmegaPeripheral::PTWI: return _PRR0._PRTWI == false;
      case eATmegaPeripheral::PTIM3: return _PRR1._PRTIM3 == false;
    #elif defined(__AVR_TRM03__)
      case eATmegaPeripheral::PADC: return _PRR0._PRADC == false;
      case eATmegaPeripheral::PUSART0: return _PRR0._PRUSART0 == false;
      case eATmegaPeripheral::PSPI: return _PRR0._PRSPI == false;
      case eATmegaPeripheral::PTIM1: return _PRR0._PRTIM1 == false;
      case eATmegaPeripheral::PTIM0: return _PRR0._PRTIM0 == false;
      case eATmegaPeripheral::PTIM2: return _PRR0._PRTIM2 == false;
      case eATmegaPeripheral::PTWI: return _PRR0._PRTWI == false;
    #elif defined(__AVR_TRM04__)
      case eATmegaPeripheral::PADC: return _PRR0._PRADC == false;
      case eATmegaPeripheral::PSPI: return _PRR0._PRSPI == false;
      case eATmegaPeripheral::PTIM1: return _PRR0._PRTIM1 == false;
      case eATmegaPeripheral::PTIM0: return _PRR0._PRTIM0 == false;
      case eATmegaPeripheral::PTIM2: return _PRR0._PRTIM2 == false;
      case eATmegaPeripheral::PTWI: return _PRR0._PRTWI == false;
      case eATmegaPeripheral::PUSART1: return _PRR1._PRUSART1 == false;
      case eATmegaPeripheral::PTIM3: return _PRR1._PRTIM3 == false;
      case eATmegaPeripheral::PUSB: return _PRR1._PRUSB == false;
    #elif defined(__AVR_TRM05__)
      case eATmegaPeripheral::PADC: return _PRR0._PRADC == false;
      case eATmegaPeripheral::PUSART0: return _PRR0._PRUSART0 == false;
      case eATmegaPeripheral::PSPI: return _PRR0._PRSPI == false;
      case eATmegaPeripheral::PTIM1: return _PRR0._PRTIM1 == false;
      case eATmegaPeripheral::PUSART1: return _PRR0._PRUSART1 == false;
      case eATmegaPeripheral::PTIM0: return _PRR0._PRTIM0 == false;
      case eATmegaPeripheral::PTIM2: return _PRR0._PRTIM2 == false;
      case eATmegaPeripheral::PTWI: return _PRR0._PRTWI == false;
    #endif
    case eATmegaPeripheral::UNDEFINED: case eATmegaPeripheral::NUM_PERIPHERALS: break;
  }
  return false;
}

inline eATmegaPeripheral _ATmega_getPeripheralForFunc( eATmegaPinFunc func ) {
  // In C++20 there is the "using-enum" statement. I wish we had C++20 over here...
  //using enum eATmegaPinFunc;
  switch(func) {
    #ifdef __AVR_TRM01__
      case eATmegaPinFunc::TOC0A: case eATmegaPinFunc::TOC0B: return eATmegaPeripheral::PTIM0;
      case eATmegaPinFunc::TOC1A: case eATmegaPinFunc::TOC1B: case eATmegaPinFunc::TOC1C: return eATmegaPeripheral::PTIM1;
      case eATmegaPinFunc::TOC2A: case eATmegaPinFunc::TOC2B: return eATmegaPeripheral::PTIM2;
      case eATmegaPinFunc::TOC3A: case eATmegaPinFunc::TOC3B: case eATmegaPinFunc::TOC3C: return eATmegaPeripheral::PTIM3;
      case eATmegaPinFunc::TOC4A: case eATmegaPinFunc::TOC4B: case eATmegaPinFunc::TOC4C: return eATmegaPeripheral::PTIM4;
      case eATmegaPinFunc::TOC5A: case eATmegaPinFunc::TOC5B: case eATmegaPinFunc::TOC5C: return eATmegaPeripheral::PTIM5;
      case eATmegaPinFunc::SPI_MISO: case eATmegaPinFunc::SPI_MOSI: case eATmegaPinFunc::SPI_SCK: case eATmegaPinFunc::SPI_CS: return eATmegaPeripheral::PSPI;
      case eATmegaPinFunc::TIMER0_CLKI: return eATmegaPeripheral::PTIM0;
      case eATmegaPinFunc::TIMER1_CLKI: case eATmegaPinFunc::TIMER1_ICP: return eATmegaPeripheral::PTIM1;
      case eATmegaPinFunc::TIMER3_CLKI: case eATmegaPinFunc::TIMER3_ICP: return eATmegaPeripheral::PTIM3;
      case eATmegaPinFunc::TIMER4_CLKI: case eATmegaPinFunc::TIMER4_ICP: return eATmegaPeripheral::PTIM4;
      case eATmegaPinFunc::TIMER5_CLKI: case eATmegaPinFunc::TIMER5_ICP: return eATmegaPeripheral::PTIM5;
      case eATmegaPinFunc::USART0_CLK: case eATmegaPinFunc::USART0_TXD: case eATmegaPinFunc::USART0_RXD: return eATmegaPeripheral::PUSART0;
      case eATmegaPinFunc::USART1_CLK: case eATmegaPinFunc::USART1_TXD: case eATmegaPinFunc::USART1_RXD: return eATmegaPeripheral::PUSART1;
      case eATmegaPinFunc::USART2_CLK: case eATmegaPinFunc::USART2_TXD: case eATmegaPinFunc::USART2_RXD: return eATmegaPeripheral::PUSART2;
      case eATmegaPinFunc::USART3_CLK: case eATmegaPinFunc::USART3_TXD: case eATmegaPinFunc::USART3_RXD: return eATmegaPeripheral::PUSART3;
      case eATmegaPinFunc::TWI_SDA: case eATmegaPinFunc::TWI_CLK: return eATmegaPeripheral::PTWI;
      case eATmegaPinFunc::ADC15: case eATmegaPinFunc::ADC14: case eATmegaPinFunc::ADC13: case eATmegaPinFunc::ADC12: case eATmegaPinFunc::ADC11: case eATmegaPinFunc::ADC10: case eATmegaPinFunc::ADC9: case eATmegaPinFunc::ADC8:
      case eATmegaPinFunc::ADC7: case eATmegaPinFunc::ADC6: case eATmegaPinFunc::ADC5: case eATmegaPinFunc::ADC4: case eATmegaPinFunc::ADC3: case eATmegaPinFunc::ADC2: case eATmegaPinFunc::ADC1: case eATmegaPinFunc::ADC0:
        return eATmegaPeripheral::PADC;
    #elif defined(__AVR_TRM02__)
      case eATmegaPinFunc::ADC7: case eATmegaPinFunc::ADC6: case eATmegaPinFunc::ADC5: case eATmegaPinFunc::ADC4: case eATmegaPinFunc::ADC3: case eATmegaPinFunc::ADC2: case eATmegaPinFunc::ADC1: case eATmegaPinFunc::ADC0:
        return eATmegaPeripheral::PADC;
      case eATmegaPinFunc::SPI_SCK: case eATmegaPinFunc::SPI_MISO: case eATmegaPinFunc::SPI_MOSI: case eATmegaPinFunc::SPI_CS: return eATmegaPeripheral::PSPI;
      case eATmegaPinFunc::TIMER3_ICP: case eATmegaPinFunc::TIMER3_ECI: return eATmegaPeripheral::PTIM3;
      case eATmegaPinFunc::TIMER1_ECI: case eATmegaPinFunc::TIMER1_ICP: return eATmegaPeripheral::PTIM1;
      case eATmegaPinFunc::TIMER0_ECI: return eATmegaPeripheral::PTIM0;
      case eATmegaPinFunc::TOC3B: case eATmegaPinFunc::TOC3A: return eATmegaPeripheral::PTIM3;
      case eATmegaPinFunc::TOC2A: case eATmegaPinFunc::TOC2B: return eATmegaPeripheral::PTIM2;
      case eATmegaPinFunc::TOC1A: case eATmegaPinFunc::TOC1B: return eATmegaPeripheral::PTIM1;
      case eATmegaPinFunc::TOC0B: case eATmegaPinFunc::TOC0A: return eATmegaPeripheral::PTIM0;
      case eATmegaPinFunc::USART0_CLK: case eATmegaPinFunc::USART0_TXD: case eATmegaPinFunc::USART0_RXD: return eATmegaPeripheral::PUSART0;
      case eATmegaPinFunc::USART1_CLK: case eATmegaPinFunc::USART1_TXD: case eATmegaPinFunc::USART1_RXD: return eATmegaPeripheral::PUSART1;
      case eATmegaPinFunc::TWI_SDA: case eATmegaPinFunc::TWI_CLK: return eATmegaPeripheral::PTWI;
    #elif defined(__AVR_TRM03__)
      case eATmegaPinFunc::ADC5: case eATmegaPinFunc::ADC4: case eATmegaPinFunc::ADC3: case eATmegaPinFunc::ADC2: case eATmegaPinFunc::ADC1: case eATmegaPinFunc::ADC0:
        return eATmegaPeripheral::PADC;
      case eATmegaPinFunc::SPI_SCK: case eATmegaPinFunc::SPI_MISO: case eATmegaPinFunc::SPI_MOSI: case eATmegaPinFunc::SPI_CS: return eATmegaPeripheral::PSPI;
      case eATmegaPinFunc::TOC2B: case eATmegaPinFunc::TOC2A: return eATmegaPeripheral::PTIM2;
      case eATmegaPinFunc::TOC1B: case eATmegaPinFunc::TOC1A: return eATmegaPeripheral::PTIM1;
      case eATmegaPinFunc::TOC0A: case eATmegaPinFunc::TOC0B: return eATmegaPeripheral::PTIM0;
      case eATmegaPinFunc::TIMER1_ICP: case eATmegaPinFunc::TIMER1_ECI: return eATmegaPeripheral::PTIM1;
      case eATmegaPinFunc::TIMER0_ECI: return eATmegaPeripheral::PTIM0;
      case eATmegaPinFunc::TWI_CLK: case eATmegaPinFunc::TWI_SDA: return eATmegaPeripheral::PTWI;
      case eATmegaPinFunc::USART_CLK: case eATmegaPinFunc::USART_TXD: case eATmegaPinFunc::USART_RXD: return eATmegaPeripheral::PUSART0;
    #elif defined(__AVR_TRM04__)
      case eATmegaPinFunc::TOC0B: case eATmegaPinFunc::TOC0A: return eATmegaPeripheral::PTIM0;
      case eATmegaPinFunc::TOC1C: case eATmegaPinFunc::TOC1B: case eATmegaPinFunc::TOC1A: return eATmegaPeripheral::PTIM1;
      case eATmegaPinFunc::TOC2B: case eATmegaPinFunc::TOC2A: return eATmegaPeripheral::PTIM2;
      case eATmegaPinFunc::TOC3A: case eATmegaPinFunc::TOC3B: case eATmegaPinFunc::TOC3C: return eATmegaPeripheral::PTIM3;
      case eATmegaPinFunc::SPI_MISO: case eATmegaPinFunc::SPI_MOSI: case eATmegaPinFunc::SPI_SCK: case eATmegaPinFunc::SPI_CS: return eATmegaPeripheral::PSPI;
      case eATmegaPinFunc::TIMER3_ICP: case eATmegaPinFunc::TIMER3_CLKI: return eATmegaPeripheral::PTIM3;
      case eATmegaPinFunc::TIMER1_ICP: case eATmegaPinFunc::TIMER1_CLKI: return eATmegaPeripheral::PTIM1;
      case eATmegaPinFunc::TIMER0_CLKI: return eATmegaPeripheral::PTIM0;
      case eATmegaPinFunc::USART1_CLK: case eATmegaPinFunc::USART1_TXD: case eATmegaPinFunc::USART1_RXD: return eATmegaPeripheral::PUSART1;
      case eATmegaPinFunc::TWI_SDA: case eATmegaPinFunc::TWI_CLK: return eATmegaPeripheral::PTWI;
      case eATmegaPinFunc::UID: case eATmegaPinFunc::UVCON: return eATmegaPeripheral::PUSB;
      case eATmegaPinFunc::ADC7: case eATmegaPinFunc::ADC6: case eATmegaPinFunc::ADC5: case eATmegaPinFunc::ADC4: case eATmegaPinFunc::ADC3: case eATmegaPinFunc::ADC2: case eATmegaPinFunc::ADC1: case eATmegaPinFunc::ADC0:
        return eATmegaPeripheral::PADC;
    #elif defined(__AVR_TRM05__)
      case eATmegaPinFunc::ADC7: case eATmegaPinFunc::ADC6: case eATmegaPinFunc::ADC5: case eATmegaPinFunc::ADC4: case eATmegaPinFunc::ADC3: case eATmegaPinFunc::ADC2: case eATmegaPinFunc::ADC1: case eATmegaPinFunc::ADC0:
        return eATmegaPeripheral::PADC;
      case eATmegaPinFunc::SPI_MISO: case eATmegaPinFunc::SPI_MOSI: case eATmegaPinFunc::SPI_SCK: case eATmegaPinFunc::SPI_CS: return eATmegaPeripheral::PSPI;
      case eATmegaPinFunc::TIMER1_ICP: case eATmegaPinFunc::TIMER1_ECI: return eATmegaPeripheral::PTIM1;
      case eATmegaPinFunc::TIMER0_ICP: case eATmegaPinFunc::TIMER0_ECI: return eATmegaPeripheral::PTIM0;
      case eATmegaPinFunc::TOC0B: case eATmegaPinFunc::TOC0A: return eATmegaPeripheral::PTIM0;
      case eATmegaPinFunc::TOC1A: case eATmegaPinFunc::TOC1B: return eATmegaPeripheral::PTIM1;
      case eATmegaPinFunc::TOC2A: case eATmegaPinFunc::TOC2B: return eATmegaPeripheral::PTIM2;
      case eATmegaPinFunc::TWI_CLK: case eATmegaPinFunc::TWI_SDA: return eATmegaPeripheral::PTWI;
      case eATmegaPinFunc::USART0_CLK: case eATmegaPinFunc::USART0_TXD: case eATmegaPinFunc::USART0_RXD: return eATmegaPeripheral::PUSART0;
      case eATmegaPinFunc::USART1_CLK: case eATmegaPinFunc::USART1_TXD: case eATmegaPinFunc::USART1_RXD: return eATmegaPeripheral::PUSART1;
    #endif
    // There are quite some pin functions that have no peripheral assignment, and that is OK!
    default: break;
  }
  return eATmegaPeripheral::UNDEFINED;
}

struct ATmegaPeripheralSet {
  inline ATmegaPeripheralSet() noexcept {
    for (bool& f : this->funcs) f = false;
  }
  template <typename... funcItemType>
  inline ATmegaPeripheralSet(funcItemType&&... items) noexcept : ATmegaPinFuncSet() {
    add(((eATmegaPinFunc&&)items)...);
  }
  inline ATmegaPeripheralSet(const ATmegaPeripheralSet&) = default;

  inline void add(eATmegaPeripheral value) noexcept {
    this->funcs[(uint8_t)value] = true;
  }
  template <typename... funcItemType>
  inline void add(eATmegaPeripheral value, funcItemType&&... items) noexcept {
    add(value);
    add(((funcItemType&&)items)...);
  }

  inline bool hasItem(eATmegaPeripheral value) const noexcept {
    return this->funcs[(uint8_t)value];
  }
  template <typename... otherFuncItem>
  inline bool hasItem(eATmegaPeripheral&& item, otherFuncItem&&... funcs) const noexcept {
    return hasItem(item) || hasItem(((otherFuncItem&&)funcs)...);
  }

  template <typename callbackType>
  inline void iterate(callbackType&& cb) const {
    for (uint8_t n = 1; n < countof(funcs); n++) {
      const bool& f = this->funcs[n];
      if (f) cb( (eATmegaPeripheral)n );
    }
  }

  inline void fromPinFuncs(const ATmegaPinFuncSet& funcSet) {
    funcSet.iterate(
      [this]( eATmegaPinFunc func ) noexcept {
        this->add( _ATmega_getPeripheralForFunc(func) );
      }
    );
  }

private:
  bool funcs[(uint8_t)eATmegaPeripheral::NUM_PERIPHERALS];
};

struct ATmegaPeripheralPowerGate {
  inline ATmegaPeripheralPowerGate(ATmegaPeripheralSet& periSet) noexcept : periSet(periSet) {
    periSet.iterate(
      [this]( eATmegaPeripheral peri ) noexcept {
        this->states[(uint8_t)peri] = _ATmega_getPeripheralPower(peri);
        _ATmega_setPeripheralPower(peri, true);
      }
    );
  }
  inline ATmegaPeripheralPowerGate(const ATmegaPeripheralPowerGate&) = delete;

  inline ~ATmegaPeripheralPowerGate() {
    periSet.iterate(
      [this]( eATmegaPeripheral peri ) noexcept {
        _ATmega_setPeripheralPower(peri, this->states[(uint8_t)peri]);
      }
    );
  }

  inline ATmegaPeripheralPowerGate& operator = (const ATmegaPeripheralPowerGate&) = delete;

private:
  ATmegaPeripheralSet& periSet;
  bool states[(uint8_t)eATmegaPeripheral::NUM_PERIPHERALS];
};

inline pin_dev_state_t _ATmega_savePinAlternates(const ATmegaPinFuncSet& funcSet) {
  // TODO: the manual states that registers of power-reduced peripherals cannot be read or written, and that
  // the resources (GPIO pins) remain occupied during power-reduction. This is a serious problem and we should
  // add power-reduction awareness to this logic!

  pin_dev_state_t state;

  ATmegaPeripheralSet periSet;
  periSet.fromPinFuncs(funcSet);

  ATmegaPeripheralPowerGate pgate(periSet);

  #ifdef __AVR_TRM01__
    // See page 75ff of ATmega2560 technical reference manual.
    if (funcSet.hasAnyFunc(
      eATmegaPinFunc::EXTMEM_AD15, eATmegaPinFunc::EXTMEM_AD14, eATmegaPinFunc::EXTMEM_AD13, eATmegaPinFunc::EXTMEM_AD12, eATmegaPinFunc::EXTMEM_AD11, eATmegaPinFunc::EXTMEM_AD10, eATmegaPinFunc::EXTMEM_AD9, eATmegaPinFunc::EXTMEM_AD8,
      eATmegaPinFunc::EXTMEM_AD7, eATmegaPinFunc::EXTMEM_AD6, eATmegaPinFunc::EXTMEM_AD5, eATmegaPinFunc::EXTMEM_AD4, eATmegaPinFunc::EXTMEM_AD3, eATmegaPinFunc::EXTMEM_AD2, eATmegaPinFunc::EXTMEM_AD1, eATmegaPinFunc::EXTMEM_AD0,
      eATmegaPinFunc::EXTMEM_ALE, eATmegaPinFunc::EXTMEM_RD, eATmegaPinFunc::EXTMEM_WR
    )) {
      state._SRE = _XMCRA._SRE;
      _XMCRA._SRE = false;
    }
    if (funcSet.hasAnyFunc(
      eATmegaPinFunc::PCI0, eATmegaPinFunc::PCI1, eATmegaPinFunc::PCI2, eATmegaPinFunc::PCI3, eATmegaPinFunc::PCI4, eATmegaPinFunc::PCI5, eATmegaPinFunc::PCI6, eATmegaPinFunc::PCI7
    )) {
      state._PCIE0 = _PCICR._PCIE0;
      _PCICR._PCIE0 = false;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC1C)) {
      state._COM1C = TIMER1._TCCRnA._COMnC;
      TIMER1._TCCRnA._COMnC = 0;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC1B)) {
      state._COM1B = TIMER1._TCCRnA._COMnB;
      TIMER1._TCCRnA._COMnB = 0;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC1A)) {
      state._COM1A = TIMER1._TCCRnA._COMnA;
      TIMER1._TCCRnA._COMnA = 0;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC2A)) {
      state._COM2A = _TIMER2._TCCRnA._COMnA;
      _TIMER2._TCCRnA._COMnA = 0;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::USART1_TXD, eATmegaPinFunc::USART1_CLK)) {
      state._USART1_TXEN = USART1._UCSRnB._TXEN;
      USART1._UCSRnB._TXEN = false;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::USART1_RXD, eATmegaPinFunc::USART1_CLK)) {
      state._USART1_RXEN = USART1._UCSRnB._RXEN;
      USART1._UCSRnB._RXEN = false;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC3C)) {
      state._COM3C = TIMER3._TCCRnA._COMnC;
      TIMER3._TCCRnA._COMnC = 0;
    }
    // There is an error in the technical reference manual signal mapping table
    // of ATmega2560 where is says that pin 3 is mapped to OC3B, but the list
    // says OC3A.
    if (funcSet.hasFunc(eATmegaPinFunc::TOC3B)) {
      state._COM3B = TIMER3._TCCRnA._COMnB;
      TIMER3._TCCRnA._COMnB = 0;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC3A)) {
      state._COM3A = TIMER3._TCCRnA._COMnA;
      TIMER3._TCCRnA._COMnA = 0;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::USART0_RXD, eATmegaPinFunc::USART0_CLK)) {
      state._USART0_RXEN = USART0._UCSRnB._RXEN;
      USART0._UCSRnB._RXEN = false;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::USART0_TXD, eATmegaPinFunc::USART0_CLK)) {
      state._USART0_TXEN = USART0._UCSRnB._TXEN;
      USART0._UCSRnB._TXEN = false;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::PCI15, eATmegaPinFunc::PCI14, eATmegaPinFunc::PCI13, eATmegaPinFunc::PCI12, eATmegaPinFunc::PCI11, eATmegaPinFunc::PCI10, eATmegaPinFunc::PCI9, eATmegaPinFunc::PCI8)) {
      state._PCIE1 = _PCICR._PCIE1;
      _PCICR._PCIE1 = false;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::TOC0B)) {
      state._COM0B = TIMER0._TCCRnA._COMnB;
      TIMER0._TCCRnA._COMnB = 0;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::TOSC1, eATmegaPinFunc::TOSC2)) {
      state._AS2 = _ASSR._AS2;
      _ASSR._AS2 = false;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC2B)) {
      state._COM2B = _TIMER2._TCCRnA._COMnB;
      _TIMER2._TCCRnA._COMnB = 0;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC4C)) {
      state._COM4C = TIMER4._TCCRnA._COMnC;
      TIMER4._TCCRnA._COMnC = 0;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC4B)) {
      state._COM4B = TIMER4._TCCRnA._COMnB;
      TIMER4._TCCRnA._COMnB = 0;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC4A)) {
      state._COM4A = TIMER4._TCCRnA._COMnA;
      TIMER4._TCCRnA._COMnA = 0;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::USART2_RXD, eATmegaPinFunc::USART2_CLK)) {
      state._USART2_RXEN = USART2._UCSRnB._RXEN;
      USART2._UCSRnB._RXEN = false;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::USART2_TXD, eATmegaPinFunc::USART2_CLK)) {
      state._USART2_TXEN = USART2._UCSRnB._TXEN;
      USART2._UCSRnB._TXEN = false;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::USART3_TXD, eATmegaPinFunc::USART3_CLK)) {
      state._USART3_RXEN = USART3._UCSRnB._RXEN;
      USART3._UCSRnB._RXEN = false;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::USART3_TXD, eATmegaPinFunc::USART3_CLK)) {
      state._USART3_TXEN = USART3._UCSRnB._TXEN;
      USART3._UCSRnB._TXEN = false;
    }
    if (funcSet.hasAnyFunc(
      eATmegaPinFunc::PCI23, eATmegaPinFunc::PCI22, eATmegaPinFunc::PCI21, eATmegaPinFunc::PCI20, eATmegaPinFunc::PCI19, eATmegaPinFunc::PCI18, eATmegaPinFunc::PCI17, eATmegaPinFunc::PCI16
    )) {
      state._PCIE2 = _PCICR._PCIE2;
      _PCICR._PCIE2 = false;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC5C)) {
      state._COM5C = TIMER5._TCCRnA._COMnC;
      TIMER5._TCCRnA._COMnC = 0;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC5B)) {
      state._COM5B = TIMER5._TCCRnA._COMnB;
      TIMER5._TCCRnA._COMnB = 0;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC5A)) {
      state._COM5A = TIMER5._TCCRnA._COMnA;
      TIMER5._TCCRnA._COMnA = 0;
    }
  #elif defined(__AVR_TRM02__)
    if (funcSet.hasAnyFunc(eATmegaPinFunc::PCI7, eATmegaPinFunc::PCI6, eATmegaPinFunc::PCI5, eATmegaPinFunc::PCI4, eATmegaPinFunc::PCI3, eATmegaPinFunc::PCI2, eATmegaPinFunc::PCI1, eATmegaPinFunc::PCI0)) {
      state._PCIE0 = _PCICR._PCIE0;
      _PCICR._PCIE0 = false;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::ADC7)) {
      state._ADC7D = _DIDR0._ADC7D;
      _DIDR0._ADC7D = false;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::ADC6)) {
      state._ADC6D = _DIDR0._ADC6D;
      _DIDR0._ADC6D = false;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::ADC5)) {
      state._ADC5D = _DIDR0._ADC5D;
      _DIDR0._ADC5D = false;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::ADC4)) {
      state._ADC4D = _DIDR0._ADC4D;
      _DIDR0._ADC4D = false;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::ADC3)) {
      state._ADC3D = _DIDR0._ADC3D;
      _DIDR0._ADC3D = false;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::ADC2)) {
      state._ADC2D = _DIDR0._ADC2D;
      _DIDR0._ADC2D = false;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::ADC1)) {
      state._ADC1D = _DIDR0._ADC1D;
      _DIDR0._ADC1D = false;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::ADC0)) {
      state._ADC0D = _DIDR0._ADC0D;
      _DIDR0._ADC0D = false;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::PCI15, eATmegaPinFunc::PCI14, eATmegaPinFunc::PCI13, eATmegaPinFunc::PCI12, eATmegaPinFunc::PCI11, eATmegaPinFunc::PCI10, eATmegaPinFunc::PCI9, eATmegaPinFunc::PCI8)) {
      state._PCIE1 = _PCICR._PCIE1;
      _PCICR._PCIE1 = false;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::SPI_SCK, eATmegaPinFunc::SPI_MISO, eATmegaPinFunc::SPI_MOSI, eATmegaPinFunc::SPI_CS)) {
      state._SPE = _SPCR._SPE;
      _SPCR._SPE = false;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC0A)) {
      state._COM0A = TIMER0._TCCRnA._COMnA;
      TIMER0._TCCRnA._COMnA = 0;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC0B)) {
      state._COM0B = TIMER0._TCCRnA._COMnB;
      TIMER0._TCCRnA._COMnB = 0;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::TOSC1, eATmegaPinFunc::TOSC2)) {
      state._AS2 = _ASSR._AS2;
      _ASSR._AS2 = false;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::PCI23, eATmegaPinFunc::PCI22, eATmegaPinFunc::PCI21, eATmegaPinFunc::PCI20, eATmegaPinFunc::PCI19, eATmegaPinFunc::PCI18, eATmegaPinFunc::PCI17, eATmegaPinFunc::PCI16)) {
      state._PCIE2 = _PCICR._PCIE2;
      _PCICR._PCIE2 = false;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::PCI31, eATmegaPinFunc::PCI30, eATmegaPinFunc::PCI29, eATmegaPinFunc::PCI28, eATmegaPinFunc::PCI27, eATmegaPinFunc::PCI26, eATmegaPinFunc::PCI25, eATmegaPinFunc::PCI24)) {
      state._PCIE3 = _PCICR._PCIE3;
      _PCICR._PCIE3 = false;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC2A)) {
      state._COM2A = _TIMER2._TCCRnA._COMnA;
      _TIMER2._TCCRnA._COMnA = 0;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC2B)) {
      state._COM2B = _TIMER2._TCCRnA._COMnB;
      _TIMER2._TCCRnA._COMnB = 0;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC1A)) {
      state._COM1A = TIMER1._TCCRnA._COMnA;
      TIMER1._TCCRnA._COMnA = 0;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC1B)) {
      state._COM1B = TIMER1._TCCRnA._COMnB;
      TIMER1._TCCRnA._COMnB = 0;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::USART1_TXD)) {
      state._USART1_TXEN = USART1._UCSRnB._TXEN;
      USART1._UCSRnB._TXEN = false;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::USART1_RXD)) {
      state._USART1_RXEN = USART1._UCSRnB._RXEN;
      USART1._UCSRnB._RXEN = false;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::USART0_TXD)) {
      state._USART0_TXEN = USART0._UCSRnB._TXEN;
      USART0._UCSRnB._TXEN = false;
    }
    // There is a bug in the ATmega164A technical reference manual where
    // it says that pin 0 is mapped to USART1 RXD in the signal mapping table
    // but the associated list says USART0 RXD.
    if (funcSet.hasFunc(eATmegaPinFunc::USART0_RXD)) {
      state._USART0_RXEN = USART0._UCSRnB._RXEN;
      USART0._UCSRnB._RXEN = false;
    }
  #elif defined(__AVR_TRM03__)
    if (funcSet.hasAnyFunc(eATmegaPinFunc::PCI7, eATmegaPinFunc::PCI6, eATmegaPinFunc::PCI5, eATmegaPinFunc::PCI4, eATmegaPinFunc::PCI3, eATmegaPinFunc::PCI2, eATmegaPinFunc::PCI1, eATmegaPinFunc::PCI0)) {
      state._PCIE0 = _PCICR._PCIE0;
      _PCICR._PCIE0 = false;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::TOSC1, eATmegaPinFunc::TOSC2)) {
      state._AS2 = _ASSR._AS2;
      _ASSR._AS2 = false;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::SPI_SCK, eATmegaPinFunc::SPI_MISO, eATmegaPinFunc::SPI_MOSI, eATmegaPinFunc::SPI_CS)) {
      state._SPE = _SPCR._SPE;
      _SPCR._SPE = false;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC2A)) {
      state._COM2A = _TIMER2._TCCRnA._COMnA;
      _TIMER2._TCCRnA._COMnA = 0;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC1B)) {
      state._COM1B = TIMER1._TCCRnA._COMnB;
      TIMER1._TCCRnA._COMnB = 0;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC1A)) {
      state._COM1A = TIMER1._TCCRnA._COMnA;
      TIMER1._TCCRnA._COMnA = 0;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::PCI14, eATmegaPinFunc::PCI13, eATmegaPinFunc::PCI12, eATmegaPinFunc::PCI11, eATmegaPinFunc::PCI10, eATmegaPinFunc::PCI9, eATmegaPinFunc::PCI8)) {
      state._PCIE1 = _PCICR._PCIE1;
      _PCICR._PCIE1 = false;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::TWI_CLK, eATmegaPinFunc::TWI_SDA)) {
      state._TWEN = _TWCR._TWEN;
      _TWCR._TWEN = false;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::ADC5)) {
      state._ADC5D = _DIDR0._ADC5D;
      _DIDR0._ADC5D = false;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::ADC4)) {
      state._ADC4D = _DIDR0._ADC4D;
      _DIDR0._ADC4D = false;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::ADC3)) {
      state._ADC3D = _DIDR0._ADC3D;
      _DIDR0._ADC3D = false;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::ADC2)) {
      state._ADC2D = _DIDR0._ADC2D;
      _DIDR0._ADC2D = false;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::ADC1)) {
      state._ADC1D = _DIDR0._ADC1D;
      _DIDR0._ADC1D = false;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::ADC0)) {
      state._ADC0D = _DIDR0._ADC0D;
      _DIDR0._ADC0D = false;
    }
    // There is a bug in the ATmega48A technical reference manual where pin 2
    // is said to be mapped to PCIE1 but logically it should be PCIE2 instead.
    // The real mapping can be read in the documentation of the PCICR register.
    if (funcSet.hasAnyFunc(eATmegaPinFunc::PCI23, eATmegaPinFunc::PCI22, eATmegaPinFunc::PCI21, eATmegaPinFunc::PCI20, eATmegaPinFunc::PCI19, eATmegaPinFunc::PCI18, eATmegaPinFunc::PCI17, eATmegaPinFunc::PCI16)) {
      state._PCIE2 = _PCICR._PCIE2;
      _PCICR._PCIE2 = false;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC0A)) {
      state._COM0A = TIMER0._TCCRnA._COMnA;
      TIMER0._TCCRnA._COMnA = 0;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC0B)) {
      state._COM0B = TIMER0._TCCRnA._COMnB;
      TIMER0._TCCRnA._COMnB = 0;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::USART_CLK)) {
      state._UMSEL = USART0._UCSRnC._UMSEL;
      USART0._UCSRnC._UMSEL = 0;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC2B)) {
      state._COM2B = _TIMER2._TCCRnA._COMnB;
      _TIMER2._TCCRnA._COMnB = 0;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::USART_TXD)) {
      state._USART0_TXEN = USART0._UCSRnB._TXEN;
      USART0._UCSRnB._TXEN = false;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::USART_RXD)) {
      state._USART0_RXEN = USART0._UCSRnB._RXEN;
      USART0._UCSRnB._RXEN = false;
    }
  #elif defined(__AVR_TRM04__)
    if (funcSet.hasAnyFunc(
      eATmegaPinFunc::EXTMEM_AD15, eATmegaPinFunc::EXTMEM_AD14, eATmegaPinFunc::EXTMEM_AD13, eATmegaPinFunc::EXTMEM_AD12, eATmegaPinFunc::EXTMEM_AD11, eATmegaPinFunc::EXTMEM_AD10, eATmegaPinFunc::EXTMEM_AD9, eATmegaPinFunc::EXTMEM_AD8,
      eATmegaPinFunc::EXTMEM_AD7, eATmegaPinFunc::EXTMEM_AD6, eATmegaPinFunc::EXTMEM_AD5, eATmegaPinFunc::EXTMEM_AD4, eATmegaPinFunc::EXTMEM_AD3, eATmegaPinFunc::EXTMEM_AD2, eATmegaPinFunc::EXTMEM_AD1, eATmegaPinFunc::EXTMEM_AD0,
      eATmegaPinFunc::EXTMEM_ALE, eATmegaPinFunc::EXTMEM_RD, eATmegaPinFunc::EXTMEM_WR
    )) {
      state._SRE = _XMCRA._SRE;
      _XMCRA._SRE = false;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC1C)) {
      state._COM1C = TIMER1._TCCRnA._COMnC;
      TIMER1._TCCRnA._COMnC = 0;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC1B)) {
      state._COM1B = TIMER1._TCCRnA._COMnB;
      TIMER1._TCCRnA._COMnB = 0;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC1A)) {
      state._COM1A = TIMER1._TCCRnA._COMnA;
      TIMER1._TCCRnA._COMnA = 0;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC2A)) {
      state._COM2A = _TIMER2._TCCRnA._COMnA;
      _TIMER2._TCCRnA._COMnA = 0;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::SPI_MISO, eATmegaPinFunc::SPI_MOSI, eATmegaPinFunc::SPI_SCK, eATmegaPinFunc::SPI_CS)) {
      state._SPE = _SPCR._SPE;
      _SPCR._SPE = false;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::PCI7, eATmegaPinFunc::PCI6, eATmegaPinFunc::PCI5, eATmegaPinFunc::PCI4, eATmegaPinFunc::PCI3, eATmegaPinFunc::PCI2, eATmegaPinFunc::PCI1, eATmegaPinFunc::PCI0)) {
      state._PCIE0 = _PCICR._PCIE0;
      _PCICR._PCIE0 = false;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::USART1_CLK, eATmegaPinFunc::USART1_TXD)) {
      state._USART1_TXEN = USART1._UCSRnB._TXEN;
      USART1._UCSRnB._TXEN = false;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::USART1_CLK, eATmegaPinFunc::USART1_RXD)) {
      state._USART1_RXEN = USART1._UCSRnB._RXEN;
      USART1._UCSRnB._RXEN = false;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::TWI_SDA, eATmegaPinFunc::TWI_CLK)) {
      state._TWEN = _TWCR._TWEN;
      _TWCR._TWEN = false;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC2B)) {
      state._COM2B = _TIMER2._TCCRnA._COMnB;
      _TIMER2._TCCRnA._COMnB = 0;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC0B)) {
      state._COM0B = TIMER0._TCCRnA._COMnB;
      TIMER0._TCCRnA._COMnB = 0;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::EINT3)) {
      state._INT3 = _EIMSK._INT3;
      _EIMSK._INT3 = false;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::EINT2)) {
      state._INT2 = _EIMSK._INT2;
      _EIMSK._INT2 = false;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::EINT1)) {
      state._INT1 = _EIMSK._INT1;
      _EIMSK._INT1 = false;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::EINT0)) {
      state._INT0 = _EIMSK._INT0;
      _EIMSK._INT0 = false;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::UVCON)) {
      state._UVCONE = _UHWCON._UVCONE;
      _UHWCON._UVCONE = false;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::UID)) {
      state._UIDE = _UHWCON._UIDE;
      _UHWCON._UIDE = false;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::EINT7)) {
      state._INT7 = _EIMSK._INT7;
      _EIMSK._INT7 = false;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::EINT6)) {
      state._INT6 = _EIMSK._INT6;
      _EIMSK._INT6 = false;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::EINT5)) {
      state._INT5 = _EIMSK._INT5;
      _EIMSK._INT5 = false;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::EINT4)) {
      state._INT4 = _EIMSK._INT4;
      _EIMSK._INT4 = false;
    }
  #elif defined(__AVR_TRM05__)
    if (funcSet.hasAnyFunc(eATmegaPinFunc::PCI7, eATmegaPinFunc::PCI6, eATmegaPinFunc::PCI5, eATmegaPinFunc::PCI4, eATmegaPinFunc::PCI3, eATmegaPinFunc::PCI2, eATmegaPinFunc::PCI1, eATmegaPinFunc::PCI0)) {
      state._PCIE0 = _PCICR._PCIE0;
      _PCICR._PCIE0 = false;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::PCI15, eATmegaPinFunc::PCI14, eATmegaPinFunc::PCI13, eATmegaPinFunc::PCI12, eATmegaPinFunc::PCI11, eATmegaPinFunc::PCI10, eATmegaPinFunc::PCI9, eATmegaPinFunc::PCI8)) {
      state._PCIE1 = _PCICR._PCIE1;
      _PCICR._PCIE1 = false;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::PCI23, eATmegaPinFunc::PCI22, eATmegaPinFunc::PCI21, eATmegaPinFunc::PCI20, eATmegaPinFunc::PCI19, eATmegaPinFunc::PCI18, eATmegaPinFunc::PCI17, eATmegaPinFunc::PCI16)) {
      state._PCIE2 = _PCICR._PCIE2;
      _PCICR._PCIE2 = false;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::PCI31, eATmegaPinFunc::PCI30, eATmegaPinFunc::PCI29, eATmegaPinFunc::PCI28, eATmegaPinFunc::PCI27, eATmegaPinFunc::PCI26, eATmegaPinFunc::PCI25, eATmegaPinFunc::PCI24)) {
      state._PCIE3 = _PCICR._PCIE3;
      _PCICR._PCIE3 = false;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::SPI_SCK, eATmegaPinFunc::SPI_MISO, eATmegaPinFunc::SPI_MOSI, eATmegaPinFunc::SPI_CS)) {
      state._SPE = _SPCR._SPE;
      _SPCR._SPE = false;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC0B)) {
      state._COM0B = TIMER0._TCCRnA._COMnB;
      TIMER0._TCCRnA._COMnB = 0;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC0A)) {
      state._COM0A = TIMER0._TCCRnA._COMnA;
      TIMER0._TCCRnA._COMnA = 0;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC2A)) {
      state._COM2A = _TIMER2._TCCRnA._COMnA;
      _TIMER2._TCCRnA._COMnA = 0;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC2B)) {
      state._COM2B = _TIMER2._TCCRnA._COMnB;
      _TIMER2._TCCRnA._COMnB = 0;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC1A)) {
      state._COM1A = TIMER1._TCCRnA._COMnA;
      TIMER1._TCCRnA._COMnA = 0;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC1B)) {
      state._COM1B = TIMER1._TCCRnA._COMnB;
      TIMER1._TCCRnA._COMnB = 0;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::TWI_CLK, eATmegaPinFunc::TWI_SDA)) {
      state._TWEN = _TWCR._TWEN;
      _TWCR._TWEN = false;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::EINT2)) {
      state._INT2 = _EIMSK._INT2;
      _EIMSK._INT2 = false;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::EINT1)) {
      state._INT1 = _EIMSK._INT1;
      _EIMSK._INT1 = false;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::EINT0)) {
      state._INT0 = _EIMSK._INT0;
      _EIMSK._INT0 = false;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::USART0_CLK, eATmegaPinFunc::USART0_TXD)) {
      state._TXEN0 = USART0._UCSRnB._TXEN;
      USART0._UCSRnB._TXEN = false;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::USART0_CLK, eATmegaPinFunc::USART0_RXD)) {
      state._RXEN0 = USART0._UCSRnB._RXEN;
      USART0._UCSRnB._RXEN = false;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::USART1_CLK, eATmegaPinFunc::USART1_TXD)) {
      state._TXEN1 = USART1._UCSRnB._TXEN;
      USART1._UCSRnB._TXEN = false;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::USART1_CLK, eATmegaPinFunc::USART1_RXD)) {
      state._RXEN1 = USART1._UCSRnB._RXEN;
      USART1._UCSRnB._RXEN = false;
    }
  #endif

  return state;
}

inline void _ATmega_restorePinAlternates(const ATmegaPinFuncSet& funcSet, const pin_dev_state_t& state) {
  ATmegaPeripheralSet periSet;
  periSet.fromPinFuncs(funcSet);

  ATmegaPeripheralPowerGate pgate(periSet);

  #ifdef __AVR_TRM01__
    // See page 75ff of ATmega2560 technical reference manual.
    if (funcSet.hasAnyFunc(
      eATmegaPinFunc::EXTMEM_AD15, eATmegaPinFunc::EXTMEM_AD14, eATmegaPinFunc::EXTMEM_AD13, eATmegaPinFunc::EXTMEM_AD12, eATmegaPinFunc::EXTMEM_AD11, eATmegaPinFunc::EXTMEM_AD10, eATmegaPinFunc::EXTMEM_AD9, eATmegaPinFunc::EXTMEM_AD8,
      eATmegaPinFunc::EXTMEM_AD7, eATmegaPinFunc::EXTMEM_AD6, eATmegaPinFunc::EXTMEM_AD5, eATmegaPinFunc::EXTMEM_AD4, eATmegaPinFunc::EXTMEM_AD3, eATmegaPinFunc::EXTMEM_AD2, eATmegaPinFunc::EXTMEM_AD1, eATmegaPinFunc::EXTMEM_AD0,
      eATmegaPinFunc::EXTMEM_ALE, eATmegaPinFunc::EXTMEM_RD, eATmegaPinFunc::EXTMEM_WR
    )) {
      _XMCRA._SRE = state._SRE;
    }
    if (funcSet.hasAnyFunc(
      eATmegaPinFunc::PCI0, eATmegaPinFunc::PCI1, eATmegaPinFunc::PCI2, eATmegaPinFunc::PCI3, eATmegaPinFunc::PCI4, eATmegaPinFunc::PCI5, eATmegaPinFunc::PCI6, eATmegaPinFunc::PCI7
    )) {
      _PCICR._PCIE0 = state._PCIE0;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC1C)) {
      TIMER1._TCCRnA._COMnC = state._COM1C;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC1B)) {
      TIMER1._TCCRnA._COMnB = state._COM1B;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC1A)) {
      TIMER1._TCCRnA._COMnA = state._COM1A;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC2A)) {
      _TIMER2._TCCRnA._COMnA = state._COM2A;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::USART1_TXD, eATmegaPinFunc::USART1_CLK)) {
      USART1._UCSRnB._TXEN = state._USART1_TXEN;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::USART1_RXD, eATmegaPinFunc::USART1_CLK)) {
      USART1._UCSRnB._RXEN = state._USART1_RXEN;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC3C)) {
      TIMER3._TCCRnA._COMnC = state._COM3C;
    }
    // There is an error in the technical reference manual signal mapping table
    // of ATmega2560 where is says that pin 3 is mapped to OC3B, but the list
    // says OC3A.
    if (funcSet.hasFunc(eATmegaPinFunc::TOC3B)) {
      TIMER3._TCCRnA._COMnB = state._COM3B;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC3A)) {
      TIMER3._TCCRnA._COMnA = state._COM3A;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::USART0_RXD, eATmegaPinFunc::USART0_CLK)) {
      USART0._UCSRnB._RXEN = state._USART0_RXEN;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::USART0_TXD, eATmegaPinFunc::USART0_CLK)) {
      USART0._UCSRnB._TXEN = state._USART0_TXEN;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::PCI15, eATmegaPinFunc::PCI14, eATmegaPinFunc::PCI13, eATmegaPinFunc::PCI12, eATmegaPinFunc::PCI11, eATmegaPinFunc::PCI10, eATmegaPinFunc::PCI9, eATmegaPinFunc::PCI8)) {
      _PCICR._PCIE1 = state._PCIE1;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::TOC0B)) {
      TIMER0._TCCRnA._COMnB = state._COM0B;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::TOSC1, eATmegaPinFunc::TOSC2)) {
      _ASSR._AS2 = state._AS2;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC2B)) {
      _TIMER2._TCCRnA._COMnB = state._COM2B;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC4C)) {
      TIMER4._TCCRnA._COMnC = state._COM4C;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC4B)) {
      TIMER4._TCCRnA._COMnB = state._COM4B;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC4A)) {
      TIMER4._TCCRnA._COMnA = state._COM4A;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::USART2_RXD, eATmegaPinFunc::USART2_CLK)) {
      USART2._UCSRnB._RXEN = state._USART2_RXEN;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::USART2_TXD, eATmegaPinFunc::USART2_CLK)) {
      USART2._UCSRnB._TXEN = state._USART2_TXEN;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::USART3_TXD, eATmegaPinFunc::USART3_CLK)) {
      USART3._UCSRnB._RXEN = state._USART3_RXEN;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::USART3_TXD, eATmegaPinFunc::USART3_CLK)) {
      USART3._UCSRnB._TXEN = state._USART3_TXEN;
    }
    if (funcSet.hasAnyFunc(
      eATmegaPinFunc::PCI23, eATmegaPinFunc::PCI22, eATmegaPinFunc::PCI21, eATmegaPinFunc::PCI20, eATmegaPinFunc::PCI19, eATmegaPinFunc::PCI18, eATmegaPinFunc::PCI17, eATmegaPinFunc::PCI16
    )) {
      _PCICR._PCIE2 = state._PCIE2;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC5C)) {
      TIMER5._TCCRnA._COMnC = state._COM5C;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC5B)) {
      TIMER5._TCCRnA._COMnB = state._COM5B;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC5A)) {
      TIMER5._TCCRnA._COMnA = state._COM5A;
    }
  #elif defined(__AVR_TRM02__)
    if (funcSet.hasAnyFunc(eATmegaPinFunc::PCI7, eATmegaPinFunc::PCI6, eATmegaPinFunc::PCI5, eATmegaPinFunc::PCI4, eATmegaPinFunc::PCI3, eATmegaPinFunc::PCI2, eATmegaPinFunc::PCI1, eATmegaPinFunc::PCI0)) {
      _PCICR._PCIE0 = state._PCIE0;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::ADC7)) {
      _DIDR0._ADC7D = state._ADC7D;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::ADC6)) {
      _DIDR0._ADC6D = state._ADC6D;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::ADC5)) {
      _DIDR0._ADC5D = state._ADC5D;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::ADC4)) {
      _DIDR0._ADC4D = state._ADC4D;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::ADC3)) {
      _DIDR0._ADC3D = state._ADC3D;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::ADC2)) {
      _DIDR0._ADC2D = state._ADC2D;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::ADC1)) {
      _DIDR0._ADC1D = state._ADC1D;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::ADC0)) {
      _DIDR0._ADC0D = state._ADC0D;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::PCI15, eATmegaPinFunc::PCI14, eATmegaPinFunc::PCI13, eATmegaPinFunc::PCI12, eATmegaPinFunc::PCI11, eATmegaPinFunc::PCI10, eATmegaPinFunc::PCI9, eATmegaPinFunc::PCI8)) {
      _PCICR._PCIE1 = state._PCIE1;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::SPI_SCK, eATmegaPinFunc::SPI_MISO, eATmegaPinFunc::SPI_MOSI, eATmegaPinFunc::SPI_CS)) {
      _SPCR._SPE = state._SPE;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC0A)) {
      TIMER0._TCCRnA._COMnA = state._COM0A;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC0B)) {
      TIMER0._TCCRnA._COMnB = state._COM0B;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::TOSC1, eATmegaPinFunc::TOSC2)) {
      _ASSR._AS2 = state._AS2;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::PCI23, eATmegaPinFunc::PCI22, eATmegaPinFunc::PCI21, eATmegaPinFunc::PCI20, eATmegaPinFunc::PCI19, eATmegaPinFunc::PCI18, eATmegaPinFunc::PCI17, eATmegaPinFunc::PCI16)) {
      _PCICR._PCIE2 = state._PCIE2;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::PCI31, eATmegaPinFunc::PCI30, eATmegaPinFunc::PCI29, eATmegaPinFunc::PCI28, eATmegaPinFunc::PCI27, eATmegaPinFunc::PCI26, eATmegaPinFunc::PCI25, eATmegaPinFunc::PCI24)) {
      _PCICR._PCIE3 = state._PCIE3;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC2A)) {
      _TIMER2._TCCRnA._COMnA = state._COM2A;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC2B)) {
      _TIMER2._TCCRnA._COMnB = state._COM2B;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC1A)) {
      TIMER1._TCCRnA._COMnA = state._COM1A;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC1B)) {
      TIMER1._TCCRnA._COMnB = state._COM1B;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::USART1_TXD)) {
      USART1._UCSRnB._TXEN = state._USART1_TXEN;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::USART1_RXD)) {
      USART1._UCSRnB._RXEN = state._USART1_RXEN;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::USART0_TXD)) {
      USART0._UCSRnB._TXEN = state._USART0_TXEN;
    }
    // There is a bug in the ATmega164A technical reference manual where
    // it says that pin 0 is mapped to USART1 RXD in the signal mapping table
    // but the associated list says USART0 RXD.
    if (funcSet.hasFunc(eATmegaPinFunc::USART0_RXD)) {
      USART0._UCSRnB._RXEN = state._USART0_RXEN;
    }
  #elif defined(__AVR_TRM03__)
    if (funcSet.hasAnyFunc(eATmegaPinFunc::PCI7, eATmegaPinFunc::PCI6, eATmegaPinFunc::PCI5, eATmegaPinFunc::PCI4, eATmegaPinFunc::PCI3, eATmegaPinFunc::PCI2, eATmegaPinFunc::PCI1, eATmegaPinFunc::PCI0)) {
      _PCICR._PCIE0 = state._PCIE0;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::TOSC1, eATmegaPinFunc::TOSC2)) {
      _ASSR._AS2 = state._AS2;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::SPI_SCK, eATmegaPinFunc::SPI_MISO, eATmegaPinFunc::SPI_MOSI, eATmegaPinFunc::SPI_CS)) {
      _SPCR._SPE = state._SPE;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC2A)) {
      _TIMER2._TCCRnA._COMnA = state._COM2A;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC1B)) {
      TIMER1._TCCRnA._COMnB = state._COM1B;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC1A)) {
      TIMER1._TCCRnA._COMnA = state._COM1A;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::PCI14, eATmegaPinFunc::PCI13, eATmegaPinFunc::PCI12, eATmegaPinFunc::PCI11, eATmegaPinFunc::PCI10, eATmegaPinFunc::PCI9, eATmegaPinFunc::PCI8)) {
      _PCICR._PCIE1 = state._PCIE1;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::TWI_CLK, eATmegaPinFunc::TWI_SDA)) {
      _TWCR._TWEN = state._TWEN;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::ADC5)) {
      _DIDR0._ADC5D = state._ADC5D;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::ADC4)) {
      _DIDR0._ADC4D = state._ADC4D;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::ADC3)) {
      _DIDR0._ADC3D = state._ADC3D;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::ADC2)) {
      _DIDR0._ADC2D = state._ADC2D;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::ADC1)) {
      _DIDR0._ADC1D = state._ADC1D;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::ADC0)) {
      _DIDR0._ADC0D = state._ADC0D;
    }
    // There is a bug in the ATmega48A technical reference manual where pin 2
    // is said to be mapped to PCIE1 but logically it should be PCIE2 instead.
    // The real mapping can be read in the documentation of the PCICR register.
    if (funcSet.hasAnyFunc(eATmegaPinFunc::PCI23, eATmegaPinFunc::PCI22, eATmegaPinFunc::PCI21, eATmegaPinFunc::PCI20, eATmegaPinFunc::PCI19, eATmegaPinFunc::PCI18, eATmegaPinFunc::PCI17, eATmegaPinFunc::PCI16)) {
      _PCICR._PCIE2 = state._PCIE2;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC0A)) {
      TIMER0._TCCRnA._COMnA = state._COM0A;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC0B)) {
      TIMER0._TCCRnA._COMnB = state._COM0B;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::USART_CLK)) {
      USART0._UCSRnC._UMSEL = state._UMSEL;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC2B)) {
      _TIMER2._TCCRnA._COMnB = state._COM2B;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::USART_TXD)) {
      USART0._UCSRnB._TXEN = state._USART0_TXEN;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::USART_RXD)) {
      USART0._UCSRnB._RXEN = state._USART0_RXEN;
    }
  #elif defined(__AVR_TRM04__)
    if (funcSet.hasAnyFunc(
      eATmegaPinFunc::EXTMEM_AD15, eATmegaPinFunc::EXTMEM_AD14, eATmegaPinFunc::EXTMEM_AD13, eATmegaPinFunc::EXTMEM_AD12, eATmegaPinFunc::EXTMEM_AD11, eATmegaPinFunc::EXTMEM_AD10, eATmegaPinFunc::EXTMEM_AD9, eATmegaPinFunc::EXTMEM_AD8,
      eATmegaPinFunc::EXTMEM_AD7, eATmegaPinFunc::EXTMEM_AD6, eATmegaPinFunc::EXTMEM_AD5, eATmegaPinFunc::EXTMEM_AD4, eATmegaPinFunc::EXTMEM_AD3, eATmegaPinFunc::EXTMEM_AD2, eATmegaPinFunc::EXTMEM_AD1, eATmegaPinFunc::EXTMEM_AD0,
      eATmegaPinFunc::EXTMEM_ALE, eATmegaPinFunc::EXTMEM_RD, eATmegaPinFunc::EXTMEM_WR
    )) {
      _XMCRA._SRE = state._SRE;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC1C)) {
      TIMER1._TCCRnA._COMnC = state._COM1C;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC1B)) {
      TIMER1._TCCRnA._COMnB = state._COM1B;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC1A)) {
      TIMER1._TCCRnA._COMnA = state._COM1A;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC2A)) {
      _TIMER2._TCCRnA._COMnA = state._COM2A;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::SPI_MISO, eATmegaPinFunc::SPI_MOSI, eATmegaPinFunc::SPI_SCK, eATmegaPinFunc::SPI_CS)) {
      _SPCR._SPE = state._SPE;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::PCI7, eATmegaPinFunc::PCI6, eATmegaPinFunc::PCI5, eATmegaPinFunc::PCI4, eATmegaPinFunc::PCI3, eATmegaPinFunc::PCI2, eATmegaPinFunc::PCI1, eATmegaPinFunc::PCI0)) {
      _PCICR._PCIE0 = state._PCIE0;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::USART1_CLK, eATmegaPinFunc::USART1_TXD)) {
      USART1._UCSRnB._TXEN = state._USART1_TXEN;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::USART1_CLK, eATmegaPinFunc::USART1_RXD)) {
      USART1._UCSRnB._RXEN = state._USART1_RXEN;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::TWI_SDA, eATmegaPinFunc::TWI_CLK)) {
      _TWCR._TWEN = state._TWEN;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC2B)) {
      _TIMER2._TCCRnA._COMnB = state._COM2B;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC0B)) {
      TIMER0._TCCRnA._COMnB = state._COM0B;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::EINT3)) {
      _EIMSK._INT3 = state._INT3;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::EINT2)) {
      _EIMSK._INT2 = state._INT2;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::EINT1)) {
      _EIMSK._INT1 = state._INT1;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::EINT0)) {
      _EIMSK._INT0 = state._INT0;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::UVCON)) {
      _UHWCON._UVCONE = state._UVCONE;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::UID)) {
      _UHWCON._UIDE = state._UIDE;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::EINT7)) {
      _EIMSK._INT7 = state._INT7;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::EINT6)) {
      _EIMSK._INT6 = state._INT6;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::EINT5)) {
      _EIMSK._INT5 = state._INT5;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::EINT4)) {
      _EIMSK._INT4 = state._INT4;
    }
  #elif defined(__AVR_TRM05__)
    if (funcSet.hasAnyFunc(eATmegaPinFunc::PCI7, eATmegaPinFunc::PCI6, eATmegaPinFunc::PCI5, eATmegaPinFunc::PCI4, eATmegaPinFunc::PCI3, eATmegaPinFunc::PCI2, eATmegaPinFunc::PCI1, eATmegaPinFunc::PCI0)) {
      _PCICR._PCIE0 = state._PCIE0;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::PCI15, eATmegaPinFunc::PCI14, eATmegaPinFunc::PCI13, eATmegaPinFunc::PCI12, eATmegaPinFunc::PCI11, eATmegaPinFunc::PCI10, eATmegaPinFunc::PCI9, eATmegaPinFunc::PCI8)) {
      _PCICR._PCIE1 = state._PCIE1;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::PCI23, eATmegaPinFunc::PCI22, eATmegaPinFunc::PCI21, eATmegaPinFunc::PCI20, eATmegaPinFunc::PCI19, eATmegaPinFunc::PCI18, eATmegaPinFunc::PCI17, eATmegaPinFunc::PCI16)) {
      _PCICR._PCIE2 = state._PCIE2;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::PCI31, eATmegaPinFunc::PCI30, eATmegaPinFunc::PCI29, eATmegaPinFunc::PCI28, eATmegaPinFunc::PCI27, eATmegaPinFunc::PCI26, eATmegaPinFunc::PCI25, eATmegaPinFunc::PCI24)) {
      _PCICR._PCIE3 = state._PCIE3;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::SPI_SCK, eATmegaPinFunc::SPI_MISO, eATmegaPinFunc::SPI_MOSI, eATmegaPinFunc::SPI_CS)) {
      _SPCR._SPE = state._SPE;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC0B)) {
      TIMER0._TCCRnA._COMnB = state._COM0B;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC0A)) {
      TIMER0._TCCRnA._COMnA = state._COM0A;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC2A)) {
      _TIMER2._TCCRnA._COMnA = state._COM2A;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC2B)) {
      _TIMER2._TCCRnA._COMnB = state._COM2B;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC1A)) {
      TIMER1._TCCRnA._COMnA = state._COM1A;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::TOC1B)) {
      TIMER1._TCCRnA._COMnB = state._COM1B;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::TWI_CLK, eATmegaPinFunc::TWI_SDA)) {
      _TWCR._TWEN = state._TWEN;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::EINT2)) {
      _EIMSK._INT2 = state._INT2;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::EINT1)) {
      _EIMSK._INT1 = state._INT1;
    }
    if (funcSet.hasFunc(eATmegaPinFunc::EINT0)) {
      _EIMSK._INT0 = state._INT0;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::USART0_CLK, eATmegaPinFunc::USART0_TXD)) {
      USART0._UCSRnB._TXEN = state._TXEN0;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::USART0_CLK, eATmegaPinFunc::USART0_RXD)) {
      USART0._UCSRnB._RXEN = state._RXEN0;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::USART1_CLK, eATmegaPinFunc::USART1_TXD)) {
      USART1._UCSRnB._TXEN = state._TXEN1;
    }
    if (funcSet.hasAnyFunc(eATmegaPinFunc::USART1_CLK, eATmegaPinFunc::USART1_RXD)) {
      USART1._UCSRnB._RXEN = state._RXEN1;
    }
  #endif
}

inline pin_dev_state_t _ATmega_savePinAlternate(uint8_t pin) {
  return _ATmega_savePinAlternates({pin});
}

inline void _ATmega_restorePinAlternate(uint8_t pin, const pin_dev_state_t& state) {
  _ATmega_restorePinAlternate({pin}, state);
}

#ifndef LOW
  #define LOW 0
#endif
#ifndef HIGH
  #define HIGH 1
#endif

inline void _ATmega_digitalWrite(int pin, int state) {
  if (pin < 0) return;

  ATmegaPinInfo info = _ATmega_getPinInfo((unsigned int)pin);

  #ifdef __AVR_TRM01__
    if (info.port == eATmegaPort::PORT_A) {
      _PORTA._PORT.setValue(info.pinidx, state == HIGH);
    }
    else if (info.port == eATmegaPort::PORT_B) {
      _PORTB._PORT.setValue(info.pinidx, state == HIGH);
    }
    else if (info.port == eATmegaPort::PORT_C) {
      _PORTC._PORT.setValue(info.pinidx, state == HIGH);
    }
    else if (info.port == eATmegaPort::PORT_D) {
      _PORTD._PORT.setValue(info.pinidx, state == HIGH);
    }
    else if (info.port == eATmegaPort::PORT_E) {
      _PORTE._PORT.setValue(info.pinidx, state == HIGH);
    }
    else if (info.port == eATmegaPort::PORT_F) {
      _PORTF._PORT.setValue(info.pinidx, state == HIGH);
    }
    else if (info.port == eATmegaPort::PORT_G) {
      _PORTG._PORT.setValue(info.pinidx, state == HIGH);
    }
    else if (info.port == eATmegaPort::PORT_H) {
      _PORTH._PORT.setValue(info.pinidx, state == HIGH);
    }
    else if (info.port == eATmegaPort::PORT_J) {
      _PORTJ._PORT.setValue(info.pinidx, state == HIGH);
    }
    else if (info.port == eATmegaPort::PORT_K) {
      _PORTK._PORT.setValue(info.pinidx, state == HIGH);
    }
    else if (info.port == eATmegaPort::PORT_L) {
      _PORTL._PORT.setValue(info.pinidx, state == HIGH);
    }
  #elif defined(__AVR_TRM02__) || defined(__AVR_TRM05__)
    if (info.port == eATmegaPort::PORT_A) {
      _PORTA._PORT.setValue(info.pinidx, state == HIGH);
    }
    else if (info.port == eATmegaPort::PORT_B) {
      _PORTB._PORT.setValue(info.pinidx, state == HIGH);
    }
    else if (info.port == eATmegaPort::PORT_C) {
      _PORTC._PORT.setValue(info.pinidx, state == HIGH);
    }
    else if (info.port == eATmegaPort::PORT_D) {
      _PORTD._PORT.setValue(info.pinidx, state == HIGH);
    }
  #elif defined(__AVR_TRM03__)
    if (info.port == eATmegaPort::PORT_B) {
      _PORTB._PORT.setValue(info.pinidx, state == HIGH);
    }
    else if (info.port == eATmegaPort::PORT_C) {
      _PORTC._PORT.setValue(info.pinidx, state == HIGH);
    }
    else if (info.port == eATmegaPort::PORT_D) {
      _PORTD._PORT.setValue(info.pinidx, state == HIGH);
    }
  #elif defined(__AVR_TRM04__)
    if (info.port == eATmegaPort::PORT_A) {
      _PORTA._PORT.setValue(info.pinidx, state == HIGH);
    }
    else if (info.port == eATmegaPort::PORT_B) {
      _PORTB._PORT.setValue(info.pinidx, state == HIGH);
    }
    else if (info.port == eATmegaPort::PORT_C) {
      _PORTC._PORT.setValue(info.pinidx, state == HIGH);
    }
    else if (info.port == eATmegaPort::PORT_D) {
      _PORTD._PORT.setValue(info.pinidx, state == HIGH);
    }
    else if (info.port == eATmegaPort::PORT_E) {
      _PORTE._PORT.setValue(info.pinidx, state == HIGH);
    }
    else if (info.port == eATmegaPort::PORT_F) {
      _PORTF._PORT.setValue(info.pinidx, state == HIGH);
    }
  #endif
}

inline int _ATmega_digitalRead(int pin) {
  int value = LOW;

  if (pin < 0) return value;

  ATmegaPinInfo info = _ATmega_getPinInfo((unsigned int)pin);

  #ifdef __AVR_TRM01__
    if (info.port == eATmegaPort::PORT_A) {
      value = _PORTA._PIN.getValue(info.pinidx);
    }
    else if (info.port == eATmegaPort::PORT_B) {
      value = _PORTB._PIN.getValue(info.pinidx);
    }
    else if (info.port == eATmegaPort::PORT_C) {
      value = _PORTC._PIN.getValue(info.pinidx);
    }
    else if (info.port == eATmegaPort::PORT_D) {
      value = _PORTD._PIN.getValue(info.pinidx);
    }
    else if (info.port == eATmegaPort::PORT_E) {
      value = _PORTE._PIN.getValue(info.pinidx);
    }
    else if (info.port == eATmegaPort::PORT_F) {
      value = _PORTF._PIN.getValue(info.pinidx);
    }
    else if (info.port == eATmegaPort::PORT_G) {
      value = _PORTG._PIN.getValue(info.pinidx);
    }
    else if (info.port == eATmegaPort::PORT_H) {
      value = _PORTH._PIN.getValue(info.pinidx);
    }
    else if (info.port == eATmegaPort::PORT_J) {
      value = _PORTJ._PIN.getValue(info.pinidx);
    }
    else if (info.port == eATmegaPort::PORT_K) {
      value = _PORTK._PIN.getValue(info.pinidx);
    }
    else if (info.port == eATmegaPort::PORT_L) {
      value = _PORTL._PIN.getValue(info.pinidx);
    }
  #elif defined(__AVR_TRM02__) || defined(__AVR_TRM05__)
    if (info.port == eATmegaPort::PORT_A) {
      value = _PORTA._PIN.getValue(info.pinidx);
    }
    else if (info.port == eATmegaPort::PORT_B) {
      value = _PORTB._PIN.getValue(info.pinidx);
    }
    else if (info.port == eATmegaPort::PORT_C) {
      value = _PORTC._PIN.getValue(info.pinidx);
    }
    else if (info.port == eATmegaPort::PORT_D) {
      value = _PORTD._PIN.getValue(info.pinidx);
    }
  #elif defined(__AVR_TRM03__)
    if (info.port == eATmegaPort::PORT_B) {
      value = _PORTB._PIN.getValue(info.pinidx);
    }
    else if (info.port == eATmegaPort::PORT_C) {
      value = _PORTC._PIN.getValue(info.pinidx);
    }
    else if (info.port == eATmegaPort::PORT_D) {
      value = _PORTD._PIN.getValue(info.pinidx);
    }
  #elif defined(__AVR_TRM04__)
    if (info.port == eATmegaPort::PORT_A) {
      value = _PORTA._PIN.getValue(info.pinidx);
    }
    else if (info.port == eATmegaPort::PORT_B) {
      value = _PORTB._PIN.getValue(info.pinidx);
    }
    else if (info.port == eATmegaPort::PORT_C) {
      value = _PORTC._PIN.getValue(info.pinidx);
    }
    else if (info.port == eATmegaPort::PORT_D) {
      value = _PORTD._PIN.getValue(info.pinidx);
    }
    else if (info.port == eATmegaPort::PORT_E) {
      value = _PORTE._PIN.getValue(info.pinidx);
    }
    else if (info.port == eATmegaPort::PORT_F) {
      value = _PORTF._PIN.getValue(info.pinidx);
    }
  #endif

  return value;
}

#ifndef OUTPUT
  #define OUTPUT 1
#endif
#ifndef INPUT
  #define INPUT 0
#endif

inline void _ATmega_pinMode(int pin, int mode) {
  if (pin < 0) return;

  ATmegaPinInfo info = _ATmega_getPinInfo((unsigned int)pin);

  #ifdef __AVR_TRM01__
    if (info.port == eATmegaPort::PORT_A) {
      _PORTA._DDR.setValue(info.pinidx, mode == OUTPUT);
    }
    else if (info.port == eATmegaPort::PORT_B) {
      _PORTB._DDR.setValue(info.pinidx, mode == OUTPUT);
    }
    else if (info.port == eATmegaPort::PORT_C) {
      _PORTC._DDR.setValue(info.pinidx, mode == OUTPUT);
    }
    else if (info.port == eATmegaPort::PORT_D) {
      _PORTD._DDR.setValue(info.pinidx, mode == OUTPUT);
    }
    else if (info.port == eATmegaPort::PORT_E) {
      _PORTE._DDR.setValue(info.pinidx, mode == OUTPUT);
    }
    else if (info.port == eATmegaPort::PORT_F) {
      _PORTF._DDR.setValue(info.pinidx, mode == OUTPUT);
    }
    else if (info.port == eATmegaPort::PORT_G) {
      _PORTG._DDR.setValue(info.pinidx, mode == OUTPUT);
    }
    else if (info.port == eATmegaPort::PORT_H) {
      _PORTH._DDR.setValue(info.pinidx, mode == OUTPUT);
    }
    else if (info.port == eATmegaPort::PORT_J) {
      _PORTJ._DDR.setValue(info.pinidx, mode == OUTPUT);
    }
    else if (info.port == eATmegaPort::PORT_K) {
      _PORTK._DDR.setValue(info.pinidx, mode == OUTPUT);
    }
    else if (info.port == eATmegaPort::PORT_L) {
      _PORTL._DDR.setValue(info.pinidx, mode == OUTPUT);
    }
  #elif defined(__AVR_TRM02__) || defined(__AVR_TRM05__)
    if (info.port == eATmegaPort::PORT_A) {
      _PORTA._DDR.setValue(info.pinidx, mode == OUTPUT);
    }
    else if (info.port == eATmegaPort::PORT_B) {
      _PORTB._DDR.setValue(info.pinidx, mode == OUTPUT);
    }
    else if (info.port == eATmegaPort::PORT_C) {
      _PORTC._DDR.setValue(info.pinidx, mode == OUTPUT);
    }
    else if (info.port == eATmegaPort::PORT_D) {
      _PORTD._DDR.setValue(info.pinidx, mode == OUTPUT);
    }
  #elif defined(__AVR_TRM03__)
    if (info.port == eATmegaPort::PORT_B) {
      _PORTB._DDR.setValue(info.pinidx, mode == OUTPUT);
    }
    else if (info.port == eATmegaPort::PORT_C) {
      _PORTC._DDR.setValue(info.pinidx, mode == OUTPUT);
    }
    else if (info.port == eATmegaPort::PORT_D) {
      _PORTD._DDR.setValue(info.pinidx, mode == OUTPUT);
    }
  #elif defined(__AVR_TRM04__)
    if (info.port == eATmegaPort::PORT_A) {
      _PORTA._DDR.setValue(info.pinidx, mode == OUTPUT);
    }
    else if (info.port == eATmegaPort::PORT_B) {
      _PORTB._DDR.setValue(info.pinidx, mode == OUTPUT);
    }
    else if (info.port == eATmegaPort::PORT_C) {
      _PORTC._DDR.setValue(info.pinidx, mode == OUTPUT);
    }
    else if (info.port == eATmegaPort::PORT_D) {
      _PORTD._DDR.setValue(info.pinidx, mode == OUTPUT);
    }
    else if (info.port == eATmegaPort::PORT_E) {
      _PORTE._DDR.setValue(info.pinidx, mode == OUTPUT);
    }
    else if (info.port == eATmegaPort::PORT_F) {
      _PORTF._DDR.setValue(info.pinidx, mode == OUTPUT);
    }
  #endif
}

#if defined(__AVR_TRM01__) || defined(__AVR_TRM02__)
  struct _ATmega_efuse {
    uint8_t _BODLEVEL : 3;
    uint8_t reserved1 : 5;
  };

  struct _ATmega_hfuse {
    uint8_t _BOOTRST : 1;
    uint8_t _BOOTSZ : 2;
    uint8_t _EESAVE : 1;
    uint8_t _WDTON : 1;
    uint8_t _SPIEN : 1;
    uint8_t _JTAGEN : 1;
    uint8_t _OCDEN : 1;
  };

  struct _ATmega_lfuse {
    uint8_t _CKSEL : 4;
    uint8_t _SUT0 : 1;
    uint8_t _SUT1 : 1;
    uint8_t _CKOUT : 1;
    uint8_t _CKDIV8 : 1;
  };

  #ifndef AVR_DEFAULT_LFUSE_VALUE
    #define AVR_DEFAULT_LFUSE_VALUE 0xFF
  #endif
  #ifndef AVR_DEFAULT_HFUSE_VALUE
    #define AVR_DEFAULT_HFUSE_VALUE 0x99
  #endif
  #ifndef AVR_DEFAULT_LFUSE_VALUE
    #define AVR_DEFAULT_LFUSE_VALUE 0x62
  #endif

#elif defined(__AVR_TRM03__)
  #if defined(__AVR_ATmega48A__) || defined(__AVR_ATmega48PA__)
    struct _ATmega_efuse {
      uint8_t _SELFPRGEN : 1;
      uint8_t reserved1 : 7;
    };

    #ifndef AVR_DEFAULT_EFUSE_VALUE
      #define AVR_DEFAULT_EFUSE_VALUE 0xFF
    #endif

  #elif defined(__AVR_ATmega88A__) || defined(__AVR_ATmega88PA__) || defined(__AVR_ATmega168A__) || defined(__AVR_ATmega168PA__)
    struct _ATmega_efuse {
      uint8_t _BOOTRST : 1;
      uint8_t _BOOTSZ : 2;
      uint8_t reserved1 : 5;
    };

    #ifndef AVR_DEFAULT_EFUSE_VALUE
      #define AVR_DEFAULT_EFUSE_VALUE 0xF9
    #endif

  #else // defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)
    struct _ATmega_efuse {
      uint8_t _BODLEVEL : 3;
      uint8_t reserved1 : 5;
    };

    #ifndef AVR_DEFAULT_EFUSE_VALUE
      #define AVR_DEFAULT_EFUSE_VALUE 0xFF
    #endif

  #endif

  #if defined(__AVR_ATmega48A__) || defined(__AVR_ATmega48PA__) || defined(__AVR_ATmega88A__) || defined(__AVR_ATmega88PA__) || defined(__AVR_ATmega168A__) || defined(__AVR_ATmega168PA__)
    struct _ATmega_hfuse {
      uint8_t _BODLEVEL : 3;
      uint8_t _EESAVE : 1;
      uint8_t _WDTON : 1;
      uint8_t _SPIEN : 1;
      uint8_t _DWEN : 1;
      uint8_t _RSTDISBL : 1;
    };

    #ifndef AVR_DEFAULT_HFUSE_VALUE
      #define AVR_DEFAULT_HFUSE_VALUE 0xCF
    #endif

  #else // defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)
    struct _ATmega_hfuse {
      uint8_t _BOOTRST : 1;
      uint8_t _BOOTSZ : 2;
      uint8_t _EESAVE : 1;
      uint8_t _WDTON : 1;
      uint8_t _SPIEN : 1;
      uint8_t _DWEN : 1;
      uint8_t _RSTDISBL : 1;
    };

    #ifndef AVR_DEFAULT_HFUSE_VALUE
      #define AVR_DEFAULT_HFUSE_VALUE 0xC9
    #endif

  #endif

  struct _ATmega_lfuse {
    uint8_t _CKSEL : 4;
    uint8_t _SUT0 : 1;
    uint8_t _SUT1 : 1;
    uint8_t _CKOUT : 1;
    uint8_t _CKDIV8 : 1;
  };

  #ifndef AVR_DEFAULT_LFUSE_VALUE
    #define AVR_DEFAULT_LFUSE_VALUE 0xC9
  #endif

#elif defined(__AVR_TRM04__)
  struct _ATmega_efuse {
    uint8_t _BODLEVEL : 3;
    uint8_t _HWBE : 1;
    uint8_t reserved1 : 4;
  };

  struct _ATmega_hfuse {
    uint8_t _BOOTRST : 1;
    uint8_t _BOOTSZ : 2;
    uint8_t _EESAVE : 1;
    uint8_t _WDTON : 1;
    uint8_t _SPIEN : 1;
    uint8_t _JTAGEN : 1;
    uint8_t _OCDEN : 1;
  };

  struct _ATmega_lfuse {
    uint8_t _CKSEL : 4;
    uint8_t _SUT0 : 1;
    uint8_t _SUT1 : 1;
    uint8_t _CKOUT : 1;
    uint8_t _CKDIV8 : 1;
  };

  // Default values if not already defined.
  #ifndef AVR_DEFAULT_EFUSE_VALUE
    #define AVR_DEFAULT_EFUSE_VALUE 0xF3
  #endif
  #ifndef AVR_DEFAULT_HFUSE_VALUE
    #define AVR_DEFAULT_HFUSE_VALUE 0x99
  #endif
  #ifndef AVR_DEFAULT_LFUSE_VALUE
    #define AVR_DEFAULT_LFUSE_VALUE 0x62
  #endif

#elif defined(__AVR_TRM05__)
  struct _ATmega_efuse {
    uint8_t _BODLEVEL0 : 1;
    uint8_t _BODLEVEL1 : 1;
    uint8_t _BODLEVEL2 : 1;
    uint8_t reserved1 : 5;
  };

  struct _ATmega_hfuse {
    uint8_t _BOOTRST : 1;
    uint8_t _BOOTSZ : 2;
    uint8_t _EESAVE : 1;
    uint8_t _WDTON : 1;
    uint8_t _SPIEN : 1;
    uint8_t _JTAGEN : 1;
    uint8_t _OCDEN : 1;
  };

  struct _ATmega_lfuse {
    uint8_t _CKSEL : 4;
    uint8_t _SUT0 : 1;
    uint8_t _SUT1 : 1;
    uint8_t _CKOUT : 1;
    uint8_t _CKDIV8 : 1;
  };

  #ifndef AVR_DEFAULT_EFUSE_VALUE
    #define AVR_DEFAULT_EFUSE_VALUE 0xFF
  #endif
  #ifndef AVR_DEFAULT_HFUSE_VALUE
    #define AVR_DEFAULT_HFUSE_VALUE 0x88
  #endif
  #ifndef AVR_DEFAULT_LFUSE_VALUE
    #define AVR_DEFAULT_LFUSE_VALUE 0x62
  #endif
#endif

struct ATmega_efuse : public _ATmega_efuse {
  inline ATmega_efuse(uint8_t val = 0) { *(uint8_t*)this = val; }
  inline ATmega_efuse(const ATmega_efuse&) = default;
};
struct ATmega_hfuse : public _ATmega_hfuse {
  inline ATmega_hfuse(uint8_t val = 0) { *(uint8_t*)this = val; }
  inline ATmega_hfuse(const ATmega_hfuse&) = default;
};
struct ATmega_lfuse : public _ATmega_lfuse {
  inline ATmega_lfuse(uint8_t val = 0) { *(uint8_t*)this = val; }
  inline ATmega_lfuse(const ATmega_lfuse&) = default;
};
