/********************
 * ftdi_eve_spi.cpp *
 ********************/

/****************************************************************************
 *   Written By Mark Pelletier  2017 - Aleph Objects, Inc.                  *
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#include "../../Marlin.h"
#include "../ui_config.h"

#if defined(FTDI_EVE_TOUCHSCREEN)

#include "ftdi_eve_constants.h"
#include "ftdi_eve_functions.h"

#if defined(PIN_DIR_OUT)
  // If SET_OUTPUT is defined, then map Marlin routines to those
  #define SET_OUTPUT                PIN_DIR_OUT
  #define SET_INPUT_PULLUP(a)       _PIN_DIR_IN(a); _PIN_HIGH(a);
  #define SET_INPUT                 PIN_DIR_IN
  #define WRITE                     SET_PIN
  #define READ                      GET_PIN
#endif

#define MULTIPLE_OF_4(val) ((((val)+3)>>2)<<2)

/********************************* SPI Functions *********************************/

void CLCD::spi_init (void) {
#if defined(USE_ARDUINO_HW_SPI)
  SPI.begin();
  SPI.beginTransaction(LCDsettings);
#else
  SET_OUTPUT(CLCD_MOD_RESET);         // CLCD_MOD_RST - Module Reset, not SPI
  WRITE(CLCD_MOD_RESET, 1);

  SET_OUTPUT(CLCD_SOFT_SPI_MOSI);       // CLCD_MOSI
  WRITE(CLCD_SOFT_SPI_MOSI, 1);

  SET_OUTPUT(CLCD_SOFT_SPI_SCLK);       // CLCD_SCLK
  WRITE(CLCD_SOFT_SPI_SCLK, 0);

  SET_OUTPUT(CLCD_SOFT_SPI_CS);         // CLCD_CS
  WRITE(CLCD_SOFT_SPI_CS, 1);

  SET_INPUT_PULLUP(CLCD_SOFT_SPI_MISO); // CLCD_MISO

  delay(50);
#endif
}

// CLCD Bitbanged SPI - Chip Select
void CLCD::spi_select (void) {
  WRITE(CLCD_SOFT_SPI_CS, 0);
}

// CLCD Bitbanged SPI - Chip Deselect
void CLCD::spi_deselect (void) {
  WRITE(CLCD_SOFT_SPI_CS, 1);
}

void CLCD::reset (void) {
  WRITE(CLCD_MOD_RESET, 0);
  delay(100);
  WRITE(CLCD_MOD_RESET, 1);
}

void CLCD::test_pulse(void)
{
  #if defined(CLCD_AUX_0)
    WRITE(CLCD_AUX_0, 1);
    delayMicroseconds(10);
    WRITE(CLCD_AUX_0, 0);
  #endif
}

uint8_t CLCD::spi_transfer (uint8_t spiOutByte) {
#ifdef IS_ARDUINO
  SPI.transfer(spiOutByte);
#else

  uint8_t spiIndex  = 0x80;
  uint8_t spiInByte = 0;
  uint8_t k;

  for(k = 0; k <8; k++) {         // Output and Read each bit of spiOutByte and spiInByte
    if(spiOutByte & spiIndex) {   // Output MOSI Bit
      WRITE(CLCD_SOFT_SPI_MOSI, 1);
    }
    else {
      WRITE(CLCD_SOFT_SPI_MOSI, 0);
    }
    WRITE(CLCD_SOFT_SPI_SCLK, 1);   // Pulse Clock
    WRITE(CLCD_SOFT_SPI_SCLK, 0);

    if(READ(CLCD_SOFT_SPI_MISO)) {
      spiInByte |= spiIndex;
    }

    spiIndex >>= 1;
  }
  return spiInByte;
#endif
}

// MEMORY READ FUNCTIONS

// Write 4-Byte Address
void CLCD::mem_read_addr (uint32_t reg_address) {
  spi_transfer((reg_address >> 16) & 0x3F);  // Address [21:16]
  spi_transfer((reg_address >> 8 ) & 0xFF);  // Address [15:8]
  spi_transfer((reg_address >> 0)  & 0xFF);  // Address [7:0]
  spi_transfer(0x00);                        // Dummy Byte
}

// Write 4-Byte Address, Read Multiple Bytes
void CLCD::mem_read_bulk (uint32_t reg_address, uint8_t *data, uint16_t len) {
  spi_select();
  mem_read_addr(reg_address);
  while(len--) {
    *data = spi_transfer(0x00);
    *data++;
  }
  spi_deselect();
}

// Write 4-Byte Address, Read 1-Byte Data
uint8_t CLCD::mem_read_8 (uint32_t reg_address) {
  spi_select();
  mem_read_addr(reg_address);
  uint8_t r_data = spi_transfer(0x00);
  spi_deselect();
  return r_data;
}

// Write 4-Byte Address, Read 2-Bytes Data
uint16_t CLCD::mem_read_16 (uint32_t reg_address) {
  spi_select();
  mem_read_addr(reg_address);
  uint16_t r_data =  (((uint16_t) spi_transfer(0x00)) << 0) |
                     (((uint16_t) spi_transfer(0x00)) << 8);
  spi_deselect();
  return r_data;
}

// Write 4-Byte Address, Read 4-Bytes Data
uint32_t CLCD::mem_read_32 (uint32_t reg_address) {
  spi_select();
  mem_read_addr(reg_address);
  uint32_t r_data =  (((uint32_t) spi_transfer(0x00)) <<  0) |
                     (((uint32_t) spi_transfer(0x00)) <<  8) |
                     (((uint32_t) spi_transfer(0x00)) << 16) |
                     (((uint32_t) spi_transfer(0x00)) << 24);
  spi_deselect();
  return r_data;
}


// MEMORY WRITE FUNCTIONS

 // Write 3-Byte Address
void CLCD::mem_write_addr (uint32_t reg_address) {
  spi_transfer((reg_address >> 16) | 0x80); // Address [21:16]
  spi_transfer((reg_address >> 8 ) & 0xFF); // Address [15:8]
  spi_transfer((reg_address >> 0)  & 0xFF); // Address [7:0]
}

// Write 3-Byte Address, Multiple Bytes, plus padding bytes
void CLCD::mem_write_bulk (uint32_t reg_address, const void *data, uint16_t len, uint8_t padding) {
  const uint8_t* p = (const uint8_t *)data;
  spi_select();
  mem_write_addr(reg_address);
  // Write data bytes
  while(len--) {
    spi_transfer(*p++);
  }
  // Write padding bytes
  while(padding--) {
    spi_transfer(0);
  }
  spi_deselect();
}

void CLCD::mem_write_bulk (uint32_t reg_address, progmem_str str, uint16_t len, uint8_t padding) { // Write 3-Byte Address, Multiple Bytes, plus padding bytes
  const uint8_t* p = (const uint8_t *) str;
  spi_select();
  mem_write_addr(reg_address);
  // Write data bytes
  while(len--) {
    spi_transfer(pgm_read_byte_near(p++));
  }
  // Write padding bytes
  while(padding--) {
    spi_transfer(0);
  }
  spi_deselect();
}

// Write 3-Byte Address, Write 1-Byte Data
void CLCD::mem_write_8 (uint32_t reg_address, uint8_t w_data) {
  spi_select();
  mem_write_addr(reg_address);
  spi_transfer(w_data);
  spi_deselect();
}

// Write 3-Byte Address, Write 2-Bytes Data
void CLCD::mem_write_16 (uint32_t reg_address, uint16_t w_data) {
  spi_select();
  mem_write_addr(reg_address);
  spi_transfer((uint8_t) ((w_data >> 0) & 0x00FF));
  spi_transfer((uint8_t) ((w_data >> 8) & 0x00FF));
  spi_deselect();
}

// Write 3-Byte Address, Write 4-Bytes Data
void CLCD::mem_write_32 (uint32_t reg_address, uint32_t w_data) {
  spi_select();
  mem_write_addr(reg_address);
  spi_transfer(w_data >> 0);
  spi_transfer(w_data >> 8);
  spi_transfer(w_data >> 16);
  spi_transfer(w_data >> 24);
  spi_deselect();
}

/**************************** FT800/810 Co-Processor Command FIFO ****************************/

uint32_t CLCD::CommandFifo::get_reg_cmd_write() {
  return mem_read_32(REG_CMD_WRITE) & 0x0FFF;
}

uint32_t CLCD::CommandFifo::get_reg_cmd_read() {
  return mem_read_32(REG_CMD_READ) & 0x0FFF;
}

bool CLCD::CommandFifo::is_idle() {
  return get_reg_cmd_read() == get_reg_cmd_write();
}

void CLCD::CommandFifo::wait_until_idle() {
  #if defined(UI_FRAMEWORK_DEBUG)
    const uint32_t startTime = millis();
  #endif
  do {
    #if defined(UI_FRAMEWORK_DEBUG)
      if(millis() - startTime > 3) {
        #if defined (SERIAL_PROTOCOLLNPGM)
          SERIAL_PROTOCOLLNPGM("Timeout on CommandFifo::Wait_Until_Idle()");
        #else
          Serial.println(F("Timeout on CommandFifo::Wait_Until_Idle()"));
        #endif
        break;
      }
    #endif
  } while(!is_idle());
}

#if defined(USE_FTDI_FT800)
void CLCD::CommandFifo::start() {
  if(command_write_ptr == 0xFFFFFFFFul) {
    command_write_ptr = get_reg_cmd_write();
  }
}

void CLCD::CommandFifo::execute() {
  if(command_write_ptr != 0xFFFFFFFFul) {
    mem_write_32(REG_CMD_WRITE, command_write_ptr);
  }
}

void CLCD::CommandFifo::reset() {
  mem_write_32(REG_CMD_WRITE, 0x00000000);
  mem_write_32(REG_CMD_READ,  0x00000000);
  command_write_ptr = 0xFFFFFFFFul;
};

template <class T> void CLCD::CommandFifo::_write_unaligned(T data, uint16_t len) {
  const char *ptr = (const char*)data;
  uint32_t bytes_tail, bytes_head;
  uint32_t command_read_ptr;

  #if defined(UI_FRAMEWORK_DEBUG)
  if(command_write_ptr == 0xFFFFFFFFul) {
    #if defined (SERIAL_PROTOCOLLNPGM)
      SERIAL_PROTOCOLLNPGM("Attempt to write to FIFO before CommandFifo::Cmd_Start().");
    #else
      Serial.println(F("Attempt to write to FIFO before CommandFifo::Cmd_Start()."));
    #endif
  }
  #endif

  /* Wait until there is enough space in the circular buffer for the transfer */
  do {
    command_read_ptr = get_reg_cmd_read();
    if (command_read_ptr <= command_write_ptr) {
      bytes_tail = 4096U - command_write_ptr;
      bytes_head = command_read_ptr;
    } else {
      bytes_tail = command_read_ptr - command_write_ptr;
      bytes_head = 0;
    }
  } while((bytes_tail + bytes_head) < len);

  /* Write as many bytes as possible following REG_CMD_WRITE */
  uint16_t bytes_to_write = min(len, bytes_tail);
  mem_write_bulk (RAM_CMD + command_write_ptr, T(ptr), bytes_to_write);
  command_write_ptr += bytes_to_write;
  ptr  += bytes_to_write;
  len  -= bytes_to_write;

  if(len > 0) {
    /* Write remaining bytes at start of circular buffer */
    mem_write_bulk (RAM_CMD, T(ptr), len);
    command_write_ptr = len;
  }

  if(command_write_ptr == 4096U) {
    command_write_ptr = 0;
  }
}

// Writes len bytes into the FIFO, if len is not
// divisible by four, zero bytes will be written
// to align to the boundary.

template <class T> void CLCD::CommandFifo::write(T data, uint16_t len) {
  const uint8_t padding = MULTIPLE_OF_4(len) - len;

  uint8_t pad_bytes[] = {0, 0, 0, 0};
  _write_unaligned(data,      len);
  _write_unaligned(pad_bytes, padding);
}
#else
uint32_t CLCD::CommandFifo::getRegCmdBSpace() {
  return mem_read_32(REG_CMDB_SPACE)  & 0x0FFF;
}

void CLCD::CommandFifo::start() {
}

void CLCD::CommandFifo::execute() {
}

void CLCD::CommandFifo::reset() {
  mem_write_32(REG_CMD_WRITE, 0x00000000);
  mem_write_32(REG_CMD_READ,  0x00000000);
};

// Writes len bytes into the FIFO, if len is not
// divisible by four, zero bytes will be written
// to align to the boundary.

template <class T> void CLCD::CommandFifo::write(T data, uint16_t len) {
  const uint8_t padding = MULTIPLE_OF_4(len) - len;

  // The FT810 provides a special register that can be used
  // for writing data without us having to do our own FIFO
  // management.
  uint32_t Command_Space = getRegCmdBSpace();
  while(Command_Space < len + padding) {
    Command_Space = getRegCmdBSpace();
  }
  mem_write_bulk(REG_CMDB_WRITE, data, len, padding);
}
#endif

// CO_PROCESSOR COMMANDS

// Writes a 32-bit (4 Bytes) Value to the Co-processor Command Buffer FIFO
void CLCD::CommandFifo::cmd (uint32_t cmd32) {
  write(&cmd32, sizeof(uint32_t));
}

// Writes a data structure - always a multiple of 32 bits - to the Co_Processor FIFO.
// Data structure includes the 32-bit Co_Processor command.
void CLCD::CommandFifo::cmd (void* data, uint16_t len) {
  write(data, len);
}

void CLCD::CommandFifo::str (const char * const data) {
  write(data, strlen(data)+1);
}

void CLCD::CommandFifo::str (progmem_str data) {
  write(data, strlen_P((const char*)data)+1);
}

#endif // FTDI_EVE_TOUCHSCREEN