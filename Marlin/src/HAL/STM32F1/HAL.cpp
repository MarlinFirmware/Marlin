/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * Copyright (c) 2016 Bob Cousins bobcousins42@googlemail.com
 * Copyright (c) 2015-2016 Nico Tonnhofer wurstnase.reprap@gmail.com
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

/**
 * HAL for stm32duino.com based on Libmaple and compatible (STM32F1)
 */

#ifdef __STM32F1__

#include "../../inc/MarlinConfig.h"
#include "HAL.h"

#include <STM32ADC.h>

// ------------------------
// Types
// ------------------------

#define __I
#define __IO volatile
 typedef struct {
   __I  uint32_t CPUID;                   /*!< Offset: 0x000 (R/ )  CPUID Base Register                                   */
   __IO uint32_t ICSR;                    /*!< Offset: 0x004 (R/W)  Interrupt Control and State Register                  */
   __IO uint32_t VTOR;                    /*!< Offset: 0x008 (R/W)  Vector Table Offset Register                          */
   __IO uint32_t AIRCR;                   /*!< Offset: 0x00C (R/W)  Application Interrupt and Reset Control Register      */
   __IO uint32_t SCR;                     /*!< Offset: 0x010 (R/W)  System Control Register                               */
   __IO uint32_t CCR;                     /*!< Offset: 0x014 (R/W)  Configuration Control Register                        */
   __IO uint8_t  SHP[12];                 /*!< Offset: 0x018 (R/W)  System Handlers Priority Registers (4-7, 8-11, 12-15) */
   __IO uint32_t SHCSR;                   /*!< Offset: 0x024 (R/W)  System Handler Control and State Register             */
   __IO uint32_t CFSR;                    /*!< Offset: 0x028 (R/W)  Configurable Fault Status Register                    */
   __IO uint32_t HFSR;                    /*!< Offset: 0x02C (R/W)  HardFault Status Register                             */
   __IO uint32_t DFSR;                    /*!< Offset: 0x030 (R/W)  Debug Fault Status Register                           */
   __IO uint32_t MMFAR;                   /*!< Offset: 0x034 (R/W)  MemManage Fault Address Register                      */
   __IO uint32_t BFAR;                    /*!< Offset: 0x038 (R/W)  BusFault Address Register                             */
   __IO uint32_t AFSR;                    /*!< Offset: 0x03C (R/W)  Auxiliary Fault Status Register                       */
   __I  uint32_t PFR[2];                  /*!< Offset: 0x040 (R/ )  Processor Feature Register                            */
   __I  uint32_t DFR;                     /*!< Offset: 0x048 (R/ )  Debug Feature Register                                */
   __I  uint32_t ADR;                     /*!< Offset: 0x04C (R/ )  Auxiliary Feature Register                            */
   __I  uint32_t MMFR[4];                 /*!< Offset: 0x050 (R/ )  Memory Model Feature Register                         */
   __I  uint32_t ISAR[5];                 /*!< Offset: 0x060 (R/ )  Instruction Set Attributes Register                   */
        uint32_t RESERVED0[5];
   __IO uint32_t CPACR;                   /*!< Offset: 0x088 (R/W)  Coprocessor Access Control Register                   */
 } SCB_Type;

// ------------------------
// Local defines
// ------------------------

#define SCS_BASE            (0xE000E000UL)                            /*!< System Control Space Base Address  */
#define SCB_BASE            (SCS_BASE +  0x0D00UL)                    /*!< System Control Block Base Address  */

#define SCB                 ((SCB_Type       *)     SCB_BASE      )   /*!< SCB configuration struct           */

/* SCB Application Interrupt and Reset Control Register Definitions */
#define SCB_AIRCR_VECTKEY_Pos              16                                             /*!< SCB AIRCR: VECTKEY Position */
#define SCB_AIRCR_VECTKEY_Msk              (0xFFFFUL << SCB_AIRCR_VECTKEY_Pos)            /*!< SCB AIRCR: VECTKEY Mask */

#define SCB_AIRCR_PRIGROUP_Pos              8                                             /*!< SCB AIRCR: PRIGROUP Position */
#define SCB_AIRCR_PRIGROUP_Msk             (7UL << SCB_AIRCR_PRIGROUP_Pos)                /*!< SCB AIRCR: PRIGROUP Mask */

// ------------------------
// Public Variables
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

uint16_t HAL_adc_result;

// ------------------------
// Private Variables
// ------------------------
STM32ADC adc(ADC1);

const uint8_t adc_pins[] = {
  #if HAS_TEMP_ADC_0
    TEMP_0_PIN,
  #endif
  #if HAS_TEMP_ADC_PROBE
    TEMP_PROBE_PIN,
  #endif
  #if HAS_HEATED_BED
    TEMP_BED_PIN,
  #endif
  #if HAS_TEMP_CHAMBER
    TEMP_CHAMBER_PIN,
  #endif
  #if HAS_TEMP_COOLER
    TEMP_COOLER_PIN,
  #endif
  #if HAS_TEMP_ADC_1
    TEMP_1_PIN,
  #endif
  #if HAS_TEMP_ADC_2
    TEMP_2_PIN,
  #endif
  #if HAS_TEMP_ADC_3
    TEMP_3_PIN,
  #endif
  #if HAS_TEMP_ADC_4
    TEMP_4_PIN,
  #endif
  #if HAS_TEMP_ADC_5
    TEMP_5_PIN,
  #endif
  #if HAS_TEMP_ADC_6
    TEMP_6_PIN,
  #endif
  #if HAS_TEMP_ADC_7
    TEMP_7_PIN,
  #endif
  #if ENABLED(FILAMENT_WIDTH_SENSOR)
    FILWIDTH_PIN,
  #endif
  #if HAS_ADC_BUTTONS
    ADC_KEYPAD_PIN,
  #endif
  #if HAS_JOY_ADC_X
    JOY_X_PIN,
  #endif
  #if HAS_JOY_ADC_Y
    JOY_Y_PIN,
  #endif
  #if HAS_JOY_ADC_Z
    JOY_Z_PIN,
  #endif
  #if ENABLED(POWER_MONITOR_CURRENT)
    POWER_MONITOR_CURRENT_PIN,
  #endif
  #if ENABLED(POWER_MONITOR_VOLTAGE)
    POWER_MONITOR_VOLTAGE_PIN,
  #endif
};

enum TempPinIndex : char {
  #if HAS_TEMP_ADC_0
    TEMP_0,
  #endif
  #if HAS_TEMP_ADC_PROBE
    TEMP_PROBE,
  #endif
  #if HAS_HEATED_BED
    TEMP_BED,
  #endif
  #if HAS_TEMP_CHAMBER
    TEMP_CHAMBER,
  #endif
  #if HAS_TEMP_COOLER
    TEMP_COOLER_PIN,
  #endif
  #if HAS_TEMP_ADC_1
    TEMP_1,
  #endif
  #if HAS_TEMP_ADC_2
    TEMP_2,
  #endif
  #if HAS_TEMP_ADC_3
    TEMP_3,
  #endif
  #if HAS_TEMP_ADC_4
    TEMP_4,
  #endif
  #if HAS_TEMP_ADC_5
    TEMP_5,
  #endif
  #if HAS_TEMP_ADC_6
    TEMP_6,
  #endif
  #if HAS_TEMP_ADC_7
    TEMP_7,
  #endif
  #if ENABLED(FILAMENT_WIDTH_SENSOR)
    FILWIDTH,
  #endif
  #if HAS_ADC_BUTTONS
    ADC_KEY,
  #endif
  #if HAS_JOY_ADC_X
    JOY_X,
  #endif
  #if HAS_JOY_ADC_Y
    JOY_Y,
  #endif
  #if HAS_JOY_ADC_Z
    JOY_Z,
  #endif
  #if ENABLED(POWER_MONITOR_CURRENT)
    POWERMON_CURRENT,
  #endif
  #if ENABLED(POWER_MONITOR_VOLTAGE)
    POWERMON_VOLTS,
  #endif
  ADC_PIN_COUNT
};

uint16_t HAL_adc_results[ADC_PIN_COUNT];

// ------------------------
// Private functions
// ------------------------
static void NVIC_SetPriorityGrouping(uint32_t PriorityGroup) {
  uint32_t reg_value;
  uint32_t PriorityGroupTmp = (PriorityGroup & (uint32_t)0x07);               /* only values 0..7 are used          */

  reg_value  =  SCB->AIRCR;                                                   /* read old register configuration    */
  reg_value &= ~(SCB_AIRCR_VECTKEY_Msk | SCB_AIRCR_PRIGROUP_Msk);             /* clear bits to change               */
  reg_value  =  (reg_value                                 |
                ((uint32_t)0x5FA << SCB_AIRCR_VECTKEY_Pos) |
                (PriorityGroupTmp << 8));                                     /* Insert write key & priority group  */
  SCB->AIRCR =  reg_value;
}

// ------------------------
// Public functions
// ------------------------

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

void HAL_init() {
  NVIC_SetPriorityGrouping(0x3);
  #if PIN_EXISTS(LED)
    OUT_WRITE(LED_PIN, LOW);
  #endif
  #if HAS_SD_HOST_DRIVE
    MSC_SD_init();
  #elif BOTH(SERIAL_USB, EMERGENCY_PARSER)
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
void HAL_idletask() {
  #if HAS_SHARED_MEDIA
    // If Marlin is using the SD card we need to lock it to prevent access from
    // a PC via USB.
    // Other HALs use IS_SD_PRINTING() and IS_SD_FILE_OPEN() to check for access but
    // this will not reliably detect delete operations. To be safe we will lock
    // the disk if Marlin has it mounted. Unfortunately there is currently no way
    // to unmount the disk from the LCD menu.
    // if (IS_SD_PRINTING() || IS_SD_FILE_OPEN())
    /* copy from lpc1768 framework, should be fixed later for process HAS_SD_HOST_DRIVE*/
    // process USB mass storage device class loop
    MarlinMSC.loop();
  #endif
}

void HAL_clear_reset_source() { }

/**
 * TODO: Check this and change or remove.
 */
uint8_t HAL_get_reset_source() { return RST_POWER_ON; }

void _delay_ms(const int delay_ms) { delay(delay_ms); }

extern "C" {
  extern unsigned int _ebss; // end of bss section
}

/**
 * TODO: Change this to correct it for libmaple
 */

// return free memory between end of heap (or end bss) and whatever is current

/*
#include <wirish/syscalls.c>
//extern caddr_t _sbrk(int incr);
#ifndef CONFIG_HEAP_END
extern char _lm_heap_end;
#define CONFIG_HEAP_END ((caddr_t)&_lm_heap_end)
#endif

extern "C" {
  static int freeMemory() {
    char top = 't';
    return &top - reinterpret_cast<char*>(sbrk(0));
  }
  int freeMemory() {
    int free_memory;
    int heap_end = (int)_sbrk(0);
    free_memory = ((int)&free_memory) - ((int)heap_end);
    return free_memory;
  }
}
*/

// ------------------------
// ADC
// ------------------------
// Init the AD in continuous capture mode
void HAL_adc_init() {
  // configure the ADC
  adc.calibrate();
  #if F_CPU > 72000000
    adc.setSampleRate(ADC_SMPR_71_5); // 71.5 ADC cycles
  #else
    adc.setSampleRate(ADC_SMPR_41_5); // 41.5 ADC cycles
  #endif
  adc.setPins((uint8_t *)adc_pins, ADC_PIN_COUNT);
  adc.setDMA(HAL_adc_results, (uint16_t)ADC_PIN_COUNT, (uint32_t)(DMA_MINC_MODE | DMA_CIRC_MODE), nullptr);
  adc.setScanMode();
  adc.setContinuous();
  adc.startConversion();
}

void HAL_adc_start_conversion(const uint8_t adc_pin) {
  //TEMP_PINS pin_index;
  TempPinIndex pin_index;
  switch (adc_pin) {
    default: return;
    #if HAS_TEMP_ADC_0
      case TEMP_0_PIN: pin_index = TEMP_0; break;
    #endif
    #if HAS_TEMP_ADC_PROBE
      case TEMP_PROBE_PIN: pin_index = TEMP_PROBE; break;
    #endif
    #if HAS_HEATED_BED
      case TEMP_BED_PIN: pin_index = TEMP_BED; break;
    #endif
    #if HAS_TEMP_CHAMBER
      case TEMP_CHAMBER_PIN: pin_index = TEMP_CHAMBER; break;
    #endif
    #if HAS_TEMP_COOLER
      case TEMP_COOLER_PIN: pin_index = TEMP_COOLER; break;
    #endif
    #if HAS_TEMP_ADC_1
      case TEMP_1_PIN: pin_index = TEMP_1; break;
    #endif
    #if HAS_TEMP_ADC_2
      case TEMP_2_PIN: pin_index = TEMP_2; break;
    #endif
    #if HAS_TEMP_ADC_3
      case TEMP_3_PIN: pin_index = TEMP_3; break;
    #endif
    #if HAS_TEMP_ADC_4
      case TEMP_4_PIN: pin_index = TEMP_4; break;
    #endif
    #if HAS_TEMP_ADC_5
      case TEMP_5_PIN: pin_index = TEMP_5; break;
    #endif
    #if HAS_TEMP_ADC_6
      case TEMP_6_PIN: pin_index = TEMP_6; break;
    #endif
    #if HAS_TEMP_ADC_7
      case TEMP_7_PIN: pin_index = TEMP_7; break;
    #endif
    #if HAS_JOY_ADC_X
      case JOY_X_PIN: pin_index = JOY_X; break;
    #endif
    #if HAS_JOY_ADC_Y
      case JOY_Y_PIN: pin_index = JOY_Y; break;
    #endif
    #if HAS_JOY_ADC_Z
      case JOY_Z_PIN: pin_index = JOY_Z; break;
    #endif
    #if ENABLED(FILAMENT_WIDTH_SENSOR)
      case FILWIDTH_PIN: pin_index = FILWIDTH; break;
    #endif
    #if HAS_ADC_BUTTONS
      case ADC_KEYPAD_PIN: pin_index = ADC_KEY; break;
    #endif
    #if ENABLED(POWER_MONITOR_CURRENT)
      case POWER_MONITOR_CURRENT_PIN: pin_index = POWERMON_CURRENT; break;
    #endif
    #if ENABLED(POWER_MONITOR_VOLTAGE)
      case POWER_MONITOR_VOLTAGE_PIN: pin_index = POWERMON_VOLTS; break;
    #endif
  }
  HAL_adc_result = HAL_adc_results[(int)pin_index] >> (12 - HAL_ADC_RESOLUTION); // shift out unused bits
}

uint16_t HAL_adc_get_result() { return HAL_adc_result; }

uint16_t analogRead(pin_t pin) {
  const bool is_analog = _GET_MODE(pin) == GPIO_INPUT_ANALOG;
  return is_analog ? analogRead(uint8_t(pin)) : 0;
}

// Wrapper to maple unprotected analogWrite
void analogWrite(pin_t pin, int pwm_val8) {
  if (PWM_PIN(pin))
    analogWrite(uint8_t(pin), pwm_val8);
}

void HAL_reboot() { nvic_sys_reset(); }

void flashFirmware(const int16_t) { HAL_reboot(); }

#endif // __STM32F1__
