/**
#incl
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
int VFDSpindle::direction = 1;
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
  VFDSerial.begin(VFD_BAUD, SERIAL_8N1);
#endif
}

int VFDSpindle::receive_data_detail()
{
  int index = 0;
  int n = VFDSerial.available();

  // We iterate max 10 times. We could iterate more, but if we are flooded it
  // just doesn't make sense. Also, note that 10 iterations should be more than
  // enough.
  for (int i = 0; i < 10 && n != 0; ++i)
  {
    if (n + index > RECEIVE_BUFFER_SIZE) { break; }

    if (n > 0)
    {
      VFDSerial.readBytes(vfd_receive_buffer + index, n);
      index += n;

      const auto waitTimePerChar = 1000000 / VFD_BAUD;
      // delayMicroseconds(waitTimePerChar * 40);
      safe_delay(10);

      n = VFDSerial.available();
    }
    else if (n == 0)
    {
      break;
    }
  }

  // if (index != 0) {
  //   Debug(false, vfd_receive_buffer, index);
  // }

  return index;
}

void VFDSpindle::send_data_detail(uint8_t* buffer, int length)
{
  // send index
  crc_check_value(buffer, length);

  digitalWrite(VFD_RTS_PIN, HIGH);
#ifdef VFD_RTS_PIN2
  digitalWrite(VFD_RTS_PIN2, HIGH);
#endif

  const auto waitTimePerChar = 1000000 / VFD_BAUD;
  delayMicroseconds(waitTimePerChar * 4);

  VFDSerial.write(buffer, length + 2);

  delayMicroseconds(waitTimePerChar * 4);

  digitalWrite(VFD_RTS_PIN, LOW);
#ifdef VFD_RTS_PIN2
  digitalWrite(VFD_RTS_PIN2, LOW);
#endif

  // Debug(true, buffer, length + 2);
}

int VFDSpindle::query(int send_length)
{
  while (true)
  {
    send_data_detail(vfd_send_buffer, send_length);

    for (int i = 0; i < 500; ++i) // We give the thing 100x10ms = 1 second before erroring.
    {
      int n = receive_data_detail();
      if (n == 0)
      {
        safe_delay(10);
      }
      else if (!validate_crc_value(vfd_receive_buffer, n))
      {
        // try again, but don't reset 'i':
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
        if (vfd_receive_buffer[0] == VFD_ADDRESS &&
          vfd_receive_buffer[1] == vfd_send_buffer[1])
        {
          // error if it doesn't add up
          return n;
        }
        else if (vfd_receive_buffer[1] != vfd_send_buffer[1])
        {
          // error can be a colliding packet
          // 
          // try again:
          send_data_detail(vfd_send_buffer, send_length);
        }
      }
    }

    SERIAL_ECHOPGM("Error communicating with VFD/RS485.\r\n");
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
  power = 0;
  direction = 1; // forward
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

void VFDSpindle::set_power(const cutter_power_t pwr)
{
  power = normalize_power(pwr);

  if (enabled && direction != 0)
  {
    set_current_direction(direction);
    set_speed(power);
    power_delay();
  }
}

void VFDSpindle::set_enabled(const bool enable)
{
  enabled = enable;

  if (!enable)
  {
    // stop
    set_current_direction(0);
  }
  else if (power != 0 && direction != 0) // && enable
  {
    set_current_direction(direction);
    set_speed(power);
    power_delay();
  }
}

void VFDSpindle::apply_power(const cutter_power_t inpow)
{
  auto rpm = normalize_power(inpow);
  set_speed(rpm);
  power_delay();
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

    if (current_speed >= (target_rpm - tolerance) &&
      current_speed <= (target_rpm + tolerance))
    {
      return;
    }
  }
}

void VFDSpindle::set_direction(const bool reverse)
{
  auto newDirection = reverse ? -1 : 1;
  if (direction != newDirection)
  {
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
      power_delay();
    }
  }
}

#endif // SPINDLE_VFD
#endif // HAS_CUTTER
