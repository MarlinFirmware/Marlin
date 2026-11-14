/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * Klipper MCU protocol encodings used by CAN_BUS.
 * Expected bytes come from Klipper's own klippy/msgproto.py.
 */

#include "../test/unit_tests.h"
#include "src/feature/canbus/klipper_proto.h"

using namespace KlipperProto;

MARLIN_TEST(klipper_proto, crc16_check_value) {
  // CRC-16/MCRF4XX check value
  const uint8_t s[] = { '1', '2', '3', '4', '5', '6', '7', '8', '9' };
  TEST_ASSERT_EQUAL_HEX16(0x6F91, crc16_ccitt(s, sizeof(s)));
}

typedef struct { int32_t value; uint8_t len; uint8_t bytes[5]; } vlq_case_t;

static const vlq_case_t vlq_cases[] = {
  {           0, 1, { 0x00 } },
  {           1, 1, { 0x01 } },
  {          95, 1, { 0x5F } },
  {          96, 2, { 0x80, 0x60 } },
  {          -1, 1, { 0x7F } },
  {         -32, 1, { 0x60 } },
  {         -33, 2, { 0xFF, 0x5F } },
  {       12287, 2, { 0xDF, 0x7F } },
  {       12288, 3, { 0x80, 0xE0, 0x00 } },
  {       -4096, 2, { 0xE0, 0x00 } },
  {       -4097, 3, { 0xFF, 0xDF, 0x7F } },
  {     1572864, 4, { 0x80, 0xE0, 0x80, 0x00 } },
  {     -262145, 3, { 0xEF, 0xFF, 0x7F } },
  {   201326592, 5, { 0x80, 0xE0, 0x80, 0x80, 0x00 } },
  {   -33554433, 4, { 0xEF, 0xFF, 0xFF, 0x7F } },
  {  2147483647, 5, { 0x87, 0xFF, 0xFF, 0xFF, 0x7F } },
  { -2147483647 - 1, 5, { 0xF8, 0x80, 0x80, 0x80, 0x00 } }
};

MARLIN_TEST(klipper_proto, vlq_encode) {
  for (const vlq_case_t &c : vlq_cases) {
    uint8_t buf[5] = { 0 };
    const uint8_t len = encode_vlq(buf, uint32_t(c.value)) - buf;
    TEST_ASSERT_EQUAL_UINT8(c.len, len);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(c.bytes, buf, c.len);
  }
}

MARLIN_TEST(klipper_proto, vlq_decode) {
  for (const vlq_case_t &c : vlq_cases) {
    const uint8_t *p = c.bytes;
    TEST_ASSERT_EQUAL_INT32(c.value, int32_t(decode_vlq(p)));
    TEST_ASSERT_EQUAL_UINT8(c.len, p - c.bytes);
  }
}

MARLIN_TEST(klipper_proto, identify_block) {
  // "identify offset=0 count=40" with sequence 0
  const uint8_t payload[] = { 0x01, 0x00, 0x28 },
                expect[] = { 0x08, 0x10, 0x01, 0x00, 0x28, 0x5E, 0x9F, 0x7E };
  uint8_t block[MESSAGE_MAX];
  TEST_ASSERT_EQUAL_UINT8(sizeof(expect), make_block(block, 0, payload, sizeof(payload)));
  TEST_ASSERT_EQUAL_HEX8_ARRAY(expect, block, sizeof(expect));
  TEST_ASSERT_TRUE(block_valid(block, sizeof(expect)));
  TEST_ASSERT_FALSE(block_valid(block, sizeof(expect) - 1));  // Incomplete
  block[3] ^= 1;
  TEST_ASSERT_FALSE(block_valid(block, sizeof(expect)));      // Bad CRC
}

MARLIN_TEST(klipper_proto, block_limits) {
  uint8_t block[MESSAGE_MAX], payload[MESSAGE_MAX] = { 0 };
  TEST_ASSERT_EQUAL_UINT8(MESSAGE_MAX, make_block(block, 15, payload, MESSAGE_MAX - MESSAGE_MIN));
  TEST_ASSERT_EQUAL_HEX8(0x1F, block[1]);
  TEST_ASSERT_TRUE(block_valid(block, MESSAGE_MAX));
  TEST_ASSERT_EQUAL_UINT8(0, make_block(block, 0, payload, MESSAGE_MAX - MESSAGE_MIN + 1));
  const uint8_t ack[] = { 0x05, 0x13 };
  TEST_ASSERT_EQUAL_UINT8(MESSAGE_MIN, make_block(block, 3, ack, 0));
  TEST_ASSERT_EQUAL_HEX8_ARRAY(ack, block, 2);
}
