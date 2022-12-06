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

// TODO: test the DMA with an actually working LPC1768 board because the one I got was faulty...
//#define HALSPI_DISABLE_DMA

/**
 * For TARGET_LPC1768
 */

/**
 * Hardware SPI and Software SPI implementations are included in this file.
 *
 * Control of the slave select pin(s) is handled by the calling routines.
 *
 * Some of the LCD interfaces/adapters result in the LCD SPI and the SD card
 * SPI sharing pins. The SCK, MOSI & MISO pins can NOT be set/cleared with
 * WRITE nor digitalWrite when the hardware SPI module within the LPC17xx is
 * active. If any of these pins are shared then the software SPI must be used.
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

// Actually: LPC176x/LPC175x
#ifdef TARGET_LPC1768

#include "../../inc/MarlinConfig.h"

#include "../shared/HAL_SPI.h"
#include "../shared/ARM/HAL_NVIC.h"

#if !ENABLED(SOFTWARE_SPI)

#ifndef LPC_MAINOSCILLATOR_FREQ
#error Missing LPC176X/LPC175X main oscillator frequency! Consult manufacturer schematics for further details (XTAL1/XTAL2 pins as guidance)
#endif

static void _spi_on_error(uint32_t code = 0) {
  for (;;) {
#if ENABLED(HALSPI_DO_ERRORBEEPS) && PIN_EXISTS(BEEPER)
    OUT_WRITE(BEEPER_PIN, HIGH);
    delay(1000);
    OUT_WRITE(BEEPER_PIN, LOW);
    delay(1000);
    OUT_WRITE(BEEPER_PIN, HIGH);
    delay(500);
    OUT_WRITE(BEEPER_PIN, LOW);
    if (code > 0)
      delay(500);
    for (uint32_t n = 0; n < code; n++) {
      OUT_WRITE(BEEPER_PIN, HIGH);
      delay(250);
      OUT_WRITE(BEEPER_PIN, LOW);
      if (n < code-1)
        delay(250);
    }
    if (code > 0)
      delay(800);
    OUT_WRITE(BEEPER_PIN, HIGH);
    delay(1000);
    OUT_WRITE(BEEPER_PIN, LOW);
    delay(2000);
#endif
  }
}

static void __attribute__((unused)) _spi_infobeep(uint32_t code) {
#if PIN_EXISTS(BEEPER)
  OUT_WRITE(BEEPER_PIN, HIGH);
  delay(500);
  OUT_WRITE(BEEPER_PIN, LOW);
  if (code > 0)
    delay(500);
  for (uint32_t n = 0; n < code; n++) {
    OUT_WRITE(BEEPER_PIN, HIGH);
    delay(200);
    OUT_WRITE(BEEPER_PIN, LOW);
    if (n < code-1)
      delay(200);
  }
  delay(500);
  OUT_WRITE(BEEPER_PIN, HIGH);
  delay(400);
  OUT_WRITE(BEEPER_PIN, LOW);
  delay(2000);
#endif
}

template <typename numberType>
inline numberType __MIN(numberType a, numberType b) {
  return ( a < b ? a : b );
}

namespace MarlinLPC {

// NXP UM10360 date: 20th of November, 2022

// There are pins for each "port" on the LPC1768. Do not be confused with other architectures
// where there is GPIO_0 .. GPIO_n linearly numbered. The naming convention on the LPC1768 is
// very unique.

struct pinsel0_reg_t {
  uint32_t p0_00 : 2;
  uint32_t p0_01 : 2;
  uint32_t p0_02 : 2;
  uint32_t p0_03 : 2;
  uint32_t p0_04 : 2;
  uint32_t p0_05 : 2;
  uint32_t p0_06 : 2;
  uint32_t p0_07 : 2;
  uint32_t p0_08 : 2;
  uint32_t p0_09 : 2;
  uint32_t p0_10 : 2;
  uint32_t p0_11 : 2;
  uint32_t reserved1 : 6;
  uint32_t p0_15 : 2;
};
static_assert(sizeof(pinsel0_reg_t) == 4, "invalid size of LPC pinsel0_reg_t");

struct pinsel1_reg_t {
  uint32_t p0_16 : 2;
  uint32_t p0_17 : 2;
  uint32_t p0_18 : 2;
  uint32_t p0_19 : 2;
  uint32_t p0_20 : 2;
  uint32_t p0_21 : 2;
  uint32_t p0_22 : 2;
  uint32_t p0_23 : 2;
  uint32_t p0_24 : 2;
  uint32_t p0_25 : 2;
  uint32_t p0_26 : 2;
  uint32_t p0_27 : 2;
  uint32_t p0_28 : 2;
  uint32_t p0_29 : 2;
  uint32_t p0_30 : 2;
  uint32_t reserved1 : 2;
};
static_assert(sizeof(pinsel1_reg_t) == 4, "invalid size of LPC pinsel1_reg_t");

struct pinsel2_reg_t {
  uint32_t p1_00 : 2;
  uint32_t p1_01 : 2;
  uint32_t reserved1 : 4;
  uint32_t p1_04 : 2;
  uint32_t reserved2 : 6;
  uint32_t p1_08 : 2;
  uint32_t p1_09 : 2;
  uint32_t p1_10 : 2;
  uint32_t reserved3 : 6;
  uint32_t p1_14 : 2;
  uint32_t p1_15 : 2;
};
static_assert(sizeof(pinsel2_reg_t) == 4, "invalid size of LPC pinsel2_reg_t");

struct pinsel3_reg_t {
  uint32_t p1_16 : 2;
  uint32_t p1_17 : 2;
  uint32_t p1_18 : 2;
  uint32_t p1_19 : 2;
  uint32_t p1_20 : 2;
  uint32_t p1_21 : 2;
  uint32_t p1_22 : 2;
  uint32_t p1_23 : 2;
  uint32_t p1_24 : 2;
  uint32_t p1_25 : 2;
  uint32_t p1_26 : 2;
  uint32_t p1_27 : 2;
  uint32_t p1_28 : 2;
  uint32_t p1_29 : 2;
  uint32_t p1_30 : 2;
  uint32_t p1_31 : 2;
};
static_assert(sizeof(pinsel3_reg_t) == 4, "invalid size of LPC pinsel3_reg_t");

struct pinsel4_reg_t {
  uint32_t p2_00 : 2;
  uint32_t p2_01 : 2;
  uint32_t p2_02 : 2;
  uint32_t p2_03 : 2;
  uint32_t p2_04 : 2;
  uint32_t p2_05 : 2;
  uint32_t p2_06 : 2;
  uint32_t p2_07 : 2;
  uint32_t p2_08 : 2;
  uint32_t p2_09 : 2;
  uint32_t p2_10 : 2;
  uint32_t p2_11 : 2;
  uint32_t p2_12 : 2;
  uint32_t p2_13 : 2;
  uint32_t p2_14 : 2;
  uint32_t p2_15 : 2;
};
static_assert(sizeof(pinsel4_reg_t) == 4, "invalid size of LPC pinsel4_reg_t");

struct pinsel7_reg_t {
  uint32_t reserved1 : 18;
  uint32_t p3_25 : 2;
  uint32_t p3_26 : 2;
  uint32_t reserved2 : 10;
};
static_assert(sizeof(pinsel7_reg_t) == 4, "invalid size of LPC pinsel7_reg_t");

struct pinsel9_reg_t {
  uint32_t reserved1 : 24;
  uint32_t p4_28 : 2;
  uint32_t p4_29 : 2;
  uint32_t reserved2 : 4;
};
static_assert(sizeof(pinsel9_reg_t) == 4, "invalid size of LPC pinsel9_reg_t");

struct pinsel10_reg_t {
  uint32_t reserved1 : 2;
  uint32_t gpio_trace : 1;
  uint32_t reserved2 : 29;
};
static_assert(sizeof(pinsel10_reg_t) == 4, "invalid size of LPC pinsel10_reg_t");

#define LPC_PINMODE_PULLUP 0
#define LPC_PINMODE_REPEATER 1
#define LPC_PINMODE_NONE 2
#define LPC_PINMODE_PULLDOWN 3

struct pinmode0_reg_t {
  uint32_t p0_00mode : 2;
  uint32_t p0_01mode : 2;
  uint32_t p0_02mode : 2;
  uint32_t p0_03mode : 2;
  uint32_t p0_04mode : 2;
  uint32_t p0_05mode : 2;
  uint32_t p0_06mode : 2;
  uint32_t p0_07mode : 2;
  uint32_t p0_08mode : 2;
  uint32_t p0_09mode : 2;
  uint32_t p0_10mode : 2;
  uint32_t p0_11mode : 2;
  uint32_t reserved1 : 6;
  uint32_t p0_15mode : 2;
};
static_assert(sizeof(pinmode0_reg_t) == 4, "invalid size of LPC pinmode0_reg_t");

struct pinmode1_reg_t {
  uint32_t p0_16mode : 2;
  uint32_t p0_17mode : 2;
  uint32_t p0_18mode : 2;
  uint32_t p0_19mode : 2;
  uint32_t p0_20mode : 2;
  uint32_t p0_21mode : 2;
  uint32_t p0_22mode : 2;
  uint32_t p0_23mode : 2;
  uint32_t p0_24mode : 2;
  uint32_t p0_25mode : 2;
  uint32_t p0_26mode : 2;
  uint32_t reserved1 : 10;
};
static_assert(sizeof(pinmode1_reg_t) == 4, "invalid size of pinmode1_reg_t");

struct pinmode2_reg_t {
  uint32_t p1_00mode : 2;
  uint32_t p1_01mode : 2;
  uint32_t reserved1 : 4;
  uint32_t p1_04mode : 2;
  uint32_t reserved2 : 6;
  uint32_t p1_08mode : 2;
  uint32_t p1_09mode : 2;
  uint32_t p1_10mode : 2;
  uint32_t reserved3 : 6;
  uint32_t p1_14mode : 2;
  uint32_t p1_15mode : 2;
};
static_assert(sizeof(pinmode2_reg_t) == 4, "invalid size of pinmode2_reg_t");

struct pinmode3_reg_t {
  uint32_t p1_16mode : 2;
  uint32_t p1_17mode : 2;
  uint32_t p1_18mode : 2;
  uint32_t p1_19mode : 2;
  uint32_t p1_20mode : 2;
  uint32_t p1_21mode : 2;
  uint32_t p1_22mode : 2;
  uint32_t p1_23mode : 2;
  uint32_t p1_24mode : 2;
  uint32_t p1_25mode : 2;
  uint32_t p1_26mode : 2;
  uint32_t p1_27mode : 2;
  uint32_t p1_28mode : 2;
  uint32_t p1_29mode : 2;
  uint32_t p1_30mode : 2;
  uint32_t p1_31mode : 2;
};
static_assert(sizeof(pinmode3_reg_t) == 4, "invalid size of pinmode3_reg_t");

struct pinmode4_reg_t {
  uint32_t p2_00mode : 2;
  uint32_t p2_01mode : 2;
  uint32_t p2_02mode : 2;
  uint32_t p2_03mode : 2;
  uint32_t p2_04mode : 2;
  uint32_t p2_05mode : 2;
  uint32_t p2_06mode : 2;
  uint32_t p2_07mode : 2;
  uint32_t p2_08mode : 2;
  uint32_t p2_09mode : 2;
  uint32_t p2_10mode : 2;
  uint32_t p2_11mode : 2;
  uint32_t p2_12mode : 2;
  uint32_t p2_13mode : 2;
  uint32_t reserved1 : 4;
};
static_assert(sizeof(pinmode4_reg_t) == 4, "invalid size of pinmode4_reg_t");

struct pinmode7_reg_t {
  uint32_t reserved1 : 18;
  uint32_t p3_25mode : 2;
  uint32_t p3_26mode : 2;
  uint32_t reserved2 : 10;
};
static_assert(sizeof(pinmode7_reg_t) == 4, "invalid size of pinmode7_reg_t");

struct pinmode9_reg_t {
  uint32_t reserved1 : 24;
  uint32_t p4_28mode : 2;
  uint32_t p4_29mode : 2;
  uint32_t reserved2 : 4;
};
static_assert(sizeof(pinmode9_reg_t) == 4, "invalid size of pinmode9_reg_t");

static pinsel0_reg_t& PINSEL0 = *(pinsel0_reg_t*)0x4002C000;
static pinsel1_reg_t& PINSEL1 = *(pinsel1_reg_t*)0x4002C004;
static pinsel2_reg_t& PINSEL2 = *(pinsel2_reg_t*)0x4002C008;
static pinsel3_reg_t& PINSEL3 = *(pinsel3_reg_t*)0x4002C00C;
static pinsel4_reg_t& PINSEL4 = *(pinsel4_reg_t*)0x4002C010;
static pinsel7_reg_t& PINSEL7 = *(pinsel7_reg_t*)0x4002C01C;
//static pinsel8_reg_t& PINSEL8 = *(pinsel8_reg_t*)0x4002C020;
static pinsel9_reg_t& PINSEL9 = *(pinsel9_reg_t*)0x4002C024;
static pinsel10_reg_t& PINSEL10 = *(pinsel10_reg_t*)0x4002C028;

static pinmode0_reg_t& PINMODE0 = *(pinmode0_reg_t*)0x4002C040;
static pinmode1_reg_t& PINMODE1 = *(pinmode1_reg_t*)0x4002C044;
static pinmode2_reg_t& PINMODE2 = *(pinmode2_reg_t*)0x4002C048;
static pinmode3_reg_t& PINMODE3 = *(pinmode3_reg_t*)0x4002C04C;
static pinmode4_reg_t& PINMODE4 = *(pinmode4_reg_t*)0x4002C050;
//static pinmode5_reg_t& PINMODE5 = *(pinmode5_reg_t*)0x4002C054;
//static pinmode6_reg_t& PINMODE6 = *(pinmode6_reg_t*)0x4002C058;
static pinmode7_reg_t& PINMODE7 = *(pinmode7_reg_t*)0x4002C05C;
static pinmode9_reg_t& PINMODE9 = *(pinmode9_reg_t*)0x4002C064;

// Left out OD and I2C-specific.
// UM10360 page 103: I am only taking the pin descriptions for LPC176x
// but support could (easily) be enabled for LPC175x aswell, if the community demands it.

#if 0

#define LPC_GPIODIR_INPUT 0
#define LPC_GPIODIR_OUTPUT 1

struct fioXdir_reg_t {
  uint32_t val;
  
  inline void set(uint8_t reg, uint8_t val) {
    if (val == LPC_GPIODIR_INPUT)
      val &= ~(1<<reg);
    else if (val == LPC_GPIODIR_OUTPUT)
      val |= (1<<reg);
  }
  inline uint8_t get(uint8_t reg) const {
    bool dir = ( val & (1<<reg) ) != 0;
    return ( dir ? LPC_GPIODIR_OUTPUT : LPC_GPIODIR_INPUT );
  }
};

struct fioXmask_reg_t {
  uint32_t val;

  inline void set(uint8_t reg, bool enable) {
    if (enable)
      val |= (1<<reg);
    else
      val &= ~(1<<reg);
  }
  inline bool get(uint8_t reg) const {
    return ( ( val & (1<<reg) ) >> reg );
  }
};

struct fioXpin_reg_t {
  uint32_t val;

  inline void set(uint8_t reg, bool high) {
    if (high)
      val |= (1<<reg);
    else
      val &= ~(1<<reg);
  }
  inline bool get(uint8_t reg) const {
    return ( ( val & (1<<reg) ) >> reg );
  }
};

static fioXdir_reg_t& FIO0DIR = *(fioXdir_reg_t*)0x2009C000;
static fioXdir_reg_t& FIO1DIR = *(fioXdir_reg_t*)0x2009C020;
static fioXdir_reg_t& FIO2DIR = *(fioXdir_reg_t*)0x2009C040;
static fioXdir_reg_t& FIO3DIR = *(fioXdir_reg_t*)0x2009C060;
static fioXdir_reg_t& FIO4DIR = *(fioXdir_reg_t*)0x2009C080;

static fioXmask_reg_t& FIO0MASK = *(fioXmask_reg_t*)0x2009C010;
static fioXmask_reg_t& FIO1MASK = *(fioXmask_reg_t*)0x2009C030;
static fioXmask_reg_t& FIO2MASK = *(fioXmask_reg_t*)0x2009C050;
static fioXmask_reg_t& FIO3MASK = *(fioXmask_reg_t*)0x2009C070;
static fioXmask_reg_t& FIO4MASK = *(fioXmask_reg_t*)0x2009C090;

static fioXpin_reg_t& FIO0PIN = *(fioXpin_reg_t*)0x2009C014;
static fioXpin_reg_t& FIO1PIN = *(fioXpin_reg_t*)0x2009C034;
static fioXpin_reg_t& FIO2PIN = *(fioXpin_reg_t*)0x2009C054;
static fioXpin_reg_t& FIO3PIN = *(fioXpin_reg_t*)0x2009C074;
static fioXpin_reg_t& FIO4PIN = *(fioXpin_reg_t*)0x2009C094;

#endif

// Find a valid port-mapping for the given GPIO to a SPI bus peripheral.
// If it fails then a recommended SPI bus with a recommended set of GPIO
// pins should be used.
// The LPC1768 is a really good architecture, even though in comparison
// to the ESP32 it does not support mapping any peripheral signal to any
// GPIO pin.
struct portMapResult_t {
  inline portMapResult_t() {
    func = 0xFF;
  }
  inline bool isMapped() const {
    return ( func != 0xFF );
  }
  uint8_t func;
};

static bool SPIFindPortMapping(
  int gpio_sck, int gpio_miso, int gpio_mosi, int gpio_cs,
  uint8_t& sspBusOut, portMapResult_t& map_sck_out, portMapResult_t& map_miso_out, portMapResult_t& map_mosi_out, portMapResult_t& map_cs_out
)
{
  portMapResult_t map_sck, map_miso, map_mosi, map_cs;
  bool found = false;

  if ((gpio_sck == P0_15 || gpio_sck == P1_20) || (gpio_miso == P0_17 || gpio_miso == P1_23) || (gpio_mosi == P0_18 || gpio_mosi == P1_24) || (gpio_cs == P0_16 || gpio_cs == P1_21)) {
    sspBusOut = 0;
    if (gpio_sck == P0_15) {
      map_sck.func = 2;
    }
    else if (gpio_sck == P1_20) {
      map_sck.func = 3;
    }
    if (gpio_miso == P0_17) {
      map_miso.func = 2;
    }
    else if (gpio_miso == P1_23) {
      map_miso.func = 3;
    }
    if (gpio_mosi == P0_18) {
      map_mosi.func = 2;
    }
    else if (gpio_mosi == P1_24) {
      map_mosi.func = 3;
    }
    if (gpio_cs == P0_16) {
      map_cs.func = 2;
    }
    else if (gpio_cs == P1_21) {
      map_cs.func = 3;
    }
    found = true;
  }
  else if ((gpio_sck == P0_07 || gpio_sck == P1_31) || gpio_miso == P0_08 || gpio_mosi == P0_09 || gpio_cs == P0_06) {
    sspBusOut = 1;
    if (gpio_sck == P0_07) {
      map_sck.func = 2;
    }
    else if (gpio_sck == P1_31) {
      map_sck.func = 2;
    }
    if (gpio_miso == P0_08) {
      map_miso.func = 2;
    }
    if (gpio_mosi == P0_09) {
      map_mosi.func = 2;
    }
    if (gpio_cs == P0_06) {
      map_cs.func = 2;
    }
    found = true;
  }
  if (found) {
    map_sck_out = map_sck;
    map_miso_out = map_miso;
    map_mosi_out = map_mosi;
    map_cs_out = map_cs;
  }
  return found;
}

struct gpioMapResult_t {
  uint8_t sspBusIdx;
  int gpio_sck;
  int gpio_miso;
  int gpio_mosi;
  int gpio_cs;
};

static void MapPortPinFunc(int pin, uint8_t func) {
#define _SWENT_MPPF(PS,PT,PN) case P##PT##_##PN: PINSEL##PS.p##PT##_##PN = func; break;
  switch(pin) {
  _SWENT_MPPF(0,0,00)_SWENT_MPPF(0,0,01)_SWENT_MPPF(0,0,02)_SWENT_MPPF(0,0,03)_SWENT_MPPF(0,0,04)_SWENT_MPPF(0,0,05)_SWENT_MPPF(0,0,06)_SWENT_MPPF(0,0,07)
  _SWENT_MPPF(0,0,08)_SWENT_MPPF(0,0,09)_SWENT_MPPF(0,0,10)_SWENT_MPPF(0,0,11)_SWENT_MPPF(0,0,15)
  _SWENT_MPPF(1,0,16)_SWENT_MPPF(1,0,17)_SWENT_MPPF(1,0,18)_SWENT_MPPF(1,0,19)_SWENT_MPPF(1,0,20)
  _SWENT_MPPF(1,0,21)_SWENT_MPPF(1,0,22)_SWENT_MPPF(1,0,23)_SWENT_MPPF(1,0,24)_SWENT_MPPF(1,0,25)
  _SWENT_MPPF(1,0,26)_SWENT_MPPF(1,0,27)_SWENT_MPPF(1,0,28)_SWENT_MPPF(1,0,29)_SWENT_MPPF(1,0,30)
  _SWENT_MPPF(2,1,00)_SWENT_MPPF(2,1,01)_SWENT_MPPF(2,1,04)_SWENT_MPPF(2,1,08)_SWENT_MPPF(2,1,09)_SWENT_MPPF(2,1,10)
  _SWENT_MPPF(2,1,14)_SWENT_MPPF(2,1,15)
  _SWENT_MPPF(3,1,16)_SWENT_MPPF(3,1,17)_SWENT_MPPF(3,1,18)_SWENT_MPPF(3,1,19)_SWENT_MPPF(3,1,20)
  _SWENT_MPPF(3,1,21)_SWENT_MPPF(3,1,22)_SWENT_MPPF(3,1,23)_SWENT_MPPF(3,1,24)_SWENT_MPPF(3,1,25)
  _SWENT_MPPF(3,1,26)_SWENT_MPPF(3,1,27)_SWENT_MPPF(3,1,28)_SWENT_MPPF(3,1,29)_SWENT_MPPF(3,1,30)
  _SWENT_MPPF(3,1,31)
  _SWENT_MPPF(4,2,00)_SWENT_MPPF(4,2,01)_SWENT_MPPF(4,2,02)_SWENT_MPPF(4,2,03)_SWENT_MPPF(4,2,04)
  _SWENT_MPPF(4,2,05)_SWENT_MPPF(4,2,06)_SWENT_MPPF(4,2,07)_SWENT_MPPF(4,2,08)_SWENT_MPPF(4,2,09)
  _SWENT_MPPF(4,2,10)_SWENT_MPPF(4,2,11)_SWENT_MPPF(4,2,12)_SWENT_MPPF(4,2,13)
  _SWENT_MPPF(7,3,25)_SWENT_MPPF(7,3,26)
  _SWENT_MPPF(9,4,28)_SWENT_MPPF(9,4,29)
  }
#undef _SWENT_MPPF
}

static void SetPortPinMode(int pin, uint8_t mode) {
#define _SWENT_MPPM(PM,PT,PN) case P##PT##_##PN: PINMODE##PM.p##PT##_##PN##mode = mode; break;
  switch(pin) {
  _SWENT_MPPM(0,0,00)_SWENT_MPPM(0,0,01)_SWENT_MPPM(0,0,02)_SWENT_MPPM(0,0,03)_SWENT_MPPM(0,0,04)_SWENT_MPPM(0,0,05)
  _SWENT_MPPM(0,0,06)_SWENT_MPPM(0,0,07)_SWENT_MPPM(0,0,08)_SWENT_MPPM(0,0,09)_SWENT_MPPM(0,0,10)_SWENT_MPPM(0,0,11)
  _SWENT_MPPM(0,0,15)
  _SWENT_MPPM(1,0,16)_SWENT_MPPM(1,0,17)_SWENT_MPPM(1,0,18)_SWENT_MPPM(1,0,19)_SWENT_MPPM(1,0,20)_SWENT_MPPM(1,0,21)
  _SWENT_MPPM(1,0,22)_SWENT_MPPM(1,0,23)_SWENT_MPPM(1,0,24)_SWENT_MPPM(1,0,25)_SWENT_MPPM(1,0,26)
  _SWENT_MPPM(2,1,00)_SWENT_MPPM(2,1,01)_SWENT_MPPM(2,1,04)_SWENT_MPPM(2,1,08)_SWENT_MPPM(2,1,09)_SWENT_MPPM(2,1,10)
  _SWENT_MPPM(2,1,14)_SWENT_MPPM(2,1,15)
  _SWENT_MPPM(3,1,16)_SWENT_MPPM(3,1,17)_SWENT_MPPM(3,1,18)_SWENT_MPPM(3,1,19)_SWENT_MPPM(3,1,20)_SWENT_MPPM(3,1,21)
  _SWENT_MPPM(3,1,22)_SWENT_MPPM(3,1,23)_SWENT_MPPM(3,1,24)_SWENT_MPPM(3,1,25)_SWENT_MPPM(3,1,26)_SWENT_MPPM(3,1,27)
  _SWENT_MPPM(3,1,28)_SWENT_MPPM(3,1,29)_SWENT_MPPM(3,1,30)_SWENT_MPPM(3,1,31)
  _SWENT_MPPM(4,2,00)_SWENT_MPPM(4,2,01)_SWENT_MPPM(4,2,02)_SWENT_MPPM(4,2,03)_SWENT_MPPM(4,2,04)_SWENT_MPPM(4,2,05)
  _SWENT_MPPM(4,2,06)_SWENT_MPPM(4,2,07)_SWENT_MPPM(4,2,08)_SWENT_MPPM(4,2,09)_SWENT_MPPM(4,2,10)_SWENT_MPPM(4,2,11)
  _SWENT_MPPM(4,2,12)_SWENT_MPPM(4,2,13)
  _SWENT_MPPM(7,3,25)_SWENT_MPPM(7,3,26)
  _SWENT_MPPM(9,4,28)_SWENT_MPPM(9,4,29)
  }
#undef _SWENT_MPPM
}

#if 0
static void SetPortPinDirection(int pin, uint8_t dir) {
#define _SWENT_SPPD_(PT,PNN,PN) case P##PT##_##PNN: FIO##PT##DIR.set(PN, dir); break;
#define _SWENT_SPPD(PT,PN) case P##PT##_##PN: FIO##PT##DIR.set(PN, dir); break;
  switch(pin) {
  _SWENT_SPPD_(0,00,0)_SWENT_SPPD_(0,01,1)_SWENT_SPPD_(0,02,2)_SWENT_SPPD_(0,03,3)_SWENT_SPPD_(0,04,4)_SWENT_SPPD_(0,05,5)_SWENT_SPPD_(0,06,6)
  _SWENT_SPPD_(0,07,7)_SWENT_SPPD_(0,08,8)_SWENT_SPPD_(0,09,9)_SWENT_SPPD(0,10)_SWENT_SPPD(0,11)_SWENT_SPPD(0,15)_SWENT_SPPD(0,16)
  _SWENT_SPPD(0,17)_SWENT_SPPD(0,18)_SWENT_SPPD(0,19)_SWENT_SPPD(0,20)_SWENT_SPPD(0,21)_SWENT_SPPD(0,22)_SWENT_SPPD(0,23)
  _SWENT_SPPD(0,24)_SWENT_SPPD(0,25)_SWENT_SPPD(0,26)_SWENT_SPPD(0,27)_SWENT_SPPD(0,28)_SWENT_SPPD(0,29)_SWENT_SPPD(0,30)
  _SWENT_SPPD_(1,00,0)_SWENT_SPPD_(1,01,1)_SWENT_SPPD_(1,04,4)_SWENT_SPPD_(1,08,8)_SWENT_SPPD_(1,09,9)_SWENT_SPPD(1,10)_SWENT_SPPD(1,14)
  _SWENT_SPPD(1,15)_SWENT_SPPD(1,16)_SWENT_SPPD(1,17)_SWENT_SPPD(1,18)_SWENT_SPPD(1,19)_SWENT_SPPD(1,20)_SWENT_SPPD(1,21)
  _SWENT_SPPD(1,22)_SWENT_SPPD(1,23)_SWENT_SPPD(1,24)_SWENT_SPPD(1,25)_SWENT_SPPD(1,26)_SWENT_SPPD(1,27)_SWENT_SPPD(1,28)
  _SWENT_SPPD(1,29)_SWENT_SPPD(1,30)_SWENT_SPPD(1,31)
  _SWENT_SPPD_(2,00,0)_SWENT_SPPD_(2,01,1)_SWENT_SPPD_(2,02,2)_SWENT_SPPD_(2,03,3)_SWENT_SPPD_(2,04,4)_SWENT_SPPD_(2,05,5)_SWENT_SPPD_(2,06,6)_SWENT_SPPD_(2,07,7)
  _SWENT_SPPD_(2,08,8)_SWENT_SPPD_(2,09,9)_SWENT_SPPD(2,10)_SWENT_SPPD(2,11)_SWENT_SPPD(2,12)_SWENT_SPPD(2,13)
  _SWENT_SPPD(3,25)_SWENT_SPPD(3,26)
  _SWENT_SPPD(4,28)_SWENT_SPPD(4,29)
  }
#undef _SWENT_SPPD
#undef _SWENT_SPPD_
}
#endif

static gpioMapResult_t SPIMapGPIO(int hint_sck, int hint_miso, int hint_mosi, int hint_cs) {
  // First try to find a direct mapping to SSP peripheral based on the given pin hints.
  uint8_t sspBusIdx;
  portMapResult_t map_sck, map_miso, map_mosi, map_cs;

  bool foundPortMap = SPIFindPortMapping(hint_sck, hint_miso, hint_mosi, hint_cs, sspBusIdx, map_sck, map_miso, map_mosi, map_cs);

  if (foundPortMap == false) {
    // Just select the default SSP config.
    sspBusIdx = 0;
    hint_sck = P0_15;
    map_sck.func = 2;
    hint_miso = P0_17;
    map_miso.func = 2;
    hint_mosi = P0_18;
    map_mosi.func = 2;
    hint_cs = P0_16;
    map_cs.func = 2;
  }

  // Map the functions that exist.
  if (map_sck.isMapped()) {
    MapPortPinFunc(hint_sck, map_sck.func);
    SetPortPinMode(hint_sck, LPC_PINMODE_NONE);
  }
  if (map_miso.isMapped()) {
    MapPortPinFunc(hint_miso, map_miso.func);
    SetPortPinMode(hint_miso, LPC_PINMODE_PULLDOWN);
  }
  if (map_mosi.isMapped()) {
    MapPortPinFunc(hint_mosi, map_mosi.func);
    SetPortPinMode(hint_mosi, LPC_PINMODE_NONE);
  }
#if 0
  if (map_cs.isMapped()) {
    MapPortPinFunc(hint_cs, map_cs.func);
    SetPortPinMode(hint_cs, LPC_PINMODE_NONE);
  }
#endif

  gpioMapResult_t res;
  res.sspBusIdx = sspBusIdx;
  res.gpio_sck = ( map_sck.isMapped() ? hint_sck : -1 );
  res.gpio_miso = ( map_miso.isMapped() ? hint_miso : -1 );
  res.gpio_mosi = ( map_mosi.isMapped() ? hint_mosi : -1 );
  res.gpio_cs = ( map_cs.isMapped() ? hint_cs : -1 );
  return res;
}

static void SPIUnmapGPIO(const gpioMapResult_t& res) {
  // Reset to architecture default configs on the pins that we previously mapped.
  if (res.gpio_sck >= 0) {
    MapPortPinFunc(res.gpio_sck, 0);
    SetPortPinMode(res.gpio_sck, LPC_PINMODE_PULLDOWN);
  }
  if (res.gpio_miso >= 0) {
    MapPortPinFunc(res.gpio_miso, 0);
    SetPortPinMode(res.gpio_miso, LPC_PINMODE_PULLDOWN);
  }
  if (res.gpio_mosi >= 0) {
    MapPortPinFunc(res.gpio_mosi, 0);
    SetPortPinMode(res.gpio_mosi, LPC_PINMODE_PULLDOWN);
  }
#if 0
  if (res.gpio_cs >= 0) {
    MapPortPinFunc(res.gpio_cs, 0);
    SetPortPinMode(res.gpio_cs, LPC_PINMODE_PULLDOWN);
  }
#endif
}

#define LPC_OSCRANGE_1_20_MHZ 0
#define LPC_OSCRANGE_15_25_MHZ 1

struct scs_reg_t {
  uint32_t reserved1 : 4;
  uint32_t OSCRANGE : 1;
  uint32_t OSCEN : 1;
  uint32_t OSCSTAT : 1;
  uint32_t reserved2 : 25;
};
static_assert(sizeof(scs_reg_t) == 4, "invalid size of LPC scs_reg_t");

static scs_reg_t& SCS = *(scs_reg_t*)0x400FC1A0;

#define LPC_CLKSRC_IRC 0      // 4MHz
#define LPC_CLKSRC_MAINOSC 1  // depending on OSCRANGE
#define LPC_CLKSRC_RTCOSC 2   // 32kHz

struct clksrcsel_reg_t {
  uint32_t CLKSRC : 2;
  uint32_t reserved1 : 30;
};
static_assert(sizeof(clksrcsel_reg_t) == 4, "invalid size of LPC clksrcsel_reg_t");

static clksrcsel_reg_t& CLKSRCSEL = *(clksrcsel_reg_t*)0x400FC10C;

struct pll0stat_reg_t {
  uint32_t MSEL0 : 15;      // M - 1
  uint32_t reserved1 : 1;
  uint32_t NSEL0 : 8;       // N - 1
  uint32_t PLLE0_STAT : 1;  // enable bit
  uint32_t PLLC0_STAT : 1;  // connect bit
  uint32_t PLOCK0 : 1;      // ready bit
  uint32_t reserved2 : 5;
};
static_assert(sizeof(pll0stat_reg_t) == 4, "invalid size of LPC pll0stat_reg_t");

static pll0stat_reg_t& PLL0STAT = *(pll0stat_reg_t*)0x400FC088;

struct cclkcfg_reg_t {
  uint32_t CCLKSEL : 8;
  uint32_t reserved1 : 24;
};
static_assert(sizeof(cclkcfg_reg_t) == 4, "invalid size of LPC cclkcfg_reg_t");

static cclkcfg_reg_t& CCLKCFG = *(cclkcfg_reg_t*)0x400FC104;

#define LPC_PCLKSEL_QUARTER 0
#define LPC_PCLKSEL_ONE 1
#define LPC_PCLKSEL_HALF 2
#define LPC_PCLKSEL_8_6 3 // 1/8 for generic, 1/6 for CAN1/2

struct pclksel0_reg_t {
  uint32_t PCLK_WDT : 2;
  uint32_t PCLK_TIMER0 : 2;
  uint32_t PCLK_TIMER1 : 2;
  uint32_t PCLK_UART0 : 2;
  uint32_t PCLK_UART1 : 2;
  uint32_t reserved1 : 2;
  uint32_t PCLK_PWM1 : 2;
  uint32_t PCLK_I2C0 : 2;
  uint32_t PCLK_SPI : 2;
  uint32_t reserved2 : 2;
  uint32_t PCLK_SSP1 : 2;
  uint32_t PCLK_DAC : 2;
  uint32_t PCLK_ADC : 2;
  uint32_t PCLK_CAN1 : 2;
  uint32_t PCLK_CAN2 : 2;
  uint32_t PCLK_ACF : 2;
};
static_assert(sizeof(pclksel0_reg_t) == 4, "invalid size of LPC pclksel0_reg_t");

static pclksel0_reg_t& PCLKSEL0 = *(pclksel0_reg_t*)0x400FC1A8;

struct pclksel1_reg_t {
  uint32_t PCLK_QEI : 2;
  uint32_t PCLK_GPIOINT : 2;
  uint32_t PCLK_PCB : 2;
  uint32_t PCLK_I2C1 : 2;
  uint32_t reserved1 : 2;
  uint32_t PCLK_SSP0 : 2;
  uint32_t PCLK_TIMER2 : 2;
  uint32_t PCLK_TIMER3 : 2;
  uint32_t PCLK_UART2 : 2;
  uint32_t PCLK_UART3 : 2;
  uint32_t PCLK_I2C2 : 2;
  uint32_t PCLK_I2S : 2;
  uint32_t reserved2 : 2;
  uint32_t PCLK_RIT : 2;
  uint32_t PCLK_SYSCON : 2;
  uint32_t PCLK_MC : 2;
};
static_assert(sizeof(pclksel1_reg_t) == 4, "invalid size of LPC pclksel1_reg_t");

static pclksel1_reg_t& PCLKSEL1 = *(pclksel1_reg_t*)0x400FC1AC;

// Enables or disables peripherals (power control for peripherals).
struct pconp_reg_t {
  uint32_t reserved1 : 1;
  uint32_t PCTIM0 : 1;
  uint32_t PCTIM1 : 1;
  uint32_t PCUART0 : 1;
  uint32_t PCUART1 : 1;
  uint32_t reserved2 : 1;
  uint32_t PCPWM1 : 1;
  uint32_t PCI2C0 : 1;
  uint32_t PCSPI : 1;
  uint32_t PCRTC : 1;
  uint32_t PCSSP1 : 1;
  uint32_t reserved3 : 1;
  uint32_t PCADC : 1;
  uint32_t PCCAN1 : 1;
  uint32_t PCCAN2 : 1;
  uint32_t PCGPIO : 1;
  uint32_t PCRIT : 1;
  uint32_t PCMCPWM : 1;
  uint32_t PCQEI : 1;
  uint32_t PCI2C1 : 1;
  uint32_t reserved4 : 1;
  uint32_t PCSSP0 : 1;
  uint32_t PCTIM2 : 1;
  uint32_t PCTIM3 : 1;
  uint32_t PCUART2 : 1;
  uint32_t PCUART3 : 1;
  uint32_t PCI2C2 : 1;
  uint32_t PC12S : 1;
  uint32_t reserved5 : 1;
  uint32_t PCGPDMA : 1;
  uint32_t PCENET : 1;
  uint32_t PCUSB : 1;
};
static_assert(sizeof(pconp_reg_t) == 4, "invalid size of LPC pconp_reg_t");

static pconp_reg_t& PCONP = *(pconp_reg_t*)0x400FC0C4;

static uint32_t GetCPUClockFrequency() {
  if (PLL0STAT.PLLE0_STAT == false || PLL0STAT.PLLC0_STAT == false) {
    // The CPU is running on the IRC.
    return 4000000;
  }

  uint32_t clksrc = 0;

  switch(CLKSRCSEL.CLKSRC) {
  case LPC_CLKSRC_IRC:
    clksrc = 4000000;
    break;
  case LPC_CLKSRC_MAINOSC:
    clksrc = LPC_MAINOSCILLATOR_FREQ;
    break;
  case LPC_CLKSRC_RTCOSC:
    clksrc = 32000;
    break;
  }

  uint32_t M = PLL0STAT.MSEL0 + 1; // M = 25
  uint32_t N = PLL0STAT.NSEL0 + 1; // N = 2

  uint32_t f_cco = (2 * M * clksrc) / N;

  uint32_t cclkdiv = CCLKCFG.CCLKSEL + 1;

  return (f_cco / cclkdiv);
}

struct sspClockResult_t {
  uint32_t pclk_ssp : 2;
  uint32_t scr : 8;
  uint32_t cpsdvsr : 8; // value between 2 and 254
};

static sspClockResult_t SPICalculateClock(uint32_t maxClockFreq) {
  uint32_t cpuFreq = GetCPUClockFrequency();

  if (maxClockFreq >= cpuFreq) {
    // Return the fastest clock.
    sspClockResult_t maxRes;
    maxRes.pclk_ssp = LPC_PCLKSEL_ONE;
    maxRes.scr = 0;
    maxRes.cpsdvsr = 2; // minimum value.
    return maxRes;
  }

  uint32_t DIV_ceil = ((cpuFreq + (maxClockFreq - 1)) / maxClockFreq);

  if (/* DIV_ceil >= 1 && */ DIV_ceil <= 256) {
    uint32_t rem_two = (DIV_ceil % 2);

    if (rem_two == 0 && DIV_ceil >= 2 && DIV_ceil <= 254) {
      sspClockResult_t accRes;
      accRes.pclk_ssp = LPC_PCLKSEL_ONE;
      accRes.scr = 0;
      accRes.cpsdvsr = DIV_ceil;
      return accRes;
    }
    else {
      // Return a very accurate SCR representation.
      sspClockResult_t accRes;
      accRes.pclk_ssp = LPC_PCLKSEL_ONE;
      accRes.scr = (DIV_ceil+rem_two)/2 - 1;
      accRes.cpsdvsr = 2; // minimum value.
      return accRes;
    }
  }

  // Brute-force find the clock result.
  // Still very fast, optimized using math.
  sspClockResult_t best;
  best.pclk_ssp = LPC_PCLKSEL_8_6;
  best.scr = 255;
  best.cpsdvsr = 254;
  uint32_t last_best_clockfreq = 0;
  bool has_result = false;

  uint32_t ps_ssp = 2;

  while (ps_ssp <= 8) {
    uint32_t ps_dvsr = (1<<7)*ps_ssp;

    while (ps_dvsr <= (254u*ps_ssp)) {
      uint32_t presc = (ps_dvsr*ps_ssp);
      uint32_t scr = (DIV_ceil/presc);
      uint32_t freq = (cpuFreq/(presc*scr));

      if (freq <= maxClockFreq) {
        if (has_result == false || last_best_clockfreq < freq) {
          last_best_clockfreq = freq;
          if (ps_ssp == 2)
            best.pclk_ssp = LPC_PCLKSEL_HALF;
          else if (ps_ssp == 4)
            best.pclk_ssp = LPC_PCLKSEL_QUARTER;
          else
            best.pclk_ssp = LPC_PCLKSEL_8_6;
          best.scr = (scr-1);
          best.cpsdvsr = ps_dvsr;
          has_result = true;
        }
      }

      ps_dvsr += ps_ssp*2;
    }

    ps_ssp *= 2;
  }

  return best;
}

struct sspNcr0_reg_t {
  uint32_t DSS : 4;
  uint32_t FRF : 2;
  uint32_t CPOL : 1;
  uint32_t CPHA : 1;
  uint32_t SCR : 8;
  uint32_t reserved1 : 16;
};
static_assert(sizeof(sspNcr0_reg_t) == 4, "invalid size of LPC sspNcr0_reg_t");

struct sspNcr1_reg_t {
  uint32_t LBM : 1;
  uint32_t SSE : 1;
  uint32_t MS : 1;
  uint32_t SOD : 1;
  uint32_t reserved1 : 28;
};
static_assert(sizeof(sspNcr1_reg_t) == 4, "invalid size of LPC sspNcr1_reg_t");

struct sspNdr_reg_t {
  uint32_t DATA : 16;
  uint32_t reserved1 : 16;
};
static_assert(sizeof(sspNdr_reg_t) == 4, "invalid size of LPC sspNdr_reg_t");

struct sspNsr_reg_t {
  uint32_t TFE : 1;
  uint32_t TNF : 1;
  uint32_t RNE : 1;
  uint32_t RFF : 1;
  uint32_t BSY : 1;
  uint32_t reserved1 : 27;
};
static_assert(sizeof(sspNsr_reg_t) == 4, "invalid size of LPC sspNsr_reg_t");

struct sspNcpsr_reg_t {
  uint32_t CPSDVSR : 8;
  uint32_t reserved1 : 24;
};
static_assert(sizeof(sspNcpsr_reg_t) == 4, "invalid size of LPC sspNcpsr_reg_t");

struct sspNimsc_reg_t {
  uint32_t RORIM : 1;
  uint32_t RTIM : 1;
  uint32_t RXIM : 1;
  uint32_t TXIM : 1;
  uint32_t reserved1 : 28;
};
static_assert(sizeof(sspNimsc_reg_t) == 4, "invalid size of LPC sspNimsc_reg_t");

struct sspNris_reg_t {
  uint32_t RORRIS : 1;
  uint32_t RTRIS : 1;
  uint32_t RXRIS : 1;
  uint32_t TXRIS : 1;
  uint32_t reserved1 : 28;
};
static_assert(sizeof(sspNris_reg_t) == 4, "invalid size of LPC sspNris_reg_t");

struct sspNmis_reg_t {
  uint32_t RORMIS : 1;
  uint32_t RTMIS : 1;
  uint32_t RXMIS : 1;
  uint32_t TXMIS : 1;
  uint32_t reserved1 : 28;
};
static_assert(sizeof(sspNmis_reg_t) == 4, "invalid size of LPC sspNmis_reg_t");

struct sspNicr_reg_t {
  uint32_t RORIC : 1;
  uint32_t RTIC : 1;
  uint32_t reserved1 : 30;
};
static_assert(sizeof(sspNicr_reg_t) == 4, "invalid size of LPC sspNicr_reg_t");

struct sspNdmacr_reg_t {
  uint32_t RXDMAE : 1;
  uint32_t TXDMAE : 1;
  uint32_t reserved1 : 30;
};
static_assert(sizeof(sspNdmacr_reg_t) == 4, "invalid size of LPC sspNdmacr_reg_t");

struct ssp_dev_t {
  sspNcr0_reg_t CR0;
  sspNcr1_reg_t CR1;
  sspNdr_reg_t DR;
  sspNsr_reg_t SR;
  sspNcpsr_reg_t CPSR;
  sspNimsc_reg_t IMSC;
  sspNris_reg_t RIS;
  sspNmis_reg_t MIS;
  sspNicr_reg_t ICR;
  sspNdmacr_reg_t DMACR;
};
static_assert(sizeof(ssp_dev_t) == 40, "invalid size of LPC ssp_dev_t");

static volatile ssp_dev_t& SSP0 = *(volatile ssp_dev_t*)0x40088000;
static volatile ssp_dev_t& SSP1 = *(volatile ssp_dev_t*)0x40030000;

inline volatile ssp_dev_t& SPIGetBusFromIndex(uint8_t idx) {
  if (idx == 0) return SSP0;
  if (idx == 1) return SSP1;
  return SSP0;  // default
}

inline uint8_t SPIGetBusIndex(volatile ssp_dev_t& SSP) {
  if (&SSP == &SSP0) return 0;
  if (&SSP == &SSP1) return 1;
  return 0; // default
}

#ifndef HALSPI_DISABLE_DMA

struct DMACIntStat_reg_t {
  uint32_t IntStat : 8;
  uint32_t reserved1 : 24;
};
static_assert(sizeof(DMACIntStat_reg_t) == 4, "invalid size of LPC DMACIntStat_reg_t");

static DMACIntStat_reg_t& DMACIntStat = *(DMACIntStat_reg_t*)0x50004000;

struct DMACIntTCStat_reg_t {
  uint32_t IntTCStat : 8;
  uint32_t reserved1 : 24;
};
static_assert(sizeof(DMACIntTCStat_reg_t) == 4, "invalid size of LPC DMACIntTCStat_reg_t");

static DMACIntTCStat_reg_t& DMACIntTCStat = *(DMACIntTCStat_reg_t*)0x50004004;

struct DMACIntTCClear_reg_t {
  uint32_t IntTCClear : 8;
  uint32_t reserved1 : 24;
};
static_assert(sizeof(DMACIntTCClear_reg_t) == 4, "invalid size of LPC DMAIntTCClear_reg_t");

static DMACIntTCClear_reg_t& DMACIntTCClear = *(DMACIntTCClear_reg_t*)0x50004008;

struct DMACIntErrStat_reg_t {
  uint32_t IntErrStat : 8;
  uint32_t reserved1 : 24;
};
static_assert(sizeof(DMACIntErrStat_reg_t) == 4, "invalid size of LPC DMACIntErrStat_reg_t");

static DMACIntErrStat_reg_t& DMACIntErrStat = *(DMACIntErrStat_reg_t*)0x5000400C;

struct DMACIntErrClr_reg_t {
  uint32_t IntErrClr : 8;
  uint32_t reserved1 : 24;
};
static_assert(sizeof(DMACIntErrClr_reg_t) == 4, "invalid size of LPC DMACIntErrClr_reg_t");

static DMACIntErrClr_reg_t& DMACIntErrClr = *(DMACIntErrClr_reg_t*)0x50004010;

struct DMACRawIntTCStat_reg_t {
  uint32_t RawIntTCStat : 8;
  uint32_t reserved1 : 24;
};
static_assert(sizeof(DMACRawIntTCStat_reg_t) == 4, "invalid size of LPC DMACRawIntTCStat_reg_t");

static DMACRawIntTCStat_reg_t& DMACRawIntTCStat = *(DMACRawIntTCStat_reg_t*)0x50004014;

struct DMACRawIntErrStat_reg_t {
  uint32_t RawIntErrStat : 8;
  uint32_t reserved1 : 24;
};
static_assert(sizeof(DMACRawIntErrStat_reg_t) == 4, "invalid size of LPC DMACRawIntErrStat_reg_t");

static DMACRawIntErrStat_reg_t& DMACRawIntErrStat = *(DMACRawIntErrStat_reg_t*)0x50004018;

struct DMACEnbldChns_reg_t {
  uint32_t EnabledChannels : 8;
  uint32_t reserved1 : 24;
};
static_assert(sizeof(DMACEnbldChns_reg_t) == 4, "invalid size of LPC DMACEnbldChns_reg_t");

static DMACEnbldChns_reg_t& DMACEnbldChns = *(DMACEnbldChns_reg_t*)0x5000401C;

struct DMACSoftBReq_reg_t {
  uint32_t SoftBReq : 16;
  uint32_t reserved1 : 16;
};
static_assert(sizeof(DMACSoftBReq_reg_t) == 4, "invalid size of LPC DMACSoftBReq_reg_t");

static DMACSoftBReq_reg_t& DMACSoftBReq = *(DMACSoftBReq_reg_t*)0x50004020;

struct DMACSoftSReq_reg_t {
  uint32_t SoftSReq : 16;
  uint32_t reserved1 : 16;
};
static_assert(sizeof(DMACSoftSReq_reg_t) == 4, "invalid size of LPC DMACSoftSReq_reg_t");

static DMACSoftSReq_reg_t& DMACSoftSReq = *(DMACSoftSReq_reg_t*)0x50004024;

struct DMACSoftLBReq_reg_t {
  uint32_t SoftLBReq : 16;
  uint32_t reserved1 : 16;
};
static_assert(sizeof(DMACSoftLBReq_reg_t) == 4, "invalid size of LPC DMACSoftLBReq_reg_t");

static DMACSoftLBReq_reg_t& DMACSoftLBReq = *(DMACSoftLBReq_reg_t*)0x50004028;

struct DMACSoftLSReq_reg_t {
  uint32_t SoftLSReq : 16;
  uint32_t reserved1 : 16;
};
static_assert(sizeof(DMACSoftLSReq_reg_t) == 4, "invalid size of LPC DMACSoftLSReq_reg_t");

static DMACSoftLSReq_reg_t& DMACSoftLSReq = *(DMACSoftLSReq_reg_t*)0x5000402C;

struct DMACConfig_reg_t {
  uint32_t E : 1;
  uint32_t M : 1;
  uint32_t reserved1 : 30;
};
static_assert(sizeof(DMACConfig_reg_t) == 4, "invalid size of LPC DMACConfig_reg_t");

static DMACConfig_reg_t& DMACConfig = *(DMACConfig_reg_t*)0x50004030;

struct DMACSync_reg_t {
  uint32_t DMACSync : 16;
  uint32_t reserved1 : 16;
};
static_assert(sizeof(DMACSync_reg_t) == 4, "invalid size of LPC DMACSync_reg_t");

static DMACSync_reg_t& DMACSync = *(DMACSync_reg_t*)0x50004034;

struct DMAReqSel_reg_t {
  uint32_t DMASEL08 : 1;
  uint32_t DMASEL09 : 1;
  uint32_t DMASEL10 : 1;
  uint32_t DMASEL11 : 1;
  uint32_t DMASEL12 : 1;
  uint32_t DMASEL13 : 1;
  uint32_t DMASEL14 : 1;
  uint32_t DMASEL15 : 1;
  uint32_t reserved : 24;
};
static_assert(sizeof(DMAReqSel_reg_t) == 4, "invalid size of LPC DMAReqSel_reg_t");

static DMAReqSel_reg_t& DMAReqSel = *(DMAReqSel_reg_t*)0x400FC1C4;

struct DMACCxLLI_reg_t {
  uint32_t reserved1 : 2;
  uint32_t LLI : 30;
};
static_assert(sizeof(DMACCxLLI_reg_t) == 4, "invalid size of LPC DMACCxLLI_reg_t");

struct DMACCxControl_reg_t {
  uint32_t TransferSize : 12;
  uint32_t SBSize : 3;
  uint32_t DBSize : 3;
  uint32_t SWidth : 3;
  uint32_t DWidth : 3;
  uint32_t reserved1 : 2;
  uint32_t SI : 1;
  uint32_t DI : 1;
  uint32_t Prot1 : 1;
  uint32_t Prot2 : 1;
  uint32_t Prot3 : 1;
  uint32_t I : 1;
};
static_assert(sizeof(DMACCxControl_reg_t) == 4, "invalid size of LPC DMACCxControl_reg_t");

struct DMACCxConfig_reg_t {
  uint32_t E : 1;
  uint32_t SrcPeripheral : 5;
  uint32_t DestPeripheral : 5;
  uint32_t TransferType : 3;
  uint32_t IE : 1;
  uint32_t ITC : 1;
  uint32_t L : 1;
  uint32_t A : 1;
  uint32_t H : 1;
  uint32_t reserved1 : 13;
};
static_assert(sizeof(DMACCxConfig_reg_t) == 4, "invalid size of LPC DMACCxConfig_reg_t");

struct DMACChannel_dev_t {
  uint32_t SrcAddr;
  uint32_t DestAddr;
  DMACCxLLI_reg_t LLI;
  DMACCxControl_reg_t Control;
  DMACCxConfig_reg_t Config;
};
static_assert(sizeof(DMACChannel_dev_t) == 20, "invalid size of LPC DMACChannel_dev_t");

static DMACChannel_dev_t& DMACC0 = *(DMACChannel_dev_t*)0x50004100;
static DMACChannel_dev_t& DMACC1 = *(DMACChannel_dev_t*)0x50004120;
static DMACChannel_dev_t& DMACC2 = *(DMACChannel_dev_t*)0x50004140;
static DMACChannel_dev_t& DMACC3 = *(DMACChannel_dev_t*)0x50004160;
static DMACChannel_dev_t& DMACC4 = *(DMACChannel_dev_t*)0x50004180;
static DMACChannel_dev_t& DMACC5 = *(DMACChannel_dev_t*)0x500041A0;
static DMACChannel_dev_t& DMACC6 = *(DMACChannel_dev_t*)0x500041C0;
static DMACChannel_dev_t& DMACC7 = *(DMACChannel_dev_t*)0x500041E0;

static DMACChannel_dev_t& DMAGetChannel(uint32_t idx) {
  switch(idx) {
  case 0: return DMACC0;
  case 1: return DMACC1;
  case 2: return DMACC2;
  case 3: return DMACC3;
  case 4: return DMACC4;
  case 5: return DMACC5;
  case 6: return DMACC6;
  case 7: return DMACC7;
  }
  return DMACC0;  // default.
}

static uint32_t __attribute__((unused)) DMAGetChannelIndex(DMACChannel_dev_t& DMACC) {
  if (&DMACC == &DMACC0) return 0;
  if (&DMACC == &DMACC1) return 1;
  if (&DMACC == &DMACC2) return 2;
  if (&DMACC == &DMACC3) return 3;
  if (&DMACC == &DMACC4) return 4;
  if (&DMACC == &DMACC5) return 5;
  if (&DMACC == &DMACC6) return 6;
  if (&DMACC == &DMACC7) return 7;
  return 0; // default.
}

#ifdef HAL_SPI_SUPPORTS_ASYNC

struct DMACCxLLI_desc_t {
  uint32_t SrcAddr;
  uint32_t DestAddr;
  DMACCxLLI_desc_t *Next;
  DMACCxControl_reg_t Control;
};
static_assert(sizeof(DMACCxLLI_desc_t) == 16, "invalid size of LPC DMACCxLLI_desc_t");

struct DMACCxLLI_desc_user_t : public DMACCxLLI_desc_t {
  // User data by software.
  bool available = true;
};

#ifndef HALSPI_LPC_STATIC_DMADESCS
#define HALSPI_LPC_STATIC_DMADESCS 3
#endif

static DMACCxLLI_desc_user_t _available_dma_descs[HALSPI_LPC_STATIC_DMADESCS];

static DMACCxLLI_desc_user_t* DMAFindFreeChainLLI() {
  for (auto& item : _available_dma_descs) {
    if (item.available) {
      return &item;
    }
  }
  return nullptr;
}

struct dma_process_t {
  DMACChannel_dev_t *current_DMACC;
  const void *current_buffer;
  size_t curoff;
  size_t txlen;
  uint8_t txunitsize;
  void (*completeCallback)(void*);
  void *complete_ud;
  DMACCxLLI_desc_user_t *last_chain;
  bool is_active = false;
};

static dma_process_t _dma_async_proc;

static void DMAProgramSSPChain(volatile ssp_dev_t& SSP, dma_process_t& proc) {
  DMACCxLLI_desc_user_t *first = nullptr;
  DMACCxLLI_desc_user_t *last = nullptr;

  uint32_t txwidth = 0;

  if (proc.txunitsize == 1) {
    txwidth = 0;
  }
  else if (proc.txunitsize == 2) {
    txwidth = 1;
  }
  else if (proc.txunitsize == 4) {
    txwidth = 2;
  }
  else
    _spi_on_error(4);

  DMACCxControl_reg_t Control;
  Control.SBSize = 0;
  Control.DBSize = 0;
  Control.SWidth = txwidth;
  Control.DWidth = txwidth;
  Control.SI = true;
  Control.DI = false;
  Control.Prot1 = 1;
  Control.Prot2 = 0;
  Control.Prot3 = 1;
  Control.I = false;

  bool init_ch_prog = false;

  auto& DMACC = *proc.current_DMACC;
  DMACC.Config.ITC = true;

  while (proc.curoff < proc.txlen) {
    size_t left = (proc.txlen - proc.curoff);
    size_t takecnt = (uint32_t)__MIN <size_t> (left, (1<<12)-1);

    Control.TransferSize = takecnt;

    uint32_t SrcAddr = ( (uint32_t)proc.current_buffer + proc.curoff * proc.txunitsize );
    uint32_t DestAddr = (uint32_t)&SSP.DR;

    if (init_ch_prog == false) {
      // We first have to program the channel itself.
      DMACC.SrcAddr = SrcAddr;
      DMACC.DestAddr = DestAddr;
      DMACC.LLI.LLI = 0;
      DMACC.Control = Control;
      init_ch_prog = true;
    }
    else {
      auto *freelli = DMAFindFreeChainLLI();

      if (freelli == nullptr) break;

      freelli->SrcAddr = SrcAddr;
      freelli->DestAddr = DestAddr;
      freelli->Next = nullptr;
      freelli->Control = Control;
      freelli->available = false;

      if (first == nullptr) {
        first = freelli;
        DMACC.LLI.LLI = ( (uint32_t)freelli >> 2 );
      }

      if (last) {
        last->Next = freelli;
      }
      last = freelli;
    }

    proc.curoff += takecnt;
  }

  if (last) {
    last->Control.I = true;
  }
  else {
    DMACC.Control.I = true;
  }

  proc.last_chain = first;
}

#endif //HAL_SPI_SUPPORTS_ASYNC

#endif //HALSPI_DISABLE_DMA

} // namespace MarlinLPC

  // Hardware SPI

#ifndef HALSPI_LOOPBEEP_TIMEOUT
#define HALSPI_LOOPBEEP_TIMEOUT 3000
#endif

  struct spi_monitored_loop
  {
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
      OUT_WRITE(BEEPER_PIN, HIGH);
      delay(500);
      OUT_WRITE(BEEPER_PIN, LOW);
      delay(200);
      OUT_WRITE(BEEPER_PIN, HIGH);
      delay(200);
      OUT_WRITE(BEEPER_PIN, LOW);
      delay(200);
      OUT_WRITE(BEEPER_PIN, HIGH);
      delay(200);
      OUT_WRITE(BEEPER_PIN, LOW);
      delay(1000);
      for (unsigned int n = 0; n < beep_code; n++) {
        OUT_WRITE(BEEPER_PIN, HIGH);
        delay(200);
        OUT_WRITE(BEEPER_PIN, LOW);
        delay(200);
      }
      delay(800);
      OUT_WRITE(BEEPER_PIN, HIGH);
      delay(1000);
      OUT_WRITE(BEEPER_PIN, LOW);
      delay(2000);
#endif
    }
  };

  static MarlinLPC::gpioMapResult_t _ssp_gpioMap;
  static volatile bool _ssp_is_active = false;
  static volatile bool _ssp_transaction_is_running = false;
  static volatile int _ssp_cs_pin;
  static volatile int _ssp_bitOrder;
  static volatile int _ssp_clockMode;
  static volatile uint8_t _ssp_framesize;
  static volatile bool _ssp_dirty_rxbuffer;

#ifdef HAL_SPI_SUPPORTS_ASYNC

  struct ssp_process_t {
    volatile MarlinLPC::ssp_dev_t *current_ssp;
    const void *current_buffer;
    size_t curoff_bytes;
    size_t txlen_bytes;
    uint8_t txunitsize;
    bool is_active = false;
    void (*completeCallback)(void*);
    void *complete_ud;
  };

  static ssp_process_t _ssp_async_proc;

#endif //HAL_SPI_SUPPORTS_ASYNC

  static void _spiAsyncBarrier() {
#ifdef HAL_SPI_SUPPORTS_ASYNC
    while (_ssp_async_proc.is_active) { /* wait until completion of any async SPI TX */ }
#ifndef HALSPI_DISABLE_DMA
    while (MarlinLPC::_dma_async_proc.is_active) { /* wait until completion of any async DMA TX */ }
#endif
#endif
  }

  void spiBegin() {
    #if PIN_EXISTS(SD_SS)
      OUT_WRITE(SD_SS_PIN, HIGH);
    #endif

    // Turn off peripherals by default.
    MarlinLPC::PCONP.PCSPI = false;
    MarlinLPC::PCONP.PCSSP0 = false;
    MarlinLPC::PCONP.PCSSP1 = false;
    MarlinLPC::PCONP.PCGPDMA = false;
  }

  static void _spiConfigClock(volatile MarlinLPC::ssp_dev_t& SSP, int clockMode) {
    if (clockMode == SPI_CLKMODE_0) {
      SSP.CR0.CPOL = false;
      SSP.CR0.CPHA = false;
    }
    else if (clockMode == SPI_CLKMODE_1) {
      SSP.CR0.CPOL = false;
      SSP.CR0.CPHA = true;
    }
    else if (clockMode == SPI_CLKMODE_2) {
      SSP.CR0.CPOL = true;
      SSP.CR0.CPHA = false;
    }
    else if (clockMode == SPI_CLKMODE_3) {
      SSP.CR0.CPOL = true;
      SSP.CR0.CPHA = true;
    }
    _ssp_clockMode = clockMode;
  }

  void spiInitEx(uint32_t maxClockFreq, int hint_sck, int hint_miso, int hint_mosi, int hint_cs) {
#ifdef HAL_SPI_SUPPORTS_ASYNC
    while (_ssp_is_active) { /* wait until completion of any other SPI activity */ }
#else
    if (_ssp_is_active)
      _spi_on_error(1);
#endif

    _ssp_gpioMap = MarlinLPC::SPIMapGPIO(hint_sck, hint_miso, hint_mosi, -1);
    _ssp_is_active = true;
    _ssp_cs_pin = hint_cs;

    MarlinLPC::sspClockResult_t clockRes = MarlinLPC::SPICalculateClock(maxClockFreq);

    uint8_t sspBusIdx = _ssp_gpioMap.sspBusIdx;
    auto& SSP = MarlinLPC::SPIGetBusFromIndex(sspBusIdx);

    if (sspBusIdx == 0) {
      MarlinLPC::PCONP.PCSSP0 = true;
      MarlinLPC::PCLKSEL1.PCLK_SSP0 = clockRes.pclk_ssp;
    }
    else if (sspBusIdx == 1) {
      MarlinLPC::PCONP.PCSSP1 = true;
      MarlinLPC::PCLKSEL0.PCLK_SSP1 = clockRes.pclk_ssp;
    }
    SSP.CPSR.CPSDVSR = clockRes.cpsdvsr;
    SSP.CR0.DSS = 7;  // 8 bit transfer
    SSP.CR0.SCR = clockRes.scr;
    SSP.CR0.FRF = 0;  // SPI
    _spiConfigClock(SSP, SPI_CLKMODE_DEFAULT);
    SSP.CR1.LBM = 0;
    SSP.CR1.SSE = 0;
    SSP.CR1.MS = 0; // master

    _ssp_transaction_is_running = false;
    _ssp_bitOrder = SPI_BITORDER_DEFAULT; // SSP peripheral has no bit-order specification support (legacy SPI peripheral only)
    _ssp_dirty_rxbuffer = false;
    _ssp_framesize = sizeof(uint8_t); // default (DSS = 7).
  }

  void spiInit(uint8_t spiRate, int hint_sck, int hint_miso, int hint_mosi, int hint_cs) {
    // Use datarates Marlin uses
    uint32_t clock;
    switch (spiRate) {
      case SPI_FULL_SPEED:    clock = 20000000; break; // 13.9mhz=20000000  6.75mhz=10000000  3.38mhz=5000000  .833mhz=1000000
      case SPI_HALF_SPEED:    clock =  5000000; break;
      case SPI_QUARTER_SPEED: clock =  2500000; break;
      case SPI_EIGHTH_SPEED:  clock =  1250000; break;
      case SPI_SPEED_5:       clock =   625000; break;
      case SPI_SPEED_6:       clock =   300000; break;
      default:
        clock = 4000000; // Default from the SPI library
    }
    spiInitEx(clock, hint_sck, hint_miso, hint_mosi, hint_cs);
  }

  static void _spiStart() {
    auto& SSP = MarlinLPC::SPIGetBusFromIndex(_ssp_gpioMap.sspBusIdx);
    SSP.CR1.SSE = true;
    if (_ssp_cs_pin >= 0) {
      MarlinLPC::MapPortPinFunc(_ssp_cs_pin, 0);  // make sure that the CS pin is configured as GPIO.
      OUT_WRITE(_ssp_cs_pin, LOW);
    }
  }
  static void _spiEnd() {
    auto& SSP = MarlinLPC::SPIGetBusFromIndex(_ssp_gpioMap.sspBusIdx);
    spi_monitored_loop tfew;
    while (SSP.SR.TFE == false) { tfew.update(1); /* wait until all items from the TX queue were pushed */ }
    spi_monitored_loop bsyw;
    while (SSP.SR.BSY) { bsyw.update(2); /* wait until the current data transfer has finished (clean shutdown) */ }
    if (_ssp_cs_pin >= 0) {
      OUT_WRITE(_ssp_cs_pin, HIGH);
    }
    SSP.CR1.SSE = false;
  }

  static void _maybe_start_transaction() {
    if (_ssp_transaction_is_running == false) {
      _spiStart();
      _ssp_transaction_is_running = true;
    }
  }
  
  void spiClose() {
    if (_ssp_is_active == false)
      _spi_on_error(2);

    _spiAsyncBarrier();

    uint8_t sspBusIdx = _ssp_gpioMap.sspBusIdx;

    if (_ssp_transaction_is_running) {
      _spiEnd();
      _ssp_transaction_is_running = false;
    }

    if (sspBusIdx == 0) {
      MarlinLPC::PCONP.PCSSP0 = false;
    }
    else if (sspBusIdx == 1) {
      MarlinLPC::PCONP.PCSSP1 = false;
    }

    MarlinLPC::SPIUnmapGPIO(_ssp_gpioMap);

    _ssp_is_active = false;
  }

  void spiSetBitOrder(int bitOrder) {
    if (_ssp_bitOrder != bitOrder) {
      _spiAsyncBarrier();

      if (_ssp_transaction_is_running) {
        _spiEnd();
      }

      _ssp_bitOrder = bitOrder;

      if (_ssp_transaction_is_running) {
        _spiStart();
      }
    }
  }

  void spiSetClockMode(int clockMode) {
    if (_ssp_clockMode != clockMode) {
      _spiAsyncBarrier();

      if (_ssp_transaction_is_running) {
        _spiEnd();
      }

      auto& SSP = MarlinLPC::SPIGetBusFromIndex(_ssp_gpioMap.sspBusIdx);

      _spiConfigClock(SSP, clockMode);

      if (_ssp_transaction_is_running) {
        _spiStart();
      }
    }
  }

  void spiEstablish() {
    _maybe_start_transaction();
  }

  // Internal.
  inline void _spiSetFrameSize(uint8_t fsize) {
    if (_ssp_framesize != fsize) {
      _spiAsyncBarrier();

      if (_ssp_transaction_is_running) {
        _spiEnd();
      }

      auto& SSP = MarlinLPC::SPIGetBusFromIndex(_ssp_gpioMap.sspBusIdx);

      SSP.CR0.DSS = (fsize * 8)-1;
      _ssp_framesize = fsize;

      if (_ssp_transaction_is_running) {
        _spiStart();
      }
    }
  }

  // Unsigned integer only.
  template <typename numberType>
  inline numberType _flip_bits(numberType v) {
    numberType result = 0;
    for (unsigned int n = 0; n < (sizeof(numberType)*8); n++)
    {
      result <<= 1;
      bool bitval = ( v & (1<<n) ) != 0;
      result |= bitval;
    }
    return result;
  }

  template <typename numberType>
  inline void _spi_push_to_queue(volatile MarlinLPC::ssp_dev_t& SSP, numberType val) {
    bool revbits = (_ssp_bitOrder == SPI_BITORDER_LSB);
    if (_ssp_framesize == 1) {
      // Push it byte-by-byte (DSS = 7).
      uint32_t num_bytes = sizeof(numberType);

      bool revbytes = (_ssp_bitOrder == SPI_BITORDER_MSB);

      for (uint32_t n = 0; n < num_bytes; n++) {
        uint32_t byte_idx;

        if (revbytes) {
          byte_idx = (num_bytes-1) - n;
        }
        else {
          byte_idx = 0;
        }
        uint32_t bitidx = byte_idx * 8;

        spi_monitored_loop tnfw;
        while (SSP.SR.TNF == false) { tnfw.update(3); /* wait for space on the TX FIFO */ }

        uint8_t byteval = (val >> bitidx) & 0xFF;

        if (revbits)
          byteval = _flip_bits(byteval);

        SSP.DR.DATA = byteval;
      }
    }
    else if (_ssp_framesize == 2) {
      spi_monitored_loop tnfw;
      while (SSP.SR.TNF == false) { tnfw.update(3); /* wait for space om´n the TX FIFO */ }

      if (revbits) {
        val = _flip_bits(val);
      }

      // The number size must match the framesize.
      SSP.DR.DATA = val;
    }
  }

  void spiSend(uint8_t val) {
    if (_ssp_gpioMap.gpio_mosi < 0) return;

    _spiSetFrameSize(sizeof(val));

    _maybe_start_transaction();

    _spiAsyncBarrier();

    auto& SSP = MarlinLPC::SPIGetBusFromIndex(_ssp_gpioMap.sspBusIdx);

    _spi_push_to_queue(SSP, val);

    // Ignore read buffer; it will be flushed if required.
    _ssp_dirty_rxbuffer = true;
  }

  void spiSend16(uint16_t val) {
    if (_ssp_gpioMap.gpio_mosi < 0) return;

    _spiSetFrameSize(sizeof(val));

    _maybe_start_transaction();

    _spiAsyncBarrier();

    auto& SSP = MarlinLPC::SPIGetBusFromIndex(_ssp_gpioMap.sspBusIdx);

    _spi_push_to_queue(SSP, val);

    // Ignore read buffer; it will be flushed if required.
    _ssp_dirty_rxbuffer = true;
  }

  static void _spi_flush_rxqueue(volatile MarlinLPC::ssp_dev_t& SSP) {
    if (_ssp_dirty_rxbuffer == false) return;

    spi_monitored_loop tfew;
    while (SSP.SR.TFE == false) { tfew.update(4); /* wait until any tx data has been acquired that is still left */ }
    spi_monitored_loop bsyw;
    while (SSP.SR.BSY) { bsyw.update(5); /* wait until transfers and their parallel receives have finished */ }
    spi_monitored_loop rnew;
    while (SSP.SR.RNE) {
      rnew.update(6);
      auto unused = SSP.DR.DATA;
      (void)unused;
    }
    _ssp_dirty_rxbuffer = false;
  }

  template <typename numberType>
  inline numberType _spi_fetch_from_queue(volatile MarlinLPC::ssp_dev_t& SSP) {
    numberType result = 0;
    bool revbits = (_ssp_bitOrder == SPI_BITORDER_MSB);

    if (_ssp_framesize == 1) {
      // Fetch it byte-by-byte (DSS = 7).
      uint32_t num_bytes = sizeof(numberType);

      bool revbytes = (_ssp_bitOrder == SPI_BITORDER_LSB);

      for (uint32_t n = 0; n < num_bytes; n++) {
        uint32_t byte_idx;

        if (revbytes) {
          byte_idx = (num_bytes-1) - n;
        }
        else {
          byte_idx = n;
        }
        uint32_t bitidx = byte_idx * 8;

        spi_monitored_loop rnew;
        while (SSP.SR.RNE == false) { rnew.update(7); /* wait until we have received any data */ }

        uint8_t byteval = SSP.DR.DATA & 0xFF;

        if (revbits) {
          byteval = _flip_bits(byteval);
        }

        result |= ( (numberType)byteval << bitidx );
      }
    }
    else if (_ssp_framesize == 2) {
      spi_monitored_loop rnew;
      while (SSP.SR.RNE == false) { rnew.update(7); /* wait until we have received any data */ }

      result = SSP.DR.DATA;

      if (revbits) {
        result = _flip_bits(result);
      }
    }
    return result;
  }

  uint8_t spiRec(uint8_t txval) {
    if (_ssp_gpioMap.gpio_miso < 0) {
      spiSend(txval);
      return 0;
    }

    _spiSetFrameSize(sizeof(uint8_t));

    _maybe_start_transaction();

    _spiAsyncBarrier();

    auto& SSP = MarlinLPC::SPIGetBusFromIndex(_ssp_gpioMap.sspBusIdx);

    _spi_flush_rxqueue(SSP);

    _spi_push_to_queue(SSP, txval);

    return _spi_fetch_from_queue <uint8_t> (SSP);
  }

  uint16_t spiRec16(uint16_t txval) {
    if (_ssp_gpioMap.gpio_miso < 0) return 0;

    _spiSetFrameSize(sizeof(uint16_t));

    _maybe_start_transaction();

    _spiAsyncBarrier();

    auto& SSP = MarlinLPC::SPIGetBusFromIndex(_ssp_gpioMap.sspBusIdx);

    _spi_flush_rxqueue(SSP);

    _spi_push_to_queue(SSP, txval);

    return _spi_fetch_from_queue <uint16_t> (SSP);
  }

  void spiRead(uint8_t *buf, uint16_t cnt, uint8_t txval) {
    if (cnt == 0) return;
    if (_ssp_gpioMap.gpio_miso < 0) {
      spiWriteRepeat(txval, cnt);
      return;
    }

    _spiSetFrameSize(sizeof(uint8_t));

    _maybe_start_transaction();

    _spiAsyncBarrier();

    auto& SSP = MarlinLPC::SPIGetBusFromIndex(_ssp_gpioMap.sspBusIdx);

    _spi_flush_rxqueue(SSP);

    for (uint16_t n = 0; n < cnt; n++) {
      _spi_push_to_queue(SSP, txval);
      buf[n] = _spi_fetch_from_queue <uint8_t> (SSP);
    }
  }

  void spiSendBlock(uint8_t token, const uint8_t *buf) {
    if (_ssp_gpioMap.gpio_mosi < 0) return;

    _spiSetFrameSize(sizeof(uint8_t));

    _maybe_start_transaction();

    _spiAsyncBarrier();

    auto& SSP = MarlinLPC::SPIGetBusFromIndex(_ssp_gpioMap.sspBusIdx);

    _spi_push_to_queue(SSP, token);

    for (uint16_t n = 0; n < 512; n++) {
      _spi_push_to_queue(SSP, buf[n]);
    }

    _ssp_dirty_rxbuffer = true;
  }

#ifndef HALSPI_DISABLE_DMA

static void _dmaStart() {
  MarlinLPC::PCONP.PCGPDMA = true;
  MarlinLPC::DMACConfig.E = true;
  MarlinLPC::DMACConfig.M = 0;
}

static void _dmaEnd() {
  MarlinLPC::DMACConfig.E = false;
  MarlinLPC::PCONP.PCGPDMA = false;
}

static void _dmacInitSSP(MarlinLPC::DMACChannel_dev_t& DMACC, uint8_t sspBusIdx) {
  if (sspBusIdx == 0) {
    DMACC.Config.DestPeripheral = 0;
  }
  else if (sspBusIdx == 1) {
    DMACC.Config.DestPeripheral = 2;
  }
  DMACC.Config.TransferType = 1; // memory to peripheral
  DMACC.Config.IE = false;
  DMACC.Config.ITC = false;
  DMACC.Config.L = false;
  DMACC.Config.H = false;
}

template <typename numberType>
inline void _dmaSendBlocking(const numberType *buf, uint32_t cnt) {
  auto& SSP = MarlinLPC::SPIGetBusFromIndex(_ssp_gpioMap.sspBusIdx);

  uint32_t txwidth = 0;

  if (sizeof(numberType) == 1) {
    txwidth = 0;
  }
  else if (sizeof(numberType) == 2) {
    txwidth = 1;
  }
  else if (sizeof(numberType) == 4) {
    txwidth = 2;
  }
  else
    _spi_on_error(5);

  auto& DMACC = MarlinLPC::DMAGetChannel(0);
  DMACC.DestAddr = (uint32_t)&SSP.DR;
  DMACC.LLI.LLI = 0;
  _dmacInitSSP(DMACC, _ssp_gpioMap.sspBusIdx);

  size_t curoff = 0;

  while (curoff < cnt) {
    uint32_t left = (cnt - curoff);
    uint32_t takecnt = __MIN <uint32_t> (left, (1<<12)-1);

    MarlinLPC::DMACCxControl_reg_t Control;
    Control.TransferSize = takecnt;
    Control.SBSize = 0;
    Control.DBSize = 0;
    Control.SWidth = txwidth;
    Control.DWidth = txwidth;
    Control.reserved1 = 0;
    Control.SI = true;
    Control.DI = false;
    Control.Prot1 = 1;
    Control.Prot2 = 0;
    Control.Prot3 = 0;
    Control.I = false;

    DMACC.SrcAddr = (uint32_t)( buf + curoff );
    DMACC.Control = Control;
    
    curoff += takecnt;

    // Kick off the DMA.
    DMACC.Config.E = true;
    while (DMACC.Config.E) { /* wait for the DMA TX to finish */ }
  }

  _ssp_dirty_rxbuffer = true;
}

template <typename numberType>
static void _dmaSendRepeatBlocking(numberType val, uint32_t repcnt) {
  auto& SSP = MarlinLPC::SPIGetBusFromIndex(_ssp_gpioMap.sspBusIdx);

  uint32_t txwidth = 0;

  if (sizeof(numberType) == 1) {
    txwidth = 0;
  }
  else if (sizeof(numberType) == 2) {
    txwidth = 1;
  }
  else if (sizeof(numberType) == 4) {
    txwidth = 2;
  }
  else
    _spi_on_error(5);

  auto& DMACC = MarlinLPC::DMAGetChannel(0);
  DMACC.SrcAddr = (uint32_t)&val;
  DMACC.DestAddr = (uint32_t)&SSP.DR;
  DMACC.LLI.LLI = 0;
  _dmacInitSSP(DMACC, _ssp_gpioMap.sspBusIdx);

  size_t curoff = 0;

  while (curoff < repcnt) {
    uint32_t left = (repcnt - curoff);
    uint32_t takecnt = __MIN <uint32_t> (left, (1<<12)-1);

    MarlinLPC::DMACCxControl_reg_t Control;
    Control.TransferSize = takecnt;
    Control.SBSize = 0;
    Control.DBSize = 0;
    Control.SWidth = txwidth;
    Control.DWidth = txwidth;
    Control.reserved1 = 0;
    Control.SI = false;
    Control.DI = false;
    Control.Prot1 = 1;
    Control.Prot2 = 0;
    Control.Prot3 = 0;
    Control.I = false;

    DMACC.Control = Control;
    
    curoff += takecnt;

    // Kick off the DMA.
    DMACC.Config.E = true;
    while (DMACC.Config.E) { /* wait for the DMA TX to finish */ }
  }

  _ssp_dirty_rxbuffer = true;
}

#endif

#ifndef HALSPI_DMA_THRESHOLD
#define HALSPI_DMA_THRESHOLD 32
#endif

  void spiWrite(const uint8_t *buf, uint16_t cnt) {
    if (cnt == 0) return;
    if (_ssp_gpioMap.gpio_mosi < 0) return;

    _spiSetFrameSize(sizeof(uint8_t));

    _maybe_start_transaction();

    _spiAsyncBarrier();

#ifndef HALSPI_DISABLE_DMA
    if (cnt > HALSPI_DMA_THRESHOLD) {
      _dmaStart();
      _dmaSendBlocking(buf, cnt);
      _dmaEnd();
      return;
    }
#endif

    auto& SSP = MarlinLPC::SPIGetBusFromIndex(_ssp_gpioMap.sspBusIdx);

    for (uint16_t n = 0; n < cnt; n++) {
      _spi_push_to_queue(SSP, buf[n]);
    }

    _ssp_dirty_rxbuffer = true;
  }

  void spiWrite16(const uint16_t *buf, uint16_t cnt) {
    if (cnt == 0) return;
    if (_ssp_gpioMap.gpio_mosi < 0) return;

    _spiSetFrameSize(sizeof(uint16_t));

    _maybe_start_transaction();

    _spiAsyncBarrier();

#ifndef HALSPI_DISABLE_DMA
    if (cnt > HALSPI_DMA_THRESHOLD) {
      _dmaStart();
      _dmaSendBlocking(buf, cnt);
      _dmaEnd();
      return;
    }
#endif

    auto& SSP = MarlinLPC::SPIGetBusFromIndex(_ssp_gpioMap.sspBusIdx);

    for (uint16_t n = 0; n < cnt; n++) {
      _spi_push_to_queue(SSP, buf[n]);
    }

    _ssp_dirty_rxbuffer = true;
  }

  void spiWriteRepeat(uint8_t val, uint16_t repcnt) {
    if (repcnt == 0) return;
    if (_ssp_gpioMap.gpio_mosi < 0) return;

    _spiSetFrameSize(sizeof(uint8_t));

    _maybe_start_transaction();

    _spiAsyncBarrier();

#ifndef HALSPI_DISABLE_DMA
    if (repcnt > HALSPI_DMA_THRESHOLD) {
      _dmaStart();
      _dmaSendRepeatBlocking(val, repcnt);
      _dmaEnd();
      return;
    }
#endif

    auto& SSP = MarlinLPC::SPIGetBusFromIndex(_ssp_gpioMap.sspBusIdx);

    for (uint16_t n = 0; n < repcnt; n++) {
      _spi_push_to_queue(SSP, val);
    }

    _ssp_dirty_rxbuffer = true;
  }

  void spiWriteRepeat16(uint16_t val, uint16_t repcnt) {
    if (repcnt == 0) return;
    if (_ssp_gpioMap.gpio_mosi < 0) return;

    _spiSetFrameSize(sizeof(uint16_t));

    _maybe_start_transaction();

    _spiAsyncBarrier();

#ifndef HALSPI_DISABLE_DMA
    if (repcnt > HALSPI_DMA_THRESHOLD) {
      _dmaStart();
      _dmaSendRepeatBlocking(val, repcnt);
      _dmaEnd();
      return;
    }
#endif

    auto& SSP = MarlinLPC::SPIGetBusFromIndex(_ssp_gpioMap.sspBusIdx);

    for (uint16_t n = 0; n < repcnt; n++) {
      _spi_push_to_queue(SSP, val);
    }

    _ssp_dirty_rxbuffer = true;
  }

#ifdef HAL_SPI_SUPPORTS_ASYNC

#ifndef HALSPI_DISABLE_DMA

static void _dmaUninstallInterrupt();

static void _dmacAdvance(MarlinLPC::dma_process_t& proc) {
  // If there is any last chain that was used then clear it.
  if (auto *last_chain = proc.last_chain) {
    MarlinLPC::DMACCxLLI_desc_user_t *iter = last_chain;

    while (iter) {
      iter->available = true;
      iter = (MarlinLPC::DMACCxLLI_desc_user_t*)iter->Next;
    }
    proc.last_chain = nullptr;
  }

  auto& DMACC = *proc.current_DMACC;

  if (proc.curoff == proc.txlen) {
    // Disable the terminal count interrupt.
    DMACC.Control.I = false;
    DMACC.Config.ITC = false;
    
    _dmaUninstallInterrupt();
    _dmaEnd();

    auto completeCallback = proc.completeCallback;
    void *complete_ud = proc.complete_ud;

    // Finished.
    proc.current_DMACC = nullptr;
    proc.current_buffer = nullptr;
    proc.curoff = 0;
    proc.txlen = 0;
    proc.txunitsize = 0;
    proc.completeCallback = nullptr;
    proc.complete_ud = nullptr;
    proc.is_active = false;

    if (completeCallback) {
      completeCallback(complete_ud);
    }
    return;
  }

  auto& SSP = MarlinLPC::SPIGetBusFromIndex(_ssp_gpioMap.sspBusIdx);

  MarlinLPC::DMAProgramSSPChain(SSP, proc);

  // Kick-off another async TX.
  DMACC.Config.E = true;
}

static void _dma_interrupt() {
  for (uint8_t n = 0; n < 8; n++) {
    uint32_t chmask = (1<<n);

    if (MarlinLPC::DMACIntTCStat.IntTCStat & chmask) {
      // We need to handle the interrupt for channel n.
      MarlinLPC::DMACIntTCClear.IntTCClear |= chmask;

      auto& DMACC = MarlinLPC::DMAGetChannel(n);

      // DMA TX has progressed, probably even finished.
      if (DMACC.LLI.LLI == 0) {
        // The transfer is completely finished.
        auto& proc = MarlinLPC::_dma_async_proc;

        // Wait till the DMA channel has disabled itself.
        while (DMACC.Config.E) { /* wait */ }

        if (proc.is_active && proc.current_DMACC == &DMACC) {
          // Try to advance the TX process.
          _dmacAdvance(proc);
        }
      }
    }
  }
}

static void _dmaInstallInterrupt() {
  nvicSetIRQHandler((IRQn_Type)26, _dma_interrupt);
  nvicInstallRedirect();
  NVIC_EnableIRQ((IRQn_Type)26);
  NVIC_SetPriority((IRQn_Type)26, 5);
}

static void _dmaUninstallInterrupt() {
  NVIC_SetPriority((IRQn_Type)26, 255);
  NVIC_DisableIRQ((IRQn_Type)26);
  nvicUninstallRedirect();
  nvicResetIRQHandler((IRQn_Type)26);
}

template <typename numberType>
inline void _dmaSendAsync(const numberType *buf, uint32_t cnt, void (*completeCallback)(void*), void *ud) {
  auto& DMACC = MarlinLPC::DMAGetChannel(0);

  auto& proc = MarlinLPC::_dma_async_proc;
  proc.current_DMACC = &DMACC;
  proc.current_buffer = buf;
  proc.curoff = 0;
  proc.txlen = cnt;
  proc.txunitsize = sizeof(numberType);
  proc.completeCallback = completeCallback;
  proc.complete_ud = ud;
  proc.is_active = true;

  _dmaStart();
  MarlinLPC::DMACIntTCClear.IntTCClear |= (1<<0);
  _dmaInstallInterrupt();
  _dmacInitSSP(DMACC, _ssp_gpioMap.sspBusIdx);
  _dmacAdvance(proc);
}

#endif

#include "../shared/SPI/bufmgmt.h"

  static void _spiDisableInterrupt(uint8_t sspBusIdx);

  template <uint32_t sspBusIdx>
  inline void _spi_interrupt( void ) {
    auto& SSP = MarlinLPC::SPIGetBusFromIndex(sspBusIdx);

    ssp_process_t& proc = _ssp_async_proc;

    if (proc.is_active == false || proc.current_ssp != &SSP)
      _spi_on_error(3);

    using namespace ::bufmgmt;

    // Check interrupt type.
    if (SSP.MIS.TXMIS) {
      bool revbits = (_ssp_bitOrder == SPI_BITORDER_LSB);
      bool revbytes = (_ssp_bitOrder == SPI_BITORDER_MSB);

      uint8_t txunitsize = proc.txunitsize;
      size_t txmax = proc.txlen_bytes;
      const void *current_buffer = proc.current_buffer;
      uint8_t framesize = _ssp_framesize;

      uint32_t curoff_bytes = proc.curoff_bytes;

      // Write as much data as possible.
      while (SSP.SR.TNF) {
        bool has_more_data = false;

        if (framesize == 1) {
          uint8_t byteval;
          if (txunitsize == 1) {
            if (curoff_bytes < txmax) {
              const uint8_t *buf = (const uint8_t*)current_buffer;
              byteval = GetByteFromNumber(
                buf[ GetNumberIndexFromTotalByteIndex <uint8_t> (curoff_bytes) ],
                GetLocalByteIndexFromTotalByteIndex <uint8_t> (curoff_bytes),
                !revbytes
              );
              has_more_data = true;
            }
          }
          else if (txunitsize == 2) {
            if (curoff_bytes < txmax) {
              const uint16_t *buf = (const uint16_t*)current_buffer;
              byteval = GetByteFromNumber(
                buf[ GetNumberIndexFromTotalByteIndex <uint16_t> (curoff_bytes) ],
                GetLocalByteIndexFromTotalByteIndex <uint16_t> (curoff_bytes),
                !revbytes
              );
              has_more_data = true;
            }
          }

          if (has_more_data) {
            if (revbits) {
              byteval = _flip_bits(byteval);
            }

            // Push the data onto the (not-full) queue, byte-by-byte (DSS = 7).
            SSP.DR.DATA = byteval;

            curoff_bytes += sizeof(uint8_t);
          }
        }
        else if (framesize == 2) {
          uint16_t val;
          if (txunitsize == 2) {
            if (curoff_bytes < txmax) {
              const uint16_t *buf = (const uint16_t*)current_buffer;
              val = buf[ curoff_bytes / sizeof(uint16_t) ];
              has_more_data = true;
            }
          }

          if (has_more_data) {
            if (revbits) {
              val = _flip_bits(val);
            }

            SSP.DR.DATA = val;

            curoff_bytes += sizeof(uint16_t);
          }
        }

        if (has_more_data) {
          // We are ignoring the reception part.
          _ssp_dirty_rxbuffer = true;
        }
        else {
          // Disable the interrupt.
          _spiDisableInterrupt(sspBusIdx);

          SSP.IMSC.TXIM = false;

          auto completeCallback = proc.completeCallback;
          auto complete_ud = proc.complete_ud;

          // We have finished the transfer, thus dismantle.
          proc.current_ssp = nullptr;
          proc.current_buffer = nullptr;
          proc.curoff_bytes = 0;
          proc.txlen_bytes = 0;
          proc.txunitsize = 0;
          proc.completeCallback = nullptr;
          proc.complete_ud = nullptr;
          proc.is_active = false;
        
          // Call any completion routine.
          if (completeCallback) {
            completeCallback(complete_ud);
          }

          goto finished;
        }
      }
      // Save progress.
      proc.curoff_bytes = curoff_bytes;

    finished:;
    }
  }

  static void _spi_interrupt_sspbus0() {
    _spi_interrupt <0> ();
  }
  static void _spi_interrupt_sspbus1() {
    _spi_interrupt <1> ();
  }

  static void _spiEnableInterrupt(uint8_t sspBusIdx) {
    if (sspBusIdx == 0) {
      nvicSetIRQHandler((IRQn_Type)14, _spi_interrupt_sspbus0);
    }
    else if (sspBusIdx == 1) {
      nvicSetIRQHandler((IRQn_Type)15, _spi_interrupt_sspbus1);
    }
    nvicInstallRedirect();
    if (sspBusIdx == 0) {
      NVIC_EnableIRQ((IRQn_Type)14);
      NVIC_SetPriority((IRQn_Type)14, 5);
    }
    else if (sspBusIdx == 1) {
      NVIC_EnableIRQ((IRQn_Type)15);
      NVIC_SetPriority((IRQn_Type)15, 5);
    }
  }

  static void _spiDisableInterrupt(uint8_t sspBusIdx) {
    if (sspBusIdx == 0) {
      NVIC_SetPriority((IRQn_Type)14, 255);
      NVIC_DisableIRQ((IRQn_Type)14);
    }
    else if (sspBusIdx == 1) {
      NVIC_SetPriority((IRQn_Type)15, 255);
      NVIC_DisableIRQ((IRQn_Type)15);
    }
    nvicUninstallRedirect();
    if (sspBusIdx == 0) {
      nvicResetIRQHandler((IRQn_Type)14);
    }
    else if (sspBusIdx == 1) {
      nvicResetIRQHandler((IRQn_Type)15);
    }
  }

  template <typename numberType>
  static void _spiStartAsyncTX(const numberType *buf, uint16_t cnt, void (*completeCallback)(void*), void *ud) {
    auto& SSP = MarlinLPC::SPIGetBusFromIndex(_ssp_gpioMap.sspBusIdx);

    // Start the async process.
    auto& proc = _ssp_async_proc;
    proc.current_ssp = &SSP;
    proc.current_buffer = buf;
    proc.curoff_bytes = 0;
    proc.txlen_bytes = cnt * sizeof(numberType);
    proc.txunitsize = sizeof(numberType);
    proc.completeCallback = completeCallback;
    proc.complete_ud = ud;
    proc.is_active = true;

    // Register the interrupt.
    _spiEnableInterrupt(_ssp_gpioMap.sspBusIdx);

    // Enable the interrupt. This should kick it off.
    SSP.IMSC.TXIM = true;
  }

  void spiWriteAsync(const uint8_t *buf, uint16_t cnt, void (*completeCallback)(void*), void *ud) {
    if (cnt == 0 || _ssp_gpioMap.gpio_mosi < 0) {
      if (completeCallback) {
        completeCallback(ud);
      }
      return;
    }

    _spiSetFrameSize(sizeof(uint8_t));

    _maybe_start_transaction();

    _spiAsyncBarrier();

#ifndef HALSPI_DISABLE_DMA
    if (cnt > HALSPI_DMA_THRESHOLD) {
      _dmaSendAsync(buf, cnt, completeCallback, ud);
      return;
    }
#endif

    _spiStartAsyncTX(buf, cnt, completeCallback, ud);
  }

  void spiWriteAsync16(const uint16_t *buf, uint16_t cnt, void (*completeCallback)(void*), void *ud) {
    if (cnt == 0 || _ssp_gpioMap.gpio_mosi < 0) {
      if (completeCallback) {
        completeCallback(ud);
      }
      return;
    }

    _spiSetFrameSize(sizeof(uint16_t));

    _maybe_start_transaction();

    _spiAsyncBarrier();

#ifndef HALSPI_DISABLE_DMA
    if (cnt > HALSPI_DMA_THRESHOLD) {
      _dmaSendAsync(buf, cnt, completeCallback, ud);
      return;
    }
#endif

    _spiStartAsyncTX(buf, cnt, completeCallback, ud);
  }

  void spiAsyncAbort() {
    cli();
    {
      auto& proc = _ssp_async_proc;

      if (proc.is_active) {
        auto& SSP = *proc.current_ssp;

        // Disable the interrupt processing.
        SSP.IMSC.TXIM = false;

        // Unregister the interrupt.
        _spiDisableInterrupt(MarlinLPC::SPIGetBusIndex(SSP));

        // Cancel the process.
        proc.is_active = false;
      }
    }
#ifndef HALSPI_DISABLE_DMA
    {
      auto& proc = MarlinLPC::_dma_async_proc;

      if (proc.is_active) {
        auto& DMACC = *proc.current_DMACC;

        DMACC.Config.H = true;
        while (DMACC.Config.A) { /* wait until the DMA channel has no more data to process */ }
        DMACC.Config.E = false;

        DMACC.Config.ITC = false;
        DMACC.Control.I = false;

        // Cancel the process.
        proc.is_active = false;
      }
    }
#endif

    sei();
  }

  void spiAsyncJoin() {
    _spiAsyncBarrier();
  }

  bool spiAsyncIsRunning() {
    if (_ssp_async_proc.is_active) return true;
  #ifndef HALSPI_DISABLE_DMA
    if (MarlinLPC::_dma_async_proc.is_active) return true;
  #endif
    return false;
  }

#endif

#endif

#endif