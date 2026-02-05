/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2024 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#include "../platforms.h"

#ifdef __PLAT_RP2040__

#include "HAL.h"
//#include "usb_serial.h"

#include "../../inc/MarlinConfig.h"
#include "../shared/Delay.h"
#include "../../module/temperature.h"  // For OVERSAMPLENR

extern "C" {
  #include "pico/bootrom.h"
  #include "hardware/watchdog.h"
  #include "pico/multicore.h"
  #include "hardware/adc.h"
  #include "pico/time.h"
}

#if HAS_SD_HOST_DRIVE
  #include "msc_sd.h"
#endif

// ------------------------
// Public Variables
// ------------------------

volatile uint32_t adc_accumulators[5] = {0}; // Accumulators for oversampling (sum of readings)
volatile uint8_t adc_counts[5] = {0};        // Count of readings accumulated per channel
volatile uint16_t adc_values[5] = {4095, 4095, 4095, 4095, 4095}; // Averaged ADC values (single reading equivalent) - initialized to max (open circuit)

// Core monitoring for watchdog
volatile uint32_t core0_last_heartbeat = 0; // Timestamp of Core 0's last activity
volatile uint32_t core1_last_heartbeat = 0; // Timestamp of Core 1's last activity
#if ENABLED(MARLIN_DEV_MODE)
  volatile bool core1_freeze_test = false;  // Flag to freeze Core 1 for watchdog testing
#endif
volatile uint8_t current_pin;
volatile bool MarlinHAL::adc_has_result;
volatile uint8_t adc_channels_enabled[5] = {false}; // Track which ADC channels are enabled

// Core 1 ADC reading task - dynamically reads all enabled channels with oversampling
void core1_adc_task() {
  static uint32_t last_temp_update = 0;

  while (true) {
    #if ENABLED(MARLIN_DEV_MODE)
      // Check if we should freeze for watchdog test
      if (core1_freeze_test) {
        // Stop updating heartbeat and spin forever
        while (core1_freeze_test) {
          busy_wait_us(100000); // 100ms delay
        }
      }
    #endif

    // Scan all enabled ADC channels
    for (uint8_t channel = 0; channel < 5; channel++) {
      if (!adc_channels_enabled[channel]) continue;

      // Enable temperature sensor if reading channel 4
      if (channel == 4) {
        adc_set_temp_sensor_enabled(true);
      }

      // Select and read the channel
      adc_select_input(channel);
      busy_wait_us(100); // Settling delay
      adc_fifo_drain();
      adc_run(true);

      // Wait for conversion with timeout
      uint32_t timeout = 10000;
      while (adc_fifo_is_empty() && timeout--) {
        busy_wait_us(1);
      }

      adc_run(false);
      uint16_t reading = adc_fifo_is_empty() ? 0 : adc_fifo_get();

      // Accumulate readings for oversampling
      adc_accumulators[channel] += reading;
      adc_counts[channel]++;

      // When we reach the full oversampling count, calculate averaged value (Marlin ISR does its own oversampling)
      if (adc_counts[channel] >= OVERSAMPLENR) {
        adc_values[channel] = adc_accumulators[channel] / OVERSAMPLENR; // Return single-reading equivalent
        adc_accumulators[channel] = 0;
        adc_counts[channel] = 0;
      }

      // Disable temp sensor after reading to save power
      if (channel == 4) {
        adc_set_temp_sensor_enabled(false);
      }
    }

    // Core 1 just provides ADC readings - don't trigger temperature updates from here
    // Let Marlin's main temperature ISR on Core 0 handle the timing and updates
    uint32_t now = time_us_32();
    if (now - last_temp_update >= 100000) { // 100ms = 100000 microseconds
      last_temp_update = now;
      #if ENABLED(USE_WATCHDOG)
        // Refresh watchdog here like AVR ISR does indirectly via temperature updates
        // Use 2 second delay to allow watchdog_init to be called during boot
        static uint32_t core1_start_time = 0;
        if (core1_start_time == 0) core1_start_time = time_us_32();

        if (time_us_32() - core1_start_time > 2000000) {
          hal.watchdog_refresh(1); // Refresh from Core 1
        }
      #endif
    }

    // Delay between full scan cycles
    busy_wait_us(10000); // 10ms between scans
  }
}

volatile uint16_t adc_result;

// ------------------------
// Public functions
// ------------------------

TERN_(POSTMORTEM_DEBUGGING, extern void install_min_serial());

// HAL initialization task
void MarlinHAL::init() {
  // Ensure F_CPU is a constant expression.
  // If the compiler breaks here, it means that delay code that should compute at compile time will not work.
  // So better safe than sorry here.
  constexpr unsigned int cpuFreq = F_CPU;
  UNUSED(cpuFreq);

  #if HAS_MEDIA && DISABLED(ONBOARD_SDIO) && PIN_EXISTS(SD_SS)
    OUT_WRITE(SD_SS_PIN, HIGH); // Try to set SD_SS_PIN inactive before any other SPI users start up
  #endif

  #if PIN_EXISTS(LED)
    OUT_WRITE(LED_PIN, LOW);
  #endif

  #if ENABLED(SRAM_EEPROM_EMULATION)
    // __HAL_RCC_PWR_CLK_ENABLE();
    // HAL_PWR_EnableBkUpAccess();           // Enable access to backup SRAM
    // __HAL_RCC_BKPSRAM_CLK_ENABLE();
    // LL_PWR_EnableBkUpRegulator();         // Enable backup regulator
    // while (!LL_PWR_IsActiveFlag_BRR());   // Wait until backup regulator is initialized
  #endif

  HAL_timer_init();

  #if ALL(EMERGENCY_PARSER, USBD_USE_CDC)
    USB_Hook_init();
  #endif

  TERN_(POSTMORTEM_DEBUGGING, install_min_serial());    // Install the min serial handler

  TERN_(HAS_SD_HOST_DRIVE, MSC_SD_init());              // Enable USB SD card access

  #if PIN_EXISTS(USB_CONNECT)
    OUT_WRITE(USB_CONNECT_PIN, !USB_CONNECT_INVERTING); // USB clear connection
    delay_ms(1000);                                     // Give OS time to notice
    WRITE(USB_CONNECT_PIN, USB_CONNECT_INVERTING);
  #endif
}

uint8_t MarlinHAL::get_reset_source() {
  return watchdog_enable_caused_reboot() ? RST_WATCHDOG : 0;
}

void MarlinHAL::reboot() { watchdog_reboot(0, 0, 1); }

// ------------------------
// Watchdog Timer
// ------------------------

#if ENABLED(USE_WATCHDOG)

  #define WDT_TIMEOUT_US TERN(WATCHDOG_DURATION_8S, 8000000, 4000000) // 4 or 8 second timeout

  extern "C" {
    #include "hardware/watchdog.h"
  }

  void MarlinHAL::watchdog_init() {
    #if DISABLED(DISABLE_WATCHDOG_INIT)
      static_assert(WDT_TIMEOUT_US > 1000, "WDT Timeout is too small, aborting");
      // Initialize Core 0 heartbeat
      core0_last_heartbeat = time_us_32();
      watchdog_enable(WDT_TIMEOUT_US/1000, true);
    #endif
  }

  void MarlinHAL::watchdog_refresh(const uint8_t core/*=0*/) {
    if (core == 0) {
      // Update Core 0 heartbeat
      core0_last_heartbeat = time_us_32();

      // Check if Core 1 is alive (2 second timeout)
      if (time_us_32() - core1_last_heartbeat < 2000000) {
        watchdog_update(); // Only refresh if Core 1 is responding
        #if DISABLED(PINS_DEBUGGING) && PIN_EXISTS(LED)
          TOGGLE(LED_PIN); // Heartbeat indicator
        #endif
      }
      // If Core 1 is stuck, don't refresh - let watchdog reset the system
    }
    else {
      // Update Core 1 heartbeat
      core1_last_heartbeat = time_us_32();

      // Check if Core 0 is alive (2 second timeout)
      if (time_us_32() - core0_last_heartbeat < 2000000) {
        watchdog_update(); // Only refresh if Core 0 is responding
        #if DISABLED(PINS_DEBUGGING) && PIN_EXISTS(LED)
          TOGGLE(LED_PIN); // Heartbeat indicator
        #endif
      }
      // If Core 0 is stuck, don't refresh - let watchdog reset the system
    }
  }

#endif // USE_WATCHDOG

// ------------------------
// ADC
// ------------------------

void MarlinHAL::adc_init() {
  analogReadResolution(HAL_ADC_RESOLUTION);
  ::adc_init();
  adc_fifo_setup(true, false, 1, false, false);
  // Launch Core 1 for continuous ADC reading
  multicore_launch_core1(core1_adc_task);
  adc_has_result = true; // Results are always available with continuous sampling
}

void MarlinHAL::adc_enable(const pin_t pin) {
  if (pin >= A0 && pin <= A3) {
    adc_gpio_init(pin);
    adc_channels_enabled[pin - A0] = true; // Mark this channel as enabled
  }
  else if (pin == HAL_ADC_MCU_TEMP_DUMMY_PIN) {
    adc_channels_enabled[4] = true; // Mark MCU temp channel as enabled
  }
}

void MarlinHAL::adc_start(const pin_t pin) {
  // Just store which pin we need to read - values are continuously updated by Core 1
  current_pin = pin;
}

uint16_t MarlinHAL::adc_value() {
  // Return the latest ADC value from Core 1's continuous readings
  const uint8_t channel = (current_pin == HAL_ADC_MCU_TEMP_DUMMY_PIN) ? 4 : (current_pin - A0);
  return adc_values[channel];
}

// Reset the system to initiate a firmware flash
void flashFirmware(const int16_t) { hal.reboot(); }

extern "C" {
  void * _sbrk(int incr);
  extern unsigned int __StackLimit;    // Lowest address the stack can grow to
}

// Return free memory between end of heap and start of stack
int freeMemory() {
  void* heap_end = _sbrk(0);
  // Use the linker-provided stack limit instead of a local variable
  // __StackLimit is the lowest address the stack can grow to
  return (char*)&__StackLimit - (char*)heap_end;
}

#endif // __PLAT_RP2040__
