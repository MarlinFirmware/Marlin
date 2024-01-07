/// @file
#pragma once
#include <stdint.h>

namespace modules {

// clang-format off
// prevent silly indenting of the whole file

/// Contains all the necessary functions for computation of CRC
namespace crc {

class CRC8 {
public:
    /// Compute/update CRC8 CCIIT from 8bits.
    /// Details: https://www.nongnu.org/avr-libc/user-manual/group__util__crc.html
    static uint8_t CCITT_update(uint8_t crc, uint8_t b);

    static constexpr uint8_t CCITT_updateCX(uint8_t crc, uint8_t b) {
        uint8_t data = crc ^ b;
        for (uint8_t i = 0; i < 8; i++) {
            if ((data & 0x80U) != 0) {
                data <<= 1U;
                data ^= 0x07U;
            } else {
                data <<= 1U;
            }
        }
        return data;
    }

    /// Compute/update CRC8 CCIIT from 16bits (convenience wrapper)
    static constexpr uint8_t CCITT_updateW(uint8_t crc, uint16_t w) {
        union U {
            uint8_t b[2];
            uint16_t w;
            explicit constexpr inline U(uint16_t w)
                : w(w) {}
        } u(w);
        return CCITT_updateCX(CCITT_updateCX(crc, u.b[0]), u.b[1]);
    }
};

} // namespace crc

// clang-format on

} // namespace modules
