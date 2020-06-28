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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

 // VFD controlling of spindle, Contributed by Stefan de Bruijn.
 //
 // This implementation is compatible with the popular Huanyang P2x VFD's.
 // For other VFD's, the implementation is pretty much the same, but with
 // slightly different commands. 

#include "vfd_spindle.h"

#if HAS_CUTTER
#ifdef SPINDLE_VFD

#if defined(__SAM3X8E__) || defined(__SAMD21G18A__)
#if (VFD_RX_PIN == 19 && VFD_TX_PIN == 18)
#include <HardwareSerial.h>
#define VFDSerial Serial1
#elif (VFD_RX_PIN == 17 && VFD_TX_PIN == 16)
#include <HardwareSerial.h>
#define VFDSerial Serial2
#elif (VFD_RX_PIN == 15 && VFD_TX_PIN == 14)
#include <HardwareSerial.h>
#define VFDSerial Serial3
#endif
#endif

#if !defined(VFDSerial)
#ifdef VFD_PARITY
  #error "VFD parity is not supported when using software serial RX/TX ports";
#endif

#include "SoftwareSerial.h"
SoftwareSerial VFDSerial(VFD_RX_PIN, VFD_TX_PIN); // RX, TX
#define USE_SOFTWARE_SERIAL
#endif

#ifndef VFD_BAUD
#error "You should define VFD_BAUD according to the VFD settings"
#endif

#ifndef VFD_ADDRESS
// VFD_ADDRESS is 1 in 99,99% of the cases
#define VFD_ADDRESS 1
#endif

#ifndef VFD_MODBUS_PROTO
// VFD_MODBUS_PROTO is 0 in 99,99% of the cases
// F9.05 = 0 or 1, which indicates 1 or 2 byte length. 0 = default.
#define VFD_MODBUS_PROTO 0
#endif

// Some static variable initialization:
uint8_t VFDSpindle::vfd_receive_buffer[VFDSpindle::RECEIVE_BUFFER_SIZE];
uint8_t VFDSpindle::vfd_send_buffer[VFDSpindle::SEND_BUFFER_SIZE];
int VFDSpindle::direction = 0;
bool VFDSpindle::enabled = false;

cutter_power_t VFDSpindle::power;

uint16_t VFDSpindle::get_crc_value(uint8_t* data_value, uint8_t length)
{
  // Implementation from the manual... not efficient, but it works:

  uint16_t crc_value = 0xffff;
  while (length--)
  {
    crc_value ^= *data_value++;
    for (int i = 0; i < 8; ++i)
    {
      if (crc_value & 0x0001)
      {
        crc_value = (crc_value >> 1) ^ 0xa001u;
      }
      else
      {
        crc_value = crc_value >> 1;
      }
    }
  }

  return crc_value;
}

void VFDSpindle::crc_check_value(uint8_t* data_value, uint8_t length)
{
  uint16_t crc_value = get_crc_value(data_value, length);

  // Low byte first, then the high byte
  data_value[length + 0] = uint8_t(crc_value & 0xFF);
  data_value[length + 1] = uint8_t(crc_value >> 8);
}

bool VFDSpindle::validate_crc_value(uint8_t* data_value, uint8_t length)
{
  uint16_t crc_value = get_crc_value(data_value, length);

  return
    data_value[length + 0] == uint8_t(crc_value & 0xff) &&
    data_value[length + 1] == uint8_t(crc_value >> 8);
}

void VFDSpindle::init_pins()
{
  // Receive mode. 
  pinMode(VFD_RTS_PIN, OUTPUT);
  digitalWrite(VFD_RTS_PIN, LOW);
#ifdef VFD_RTS_PIN2
  pinMode(VFD_RTS_PIN2, OUTPUT);
  digitalWrite(VFD_RTS_PIN2, LOW);
#endif

  // Start serial port
#ifdef USE_SOFTWARE_SERIAL
  VFDSerial.begin(VFD_BAUD);// , SERIAL_8N1);
#else
  VFDSerial.begin(VFD_BAUD, VFD_PARITY);
#endif
}

int VFDSpindle::receive_data_detail()
{
  const auto waitTimePerChar = 1000000 / VFD_BAUD;
  const auto timeForEndPacket = 4 * waitTimePerChar;
  const auto maxWaitIterations = VFD_BAUD; // 0.1 second

  int index = 0;
  int n;
  for (int i = 0; i < maxWaitIterations && (n = VFDSerial.available()) == 0; ++i)
  {
    delayMicroseconds(waitTimePerChar);
  }

  if (n == 0)
  {
    // No data received within the allotted time:
    SERIAL_ECHOPGM("VFD/RS485 error: no response from VFD/RS485 within the allotted time.\r\n");
    return 0;
  }
  else
  {
    for (;;)
    {
      if (n + index > RECEIVE_BUFFER_SIZE)
      {
        // Read the remainder to flush the buffer:
        while ((n = VFDSerial.available()) != 0)
        {
          VFDSerial.read();
        }

        SERIAL_ECHOPGM("VFD/RS485 error: packet that was received from VFD/RS485 was too long and is ignored.\r\n");
        return 0;
      }
      else
      {
        // We have data. Read it to our buffer:
        VFDSerial.readBytes(vfd_receive_buffer + index, n);
        index += n;

        // Check if new data is available:
        n = VFDSerial.available();
        if (n == 0)
        {
          // Not yet, but we might just be polling too fast. The spec sais that we
          // need to wait the time of 4 characters:
          //
          // delayMicroseconds(timeForEndPacket);
          //
          // ... but that doesn't work. Instead, it can apparently take up to 20 ms
          // before the thing does its job and give us the right answer. Here goes.

          for (int i = 0; i < 20 && n == 0; ++i)
          {
            safe_delay(1);
            n = VFDSerial.available();
          }

          if (n == 0)
          {
            // Still no data, this means we're done.

#ifdef VFD_RS485_DEBUG
#ifdef VFD_RS485_DEBUG_PCK
            debug_rs485(false, vfd_receive_buffer, index);
#endif
#endif

            return index;
          }
        }
        // Otherwise there's new data (n>0) which we need to process
      }
    }
  }
}

void VFDSpindle::send_data_detail(uint8_t* buffer, int length)
{
  // send index
  crc_check_value(buffer, length);
  VFDSerial.flush();

  // We assume half-duplex communication:
  digitalWrite(VFD_RTS_PIN, HIGH);
#ifdef VFD_RTS_PIN2
  digitalWrite(VFD_RTS_PIN2, HIGH);
#endif

  const auto waitTimePerChar = 1000000 / VFD_BAUD;
  delayMicroseconds(waitTimePerChar * 4);

  // Before we set the MAX485 chip RTS to low, we have to flush:
  VFDSerial.write(buffer, length + 2);
  VFDSerial.flush();

  // And immediately set it back to low, to ensure that
  // incoming data gets processed by the MAX485.
  digitalWrite(VFD_RTS_PIN, LOW);
#ifdef VFD_RTS_PIN2
  digitalWrite(VFD_RTS_PIN2, LOW);
#endif

#ifdef VFD_RS485_DEBUG
#ifdef VFD_RS485_DEBUG_PCK
  debug_rs485(true, buffer, length + 2);
#endif
#endif
}

int VFDSpindle::query(int send_length)
{
  while (true)
  {
    send_data_detail(vfd_send_buffer, send_length);

    // Each iteration of receive_data_detail will take at most 0.1 seconds. 50 iterations = ~5 seconds
    // Under normal circumstances the communication should be *much* faster though, to be exact: it would 
    // take `((number of characters + 8) * 9) / baud` seconds. Typical is 8 characters, which is 0.0075s.
    //
    // All measures here are mostly there for when things are wrong, like when your VFD is unresponsive.
    // You don't want a broken piece, so better to just wait until the problem is resolved...
    for (int i = 0; i < 50; ++i)
    {
      int n = receive_data_detail();

      // n = 0 -> No response. Give it some time:
      if (n == 0)
      {
        safe_delay(10);
        send_data_detail(vfd_send_buffer, send_length);
      }
      else if (n > 0)
      {
        // If we have a response, we have to check the CRC16 checksum:
        if (!validate_crc_value(vfd_receive_buffer, n - 2))
        {
          // No luck, we have to try again, because the checksum failed:
          SERIAL_ECHOPGM("VFD/RS485 error: communication checksum failed, have to retry.\r\n");

          send_data_detail(vfd_send_buffer, send_length);
        }
        else
        {
          // CRC validates OK:
          // 
          // We expect: 01.    03.   0002   0002
          //            [addr] [cmd] [len]  [data]
          //
          // We ignore len; it can vary with packages and we already checked CRC.
          if (vfd_receive_buffer[0] == VFD_ADDRESS && vfd_receive_buffer[1] == vfd_send_buffer[1])
          {
            // error if it doesn't add up
            return n;
          }
          else if (vfd_receive_buffer[1] != vfd_send_buffer[1])
          {
            // error can be a colliding packet
            // 
            // try again:
            SERIAL_ECHOPGM("VFD/RS485 error: response originated from other modbus address\r\n");

            send_data_detail(vfd_send_buffer, send_length);
          }
        }
      }
    }

    SERIAL_ECHOPGM("VFD/RS485 error: general error communicating with VFD/RS485. Check baud rate and parity settings!\r\n");
  }
}

// Gets the current direction status of the VFD:
// -1 = backward, 1 = forward, 0 = idle, -2 = communication error.
int VFDSpindle::get_direction_state()
{
  // Send: 01 03 30 00 00 01

  vfd_send_buffer[0] = VFD_ADDRESS;
  vfd_send_buffer[1] = 0x03; // READ
  vfd_send_buffer[2] = 0x30; // Command group ID
  vfd_send_buffer[3] = 0x00;
  vfd_send_buffer[4] = 0x00; // Message ID
  vfd_send_buffer[5] = 0x01;

  int count = query(6);

  if (count < 6) { return -2; }

  // Receive: 01 03 00 02 00 02
  //                      ----- status
  uint16_t status =
    (uint16_t(vfd_receive_buffer[4]) << 8) |
    uint16_t(vfd_receive_buffer[5]);

  switch (status)
  {
  case 1: return 1;
  case 2: return -1;
  case 3: return 0;
  default:
    // Other values will result in -2, which will result in a command to set it.
    return -2;
  }
}

uint16_t VFDSpindle::get_max_rpm()
{
  static uint16_t max_rpm = 0;

  if (max_rpm == 0)
  {
    // Send: 01 03 B005 0002

    vfd_send_buffer[0] = VFD_ADDRESS;
    vfd_send_buffer[1] = 0x03; // READ
    vfd_send_buffer[2] = 0xB0; // B0.05 = Get RPM
    vfd_send_buffer[3] = 0x05;
    vfd_send_buffer[4] = 0x00; // Read 2 values
    vfd_send_buffer[5] = 0x02;

    int count = query(6);

    //  Recv: 01 03 00 04 5D C0 03 F6 
    //                    -- -- = 24000 (val #1)
    uint16_t rpm =
      (uint16_t(vfd_receive_buffer[4]) << 8) |
      uint16_t(vfd_receive_buffer[5]);

    max_rpm = rpm;

#ifdef VFD_RS485_DEBUG
    SERIAL_ECHOPGM("VFD max rpm is ");
    SERIAL_ECHO(rpm);
    SERIAL_ECHOLNPGM(".\n");
#endif
  }

  return max_rpm;
}

uint16_t VFDSpindle::get_current_rpm()
{
  // Send: 01 03 700C 0002

  vfd_send_buffer[0] = VFD_ADDRESS;
  vfd_send_buffer[1] = 0x03; // READ
  vfd_send_buffer[2] = 0x70; // B0.05 = Get RPM
  vfd_send_buffer[3] = 0x0C;
  vfd_send_buffer[4] = 0x00; // Read 2 values
  vfd_send_buffer[5] = 0x02;

  int count = query(6);

  //  Recv: 01 03 0004 095D 0000
  //                   ---- = 2397 (val #1)
  uint16_t rpm =
    (uint16_t(vfd_receive_buffer[4]) << 8) |
    uint16_t(vfd_receive_buffer[5]);

  return rpm;
}

// direction<0: reverse
// direction>0: forward
// direction=0: stop
void VFDSpindle::set_current_direction(int direction)
{
  vfd_send_buffer[0] = VFD_ADDRESS;
  vfd_send_buffer[1] = 0x06; // WRITE
  vfd_send_buffer[2] = 0x20; // Command ID 0x2000
  vfd_send_buffer[3] = 0x00;
  vfd_send_buffer[4] = 0x00;
  vfd_send_buffer[5] = direction < 0 ? 0x02 : (direction == 0 ? 0x06 : 0x01);

  query(6);
}

void VFDSpindle::set_speed(uint16_t rpm)
{
  // We have to know the max RPM before we can set the current RPM:
  auto max_rpm = get_max_rpm();

  // Speed is in [0..10'000] where 10'000 = 100%.
  // We have to use a 32-bit integer here; typical values are 10k/24k rpm.
  // I've never seen a 400K RPM spindle in my life, and they aren't supported
  // by this modbus protocol anyways... So I guess this is OK.
  uint16_t speed = (uint32_t(rpm) * 10000L) / uint32_t(max_rpm);
  if (speed < 0) { speed = 0; }
  if (speed > 10000) { speed = 10000; }

  vfd_send_buffer[0] = VFD_ADDRESS;
  vfd_send_buffer[1] = 0x06; // WRITE
  vfd_send_buffer[2] = 0x10; // Command ID 0x1000
  vfd_send_buffer[3] = 0x00;
  vfd_send_buffer[4] = uint8_t(speed >> 8); // RPM
  vfd_send_buffer[5] = uint8_t(speed & 0xFF);

  query(6);

#ifdef VFD_RS485_DEBUG
  SERIAL_ECHOPGM("VFD speed set to ");
  SERIAL_ECHO(speed);
  SERIAL_ECHOLNPGM(" (0-10k).\n");
#endif
}

// Apparently some G-codes return 0-255 instead of a normal value... This
// normalizes these numbers to RPM's.
uint16_t VFDSpindle::normalize_power(int32_t value)
{
  if (value < 0)
  {
    int32_t rpm = int32_t(get_max_rpm()) * (-value) / 255L;
    return uint16_t(rpm);
  }
  else
  {
    return uint16_t(value);
  }
}

void VFDSpindle::init()
{
#ifdef VFD_RS485_DEBUG
  SERIAL_ECHOPGM("VFD initializing.\r\n");
#endif

  power = 0;
  direction = 0; // forward
  enabled = false;

  init_pins();
  safe_delay(10); // give it some time to start up...

  // Initialize max_rpm value:
  get_max_rpm();

  // If the spindle is running, we're going to stop it:
  if (get_direction_state() != 0)
  {
    set_current_direction(0);
  }

  // Set speed to 0 RPM:
  if (get_current_rpm() != 0)
  {
    set_speed(0);
  }

}

void VFDSpindle::set_power(const int32_t pwr)
{
#ifdef VFD_RS485_DEBUG
  SERIAL_ECHOPGM("VFD set power to ");
  SERIAL_ECHO(pwr);
  SERIAL_ECHOLNPGM(" rpm.\r\n");
#endif

  auto norm = normalize_power(pwr);
  power = norm;

  if (norm == 0)
  {
    enabled = false;

    set_speed(norm);
    set_current_direction(0);
    power_delay();
  }

  if (enabled && direction != 0)
  {
    set_current_direction(direction);
    set_speed(norm);
    power_delay();
  }
}

void VFDSpindle::apply_power(const int32_t inpow)
{
  static int32_t last_power_applied = -1;
  if (inpow == last_power_applied) return;
  last_power_applied = inpow;

  set_power(inpow);
}

void VFDSpindle::set_enabled(const bool enable)
{
#ifdef VFD_RS485_DEBUG
  SERIAL_ECHOPGM("VFD set enabled ");
  SERIAL_ECHO(enable?"true":"false");
  SERIAL_ECHOLNPGM(".\r\n");
#endif

  enabled = enable;

  if (!enable)
  {
    // stop
    set_current_direction(0);
    power_delay();
  }
  else if (power != 0 && direction != 0) // && enable
  {
    set_current_direction(direction);
    set_speed(power);
    power_delay();
  }
}

// Wait for spindle to spin up or spin down
void VFDSpindle::power_delay()
{
  int target_rpm = power;

  // Wait while we're not up to speed. Tolerance is 5% of max rpm.
  // See VFD doc for details.
  auto tolerance = get_max_rpm() / 20;
  while (true)
  {
    auto current_speed = get_current_rpm();

#ifdef VFD_RS485_DEBUG
    SERIAL_ECHOPGM("VFD current speed ");
    SERIAL_ECHO(current_speed);
    SERIAL_ECHOLNPGM(".\r\n");
#endif

    if (current_speed >= (target_rpm - tolerance) &&
      current_speed <= (target_rpm + tolerance))
    {
      return;
    }
    else
    {
      safe_delay(100);
    }
  }
}

void VFDSpindle::set_direction(const bool reverse)
{
  auto newDirection = reverse ? -1 : 1;
  if (direction != newDirection)
  {
#ifdef VFD_RS485_DEBUG
    SERIAL_ECHOPGM("VFD set direction to ");
    SERIAL_ECHO(newDirection<0?"reverse":"forward");
    SERIAL_ECHOLNPGM(".\r\n");
#endif

    direction = newDirection;

    // If it's enabled, we have to reverse the running direction
    if (enabled)
    {
      // First stop...
      auto rpm = power;
      power = 0;

      // Wait till we've stopped:
      set_current_direction(0);
      power_delay();

      // Enable again and wait till we hit the target rpm:
      set_current_direction(direction);
      power = rpm;
      set_speed(power);
      power_delay();
    }
  }
}

#endif // SPINDLE_VFD
#endif // HAS_CUTTER
