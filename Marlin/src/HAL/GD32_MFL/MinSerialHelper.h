/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../platforms.h"

#ifdef ARDUINO_ARCH_MFL

enum class USART_Min_Regs : uint32_t {
	STAT0 = 0x00U,
	DATA = 0x04U,
	BAUD = 0x08U,
	CTL0 = 0x0CU,
	CTL1 = 0x14U
};

static constexpr uintptr_t USART_base[] = {
	0x40013800, // USART0
	0x40004400, // USART1
	0x40004800, // USART2
	0x40004C00, // UART3
	0x40005000 	// UART4
};


///////////////////////////// REGISTER BITS /////////////////////////////

#define BIT_DEF(start, end)	((start << 16) | (end - start + 1))

enum class STAT0_bits : uint32_t {
    TBE = BIT_DEF(7, 7)
};

enum class CTL0_bits : uint32_t {
    TEN = BIT_DEF(3, 3),
    UEN = BIT_DEF(13, 13)
};

inline volatile uint32_t* reg_address(USART_Min_Regs reg) {
	return reinterpret_cast<volatile uint32_t*>(USART_base[SERIAL_PORT] + static_cast<uint32_t>(reg));
}

template<typename T>
inline T register_read(USART_Min_Regs reg) {
	return *reinterpret_cast<volatile T*>(reg_address(reg));
}

template<typename T>
inline void register_write(USART_Min_Regs reg, T value) {
	*reinterpret_cast<volatile T*>(reg_address(reg)) = value;
}

template <typename RegType>
inline uint32_t bit_read(RegType reg, uint32_t bits) {
	uint32_t regval = *reg_address(reg);
	const uint32_t width = bits & 0xFFU;
	const uint32_t bitno = bits >> 16U;
	regval >>= bitno;
	regval &= ((1U << width) - 1);

    return regval;
}

template <typename RegType>
inline void bit_write(RegType reg, uint32_t bits, uint32_t value) {
	uint32_t regval = *reg_address(reg);
	const uint32_t width = bits & 0xFFU;
	const uint32_t bitno = bits >> 16U;
	regval &= ~(((1U << width) - 1) << bitno);
	regval |= value << bitno;
	*reg_address(reg) = regval;
}

static constexpr rcu::RCU_PCLK clockRegs[] = {
	rcu::RCU_PCLK::PCLK_USART0,
	rcu::RCU_PCLK::PCLK_USART1,
	rcu::RCU_PCLK::PCLK_USART2,
	rcu::RCU_PCLK::PCLK_UART3,
	rcu::RCU_PCLK::PCLK_UART4
};

#endif // ARDUINO_ARCH_MFL
