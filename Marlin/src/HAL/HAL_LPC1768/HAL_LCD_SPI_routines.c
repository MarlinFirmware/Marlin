/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016, 2017 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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

/**
 * Low level pin manipulation routines - used by all the drivers.
 *
 * These are based on the LPC1768 pinMode, digitalRead & digitalWrite routines.
 *
 * Couldn't just call exact copies because the overhead killed the LCD update speed
 * With an intermediate level the softspi was running in the 10-20kHz range which
 * resulted in using about about 25% of the CPU's time.
 */

#if defined(TARGET_LPC1768)

  #include <LPC17xx.h>
  #include <lpc17xx_pinsel.h>
  #include "src/core/macros.h"
  #include "pinmapping.h"


  #define LPC_PORT_OFFSET         (0x0020)
  #define LPC_PIN(pin)            (1UL << pin)
  #define LPC_GPIO(port)          ((volatile LPC_GPIO_TypeDef *)(LPC_GPIO0_BASE + LPC_PORT_OFFSET * port))

  #define INPUT 0
  #define OUTPUT 1
  #define INPUT_PULLUP 2

  #ifdef __cplusplus
      extern "C" {
  #endif

  // IO functions
  // As defined by Arduino INPUT(0x0), OUPUT(0x1), INPUT_PULLUP(0x2)
  void pinMode_LCD(int pin, int mode) {
    if (!WITHIN(pin, 0, NUM_DIGITAL_PINS - 1) || pin_map[pin].port == 0xFF)
      return;

    PINSEL_CFG_Type config = { pin_map[pin].port,
                               pin_map[pin].pin,
                               PINSEL_FUNC_0,
                               PINSEL_PINMODE_TRISTATE,
                               PINSEL_PINMODE_NORMAL };
    switch(mode) {
    case INPUT:
      LPC_GPIO(pin_map[pin].port)->FIODIR &= ~LPC_PIN(pin_map[pin].pin);
      PINSEL_ConfigPin(&config);
      break;
    case OUTPUT:
      LPC_GPIO(pin_map[pin].port)->FIODIR |=  LPC_PIN(pin_map[pin].pin);
      PINSEL_ConfigPin(&config);
      break;
    case INPUT_PULLUP:
      LPC_GPIO(pin_map[pin].port)->FIODIR &= ~LPC_PIN(pin_map[pin].pin);
      config.Pinmode = PINSEL_PINMODE_PULLUP;
      PINSEL_ConfigPin(&config);
      break;
    default:
      break;
    }
  }


  void u8g_SetPinOutput(uint8_t internal_pin_number) {
     pinMode_LCD(internal_pin_number, 1);  // OUTPUT
  }

  void u8g_SetPinInput(uint8_t internal_pin_number) {
     pinMode_LCD(internal_pin_number, 0);  // INPUT
  }



  void u8g_SetPinLevel(uint8_t  pin, uint8_t  pin_status) {
    if (!WITHIN(pin, 0, NUM_DIGITAL_PINS - 1) || pin_map[pin].port == 0xFF)
      return;

    if (pin_status)
      LPC_GPIO(pin_map[pin].port)->FIOSET = LPC_PIN(pin_map[pin].pin);
    else
      LPC_GPIO(pin_map[pin].port)->FIOCLR = LPC_PIN(pin_map[pin].pin);
  }

  uint8_t u8g_GetPinLevel(uint8_t pin) {
    if (!WITHIN(pin, 0, NUM_DIGITAL_PINS - 1) || pin_map[pin].port == 0xFF) {
      return 0;  // false
    }
    return (uint32_t)LPC_GPIO(pin_map[pin].port)->FIOPIN & (uint32_t)LPC_PIN(pin_map[pin].pin) ? 1 : 0;
  }


  #ifdef __cplusplus
    }
  #endif

#endif
