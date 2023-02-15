/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 * Copyright (c) 2017 Victor Perez
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

/*
  HAL SPI implementation by Martin Turski, company owner of EirDev
  Inclusion date: 24th of November, 2022
  Contact mail: turningtides@outlook.de
  ---

  Contact Martin if there is any grave SPI design or functionality issue.
  Include a link to the Marlin FW GitHub issue post. Otherwise the mail
  may be ignored. This implementation has been created specifically for the
  Marlin FW. It was made with performance and simplicity-of-maintenance in
  mind, while keeping all the SPI requirements in check.

  Original pull request: https://github.com/MarlinFirmware/Marlin/pull/24911
*/

#ifdef ARDUINO_ARCH_ESP32

#include "../../inc/MarlinConfig.h"

#include "../shared/HAL_SPI.h"

#if !ENABLED(SOFTWARE_SPI) && !ENABLED(HALSPI_HW_GENERIC)

#include "sdk/BitManage.h"

template <typename hostNumberType, bool write_only = false>
using esp32BitManager = eir::BitManager <hostNumberType, eir::template choose_default_number_specificator <hostNumberType>::template spec, eir::typelist <uint32_t>>;

// ------------------------
// Hardware SPI
// tested using MKS TinyBee V1.0 (ESP32-WROOM-32U / ESP32-D0WD (rev1))
// ------------------------

/*
  The ESP32 SPI DMA hardware implementation problem.
  - written by Martin Turski on the 30th of January, 2023

  Reliability of a platform implementation comes first. Thus I have decided to cut away the optional SPI DMA
  implementation from the first generation of ESP32 chips. Continue reading so I can elaborate on why.

  Numerous people on the internet have tried to reach the Espressif support about custom SPI DMA implementations
  but failed.
  - https://www.esp32.com/viewtopic.php?t=16036 (https://web.archive.org/web/20230130174342/https://www.esp32.com/viewtopic.php?t=16036)
  - https://esp32.com/viewtopic.php?t=5152 (https://web.archive.org/web/20230130174608/https://esp32.com/viewtopic.php?t=5152)
  - https://esp32.com/viewtopic.php?t=10075 (https://web.archive.org/web/20230130174757/https://esp32.com/viewtopic.php?t=10075)
  The esp-idf so called "SPI master driver" does not, by design, support automatic MSBFIRST 16bit SPI frames. I want to highlight
  the ESP32 hardware "workaround" that points to reliability issues in the silicon.
  - (https://www.esp32.com/viewtopic.php?t=8433 spi_master.zip:spi_master.c:line 754)
  Support issues make people give up on the MCU altogether.
  - https://esp32.com/viewtopic.php?t=14732 (https://web.archive.org/web/20230130185951/https://esp32.com/viewtopic.php?t=14732)

  I would appreciate good help! I want to implement the ESP32 DMA SPI for the ESP32-D0WD, revision 1. The best way
  would be in the form of a sample project easily compilable & runnable on the MKS TinyBee V1.0. The example project
  must not use the SPI Master driver from esp-idf. Please contact me in the original pull request if you have found a solution.

  ESP32 SPI DMA machine-state dump at DMASendBlocking call related to the error: https://pastebin.com/LNBzJvRy

  Other SPI issues of ESP32 that seem fatal:
  - https://www.esp32.com/viewtopic.php?t=31389 (https://web.archive.org/web/20230130184649/https://www.esp32.com/viewtopic.php?t=31389)

  Please note that SPI with interrupts has been 100% tested and works just fine. Hence it is not a big deal to leave-out DMA acceleration.
*/
#ifndef HALSPI_DISABLE_DMA
  #define HALSPI_DISABLE_DMA
#endif

static void IRAM_ATTR _spi_on_error(const uint32_t code=0) {
  for (;;) {
    #if defined(HALSPI_DO_ERRORBEEPS) && PIN_EXISTS(BEEPER)
      OUT_WRITE(BEEPER_PIN, HIGH); delay(500);
      OUT_WRITE(BEEPER_PIN, LOW);  delay(500);
      OUT_WRITE(BEEPER_PIN, HIGH); delay(500);
      OUT_WRITE(BEEPER_PIN, LOW);  delay(500);
      OUT_WRITE(BEEPER_PIN, HIGH); delay(150);
      OUT_WRITE(BEEPER_PIN, LOW);  delay(150);
      OUT_WRITE(BEEPER_PIN, HIGH); delay(150);
      OUT_WRITE(BEEPER_PIN, LOW);  delay(150);
      OUT_WRITE(BEEPER_PIN, HIGH); delay(150);
      OUT_WRITE(BEEPER_PIN, LOW);  delay(code ? 800 : 150);
      for (uint32_t n = 0; n < code; n++) {
        OUT_WRITE(BEEPER_PIN, HIGH); delay(250);
        OUT_WRITE(BEEPER_PIN, LOW);
        if (n < code - 1) delay(250);
      }
      delay(3000);
    #endif
  }
}

#ifndef HALSPI_LOOPBEEP_TIMEOUT
#define HALSPI_LOOPBEEP_TIMEOUT 3000
#endif

struct spi_monitored_loop {
private:
  #if defined(HALSPI_DO_LOOPBEEPS) && PIN_EXISTS(BEEPER)
    uint32_t _start_millis;
  #endif
public:
  inline spi_monitored_loop() {
    #if defined(HALSPI_DO_LOOPBEEPS) && PIN_EXISTS(BEEPER)
      _start_millis = millis();
    #endif
  }
  inline void update(unsigned int beep_code) {
    #if defined(HALSPI_DO_LOOPBEEPS) && PIN_EXISTS(BEEPER)
      if ((millis() - _start_millis) <= HALSPI_LOOPBEEP_TIMEOUT) return;
      OUT_WRITE(BEEPER_PIN, HIGH); delay(500);
      OUT_WRITE(BEEPER_PIN, LOW);  delay(200);
      OUT_WRITE(BEEPER_PIN, HIGH); delay(200);
      OUT_WRITE(BEEPER_PIN, LOW);  delay(200);
      OUT_WRITE(BEEPER_PIN, HIGH); delay(200);
      OUT_WRITE(BEEPER_PIN, LOW);  delay(1000);
      for (unsigned int n = 0; n < beep_code; n++) {
        OUT_WRITE(BEEPER_PIN, HIGH); delay(200);
        OUT_WRITE(BEEPER_PIN, LOW);  delay(200);
      }
      delay(800);
      OUT_WRITE(BEEPER_PIN, HIGH); delay(1000);
      OUT_WRITE(BEEPER_PIN, LOW);  delay(2000);
    #endif
  }
};

static void __attribute__((unused)) _spi_infobeep(uint32_t code) {
#if PIN_EXISTS(BEEPER)
  OUT_WRITE(BEEPER_PIN, HIGH); delay(500);
  OUT_WRITE(BEEPER_PIN, LOW);  delay(500);
  for (uint32_t n = 0; n < code; n++) {
    OUT_WRITE(BEEPER_PIN, HIGH); delay(200);
    OUT_WRITE(BEEPER_PIN, LOW);  delay(200);
  }
  delay(300);
  OUT_WRITE(BEEPER_PIN, HIGH); delay(400);
  OUT_WRITE(BEEPER_PIN, LOW);  delay(1000);
#endif
}

// ESP32 TRM DATE: 14th of November, 2022

template <typename numberType>
inline numberType _MIN(numberType a, numberType b) {
  return ( a < b ? a : b );
}

namespace MarlinESP32 {

#define __ESP32_DEFREG(tn,n,l) static volatile tn& n = *(volatile tn*)l

template <typename T>
inline void dwrite(volatile T& v, const T& V) noexcept {
  if constexpr ( sizeof(T) == sizeof(uint8_t) ) {
    (volatile uint8_t&)v = (const uint8_t&)V;
  }
  else if constexpr ( sizeof(T) == sizeof(uint16_t) ) {
    (volatile uint16_t&)v = (const uint16_t&)V;
  }
  else if constexpr ( sizeof(T) == sizeof(uint32_t) ) {
    (volatile uint32_t&)v = (const uint32_t&)V;
  }
  else {
    v = V;
  }
}

struct spi_cmd_reg_t {
  uint32_t reserved1 : 18;
  uint32_t SPI_USR : 1;
  uint32_t reserved2 : 13;

  void serial_dump() volatile {
    SERIAL_ECHOLNPGM("SPI_CMD_REG.reserved1 = ", reserved1, ";");
    SERIAL_ECHOLNPGM("SPI_CMD_REG.SPI_USR = ", SPI_USR, ";");
    SERIAL_ECHOLNPGM("SPI_CMD_REG.reserved2 = ", reserved2, ";");
  }
};
static_assert(sizeof(spi_cmd_reg_t) == 4, "invalid size for ESP32 spi_cmd_reg_t");

#define _ESP32_BIT_ORDER_MSB 0
#define _ESP32_BIT_ORDER_LSB 1

struct spi_ctrl_reg_t {
  uint32_t reserved1 : 13;
  uint32_t SPI_FASTRD_MODE : 1;
  uint32_t SPI_FREAD_DUAL : 1;
  uint32_t reserved2 : 5;
  uint32_t SPI_FREAD_QUAD : 1;
  uint32_t SPI_WP : 1;
  uint32_t reserved3 : 1;
  uint32_t SPI_FREAD_DIO : 1;
  uint32_t SPI_FREAD_QIO : 1;
  uint32_t SPI_RD_BIT_ORDER : 1;
  uint32_t SPI_WR_BIT_ORDER : 1;
  uint32_t reserved4 : 5;

  void serial_dump() volatile {
    SERIAL_ECHOLNPGM("SPI_CTRL_REG.reserved1 = ", reserved1, ";");
    SERIAL_ECHOLNPGM("SPI_CTRL_REG.SPI_FASTRD_MODE = ", SPI_FASTRD_MODE, ";");
    SERIAL_ECHOLNPGM("SPI_CTRL_REG.SPI_FREAD_DUAL = ", SPI_FREAD_DUAL, ";");
    SERIAL_ECHOLNPGM("SPI_CTRL_REG.reserved2 = ", reserved2, ";");
    SERIAL_ECHOLNPGM("SPI_CTRL_REG.SPI_FREAD_QUAD = ", SPI_FREAD_QUAD, ";");
    SERIAL_ECHOLNPGM("SPI_CTRL_REG.SPI_WP = ", SPI_WP, ";");
    SERIAL_ECHOLNPGM("SPI_CTRL_REG.reserved3 = ", reserved3, ";");
    SERIAL_ECHOLNPGM("SPI_CTRL_REG.SPI_FREAD_DIO = ", SPI_FREAD_DIO, ";");
    SERIAL_ECHOLNPGM("SPI_CTRL_REG.SPI_FREAD_QIO = ", SPI_FREAD_QIO, ";");
    SERIAL_ECHOLNPGM("SPI_CTRL_REG.SPI_RD_BIT_ORDER = ", SPI_RD_BIT_ORDER, ";");
    SERIAL_ECHOLNPGM("SPI_CTRL_REG.SPI_WR_BIT_ORDER = ", SPI_WR_BIT_ORDER, ";");
    SERIAL_ECHOLNPGM("SPI_CTRL_REG.reserved4 = ", reserved4, ";");
  }
};
static_assert(sizeof(spi_ctrl_reg_t) == 4, "invalid size for ESP32 spi_ctrl_reg_t");

struct spi_ctrl1_reg_t {
  uint32_t reserved1 : 28;
  uint32_t SPI_CS_HOLD_DELAY : 4;

  void serial_dump() volatile {
    SERIAL_ECHOLNPGM("SPI_CTRL1_REG.reserved1 = ", reserved1, ";");
    SERIAL_ECHOLNPGM("SPI_CTRL1_REG.SPI_CS_HOLD_DELAY = ", SPI_CS_HOLD_DELAY, ";");
  }
};
static_assert(sizeof(spi_ctrl1_reg_t) == 4, "invalid size for ESP32 spi_ctrl1_reg_t");

struct spi_ctrl2_reg_t {
  uint32_t SPI_SETUP_TIME : 4;
  uint32_t SPI_HOLD_TIME : 4;
  uint32_t reserved1 : 4;
  uint32_t SPI_CK_OUT_HIGH_MODE : 4;
  uint32_t SPI_MISO_DELAY_MODE : 2;
  uint32_t SPI_MISO_DELAY_NUM : 3;
  uint32_t SPI_MOSI_DELAY_MODE : 2;
  uint32_t SPI_MOSI_DELAY_NUM : 3;
  uint32_t SPI_CS_DELAY_MODE : 2;
  uint32_t SPI_CS_DELAY_NUM : 4;

  void serial_dump() volatile {
    SERIAL_ECHOLNPGM("SPI_CTRL2_REG.SPI_SETUP_TIME = ", SPI_SETUP_TIME, ";");
    SERIAL_ECHOLNPGM("SPI_CTRL2_REG.SPI_HOLD_TIME = ", SPI_HOLD_TIME, ";");
    SERIAL_ECHOLNPGM("SPI_CTRL2_REG.reserved1 = ", reserved1, ";");
    SERIAL_ECHOLNPGM("SPI_CTRL2_REG.SPI_CK_OUT_HIGH_MODE = ", SPI_CK_OUT_HIGH_MODE, ";");
    SERIAL_ECHOLNPGM("SPI_CTRL2_REG.SPI_MISO_DELAY_MODE = ", SPI_MISO_DELAY_MODE, ";");
    SERIAL_ECHOLNPGM("SPI_CTRL2_REG.SPI_MISO_DELAY_NUM = ", SPI_MISO_DELAY_NUM, ";");
    SERIAL_ECHOLNPGM("SPI_CTRL2_REG.SPI_MOSI_DELAY_MODE = ", SPI_MOSI_DELAY_MODE, ";");
    SERIAL_ECHOLNPGM("SPI_CTRL2_REG.SPI_MOSI_DELAY_NUM = ", SPI_MOSI_DELAY_NUM, ";");
    SERIAL_ECHOLNPGM("SPI_CTRL2_REG.SPI_CS_DELAY_MODE = ", SPI_CS_DELAY_MODE, ";");
    SERIAL_ECHOLNPGM("SPI_CTRL2_REG.SPI_CS_DELAY_NUM = ", SPI_CS_DELAY_NUM, ";");
  }
};
static_assert(sizeof(spi_ctrl2_reg_t) == 4, "invalid size for ESP32 spi_ctrl2_reg_t");

struct spi_clock_reg_t {
  uint32_t SPI_CLKCNT_L : 6;
  uint32_t SPI_CLKCNT_H : 6;
  uint32_t SPI_CLKCNT_N : 6;
  uint32_t SPI_CLKDIV_PRE : 13;
  uint32_t SPI_CLK_EQU_SYSCLK : 1;

  void serial_dump() volatile {
    SERIAL_ECHOLNPGM("SPI_CLOCK_REG.SPI_CLKCNT_L = ", SPI_CLKCNT_L, ";");
    SERIAL_ECHOLNPGM("SPI_CLOCK_REG.SPI_CLKCNT_H = ", SPI_CLKCNT_H, ";");
    SERIAL_ECHOLNPGM("SPI_CLOCK_REG.SPI_CLKCNT_N = ", SPI_CLKCNT_N, ";");
    SERIAL_ECHOLNPGM("SPI_CLOCK_REG.SPI_CLKDIV_PRE = ", SPI_CLKDIV_PRE, ";");
    SERIAL_ECHOLNPGM("SPI_CLOCK_REG.SPI_CLK_EQU_SYSCLK = ", SPI_CLK_EQU_SYSCLK, ";");
  }
};
static_assert(sizeof(spi_clock_reg_t) == 4, "invalid size for ESP32 spi_clock_reg_t");

struct spi_user_reg_t {
  uint32_t SPI_DOUTDIN : 1;
  uint32_t reserved1 : 3;
  uint32_t SPI_CS_HOLD : 1;
  uint32_t SPI_CS_SETUP : 1;
  uint32_t SPI_CK_I_EDGE : 1;
  uint32_t SPI_CK_OUT_EDGE : 1;
  uint32_t reserved2 : 2;
  uint32_t SPI_RD_BYTE_ORDER : 1;
  uint32_t SPI_WR_BYTE_ORDER : 1;
  uint32_t SPI_FWRITE_DUAL : 1;
  uint32_t SPI_FWRITE_QUAD : 1;
  uint32_t SPI_FWRITE_DIO : 1;
  uint32_t SPI_FWRITE_QIO : 1;
  uint32_t SPI_SIO : 1;
  uint32_t reserved3 : 7;
  uint32_t SPI_USR_MISO_HIGHPART : 1;
  uint32_t SPI_USR_MOSI_HIGHPART : 1;
  uint32_t SPI_USR_DUMMY_IDLE : 1;
  uint32_t SPI_USR_MOSI : 1;
  uint32_t SPI_USR_MISO : 1;
  uint32_t SPI_USR_DUMMY : 1;
  uint32_t SPI_USR_ADDR : 1;
  uint32_t SPI_USR_COMMAND : 1;

  void serial_dump() volatile {
    SERIAL_ECHOLNPGM("SPI_USER_REG.SPI_DOUTDIN = ", SPI_DOUTDIN, ";");
    SERIAL_ECHOLNPGM("SPI_USER_REG.reserved1 = ", reserved1, ";");
    SERIAL_ECHOLNPGM("SPI_USER_REG.SPI_CS_HOLD = ", SPI_CS_HOLD, ";");
    SERIAL_ECHOLNPGM("SPI_USER_REG.SPI_CS_SETUP = ", SPI_CS_SETUP, ";");
    SERIAL_ECHOLNPGM("SPI_USER_REG.SPI_CK_I_EDGE = ", SPI_CK_I_EDGE, ";");
    SERIAL_ECHOLNPGM("SPI_USER_REG.SPI_CK_OUT_EDGE = ", SPI_CK_OUT_EDGE, ";");
    SERIAL_ECHOLNPGM("SPI_USER_REG.reserved2 = ", reserved2, ";");
    SERIAL_ECHOLNPGM("SPI_USER_REG.SPI_RD_BYTE_ORDER = ", SPI_RD_BYTE_ORDER, ";");
    SERIAL_ECHOLNPGM("SPI_USER_REG.SPI_WR_BYTE_ORDER = ", SPI_WR_BYTE_ORDER, ";");
    SERIAL_ECHOLNPGM("SPI_USER_REG.SPI_FWRITE_DUAL = ", SPI_FWRITE_DUAL, ";");
    SERIAL_ECHOLNPGM("SPI_USER_REG.SPI_FWRITE_QUAD = ", SPI_FWRITE_QUAD, ";");
    SERIAL_ECHOLNPGM("SPI_USER_REG.SPI_FWRITE_DIO = ", SPI_FWRITE_DIO, ";");
    SERIAL_ECHOLNPGM("SPI_USER_REG.SPI_FWRITE_QIO = ", SPI_FWRITE_QIO, ";");
    SERIAL_ECHOLNPGM("SPI_USER_REG.SPI_SIO = ", SPI_SIO, ";");
    SERIAL_ECHOLNPGM("SPI_USER_REG.reserved3 = ", reserved3, ";");
    SERIAL_ECHOLNPGM("SPI_USER_REG.SPI_USR_MISO_HIGHPART = ", SPI_USR_MISO_HIGHPART, ";");
    SERIAL_ECHOLNPGM("SPI_USER_REG.SPI_USR_MOSI_HIGHPART = ", SPI_USR_MOSI_HIGHPART, ";");
    SERIAL_ECHOLNPGM("SPI_USER_REG.SPI_USR_DUMMY_IDLE = ", SPI_USR_DUMMY_IDLE, ";");
    SERIAL_ECHOLNPGM("SPI_USER_REG.SPI_USR_MOSI = ", SPI_USR_MOSI, ";");
    SERIAL_ECHOLNPGM("SPI_USER_REG.SPI_USR_MISO = ", SPI_USR_MISO, ";");
    SERIAL_ECHOLNPGM("SPI_USER_REG.SPI_USR_DUMMY = ", SPI_USR_DUMMY, ";");
    SERIAL_ECHOLNPGM("SPI_USER_REG.SPI_USR_ADDR = ", SPI_USR_ADDR, ";");
    SERIAL_ECHOLNPGM("SPI_USER_REG.SPI_USR_COMMAND = ", SPI_USR_COMMAND, ";");
  }
};
static_assert(sizeof(spi_user_reg_t) == 4, "invalid size for ESP32 spi_user_reg_t");

struct spi_user1_reg_t {
  uint32_t SPI_USR_DUMMY_CYCLELEN : 8;
  uint32_t reserved1 : 18;
  uint32_t SPI_USR_ADDR_BITLEN : 6;

  void serial_dump() volatile {
    SERIAL_ECHOLNPGM("SPI_USER1_REG.SPI_USR_DUMMY_CYCLELEN = ", SPI_USR_DUMMY_CYCLELEN, ";");
    SERIAL_ECHOLNPGM("SPI_USER1_REG.reserved1 = ", reserved1, ";");
    SERIAL_ECHOLNPGM("SPI_USER1_REG.SPI_USR_ADDR_BITLEN = ", SPI_USR_ADDR_BITLEN, ";");
  }
};
static_assert(sizeof(spi_user1_reg_t) == 4, "invalid size for ESP32 spi_user1_reg_t");

struct spi_user2_reg_t {
  uint32_t SPI_USR_COMMAND_VALUE : 16;
  uint32_t reserved1 : 12;
  uint32_t SPI_USR_COMMAND_BITLEN : 4;

  void serial_dump() volatile {
    SERIAL_ECHOLNPGM("SPI_USER2_REG.SPI_USR_COMMAND_VALUE = ", SPI_USR_COMMAND_VALUE, ";");
    SERIAL_ECHOLNPGM("SPI_USER2_REG.reserved1 = ", reserved1, ";");
    SERIAL_ECHOLNPGM("SPI_USER2_REG.SPI_USR_COMMAND_BITLEN = ", SPI_USR_COMMAND_BITLEN, ";");
  }
};
static_assert(sizeof(spi_user2_reg_t) == 4, "invalid size for ESP32 spi_user2_reg_t");

struct spi_mosi_dlen_reg_t {
  uint32_t SPI_USR_MOSI_DBITLEN : 24;
  uint32_t reserved1 : 8;

  void serial_dump() volatile {
    SERIAL_ECHOLNPGM("SPI_MOSI_DLEN_REG.SPI_USR_MOSI_DBITLEN = ", SPI_USR_MOSI_DBITLEN, ";");
    SERIAL_ECHOLNPGM("SPI_MOSI_DLEN_REG.reserved1 = ", reserved1, ";");
  }
};
static_assert(sizeof(spi_mosi_dlen_reg_t) == 4, "invalid size for ESP32 spi_mosi_dlen_reg_t");

struct spi_miso_dlen_reg_t {
  uint32_t SPI_USR_MISO_DBITLEN : 24;
  uint32_t reserved1 : 8;

  void serial_dump() volatile {
    SERIAL_ECHOLNPGM("SPI_MISO_DLEN_REG.SPI_USR_MISO_DBITLEN = ", SPI_USR_MISO_DBITLEN, ";");
    SERIAL_ECHOLNPGM("SPI_MISO_DLEN_REG.reserved1 = ", reserved1, ";");
  }
};
static_assert(sizeof(spi_miso_dlen_reg_t) == 4, "invalid size for ESP32 spi_miso_dlen_reg_t");

struct spi_pin_reg_t {
  uint32_t SPI_CS0_DIS : 1;
  uint32_t SPI_CS1_DIS : 1;
  uint32_t SPI_CS2_DIS : 1;
  uint32_t reserved1 : 2;
  uint32_t SPI_CK_DIS : 1;
  uint32_t SPI_MASTER_CS_POL : 3;
  uint32_t reserved2 : 2;
  uint32_t SPI_MASTER_CK_SEL : 3;
  uint32_t reserved3 : 15;
  uint32_t SPI_CK_IDLE_EDGE : 1;
  uint32_t SPI_CS_KEEP_ACTIVE : 1;
  uint32_t reserved4 : 1;

  void serial_dump() volatile {
    SERIAL_ECHOLNPGM("SPI_PIN_REG.SPI_CS0_DIS = ", SPI_CS0_DIS, ";");
    SERIAL_ECHOLNPGM("SPI_PIN_REG.SPI_CS1_DIS = ", SPI_CS1_DIS, ";");
    SERIAL_ECHOLNPGM("SPI_PIN_REG.SPI_CS2_DIS = ", SPI_CS2_DIS, ";");
    SERIAL_ECHOLNPGM("SPI_PIN_REG.reserved1 = ", reserved1, ";");
    SERIAL_ECHOLNPGM("SPI_PIN_REG.SPI_CK_DIS = ", SPI_CK_DIS, ";");
    SERIAL_ECHOLNPGM("SPI_PIN_REG.SPI_MASTER_CS_POL = ", SPI_MASTER_CS_POL, ";");
    SERIAL_ECHOLNPGM("SPI_PIN_REG.reserved2 = ", reserved2, ";");
    SERIAL_ECHOLNPGM("SPI_PIN_REG.SPI_MASTER_CK_SEL = ", SPI_MASTER_CK_SEL, ";");
    SERIAL_ECHOLNPGM("SPI_PIN_REG.reserved3 = ", reserved3, ";");
    SERIAL_ECHOLNPGM("SPI_PIN_REG.SPI_CK_IDLE_EDGE = ", SPI_CK_IDLE_EDGE, ";");
    SERIAL_ECHOLNPGM("SPI_PIN_REG.SPI_CS_KEEP_ACTIVE = ", SPI_CS_KEEP_ACTIVE, ";");
    SERIAL_ECHOLNPGM("SPI_PIN_REG.reserved4 = ", reserved4, ";");
  }
};
static_assert(sizeof(spi_pin_reg_t) == 4, "invalid size for ESP32 spi_pin_reg_t");

struct spi_slave_reg_t {
  uint32_t SPI_SLV_RD_BUF_DONE : 1;
  uint32_t SPI_SLV_WR_BUF_DONE : 1;
  uint32_t SPI_SLV_RD_STA_DONE : 1;
  uint32_t SPI_SLV_WR_STA_DONE : 1;
  uint32_t SPI_TRANS_DONE : 1;
  uint32_t SPI_SLV_RD_BUF_INTEN : 1;
  uint32_t SPI_SLV_WR_BUF_INTEN : 1;
  uint32_t SPI_SLV_RD_STA_INTEN : 1;
  uint32_t SPI_SLV_WR_STA_INTEN : 1;
  uint32_t SPI_TRANS_INTEN : 1;
  uint32_t SPI_CS_I_MODE : 2;
  uint32_t reserved1 : 5;
  uint32_t SPI_SLV_LAST_COMMAND : 3;
  uint32_t SPI_SLV_LAST_STATE : 3;    // read-only
  uint32_t SPI_TRANS_CNT : 4;         // read-only
  uint32_t SPI_SLV_CMD_DEFINE : 1;
  uint32_t SPI_SLV_WR_RD_STA_EN : 1;
  uint32_t SPI_SLV_WR_RD_BUF_EN : 1;
  uint32_t SPI_SLAVE_MODE : 1;
  uint32_t SPI_SYNC_RESET : 1;

  void serial_dump() volatile {
    SERIAL_ECHOLNPGM("SPI_SLAVE_REG.SPI_SLV_RD_BUF_DONE = ", SPI_SLV_RD_BUF_DONE, ";");
    SERIAL_ECHOLNPGM("SPI_SLAVE_REG.SPI_SLV_WR_BUF_DONE = ", SPI_SLV_WR_BUF_DONE, ";");
    SERIAL_ECHOLNPGM("SPI_SLAVE_REG.SPI_SLV_RD_STA_DONE = ", SPI_SLV_RD_STA_DONE, ";");
    SERIAL_ECHOLNPGM("SPI_SLAVE_REG.SPI_SLV_WR_STA_DONE = ", SPI_SLV_WR_STA_DONE, ";");
    SERIAL_ECHOLNPGM("SPI_SLAVE_REG.SPI_TRANS_DONE = ", SPI_TRANS_DONE, ";");
    SERIAL_ECHOLNPGM("SPI_SLAVE_REG.SPI_SLV_RD_BUF_INTEN = ", SPI_SLV_RD_BUF_INTEN, ";");
    SERIAL_ECHOLNPGM("SPI_SLAVE_REG.SPI_SLV_WR_BUF_INTEN = ", SPI_SLV_WR_BUF_INTEN, ";");
    SERIAL_ECHOLNPGM("SPI_SLAVE_REG.SPI_SLV_RD_STA_INTEN = ", SPI_SLV_RD_STA_INTEN, ";");
    SERIAL_ECHOLNPGM("SPI_SLAVE_REG.SPI_SLV_WR_STA_INTEN = ", SPI_SLV_WR_STA_INTEN, ";");
    SERIAL_ECHOLNPGM("SPI_SLAVE_REG.SPI_TRANS_INTEN = ", SPI_TRANS_INTEN, ";");
    SERIAL_ECHOLNPGM("SPI_SLAVE_REG.SPI_CS_I_MODE = ", SPI_CS_I_MODE, ";");
    SERIAL_ECHOLNPGM("SPI_SLAVE_REG.reserved1 = ", reserved1, ";");
    SERIAL_ECHOLNPGM("SPI_SLAVE_REG.SPI_SLV_LAST_COMMAND = ", SPI_SLV_LAST_COMMAND, ";");
    SERIAL_ECHOLNPGM("SPI_SLAVE_REG.SPI_SLV_LAST_STATE = ", SPI_SLV_LAST_STATE, ";");
    SERIAL_ECHOLNPGM("SPI_SLAVE_REG.SPI_TRANS_CNT = ", SPI_TRANS_CNT, ";");
    SERIAL_ECHOLNPGM("SPI_SLAVE_REG.SPI_SLV_CMD_DEFINE = ", SPI_SLV_CMD_DEFINE, ";");
    SERIAL_ECHOLNPGM("SPI_SLAVE_REG.SPI_SLV_WR_RD_STA_EN = ", SPI_SLV_WR_RD_STA_EN, ";");
    SERIAL_ECHOLNPGM("SPI_SLAVE_REG.SPI_SLV_WR_RD_BUF_EN = ", SPI_SLV_WR_RD_BUF_EN, ";");
    SERIAL_ECHOLNPGM("SPI_SLAVE_REG.SPI_SLAVE_MODE = ", SPI_SLAVE_MODE, ";");
    SERIAL_ECHOLNPGM("SPI_SLAVE_REG.SPI_SYNC_RESET = ", SPI_SYNC_RESET, ";");
  }
};
static_assert(sizeof(spi_slave_reg_t) == 4, "invalid size for ESP32 spi_slave_reg_t");

struct spi_slave1_reg_t {
  uint32_t SPI_SLV_RDBUF_DUMMY_EN : 1;
  uint32_t SPI_SLV_WRBUF_DUMMY_EN : 1;
  uint32_t SPI_SLV_RDSTA_DUMMY_EN : 1;
  uint32_t SPI_SLV_WRSTA_DUMMY_EN : 1;
  uint32_t SPI_SLV_WR_ADDR_BITLEN : 6;
  uint32_t SPI_SLV_RD_ADDR_BITLEN : 6;
  uint32_t reserved1 : 9;
  uint32_t SPI_SLV_STATUS_READBACK : 1;
  uint32_t SPI_SLV_STATUS_FAST_EN : 1;
  uint32_t SPI_SLV_STATUS_BITLEN : 5;

  void serial_dump() volatile {
    SERIAL_ECHOLNPGM("SPI_SLAVE1_REG.SPI_SLV_RDBUF_DUMMY_EN = ", SPI_SLV_RDBUF_DUMMY_EN, ";");
    SERIAL_ECHOLNPGM("SPI_SLAVE1_REG.SPI_SLV_WRBUF_DUMMY_EN = ", SPI_SLV_WRBUF_DUMMY_EN, ";");
    SERIAL_ECHOLNPGM("SPI_SLAVE1_REG.SPI_SLV_RDSTA_DUMMY_EN = ", SPI_SLV_RDSTA_DUMMY_EN, ";");
    SERIAL_ECHOLNPGM("SPI_SLAVE1_REG.SPI_SLV_WRSTA_DUMMY_EN = ", SPI_SLV_WRSTA_DUMMY_EN, ";");
    SERIAL_ECHOLNPGM("SPI_SLAVE1_REG.SPI_SLV_WR_ADDR_BITLEN = ", SPI_SLV_WR_ADDR_BITLEN, ";");
    SERIAL_ECHOLNPGM("SPI_SLAVE1_REG.SPI_SLV_RD_ADDR_BITLEN = ", SPI_SLV_RD_ADDR_BITLEN, ";");
    SERIAL_ECHOLNPGM("SPI_SLAVE1_REG.reserved1 = ", reserved1, ";");
    SERIAL_ECHOLNPGM("SPI_SLAVE1_REG.SPI_SLV_STATUS_READBACK = ", SPI_SLV_STATUS_READBACK, ";");
    SERIAL_ECHOLNPGM("SPI_SLAVE1_REG.SPI_SLV_STATUS_FAST_EN = ", SPI_SLV_STATUS_FAST_EN, ";");
    SERIAL_ECHOLNPGM("SPI_SLAVE1_REG.SPI_SLV_STATUS_BITLEN = ", SPI_SLV_STATUS_BITLEN, ";");
  }
};
static_assert(sizeof(spi_slave1_reg_t) == 4, "invalid size for ESP32 spi_slave1_reg_t");

struct spi_slave2_reg_t {
  uint32_t SPI_SLV_RDSTA_DUMMY_CYCLELEN : 8;
  uint32_t SPI_SLV_WRSTA_DUMMY_CYCLELEN : 8;
  uint32_t SPI_SLV_RDBUF_DUMMY_CYCLELEN : 8;
  uint32_t SPI_SLV_WRBUF_DUMMY_CYCLELEN : 8;

  void serial_dump() volatile {
    SERIAL_ECHOLNPGM("SPI_SLAVE2_REG.SPI_SLV_RDSTA_DUMMY_CYCLELEN = ", SPI_SLV_RDSTA_DUMMY_CYCLELEN, ";");
    SERIAL_ECHOLNPGM("SPI_SLAVE2_REG.SPI_SLV_WRSTA_DUMMY_CYCLELEN = ", SPI_SLV_WRSTA_DUMMY_CYCLELEN, ";");
    SERIAL_ECHOLNPGM("SPI_SLAVE2_REG.SPI_SLV_RDBUF_DUMMY_CYCLELEN = ", SPI_SLV_RDBUF_DUMMY_CYCLELEN, ";");
    SERIAL_ECHOLNPGM("SPI_SLAVE2_REG.SPI_SLV_WRBUF_DUMMY_CYCLELEN = ", SPI_SLV_WRBUF_DUMMY_CYCLELEN, ";");
  }
};
static_assert(sizeof(spi_slave2_reg_t) == 4, "invalid size for ESP32 spi_slave2_reg_t");

struct spi_slave3_reg_t {
  uint32_t SPI_SLV_RDBUF_CMD_VALUE : 8;
  uint32_t SPI_SLV_WRBUF_CMD_VALUE : 8;
  uint32_t SPI_SLV_RDSTA_CMD_VALUE : 8;
  uint32_t SPI_SLV_WRSTA_CMD_VALUE : 8;

  void serial_dump() volatile {
    SERIAL_ECHOLNPGM("SPI_SLAVE3_REG.SPI_SLV_RDBUF_CMD_VALUE = ", SPI_SLV_RDBUF_CMD_VALUE, ";");
    SERIAL_ECHOLNPGM("SPI_SLAVE3_REG.SPI_SLV_WRBUF_CMD_VALUE = ", SPI_SLV_WRBUF_CMD_VALUE, ";");
    SERIAL_ECHOLNPGM("SPI_SLAVE3_REG.SPI_SLV_RDSTA_CMD_VALUE = ", SPI_SLV_RDSTA_CMD_VALUE, ";");
    SERIAL_ECHOLNPGM("SPI_SLAVE3_REG.SPI_SLV_WRSTA_CMD_VALUE = ", SPI_SLV_WRSTA_CMD_VALUE, ";");
  }
};
static_assert(sizeof(spi_slave3_reg_t) == 4, "invalid size for ESP32 spi_slave3_reg_t");

struct spi_slv_wrbuf_dlen_reg_t {
  uint32_t SPI_SLV_WRBUF_DBITLEN : 24;
  uint32_t reserved1 : 8;

  void serial_dump() volatile {
    SERIAL_ECHOLNPGM("SPI_SLV_WRBUF_DLEN_REG.SPI_SLV_WRBUF_DBITLEN = ", SPI_SLV_WRBUF_DBITLEN, ";");
    SERIAL_ECHOLNPGM("SPI_SLV_WRBUF_DLEN_REG.reserved1 = ", reserved1, ";");
  }
};
static_assert(sizeof(spi_slv_wrbuf_dlen_reg_t) == 4, "invalid size for ESP32 spi_slv_wrbuf_dlen_reg_t");

struct spi_slv_rdbuf_dlen_reg_t {
  uint32_t SPI_SLV_RDBUF_DBITLEN : 24;
  uint32_t reserved1 : 8;

  void serial_dump() volatile {
    SERIAL_ECHOLNPGM("SPI_SLV_RDBUF_DLEN_REG.SPI_SLV_RDBUF_DBITLEN = ", SPI_SLV_RDBUF_DBITLEN, ";");
    SERIAL_ECHOLNPGM("SPI_SLV_RDBUF_DLEN_REG.reserved1 = ", reserved1, ";");
  }
};
static_assert(sizeof(spi_slv_rdbuf_dlen_reg_t) == 4, "invalid size for ESP32 spi_slv_rdbuf_dlen_reg_t");

struct spi_slv_rd_bit_reg_t {
  uint32_t SPI_SLV_RDATA_BIT : 24;
  uint32_t reserved1 : 8;

  void serial_dump() volatile {
    SERIAL_ECHOLNPGM("SPI_SLV_RD_BIT_REG.SPI_SLV_RDATA_BIT = ", SPI_SLV_RDATA_BIT, ";");
    SERIAL_ECHOLNPGM("SPI_SLV_RD_BIT_REG.reserved1 = ", reserved1, ";");
  }
};
static_assert(sizeof(spi_slv_rd_bit_reg_t) == 4, "invalid size for ESP32 spi_slv_rd_bit_reg_t");

struct spi_ext2_reg_t {
  uint32_t SPI_ST : 3;  // read-only
  uint32_t reserved1 : 29;

  void serial_dump() volatile {
    SERIAL_ECHOLNPGM("SPI_EXT2_REG.SPI_ST = ", SPI_ST, ";");
    SERIAL_ECHOLNPGM("SPI_EXT2_REG.reserved1 = ", reserved1, ";");
  }
};
static_assert(sizeof(spi_ext2_reg_t) == 4, "invalid size for ESP32 spi_ext2_reg_t");

struct spi_dma_conf_reg_t {
  uint32_t reserved1 : 2;
  uint32_t SPI_IN_RST : 1;
  uint32_t SPI_OUT_RST : 1;
  uint32_t SPI_AHBM_FIFO_RST : 1;
  uint32_t SPI_AHBM_RST : 1;
  uint32_t reserved2 : 3;
  uint32_t SPI_OUT_EOF_MODE : 1;
  uint32_t SPI_OUTDSCR_BURST_EN : 1;
  uint32_t SPI_INDSCR_BURST_EN : 1;
  uint32_t SPI_OUT_DATA_BURST_EN : 1;
  uint32_t reserved3 : 1;
  uint32_t SPI_DMA_RX_STOP : 1;
  uint32_t SPI_DMA_TX_STOP : 1;
  uint32_t SPI_DMA_CONTINUE : 1;
  uint32_t reserved4 : 15;

  void serial_dump() volatile {
    SERIAL_ECHOLNPGM("SPI_DMA_CONF_REG.reserved1 = ", reserved1, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_CONF_REG.SPI_IN_RST = ", SPI_IN_RST, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_CONF_REG.SPI_OUT_RST = ", SPI_OUT_RST, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_CONF_REG.SPI_AHBM_FIFO_RST = ", SPI_AHBM_FIFO_RST, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_CONF_REG.SPI_AHBM_RST = ", SPI_AHBM_RST, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_CONF_REG.reserved2 = ", reserved2, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_CONF_REG.SPI_OUT_EOF_MODE = ", SPI_OUT_EOF_MODE, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_CONF_REG.SPI_OUTDSCR_BURST_EN = ", SPI_OUTDSCR_BURST_EN, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_CONF_REG.SPI_INDSCR_BURST_EN = ", SPI_INDSCR_BURST_EN, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_CONF_REG.SPI_OUT_DATA_BURST_EN = ", SPI_OUT_DATA_BURST_EN, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_CONF_REG.reserved3 = ", reserved3, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_CONF_REG.SPI_DMA_RX_STOP = ", SPI_DMA_RX_STOP, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_CONF_REG.SPI_DMA_TX_STOP = ", SPI_DMA_TX_STOP, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_CONF_REG.SPI_DMA_CONTINUE = ", SPI_DMA_CONTINUE, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_CONF_REG.reserved4 = ", reserved4, ";");
  }
};
static_assert(sizeof(spi_dma_conf_reg_t) == 4, "invalid size for ESP32 spi_dma_conf_reg_t");

struct spi_dma_out_link_reg_t {
  uint32_t SPI_OUTLINK_ADDR : 20;    // ESP32 base address: 0x3FF00000
  uint32_t reserved1 : 8;
  uint32_t SPI_OUTLINK_STOP : 1;
  uint32_t SPI_OUTLINK_START : 1;
  uint32_t SPI_OUTLINK_RESTART : 1;
  uint32_t reserved2 : 1;

  void serial_dump() volatile {
    SERIAL_ECHOLNPGM("SPI_DMA_OUT_LINK_REG.SPI_OUTLINK_ADDR = ", SPI_OUTLINK_ADDR, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_OUT_LINK_REG.reserved1 = ", reserved1, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_OUT_LINK_REG.SPI_OUTLINK_STOP = ", SPI_OUTLINK_STOP, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_OUT_LINK_REG.SPI_OUTLINK_START = ", SPI_OUTLINK_START, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_OUT_LINK_REG.SPI_OUTLINK_RESTART = ", SPI_OUTLINK_RESTART, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_OUT_LINK_REG.reserved2 = ", reserved2, ";");
  }
};
static_assert(sizeof(spi_dma_out_link_reg_t) == 4, "invalid size for ESP32 spi_dma_out_link_reg_t");

struct spi_dma_in_link_reg_t {
  uint32_t SPI_INLINK_ADDR : 20;    // ESP32 base address: 0x3FF00000
  uint32_t SPI_INLINK_AUTO_RET : 1;
  uint32_t reserved1 : 7;
  uint32_t SPI_INLINK_STOP : 1;
  uint32_t SPI_INLINK_START : 1;
  uint32_t SPI_INLINK_RESTART : 1;
  uint32_t reserved2 : 1;

  void serial_dump() volatile {
    SERIAL_ECHOLNPGM("SPI_DMA_IN_LINK_REG.SPI_INLINK_ADDR = ", SPI_INLINK_ADDR, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_IN_LINK_REG.SPI_INLINK_AUTO_RET = ", SPI_INLINK_AUTO_RET, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_IN_LINK_REG.reserved1 = ", reserved1, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_IN_LINK_REG.SPI_INLINK_STOP = ", SPI_INLINK_STOP, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_IN_LINK_REG.SPI_INLINK_START = ", SPI_INLINK_START, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_IN_LINK_REG.SPI_INLINK_RESTART = ", SPI_INLINK_RESTART, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_IN_LINK_REG.reserved2 = ", reserved2, ";");
  }
};
static_assert(sizeof(spi_dma_in_link_reg_t) == 4, "invalid size for ESP32 spi_dma_in_link_reg_t");

struct spi_dma_status_reg_t {
  uint32_t SPI_DMA_TX_EN : 1; // read-only
  uint32_t SPI_DMA_RX_EN : 1; // read-only
  uint32_t reserved1 : 30;

  void serial_dump() volatile {
    SERIAL_ECHOLNPGM("SPI_DMA_STATUS_REG.SPI_DMA_TX_EN = ", SPI_DMA_TX_EN, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_STATUS_REG.SPI_DMA_RX_EN = ", SPI_DMA_RX_EN, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_STATUS_REG.reserved1 = ", reserved1, ";");
  }
};
static_assert(sizeof(spi_dma_status_reg_t) == 4, "invalid size for ESP32 spi_dma_status_reg_t");

struct spi_dma_int_ena_reg_t {
  uint32_t SPI_INLINK_DSCR_EMPTY_INT_ENA : 1;
  uint32_t SPI_OUTLINK_DSCR_ERROR_INT_ENA : 1;
  uint32_t SPI_INLINK_DSCR_ERROR_INT_ENA : 1;
  uint32_t SPI_IN_DONE_INT_ENA : 1;
  uint32_t SPI_IN_ERR_EOF_INT_ENA : 1;
  uint32_t SPI_IN_SUC_EOF_INT_ENA : 1;
  uint32_t SPI_OUT_DONE_INT_ENA : 1;
  uint32_t SPI_OUT_EOF_INT_ENA : 1;
  uint32_t SPI_OUT_TOTAL_EOF_INT_ENA : 1;
  uint32_t reserved1 : 23;

  void serial_dump() volatile {
    SERIAL_ECHOLNPGM("SPI_DMA_INT_ENA_REG.SPI_INLINK_DSCR_EMPTY_INT_ENA = ", SPI_INLINK_DSCR_EMPTY_INT_ENA, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_INT_ENA_REG.SPI_OUTLINK_DSCR_ERROR_INT_ENA = ", SPI_OUTLINK_DSCR_ERROR_INT_ENA, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_INT_ENA_REG.SPI_INLINK_DSCR_ERROR_INT_ENA = ", SPI_INLINK_DSCR_ERROR_INT_ENA, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_INT_ENA_REG.SPI_IN_DONE_INT_ENA = ", SPI_IN_DONE_INT_ENA, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_INT_ENA_REG.SPI_IN_ERR_EOF_INT_ENA = ", SPI_IN_ERR_EOF_INT_ENA, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_INT_ENA_REG.SPI_IN_SUC_EOF_INT_ENA = ", SPI_IN_SUC_EOF_INT_ENA, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_INT_ENA_REG.SPI_OUT_DONE_INT_ENA = ", SPI_OUT_DONE_INT_ENA, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_INT_ENA_REG.SPI_OUT_EOF_INT_ENA = ", SPI_OUT_EOF_INT_ENA, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_INT_ENA_REG.SPI_OUT_TOTAL_EOF_INT_ENA = ", SPI_OUT_TOTAL_EOF_INT_ENA, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_INT_ENA_REG.reserved1 = ", reserved1, ";");
  }
};
static_assert(sizeof(spi_dma_int_ena_reg_t) == 4, "invalid size for ESP32 spi_dma_int_ena_reg_t");

struct spi_dma_int_raw_reg_t {
  uint32_t SPI_INLINK_DSCR_EMPTY_INT_RAW : 1;   // read-only
  uint32_t SPI_OUTLINK_DSCR_ERROR_INT_RAW : 1;  // read-only
  uint32_t SPI_INLINK_DSCR_ERROR_INT_RAW : 1;   // read-only
  uint32_t SPI_IN_DONE_INT_RAW : 1;             // read-only
  uint32_t SPI_IN_ERR_EOF_INT_RAW : 1;          // read-only
  uint32_t SPI_IN_SUC_EOF_INT_RAW : 1;          // read-only
  uint32_t SPI_OUT_DONE_INT_RAW : 1;            // read-only
  uint32_t SPI_OUT_EOF_INT_RAW : 1;             // read-only
  uint32_t SPI_OUT_TOTAL_EOF_INT_RAW : 1;       // read-only
  uint32_t reserved1 : 23;

  void serial_dump() volatile {
    SERIAL_ECHOLNPGM("SPI_DMA_INT_RAW_REG.SPI_INLINK_DSCR_EMPTY_INT_RAW = ", SPI_INLINK_DSCR_EMPTY_INT_RAW, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_INT_RAW_REG.SPI_OUTLINK_DSCR_ERROR_INT_RAW = ", SPI_OUTLINK_DSCR_ERROR_INT_RAW, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_INT_RAW_REG.SPI_INLINK_DSCR_ERROR_INT_RAW = ", SPI_INLINK_DSCR_ERROR_INT_RAW, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_INT_RAW_REG.SPI_IN_DONE_INT_RAW = ", SPI_IN_DONE_INT_RAW, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_INT_RAW_REG.SPI_IN_ERR_EOF_INT_RAW = ", SPI_IN_ERR_EOF_INT_RAW, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_INT_RAW_REG.SPI_IN_SUC_EOF_INT_RAW = ", SPI_IN_SUC_EOF_INT_RAW, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_INT_RAW_REG.SPI_OUT_DONE_INT_RAW = ", SPI_OUT_DONE_INT_RAW, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_INT_RAW_REG.SPI_OUT_EOF_INT_RAW = ", SPI_OUT_EOF_INT_RAW, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_INT_RAW_REG.SPI_OUT_TOTAL_EOF_INT_RAW = ", SPI_OUT_TOTAL_EOF_INT_RAW, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_INT_RAW_REG.reserved1 = ", reserved1, ";");
  }
};
static_assert(sizeof(spi_dma_int_raw_reg_t) == 4, "invalid size for ESP32 spi_dma_int_raw_reg_t");

struct spi_dma_int_st_reg_t {
  uint32_t SPI_INLINK_DSCR_EMPTY_INT_ST : 1;    // read-only
  uint32_t SPI_OUTLINK_DSCR_ERROR_INT_ST : 1;   // read-only
  uint32_t SPI_INLINK_DSCR_ERROR_INT_ST : 1;    // read-only
  uint32_t SPI_IN_DONE_INT_ST : 1;              // read-only
  uint32_t SPI_IN_ERR_EOF_INT_ST : 1;           // read-only
  uint32_t SPI_IN_SUC_EOF_INT_ST : 1;           // read-only
  uint32_t SPI_OUT_DONE_INT_ST : 1;             // read-only
  uint32_t SPI_OUT_EOF_INT_ST : 1;              // read-only
  uint32_t SPI_OUT_TOTAL_EOF_INT_ST : 1;        // read-only
  uint32_t reserved1 : 23;

  void serial_dump() volatile {
    SERIAL_ECHOLNPGM("SPI_DMA_INT_ST_REG.SPI_INLINK_DSCR_EMPTY_INT_ST = ", SPI_INLINK_DSCR_EMPTY_INT_ST, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_INT_ST_REG.SPI_OUTLINK_DSCR_ERROR_INT_ST = ", SPI_OUTLINK_DSCR_ERROR_INT_ST, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_INT_ST_REG.SPI_INLINK_DSCR_ERROR_INT_ST = ", SPI_INLINK_DSCR_ERROR_INT_ST, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_INT_ST_REG.SPI_IN_DONE_INT_ST = ", SPI_IN_DONE_INT_ST, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_INT_ST_REG.SPI_IN_ERR_EOF_INT_ST = ", SPI_IN_ERR_EOF_INT_ST, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_INT_ST_REG.SPI_IN_SUC_EOF_INT_ST = ", SPI_IN_SUC_EOF_INT_ST, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_INT_ST_REG.SPI_OUT_DONE_INT_ST = ", SPI_OUT_DONE_INT_ST, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_INT_ST_REG.SPI_OUT_EOF_INT_ST = ", SPI_OUT_EOF_INT_ST, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_INT_ST_REG.SPI_OUT_TOTAL_EOF_INT_ST = ", SPI_OUT_TOTAL_EOF_INT_ST, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_INT_ST_REG.reserved1 = ", reserved1, ";");
  }
};
static_assert(sizeof(spi_dma_int_st_reg_t) == 4, "invalid size for ESP32 spi_dma_int_st_reg_t");

struct spi_dma_int_clr_reg_t {
  uint32_t SPI_INLINK_DSCR_EMPTY_INT_CLR : 1;
  uint32_t SPI_OUTLINK_DSCR_ERROR_INT_CLR : 1;
  uint32_t SPI_INLINK_DSCR_ERROR_INT_CLR : 1;
  uint32_t SPI_IN_DONE_INT_CLR : 1;
  uint32_t SPI_IN_ERR_EOF_INT_CLR : 1;
  uint32_t SPI_IN_SUC_EOF_INT_CLR : 1;
  uint32_t SPI_OUT_DONE_INT_CLR : 1;
  uint32_t SPI_OUT_EOF_INT_CLR : 1;
  uint32_t SPI_OUT_TOTAL_EOF_INT_CLR : 1;
  uint32_t reserved1 : 23;

  void serial_dump() volatile {
    SERIAL_ECHOLNPGM("SPI_DMA_INT_CLR_REG.SPI_INLINK_DSCR_EMPTY_INT_CLR = ", SPI_INLINK_DSCR_EMPTY_INT_CLR, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_INT_CLR_REG.SPI_OUTLINK_DSCR_ERROR_INT_CLR = ", SPI_OUTLINK_DSCR_ERROR_INT_CLR, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_INT_CLR_REG.SPI_INLINK_DSCR_ERROR_INT_CLR = ", SPI_INLINK_DSCR_ERROR_INT_CLR, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_INT_CLR_REG.SPI_IN_DONE_INT_CLR = ", SPI_IN_DONE_INT_CLR, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_INT_CLR_REG.SPI_IN_ERR_EOF_INT_CLR = ", SPI_IN_ERR_EOF_INT_CLR, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_INT_CLR_REG.SPI_IN_SUC_EOF_INT_CLR = ", SPI_IN_SUC_EOF_INT_CLR, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_INT_CLR_REG.SPI_OUT_DONE_INT_CLR = ", SPI_OUT_DONE_INT_CLR, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_INT_CLR_REG.SPI_OUT_EOF_INT_CLR = ", SPI_OUT_EOF_INT_CLR, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_INT_CLR_REG.SPI_OUT_TOTAL_EOF_INT_CLR = ", SPI_OUT_TOTAL_EOF_INT_CLR, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_INT_CLR_REG.reserved1 = ", reserved1, ";");
  }
};
static_assert(sizeof(spi_dma_int_clr_reg_t) == 4, "invalid size for ESP32 spi_dma_int_clr_reg_t");

struct spi_dma_rstatus_reg_t {
  uint32_t TX_DES_ADDRESS : 20; // read-only, ESP32 base address: 0x3FF00000
  uint32_t reserved1 : 10;
  uint32_t TX_FIFO_FULL : 1;    // read-only
  uint32_t TX_FIFO_EMPTY : 1;   // read-only

  void serial_dump() volatile {
    SERIAL_ECHOLNPGM("SPI_DMA_RSTATUS_REG.TX_DES_ADDRESS = ", TX_DES_ADDRESS, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_RSTATUS_REG.reserved1 = ", reserved1, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_RSTATUS_REG.TX_FIFO_FULL = ", TX_FIFO_FULL, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_RSTATUS_REG.TX_FIFO_EMPTY = ", TX_FIFO_EMPTY, ";");
  }
};
static_assert(sizeof(spi_dma_rstatus_reg_t) == 4, "invalid size for ESP32 spi_dma_rstatus_reg_t");

struct spi_dma_tstatus_reg_t {
  uint32_t RX_DES_ADDRESS : 20; // read-only, ESP32 base address: 0x3FF00000
  uint32_t reserved1 : 10;
  uint32_t RX_FIFO_FULL : 1;    // read-only
  uint32_t RX_FIFO_EMPTY : 1;   // read-only

  void serial_dump() volatile {
    SERIAL_ECHOLNPGM("SPI_DMA_TSTATUS_REG.RX_DES_ADDRESS = ", RX_DES_ADDRESS, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_TSTATUS_REG.reserved1 = ", reserved1, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_TSTATUS_REG.RX_FIFO_FULL = ", RX_FIFO_FULL, ";");
    SERIAL_ECHOLNPGM("SPI_DMA_TSTATUS_REG.RX_FIFO_EMPTY = ", RX_FIFO_EMPTY, ";");
  }
};
static_assert(sizeof(spi_dma_tstatus_reg_t) == 4, "invalid size for ESP32 spi_dma_tstatus_reg_t");

struct spi_dev_t {
  spi_cmd_reg_t SPI_CMD_REG;
  uint32_t SPI_ADDR_REG;
  spi_ctrl_reg_t SPI_CTRL_REG;
  spi_ctrl1_reg_t SPI_CTRL1_REG;
  uint32_t SPI_RD_STATUS_REG;
  spi_ctrl2_reg_t SPI_CTRL2_REG;
  spi_clock_reg_t SPI_CLOCK_REG;
  spi_user_reg_t SPI_USER_REG;
  spi_user1_reg_t SPI_USER1_REG;
  spi_user2_reg_t SPI_USER2_REG;
  spi_mosi_dlen_reg_t SPI_MOSI_DLEN_REG;
  spi_miso_dlen_reg_t SPI_MISO_DLEN_REG;
  uint32_t SPI_SLV_WR_STATUS_REG;
  spi_pin_reg_t SPI_PIN_REG;
  spi_slave_reg_t SPI_SLAVE_REG;
  spi_slave1_reg_t SPI_SLAVE1_REG;
  spi_slave2_reg_t SPI_SLAVE2_REG;
  spi_slave3_reg_t SPI_SLAVE3_REG;
  spi_slv_wrbuf_dlen_reg_t SPI_SLV_WRBUF_DLEN_REG;
  spi_slv_rdbuf_dlen_reg_t SPI_SLV_RDBUF_DLEN_REG;
  uint8_t pad1[0x14];
  spi_slv_rd_bit_reg_t SPI_SLV_RD_BIT_REG;
  uint8_t pad2[0x18];
  uint32_t SPI_W_REG[16];
  uint32_t SPI_TX_CRC_REG;
  uint8_t pad3[0x34];
  spi_ext2_reg_t SPI_EXT2_REG;
  uint8_t pad4[4];
  spi_dma_conf_reg_t SPI_DMA_CONF_REG;
  spi_dma_out_link_reg_t SPI_DMA_OUT_LINK_REG;
  spi_dma_in_link_reg_t SPI_DMA_IN_LINK_REG;
  spi_dma_status_reg_t SPI_DMA_STATUS_REG;
  spi_dma_int_ena_reg_t SPI_DMA_INT_ENA_REG;
  spi_dma_int_raw_reg_t SPI_DMA_INT_RAW_REG;
  spi_dma_int_st_reg_t SPI_DMA_INT_ST_REG;
  spi_dma_int_clr_reg_t SPI_DMA_INT_CLR_REG;
  const uint32_t SPI_IN_ERR_EOF_DES_ADDR_REG;
  const uint32_t SPI_IN_SUC_EOF_DES_ADDR_REG;
  const uint32_t SPI_INLINK_DSCR_REG;
  const uint32_t SPI_INLINK_DSCR_BF0_REG;
  const uint32_t SPI_INLINK_DSCR_BF1_REG;
  const uint32_t SPI_OUT_EOF_BFR_DES_ADDR_REG;
  const uint32_t SPI_OUT_EOF_DES_ADDR_REG;
  const uint32_t SPI_OUTLINK_DSCR_REG;
  const uint32_t SPI_OUTLINK_DSCR_BF0_REG;
  const uint32_t SPI_OUTLINK_DSCR_BF1_REG;
  spi_dma_rstatus_reg_t SPI_DMA_RSTATUS_REG;
  spi_dma_tstatus_reg_t SPI_DMA_TSTATUS_REG;

  void serial_dump() volatile {
    SERIAL_ECHOLNPGM("SPI_ADDR_REG = ", SPI_ADDR_REG, ";");
    SERIAL_ECHOLNPGM("SPI_RD_STATUS_REG = ", SPI_RD_STATUS_REG, ";");
    SERIAL_ECHOLNPGM("SPI_SLV_WR_STATUS_REG = ", SPI_SLV_WR_STATUS_REG, ";");
    SERIAL_ECHOLNPGM("SPI_TX_CRC_REG = ", SPI_TX_CRC_REG, ";");

    SPI_CMD_REG.serial_dump();
    SPI_CTRL_REG.serial_dump();
    SPI_CTRL1_REG.serial_dump();
    SPI_CTRL2_REG.serial_dump();
    SPI_CLOCK_REG.serial_dump();
    SPI_USER_REG.serial_dump();
    SPI_USER1_REG.serial_dump();
    SPI_USER2_REG.serial_dump();
    SPI_MOSI_DLEN_REG.serial_dump();
    SPI_MISO_DLEN_REG.serial_dump();
    SPI_PIN_REG.serial_dump();
    SPI_SLAVE_REG.serial_dump();
    SPI_SLAVE1_REG.serial_dump();
    SPI_SLAVE2_REG.serial_dump();
    SPI_SLAVE3_REG.serial_dump();
    SPI_SLV_WRBUF_DLEN_REG.serial_dump();
    SPI_SLV_RDBUF_DLEN_REG.serial_dump();
    SPI_SLV_RD_BIT_REG.serial_dump();
    SPI_EXT2_REG.serial_dump();
    SPI_DMA_CONF_REG.serial_dump();
    SPI_DMA_OUT_LINK_REG.serial_dump();
    SPI_DMA_IN_LINK_REG.serial_dump();
    SPI_DMA_STATUS_REG.serial_dump();
    SPI_DMA_INT_ENA_REG.serial_dump();
    SPI_DMA_INT_RAW_REG.serial_dump();
    //SPI_DMA_INT_ST_REG.serial_dump();
    //SPI_DMA_INT_CLR_REG.serial_dump();

    SERIAL_ECHOLNPGM("SPI_IN_ERR_EOF_DES_ADDR_REG = ", SPI_IN_ERR_EOF_DES_ADDR_REG, ";");
    SERIAL_ECHOLNPGM("SPI_IN_SUC_EOF_DES_ADDR_REG = ", SPI_IN_SUC_EOF_DES_ADDR_REG, ";");
    SERIAL_ECHOLNPGM("SPI_INLINK_DSCR_REG = ", SPI_INLINK_DSCR_REG, ";");
    SERIAL_ECHOLNPGM("SPI_INLINK_DSCR_BF0_REG = ", SPI_INLINK_DSCR_BF0_REG, ";");
    SERIAL_ECHOLNPGM("SPI_INLINK_DSCR_BF1_REG = ", SPI_INLINK_DSCR_BF1_REG, ";");
    SERIAL_ECHOLNPGM("SPI_OUT_EOF_BFR_DES_ADDR_REG = ", SPI_OUT_EOF_BFR_DES_ADDR_REG, ";");
    SERIAL_ECHOLNPGM("SPI_OUT_EOF_DES_ADDR_REG = ", SPI_OUT_EOF_DES_ADDR_REG, ";");
    SERIAL_ECHOLNPGM("SPI_OUTLINK_DSCR_REG = ", SPI_OUTLINK_DSCR_REG, ";");
    SERIAL_ECHOLNPGM("SPI_OUTLINK_DSCR_BF0_REG = ", SPI_OUTLINK_DSCR_BF0_REG, ";");
    SERIAL_ECHOLNPGM("SPI_OUTLINK_DSCR_BF1_REG = ", SPI_OUTLINK_DSCR_BF1_REG, ";");

    SPI_DMA_RSTATUS_REG.serial_dump();
    SPI_DMA_TSTATUS_REG.serial_dump();
  }
};
static_assert(offsetof(spi_dev_t, SPI_CLOCK_REG) == 0x18, "invalid ESP32 offsetof SPI_CLOCK_REG");
static_assert(offsetof(spi_dev_t, SPI_W_REG[0]) == 0x80, "invalid ESP32 offsetof SPI_W_REG");
static_assert(offsetof(spi_dev_t, SPI_EXT2_REG) == 0xF8, "invalid ESP32 offsetof SPI_EXT2_REG");
static_assert(sizeof(spi_dev_t) == 0x150, "wrong formatting of ESP32 spi_dev_t!");

#ifdef HALSPI_ESP32_ENABLE_INTERNBUS
__ESP32_DEFREG(spi_dev_t, SPI0, 0x3FF43000); // shares signals with SPI1 (USAGE DISALLOWED)
__ESP32_DEFREG(spi_dev_t, SPI1, 0x3FF42000); // (USAGE DISALLOWED)
#endif
__ESP32_DEFREG(spi_dev_t, SPI2, 0x3FF64000); // HSPI
__ESP32_DEFREG(spi_dev_t, SPI3, 0x3FF65000); // VSPI

#define _SPI_DEFAULT_BUS 2
#define __SPI_BUSOBJ(IDX) (SPI##IDX)
#define _SPI_BUSOBJ(IDX) (__SPI_BUSOBJ(IDX))

inline volatile spi_dev_t& SPIGetBusFromIndex(uint8_t idx) {
  switch(idx) {
#ifdef HALSPI_ESP32_ENABLE_INTERNBUS
  case 0: return SPI0;
  case 1: return SPI1;
#endif
  case 2: return SPI2;
  case 3: return SPI3;
  default: return _SPI_BUSOBJ(_SPI_DEFAULT_BUS);
  }
}

inline uint8_t SPIGetBusIndex(volatile spi_dev_t& SPI) {
#ifdef HALSPI_ESP32_ENABLE_INTERNBUS
  if (&SPI == &SPI0) return 0;
  else if (&SPI == &SPI1) return 1;
  else
#endif
  if (&SPI == &SPI2) return 2;
  else if (&SPI == &SPI3) return 3;
  return _SPI_DEFAULT_BUS;
}

// page 57 of the ESP32 technical reference manual contains default GPIO states after reset.
// MOSI: SPID/HSPID/VSPID
// MISO: SPIQ/HSPIQ/VSPIQ
// CS: SPICS0/HSPICS0/VSPICS0
// CLK: SPICLK/HSPICLK/VSPICLK

inline void SPIGetSignalForBus(uint8_t busIdx, uint8_t& sckOut, uint8_t& misoOut, uint8_t& mosiOut, uint8_t& csOut) {
#ifdef HALSPI_ESP32_ENABLE_INTERNBUS
  if (busIdx == 0 || busIdx == 1) {
    sckOut = 0;
    misoOut = 1;
    mosiOut = 2;
    csOut = 5;
  }
  else
#endif
  if (busIdx == 2) {
    sckOut = 8;
    misoOut = 9;
    mosiOut = 10;
    csOut = 11;
  }
  else if (busIdx == 3) {
    sckOut = 63;
    misoOut = 64;
    mosiOut = 65;
    csOut = 68;
  }
  else {
    sckOut = 0xFF;
    misoOut = 0xFF;
    mosiOut = 0xFF;
    csOut = 0xFF;
  }
}

inline gpio_num_t _GetInternalPinName(int pin) {
  if (pin >= 0 && pin <= GPIO_NUM_MAX) {
    return (gpio_num_t)pin;
  }
  return (gpio_num_t)-1;  // NC
}

#define SPI_IOMUX_INVAL 0xFF

// page 57 of ESP32 ref manual.
// IOMUX is faster than GPIO matrix.
inline bool SPIFindIOMUXMapping(
  int gpio_sck, int gpio_miso, int gpio_mosi, int gpio_cs,
  uint8_t& spibusIdxOut, uint8_t& func_sck_out, uint8_t& func_miso_out, uint8_t& func_mosi_out, uint8_t& func_cs_out
) {
  uint8_t func_sck = SPI_IOMUX_INVAL;
  uint8_t func_miso = SPI_IOMUX_INVAL;
  uint8_t func_mosi = SPI_IOMUX_INVAL;
  uint8_t func_cs = SPI_IOMUX_INVAL;
  bool found = false;
#ifdef HALSPI_ESP32_ENABLE_INTERNBUS
  if (gpio_sck == 6 || gpio_miso == 7 || gpio_mosi == 8 || gpio_cs == 11) {
    if (gpio_sck == 6) func_sck = 1;
    if (gpio_miso == 7) func_miso = 1;
    if (gpio_mosi == 8) func_mosi = 1;
    if (gpio_cs == 11) func_cs = 1;
    spibusIdxOut = 0;
    found = true;
  }
  else
#endif
  if (gpio_sck == 14 || gpio_miso == 12 || gpio_mosi == 13 || gpio_cs == 15) {
    if (gpio_sck == 14) func_sck = 1;
    if (gpio_miso == 12) func_miso = 1;
    if (gpio_mosi == 13) func_mosi = 1;
    if (gpio_cs == 15) func_cs = 1;
    spibusIdxOut = 2;
    found = true;
  }
  else if (gpio_sck == 18 || gpio_miso == 19 || gpio_mosi == 23 || gpio_cs == 5) {
    if (gpio_sck == 18) func_sck = 1;
    if (gpio_miso == 19) func_miso = 1;
    if (gpio_mosi == 23) func_mosi = 1;
    if (gpio_cs == 5) func_cs = 1;
    spibusIdxOut = 3;
    found = true;
  }

  if (found) {
    func_sck_out = func_sck;
    func_miso_out = func_miso;
    func_mosi_out = func_mosi;
    func_cs_out = func_cs;
  }
  return found;
}

// There is very little documentation on certain ESP32 internals.

struct gpioMapResult_t {
  uint8_t spibusIdx;
  int gpio_sck;
  int gpio_miso;
  int gpio_mosi;
  int gpio_cs;
  bool datasig_is_direct_iomux;
};

inline gpioMapResult_t SPIMapGPIO(int gpio_sck, int gpio_miso, int gpio_mosi, int gpio_cs) {
  if (gpio_sck < 0 && gpio_miso < 0 && gpio_mosi < 0 && gpio_cs < 0) {
    // Select SPI2.
    gpio_sck = 14;
    gpio_miso = 12;
    gpio_mosi = 13;
    gpio_cs = 15;
  }

  uint8_t spibusIdx = _SPI_DEFAULT_BUS;  // default SPI bus index

  uint8_t func_sck, func_miso, func_mosi, func_cs;
  bool fastiomux = SPIFindIOMUXMapping(
    gpio_sck, gpio_miso, gpio_mosi, gpio_cs,
    spibusIdx, func_sck, func_miso, func_mosi, func_cs
  );

  bool has_sck_map = false;
  bool has_miso_map = false;
  bool has_mosi_map = false;
#if 0
  bool has_cs_map = false;
#endif

  uint8_t sck_sig, miso_sig, mosi_sig, cs_sig;
  SPIGetSignalForBus(spibusIdx, sck_sig, miso_sig, mosi_sig, cs_sig);

  (void)cs_sig;

  bool miso_iomux = false;

  if (fastiomux) {
    if (func_sck != SPI_IOMUX_INVAL) {
      gpio_iomux_in((unsigned int)gpio_sck, sck_sig);
      gpio_iomux_out((unsigned int)gpio_sck, func_sck, false);
      has_sck_map = true;
    }
    if (func_miso != SPI_IOMUX_INVAL) {
      gpio_iomux_in((unsigned int)gpio_miso, miso_sig);
      gpio_iomux_out((unsigned int)gpio_miso, func_miso, false);
      has_miso_map = true;
      miso_iomux = true;
    }
    if (func_mosi != SPI_IOMUX_INVAL) {
      gpio_iomux_in((unsigned int)gpio_mosi, mosi_sig);
      gpio_iomux_out((unsigned int)gpio_mosi, func_mosi, false);
      has_mosi_map = true;
    }
#if 0
    if (func_cs != SPI_IOMUX_INVAL) {
      gpio_iomux_in((unsigned int)gpio_cs, cs_sig);
      gpio_iomux_out((unsigned int)gpio_cs, func_cs, false);
      has_cs_map = true;
      has_mosi_map = true;
    }
#endif
  }

  if (has_sck_map == false && gpio_sck >= 0) {
    gpio_matrix_out((unsigned int)gpio_sck, sck_sig, false, false);
    pinMode(gpio_sck, OUTPUT);
  }
  if (has_miso_map == false && gpio_miso >= 0) {
    gpio_matrix_in((unsigned int)gpio_miso, miso_sig, false);
    pinMode(gpio_miso, INPUT);
  }
  if (has_mosi_map == false && gpio_mosi >= 0) {
    gpio_matrix_out((unsigned int)gpio_mosi, mosi_sig, false, false);
    pinMode(gpio_mosi, OUTPUT);
  }
#if 0
  if (has_cs_map == false && gpio_cs >= 0) {
    gpio_matrix_out((unsigned int)gpio_cs, cs_sig, false, false);
    pinMode(gpio_cs, OUTPUT);
  }
#endif

  gpioMapResult_t result;
  result.spibusIdx = spibusIdx;
  result.gpio_sck = gpio_sck;
  result.gpio_miso = gpio_miso;
  result.gpio_mosi = gpio_mosi;
  result.gpio_cs = gpio_cs;
  result.datasig_is_direct_iomux = (miso_iomux);
  return result;
}

inline void SPIUnmapGPIO(gpioMapResult_t& result) {
  if (result.gpio_sck >= 0) {
    gpio_reset_pin(_GetInternalPinName(result.gpio_sck));
  }
  if (result.gpio_miso >= 0) {
    gpio_reset_pin(_GetInternalPinName(result.gpio_miso));
  }
  if (result.gpio_mosi >= 0) {
    gpio_reset_pin(_GetInternalPinName(result.gpio_mosi));
  }
#if 0
  if (result.gpio_cs >= 0) {
    gpio_reset_pin(_GetInternalPinName(result.gpio_cs));
  }
#endif
}

struct dport_perip_clk_en_reg_t {
  uint32_t reserved1 : 1;
  uint32_t DPORT_SPI01_CLK_EN : 1;
  uint32_t DPORT_UART_CLK_EN : 1;
  uint32_t reserved2 : 1;
  uint32_t DPORT_I2S0_CLK_EN : 1;
  uint32_t DPORT_UART1_CLK_EN : 1;
  uint32_t DPORT_SPI2_CLK_EN : 1;
  uint32_t DPORT_I2C_EXT0_CLK_EN : 1;
  uint32_t DPORT_UHCI0_CLK_EN : 1;
  uint32_t DPORT_RMT_CLK_EN : 1;
  uint32_t DPORT_PCNT_CLK_EN : 1;
  uint32_t DPORT_LEDC_CLK_EN : 1;
  uint32_t DPORT_UHCI1_CLK_EN : 1;
  uint32_t DPORT_TIMERGROUP_CLK_EN : 1;
  uint32_t DPORT_EFUSE_CLK_EN : 1;
  uint32_t DPORT_TIMERGROUP1_CLK_EN : 1;
  uint32_t DPORT_SPI3_CLK_EN : 1;
  uint32_t DPORT_PWM0_CLK_EN : 1;
  uint32_t DPORT_I2C_EXT1_CLK_EN : 1;
  uint32_t DPORT_TWAI_CLK_EN : 1;
  uint32_t DPORT_PWM1_CLK_EN : 1;
  uint32_t DPORT_I2S1_CLK_EN : 1;
  uint32_t DPORT_SPI_DMA_CLK_EN : 1;
  uint32_t DPORT_UART2_CLK_EN : 1;
  uint32_t DPORT_UART_MEM_CLK_EN : 1;
  uint32_t reserved3 : 1;
  uint32_t reserved4 : 1;
  uint32_t reserved5 : 5;
};
static_assert(sizeof(dport_perip_clk_en_reg_t) == 4, "invalid size of ESP32 dport_perip_clk_en_reg_t");

struct dport_perip_rst_en_reg_t {
  uint32_t reserved1 : 1;
  uint32_t DPORT_SPI01_RST : 1;
  uint32_t DPORT_UART_RST : 1;
  uint32_t reserved2 : 1;
  uint32_t DPORT_I2S0_RST : 1;
  uint32_t DPORT_UART1_RST : 1;
  uint32_t DPORT_SPI2_RST : 1;
  uint32_t DPORT_I2C_EXT0_RST : 1;
  uint32_t DPORT_UHCI0_RST : 1;
  uint32_t DPORT_RMT_RST : 1;
  uint32_t DPORT_PCNT_RST : 1;
  uint32_t DPORT_LEDC_RST : 1;
  uint32_t DPORT_UHCI1_RST : 1;
  uint32_t DPORT_TIMERGROUP_RST : 1;
  uint32_t DPORT_EFUSE_RST : 1;
  uint32_t DPORT_TIMERGROUP1_RST : 1;
  uint32_t DPORT_SPI3_RST : 1;
  uint32_t DPORT_PWM0_RST : 1;
  uint32_t DPORT_I2C_EXT1_RST : 1;
  uint32_t DPORT_TWAI_RST : 1;
  uint32_t DPORT_PWM1_RST : 1;
  uint32_t DPORT_I2S1_RST : 1;
  uint32_t DPORT_SPI_DMA_RST : 1;
  uint32_t DPORT_UART2_RST : 1;
  uint32_t DPORT_UART_MEM_RST : 1;
  uint32_t reserved3 : 1;
  uint32_t reserved4 : 1;
  uint32_t reserved5 : 5;
};
static_assert(sizeof(dport_perip_rst_en_reg_t) == 4, "invalid size of ESP32 dport_perip_rst_en_reg_t");

__ESP32_DEFREG(dport_perip_clk_en_reg_t, DPORT_PERIP_CLK_EN_REG, 0x3FF000C0);
__ESP32_DEFREG(dport_perip_rst_en_reg_t, DPORT_PERIP_RST_EN_REG, 0x3FF000C4);

#define DPORT_SPI_DMA_CHAN_SEL_NONE 0
#define DPORT_SPI_DMA_CHAN_SEL_CHAN1 1
#define DPORT_SPI_DMA_CHAN_SEL_CHAN2 2

struct dport_spi_dma_chan_sel_reg_t {
  uint32_t DPORT_SPI_SPI1_DMA_CHAN_SEL : 2;
  uint32_t DPORT_SPI_SPI2_DMA_CHAN_SEL : 2;
  uint32_t DPORT_SPI_SPI3_DMA_CHAN_SEL : 2;
  uint32_t reserved1 : 26;
};
static_assert(sizeof(dport_spi_dma_chan_sel_reg_t) == sizeof(uint32_t), "invalid size of ESP32 dport_spi_dma_chan_sel_reg_t");

__ESP32_DEFREG(dport_spi_dma_chan_sel_reg_t, DPORT_SPI_DMA_CHAN_SEL_REG, 0x3FF005A8);

#if 0

struct dport_cpu_per_conf_reg_t {
  uint32_t DPORT_CPU_CPUPERIOD_SEL : 2;
  uint32_t reserved1 : 30;
};
static_assert(sizeof(dport_cpu_per_conf_reg_t) == 4, "invalid size of ESP32 dport_cpu_per_conf_reg_t");

__ESP32_DEFREG(dport_cpu_per_conf_reg_t, DPORT_CPU_PER_CONF_REG, 0x3FF0003C);

#define RTC_CNTL_SOC_CLK_XTL 0
#define RTC_CNTL_SOC_CLK_PLL 1
#define RTC_CNTL_SOC_CLK_RC_FAST 2
#define RTC_CNTL_SOC_CLK_APLL 3

struct rtc_cntl_clk_conf_reg_t {
  uint32_t reserved1 : 4;
  uint32_t RTC_CNTRL_CK8M_DIV : 2;
  uint32_t RTC_CNTL_ENB_CK8M : 1;
  uint32_t RTC_CNTL_ENB_CK8M_DIV : 1;
  uint32_t RTC_CNTL_DIG_XTAL32K_EN : 1;
  uint32_t RTC_CNTL_DIG_CLK8M_D256_EN : 1;
  uint32_t RTC_CNTL_DIG_CLK8M_EN : 1;
  uint32_t reserved2 : 1;
  uint32_t RTC_CNTL_CK8M_DIV_SEL : 3;
  uint32_t reserved3 : 2;
  uint32_t RTC_CNTL_CK8M_DFREQ : 8;
  uint32_t RTC_CNTL_CK8M_FORCE_PD : 1;
  uint32_t RTC_CNTL_CK8M_FORCE_PU : 1;
  uint32_t RTC_CNTL_SOC_CLK_SEL : 2;
  uint32_t RTC_CNTL_FAST_CLK_RTC_SEL : 1;
  uint32_t RTC_CNTL_ANA_CLK_RTC_SEL : 2;
};
static_assert(sizeof(rtc_cntl_clk_conf_reg_t) == 4, "invalid size of ESP32 rtc_cntl_clk_conf_reg_t");

__ESP32_DEFREG(rtc_cntl_clk_conf_reg_t, RTC_CNTL_CLK_CONF_REG, 0x3FF48070);

struct syscon_conf_reg_t {
  uint32_t SYSCON_PRE_DIV_CNT : 10;
  uint32_t SYSCON_CLK_320M_EN : 1;
  uint32_t SYSCON_CLK_EN : 1;
  uint32_t SYSCON_RST_TICK_CNT : 1;
  uint32_t SYSCON_QUICK_CLK_CHNG : 1;
  uint32_t reserved1 : 18;
};

// Undocumented.
__ESP32_DEFREG(syscon_conf_reg_t, SYSCON_CONF_REG, 0x3FF66000);

#endif

// You can transfer up to 64bytes in one burst using default SPI without DMA on the ESP32.

struct clkcnt_res_t {
  uint32_t n : 6;
  uint32_t l : 6;
  uint32_t h : 6;
  uint32_t pre : 13;
  uint32_t sysclock : 1;

  inline uint32_t GetFrequencyDivider(void) const {
    return (n+1)*(pre+1);
  }
};

static void SPIConfigureClock(volatile spi_dev_t& SPI, int clkMode, bool is_direct_io, const clkcnt_res_t& clkdiv) {
  if (clkMode == SPI_CLKMODE_0) {
    SPI.SPI_PIN_REG.SPI_CK_IDLE_EDGE = 0;
    SPI.SPI_USER_REG.SPI_CK_OUT_EDGE = 0;
  }
  else if (clkMode == SPI_CLKMODE_1) {
    SPI.SPI_PIN_REG.SPI_CK_IDLE_EDGE = 0;
    SPI.SPI_USER_REG.SPI_CK_OUT_EDGE = 1;
  }
  else if (clkMode == SPI_CLKMODE_2) {
    SPI.SPI_PIN_REG.SPI_CK_IDLE_EDGE = 1;
    SPI.SPI_USER_REG.SPI_CK_OUT_EDGE = 1;
  }
  else if (clkMode == SPI_CLKMODE_3) {
    SPI.SPI_PIN_REG.SPI_CK_IDLE_EDGE = 1;
    SPI.SPI_USER_REG.SPI_CK_OUT_EDGE = 0;
  }

  spi_clock_reg_t _SPI_CLOCK_REG;
  _SPI_CLOCK_REG.SPI_CLK_EQU_SYSCLK = clkdiv.sysclock;
  _SPI_CLOCK_REG.SPI_CLKDIV_PRE = clkdiv.pre;
  _SPI_CLOCK_REG.SPI_CLKCNT_N = clkdiv.n;
  _SPI_CLOCK_REG.SPI_CLKCNT_L = clkdiv.l;
  _SPI_CLOCK_REG.SPI_CLKCNT_H = clkdiv.h;
  dwrite(SPI.SPI_CLOCK_REG, _SPI_CLOCK_REG);

#ifndef HALSPI_ESP32_DISABLE_ADV_DELAYCONF
  uint32_t apbFreqDiv = clkdiv.GetFrequencyDivider();

  if (is_direct_io) {
    if (apbFreqDiv > 4) {
      SPI.SPI_CTRL2_REG.SPI_MISO_DELAY_MODE = 0;
    }
    else {
      if (clkMode == SPI_CLKMODE_0 || clkMode == SPI_CLKMODE_3) {
        SPI.SPI_CTRL2_REG.SPI_MISO_DELAY_MODE = 2;
      }
      else {
        SPI.SPI_CTRL2_REG.SPI_MISO_DELAY_MODE = 1;
      }
    }
    SPI.SPI_USER_REG.SPI_USR_DUMMY = 0;
  }
  else {
    if (apbFreqDiv <= 2) {
      SPI.SPI_CTRL2_REG.SPI_MISO_DELAY_MODE = 0;
      SPI.SPI_USER_REG.SPI_USR_DUMMY = 1;
      SPI.SPI_USER1_REG.SPI_USR_DUMMY_CYCLELEN = 0;
    }
    else if (apbFreqDiv < 8) {
      SPI.SPI_CTRL2_REG.SPI_MISO_DELAY_MODE = 0;
      SPI.SPI_USER_REG.SPI_USR_DUMMY = 0;
    }
    else {
      if (clkMode == SPI_CLKMODE_0 || clkMode == SPI_CLKMODE_3) {
        SPI.SPI_CTRL2_REG.SPI_MISO_DELAY_MODE = 2;
      }
      else {
        SPI.SPI_CTRL2_REG.SPI_MISO_DELAY_MODE = 1;
      }
      SPI.SPI_USER_REG.SPI_USR_DUMMY = 0;
    }
  }
#else
  SPI.SPI_USER_REG.SPI_USR_DUMMY = 0;
  SPI.SPI_USER1_REG.SPI_USR_DUMMY_CYCLELEN = 0;
  SPI.SPI_CTRL2_REG.SPI_MISO_DELAY_MODE = 0;
#endif
  SPI.SPI_CTRL2_REG.SPI_MISO_DELAY_NUM = 0;
  SPI.SPI_CTRL2_REG.SPI_MOSI_DELAY_MODE = 0;
  SPI.SPI_CTRL2_REG.SPI_MOSI_DELAY_NUM = 0;
}

#if 0
static const uint8_t _spi_clkcnt_n_primes[] = {
  2, 3, 4, 5, 11, 13, 17, 19, 23, 29, 31,
  37, 41, 43, 47, 53, 59, 61
};
#endif

template <eir::unsigned_integral T1, eir::unsigned_integral T2>
inline auto DIST(T1 a, T2 b) noexcept -> decltype(a-b)
{
  if (a < b) {
    return b - a;
  }
  else {
    return a - b;
  }
}

// see page 121 of ESP32 technical reference manual (GP-SPI Clock Control)
inline clkcnt_res_t SPIApproximateClockDivider(uint32_t maxClockFreq, uint32_t spibasefreq) {
#if 0
  // compare with https://github.com/espressif/esp-idf/blob/0a93ee1337c15668c7f9998a53822f281a17670e/components/hal/esp32/include/hal/spi_ll.h#L551
  // Cannot get this code to work.

  clkcnt_res_t res;

  if (spibasefreq >= maxClockFreq) {
    res.sysclock = true;
    res.pre = 0;
    res.n = 0;
    res.l = 0;
    res.h = 0;
    return res;
  }
  res.sysclock = false;

  // maxClockFreq = f_spi
  // spibasefreq = f_apb

  uint32_t approx_div = CEIL_DIV(spibasefreq, maxClockFreq);  // pre + 1
  if (approx_div <= (1<<13)) {  // 2^13
    res.pre = (approx_div - 1);
    res.n = 1;  // n needs to be at least 2 so we have one LOW and one HIGH pulse (this detail is found nowhere in the ESP32 docs)
    goto finalize;
  }

  {
    uint8_t best_approx_prime = 2;  // n + 1
    uint32_t best_approx = 2; // to avoid annoying GCC warning
    bool found_best_approx_prime = false;

    for (uint8_t prime : _spi_clkcnt_n_primes) {
      uint32_t primediv = (approx_div / prime);
      uint32_t approx_freqdiv = (primediv * approx_div);
      if (approx_freqdiv <= (1<<6)*(1<<13)) { // (2^6 * 2^13)
        if (found_best_approx_prime == false || (best_approx > approx_freqdiv)) {
          best_approx_prime = prime;
          best_approx = approx_freqdiv;
          found_best_approx_prime = true;
        }
      }
    }

    if (found_best_approx_prime == false || (best_approx_prime*approx_div <= 1)) {
      best_approx_prime = 2;
    }

    res.n = (best_approx_prime - 1);
    res.pre = (approx_div - 1);
  }
finalize:
  res.h = 0u;
  res.l = eir::Maximum( ((unsigned int)(res.n+1u) / 2u), 1u ) - 1u;
  return res;
#else
  // Taken from the ArduinoCore ESP32 because the ESP32 clock documentation is VERY lacking!
  if(maxClockFreq >= spibasefreq) {
    clkcnt_res_t sysreg;
    sysreg.sysclock = true;
    sysreg.pre = 0;
    sysreg.n = 0;
    sysreg.l = 0;
    sysreg.h = 0;
    return sysreg;
  }

  clkcnt_res_t minFreqReg;
  minFreqReg.sysclock = false;
  minFreqReg.pre = (1<<13)-1;
  minFreqReg.n = (1<<6)-1;
  minFreqReg.l = 0;
  minFreqReg.h = 0;
  uint32_t minFreq = (spibasefreq / minFreqReg.GetFrequencyDivider());
  if(maxClockFreq < minFreq) {
    return minFreqReg;
  }

  uint8_t calN = 1;
  clkcnt_res_t bestReg;
  bestReg.pre = 0;
  bestReg.n = 0;
  bestReg.l = 0;
  bestReg.h = 0;
  bestReg.sysclock = false;
  uint32_t bestFreq = 0;
  bool has_best_freq = false;

  while(calN <= (1<<6)-1) {
    clkcnt_res_t reg;
    reg.pre = 0;
    reg.n = 0;
    reg.l = 0;
    reg.h = 0;
    reg.sysclock = false;
    uint32_t calFreq;
    int32_t calPre;
    int8_t calPreVari = -2;

    reg.n = calN;
    reg.l = ((reg.n + 1) / 2);

    calPre = ((int32_t)((spibasefreq / (reg.n + 1)) / maxClockFreq) - 1) + calPreVari;
    while(calPreVari++ <= 1) {
      calPre++;
      if(calPre > (1<<13)-1) {
        reg.pre = (1<<13)-1;
      } else if(calPre <= 0) {
        reg.pre = 0;
      } else {
        reg.pre = (uint32_t)calPre;
      }
      calFreq = (spibasefreq / reg.GetFrequencyDivider());
      if(calFreq == maxClockFreq) {
        return reg;
      }
      else if(calFreq < maxClockFreq) {
        if(has_best_freq == false || ((maxClockFreq - calFreq) < (maxClockFreq - bestFreq))) {
          bestFreq = calFreq;
          bestReg = reg;
          has_best_freq = true;
        }
      }
    }
    calN++;
  }
  return bestReg;
#endif
}

static void SPIConfigureBitOrder(volatile spi_dev_t& SPI, int bitOrder) {
  if (bitOrder == SPI_BITORDER_MSB) {
    SPI.SPI_CTRL_REG.SPI_WR_BIT_ORDER = _ESP32_BIT_ORDER_MSB;
    SPI.SPI_CTRL_REG.SPI_RD_BIT_ORDER = _ESP32_BIT_ORDER_MSB;
  }
  else {
    SPI.SPI_CTRL_REG.SPI_WR_BIT_ORDER = _ESP32_BIT_ORDER_LSB;
    SPI.SPI_CTRL_REG.SPI_RD_BIT_ORDER = _ESP32_BIT_ORDER_LSB;
  }
}

inline uint16_t SPIGetWriteBufferSize(volatile spi_dev_t& SPI) {
  return 64;//( SPI.SPI_USER_REG.SPI_USR_MOSI_HIGHPART == 1 ? 32 : 64 );
}

inline uint16_t SPIGetWriteBufferStartIndex(volatile spi_dev_t& SPI) {
  return 0;//( SPI.SPI_USER_REG.SPI_USR_MOSI_HIGHPART == 1 ? 8 : 0 );
}

template <eir::bitmanager_type bitManType>
inline void IRAM_ATTR SPIPrepareWriteBitManager(volatile spi_dev_t& SPI, bitManType& bitman) noexcept {
  bool wr_msbfirst = (SPI.SPI_CTRL_REG.SPI_WR_BIT_ORDER == _ESP32_BIT_ORDER_MSB);

  bitman.SetDefaultStorageProperty(
    wr_msbfirst ? eir::endian::eSpecificEndian::BIG_ENDIAN : eir::endian::eSpecificEndian::LITTLE_ENDIAN,
    false
  );
}

template <eir::bitmanager_type bitManType>
inline void IRAM_ATTR SPIPrepareReadBitManager(volatile spi_dev_t& SPI, bitManType& bitman) noexcept {
  bool rd_msbfirst = (SPI.SPI_CTRL_REG.SPI_RD_BIT_ORDER == _ESP32_BIT_ORDER_MSB);

  bitman.SetDefaultStorageProperty(
    rd_msbfirst ? eir::endian::eSpecificEndian::BIG_ENDIAN : eir::endian::eSpecificEndian::LITTLE_ENDIAN,
    false
  );
}

template <typename numberType>
inline void IRAM_ATTR SPIWriteDataToTransferIsolated(
  volatile spi_dev_t& SPI, const numberType *buf, uint16_t cnt, uint32_t srcByteStartIdx,
  uint16_t& cntSentBytes_out
) {
  uint16_t start_num_idx = SPIGetWriteBufferStartIndex(SPI);

  esp32BitManager <const numberType> src_bitman( buf, cnt );

  SPIPrepareWriteBitManager( SPI, src_bitman );

  eir::BitNumberIteratorForStruct <numberType> src_iter;
  src_iter.addBytes( srcByteStartIdx );
  src_bitman.SetIterator( src_iter );

  eir::BitNumberIteratorForStruct <uint32_t> txiter( start_num_idx );
  while ( src_bitman.IsAtEnd() == false && txiter.getNumberOffset() < COUNT(SPI.SPI_W_REG)  ) {
    uint32_t txitem = 0u;
    uint32_t n = txiter.getNumberOffset();
    src_bitman.FetchSingle( txitem, txiter );
    SPI.SPI_W_REG[n] = txitem;
  }

  cntSentBytes_out = ( txiter.getTotalByteOffset() );
}

inline uint16_t SPIGetReadBufferSize(volatile spi_dev_t& SPI) {
  return 64;//( SPI.SPI_USER_REG.SPI_USR_MISO_HIGHPART == 1 ? 32 : 64 );
}

inline uint16_t SPIGetReadBufferStartIndex(volatile spi_dev_t& SPI) {
  return 0;//( SPI.SPI_USER_REG.SPI_USR_MISO_HIGHPART == 1 ? 8 : 0 );
}

static void SPITransaction(volatile spi_dev_t& SPI, uint32_t txcount_bytes) {
  if (txcount_bytes == 0) return;

  uint32_t txcount_bits = ( txcount_bytes * 8u );

  if (SPI.SPI_USER_REG.SPI_USR_MOSI == true) {
    SPI.SPI_MOSI_DLEN_REG.SPI_USR_MOSI_DBITLEN = txcount_bits - 1;
  }
  if (SPI.SPI_USER_REG.SPI_USR_MISO == true) {
    SPI.SPI_MISO_DLEN_REG.SPI_USR_MISO_DBITLEN = txcount_bits - 1;
  }

  SPI.SPI_CMD_REG.SPI_USR = true;

  spi_monitored_loop usrw;
  while (SPI.SPI_CMD_REG.SPI_USR) {
    /* wait until transfer has finished */
    usrw.update(1);
  }
}

static void SPIResetBus(volatile spi_dev_t& SPI) {
  spi_cmd_reg_t SPI_CMD_REG;
  SPI_CMD_REG.reserved1 = 0;
  SPI_CMD_REG.SPI_USR = 0;
  SPI_CMD_REG.reserved2 = 0;
  dwrite(SPI.SPI_CMD_REG, SPI_CMD_REG);

  SPI.SPI_ADDR_REG = 0;

  spi_ctrl_reg_t SPI_CTRL_REG;
  SPI_CTRL_REG.reserved1 = 0;
  SPI_CTRL_REG.SPI_FASTRD_MODE = 1;
  SPI_CTRL_REG.SPI_FREAD_DUAL = 0;
  SPI_CTRL_REG.reserved2 = 0;
  SPI_CTRL_REG.SPI_FREAD_QUAD = 0;
  SPI_CTRL_REG.SPI_WP = 1;
  SPI_CTRL_REG.reserved3 = 0;
  SPI_CTRL_REG.SPI_FREAD_DIO = 0;
  SPI_CTRL_REG.SPI_FREAD_QIO = 0;
  SPI_CTRL_REG.SPI_RD_BIT_ORDER = _ESP32_BIT_ORDER_MSB;
  SPI_CTRL_REG.SPI_WR_BIT_ORDER = _ESP32_BIT_ORDER_MSB;
  SPI_CTRL_REG.reserved4 = 0;
  dwrite(SPI.SPI_CTRL_REG, SPI_CTRL_REG);

  spi_ctrl1_reg_t SPI_CTRL1_REG;
  SPI_CTRL1_REG.reserved1 = 0;
  SPI_CTRL1_REG.SPI_CS_HOLD_DELAY = 5;
  dwrite(SPI.SPI_CTRL1_REG, SPI_CTRL1_REG);

  SPI.SPI_RD_STATUS_REG = 0;

  spi_ctrl2_reg_t SPI_CTRL2_REG;
  SPI_CTRL2_REG.SPI_SETUP_TIME = 1;
  SPI_CTRL2_REG.SPI_HOLD_TIME = 1;
  SPI_CTRL2_REG.reserved1 = 0;
  SPI_CTRL2_REG.SPI_CK_OUT_HIGH_MODE = 0;
  SPI_CTRL2_REG.SPI_MISO_DELAY_MODE = 0;
  SPI_CTRL2_REG.SPI_MISO_DELAY_NUM = 0;
  SPI_CTRL2_REG.SPI_MOSI_DELAY_MODE = 0;
  SPI_CTRL2_REG.SPI_MOSI_DELAY_NUM = 0;
  SPI_CTRL2_REG.SPI_CS_DELAY_MODE = 0;
  SPI_CTRL2_REG.SPI_CS_DELAY_NUM = 0;
  dwrite(SPI.SPI_CTRL2_REG, SPI_CTRL2_REG);

  spi_clock_reg_t SPI_CLOCK_REG;
  SPI_CLOCK_REG.SPI_CLKCNT_L = 3;
  SPI_CLOCK_REG.SPI_CLKCNT_H = 1;
  SPI_CLOCK_REG.SPI_CLKCNT_N = 3;
  SPI_CLOCK_REG.SPI_CLKDIV_PRE = 0;
  SPI_CLOCK_REG.SPI_CLK_EQU_SYSCLK = 1;
  dwrite(SPI.SPI_CLOCK_REG, SPI_CLOCK_REG);

  spi_user_reg_t SPI_USER_REG;
  SPI_USER_REG.SPI_DOUTDIN = 0;
  SPI_USER_REG.reserved1 = 0;
  SPI_USER_REG.SPI_CS_HOLD = 0;
  SPI_USER_REG.SPI_CS_SETUP = 0;
  SPI_USER_REG.SPI_CK_I_EDGE = 1;
  SPI_USER_REG.SPI_CK_OUT_EDGE = 0;
  SPI_USER_REG.reserved2 = 0;
  SPI_USER_REG.SPI_RD_BYTE_ORDER = 0;
  SPI_USER_REG.SPI_WR_BYTE_ORDER = 0;
  SPI_USER_REG.SPI_FWRITE_DUAL = 0;
  SPI_USER_REG.SPI_FWRITE_QUAD = 0;
  SPI_USER_REG.SPI_FWRITE_DIO = 0;
  SPI_USER_REG.SPI_FWRITE_QIO = 0;
  SPI_USER_REG.SPI_SIO = 0;
  SPI_USER_REG.reserved3 = 0;
  SPI_USER_REG.SPI_USR_MISO_HIGHPART = 0;
  SPI_USER_REG.SPI_USR_MOSI_HIGHPART = 0;
  SPI_USER_REG.SPI_USR_DUMMY_IDLE = 0;
  SPI_USER_REG.SPI_USR_MOSI = 0;
  SPI_USER_REG.SPI_USR_MISO = 0;
  SPI_USER_REG.SPI_USR_DUMMY = 0;
  SPI_USER_REG.SPI_USR_ADDR = 0;
  SPI_USER_REG.SPI_USR_COMMAND = 1;
  dwrite(SPI.SPI_USER_REG, SPI_USER_REG);

  SPI.SPI_USER1_REG.SPI_USR_DUMMY_CYCLELEN = 0;

  spi_user2_reg_t SPI_USER2_REG;
  SPI_USER2_REG.SPI_USR_COMMAND_VALUE = 0;
  SPI_USER2_REG.reserved1 = 0;
  SPI_USER2_REG.SPI_USR_COMMAND_BITLEN = 7;
  dwrite(SPI.SPI_USER2_REG, SPI_USER2_REG);

  spi_mosi_dlen_reg_t SPI_MOSI_DLEN_REG;
  SPI_MOSI_DLEN_REG.SPI_USR_MOSI_DBITLEN = 0;
  SPI_MOSI_DLEN_REG.reserved1 = 0;
  dwrite(SPI.SPI_MOSI_DLEN_REG, SPI_MOSI_DLEN_REG);

  spi_miso_dlen_reg_t SPI_MISO_DLEN_REG;
  SPI_MISO_DLEN_REG.SPI_USR_MISO_DBITLEN = 0;
  SPI_MISO_DLEN_REG.reserved1 = 0;
  dwrite(SPI.SPI_MISO_DLEN_REG, SPI_MISO_DLEN_REG);

  SPI.SPI_SLV_WR_STATUS_REG = 0;

  spi_pin_reg_t SPI_PIN_REG;
  SPI_PIN_REG.SPI_CS0_DIS = 0;
  SPI_PIN_REG.SPI_CS1_DIS = 1;
  SPI_PIN_REG.SPI_CS2_DIS = 1;
  SPI_PIN_REG.reserved1 = 0;
  SPI_PIN_REG.SPI_CK_DIS = 0;
  SPI_PIN_REG.SPI_MASTER_CS_POL = 0;
  SPI_PIN_REG.reserved2 = 0;
  SPI_PIN_REG.SPI_MASTER_CK_SEL = 0;
  SPI_PIN_REG.reserved3 = 0;
  SPI_PIN_REG.SPI_CK_IDLE_EDGE = 0;
  SPI_PIN_REG.SPI_CS_KEEP_ACTIVE = 0;
  SPI_PIN_REG.reserved4 = 0;
  dwrite(SPI.SPI_PIN_REG, SPI_PIN_REG);

  spi_slave_reg_t SPI_SLAVE_REG;
  SPI_SLAVE_REG.SPI_SLV_RD_BUF_DONE = 0;
  SPI_SLAVE_REG.SPI_SLV_WR_BUF_DONE = 0;
  SPI_SLAVE_REG.SPI_SLV_RD_STA_DONE = 0;
  SPI_SLAVE_REG.SPI_SLV_WR_STA_DONE = 0;
  SPI_SLAVE_REG.SPI_TRANS_DONE = 0;
  SPI_SLAVE_REG.SPI_SLV_RD_BUF_INTEN = 0;
  SPI_SLAVE_REG.SPI_SLV_WR_BUF_INTEN = 0;
  SPI_SLAVE_REG.SPI_SLV_RD_STA_INTEN = 0;
  SPI_SLAVE_REG.SPI_SLV_WR_STA_INTEN = 0;
  SPI_SLAVE_REG.SPI_TRANS_INTEN = 0;
  SPI_SLAVE_REG.SPI_CS_I_MODE = 0;
  SPI_SLAVE_REG.reserved1 = 0;
  SPI_SLAVE_REG.SPI_SLV_LAST_COMMAND = 0;
  SPI_SLAVE_REG.SPI_SLV_LAST_STATE = 0;
  SPI_SLAVE_REG.SPI_TRANS_CNT = 0;
  SPI_SLAVE_REG.SPI_SLV_CMD_DEFINE = 0;
  SPI_SLAVE_REG.SPI_SLV_WR_RD_STA_EN = 0;
  SPI_SLAVE_REG.SPI_SLV_WR_RD_BUF_EN = 0;
  SPI_SLAVE_REG.SPI_SLAVE_MODE = 0;
  SPI_SLAVE_REG.SPI_SYNC_RESET = 0;
  dwrite(SPI.SPI_SLAVE_REG, SPI_SLAVE_REG);

  spi_slave1_reg_t SPI_SLAVE1_REG;
  SPI_SLAVE1_REG.SPI_SLV_RDBUF_DUMMY_EN = 0;
  SPI_SLAVE1_REG.SPI_SLV_WRBUF_DUMMY_EN = 0;
  SPI_SLAVE1_REG.SPI_SLV_RDSTA_DUMMY_EN = 0;
  SPI_SLAVE1_REG.SPI_SLV_WRSTA_DUMMY_EN = 0;
  SPI_SLAVE1_REG.SPI_SLV_WR_ADDR_BITLEN = 0;
  SPI_SLAVE1_REG.SPI_SLV_RD_ADDR_BITLEN = 0;
  SPI_SLAVE1_REG.reserved1 = 0;
  SPI_SLAVE1_REG.SPI_SLV_STATUS_READBACK = 1;
  SPI_SLAVE1_REG.SPI_SLV_STATUS_FAST_EN = 0;
  SPI_SLAVE1_REG.SPI_SLV_STATUS_BITLEN = 0;
  dwrite(SPI.SPI_SLAVE1_REG, SPI_SLAVE1_REG);

  spi_slave2_reg_t SPI_SLAVE2_REG;
  SPI_SLAVE2_REG.SPI_SLV_RDSTA_DUMMY_CYCLELEN = 0;
  SPI_SLAVE2_REG.SPI_SLV_WRSTA_DUMMY_CYCLELEN = 0;
  SPI_SLAVE2_REG.SPI_SLV_RDBUF_DUMMY_CYCLELEN = 0;
  SPI_SLAVE2_REG.SPI_SLV_WRBUF_DUMMY_CYCLELEN = 0;
  dwrite(SPI.SPI_SLAVE2_REG, SPI_SLAVE2_REG);

  spi_slave3_reg_t SPI_SLAVE3_REG;
  SPI_SLAVE3_REG.SPI_SLV_RDBUF_CMD_VALUE = 0;
  SPI_SLAVE3_REG.SPI_SLV_WRBUF_CMD_VALUE = 0;
  SPI_SLAVE3_REG.SPI_SLV_RDSTA_CMD_VALUE = 0;
  SPI_SLAVE3_REG.SPI_SLV_WRSTA_CMD_VALUE = 0;
  dwrite(SPI.SPI_SLAVE3_REG, SPI_SLAVE3_REG);

  spi_slv_wrbuf_dlen_reg_t SPI_SLV_WRBUF_DLEN_REG;
  SPI_SLV_WRBUF_DLEN_REG.SPI_SLV_WRBUF_DBITLEN = 0;
  SPI_SLV_WRBUF_DLEN_REG.reserved1 = 0;
  dwrite(SPI.SPI_SLV_WRBUF_DLEN_REG, SPI_SLV_WRBUF_DLEN_REG);

  spi_slv_rdbuf_dlen_reg_t SPI_SLV_RDBUF_DLEN_REG;
  SPI_SLV_RDBUF_DLEN_REG.SPI_SLV_RDBUF_DBITLEN = 0;
  SPI_SLV_RDBUF_DLEN_REG.reserved1 = 0;
  dwrite(SPI.SPI_SLV_RDBUF_DLEN_REG, SPI_SLV_RDBUF_DLEN_REG);

  spi_slv_rd_bit_reg_t SPI_SLV_RD_BIT_REG;
  SPI_SLV_RD_BIT_REG.SPI_SLV_RDATA_BIT = 0;
  SPI_SLV_RD_BIT_REG.reserved1 = 0;
  dwrite(SPI.SPI_SLV_RD_BIT_REG, SPI_SLV_RD_BIT_REG);

  for (uint32_t n = 0; n < 16; n++)
    SPI.SPI_W_REG[n] = 0;

  SPI.SPI_TX_CRC_REG = 0;

  //SPI.SPI_EXT2_REG.SPI_ST = 0;
  SPI.SPI_EXT2_REG.reserved1 = 0;

  spi_dma_conf_reg_t SPI_DMA_CONF_REG;
  SPI_DMA_CONF_REG.reserved1 = 0;
  SPI_DMA_CONF_REG.SPI_IN_RST = 0;
  SPI_DMA_CONF_REG.SPI_OUT_RST = 0;
  SPI_DMA_CONF_REG.SPI_AHBM_FIFO_RST = 0;
  SPI_DMA_CONF_REG.SPI_AHBM_RST = 0;
  SPI_DMA_CONF_REG.reserved2 = 0;
  SPI_DMA_CONF_REG.SPI_OUT_EOF_MODE = 1;
  SPI_DMA_CONF_REG.SPI_OUTDSCR_BURST_EN = 0;
  SPI_DMA_CONF_REG.SPI_INDSCR_BURST_EN = 0;
  SPI_DMA_CONF_REG.SPI_OUT_DATA_BURST_EN = 0;
  SPI_DMA_CONF_REG.reserved3 = 0;
  SPI_DMA_CONF_REG.SPI_DMA_RX_STOP = 0;
  SPI_DMA_CONF_REG.SPI_DMA_TX_STOP = 0;
  SPI_DMA_CONF_REG.SPI_DMA_CONTINUE = 0;
  SPI_DMA_CONF_REG.reserved4 = 0;
  dwrite(SPI.SPI_DMA_CONF_REG, SPI_DMA_CONF_REG);

  spi_dma_out_link_reg_t SPI_DMA_OUT_LINK_REG;
  SPI_DMA_OUT_LINK_REG.SPI_OUTLINK_ADDR = 0;
  SPI_DMA_OUT_LINK_REG.reserved1 = 0;
  SPI_DMA_OUT_LINK_REG.SPI_OUTLINK_STOP = 0;
  SPI_DMA_OUT_LINK_REG.SPI_OUTLINK_START = 0;
  SPI_DMA_OUT_LINK_REG.SPI_OUTLINK_RESTART = 0;
  SPI_DMA_OUT_LINK_REG.reserved2 = 0;
  dwrite(SPI.SPI_DMA_OUT_LINK_REG, SPI_DMA_OUT_LINK_REG);

  spi_dma_in_link_reg_t SPI_DMA_IN_LINK_REG;
  SPI_DMA_IN_LINK_REG.SPI_INLINK_ADDR = 0;
  SPI_DMA_IN_LINK_REG.SPI_INLINK_AUTO_RET = 0;
  SPI_DMA_IN_LINK_REG.reserved1 = 0;
  SPI_DMA_IN_LINK_REG.SPI_INLINK_STOP = 0;
  SPI_DMA_IN_LINK_REG.SPI_INLINK_START = 0;
  SPI_DMA_IN_LINK_REG.SPI_INLINK_RESTART = 0;
  SPI_DMA_IN_LINK_REG.reserved2 = 0;
  dwrite(SPI.SPI_DMA_IN_LINK_REG, SPI_DMA_IN_LINK_REG);

  SPI.SPI_DMA_STATUS_REG.reserved1 = 0;

  spi_dma_int_ena_reg_t SPI_DMA_INT_ENA_REG;
  SPI_DMA_INT_ENA_REG.SPI_INLINK_DSCR_EMPTY_INT_ENA = 0;
  SPI_DMA_INT_ENA_REG.SPI_OUTLINK_DSCR_ERROR_INT_ENA = 0;
  SPI_DMA_INT_ENA_REG.SPI_INLINK_DSCR_ERROR_INT_ENA = 0;
  SPI_DMA_INT_ENA_REG.SPI_IN_DONE_INT_ENA = 0;
  SPI_DMA_INT_ENA_REG.SPI_IN_ERR_EOF_INT_ENA = 0;
  SPI_DMA_INT_ENA_REG.SPI_IN_SUC_EOF_INT_ENA = 0;
  SPI_DMA_INT_ENA_REG.SPI_OUT_DONE_INT_ENA = 0;
  SPI_DMA_INT_ENA_REG.SPI_OUT_EOF_INT_ENA = 0;
  SPI_DMA_INT_ENA_REG.SPI_OUT_TOTAL_EOF_INT_ENA = 0;
  SPI_DMA_INT_ENA_REG.reserved1 = 0;
  dwrite(SPI.SPI_DMA_INT_ENA_REG, SPI_DMA_INT_ENA_REG);

  SPI.SPI_DMA_INT_RAW_REG.reserved1 = 0;

  SPI.SPI_DMA_INT_ST_REG.reserved1 = 0;

  SPI.SPI_DMA_RSTATUS_REG.reserved1 = 0;
  SPI.SPI_DMA_TSTATUS_REG.reserved1 = 0;
}

#ifdef HAL_SPI_SUPPORTS_ASYNC

struct spi_async_process_t {
  volatile spi_dev_t *current_spibus;
  const void *current_buffer;
  size_t curoff_bytes;
  size_t txlen;
  uint8_t txunitsize;
  void (*completeCallback)(void*);
  void *complete_ud;
  bool is_active = false;
};
static volatile spi_async_process_t _current_spi_proc;

#ifdef HALSPI_ESP32_ENABLE_INTERNBUS
static intr_handle_t _spi0_interrupt;
static intr_handle_t _spi1_interrupt;
#endif
static intr_handle_t _spi2_interrupt;
static intr_handle_t _spi3_interrupt;

#ifdef HALSPI_DEBUG
static int _cpu_core_id = -1;
#endif

static void IRAM_ATTR spi_async_fill_buffer(volatile spi_async_process_t& proc) {
#ifdef HALSPI_DEBUG
  if (_cpu_core_id == -1)
    _cpu_core_id = xPortGetCoreID();
  else if (_cpu_core_id != xPortGetCoreID())
    _spi_on_error(13);
#endif

  auto& SPI = *proc.current_spibus;

  bool has_prepared_spibus = false;
  uint16_t writecnt_bytes = 0;

  if (proc.txunitsize == 1) {
    SPIWriteDataToTransferIsolated(SPI, (const uint8_t*)proc.current_buffer, proc.txlen, proc.curoff_bytes, writecnt_bytes);
    has_prepared_spibus = true;
  }
  else if (proc.txunitsize == 2) {
    SPIWriteDataToTransferIsolated(SPI, (const uint16_t*)proc.current_buffer, proc.txlen, proc.curoff_bytes, writecnt_bytes);
    has_prepared_spibus = true;
  }
  else if (proc.txunitsize == 4) {
    SPIWriteDataToTransferIsolated(SPI, (const uint32_t*)proc.current_buffer, proc.txlen, proc.curoff_bytes, writecnt_bytes);
    has_prepared_spibus = true;
  }

  if (has_prepared_spibus == false || writecnt_bytes == 0) {
  #ifdef HALSPI_DEBUG
    if (proc.curoff_bytes < proc.txlen * proc.txunitsize) {
      _spi_on_error(12);
    }
  #endif

    // Disable the interrupt.
    SPI.SPI_SLAVE_REG.SPI_TRANS_INTEN = false;

    auto cb = proc.completeCallback;
    auto ud = proc.complete_ud;

    proc.current_spibus = nullptr;
    proc.current_buffer = nullptr;
    proc.curoff_bytes = 0;
    proc.txlen = 0;
    proc.txunitsize = 0;
    proc.completeCallback = nullptr;
    proc.complete_ud = nullptr;
    proc.is_active = false;

    // Call any completion handler, if provided by the user.
    if (cb) {
      cb(ud);
    }
  }
  else {
    proc.curoff_bytes += writecnt_bytes;

    uint32_t txcount_bits = ( writecnt_bytes * 8 );

    if (SPI.SPI_USER_REG.SPI_USR_MOSI == true) {
      SPI.SPI_MOSI_DLEN_REG.SPI_USR_MOSI_DBITLEN = txcount_bits - 1;
    }
    if (SPI.SPI_USER_REG.SPI_USR_MISO == true) {
      SPI.SPI_MISO_DLEN_REG.SPI_USR_MISO_DBITLEN = txcount_bits - 1;
    }

    // Kick-off another async SPI transfer.
    SPI.SPI_CMD_REG.SPI_USR = true;
  }
}

static void IRAM_ATTR spi_async_process_isr(void *ud, uint32_t spibusIdx) {
  auto& SPI = SPIGetBusFromIndex(spibusIdx);

  // Check the type of the triggered interrupt.
  if (SPI.SPI_SLAVE_REG.SPI_TRANS_DONE) {
    // Clear the interrupt.
    SPI.SPI_SLAVE_REG.SPI_TRANS_DONE = false;

    auto& proc = _current_spi_proc;

    if (proc.is_active && proc.current_spibus == &SPI) {
      // Is the SPI bus ready? Otherwise we could have a spurious interrupt call.
      if (SPI.SPI_CMD_REG.SPI_USR == false) {
        spi_async_fill_buffer(proc);
      }
    }
  }
}

#ifdef HALSPI_ESP32_ENABLE_INTERNBUS
static void IRAM_ATTR spi_async_process_isr_spibus0(void *ud) {
  spi_async_process_isr(ud, 0);
}
static void IRAM_ATTR spi_async_process_isr_spibus1(void *ud) {
  spi_async_process_isr(ud, 1);
}
#endif
static void IRAM_ATTR spi_async_process_isr_spibus2(void *ud) {
  spi_async_process_isr(ud, 2);
}
static void IRAM_ATTR spi_async_process_isr_spibus3(void *ud) {
  spi_async_process_isr(ud, 3);
}

static void SPIInstallAsync(volatile spi_dev_t& SPI, intr_handle_t& handleOut) {
  int intsrc = -1;
  void (*inthandler)(void*) = nullptr;

#ifdef HALSPI_ESP32_ENABLE_INTERNBUS
  if (&SPI == &SPI0) {
    intsrc = ETS_SPI0_INTR_SOURCE;
    inthandler = spi_async_process_isr_spibus0;
  }
  else if (&SPI == &SPI1) {
    intsrc = ETS_SPI1_INTR_SOURCE;
    inthandler = spi_async_process_isr_spibus1;
  }
  else
#endif
  if (&SPI == &SPI2) {
    intsrc = ETS_SPI2_INTR_SOURCE;
    inthandler = spi_async_process_isr_spibus2;
  }
  else if (&SPI == &SPI3) {
    intsrc = ETS_SPI3_INTR_SOURCE;
    inthandler = spi_async_process_isr_spibus3;
  }
  else
    _spi_on_error(1);

  esp_err_t err = esp_intr_alloc(intsrc, 0 /*ESP_INTR_FLAG_IRAM*/, inthandler, nullptr, &handleOut);

  if (err != ESP_OK)
    _spi_on_error(1);
}

static void __attribute__((unused)) SPIUninstallAsync(intr_handle_t handle) {
  // Unregister the ISR.
  esp_err_t err = esp_intr_free(handle);

  if (err != ESP_OK)
    _spi_on_error(2);
}

static void SPIAsyncInitialize() {
#ifdef HALSPI_ESP32_ENABLE_INTERNBUS
  SPIInstallAsync(SPI0, _spi0_interrupt);
  SPIInstallAsync(SPI1, _spi1_interrupt);
#endif
  SPIInstallAsync(SPI2, _spi2_interrupt);
  SPIInstallAsync(SPI3, _spi3_interrupt);
}

static void SPIStartRawAsync(volatile spi_dev_t& SPI, const void *buf, uint32_t txlen, uint8_t txunitsize, void (*completeCallback)(void*), void *ud) {
  auto& proc = _current_spi_proc;

  spi_monitored_loop asyncw;
  while (proc.is_active) { asyncw.update(2); /* wait for any async process to conclude before we start another */ }

  cli();

  proc.current_spibus = &SPI;
  proc.current_buffer = buf;
  proc.curoff_bytes = 0;
  proc.txlen = txlen;
  proc.txunitsize = txunitsize;
  proc.completeCallback = completeCallback;
  proc.complete_ud = ud;
  proc.is_active = true;

  sei();

  // Enable the interrupt (make sure that we do not trigger it over here already).
  SPI.SPI_SLAVE_REG.SPI_TRANS_DONE = false;
  SPI.SPI_SLAVE_REG.SPI_TRANS_INTEN = true;

  // Kick it off.
  spi_async_fill_buffer(proc);
}

static void SPIAbortRawAsync() {
  cli();

  auto& proc = _current_spi_proc;

  if (proc.is_active) {
    proc.is_active = false;

    auto& SPI = *proc.current_spibus;

    SPI.SPI_SLAVE_REG.SPI_TRANS_INTEN = false;
    SPI.SPI_SLAVE_REG.SPI_TRANS_DONE = false;
  }

  sei();
}

#endif //HAL_SPI_SUPPORTS_ASYNC

#ifndef HALSPI_DISABLE_DMA

#define SPIDMA_OWNER_CPU 0
#define SPIDMA_OWNER_DMAC 1

// Has to be allocated inside the DMA-accessible memory range.
// page 113 of ESP32 TRM.
struct dma_descriptor_t {
  // Configuration bits.
  uint32_t size : 12;
  uint32_t length : 12;
  uint32_t reserved : 5;
  uint32_t sosf : 1; // start-of-sub-frame (undocumented)
  uint32_t eof : 1;
  uint32_t owner : 1;

  void *address;
  volatile dma_descriptor_t *next;
};
static_assert(sizeof(dma_descriptor_t) == 12, "invalid size of ESP32 dma_descriptor_t");

#ifndef HALSPI_ESP32_DMADESC_COUNT
  // The default count of DMA descriptors usable.
  #define HALSPI_ESP32_DMADESC_COUNT 1
#endif

#ifdef HALSPI_ESP32_STATIC_DMADESCS
static volatile dma_descriptor_t _usable_dma_descs_static[HALSPI_ESP32_DMADESC_COUNT];
#else
static volatile dma_descriptor_t *_usable_dma_descs_dynamic = nullptr;
static uint32_t _usable_dma_descs_count = 0;
#endif

// Specifies the valid memory range for DMA descriptors (not specified if necessary for DMA buffers).

// For ESP32 basic.
#define ESP32_DMADESC_HWPTR(ptr) ((uint32_t)ptr - 0x3FF00000)

inline bool DMAIsValidPointer(volatile const void *ptr) noexcept {
  size_t addr = (size_t)ptr;

  return ( addr >= SOC_DMA_LOW && addr <= SOC_DMA_HIGH );
}

inline bool DMAIsValidDescriptor(volatile dma_descriptor_t *desc) noexcept {
  return DMAIsValidPointer(desc);
}

inline void DMAInitializeMachine() {
#ifdef HALSPI_ESP32_STATIC_DMADESCS
  for (auto& desc : _usable_dma_descs_static) {
    if (DMAIsValidDescriptor(&desc) == false) {
      _spi_on_error(3);
    }
    desc.owner = SPIDMA_OWNER_CPU;
    desc.reserved = 0;
    desc.sosf = false;
  }
#else
  void *dmabuf = heap_caps_malloc( sizeof(dma_descriptor_t)*HALSPI_ESP32_DMADESC_COUNT, MALLOC_CAP_DMA );
  if (dmabuf == nullptr)
    _spi_on_error(3);
  _usable_dma_descs_dynamic = (volatile dma_descriptor_t*)dmabuf;
  _usable_dma_descs_count = HALSPI_ESP32_DMADESC_COUNT;
  if (DMAIsValidDescriptor(_usable_dma_descs_dynamic) == false)
    _spi_on_error(3);
  for (uint32_t n = 0; n < _usable_dma_descs_count; n++) {
    auto& desc = _usable_dma_descs_dynamic[n];

    desc.owner = SPIDMA_OWNER_CPU;
    desc.reserved = 0;
    desc.sosf = false;
  }
#endif
}

/* Important restriction on DMA-usable memory addresses and their buffer sizes:
    Please note that the buffer address pointer field in in_link descriptors should be word-aligned, and the size field (...).
  -> each DMA-depending Marlin operation will need to use a special memory allocation function that ensures DMA-capability!
  -> each DMA-internal memory buffer accepting function has to deny the buffer if it does not match
     the requirements!
*/
inline bool DMAIsValidWriteDataBuffer(const void *buf, size_t bufsz) noexcept {
  if (DMAIsValidPointer(buf) == false) return false;

  size_t bufaddr = (size_t)buf;

  if (bufaddr % sizeof(uint32_t) != 0) return false;
  if (bufsz % sizeof(uint32_t) != 0) return false;

  return true;
}
inline bool DMAIsValidReadDataBuffer(const void *buf) noexcept {
  if (DMAIsValidPointer(buf) == false) return false;

  size_t bufaddr = (size_t)buf;

  if (bufaddr % sizeof(uint32_t) != 0) return false;

  return true;
}

// Not every SPI bus is DMA-capable.
inline bool DMAIsCapableSPIBus(volatile spi_dev_t& SPI) {
#ifdef HALSPI_ENABLE_INTERNBUS
  if (&SPI == &SPI1) return true;
#endif
  if (&SPI == &SPI2) return true;
  if (&SPI == &SPI3) return true;
  return false;
}

struct dma_process_t {
  const void *current_buffer;
  size_t bufsize;
  size_t txlen;
  size_t curoff;
  bool is_active;
};

static volatile dma_process_t dma_current_process;

static void DMABusInitialize(volatile spi_dev_t& SPI) {
#ifdef HALSPI_DEBUG
  if (DMAIsCapableSPIBus(SPI) == false)
    _spi_on_error(4);
#endif

  // Reset and enable the SPI DMA clock.
  DPORT_PERIP_RST_EN_REG.DPORT_SPI_DMA_RST = true;
  DPORT_PERIP_RST_EN_REG.DPORT_SPI_DMA_RST = false;
  DPORT_PERIP_CLK_EN_REG.DPORT_SPI_DMA_CLK_EN = true;

  // Select the SPI DMA channel.
  auto spibusIdx = SPIGetBusIndex(SPI);

  unsigned int dma_chansel =
#ifdef HALSPI_ESP32_DMA_SELECT_CHAN1
    DPORT_SPI_DMA_CHAN_SEL_CHAN1
#elif defined(HALSPI_ESP32_DMA_SELECT_CHAN2)
    DPORT_SPI_DMA_CHAN_SEL_CHAN2
#else
    DPORT_SPI_DMA_CHAN_SEL_CHAN1
#endif
    ;
#ifdef HALSPI_ESP32_ENABLE_INTERNBUS
  if (spibusIdx == 1) {
    DPORT_SPI_DMA_CHAN_SEL_REG.DPORT_SPI_SPI1_DMA_CHAN_SEL = dma_chansel;
  }
  else
#endif
  if (spibusIdx == 2) {
    DPORT_SPI_DMA_CHAN_SEL_REG.DPORT_SPI_SPI2_DMA_CHAN_SEL = dma_chansel;
  }
  else if (spibusIdx == 3) {
    DPORT_SPI_DMA_CHAN_SEL_REG.DPORT_SPI_SPI3_DMA_CHAN_SEL = dma_chansel;
  }
}

static void DMABusShutdown(volatile spi_dev_t& SPI) {
  // Unselect the SPI DMA channel.
  auto spibusIdx = SPIGetBusIndex(SPI);

#ifdef HALSPI_ESP32_ENABLE_INTERNBUS
  if (spibusIdx == 1) {
    DPORT_SPI_DMA_CHAN_SEL_REG.DPORT_SPI_SPI1_DMA_CHAN_SEL = DPORT_SPI_DMA_CHAN_SEL_NONE;
  }
  else
#endif
  if (spibusIdx == 2) {
    DPORT_SPI_DMA_CHAN_SEL_REG.DPORT_SPI_SPI2_DMA_CHAN_SEL = DPORT_SPI_DMA_CHAN_SEL_NONE;
  }
  else if (spibusIdx == 3) {
    DPORT_SPI_DMA_CHAN_SEL_REG.DPORT_SPI_SPI3_DMA_CHAN_SEL = DPORT_SPI_DMA_CHAN_SEL_NONE;
  }

  // Disable the SPI DMA clock.
  DPORT_PERIP_CLK_EN_REG.DPORT_SPI_DMA_CLK_EN = false;
}

// TODO: manage list of free descriptors to speed up the process of generating a DMA chain.

static volatile dma_descriptor_t* DMAGetFreeDescriptor() noexcept {
#ifdef HALSPI_ESP32_STATIC_DMADESCS
  for (auto& desc : _usable_dma_descs_static) {
#else
  for (uint32_t _n = 0; _n < _usable_dma_descs_count; _n++) {
    auto& desc = _usable_dma_descs_dynamic[_n];
#endif
    if (desc.owner == SPIDMA_OWNER_CPU) {
#ifdef HALSPI_DEBUG
      if (DMAIsValidDescriptor(&desc) == false)
        _spi_on_error(5);
#endif

      return &desc;
    }
  }
  return nullptr;
}

static volatile dma_descriptor_t* DMAGenerateAcquireChain(volatile dma_process_t& proc) noexcept {
  volatile dma_descriptor_t *startdesc = nullptr;
  volatile dma_descriptor_t *chainend = nullptr;

  while (proc.curoff < proc.txlen) {
    volatile dma_descriptor_t *freedesc = DMAGetFreeDescriptor();

    if (freedesc == nullptr) break;

    size_t left_to_transmit = (proc.txlen - proc.curoff);
    uint32_t txcount = eir::Minimum((1u<<12u)-4u, left_to_transmit);
    uint32_t txbufsz = ALIGN_SIZE(txcount, (uint32_t)4u);

    freedesc->size = txbufsz;
    freedesc->length = txcount;
    freedesc->address = ((uint8_t*)proc.current_buffer + proc.curoff);

    // Advance the process.
    proc.curoff += txcount;

    // Give ownership of the descriptor to the DMA controller.
    // (this decision turns valid as soon as the DMAC is kicked-off!)
    freedesc->owner = SPIDMA_OWNER_DMAC;

    if (chainend != nullptr) {
      chainend->eof = false;
      chainend->next = freedesc;
    }
    chainend = freedesc;

    if (startdesc == nullptr) {
      startdesc = freedesc;
    }
  }

  if (chainend) {
    chainend->eof = true;
    chainend->next = nullptr;
  }

  return startdesc;
}

static void DMASendBlocking(volatile spi_dev_t& SPI, const void *buf, size_t bufsize, size_t txlen) {
  DMABusInitialize(SPI);

  // Reset the DMA state machine.
  {
    SPI.SPI_DMA_CONF_REG.SPI_OUT_RST = true;
    SPI.SPI_DMA_CONF_REG.SPI_IN_RST = true;
    SPI.SPI_DMA_CONF_REG.SPI_AHBM_RST = true;
    SPI.SPI_DMA_CONF_REG.SPI_AHBM_FIFO_RST = true;

    SPI.SPI_DMA_IN_LINK_REG.SPI_INLINK_START = false;
    SPI.SPI_DMA_OUT_LINK_REG.SPI_OUTLINK_START = false;

    SPI.SPI_DMA_CONF_REG.SPI_OUT_RST = false;
    SPI.SPI_DMA_CONF_REG.SPI_IN_RST = false;
    SPI.SPI_DMA_CONF_REG.SPI_AHBM_RST = false;
    SPI.SPI_DMA_CONF_REG.SPI_AHBM_FIFO_RST = false;
  }

  // Configure DMA for the SPI bus.
  spi_dma_conf_reg_t SPI_DMA_CONF_REG;
  SPI_DMA_CONF_REG.SPI_DMA_CONTINUE = false;
  SPI_DMA_CONF_REG.SPI_DMA_TX_STOP = false;
  SPI_DMA_CONF_REG.SPI_DMA_RX_STOP = false;
  SPI_DMA_CONF_REG.SPI_OUT_DATA_BURST_EN = false;
  SPI_DMA_CONF_REG.SPI_INDSCR_BURST_EN = false;
  SPI_DMA_CONF_REG.SPI_OUTDSCR_BURST_EN = false;
  SPI_DMA_CONF_REG.SPI_OUT_EOF_MODE = 1;
  SPI_DMA_CONF_REG.SPI_AHBM_RST = false;
  SPI_DMA_CONF_REG.SPI_AHBM_FIFO_RST = false;
  SPI_DMA_CONF_REG.SPI_OUT_RST = false;
  SPI_DMA_CONF_REG.SPI_IN_RST = false;
  SPI_DMA_CONF_REG.reserved1 = 0;
  SPI_DMA_CONF_REG.reserved2 = 0;
  SPI_DMA_CONF_REG.reserved3 = 0;
  SPI_DMA_CONF_REG.reserved4 = 0;
  dwrite(SPI.SPI_DMA_CONF_REG, SPI_DMA_CONF_REG);

  auto& proc = dma_current_process;
  proc.is_active = true;
  proc.current_buffer = buf;
  proc.bufsize = bufsize;
  proc.txlen = txlen;
  proc.curoff = 0;

#if 0
  static volatile dma_descriptor_t _rxdesc;
  _rxdesc.size = 0;
  _rxdesc.length = 0;
  _rxdesc.reserved = 0;
  _rxdesc.sosf = false;
  _rxdesc.eof = true;
  _rxdesc.owner = SPIDMA_OWNER_DMAC;
  _rxdesc.address = nullptr;
  _rxdesc.next = nullptr;
  {
    // Workaround for SPI DMA hardware bug.
    // (https://www.esp32.com/viewtopic.php?t=8433 spi_master.zip:spi_master.c:line 754)
    // TODO: does it really matter???
    spi_dma_in_link_reg_t SPI_DMA_IN_LINK_REG;
    SPI_DMA_IN_LINK_REG.SPI_INLINK_ADDR = ESP32_DMADESC_HWPTR(&_rxdesc);
    SPI_DMA_IN_LINK_REG.SPI_INLINK_AUTO_RET = false;
    SPI_DMA_IN_LINK_REG.reserved1 = 0;
    SPI_DMA_IN_LINK_REG.SPI_INLINK_STOP = false;
    SPI_DMA_IN_LINK_REG.SPI_INLINK_START = true;
    SPI_DMA_IN_LINK_REG.SPI_INLINK_RESTART = false;
    SPI_DMA_IN_LINK_REG.reserved2 = 0;
    dwrite(SPI.SPI_DMA_IN_LINK_REG, SPI_DMA_IN_LINK_REG);
  }

  SPI.SPI_USER_REG.SPI_USR_MISO = true;
  SPI.SPI_USER_REG.SPI_USR_MOSI = true;
  SPI.SPI_CTRL_REG.SPI_FASTRD_MODE = false;
  SPI.SPI_CTRL_REG.reserved2 = 0;
  SPI.SPI_CTRL2_REG.SPI_SETUP_TIME = 0;
  SPI.SPI_CTRL2_REG.SPI_HOLD_TIME = 0;
  SPI.SPI_USER1_REG.SPI_USR_DUMMY_CYCLELEN = 0;
  SPI.SPI_USER1_REG.SPI_USR_ADDR_BITLEN = 0;
  SPI.SPI_USER2_REG.SPI_USR_COMMAND_BITLEN = 0;
  SPI.SPI_SLAVE1_REG.SPI_SLV_STATUS_READBACK = 0;
  SPI.SPI_DMA_INT_CLR_REG.SPI_INLINK_DSCR_ERROR_INT_CLR = true;

  SERIAL_ECHOLNPGM("RX CHAIN PTR: ", (uint32_t)&_rxdesc);
  SERIAL_ECHOLNPGM("SOC_DMA: ", SOC_DMA_LOW, " to ", SOC_DMA_HIGH);

  SPI.SPI_MOSI_DLEN_REG.SPI_USR_MOSI_DBITLEN = 0u;
  SPI.SPI_MISO_DLEN_REG.SPI_USR_MISO_DBITLEN = 0u;
#endif

#if 0
  // DUMP MACHINE STATE.
  {
    static bool _dumped = false;

    if (_dumped == false) {
      SPI.serial_dump();
      _dumped = true;
    }
  }
#endif

  //OUT_WRITE(BEEPER_PIN, HIGH);

  // Is there data left to send?
  while (proc.curoff < proc.txlen) {
    // Generate a transfer chain.
    volatile dma_descriptor_t *chain = DMAGenerateAcquireChain(proc);

    if (chain == nullptr)
      _spi_on_error(14);

    //SERIAL_ECHOLNPGM("TX CHAIN PTR: ", (uint32_t)chain);
    //SERIAL_ECHOLNPGM("CHAIN TXCNT: ", chain->length);
    //SERIAL_ECHOLNPGM("CHAIN SIZE: ", chain->size);

    // Configure the transfer.
    spi_dma_out_link_reg_t SPI_DMA_OUT_LINK_REG;
    SPI_DMA_OUT_LINK_REG.SPI_OUTLINK_ADDR = ESP32_DMADESC_HWPTR(chain);//(int)chain & 0xFFFFF;
    SPI_DMA_OUT_LINK_REG.reserved1 = 0;
    SPI_DMA_OUT_LINK_REG.SPI_OUTLINK_STOP = false;
    SPI_DMA_OUT_LINK_REG.SPI_OUTLINK_START = true;  // cleared by hardware on TX termination.
    SPI_DMA_OUT_LINK_REG.SPI_OUTLINK_RESTART = false;
    SPI_DMA_OUT_LINK_REG.reserved2 = 0;
    dwrite(SPI.SPI_DMA_OUT_LINK_REG, SPI_DMA_OUT_LINK_REG);

#if 0
    SPI.SPI_DMA_IN_LINK_REG.SPI_INLINK_ADDR = ESP32_DMADESC_HWPTR(chain); // ???
    SPI.SPI_DMA_IN_LINK_REG.SPI_INLINK_START = true;
#elif 0
  {
    // Workaround for SPI DMA hardware bug.
    // (https://www.esp32.com/viewtopic.php?t=8433 spi_master.zip:spi_master.c:line 754)
    // TODO: does it really matter???
    spi_dma_in_link_reg_t SPI_DMA_IN_LINK_REG;
    SPI_DMA_IN_LINK_REG.SPI_INLINK_ADDR = ESP32_DMADESC_HWPTR(chain);
    SPI_DMA_IN_LINK_REG.SPI_INLINK_AUTO_RET = false;
    SPI_DMA_IN_LINK_REG.reserved1 = 0;
    SPI_DMA_IN_LINK_REG.SPI_INLINK_STOP = false;
    SPI_DMA_IN_LINK_REG.SPI_INLINK_START = true;
    SPI_DMA_IN_LINK_REG.SPI_INLINK_RESTART = false;
    SPI_DMA_IN_LINK_REG.reserved2 = 0;
    dwrite(SPI.SPI_DMA_IN_LINK_REG, SPI_DMA_IN_LINK_REG);
  }
#endif

    //SPI.SPI_DMA_RSTATUS_REG.serial_dump();
    //SPI.SPI_DMA_TSTATUS_REG.serial_dump();

    //SERIAL_ECHOLNPGM("CUR INLINK DESCR PTR: ", SPI.SPI_INLINK_DSCR_REG);
    //SERIAL_ECHOLNPGM("CUR OUTLINK DESCR PTR: ", SPI.SPI_OUTLINK_DSCR_REG);
    //SERIAL_ECHOLNPGM("AFTER WRITE: ", SPI.SPI_DMA_OUT_LINK_REG.SPI_OUTLINK_ADDR);
    //SERIAL_ECHOLNPGM("TX DMA STATUS (kickoff): ", SPI.SPI_DMA_STATUS_REG.SPI_DMA_TX_EN);
    //SERIAL_ECHOLNPGM("RX DMA STATUS (kickoff): ", SPI.SPI_DMA_STATUS_REG.SPI_DMA_RX_EN);

    //SERIAL_ECHOLNPGM("SPI STATE MACHINE ST (kickoff): ", SPI.SPI_EXT2_REG.SPI_ST);

    // Kick it off.
    SPI.SPI_CMD_REG.SPI_USR = true;

    /* wait until DMA transfer has finished */
    spi_monitored_loop usrw;
#if 0
    while (SPI.SPI_DMA_RSTATUS_REG.TX_FIFO_EMPTY == false) {
      usrw.update(3);
    }
#endif
    while (SPI.SPI_DMA_STATUS_REG.SPI_DMA_TX_EN) {
      usrw.update(3);
    }
    while (SPI.SPI_CMD_REG.SPI_USR) {
      usrw.update(3);
    }

#if 0
    if (SPI.SPI_DMA_INT_RAW_REG.SPI_OUTLINK_DSCR_ERROR_INT_RAW) {
      _spi_on_error(20);
    }
    if (SPI.SPI_DMA_INT_RAW_REG.SPI_INLINK_DSCR_ERROR_INT_RAW) {
      //_spi_on_error(21);
      SPI.SPI_DMA_INT_CLR_REG.SPI_INLINK_DSCR_ERROR_INT_CLR = true;
    }
    if (SPI.SPI_DMA_INT_RAW_REG.SPI_IN_ERR_EOF_INT_RAW) {
      _spi_on_error(22);
    }
    if (SPI.SPI_DMA_INT_RAW_REG.SPI_INLINK_DSCR_EMPTY_INT_RAW) {
      _spi_on_error(23);
    }
#endif

    //SERIAL_ECHOLNPGM("CHAIN LENGTH (after tx): ", chain->length);

    // Free the DMA chain back to CPU ownership, since that is unfortunately NOT done by hardware!
    while (chain) {
      chain->owner = SPIDMA_OWNER_CPU;
      chain = chain->next;
    }
  }

  //SERIAL_ECHOLNPGM("AFTER OP: ", SPI.SPI_DMA_OUT_LINK_REG.SPI_OUTLINK_ADDR);
  //SERIAL_ECHOLNPGM("RSTATUS ADDR: ", SPI.SPI_DMA_RSTATUS_REG.TX_DES_ADDRESS);
  //SERIAL_ECHOLNPGM("RSTATUS FIFO EMPTY: ", SPI.SPI_DMA_RSTATUS_REG.TX_FIFO_EMPTY);
  //SERIAL_ECHOLNPGM("RSTATUS FIFO FULL: ", SPI.SPI_DMA_RSTATUS_REG.TX_FIFO_FULL);
  //SERIAL_ECHOLNPGM("TX DMA STATUS (finish): ", SPI.SPI_DMA_STATUS_REG.SPI_DMA_TX_EN);
  //SERIAL_ECHOLNPGM("RX DMA STATUS (finish): ", SPI.SPI_DMA_STATUS_REG.SPI_DMA_RX_EN);
  ////SERIAL_ECHOLNPGM("SPI STATE MACHINE ST (finish): ", SPI.SPI_EXT2_REG.SPI_ST);
  //SERIAL_ECHOLNPGM("TX COUNT: ", SPI.SPI_DMA_STATUS_REG.reserved1);

  //OUT_WRITE(BEEPER_PIN, LOW);

  proc.is_active = false;

  DMABusShutdown(SPI);
}

#endif //HALSPI_DISABLE_DMA

} // namespace MarlinESP32

static void _spiAsyncBarrier() {
#ifdef HAL_SPI_SUPPORTS_ASYNC
  spi_monitored_loop asyncw;
  while (MarlinESP32::_current_spi_proc.is_active) { asyncw.update(4); /* wait until any async-SPI process has finished */ }
#endif
}

static MarlinESP32::gpioMapResult_t _spi_gpiomap;
static volatile bool _spi_is_active = false;
static volatile bool _spi_transaction_is_running = false;
static MarlinESP32::clkcnt_res_t _spi_clkcnt;
#ifdef HALSPI_DEBUG
static int _spi_core_id;
#endif

void spiBegin() {
#ifdef HALSPI_DEBUG
  _spi_core_id = xPortGetCoreID();
#endif

#ifdef HAL_SPI_SUPPORTS_ASYNC
  MarlinESP32::SPIAsyncInitialize();
#endif

#ifndef HALSPI_DISABLE_DMA
  // Security checks.
  MarlinESP32::DMAInitializeMachine();
#endif

  #if ENABLED(SDSUPPORT) && PIN_EXISTS(SD_SS)
    OUT_WRITE(SD_SS_PIN, HIGH);
  #endif

  // By default disable the clock signals.
#ifdef HALSPI_ESP32_ENABLE_INTERNBUS
  // SPI0/1 are used for instruction-flash-memory access so it is not a good idea
  // to use them in Marlin!
  MarlinESP32::DPORT_PERIP_CLK_EN_REG.DPORT_SPI01_CLK_EN = false;
#endif
  MarlinESP32::DPORT_PERIP_CLK_EN_REG.DPORT_SPI2_CLK_EN = false;
  MarlinESP32::DPORT_PERIP_CLK_EN_REG.DPORT_SPI3_CLK_EN = false;
#if !defined(HALSPI_DISABLE_DMA)
  MarlinESP32::DPORT_PERIP_CLK_EN_REG.DPORT_SPI_DMA_CLK_EN = false;
#endif

  // Reset DMA channel selections.
#ifdef HALSPI_ESP32_ENABLE_INTERNBUS
  MarlinESP32::DPORT_SPI_DMA_CHAN_SEL_REG.DPORT_SPI_SPI1_DMA_CHAN_SEL = DPORT_SPI_DMA_CHAN_SEL_NONE;
#endif
  MarlinESP32::DPORT_SPI_DMA_CHAN_SEL_REG.DPORT_SPI_SPI2_DMA_CHAN_SEL = DPORT_SPI_DMA_CHAN_SEL_NONE;
  MarlinESP32::DPORT_SPI_DMA_CHAN_SEL_REG.DPORT_SPI_SPI3_DMA_CHAN_SEL = DPORT_SPI_DMA_CHAN_SEL_NONE;

#ifdef HALSPI_ESP32_ENABLE_INTERNBUS
  MarlinESP32::SPIResetBus(MarlinESP32::SPI0);
  MarlinESP32::SPIResetBus(MarlinESP32::SPI1);
#endif
  MarlinESP32::SPIResetBus(MarlinESP32::SPI2);
  MarlinESP32::SPIResetBus(MarlinESP32::SPI3);
}

void spiSetupChipSelect(int pin) {
  OUT_WRITE(pin, HIGH);
}

void spiInitEx(uint32_t maxClockFreq, int hint_sck, int hint_miso, int hint_mosi, int hint_cs) {
#ifdef HALSPI_DEBUG
  if (_spi_core_id != xPortGetCoreID())
    _spi_on_error(10);
#endif

#ifdef HAL_SPI_SUPPORTS_ASYNC
  spi_monitored_loop actw;
  while (_spi_is_active) { actw.update(5); /* wait until any other transaction has finished */ }
#else
  if (_spi_is_active)
    _spi_on_error(6);
#endif

  _spi_gpiomap = MarlinESP32::SPIMapGPIO(hint_sck, hint_miso, hint_mosi, hint_cs);
  _spi_is_active = true;

  uint8_t spibusIdx = _spi_gpiomap.spibusIdx;

  auto& SPI = MarlinESP32::SPIGetBusFromIndex(spibusIdx);

  // Enable the clock signal and reset the peripheral.
#ifdef HALSPI_ESP32_ENABLE_INTERNBUS
  if (spibusIdx == 0 || spibusIdx == 1) {
    MarlinESP32::DPORT_PERIP_RST_EN_REG.DPORT_SPI01_RST = true;
    MarlinESP32::DPORT_PERIP_RST_EN_REG.DPORT_SPI01_RST = false;
    MarlinESP32::DPORT_PERIP_CLK_EN_REG.DPORT_SPI01_CLK_EN = true;
  }
  else
#endif
  if (spibusIdx == 2) {
    MarlinESP32::DPORT_PERIP_RST_EN_REG.DPORT_SPI2_RST = true;
    MarlinESP32::DPORT_PERIP_RST_EN_REG.DPORT_SPI2_RST = false;
    MarlinESP32::DPORT_PERIP_CLK_EN_REG.DPORT_SPI2_CLK_EN = true;
  }
  else if (spibusIdx == 3) {
    MarlinESP32::DPORT_PERIP_RST_EN_REG.DPORT_SPI3_RST = true;
    MarlinESP32::DPORT_PERIP_RST_EN_REG.DPORT_SPI3_RST = false;
    MarlinESP32::DPORT_PERIP_CLK_EN_REG.DPORT_SPI3_CLK_EN = true;
  }

  // By default we transfer using MSB.
  MarlinESP32::SPIConfigureBitOrder(SPI, SPI_BITORDER_MSB);

  // Chapter 7.4.2 is relevant for the direct-IO/IOMUX timing optimization.
  uint32_t apbfreq = getApbFrequency(); // from Arduino SDK, depends on undocumented things.
  MarlinESP32::clkcnt_res_t clkdiv = MarlinESP32::SPIApproximateClockDivider(maxClockFreq, apbfreq);

  _spi_clkcnt = clkdiv;

  // Set basic SPI configuration.
  SPI.SPI_SLAVE_REG.SPI_SLAVE_MODE = 0; // master.

  // Enable the required SPI phases.
  SPI.SPI_USER_REG.SPI_USR_COMMAND = false;
  SPI.SPI_USER_REG.SPI_USR_ADDR = false;
  SPI.SPI_USER_REG.SPI_USR_DUMMY = false;
  SPI.SPI_USER_REG.SPI_USR_DUMMY_IDLE = false;
  SPI.SPI_USER_REG.SPI_SIO = false;
  SPI.SPI_USER_REG.SPI_USR_MOSI = (_spi_gpiomap.gpio_mosi >= 0);
  SPI.SPI_USER_REG.SPI_USR_MISO = (_spi_gpiomap.gpio_miso >= 0);

#ifdef HAL_SPI_SUPPORTS_ASYNC
  // Clear important interrupt settings.
  SPI.SPI_SLAVE_REG.SPI_TRANS_INTEN = false;
  SPI.SPI_SLAVE_REG.SPI_SLV_WR_STA_INTEN = false;
  SPI.SPI_SLAVE_REG.SPI_SLV_RD_STA_INTEN = false;
  SPI.SPI_SLAVE_REG.SPI_SLV_RD_BUF_INTEN = false;
  SPI.SPI_SLAVE_REG.SPI_SLV_WR_BUF_INTEN = false;
#endif

  // Configure the SPI clock to MODE0 by default.
  MarlinESP32::SPIConfigureClock(SPI, SPI_CLKMODE_0, _spi_gpiomap.datasig_is_direct_iomux, clkdiv);

  // Enable full-duplex communication (4lines R/W)
  SPI.SPI_USER_REG.SPI_DOUTDIN = true;
  // On ESP32 the input and output buffers of SPI are always being randomly overwritten/trashed.
  // Thus it makes little sense to divide them! Purpose of half-half division is performance
  // optimization using fill-while-writing.
  // Notes:
  // "If the data length is over 64 bytes, the extra part will be written from SPI_W0_REG."
  // - page 120 of ESP32 technical reference manual.
  // The buffer on the HIGHPART is limited to the HIGHPART.
  SPI.SPI_USER_REG.SPI_USR_MOSI_HIGHPART = false;
  SPI.SPI_USER_REG.SPI_USR_MISO_HIGHPART = false;

  // Disable any hardware-chip-select.
  SPI.SPI_PIN_REG.SPI_CS0_DIS = true;
  SPI.SPI_PIN_REG.SPI_CS1_DIS = true;
  SPI.SPI_PIN_REG.SPI_CS2_DIS = true;

  _spi_transaction_is_running = false;
}

static void _maybe_start_transaction() {
  if (_spi_transaction_is_running) return;
  if (_spi_gpiomap.gpio_cs >= 0) {
    OUT_WRITE(_spi_gpiomap.gpio_cs, LOW);
  }
  _spi_transaction_is_running = true;
}

void spiInit(uint8_t spiRate, int hint_sck, int hint_miso, int hint_mosi, int hint_cs) {
  uint32_t clock;

  switch (spiRate) {
    case SPI_FULL_SPEED:      clock = 16000000; break;
    case SPI_HALF_SPEED:      clock = 8000000;  break;
    case SPI_QUARTER_SPEED:   clock = 4000000;  break;
    case SPI_EIGHTH_SPEED:    clock = 2000000;  break;
    case SPI_SIXTEENTH_SPEED: clock = 1000000;  break;
    case SPI_SPEED_5:         clock = 500000;   break;
    case SPI_SPEED_6:         clock = 250000;   break;
    default:                  clock = 1000000; // Default from the SPI library
  }
  spiInitEx(clock, hint_sck, hint_miso, hint_mosi, hint_cs);
}

void spiClose() {
  _spiAsyncBarrier();

  if (_spi_is_active == false)
    _spi_on_error(7);

  if (_spi_transaction_is_running) {
    if (_spi_gpiomap.gpio_cs >= 0) {
      OUT_WRITE(_spi_gpiomap.gpio_cs, HIGH);
    }
    _spi_transaction_is_running = false;
  }

  // Disable the clock signal.
  uint8_t spibusIdx = _spi_gpiomap.spibusIdx;

#ifdef HALSPI_ESP32_ENABLE_INTERNBUS
  if (spibusIdx == 0 || spibusIdx == 1) {
    MarlinESP32::DPORT_PERIP_CLK_EN_REG.DPORT_SPI01_CLK_EN = false;
  }
  else
#endif
  if (spibusIdx == 2) {
    MarlinESP32::DPORT_PERIP_CLK_EN_REG.DPORT_SPI2_CLK_EN = false;
  }
  else if (spibusIdx == 3) {
    MarlinESP32::DPORT_PERIP_CLK_EN_REG.DPORT_SPI3_CLK_EN = false;
  }

  MarlinESP32::SPIUnmapGPIO(_spi_gpiomap);

  _spi_is_active = false;
}

void spiSetBitOrder(int bitOrder) {
  _spiAsyncBarrier();

  auto& SPI = MarlinESP32::SPIGetBusFromIndex(_spi_gpiomap.spibusIdx);

  MarlinESP32::SPIConfigureBitOrder(SPI, bitOrder);
}

void spiSetClockMode(int mode) {
  _spiAsyncBarrier();

  auto& SPI = MarlinESP32::SPIGetBusFromIndex(_spi_gpiomap.spibusIdx);

  MarlinESP32::SPIConfigureClock(SPI, mode, _spi_gpiomap.datasig_is_direct_iomux, _spi_clkcnt);
}

void spiEstablish() {
  _maybe_start_transaction();
}

void spiSend(uint8_t txval) {
  auto& SPI = MarlinESP32::SPIGetBusFromIndex(_spi_gpiomap.spibusIdx);

  if (SPI.SPI_USER_REG.SPI_USR_MOSI == false) return;

  _spiAsyncBarrier();

  _maybe_start_transaction();

  uint16_t start_num_idx = MarlinESP32::SPIGetWriteBufferStartIndex(SPI);

  SPI.SPI_W_REG[start_num_idx] = txval;
  MarlinESP32::SPITransaction(SPI, 1);
}

void spiSend16(uint16_t txval) {
  auto& SPI = MarlinESP32::SPIGetBusFromIndex(_spi_gpiomap.spibusIdx);

  if (SPI.SPI_USER_REG.SPI_USR_MOSI == false) return;

  _spiAsyncBarrier();

  _maybe_start_transaction();

  eir::BitOrderingConverter bitconv(
    ( SPI.SPI_CTRL_REG.SPI_WR_BIT_ORDER == _ESP32_BIT_ORDER_MSB ? endian::eSpecificEndian::BIG_ENDIAN : endian::eSpecificEndian::LITTLE_ENDIAN ),
    false // already done by HW
  );

  uint16_t start_num_idx = MarlinESP32::SPIGetWriteBufferStartIndex(SPI);

  SPI.SPI_W_REG[start_num_idx] = bitconv.Identity( txval );
  MarlinESP32::SPITransaction(SPI, sizeof(uint16_t));
}

uint8_t spiRec(uint8_t txval) {
  auto& SPI = MarlinESP32::SPIGetBusFromIndex(_spi_gpiomap.spibusIdx);

  if (SPI.SPI_USER_REG.SPI_USR_MISO == false) return 0;

  _spiAsyncBarrier();

  _maybe_start_transaction();

  if (SPI.SPI_USER_REG.SPI_USR_MOSI) {
    uint16_t start_num_idx = MarlinESP32::SPIGetWriteBufferStartIndex(SPI);
    SPI.SPI_W_REG[start_num_idx] = txval;
  }

  MarlinESP32::SPITransaction(SPI, sizeof(uint8_t));

  uint16_t start_num_idx = MarlinESP32::SPIGetReadBufferStartIndex(SPI);

  return (uint8_t)SPI.SPI_W_REG[start_num_idx];
}

uint16_t spiRec16(uint16_t txval) {
  auto& SPI = MarlinESP32::SPIGetBusFromIndex(_spi_gpiomap.spibusIdx);

  if (SPI.SPI_USER_REG.SPI_USR_MISO == false) return 0;

  _spiAsyncBarrier();

  _maybe_start_transaction();

  eir::BitOrderingConverter bitconv(
    ( SPI.SPI_CTRL_REG.SPI_RD_BIT_ORDER == _ESP32_BIT_ORDER_MSB ? endian::eSpecificEndian::BIG_ENDIAN : endian::eSpecificEndian::LITTLE_ENDIAN ),
    false // already done by HW
  );

  if (SPI.SPI_USER_REG.SPI_USR_MOSI) {
    uint16_t start_num_idx = MarlinESP32::SPIGetWriteBufferStartIndex(SPI);
    SPI.SPI_W_REG[start_num_idx] = bitconv.Identity( txval );
  }

  MarlinESP32::SPITransaction(SPI, sizeof(uint16_t));

  uint16_t start_num_idx = MarlinESP32::SPIGetReadBufferStartIndex(SPI);

  uint16_t bits = (uint16_t)SPI.SPI_W_REG[start_num_idx];

  return bitconv.Identity( bits );
}

void spiRead(uint8_t *buf, uint16_t cnt, uint8_t txval) {
  if (cnt == 0) return;

  auto& SPI = MarlinESP32::SPIGetBusFromIndex(_spi_gpiomap.spibusIdx);

  if (SPI.SPI_USER_REG.SPI_USR_MISO == false) {
    spiWriteRepeat(txval, cnt);
    return;
  }

  _spiAsyncBarrier();

  _maybe_start_transaction();

#if 0
#ifndef HALSPI_DISABLE_DMA
  if (MarlinESP32::DMAIsCapableSPIBus(SPI) && MarlinESP32::DMAIsValidWriteDataBuffer(buf, cnt)
#ifndef HALSPI_ESP32_DMA_ALWAYS
    && cnt > MarlinESP32::SPIGetReadBufferSize(SPI)
#endif
  ) {
    // For bigger transfers we should use DMA.
    //TODO.
    return;
  }

  // Use direct SPI for such small transfer sizes.
#endif
#endif
  // The no-DMA version.

  esp32BitManager <uint8_t, true> recv_bitman( buf, cnt );

  uint16_t start_num_idx = MarlinESP32::SPIGetReadBufferStartIndex(SPI);

  esp32BitManager <volatile uint32_t> txbuf_bitman( &SPI.SPI_W_REG[start_num_idx], COUNT(SPI.SPI_W_REG) - start_num_idx );

  using reptx_t = typename eir::biggest_type <typename decltype(txbuf_bitman)::supportedTypes>::type;

  eir::BitRepetitionCache <uint8_t, reptx_t> repcache( txval );

  bool is_write_avail = (SPI.SPI_USER_REG.SPI_USR_MOSI);

  eir::BitManagerTemplates::Receive( recv_bitman, txbuf_bitman,
    [&] ( const auto& bititer ) LAINLINE
    {
      if (is_write_avail) {
        eir::BitNumberIteratorForStruct <reptx_t> repiter;
        txbuf_bitman.PutRepeatable( repcache.GetData(), repiter, repiter, bititer );
        txbuf_bitman.Flush();
      }
      auto bytecnt = bititer.getTotalByteOffset();
      MarlinESP32::SPITransaction(SPI, (uint32_t)bytecnt);
      if (is_write_avail) {
        txbuf_bitman.SetIterator({});
      }
    }
  );
}

void spiSendBlock(uint8_t token, const uint8_t *buf) {
  auto& SPI = MarlinESP32::SPIGetBusFromIndex(_spi_gpiomap.spibusIdx);

  if (SPI.SPI_USER_REG.SPI_USR_MOSI == false) return;

  _spiAsyncBarrier();

  _maybe_start_transaction();

#ifndef HALSPI_DISABLE_DMA
  if (MarlinESP32::DMAIsCapableSPIBus(SPI) && MarlinESP32::DMAIsValidReadDataBuffer(buf)
#ifndef HALSPI_ESP32_DMA_ALWAYS
#ifdef HALSPI_DMA_THRESHOLD
    && 512 > HALSPI_DMA_THRESHOLD
#endif
#endif
  ) {
    // Only attempt DMA transfer if the buffer is valid.
    spiSend(token);
    MarlinESP32::DMASendBlocking(SPI, buf, 512, 512);
    return;
  }

  // Use direct SPI otherwise.
#endif

  uint16_t start_num_idx = MarlinESP32::SPIGetWriteBufferStartIndex(SPI);

  esp32BitManager <const uint8_t> src_bitman( buf, 512 );

  esp32BitManager <volatile uint32_t, true> buf_bitman( &SPI.SPI_W_REG[start_num_idx], COUNT(SPI.SPI_W_REG) - start_num_idx );

  buf_bitman.Put( token );

  eir::BitManagerTemplates::Send( src_bitman, buf_bitman,
    [&] ( void ) LAINLINE
    {
      auto bytecnt = buf_bitman.GetIterator().getTotalByteOffset();
      MarlinESP32::SPITransaction( SPI, (uint32_t)bytecnt );
    }
  );
}

void spiWrite(const uint8_t *buf, uint16_t cnt) {
  if (cnt == 0) return;

  auto& SPI = MarlinESP32::SPIGetBusFromIndex(_spi_gpiomap.spibusIdx);

  if (SPI.SPI_USER_REG.SPI_USR_MOSI == false) return;

  _spiAsyncBarrier();

  _maybe_start_transaction();

#ifndef HALSPI_DISABLE_DMA
  if (MarlinESP32::DMAIsCapableSPIBus(SPI) && MarlinESP32::DMAIsValidReadDataBuffer(buf)
#ifndef HALSPI_ESP32_DMA_ALWAYS
#ifdef HALSPI_DMA_THRESHOLD
    && cnt > HALSPI_DMA_THRESHOLD
#else
    && cnt > MarlinESP32::SPIGetWriteBufferSize(SPI)
#endif //HALSPI_DMA_THRESHOLD
#endif
    ) {
    // For bigger transfers we should use DMA.
    MarlinESP32::DMASendBlocking(SPI, buf, ALIGN_SIZE(cnt, (uint16_t)4u), cnt);
    return;
  }

  // Use direct SPI for small transfer sizes.
#endif

  uint16_t start_num_idx = MarlinESP32::SPIGetWriteBufferStartIndex(SPI);

  esp32BitManager <const uint8_t> src_bitMan( buf, cnt );

  eir::BitManagerTemplates::SendFixed( &SPI.SPI_W_REG[start_num_idx], COUNT(SPI.SPI_W_REG) - start_num_idx, src_bitMan,
    [&] ( const auto& iter ) LAINLINE
    {
      MarlinESP32::SPITransaction( SPI, iter.getTotalByteOffset() );
    }
  );
}

void spiWrite16(const uint16_t *buf, uint16_t cnt) {
  if (cnt == 0) return;

  auto& SPI = MarlinESP32::SPIGetBusFromIndex(_spi_gpiomap.spibusIdx);

  if (SPI.SPI_USER_REG.SPI_USR_MOSI == false) return;

  _spiAsyncBarrier();

  _maybe_start_transaction();

  // Problem: before we can kick-off the DMA transfer to the SPI device we have to FORMAT THE ENTIRE DMA BUFFER to match
  // the byte-by-byte requirement of the DMA-to-SPI-buffer filling! This is kind of a bummer because it means that DMA
  // transfers of color buffers have to be filled in a special way, at best directly when they are created.
  // Not only that but also the buffer needs to meet special allocation rules. Yikes!

  // This would call for a special DMA-capable buffer filling structure/framework that is also part of the HAL and
  // specialized per architecture.
  // At least, ESP32 SPI is pretty fast as it currently stands, even without async DMA.

  // LSBFIRST transfers can be pushed through the DMAC just fine because no bit-reversing/byte-swapping is necessary.

#if 0
#ifndef HALSPI_DISABLE_DMA
  if (MarlinESP32::DMAIsCapableSPIBus(SPI) && MarlinESP32::DMAIsValidReadDataBuffer(buf) && cnt > MarlinESP32::SPIGetWriteBufferSize(SPI)) {
    // For bigger transfers we should use DMA.
    //TODO.
    return;
  }

  // Use direct SPI for small transfer sizes.
#endif
#endif

  uint16_t start_num_idx = MarlinESP32::SPIGetWriteBufferStartIndex(SPI);

  esp32BitManager <const uint16_t> src_bitMan( buf, cnt );

  MarlinESP32::SPIPrepareWriteBitManager( SPI, src_bitMan );
  
  eir::BitManagerTemplates::SendFixed( &SPI.SPI_W_REG[start_num_idx], COUNT(SPI.SPI_W_REG) - start_num_idx, src_bitMan,
    [&] ( const auto& iter ) LAINLINE
    {
      MarlinESP32::SPITransaction( SPI, iter.getTotalByteOffset() );
    }
  );
}

void spiWriteRepeat(uint8_t val, uint16_t repcnt) {
  if (repcnt == 0) return;

  auto& SPI = MarlinESP32::SPIGetBusFromIndex(_spi_gpiomap.spibusIdx);

  if (SPI.SPI_USER_REG.SPI_USR_MOSI == false) return;

  _spiAsyncBarrier();

  _maybe_start_transaction();

  // There is no good repetition output engine on the ESP32 DMAC.
  // Thus we have to use generic SPI.

  uint16_t start_num_idx = MarlinESP32::SPIGetWriteBufferStartIndex(SPI);

  esp32BitManager <volatile uint32_t, true> buf_bitman( &SPI.SPI_W_REG[start_num_idx], COUNT(SPI.SPI_W_REG) - start_num_idx );

  eir::BitManagerTemplates::RepeatSendCountTo( val, repcnt, buf_bitman,
    [&] ( void ) LAINLINE
    {
      auto bytecnt = buf_bitman.GetIterator().getTotalByteOffset();
      MarlinESP32::SPITransaction( SPI, (uint32_t)bytecnt );
    }
  );
}

void spiWriteRepeat16(uint16_t val, uint16_t repcnt) {
  if (repcnt == 0) return;

  auto& SPI = MarlinESP32::SPIGetBusFromIndex(_spi_gpiomap.spibusIdx);

  if (SPI.SPI_USER_REG.SPI_USR_MOSI == false) return;

  _spiAsyncBarrier();

  _maybe_start_transaction();

  // There is no good repetition output engine on the ESP32 DMAC.
  // Thus we have to use generic SPI.

  uint16_t start_num_idx = MarlinESP32::SPIGetWriteBufferStartIndex(SPI);

  esp32BitManager <volatile uint32_t, true> buf_bitman( &SPI.SPI_W_REG[start_num_idx], COUNT(SPI.SPI_W_REG) - start_num_idx );

  eir::BitOrderingConverter bitconv(
    ( SPI.SPI_CTRL_REG.SPI_WR_BIT_ORDER == _ESP32_BIT_ORDER_MSB ? endian::eSpecificEndian::BIG_ENDIAN : endian::eSpecificEndian::LITTLE_ENDIAN ),
    false // already done by HW
  );

  eir::BitManagerTemplates::RepeatSendCountTo( bitconv.Identity( val ), repcnt, buf_bitman,
    [&] ( void ) LAINLINE
    {
      auto bytecnt = buf_bitman.GetIterator().getTotalByteOffset();
      MarlinESP32::SPITransaction( SPI, (uint32_t)bytecnt );
    }
  );
}

#ifdef HAL_SPI_SUPPORTS_ASYNC

void spiWriteAsync(const uint8_t *buf, uint16_t nbyte, void (*completeCallback)(void*), void *ud) {
  auto& SPI = MarlinESP32::SPIGetBusFromIndex(_spi_gpiomap.spibusIdx);

  if (SPI.SPI_USER_REG.SPI_USR_MOSI == false) return;

  if (nbyte == 0) {
    if (completeCallback)
      completeCallback(ud);
    return;
  }

  _maybe_start_transaction();

  // TODO: we could implement this async using DMA aswell because 8bit transfers are easily done using
  // the hardware. Right now it is fine using SPI interrupts anyway (CPU is not being slowed down by slow SPI rates).

  MarlinESP32::SPIStartRawAsync(SPI, buf, nbyte, sizeof(uint8_t), completeCallback, ud);
}

void spiWriteAsync16(const uint16_t *buf, uint16_t txcnt, void (*completeCallback)(void*), void *ud) {
  auto& SPI = MarlinESP32::SPIGetBusFromIndex(_spi_gpiomap.spibusIdx);

  if (SPI.SPI_USER_REG.SPI_USR_MOSI == false) return;

  if (txcnt == 0) {
    if (completeCallback)
      completeCallback(ud);
    return;
  }

  _maybe_start_transaction();

  MarlinESP32::SPIStartRawAsync(SPI, buf, txcnt, sizeof(uint16_t), completeCallback, ud);
}

void spiAsyncAbort() {
  MarlinESP32::SPIAbortRawAsync();
}

void spiAsyncJoin() {
  _spiAsyncBarrier();
}

bool spiAsyncIsRunning() {
  return (MarlinESP32::_current_spi_proc.is_active);
}

#endif

#endif

#endif
