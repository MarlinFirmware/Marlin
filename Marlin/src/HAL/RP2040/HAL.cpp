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

// Core 1 watchdog configuration
#define CORE1_MAX_RESETS 5  // Maximum number of Core 1 resets before halting system

// ------------------------
// Public Variables
// ------------------------

volatile uint32_t adc_accumulators[5] = {0}; // Accumulators for oversampling (sum of readings)
volatile uint8_t adc_counts[5] = {0};        // Count of readings accumulated per channel
volatile uint16_t adc_values[5] = {512, 512, 512, 512, 512}; // Final oversampled ADC values (averages) - initialized to mid-range

// Core 1 watchdog monitoring
volatile uint32_t core1_last_heartbeat = 0; // Timestamp of Core 1's last activity
volatile bool core1_watchdog_triggered = false; // Flag to indicate Core 1 reset
volatile uint8_t core1_reset_count = 0; // Count of Core 1 resets - halt system if >= CORE1_MAX_RESETS
volatile uint8_t current_pin;
volatile bool MarlinHAL::adc_has_result;
volatile uint8_t adc_channels_enabled[5] = {false}; // Track which ADC channels are enabled

// Helper function for LED blinking patterns
void blink_led_pattern(uint8_t blink_count, uint32_t blink_duration_us = 100000) {
  #if DISABLED(PINS_DEBUGGING) && PIN_EXISTS(LED)
    for (uint8_t i = 0; i < blink_count; i++) {
      WRITE(LED_PIN, HIGH);
      busy_wait_us(blink_duration_us);
      WRITE(LED_PIN, LOW);
      if (i < blink_count - 1) { // Don't delay after the last blink
        busy_wait_us(blink_duration_us);
      }
    }
  #endif
}

// Core 1 ADC reading task - dynamically reads all enabled channels with oversampling
void core1_adc_task() {
  static uint32_t last_led_toggle = 0;
  const uint8_t OVERSAMPLENR = 16; // Standard Marlin oversampling count

  // Signal successful Core 1 startup/restart
  SERIAL_ECHO_MSG("Core 1 ADC task started");

  while (true) {
    // Update heartbeat timestamp at start of each scan cycle
    core1_last_heartbeat = time_us_32();

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

      // Update the averaged value with current accumulation (provides immediate valid data)
      adc_values[channel] = adc_accumulators[channel] / adc_counts[channel];

      // When we reach the full oversampling count, reset accumulator for next cycle
      if (adc_counts[channel] >= OVERSAMPLENR) {
        adc_accumulators[channel] = 0;
        adc_counts[channel] = 0;
      }

      // Disable temp sensor after reading to save power
      if (channel == 4) {
        adc_set_temp_sensor_enabled(false);
      }
    }

    // Core 1 LED indicator: Double blink every 2 seconds to show Core 1 is active
    uint32_t now = time_us_32();
    if (now - last_led_toggle >= 2000000) { // 2 seconds
      last_led_toggle = now;
      #if DISABLED(PINS_DEBUGGING) && PIN_EXISTS(LED)
        // Triple blink pattern if watchdog was triggered (shows Core 1 was reset)
        if (core1_watchdog_triggered) {
          core1_watchdog_triggered = false; // Clear flag
          blink_led_pattern(3); // Triple blink for watchdog reset
        } else {
          blink_led_pattern(2); // Normal double blink
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
      watchdog_enable(WDT_TIMEOUT_US/1000, true);
    #endif
  }

  void MarlinHAL::watchdog_refresh() {
    // If Core 1 has reset CORE1_MAX_RESETS+ times, stop updating watchdog to halt system
    if (core1_reset_count >= CORE1_MAX_RESETS) {
      SERIAL_ECHO_MSG("Core 1 reset limit exceeded (", core1_reset_count, " resets) - halting system for safety");
      return; // Don't update watchdog - system will halt
    }

    watchdog_update();

    // Check Core 1 watchdog (15 second timeout)
    uint32_t now = time_us_32();
    if (now - core1_last_heartbeat > 15000000) { // 15 seconds
      // Core 1 appears stuck - reset it
      multicore_reset_core1();
      multicore_launch_core1(core1_adc_task);
      core1_watchdog_triggered = true; // Signal for LED indicator
      core1_reset_count++; // Increment reset counter
      SERIAL_ECHO_MSG("Core 1 ADC watchdog triggered - resetting Core 1 (attempt ", core1_reset_count, ")");
    }

    #if DISABLED(PINS_DEBUGGING) && PIN_EXISTS(LED)
      // Core 0 LED indicator: Single toggle every watchdog refresh (shows Core 0 activity)
      TOGGLE(LED_PIN);
    #endif
  }

#endif

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
  extern unsigned int __bss_end__; // end of bss section
}

// Return free memory between end of heap (or end bss) and whatever is current
int freeMemory() {
  int free_memory, heap_end = (int)_sbrk(0);
  return (int)&free_memory - (heap_end ?: (int)&__bss_end__);
}

#endif // __PLAT_RP2040__
