/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#ifdef ARDUINO_ARCH_MFL

#include "../../inc/MarlinConfig.h"

#if ENABLED(ONBOARD_SDIO)

#include <PinOpsMap.hpp>
#include <PinOps.hpp>
#include "SDIO_Card_DMA.hpp"
#include "sdio.h"

using namespace sdio;

inline constexpr uint32_t TARGET_SDIO_CLOCK = 6000000U;
inline constexpr uint32_t SDIO_BLOCK_SIZE = 512U;
inline constexpr uint8_t SDIO_READ_RETRIES = 3U;
inline constexpr uint32_t SD_TIMEOUT = 500;

Card_State cardState = Card_State::READY;

// configure the bus width mode
// Options are: Bus_Width::WIDTH_1BIT
//              Bus_Width::WIDTH_4BIT
//
bool SDIO_SetBusWidth(Bus_Width width) {
	return (CardDMA_I.set_hardware_bus_width(width) == SDIO_Error_Type::OK);
}

void mfl_sdio_init() {
	pinOpsPinout(SD_CMD_PinOps, static_cast<pin_size_t>(SDIO_CMD_PIN));
	pinOpsPinout(SD_CK_PinOps, static_cast<pin_size_t>(SDIO_CK_PIN));
	pinOpsPinout(SD_DATA0_PinOps, static_cast<pin_size_t>(SDIO_D0_PIN));
	pinOpsPinout(SD_DATA1_PinOps, static_cast<pin_size_t>(SDIO_D1_PIN));
	pinOpsPinout(SD_DATA2_PinOps, static_cast<pin_size_t>(SDIO_D2_PIN));
	pinOpsPinout(SD_DATA3_PinOps, static_cast<pin_size_t>(SDIO_D3_PIN));

	NVIC_EnableIRQ(DMA1_Channel3_4_IRQn);
	NVIC_EnableIRQ(SDIO_IRQn);
}

// Initializes the SDIO card
// @return `true` if the initialization is successful, `false` otherwise
bool SDIO_Init() {
	SDIO_Error_Type result = SDIO_Error_Type::OK;
	uint8_t retryCount = SDIO_READ_RETRIES;

	mfl_sdio_init();

	uint8_t retries = retryCount;
	for (;;) {
		hal.watchdog_refresh();
		result = CardDMA_I.init();
		if (result == SDIO_Error_Type::OK) break;
		if (!--retries) return false;
	}

    #if ENABLED(MARLIN_DEV_MODE)
		SERIAL_ECHOPGM("\nincreasing clock!");
	#endif
	CardDMA_I.set_desired_clock(TARGET_SDIO_CLOCK, false, false);

	retries = retryCount;
	for (;;) {
		hal.watchdog_refresh();
		if (SDIO_SetBusWidth(Bus_Width::WIDTH_4BIT)) break;
		if (!--retries) break;
	}

	CardDMA_I.set_desired_clock(TARGET_SDIO_CLOCK, true, true);

	// Fallback
	if (!retries) {
		mfl_sdio_init();
		retries = retryCount;
		for (;;) {
			hal.watchdog_refresh();
			result = CardDMA_I.init();
			if (result == SDIO_Error_Type::OK) break;
			if (!--retries) return false;
		}
		CardDMA_I.set_desired_clock(TARGET_SDIO_CLOCK, false, true);
	}

	return true;
}

// @brief Read or Write a block
// @details Read or Write a block with SDIO
// @param block The block index
// @param src The data buffer source for a write
// @param dst The data buffer destination for a read
// @return true on success
static bool SDIO_ReadWriteBlock_DMA(uint32_t block, const uint8_t* src, uint8_t* dst) {
	hal.watchdog_refresh();
	SDIO_Error_Type result = SDIO_Error_Type::OK;

	// Write
	if (src) {
		result = CardDMA_I.write(reinterpret_cast<uint8_t*>(const_cast<uint8_t*>(src)), block, 1);
	}
	// Read
	else {
		result = CardDMA_I.read(dst, block, 1);
	}
	if (result != SDIO_Error_Type::OK) {
		return false;
	}

	millis_t timeout = millis() + SD_TIMEOUT;
	while (CardDMA_I.get_state() != sdio::Operational_State::READY) {
		if (ELAPSED(millis(), timeout)) {
			return false;
		}
	}

	CardDMA_I.check_dma_complete();

	timeout = millis() + SD_TIMEOUT;
	do {
		result = CardDMA_I.get_card_state(&cardState);
		if (ELAPSED(millis(), timeout)) {
			return false;
		}
	} while (result == SDIO_Error_Type::OK && cardState != sdio::Card_State::TRANSFER);

	return true;
}

// @brief Read a block
// @details Read a block from media with SDIO
// @param block The block index
// @param dst The block buffer
// @return true on success
bool SDIO_ReadBlock(uint32_t block, uint8_t* dst) {
	// Check if the address is aligned to 4 bytes
	if (reinterpret_cast<uint32_t>(dst) & 0x03) {
		return false;
	}

	uint8_t retries = SDIO_READ_RETRIES;
	while (retries--) {
		if (SDIO_ReadWriteBlock_DMA(block, nullptr, dst)) {
			return true;
		} //else {
			//hal.watchdog_refresh();
		//}
	}

	return false;
}

// @brief Write a block
// @details Write a block to media with SDIO
// @param block The block index
// @param src The block data
// @return true on success
bool SDIO_WriteBlock(uint32_t block, const uint8_t* src) {
	// Check if the address is aligned to 4 bytes
	if (reinterpret_cast<uint32_t>(src) & 0x03) {
		return false;
	}

	uint8_t retries = SDIO_READ_RETRIES;
	while (retries--) {
		if (SDIO_ReadWriteBlock_DMA(block, src, nullptr)) {
			return true;
			delay(10);
		} //else {
			//hal.watchdog_refresh();
		//}
	}

	return false;
}

// Checks if the SDIO card is ready for operations.
// @return true if the SDIO card is ready, false otherwise
bool SDIO_IsReady() {
	return (CardDMA_I.get_state() == sdio::Operational_State::READY);
}

// @brief Get the size of the SDIO card.
// @return The size of the SDIO card in bytes.
uint32_t SDIO_GetCardSize() {
	return CardDMA_I.get_card_capacity();
}

// Handles the DMA1 interrupt for the specified channel.
// @param channelx The channel number for which the interrupt is being handled.
// @throws None
void DMA1_IRQHandler() {
	auto& dma_instance = CardDMA_I.get_dma_instance();
	bool is_receive = CardDMA_I.get_is_sdio_rx();

    // Check for Transfer Complete Interrupt
    if (dma_instance.get_interrupt_flag(dma::Interrupt_Flags::INTR_FLAG_FTFIF)) {
    	dma_instance.set_interrupt_enable(dma::Interrupt_Type::INTR_FTFIE, false);
    	dma_instance.set_interrupt_enable(dma::Interrupt_Type::INTR_ERRIE, false);
        dma_instance.clear_interrupt_flag(dma::Interrupt_Flags::INTR_FLAG_FTFIF);
        if (is_receive) {
        	CardDMA_I.set_sdio_dma_enable(false);
        	CardDMA_I.clear_sdio_data_flags();
			CardDMA_I.set_state(sdio::Operational_State::READY);
			// TODO:
			// Rx complete callback
        } else {
        	CardDMA_I.set_data_end_interrupt();
        }
        // Signal that transfer is complete
        CardDMA_I.set_transfer_end(true);
    }

    else if (dma_instance.get_interrupt_flag(dma::Interrupt_Flags::INTR_FLAG_ERRIF)) {
		dma_instance.set_interrupt_enable(dma::Interrupt_Type::INTR_HTFIE, false);
    	dma_instance.set_interrupt_enable(dma::Interrupt_Type::INTR_ERRIE, false);
    	dma_instance.set_interrupt_enable(dma::Interrupt_Type::INTR_FTFIE, false);
		// Clear all flags
		dma_instance.clear_interrupt_flag(dma::Interrupt_Flags::INTR_FLAG_GIF);
		// Signal that an error occurred
		CardDMA_I.set_transfer_error(SDIO_Error_Type::ERROR);
		CardDMA_I.set_state(sdio::Operational_State::READY);
	}
}


extern "C" {

	void SDIO_IRQHandler(void) {
		CardDMA_I.handle_interrupts();
	}

	void DMA1_Channel3_4_IRQHandler(void) {
		DMA1_IRQHandler();
	}

} // extern "C"


#endif // ONBOARD_SDIO
#endif // ARDUINO_ARCH_MFL
