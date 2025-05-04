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

#include "../../platforms.h"

#ifdef ARDUINO_ARCH_MFL

#include "../../../inc/MarlinConfig.h"
#include "../../shared/Delay.h"

#include "SDCard.h"
#include <string.h>
#include <DMA.hpp>

namespace sdio {

auto CardDMA::get_instance() -> CardDMA& {
  static CardDMA instance;
  return instance;
}

CardDMA::CardDMA() :
    sdcard_csd_{0U, 0U, 0U, 0U},
    sdcard_cid_{0U, 0U, 0U, 0U},
    sdcard_scr_{0U, 0U},
    desired_clock_(Default_Desired_Clock),
    total_bytes_(0U),
    sdio_(SDIO::get_instance()),
    config_(sdio_.get_config()),
    dmaBase_(dma::DMA_Base::DMA1_BASE),
    dmaChannel_(dma::DMA_Channel::CHANNEL3),
    dma_(dma::DMA::get_instance(dmaBase_, dmaChannel_).value()),
    sdcard_rca_(0U),
    transfer_error_(SDIO_Error_Type::OK),
    interface_version_(Interface_Version::UNKNOWN),
    card_type_(Card_Type::UNKNOWN),
    current_state_(Operational_State::READY),
    transfer_end_(false),
    multiblock_(false),
    is_rx_(false)
{
}

// Initialize card and put in standby state
auto CardDMA::init() -> SDIO_Error_Type {
  // Reset SDIO peripheral
  sdio_.reset();
  sync_domains();

  // Initialize SDIO peripheral
  // If no SDIO_Config structure is provided the default is used.
  // The default provides the parameters for initialization
  // using a very low clock speed (typically <= 400KHz).
  sdio_.init();
  sync_domains();

  SDIO_Error_Type result = begin_startup_procedure();
  if (result != SDIO_Error_Type::OK) {
    return result;
  }

  return card_init();
}

// Startup command procedure according to SDIO specification
auto CardDMA::begin_startup_procedure() -> SDIO_Error_Type {
  sdio_.set_power_mode(Power_Control::POWER_ON);
  sdio_.set_clock_enable(true);
  sync_domains();

  // CMD0 (GO_IDLE_STATE)
  if (send_command_and_check(Command_Index::CMD0, 0, Command_Response::RSP_NONE, Wait_Type::WT_NONE, [this]() {
    return this->get_command_sent_result();
  }) != SDIO_Error_Type::OK) {
    return SDIO_Error_Type::CMD0_FAILED;
  }

  // CMD8
  if (send_command_and_check(Command_Index::CMD8, Check_Pattern, Command_Response::RSP_SHORT, Wait_Type::WT_NONE, [this]() {
    return this->get_r7_result();
  }) != SDIO_Error_Type::OK) {
    // V1.0 card
    // CMD0 (GO_IDLE_STATE)
    interface_version_ = Interface_Version::INTERFACE_V1_1;
    if (send_command_and_check(Command_Index::CMD0, 0, Command_Response::RSP_NONE, Wait_Type::WT_NONE, [this]() {
      return this->get_command_sent_result();
    }) != SDIO_Error_Type::OK) {
      return SDIO_Error_Type::CMD0_FAILED;
    }
  } else {
    // V2.0 card
    // CMD55
    interface_version_ = Interface_Version::INTERFACE_V2_0;
    if (send_command_and_check(Command_Index::CMD55, 0, Command_Response::RSP_SHORT, Wait_Type::WT_NONE, [this, cmd = Command_Index::CMD55]() {
      return get_r1_result(cmd);
    }) != SDIO_Error_Type::OK) {
      return SDIO_Error_Type::CMD55_FAILED;
    }
  }

  if (send_command_and_check(Command_Index::CMD55, 0, Command_Response::RSP_SHORT, Wait_Type::WT_NONE, [this, cmd = Command_Index::CMD55]() {
    return get_r1_result(cmd);
  }) != SDIO_Error_Type::OK) {
    return SDIO_Error_Type::CMD55_FAILED;
  }

  return validate_voltage();
}

// Voltage validation
auto CardDMA::validate_voltage() -> SDIO_Error_Type {
  uint32_t response = 0U;
  uint32_t count = 0U;
  bool valid_voltage = false;

  while (count < Max_Voltage_Checks && valid_voltage == false) {
    if (send_command_and_check(Command_Index::CMD55, 0, Command_Response::RSP_SHORT,
           Wait_Type::WT_NONE, [this, cmd = Command_Index::CMD55]() {
      return get_r1_result(cmd);
    }) != SDIO_Error_Type::OK) {
      return SDIO_Error_Type::CMD55_FAILED;
    }

    if (send_command_and_check(Command_Index::ACMD41, Voltage_Window | SDCARD_HCS | Switch_1_8V_Capacity,
           Command_Response::RSP_SHORT, Wait_Type::WT_NONE, [this, cmd = Command_Index::INVALID, index = false, crc = true]() {
      return check_sdio_status(cmd, index, crc);
    }) != SDIO_Error_Type::OK) {
      return SDIO_Error_Type::ACMD41_FAILED;
    }
    response = sdio_.get_response(Response_Type::RESPONSE0);
    valid_voltage = ((response >> 31U) == 1U);
    count++;
  }

  if (count >= Max_Voltage_Checks) {
    return SDIO_Error_Type::INVALID_VOLTAGE;
  }

  card_type_ = (response & SDCARD_HCS) ? Card_Type::SDCARD_HIGH_CAPACITY : Card_Type::SDCARD_STANDARD_CAPACITY;

  #if ENABLED(MARLIN_DEV_MODE)
    if (card_type_ == Card_Type::SDCARD_HIGH_CAPACITY) {
      SERIAL_ECHOPGM("\n SDHC!");
    } else {
      SERIAL_ECHOPGM("\n SDSC!");
    }
  #endif

  return SDIO_Error_Type::OK;
}

// Shutdown
void CardDMA::begin_shutdown_procedure() {
  sdio_.set_power_mode(Power_Control::POWER_OFF);
}

// Initialize card
auto CardDMA::card_init() -> SDIO_Error_Type {
  if (sdio_.get_power_mode() == static_cast<uint32_t>(Power_Control::POWER_OFF)) {
    return SDIO_Error_Type::INVALID_OPERATION;
  }

  // Skip CID/RCA for IO cards
  if (card_type_ != Card_Type::SD_IO_CARD) {
    if (send_command_and_check(Command_Index::CMD2, 0U, Command_Response::RSP_LONG, Wait_Type::WT_NONE,
           [this, cmd = Command_Index::INVALID, index = false, crc = true]() {
      return check_sdio_status(cmd, index, crc);
    }) != SDIO_Error_Type::OK) {
      return SDIO_Error_Type::CMD2_FAILED;
    }
    // Store CID
    store_cid();

    // Get RCA
    uint16_t r6_rca;
    if (send_command_and_check(Command_Index::CMD3, 0U, Command_Response::RSP_SHORT, Wait_Type::WT_NONE,
           [this, cmd = Command_Index::CMD3, rca = &r6_rca]() {
      return get_r6_result(cmd, rca);
    }) != SDIO_Error_Type::OK) {
      return SDIO_Error_Type::CMD3_FAILED;
    }
    // Store RCA
    sdcard_rca_ = r6_rca;
    if (send_command_and_check(Command_Index::CMD9, static_cast<uint32_t>(sdcard_rca_ << RCA_Shift),
           Command_Response::RSP_LONG, Wait_Type::WT_NONE,
           [this, cmd = Command_Index::INVALID, index = false, crc = true]() {
      return check_sdio_status(cmd, index, crc);
    }) != SDIO_Error_Type::OK) {
      return SDIO_Error_Type::CMD9_FAILED;
    }
    // Store CSD
    store_csd();
  }

  Card_Info card_info;
  SDIO_Error_Type result = get_card_specific_data(&card_info);
  if (result != SDIO_Error_Type::OK) {
    return result;
  }

  if (select_deselect() != SDIO_Error_Type::OK) {
    return  SDIO_Error_Type::SELECT_DESELECT_FAILED;
  }

  return SDIO_Error_Type::OK;
}

auto CardDMA::store_cid() -> SDIO_Error_Type {
  // Store the CID register values
  sdcard_cid_[0] = sdio_.get_response(Response_Type::RESPONSE0);
  sdcard_cid_[1] = sdio_.get_response(Response_Type::RESPONSE1);
  sdcard_cid_[2] = sdio_.get_response(Response_Type::RESPONSE2);
  sdcard_cid_[3] = sdio_.get_response(Response_Type::RESPONSE3);

  return SDIO_Error_Type::OK;
}

auto CardDMA::store_csd() -> SDIO_Error_Type {
  // Store the CSD register values
  sdcard_csd_[0] = sdio_.get_response(Response_Type::RESPONSE0);
  sdcard_csd_[1] = sdio_.get_response(Response_Type::RESPONSE1);
  sdcard_csd_[2] = sdio_.get_response(Response_Type::RESPONSE2);
  sdcard_csd_[3] = sdio_.get_response(Response_Type::RESPONSE3);

  return SDIO_Error_Type::OK;
}

auto CardDMA::set_hardware_bus_width(Bus_Width width) -> SDIO_Error_Type {
  if (card_type_ == Card_Type::SD_MMC || width == Bus_Width::WIDTH_8BIT) {
    return SDIO_Error_Type::UNSUPPORTED_FUNCTION;
  }

  // Retrieve SCR
  SDIO_Error_Type result = get_scr(sdcard_rca_, sdcard_scr_);
  if (result != SDIO_Error_Type::OK) {
    return result;
  }

  // Check and set bus width
  // This function is only used to set a higher width than the default 1bit
  // so no 1bit configuration logic is required.
  if (width == Bus_Width::WIDTH_4BIT) {
    // Send CMD55 (APP_CMD)
    if (send_command_and_check(Command_Index::CMD55, static_cast<uint32_t>(sdcard_rca_ << RCA_Shift), Command_Response::RSP_SHORT,
           Wait_Type::WT_NONE, [this, cmd = Command_Index::CMD55]() {
      return get_r1_result(cmd);
    }) != SDIO_Error_Type::OK) {
      return SDIO_Error_Type::CMD55_FAILED;
    }

    // Send ACMD6 (SET_BUS_WIDTH)
    if (send_command_and_check(Command_Index::ACMD6, 2U, Command_Response::RSP_SHORT,
           Wait_Type::WT_NONE, [this, cmd = Command_Index::ACMD6]() {
      return get_r1_result(cmd);
    }) != SDIO_Error_Type::OK) {
      return SDIO_Error_Type::ACMD6_FAILED;
    }

    #if ENABLED(MARLIN_DEV_MODE)
      SERIAL_ECHOPGM("\n wide bus set!");
    #endif
    sdio_.set_bus_width(Bus_Width::WIDTH_4BIT);

    return SDIO_Error_Type::OK;
  }

  return SDIO_Error_Type::UNSUPPORTED_FUNCTION;
}

auto CardDMA::read(uint8_t* buf, uint32_t address, uint32_t count) -> SDIO_Error_Type {
  if (current_state_ == Operational_State::READY) {
    transfer_error_ = SDIO_Error_Type::OK;
    current_state_ = Operational_State::BUSY;
    is_rx_ = true;
    multiblock_ = (count > 1);

    sdio_.clear_data_state_machine(Transfer_Direction::CARD_TO_SDIO);

    // Enable the interrupts
    sdio_.set_interrupt_enable(Interrupt_Type::DTCRCERRIE, true);
    sdio_.set_interrupt_enable(Interrupt_Type::DTTMOUTIE, true);
    sdio_.set_interrupt_enable(Interrupt_Type::RXOREIE, true);
    sdio_.set_interrupt_enable(Interrupt_Type::DTENDIE, true);
    sdio_.set_interrupt_enable(Interrupt_Type::STBITEIE, true);

    total_bytes_ = BLOCK_SIZE * count;

    // Set DMA transfer parameters
    set_dma_parameters(buf, (total_bytes_ / 4U), false);
    sdio_.set_dma_enable(true);

    if (card_type_ != Card_Type::SDCARD_HIGH_CAPACITY) {
      address *= 512U;
    }

    // CMD16 set card block size
    if (send_command_and_check(Command_Index::CMD16, BLOCK_SIZE, Command_Response::RSP_SHORT,
           Wait_Type::WT_NONE, [this, cmd = Command_Index::CMD16]() {
      return get_r1_result(cmd);
    }) != SDIO_Error_Type::OK) {
      sdio_.clear_multiple_interrupt_flags(clear_common_flags);
      current_state_ = Operational_State::READY;
      return SDIO_Error_Type::CMD16_FAILED;
    }

    Block_Size block_size = get_data_block_size_index(BLOCK_SIZE);

    sdio_.set_data_state_machine_and_send(Data_Timeout, total_bytes_, block_size,
         Transfer_Mode::BLOCK, Transfer_Direction::CARD_TO_SDIO, true);

    // CMD17/CMD18 (READ_SINGLE_BLOCK/READ_MULTIPLE_BLOCKS) send read command
    Command_Index read_cmd = (count > 1U) ? Command_Index::CMD18 : Command_Index::CMD17;
    if (send_command_and_check(read_cmd, address,
         Command_Response::RSP_SHORT, Wait_Type::WT_NONE, [this, cmd = read_cmd]() {
      return get_r1_result(cmd);
    }) != SDIO_Error_Type::OK) {
      sdio_.clear_multiple_interrupt_flags(clear_common_flags);
      current_state_ = Operational_State::READY;
      return (count > 1U) ? SDIO_Error_Type::CMD18_FAILED : SDIO_Error_Type::CMD17_FAILED;
    }
    return SDIO_Error_Type::OK;
  } else {
    return SDIO_Error_Type::BUSY;
  }
}

auto CardDMA::write(uint8_t* buf, uint32_t address, uint32_t count) -> SDIO_Error_Type {
  // Enable the interrupts
  sdio_.set_interrupt_enable(Interrupt_Type::DTCRCERRIE, true);
  sdio_.set_interrupt_enable(Interrupt_Type::DTTMOUTIE, true);
  sdio_.set_interrupt_enable(Interrupt_Type::STBITEIE, true);
  sdio_.set_interrupt_enable(Interrupt_Type::TXUREIE, true);

  if (card_type_ != Card_Type::SDCARD_HIGH_CAPACITY) {
    address *= 512U;
  }

  // CMD16
  if (send_command_and_check(Command_Index::CMD16, BLOCK_SIZE, Command_Response::RSP_SHORT,
         Wait_Type::WT_NONE, [this, cmd = Command_Index::CMD16]() {
    return get_r1_result(cmd);
  }) != SDIO_Error_Type::OK) {
    sdio_.clear_multiple_interrupt_flags(clear_common_flags);
    return SDIO_Error_Type::CMD16_FAILED;
  }

  // CMD25/CMD24 (WRITE_MULTIPLE_BLOCK/WRITE_BLOCK) send write command
  Command_Index write_cmd = (count > 1U) ? Command_Index::CMD25 : Command_Index::CMD24;
  if (send_command_and_check(write_cmd, address, Command_Response::RSP_SHORT,
     Wait_Type::WT_NONE, [this, cmd = write_cmd]() {
    return get_r1_result(cmd);
  }) != SDIO_Error_Type::OK) {
    sdio_.clear_multiple_interrupt_flags(clear_common_flags);
    return (count > 1U) ? SDIO_Error_Type::CMD25_FAILED : SDIO_Error_Type::CMD24_FAILED;
  }

  total_bytes_ = BLOCK_SIZE * count;
  // Start DMA transfer
  set_dma_parameters(buf, (total_bytes_ / 4U), true);
  sdio_.set_dma_enable(true);

  sdio_.clear_data_state_machine(Transfer_Direction::SDIO_TO_CARD);
  Block_Size block_size = get_data_block_size_index(total_bytes_);

  sdio_.set_data_state_machine_and_send(Data_Timeout, total_bytes_, block_size,
     Transfer_Mode::BLOCK, Transfer_Direction::SDIO_TO_CARD, true);

  while (dma_.get_flag(dma::Status_Flags::FLAG_FTFIF) || dma_.get_flag(dma::Status_Flags::FLAG_ERRIF)) {
    // Wait for the IRQ handler to clear
  }

  return SDIO_Error_Type::OK;
}

auto CardDMA::erase(uint32_t address_start, uint32_t address_end) -> SDIO_Error_Type {
  SDIO_Error_Type result = SDIO_Error_Type::OK;

  // Card command classes CSD
  uint8_t temp_byte = static_cast<uint8_t>((sdcard_csd_[1] & (0xFFU << 24U)) >> 24U);
  uint16_t classes = static_cast<uint16_t>(temp_byte << 4U);
  temp_byte = static_cast<uint8_t>((sdcard_csd_[1] & (0xFFU << 16U)) >> 16U);
  classes |= static_cast<uint16_t>((temp_byte & 0xF0U) >> 4U);

  if ((classes & (1U << static_cast<uint32_t>(Card_Command_Class::ERASE))) == Clear) {
    return SDIO_Error_Type::UNSUPPORTED_FUNCTION;
  }

  uint32_t delay_time = 120000U / sdio_.get_clock_divider();

  if (sdio_.get_response(Response_Type::RESPONSE0) & Card_Locked) {
    return SDIO_Error_Type::LOCK_UNLOCK_FAILED;
  }

  // Size is fixed at 512 bytes for SDHC
  if (card_type_ != Card_Type::SDCARD_HIGH_CAPACITY) {
    address_start *= 512U;
    address_end *= 512U;
  }

  if ((card_type_ == Card_Type::SDCARD_STANDARD_CAPACITY) || (card_type_ == Card_Type::SDCARD_HIGH_CAPACITY)) {
    // CMD32 (ERASE_WR_BLK_START)
    if (send_command_and_check(Command_Index::CMD32, address_start, Command_Response::RSP_SHORT,
           Wait_Type::WT_NONE, [this, cmd = Command_Index::CMD32]() {
      return get_r1_result(cmd);
    }) != SDIO_Error_Type::OK) {
      return SDIO_Error_Type::CMD32_FAILED;
    }
    // CMD33 (ERASE_WR_BLK_END)
    if (send_command_and_check(Command_Index::CMD33, address_end, Command_Response::RSP_SHORT,
           Wait_Type::WT_NONE, [this, cmd = Command_Index::CMD33]() {
      return get_r1_result(cmd);
    }) != SDIO_Error_Type::OK) {
      return SDIO_Error_Type::CMD33_FAILED;
    }
  }

  // CMD38 (ERASE)
  if (send_command_and_check(Command_Index::CMD38, 0U, Command_Response::RSP_SHORT,
         Wait_Type::WT_NONE, [this, cmd = Command_Index::CMD38]() {
    return get_r1_result(cmd);
  }) != SDIO_Error_Type::OK) {
    return SDIO_Error_Type::CMD38_FAILED;
  }

  // Loop until the counter reaches the calculated time
  for (uint32_t count = 0U; count < delay_time; count++) {}

  Card_State card_state;
  result = get_card_state(&card_state);
  while ((result == SDIO_Error_Type::OK) && ((card_state == Card_State::PROGRAMMING) || (card_state == Card_State::RECEIVE_DATA))) {
    result = get_card_state(&card_state);
  }

  return result;
}

void CardDMA::handle_interrupts() {
  transfer_error_ = SDIO_Error_Type::OK;

  if (sdio_.get_interrupt_flag(Interrupt_Flags::FLAG_INTR_DTEND)) {
    sdio_.clear_interrupt_flag(Clear_Flags::FLAG_DTENDC);
    // Disable all interrupts
    disable_all_interrupts();
    sdio_.set_data_state_machine_enable(false);

    if (multiblock_ && !is_rx_) {
      transfer_error_ = stop_transfer();
      if (transfer_error_ != SDIO_Error_Type::OK) {
        return;
      }
    }

    if (!is_rx_) {
      sdio_.set_dma_enable(false);
      current_state_ = Operational_State::READY;
    }
  } else if (sdio_.get_interrupt_flag(Interrupt_Flags::FLAG_INTR_DTCRCERR) ||
             sdio_.get_interrupt_flag(Interrupt_Flags::FLAG_INTR_DTTMOUT) ||
             sdio_.get_interrupt_flag(Interrupt_Flags::FLAG_INTR_STBITE) ||
             sdio_.get_interrupt_flag(Interrupt_Flags::FLAG_INTR_TXURE) ||
             sdio_.get_interrupt_flag(Interrupt_Flags::FLAG_INTR_RXORE)) {

    if (sdio_.get_interrupt_flag(Interrupt_Flags::FLAG_INTR_DTCRCERR)) {
      transfer_error_ = SDIO_Error_Type::DATA_CRC_ERROR;
    }
    if (sdio_.get_interrupt_flag(Interrupt_Flags::FLAG_INTR_DTTMOUT)) {
      transfer_error_ = SDIO_Error_Type::DATA_TIMEOUT;
    }
    if (sdio_.get_interrupt_flag(Interrupt_Flags::FLAG_INTR_STBITE)) {
      transfer_error_ = SDIO_Error_Type::START_BIT_ERROR;
    }
    if (sdio_.get_interrupt_flag(Interrupt_Flags::FLAG_INTR_TXURE)) {
      transfer_error_ = SDIO_Error_Type::TX_FIFO_UNDERRUN;
    }
    if (sdio_.get_interrupt_flag(Interrupt_Flags::FLAG_INTR_RXORE)) {
      transfer_error_ = SDIO_Error_Type::RX_FIFO_OVERRUN;
    }
    sdio_.clear_multiple_interrupt_flags(clear_data_flags);
    sdio_.clear_interrupt_flag(Clear_Flags::FLAG_STBITEC);
    disable_all_interrupts();

    dma_.set_transfer_abandon();
  }
}

auto CardDMA::select_deselect() -> SDIO_Error_Type {
  // CMD7 (SELECT/DESELECT_CARD)
  if (send_command_and_check(Command_Index::CMD7, static_cast<uint32_t>(sdcard_rca_ << RCA_Shift),
         Command_Response::RSP_SHORT, Wait_Type::WT_NONE, [this, cmd = Command_Index::CMD7]() {
    return get_r1_result(cmd);
  }) != SDIO_Error_Type::OK) {
    return SDIO_Error_Type::CMD7_FAILED;
  }
  return SDIO_Error_Type::OK;
}

auto CardDMA::get_card_interface_status(uint32_t* status) -> SDIO_Error_Type {
  if (status == nullptr) return SDIO_Error_Type::INVALID_PARAMETER;

  // CMD13 (SEND_STATUS)
  if (send_command_and_check(Command_Index::CMD13, static_cast<uint32_t>(sdcard_rca_ << RCA_Shift),
         Command_Response::RSP_SHORT, Wait_Type::WT_NONE, [this, cmd = Command_Index::CMD13]() {
    return get_r1_result(cmd);
  }) != SDIO_Error_Type::OK) {
    return SDIO_Error_Type::CMD13_FAILED;
  }

  *status = sdio_.get_response(Response_Type::RESPONSE0);

  return SDIO_Error_Type::OK;
}

auto CardDMA::get_sdcard_status(uint32_t* status) -> SDIO_Error_Type {
  uint32_t count = 0U;

  // CMD16 (SET_BLOCKLEN)
  if (send_command_and_check(Command_Index::CMD16, 64U,
         Command_Response::RSP_SHORT, Wait_Type::WT_NONE, [this, cmd = Command_Index::CMD16]() {
    return get_r1_result(cmd);
  }) != SDIO_Error_Type::OK) {
    return SDIO_Error_Type::CMD16_FAILED;
  }

  // CMD55 (APP_CMD)
  if (send_command_and_check(Command_Index::CMD55, static_cast<uint32_t>(sdcard_rca_ << RCA_Shift),
         Command_Response::RSP_SHORT, Wait_Type::WT_NONE, [this, cmd = Command_Index::CMD55]() {
    return get_r1_result(cmd);
  }) != SDIO_Error_Type::OK) {
    return SDIO_Error_Type::CMD55_FAILED;
  }

  sdio_.set_data_state_machine_and_send(Data_Timeout, 64U, Block_Size::BYTES_64, Transfer_Mode::BLOCK, Transfer_Direction::CARD_TO_SDIO, true);

  // ACMD13 (SD_STATUS)
  if (send_command_and_check(Command_Index::ACMD13, 0U,
         Command_Response::RSP_SHORT, Wait_Type::WT_NONE, [this, cmd = Command_Index::ACMD13]() {
    return get_r1_result(cmd);
  }) != SDIO_Error_Type::OK) {
    return SDIO_Error_Type::ACMD13_FAILED;
  }

  while (!sdio_.check_scr_flags()) {
    if (sdio_.get_flag(Status_Flags::FLAG_RFH)) {
      for (count = 0U; count < FIFO_Half_Words; count++) {
        *(status + count) = sdio_.read_fifo_word();
      }
      status += FIFO_Half_Words;
    }

    //SDIO_Error_Type result = SDIO_Error_Type::OK;
    if (sdio_.get_flag(Status_Flags::FLAG_DTCRCERR)) {
      sdio_.clear_flag(Clear_Flags::FLAG_DTCRCERRC);
      return SDIO_Error_Type::DATA_CRC_ERROR;
    } else if (sdio_.get_flag(Status_Flags::FLAG_DTTMOUT)) {
      sdio_.clear_flag(Clear_Flags::FLAG_DTTMOUTC);
      return SDIO_Error_Type::DATA_TIMEOUT;
    } else if (sdio_.get_flag(Status_Flags::FLAG_RXORE)) {
      sdio_.clear_flag(Clear_Flags::FLAG_RXOREC);
      return SDIO_Error_Type::RX_FIFO_OVERRUN;
    } else if (sdio_.get_flag(Status_Flags::FLAG_STBITE)) {
      sdio_.clear_flag(Clear_Flags::FLAG_STBITEC);
      return SDIO_Error_Type::START_BIT_ERROR;
    }
    while (sdio_.get_flag(Status_Flags::FLAG_RXDTVAL)) {
      *status = sdio_.read_fifo_word();
      ++status;
    }

    sdio_.clear_multiple_interrupt_flags(clear_data_flags);
    status -= 16U;
    for (count = 0U; count < 16U; count++) {
      status[count] = ((status[count] & 0xFFU) << 24U) |
                      ((status[count] & (0xFFU << 8U)) << 8U) |
                      ((status[count] & (0xFFU << 16U)) >> 8U) |
                      ((status[count] & (0xFFU << 24U)) >> 24U);
    }
  }

  return SDIO_Error_Type::OK;
}

void CardDMA::check_dma_complete() {
  while ((dma_.get_flag(dma::Status_Flags::FLAG_FTFIF)) || (dma_.get_flag(dma::Status_Flags::FLAG_ERRIF))) {
  // Wait for the IRQ handler to clear
  }
}

auto CardDMA::stop_transfer() -> SDIO_Error_Type {
  // CMD12 (STOP_TRANSMISSION)
  if (send_command_and_check(Command_Index::CMD12, 0, Command_Response::RSP_SHORT, Wait_Type::WT_NONE, [this, cmd = Command_Index::CMD12]() {
    return get_r1_result(cmd);
  }) != SDIO_Error_Type::OK) {
    return SDIO_Error_Type::CMD12_FAILED;
  }
  return SDIO_Error_Type::OK;
}

auto CardDMA::get_transfer_state() -> Transfer_State {
  Transfer_State transfer_state = Transfer_State::IDLE;
  if (sdio_.get_flag(Status_Flags::FLAG_TXRUN) | sdio_.get_flag(Status_Flags::FLAG_RXRUN)) {
    transfer_state = Transfer_State::BUSY;
  }

  return transfer_state;
}

[[nodiscard]] auto CardDMA::get_card_capacity() const -> uint32_t {
  auto extract_bits = [](uint32_t value, uint8_t start_bit, uint8_t length) -> uint32_t {
    return (value >> start_bit) & ((1U << length) - 1U);
  };

  uint32_t capacity = 0U;
  uint32_t device_size = 0U;

  if (card_type_ == Card_Type::SDCARD_STANDARD_CAPACITY) {
    // Extract fields from CSD data using bit manipulation
    uint8_t device_size_high = static_cast<uint8_t>(extract_bits(sdcard_csd_[1], 8U, 2U)); // Bits [73:72]
    uint8_t device_size_mid = static_cast<uint8_t>(extract_bits(sdcard_csd_[1], 0U, 8U));  // Bits [71:64]
    uint8_t device_size_low = static_cast<uint8_t>(extract_bits(sdcard_csd_[2], 24U, 2U)); // Bits [63:62]

    device_size = static_cast<uint32_t>((device_size_high) << 10U) |
            static_cast<uint32_t>((device_size_mid) << 2U) |
            static_cast<uint32_t>((device_size_low));

    uint8_t device_size_multiplier_high = static_cast<uint8_t>(extract_bits(sdcard_csd_[2], 16U, 2U));    // Bits [49:48]
    uint8_t device_size_multiplier_low = static_cast<uint8_t>(extract_bits(sdcard_csd_[2], 8U, 1U));      // Bit [47]
    uint8_t device_size_multiplier = static_cast<uint8_t>((device_size_multiplier_high << 1U) | device_size_multiplier_low);
    uint8_t read_block_length = static_cast<uint8_t>(extract_bits(sdcard_csd_[1], 16U, 4U));              // Bits [83:80]

    // Capacity = (device_size + 1) * MULT * BLOCK_LEN
    uint32_t mult = static_cast<uint32_t>(1U << (device_size_multiplier + 2U)); // MULT = 2 ^ (C_SIZE_MULT + 2)
    uint32_t block_length = static_cast<uint32_t>(1U << read_block_length);     // BLOCK_LEN = 2 ^ READ_BL_LEN

    capacity = (device_size + 1U) * mult * block_length;
    capacity /= KILOBYTE; // Convert capacity to kilobytes

  } else if (card_type_ == Card_Type::SDCARD_HIGH_CAPACITY) {
    // High-capacity card calculation
    uint8_t device_size_high = static_cast<uint8_t>(extract_bits(sdcard_csd_[1], 0U, 6U));  // Bits [69:64]
    uint8_t device_size_mid = static_cast<uint8_t>(extract_bits(sdcard_csd_[2], 24U, 8U));  // Bits [55:48]
    uint8_t device_size_low = static_cast<uint8_t>(extract_bits(sdcard_csd_[2], 16U, 8U));  // Bits [47:40]

    device_size = static_cast<uint32_t>((device_size_high) << 16U) |
            static_cast<uint32_t>((device_size_mid) << 8U) |
            static_cast<uint32_t>(device_size_low);

    // Capacity in kilobytes
    capacity = (device_size + 1U) * BLOCK_SIZE;
  }

  return capacity;
}

auto CardDMA::get_card_specific_data(Card_Info* info) -> SDIO_Error_Type {
  if (info == nullptr) return SDIO_Error_Type::INVALID_PARAMETER;

  // Store basic card information
  info->type = card_type_;
  info->relative_address = sdcard_rca_;

  // Helper function to convert 32-bit registers to byte array
  auto convert_registers_to_bytes = [](const uint32_t* registers, uint8_t* bytes, size_t reg_count) {
    for (size_t i = 0U; i < reg_count; ++i) {
      for (size_t j = 0U; j < 4U; ++j) {
        bytes[i * 4U + j] = (registers[i] >> (24U - j * 8U)) & 0xFFU;
      }
    }
  };

  // Process CID data
  uint8_t cid_bytes[16];
  convert_registers_to_bytes(sdcard_cid_, cid_bytes, 4);

  info->cid = {
    .manufacture_id = cid_bytes[0],
    .oem_id = static_cast<uint16_t>(
            (cid_bytes[1] << 8U) |
             cid_bytes[2]
    ),
    .name0 = static_cast<uint32_t>(
            (cid_bytes[3] << 24U) |
            (cid_bytes[4] << 16U) |
            (cid_bytes[5] << 8U) |
             cid_bytes[6]
    ),
    .name1 = cid_bytes[7],
    .revision = cid_bytes[8],
    .serial_number = static_cast<uint32_t>(
            (cid_bytes[9] << 24U) |
            (cid_bytes[10] << 16U) |
            (cid_bytes[11] << 8U) |
             cid_bytes[12]
    ),
    .manufacture_date = static_cast<uint16_t>(
            ((cid_bytes[13] & 0x0FU) << 8U) |
              cid_bytes[14]
    ),
    .checksum = static_cast<uint8_t>(((cid_bytes[15] & 0xFEU) >> 1U))
  };

  // Process CSD data
  uint8_t csd_bytes[16];
  convert_registers_to_bytes(sdcard_csd_, csd_bytes, 4U);

  // Fill common CSD fields
  info->csd = {
    .transfer_speed = csd_bytes[3],
    .card_command_class = static_cast<uint16_t>((csd_bytes[4] << 4U) | ((csd_bytes[5] & 0xF0U) >> 4U)),
  };

  // Process card-type specific CSD fields and calculate capacity
  if (card_type_ == Card_Type::SDCARD_STANDARD_CAPACITY) {
    process_sdsc_specific_csd(info, csd_bytes);
  } else if (card_type_ == Card_Type::SDCARD_HIGH_CAPACITY) {
    process_sdhc_specific_csd(info, csd_bytes);
  }

  // Fill remaining common CSD fields
  process_common_csd_tail(info, csd_bytes);

  return SDIO_Error_Type::OK;
}

constexpr auto CardDMA::get_data_block_size_index(uint16_t size) -> Block_Size {
  if (size < 1 || size > 16384) return Block_Size::BYTES_1;

  // Check if size is a power of two
  if ((size & (size - 1)) != 0) return Block_Size::BYTES_1;

  // Count trailing zeros to find the index
  uint16_t index = 0;
  while ((size >>= 1) != 0) ++index;

  return static_cast<Block_Size>(index);
}

auto CardDMA::get_card_state(Card_State* card_state) -> SDIO_Error_Type {
  // CMD13 (SEND_STATUS)
  if (send_command_and_check(Command_Index::CMD13, static_cast<uint32_t>(sdcard_rca_ << RCA_Shift),
         Command_Response::RSP_SHORT, Wait_Type::WT_NONE, [this, cmd = Command_Index::CMD13]() {
    return get_r1_result(cmd);
  }) != SDIO_Error_Type::OK) {
    return SDIO_Error_Type::CMD13_FAILED;
  }

  uint32_t response = sdio_.get_response(Response_Type::RESPONSE0);
  *card_state = static_cast<Card_State>((response >> 9U) & CardStateMask);
  if ((response & All_R1_Error_Bits) == 0U) {
    return SDIO_Error_Type::OK;
  }

  if (response & All_R1_Error_Bits) {
    for (const auto& entry : errorTableR1) {
      if (TEST(response, entry.bit_position)) {
        return entry.errorType;
      }
    }
    return SDIO_Error_Type::ERROR;
  }

  return SDIO_Error_Type::OK;
}

auto CardDMA::get_command_sent_result() -> SDIO_Error_Type {
  constexpr uint32_t MAX_TIMEOUT = 0x00FFFFFFU;
  uint32_t timeout = MAX_TIMEOUT;

  // Wait for command sent flag or timeout
  while (!sdio_.get_flag(Status_Flags::FLAG_CMDSEND) && timeout) {
    --timeout;
  }

  // Check if timeout occurred
  if (timeout == 0U) {
    return SDIO_Error_Type::RESPONSE_TIMEOUT;
  }

  // Clear command flags and return success
  sdio_.clear_multiple_interrupt_flags(clear_command_flags);
  return SDIO_Error_Type::OK;
}

auto CardDMA::check_sdio_status(Command_Index index, bool check_index, bool ignore_crc) -> SDIO_Error_Type {
  // Wait until one of the relevant flags is set
  if (!sdio_.wait_cmd_flags()) {
    return SDIO_Error_Type::RESPONSE_TIMEOUT;
  }

  // Check the cmd received bit first, since noise can sometimes
  // cause the timeout bit to get erroneously set
  // If cmd was received we can safely ignore other checks
  if (sdio_.get_flag(Status_Flags::FLAG_CMDRECV)) {
    // If cmd was received, check the index
    // Responses that dont do an index check will send an invalid cmd index
    if (check_index && (index != Command_Index::INVALID)) {
      uint8_t received_idx = sdio_.get_command_index();
      if (received_idx != static_cast<uint8_t>(index)) {
        sdio_.clear_multiple_interrupt_flags(clear_command_flags);
        return SDIO_Error_Type::ILLEGAL_COMMAND;
      }
    }

    // Command received successfully
    sdio_.clear_multiple_interrupt_flags(clear_command_flags);
    return SDIO_Error_Type::OK;
  }

  // Check for timeout
  if (sdio_.get_flag(Status_Flags::FLAG_CMDTMOUT)) {
    sdio_.clear_flag(Clear_Flags::FLAG_CMDTMOUTC);
    return SDIO_Error_Type::RESPONSE_TIMEOUT;
  }

  // Check for CRC error if not ignored
  if (!ignore_crc && sdio_.get_flag(Status_Flags::FLAG_CCRCERR)) {
    sdio_.clear_flag(Clear_Flags::FLAG_CCRCERRC);
    return SDIO_Error_Type::COMMAND_CRC_ERROR;
  }

  // Final index check (redundant with the first check, but keeping for safety)
  // This code path should rarely be taken due to the earlier checks
  if (check_index && (index != Command_Index::INVALID)) {
    uint8_t received_idx = sdio_.get_command_index();
    if (received_idx != static_cast<uint8_t>(index)) {
      sdio_.clear_multiple_interrupt_flags(clear_command_flags);
      return SDIO_Error_Type::ILLEGAL_COMMAND;
    }
  }

  // Clear all flags and return success
  sdio_.clear_multiple_interrupt_flags(clear_command_flags);
  return SDIO_Error_Type::OK;
}

auto CardDMA::get_r1_result(Command_Index index) -> SDIO_Error_Type {
  SDIO_Error_Type result = check_sdio_status(index, true, false);
  if (result != SDIO_Error_Type::OK) return result;

  // Get the R1 response and check for errors
  uint32_t response = sdio_.get_response(Response_Type::RESPONSE0);

  if (response & All_R1_Error_Bits) {
    for (const auto& entry : errorTableR1) {
      if (TEST(response, entry.bit_position)) {
        return entry.errorType;
      }
    }
    return SDIO_Error_Type::ERROR;
  }

  return SDIO_Error_Type::OK;
}

auto CardDMA::get_r6_result(Command_Index index, uint16_t* rca) -> SDIO_Error_Type {
  SDIO_Error_Type result = check_sdio_status(index, true, false);
  if (result != SDIO_Error_Type::OK) return result;

  uint32_t response = sdio_.get_response(Response_Type::RESPONSE0);

  if (response & R6_Error_Bits) {
    for (const auto& entry : errorTableR6) {
      if (TEST(response, entry.bit_position)) {
        return entry.errorType;
      }
    }
    return SDIO_Error_Type::ERROR;
  }
  *rca = static_cast<uint16_t>(response >> 16U);

  return SDIO_Error_Type::OK;
}

auto CardDMA::get_scr(uint16_t rca, uint32_t* scr) -> SDIO_Error_Type {
  uint32_t temp_scr[2] = {0U, 0U};
  uint32_t index_scr = 0U;

  // CMD16 (SET_BLOCKLEN)
  if (send_command_and_check(Command_Index::CMD16, 8U, Command_Response::RSP_SHORT,
       Wait_Type::WT_NONE, [this, cmd = Command_Index::CMD16]() {
    return get_r1_result(cmd);
  }) != SDIO_Error_Type::OK) {
    return SDIO_Error_Type::CMD16_FAILED;
  }

  // CMD55 (APP_CMD)
  if (send_command_and_check(Command_Index::CMD55, static_cast<uint32_t>(sdcard_rca_ << RCA_Shift),
       Command_Response::RSP_SHORT, Wait_Type::WT_NONE, [this, cmd = Command_Index::CMD55]() {
    return get_r1_result(cmd);
  }) != SDIO_Error_Type::OK) {
    return SDIO_Error_Type::CMD55_FAILED;
  }

  // Set data parameters
  sdio_.set_data_state_machine_and_send(Data_Timeout, 8U, Block_Size::BYTES_8,
       Transfer_Mode::BLOCK, Transfer_Direction::CARD_TO_SDIO, true);

  // ACMD51 (SEND_SCR)
  if (send_command_and_check(Command_Index::ACMD51, 0U, Command_Response::RSP_SHORT,
       Wait_Type::WT_NONE, [this, cmd = Command_Index::ACMD51]() {
    return get_r1_result(cmd);
  }) != SDIO_Error_Type::OK) {
    return SDIO_Error_Type::ACMD51_FAILED;
  }

  // Store SCR
  while (!sdio_.check_scr_flags()) {
    if (sdio_.get_flag(Status_Flags::FLAG_RXDTVAL)) {
      temp_scr[index_scr++] = sdio_.read_fifo_word();
    }
  }

  // Check for errors
  if (sdio_.get_flag(Status_Flags::FLAG_DTTMOUT)) {
    sdio_.clear_flag(Clear_Flags::FLAG_DTTMOUTC);
    return SDIO_Error_Type::DATA_TIMEOUT;
  }
  else if (sdio_.get_flag(Status_Flags::FLAG_DTCRCERR)) {
    sdio_.clear_flag(Clear_Flags::FLAG_DTCRCERRC);
    return SDIO_Error_Type::DATA_CRC_ERROR;
  }
  else if (sdio_.get_flag(Status_Flags::FLAG_RXORE)) {
    sdio_.clear_flag(Clear_Flags::FLAG_RXOREC);
    return SDIO_Error_Type::RX_FIFO_OVERRUN;
  }

  sdio_.clear_multiple_interrupt_flags(clear_data_flags);

  constexpr uint32_t BYTE0_MASK = 0xFFU;
  constexpr uint32_t BYTE1_MASK = 0xFF00U;
  constexpr uint32_t BYTE2_MASK = 0xFF0000U;
  constexpr uint32_t BYTE3_MASK = 0xFF000000U;

  // Byte-swap the SCR values (convert from big-endian to little-endian)
  scr[0] = ((temp_scr[1] & BYTE0_MASK) << 24) |
           ((temp_scr[1] & BYTE1_MASK) << 8)  |
           ((temp_scr[1] & BYTE2_MASK) >> 8)  |
           ((temp_scr[1] & BYTE3_MASK) >> 24);

  scr[1] = ((temp_scr[0] & BYTE0_MASK) << 24) |
           ((temp_scr[0] & BYTE1_MASK) << 8)  |
           ((temp_scr[0] & BYTE2_MASK) >> 8)  |
           ((temp_scr[0] & BYTE3_MASK) >> 24);

  return SDIO_Error_Type::OK;
}

// DMA for rx/tx is always DMA1 channel 3
void CardDMA::set_dma_parameters(uint8_t* buf, uint32_t count, bool is_write) {
  constexpr uint32_t flag_bits = (1U << static_cast<uint32_t>(dma::INTF_Bits::GIF3));
  dma_.clear_flags(flag_bits);

  // Disable and reset DMA
  dma_.set_channel_enable(false);
  dma_.clear_channel();

  dma_.init(dma::DMA_Config{
    .count = count,
    .memory_address = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(buf)),
    .peripheral_address = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(sdio_.reg_address(SDIO_Regs::FIFO))),
    .peripheral_bit_width = dma::Bit_Width::WIDTH_32BIT,
    .memory_bit_width = dma::Bit_Width::WIDTH_32BIT,
    .peripheral_increase = dma::Increase_Mode::INCREASE_DISABLE,
    .memory_increase = dma::Increase_Mode::INCREASE_ENABLE,
    .channel_priority = dma::Channel_Priority::MEDIUM_PRIORITY,
    .direction = is_write ? dma::Transfer_Direction::M2P : dma::Transfer_Direction::P2M
  });

  dma_.set_memory_to_memory_enable(false);
  dma_.set_circulation_mode_enable(false);

  // Enable DMA interrupts for transfer complete and error
  dma_.set_interrupt_enable(dma::Interrupt_Type::INTR_FTFIE, true);
  dma_.set_interrupt_enable(dma::Interrupt_Type::INTR_ERRIE, true);

  // Start the DMA channel
  dma_.set_channel_enable(true);
}

auto CardDMA::wait_for_card_ready() -> SDIO_Error_Type {
  constexpr uint32_t MAX_TIMEOUT = 0x00FFFFFFU;
  uint32_t timeout = MAX_TIMEOUT;
  uint32_t response = sdio_.get_response(Response_Type::RESPONSE0);

  // Poll until card is ready for data or timeout occurs
  while (((response & static_cast<uint32_t>(R1_Status::READY_FOR_DATA)) == 0U) && timeout) {
    --timeout;

    // CMD13 (SEND_STATUS)
    if (send_command_and_check(Command_Index::CMD13, static_cast<uint32_t>(sdcard_rca_ << RCA_Shift),
         Command_Response::RSP_SHORT, Wait_Type::WT_NONE, [this, cmd = Command_Index::CMD13]() {
      return get_r1_result(cmd);
    }) != SDIO_Error_Type::OK) {
      return SDIO_Error_Type::CMD13_FAILED;
    }

    // Get updated response
    response = sdio_.get_response(Response_Type::RESPONSE0);
  }

  // Return error if timeout occurred, otherwise success
  return timeout ? SDIO_Error_Type::OK : SDIO_Error_Type::ERROR;
}

} // namespace sdio

sdio::CardDMA& CardDMA_I = sdio::CardDMA::get_instance();

#endif // ARDUINO_ARCH_MFL
