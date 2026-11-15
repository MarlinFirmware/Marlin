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
 * feature/canbus/klipper_can.cpp
 *
 * The admin protocol follows Klipper's src/generic/canserial.c (GPLv3, Copyright Kevin O'Connor).
 */

#include "../../inc/MarlinConfig.h"

#if ENABLED(CAN_BUS)

#include "klipper_can.h"
#include "../../MarlinCore.h" // for marlin.idle()

using namespace KlipperProto;

// Admin commands (byte 0 of a frame to ADMIN_ID)
#define CMD_QUERY_UNASSIGNED    0x00
#define CMD_SET_KLIPPER_NODEID  0x01
#define RESP_NEED_NODEID        0x20

// Fixed message IDs (klippy/msgproto.py DefaultMessages)
#define MSGID_IDENTIFY_RESPONSE 0
#define MSGID_IDENTIFY          1
#define IDENTIFY_CHUNK          40

uint8_t KlipperCAN::seq, // = 0
        KlipperCAN::rx_buf[KlipperProto::MESSAGE_MAX * 2],
        KlipperCAN::rx_have; // = 0

uint8_t KlipperCAN::query_unassigned(node_t nodes[], const uint8_t max, const millis_t wait_ms/*=500*/) {
  can_frame_t frame = { ADMIN_ID, 1, { CMD_QUERY_UNASSIGNED } };
  if (!CANBus::send(frame)) return 0;

  uint8_t count = 0;
  for (const millis_t end_ms = millis() + wait_ms; PENDING(millis(), end_ms);) {
    if (!CANBus::read(frame)) { marlin.idle(); continue; }
    if (frame.id != ADMIN_RESP_ID || frame.len < 7 || frame.data[0] != RESP_NEED_NODEID) continue;
    // Ignore repeated replies from the same node
    bool dup = false;
    for (uint8_t i = 0; i < count && !dup; ++i) dup = !memcmp(nodes[i].uuid, &frame.data[1], KLIPPER_CAN_UUID_LEN);
    if (dup || count >= max) continue;
    memcpy(nodes[count].uuid, &frame.data[1], KLIPPER_CAN_UUID_LEN);
    nodes[count].app = frame.len > 7 ? frame.data[7] : uint8_t(APP_KLIPPER);
    ++count;
  }
  return count;
}

bool KlipperCAN::set_nodeid(const uint8_t (&uuid)[KLIPPER_CAN_UUID_LEN], const uint8_t nodeid) {
  can_frame_t frame = { ADMIN_ID, 8, { CMD_SET_KLIPPER_NODEID } };
  memcpy(&frame.data[1], uuid, KLIPPER_CAN_UUID_LEN);
  frame.data[7] = nodeid;
  seq = 0;
  rx_have = 0;
  return CANBus::send(frame);
}

// Wrap a payload in a message block and send it to the node in 8-byte frames
bool KlipperCAN::send_block(const uint8_t nodeid, const uint8_t *payload, const uint8_t len) {
  uint8_t block[MESSAGE_MAX];
  const uint8_t blen = make_block(block, seq, payload, len);
  if (!blen) return false;

  can_frame_t frame;
  frame.id = tx_id(nodeid);
  for (uint8_t pos = 0; pos < blen;) {
    frame.len = _MIN(blen - pos, 8);
    memcpy(frame.data, &block[pos], frame.len);
    for (const millis_t end_ms = millis() + 50; !CANBus::send(frame);) {
      if (ELAPSED(millis(), end_ms)) return false;
      marlin.idle();
    }
    pos += frame.len;
  }
  return true;
}

// Collect the node's byte stream until one valid message block is found. Return its length or 0.
// Bytes after the block stay in the buffer for the next call.
uint8_t KlipperCAN::receive_block(const uint8_t nodeid, uint8_t (&block)[MESSAGE_MAX], const millis_t timeout_ms) {
  can_frame_t frame;
  for (const millis_t end_ms = millis() + timeout_ms;;) {
    // Drop bytes that can't start a block, then look for a whole block
    for (;;) {
      while (rx_have && (rx_buf[0] < MESSAGE_MIN || rx_buf[0] > MESSAGE_MAX)) memmove(rx_buf, rx_buf + 1, --rx_have);
      if (!rx_have || rx_have < rx_buf[0]) break;
      if (block_valid(rx_buf, rx_have)) {
        const uint8_t blen = rx_buf[0];
        memcpy(block, rx_buf, blen);
        rx_have -= blen;
        memmove(rx_buf, rx_buf + blen, rx_have);
        seq = block[1] & MESSAGE_SEQ_MASK;  // The node reports the sequence number it expects next
        return blen;
      }
      memmove(rx_buf, rx_buf + 1, --rx_have);   // Bad block. Skip a byte and look again.
    }
    if (ELAPSED(millis(), end_ms)) return 0;
    if (!CANBus::read(frame)) { marlin.idle(); continue; }
    if (frame.id != rx_id(nodeid)) continue;
    for (uint8_t i = 0; i < frame.len && rx_have < sizeof(rx_buf); ++i) rx_buf[rx_have++] = frame.data[i];
  }
}

int32_t KlipperCAN::read_identify(const uint8_t nodeid, uint32_t &adler) {
  uint32_t offset = 0, s1 = 1, s2 = 0;
  for (uint8_t tries = 0; tries < 5;) {
    uint8_t payload[12], *p = payload;
    *p++ = MSGID_IDENTIFY;
    p = encode_vlq(p, offset);
    p = encode_vlq(p, IDENTIFY_CHUNK);
    if (!send_block(nodeid, payload, p - payload)) return -1;

    // Look for the identify_response. An empty block is an ack, or a nak with the expected sequence.
    uint8_t block[MESSAGE_MAX];
    bool got_data = false;
    for (uint8_t blen; !got_data && (blen = receive_block(nodeid, block, 250));) {
      if (blen == MESSAGE_MIN) continue;
      const uint8_t *r = &block[MESSAGE_HEADER_SIZE];
      if (*r++ != MSGID_IDENTIFY_RESPONSE) continue;
      if (decode_vlq(r) != offset) continue;
      const uint8_t dlen = *r++;
      if (dlen == 0) { adler = (s2 << 16) | s1; return offset; } // End of the dictionary
      for (uint8_t i = 0; i < dlen; ++i) { s1 = (s1 + r[i]) % 65521; s2 = (s2 + s1) % 65521; }
      offset += dlen;
      got_data = true;
    }
    if (got_data) tries = 0; else ++tries; // Resend with the node's sequence number on timeout or nak
  }
  return -1;
}

#endif // CAN_BUS
