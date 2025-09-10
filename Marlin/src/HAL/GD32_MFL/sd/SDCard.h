//
// MFL gd32f30x SDCARD using DMA through SDIO in C++
//
// Copyright (C) 2025 B. Mourit <bnmguy@gmail.com>
//
// This software is free software: you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with this software.
// If not, see <https://www.gnu.org/licenses/>.
//
#pragma once

#include "../../../inc/MarlinConfig.h"

#include <SDIO.hpp>

namespace sdio {

class DMA;

class CardDMA {
public:
  static auto get_instance() -> CardDMA&;

  // Initialization
  auto init() -> SDIO_Error_Type;
  auto card_init() -> SDIO_Error_Type;

  // Startup and shutdown procedures
  auto begin_startup_procedure() -> SDIO_Error_Type;
  void begin_shutdown_procedure();

  // Configuration
  auto set_hardware_bus_width(Bus_Width width) -> SDIO_Error_Type;
  auto send_bus_width_command(uint32_t width_value) -> SDIO_Error_Type;

  // Main read/write/erase functions
  auto read(uint8_t* buf, uint32_t address, uint32_t count) -> SDIO_Error_Type;
  auto write(uint8_t* buf, uint32_t address, uint32_t count) -> SDIO_Error_Type;
  auto erase(uint32_t address_start, uint32_t address_end) -> SDIO_Error_Type;

  // Card select
  auto select_deselect() -> SDIO_Error_Type;

  // Status and state
  auto get_card_interface_status(uint32_t* status) -> SDIO_Error_Type;
  auto get_sdcard_status(uint32_t* status) -> SDIO_Error_Type;
  auto get_transfer_state() -> Transfer_State;
  auto get_card_state(Card_State* card_state) -> SDIO_Error_Type;
  auto check_sdio_status(Command_Index index = Command_Index::INVALID, bool check_index = false, bool ignore_crc = false) -> SDIO_Error_Type;

  // DMA
  void set_dma_parameters(uint8_t* buf, uint32_t count, bool is_write);
  void check_dma_complete();

  // Stop transfer
  auto stop_transfer() -> SDIO_Error_Type;

  // Card information
  auto get_card_specific_data(Card_Info* info) -> SDIO_Error_Type;
  constexpr auto get_data_block_size_index(uint16_t size) -> Block_Size;
  [[nodiscard]] auto get_card_capacity() const -> uint32_t;

  // SDIO configuration
  void sdio_configure(const SDIO_Config config) { sdio_.init(config); }

  // Interrupt handler
  void handle_interrupts();

  // Variable stored parameters
  auto get_scr(uint16_t rca, uint32_t* scr) -> SDIO_Error_Type;
  auto store_cid() -> SDIO_Error_Type;
  auto store_csd() -> SDIO_Error_Type;

  // Inlined accessor methods
  auto get_config() -> SDIO_Config& { return config_; }
  auto get_dma_instance() -> dma::DMA& { return dma_; }
  void set_data_end_interrupt() { sdio_.set_interrupt_enable(Interrupt_Type::DTENDIE, true); }
  void set_sdio_dma_enable(bool enable) { sdio_.set_dma_enable(enable); }
  auto get_is_sdio_rx() -> bool { return is_rx_; }
  void clear_sdio_data_flags() { sdio_.clear_multiple_interrupt_flags(clear_data_flags); }
  void clear_sdio_cmd_flags() { sdio_.clear_multiple_interrupt_flags(clear_command_flags); }
  void clear_sdio_common_flags() { sdio_.clear_multiple_interrupt_flags(clear_common_flags); }
  auto get_state() -> Operational_State { return current_state_; }
  void set_state(Operational_State state) { current_state_ = state; }
  void set_transfer_error(SDIO_Error_Type error) { transfer_error_ = error; }
  void set_transfer_end(bool end) { transfer_end_ = end; };

  auto set_desired_clock(uint32_t desired_clock, bool wide_bus, bool low_power) -> SDIO_Error_Type {
    sdio_.init(SDIO_Config{
      .desired_clock = desired_clock,
      .enable_bypass = false,
      .enable_powersave = low_power,
      .enable_hwclock = false,
      .clock_edge = Clock_Edge::RISING_EDGE,
      .width = wide_bus ? Bus_Width::WIDTH_4BIT : Bus_Width::WIDTH_1BIT
    });

    sync_domains();
    desired_clock_ = desired_clock;

    return SDIO_Error_Type::OK;
  }

private:
  CardDMA();

  // Prevent copying or assigning
  CardDMA(const CardDMA&) = delete;
  auto operator=(const CardDMA&) -> CardDMA& = delete;

  // Helper function
  auto wait_for_card_ready() -> SDIO_Error_Type;

  // Member variables
  alignas(4) uint32_t sdcard_csd_[4];
  alignas(4) uint32_t sdcard_cid_[4];
  alignas(4) uint32_t sdcard_scr_[2];
  uint32_t desired_clock_;
  uint32_t total_bytes_;
  SDIO& sdio_;
  SDIO_Config& config_;
  const dma::DMA_Base dmaBase_;
  const dma::DMA_Channel dmaChannel_;
  dma::DMA& dma_;
  uint16_t sdcard_rca_;
  SDIO_Error_Type transfer_error_;
  Interface_Version interface_version_;
  Card_Type card_type_;
  Operational_State current_state_;
  bool transfer_end_;
  bool multiblock_;
  bool is_rx_;

  // Private helper methods
  auto validate_voltage() -> SDIO_Error_Type;
  auto get_command_sent_result() -> SDIO_Error_Type;
  auto get_r1_result(Command_Index index) -> SDIO_Error_Type;
  auto get_r6_result(Command_Index index, uint16_t* rca) -> SDIO_Error_Type;
  auto get_r7_result() -> SDIO_Error_Type { return check_sdio_status(Command_Index::INVALID, false, false); };
  void sync_domains() { delayMicroseconds(8); }

  auto validate_transfer_params(uint32_t* buf, uint16_t size) -> bool {
    if (buf == nullptr) return false;
    // Size must be > 0, <= 2048 and power of 2
    return size > 0U && size <= 2048U && !(size & (size - 1U));
  }

  void process_sdsc_specific_csd(Card_Info* info, const uint8_t* csd_bytes) {
    const uint32_t device_size = ((csd_bytes[6] & 0x3U) << 10) |
                                 (csd_bytes[7] << 2) |
                                 ((csd_bytes[8] >> 6) & 0x3U);

    const uint8_t device_size_multiplier = ((csd_bytes[9] & 0x3U) << 1) |
                                           ((csd_bytes[10] >> 7) & 0x1U);

    // Store calculated values
    info->csd.device_size = device_size;
    info->csd.device_size_multiplier = device_size_multiplier;

    // Calculate block size and capacity
    info->block_size = 1U << info->csd.read_block_length;
    info->capacity = (device_size + 1U) *
                     (1U << (device_size_multiplier + 2U)) *
                     info->block_size;
  }

  void process_sdhc_specific_csd(Card_Info* info, const uint8_t* csd_bytes) {
    info->csd.device_size = static_cast<uint32_t>((csd_bytes[7] & 0x3FU) << 16) |
                            static_cast<uint32_t>((csd_bytes[8]) << 8) |
                            static_cast<uint32_t>(csd_bytes[9]);

    // Set block size and calculate capacity
    info->block_size = BLOCK_SIZE;
    info->capacity = static_cast<uint32_t>((info->csd.device_size + 1U) *
                     BLOCK_SIZE * KILOBYTE);
  }

  void process_common_csd_tail(Card_Info* info, const uint8_t* csd_bytes) {
    // Calculate sector_size
    info->csd.sector_size = static_cast<uint8_t>(((csd_bytes[9] & 0x3FU) << 1) |
                                                  (csd_bytes[10] & 0x80U) >> 7);

    // Calculate speed_factor and write_block_length
    info->csd.speed_factor = static_cast<uint8_t>((csd_bytes[11] & 0x1CU) >> 2);
    info->csd.write_block_length = static_cast<uint8_t>(((csd_bytes[11] & 0x3U) << 2) |
                                                        ((csd_bytes[12] & 0xC0U) >> 6));

    // Calculate checksum
    info->csd.checksum = static_cast<uint8_t>((csd_bytes[15] & 0xFEU) >> 1);
  }

  void disable_all_interrupts() {
    sdio_.set_interrupt_enable(Interrupt_Type::DTCRCERRIE, false);
    sdio_.set_interrupt_enable(Interrupt_Type::DTTMOUTIE, false);
    sdio_.set_interrupt_enable(Interrupt_Type::DTENDIE, false);
    sdio_.set_interrupt_enable(Interrupt_Type::STBITEIE, false);
    sdio_.set_interrupt_enable(Interrupt_Type::TFHIE, false);
    sdio_.set_interrupt_enable(Interrupt_Type::RFHIE, false);
    sdio_.set_interrupt_enable(Interrupt_Type::TXUREIE, false);
    sdio_.set_interrupt_enable(Interrupt_Type::RXOREIE, false);
  }

  template <typename CheckFunc>
  auto send_command_and_check(Command_Index command, uint32_t argument,
     Command_Response response, Wait_Type type, CheckFunc check_result) -> SDIO_Error_Type {
    sdio_.set_command_state_machine(command, argument, response, type);
    sync_domains();
    sdio_.set_command_state_machine_enable(true);
    return check_result();
  }
};

} // namespace sdio

extern sdio::CardDMA& CardDMA_I;
