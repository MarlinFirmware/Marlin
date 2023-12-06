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

/**
 * HAL for stm32duino.com based on Libmaple and compatible (STM32F1)
 */

#ifdef __STM32F1__

#include "../../inc/MarlinConfig.h"
#include "HAL.h"

#include "adc.h"
uint16_t adc_results[ADC_COUNT];

// ------------------------
// Serial ports
// ------------------------

#if defined(SERIAL_USB) && !HAS_SD_HOST_DRIVE

  USBSerial SerialUSB;
  DefaultSerial1 MSerial0(true, SerialUSB);

  #if ENABLED(EMERGENCY_PARSER)
    #include "../libmaple/usb/stm32f1/usb_reg_map.h"
    #include "libmaple/usb_cdcacm.h"
    // The original callback is not called (no way to retrieve address).
    // That callback detects a special STM32 reset sequence: this functionality is not essential
    // as M997 achieves the same.
    void my_rx_callback(unsigned int, void*) {
      // max length of 16 is enough to contain all emergency commands
      uint8 buf[16];

      //rx is usbSerialPart.endpoints[2]
      uint16 len = usb_get_ep_rx_count(USB_CDCACM_RX_ENDP);
      uint32 total = usb_cdcacm_data_available();

      if (len == 0 || total == 0 || !WITHIN(total, len, COUNT(buf)))
        return;

      // cannot get character by character due to bug in composite_cdcacm_peek_ex
      len = usb_cdcacm_peek(buf, total);

      for (uint32 i = 0; i < len; i++)
        emergency_parser.update(MSerial0.emergency_state, buf[i + total - len]);
    }
  #endif
#endif

// ------------------------
// Watchdog Timer
// ------------------------

#if ENABLED(USE_WATCHDOG)

  #include <libmaple/iwdg.h>

  void watchdogSetup() {
    // do whatever. don't remove this function.
  }

  /**
   *  The watchdog clock is 40Khz. So for a 4s or 8s interval use a /256 preescaler and 625 or 1250 reload value (counts down to 0).
   */
  #define STM32F1_WD_RELOAD TERN(WATCHDOG_DURATION_8S, 1250, 625) // 4 or 8 second timeout

  /**
   * @brief  Initialize the independent hardware watchdog.
   *
   * @return No return
   *
   * @details The watchdog clock is 40Khz. So for a 4s or 8s interval use a /256 preescaler and 625 or 1250 reload value (counts down to 0).
   */
  void MarlinHAL::watchdog_init() {
    #if DISABLED(DISABLE_WATCHDOG_INIT)
      iwdg_init(IWDG_PRE_256, STM32F1_WD_RELOAD);
    #endif
  }

  // Reset watchdog. MUST be called every 4 or 8 seconds after the
  // first watchdog_init or the STM32F1 will reset.
  void MarlinHAL::watchdog_refresh() {
    #if DISABLED(PINS_DEBUGGING) && PIN_EXISTS(LED)
      TOGGLE(LED_PIN);  // heartbeat indicator
    #endif
    iwdg_feed();
  }

#endif // USE_WATCHDOG

// ------------------------
// ADC
// ------------------------

// Watch out for recursion here! Our pin_t is signed, so pass through to Arduino -> analogRead(uint8_t)

uint16_t analogRead(const pin_t pin) {
  const bool is_analog = _GET_MODE(pin) == GPIO_INPUT_ANALOG;
  return is_analog ? analogRead(uint8_t(pin)) : 0;
}

// Wrapper to maple unprotected analogWrite
void analogWrite(const pin_t pin, int pwm_val8) {
  if (PWM_PIN(pin)) analogWrite(uint8_t(pin), pwm_val8);
}

uint16_t MarlinHAL::adc_result;

#ifndef VOXELAB_N32

#include <STM32ADC.h>

// Init the AD in continuous capture mode
void MarlinHAL::adc_init() {
  static const uint8_t adc_pins[] = {
    OPTITEM(HAS_TEMP_ADC_0, TEMP_0_PIN)
    OPTITEM(HAS_TEMP_ADC_1, TEMP_1_PIN)
    OPTITEM(HAS_TEMP_ADC_2, TEMP_2_PIN)
    OPTITEM(HAS_TEMP_ADC_3, TEMP_3_PIN)
    OPTITEM(HAS_TEMP_ADC_4, TEMP_4_PIN)
    OPTITEM(HAS_TEMP_ADC_5, TEMP_5_PIN)
    OPTITEM(HAS_TEMP_ADC_6, TEMP_6_PIN)
    OPTITEM(HAS_TEMP_ADC_7, TEMP_7_PIN)
    OPTITEM(HAS_HEATED_BED, TEMP_BED_PIN)
    OPTITEM(HAS_TEMP_CHAMBER, TEMP_CHAMBER_PIN)
    OPTITEM(HAS_TEMP_ADC_PROBE, TEMP_PROBE_PIN)
    OPTITEM(HAS_TEMP_COOLER, TEMP_COOLER_PIN)
    OPTITEM(HAS_TEMP_BOARD, TEMP_BOARD_PIN)
    OPTITEM(HAS_TEMP_SOC, TEMP_SOC_PIN)
    OPTITEM(FILAMENT_WIDTH_SENSOR, FILWIDTH_PIN)
    OPTITEM(HAS_ADC_BUTTONS, ADC_KEYPAD_PIN)
    OPTITEM(HAS_JOY_ADC_X, JOY_X_PIN)
    OPTITEM(HAS_JOY_ADC_Y, JOY_Y_PIN)
    OPTITEM(HAS_JOY_ADC_Z, JOY_Z_PIN)
    OPTITEM(POWER_MONITOR_CURRENT, POWER_MONITOR_CURRENT_PIN)
    OPTITEM(POWER_MONITOR_VOLTAGE, POWER_MONITOR_VOLTAGE_PIN)
  };
  static STM32ADC adc(ADC1);
  // Configure the ADC
  adc.calibrate();
  adc.setSampleRate((F_CPU > 72000000) ? ADC_SMPR_71_5 : ADC_SMPR_41_5); // 71.5 or 41.5 ADC cycles
  adc.setPins((uint8_t *)adc_pins, ADC_COUNT);
  adc.setDMA(adc_results, uint16_t(ADC_COUNT), uint32_t(DMA_MINC_MODE | DMA_CIRC_MODE), nullptr);
  adc.setScanMode();
  adc.setContinuous();
  adc.startConversion();
}

#endif // !VOXELAB_N32

void MarlinHAL::adc_start(const pin_t pin) {
  #define __TCASE(N,I) case N: pin_index = I; break;
  #define _TCASE(C,N,I) TERN_(C, __TCASE(N, I))
  ADCIndex pin_index;
  switch (pin) {
    default: return;
    _TCASE(HAS_TEMP_ADC_0,        TEMP_0_PIN,                TEMP_0)
    _TCASE(HAS_TEMP_ADC_1,        TEMP_1_PIN,                TEMP_1)
    _TCASE(HAS_TEMP_ADC_2,        TEMP_2_PIN,                TEMP_2)
    _TCASE(HAS_TEMP_ADC_3,        TEMP_3_PIN,                TEMP_3)
    _TCASE(HAS_TEMP_ADC_4,        TEMP_4_PIN,                TEMP_4)
    _TCASE(HAS_TEMP_ADC_5,        TEMP_5_PIN,                TEMP_5)
    _TCASE(HAS_TEMP_ADC_6,        TEMP_6_PIN,                TEMP_6)
    _TCASE(HAS_TEMP_ADC_7,        TEMP_7_PIN,                TEMP_7)
    _TCASE(HAS_HEATED_BED,        TEMP_BED_PIN,              TEMP_BED)
    _TCASE(HAS_TEMP_CHAMBER,      TEMP_CHAMBER_PIN,          TEMP_CHAMBER)
    _TCASE(HAS_TEMP_ADC_PROBE,    TEMP_PROBE_PIN,            TEMP_PROBE)
    _TCASE(HAS_TEMP_COOLER,       TEMP_COOLER_PIN,           TEMP_COOLER)
    _TCASE(HAS_TEMP_BOARD,        TEMP_BOARD_PIN,            TEMP_BOARD)
    _TCASE(HAS_TEMP_SOC,          TEMP_SOC_PIN,              TEMP_SOC)
    _TCASE(HAS_JOY_ADC_X,         JOY_X_PIN,                 JOY_X)
    _TCASE(HAS_JOY_ADC_Y,         JOY_Y_PIN,                 JOY_Y)
    _TCASE(HAS_JOY_ADC_Z,         JOY_Z_PIN,                 JOY_Z)
    _TCASE(FILAMENT_WIDTH_SENSOR, FILWIDTH_PIN,              FILWIDTH)
    _TCASE(HAS_ADC_BUTTONS,       ADC_KEYPAD_PIN,            ADC_KEY)
    _TCASE(POWER_MONITOR_CURRENT, POWER_MONITOR_CURRENT_PIN, POWERMON_CURRENT)
    _TCASE(POWER_MONITOR_VOLTAGE, POWER_MONITOR_VOLTAGE_PIN, POWERMON_VOLTAGE)
  }
  adc_result = (adc_results[(int)pin_index] & 0xFFF) >> (12 - HAL_ADC_RESOLUTION); // shift out unused bits
}

// ------------------------
// Public functions
// ------------------------

void NVIC_SetPriorityGrouping(uint32_t PriorityGroup) {
  uint32_t reg_value;
  uint32_t PriorityGroupTmp = (PriorityGroup & (uint32_t)0x07);               // only values 0..7 are used

  reg_value  =  SCB->AIRCR;                                                   // read old register configuration
  reg_value &= ~(SCB_AIRCR_VECTKEY_Msk | SCB_AIRCR_PRIGROUP_Msk);             // clear bits to change
  reg_value  =  (reg_value                                 |
                ((uint32_t)0x5FA << SCB_AIRCR_VECTKEY_Pos) |
                (PriorityGroupTmp << 8));                                     // Insert write key & priority group
  SCB->AIRCR =  reg_value;
}

void flashFirmware(const int16_t) { hal.reboot(); }

//
// Leave PA11/PA12 intact if USBSerial is not used
//
#if SERIAL_USB
  namespace wirish { namespace priv {
    #if SERIAL_PORT > 0
      #if SERIAL_PORT2
        #if SERIAL_PORT2 > 0
          void board_setup_usb() {}
        #endif
      #else
        void board_setup_usb() {}
      #endif
    #endif
  } }
#endif

TERN_(POSTMORTEM_DEBUGGING, extern void install_min_serial());

// ------------------------
// MarlinHAL class
// ------------------------

void MarlinHAL::init() {
  NVIC_SetPriorityGrouping(0x3);
  #if PIN_EXISTS(LED)
    OUT_WRITE(LED_PIN, LOW);
  #endif
  #if HAS_SD_HOST_DRIVE
    MSC_SD_init();
  #elif ALL(SERIAL_USB, EMERGENCY_PARSER)
    usb_cdcacm_set_hooks(USB_CDCACM_HOOK_RX, my_rx_callback);
  #endif
  #if PIN_EXISTS(USB_CONNECT)
    OUT_WRITE(USB_CONNECT_PIN, !USB_CONNECT_INVERTING);  // USB clear connection
    delay(1000);                                         // Give OS time to notice
    WRITE(USB_CONNECT_PIN, USB_CONNECT_INVERTING);
  #endif
  TERN_(POSTMORTEM_DEBUGGING, install_min_serial());    // Install the minimal serial handler
}

// HAL idle task
void MarlinHAL::idletask() {
  #if HAS_SHARED_MEDIA
    /**
     * When Marlin is using the SD card it should be locked to prevent it being
     * accessed from a PC over USB.
     * Other HALs use (IS_SD_PRINTING() || IS_SD_FILE_OPEN()) to check for access
     * but this won't reliably detect other file operations. To be safe we just lock
     * the drive whenever Marlin has it mounted. LCDs should include an Unmount
     * command so drives can be released as needed.
     */
    /* Copied from LPC1768 framework. Should be fixed later to process HAS_SD_HOST_DRIVE */
    //if (!drive_locked()) // TODO
    MarlinMSC.loop(); // Process USB mass storage device class loop
  #endif
}

void MarlinHAL::reboot() { nvic_sys_reset(); }

#endif // __STM32F1__
