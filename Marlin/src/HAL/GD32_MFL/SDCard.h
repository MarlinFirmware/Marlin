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

#include "../../inc/MarlinConfig.h"

#include "SDIO.hpp"

namespace sdio {

class DMA;

class CardDMA {
public:
  static CardDMA& get_instance();

  SDIO_Error_Type init();
  SDIO_Error_Type card_init();
  SDIO_Error_Type begin_startup_procedure();
  void begin_shutdown_procedure();
  // Configuration
  SDIO_Error_Type set_hardware_bus_width(Bus_Width width);
  // Main read/write functions for single and multiblock transfers
  SDIO_Error_Type read(uint8_t* buf, uint32_t address, uint32_t count);
  SDIO_Error_Type write(uint8_t* buf, uint32_t address, uint32_t count);
  // DMA transfers
  // Other card functions
  SDIO_Error_Type erase(uint32_t address_start, uint32_t address_end);
  // Interrupt handler
  void handle_interrupts();
  // Card select
  SDIO_Error_Type select_deselect();

  SDIO_Error_Type get_card_interface_status(uint32_t* status);
  SDIO_Error_Type get_sdcard_status(uint32_t* status);

  void check_dma_complete();
  SDIO_Error_Type stop_transfer();

  Transfer_State get_transfer_state();
  uint32_t get_card_capacity() const;

  SDIO_Error_Type send_bus_width_command(uint32_t width_value);

  SDIO_Error_Type get_card_specific_data(Card_Info* info);
  constexpr Block_Size get_data_block_size_index(uint16_t size);

  SDIO_Error_Type get_card_state(Card_State* card_state);
  SDIO_Error_Type check_sdio_status(Command_Index index = Command_Index::INVALID, bool check_index = false, bool ignore_crc = false);

  // DMA configuration
  void set_dma_parameters(uint8_t* buf, uint32_t count, bool is_write);

  // SDIO configuration
  void sdio_configure(const SDIO_Config config) { sdio_.init(config); }

  // Varaible stored parameters
  SDIO_Error_Type get_scr(uint16_t rca, uint32_t* scr);
  SDIO_Error_Type store_cid();
  SDIO_Error_Type store_csd();

  // Accessor methods
  SDIO_Config& get_config() { return config_; }
  dma::DMA& get_dma_instance() { return dma_; }
  void set_data_end_interrupt() { sdio_.set_interrupt_enable(Interrupt_Type::DTENDIE, true); }
  void set_sdio_dma_enable(bool enable) { sdio_.set_dma_enable(enable); }
  bool get_is_sdio_rx() { return is_rx_; }
  void clear_sdio_data_flags() { sdio_.clear_multiple_interrupt_flags(clear_data_flags); }
  void clear_sdio_cmd_flags() { sdio_.clear_multiple_interrupt_flags(clear_command_flags); }
  void clear_sdio_common_flags() { sdio_.clear_multiple_interrupt_flags(clear_common_flags); }
  Operational_State get_state() { return current_state_; }
  void set_state(Operational_State state) { current_state_ = state; }
  void set_transfer_end(bool value) { transfer_end_ = value; }
  void set_transfer_error(SDIO_Error_Type error) { transfer_error_ = error; }

  inline SDIO_Error_Type set_desired_clock(uint32_t desired_clock, bool wide_bus, bool low_power) {
    sdio_.init({
      desired_clock,
      Clock_Edge::RISING_EDGE,
      wide_bus ? Bus_Width::WIDTH_4BIT : Bus_Width::WIDTH_1BIT,
      false,
      low_power,
      false
    });
    sync_domains();
    desired_clock_ = desired_clock;

    return SDIO_Error_Type::OK;
  }

private:
  CardDMA();

  // Prevent copying or assigning
  CardDMA(const CardDMA&) = delete;
  CardDMA& operator=(const CardDMA&) = delete;

  // Helper function
  SDIO_Error_Type wait_for_card_ready();

  // Member variables
  alignas(4) uint32_t sdcard_csd_[4];
  alignas(4) uint32_t sdcard_cid_[4];
  alignas(4) uint32_t sdcard_scr_[2];
  uint32_t desired_clock_;
  uint32_t stop_condition_;
  uint32_t total_bytes_;
  uint32_t count_;
  SDIO& sdio_;
  SDIO_Config& config_;
  const dma::DMA_Base dmaBase_;
  const dma::DMA_Channel dmaChannel_;
  dma::DMA& dma_;
  uint16_t sdcard_rca_;
  SDIO_Error_Type transfer_error_;
  Interface_Version interface_version_;
  Card_Type card_type_;
  volatile bool transfer_end_;
  volatile bool is_rx_;
  volatile bool multiblock_;
  volatile Operational_State current_state_;

  // Private helper methods
  SDIO_Error_Type validate_voltage();
  SDIO_Error_Type get_r1_result(Command_Index index);
  //SDIO_Error_Type get_r2_r3_result();
  SDIO_Error_Type get_r6_result(Command_Index index, uint16_t* rca);
  SDIO_Error_Type get_r7_result();
  //SDIO_Error_Type get_r1_error_type(uint32_t response);
  SDIO_Error_Type get_command_sent_result();

  inline void sync_domains() {
    delayMicroseconds(8);
  }

  inline bool validate_transfer_params(uint32_t* buf, uint16_t size) {
    if (buf == nullptr) return false;
    // Size must be > 0, <= 2048 and power of 2
    if ((size == 0U) || (size > 2048U) || (size & (size - 1U))) {
      return false;
    }
    return true;
  }

  void process_sdsc_specific_csd(Card_Info* info, const uint8_t* csd_bytes) {
    info->csd.device_size = (static_cast<uint32_t>(csd_bytes[6] & 0x03U) << 10U) |
                 (static_cast<uint32_t>(csd_bytes[7]) << 2U) |
                 (static_cast<uint32_t>((csd_bytes[8] & 0xC0U) >> 6U));
    info->csd.device_size_multiplier = static_cast<uint8_t>((csd_bytes[9] & 0x03U) << 1U |
                                 (csd_bytes[10] & 0x80U) >> 7U);

    info->block_size = static_cast<uint32_t>(1 << info->csd.read_block_length);
    info->capacity = static_cast<uint32_t>((info->csd.device_size + 1U) *
            (1U << (info->csd.device_size_multiplier + 2U)) *
            info->block_size);
  }

  void process_sdhc_specific_csd(Card_Info* info, const uint8_t* csd_bytes) {
    info->csd.device_size = static_cast<uint32_t>((csd_bytes[7] & 0x3FU) << 16U) |
                 static_cast<uint32_t>((csd_bytes[8]) << 8U) |
                 static_cast<uint32_t>(csd_bytes[9]);

    info->block_size = BLOCK_SIZE;
    info->capacity = static_cast<uint32_t>((info->csd.device_size + 1U) *
            BLOCK_SIZE * KILOBYTE);
  }

  void process_common_csd_tail(Card_Info* info, const uint8_t* csd_bytes) {
    info->csd.sector_size = static_cast<uint8_t>(((csd_bytes[9] & 0x3FU) << 1U) |
                          (csd_bytes[10] & 0x80U) >> 7U);
    info->csd.speed_factor = static_cast<uint8_t>((csd_bytes[11] & 0x1CU) >> 2U);
    info->csd.write_block_length = static_cast<uint8_t>(((csd_bytes[11] & 0x03U) << 2U) |
                              ((csd_bytes[12] & 0xC0U) >> 6U));
    info->csd.checksum = static_cast<uint8_t>((csd_bytes[15] & 0xFEU) >> 1U);
  }

  inline void disable_all_interrupts() {
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
  inline SDIO_Error_Type send_command_and_check(Command_Index command, uint32_t argument,
           Command_Response response, Wait_Type type, CheckFunc check_result) {
    sdio_.set_command_state_machine(command, argument, response, type);
    sync_domains();
    sdio_.set_command_state_machine_enable(true);
    return check_result();
  }
};

} // namespace sdio

extern sdio::CardDMA& CardDMA_I;
