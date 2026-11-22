/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * HAL/STM32/canbus.cpp - CAN bus for STM32 using the bxCAN (F1/F2/F4/F7) or FDCAN (G0/G4/H7) peripheral
 *
 * FDCAN is run in classic CAN 2.0 mode so it can share a bus with bxCAN boards and
 * with toolhead boards running Klipper, which also use classic CAN.
 *
 * The STM32 HAL CAN drivers are built only when HAL_CAN_MODULE_ENABLED / HAL_FDCAN_MODULE_ENABLED
 * are defined. ini/features.ini adds both flags to the build when CAN_BUS is enabled.
 */

#include "../platforms.h"

#ifdef HAL_STM32

#include "../../inc/MarlinConfig.h"

#if ENABLED(CAN_BUS)

#include "../shared/canbus.h"

#if defined(FDCAN1) && defined(HAL_FDCAN_MODULE_ENABLED)
  #define CAN_USE_FDCAN 1
#elif defined(CAN1) && defined(HAL_CAN_MODULE_ENABLED)
  #define CAN_USE_BXCAN 1
#else
  #error "CAN_BUS requires an STM32 with a bxCAN or FDCAN peripheral, built with HAL_CAN_MODULE_ENABLED or HAL_FDCAN_MODULE_ENABLED."
#endif

static_assert(!(CAN_RX_QUEUE_SIZE & (CAN_RX_QUEUE_SIZE - 1)), "CAN_RX_QUEUE_SIZE must be a power of 2.");

// Receive queue, filled by the receive interrupt (or poll) and emptied by CANBus::read()
static can_frame_t rx_queue[CAN_RX_QUEUE_SIZE];
static volatile uint8_t rx_head, rx_tail;
static can_stats_t can_stats;
static uint32_t can_bitrate;

static void queue_rx_frame(const can_frame_t &frame) {
  const uint8_t next = (rx_head + 1) & (CAN_RX_QUEUE_SIZE - 1);
  if (next == rx_tail) { can_stats.rx_overflow++; return; }
  rx_queue[rx_head] = frame;
  rx_head = next;
  can_stats.rx_frames++;
}

bool CANBus::read(can_frame_t &frame) {
  TERN_(CAN_POLL_RX, poll());
  if (rx_tail == rx_head) return false;
  frame = rx_queue[rx_tail];
  rx_tail = (rx_tail + 1) & (CAN_RX_QUEUE_SIZE - 1);
  return true;
}

uint32_t CANBus::bitrate() { return can_bitrate; }

/**
 * Find nominal bit timing for the given kernel clock and bitrate.
 * Use the smallest prescaler that gives a whole number of time quanta (8 to 25)
 * per bit, with the sample point near 87.5% as used by CANopen and Klipper.
 */
typedef struct { uint16_t prescaler; uint8_t seg1, seg2, sjw; } can_timing_t;

static bool compute_timing(const uint32_t clock, const uint32_t bitrate, const uint8_t max_seg1, const uint8_t max_seg2, can_timing_t &t) {
  if (!clock || !bitrate) return false;
  for (uint16_t brp = 1; brp <= 512; ++brp) {
    if (clock % (brp * bitrate)) continue;
    const uint32_t tq = clock / (brp * bitrate);
    if (tq > 25) continue;
    if (tq < 8) break;
    uint8_t seg2 = _MAX(2U, (tq + 4) / 8);          // ~12.5% after the sample point
    uint8_t seg1 = tq - 1 - seg2;                     // Sync segment is 1 quantum
    if (seg1 > max_seg1) { seg1 = max_seg1; seg2 = tq - 1 - seg1; }
    if (seg2 > max_seg2) continue;
    t.prescaler = brp; t.seg1 = seg1; t.seg2 = seg2; t.sjw = _MIN(seg2, 4U);
    return true;
  }
  return false;
}

// Configure CAN_RX_PIN / CAN_TX_PIN and return the peripheral that owns them, or nullptr
static void* can_pins_peripheral() {
  const PinName rx = digitalPinToPinName(CAN_RX_PIN), tx = digitalPinToPinName(CAN_TX_PIN);
  void * const prx = pinmap_peripheral(rx, PinMap_CAN_RD),
       * const ptx = pinmap_peripheral(tx, PinMap_CAN_TD);
  if (!prx || prx != ptx) return nullptr;
  pinmap_pinout(rx, PinMap_CAN_RD);
  pinmap_pinout(tx, PinMap_CAN_TD);
  return prx;
}

#if CAN_USE_FDCAN

  /**
   * FDCAN (STM32G0, G4, H7)
   */
  static FDCAN_HandleTypeDef hfdcan;

  static void fdcan_drain_fifo0() {
    FDCAN_RxHeaderTypeDef hdr;
    can_frame_t frame;
    while (HAL_FDCAN_GetRxFifoFillLevel(&hfdcan, FDCAN_RX_FIFO0)) {
      if (HAL_FDCAN_GetRxMessage(&hfdcan, FDCAN_RX_FIFO0, &hdr, frame.data) != HAL_OK) break;
      frame.id = hdr.IdType == FDCAN_EXTENDED_ID ? (hdr.Identifier | CAN_FRAME_EXT) : hdr.Identifier;
      frame.len = _MIN(uint32_t(hdr.DataLength >> 16), 8U); // FDCAN_DLC_BYTES_n is n << 16 for n <= 8
      queue_rx_frame(frame);
    }
  }

  #if defined(STM32G0xx)
    // On the G0 the FDCAN interrupts share vectors with TIM16/TIM17, which the STM32 core
    // claims for HardwareTimer, so frames are collected by polling instead.
    #define CAN_POLL_RX 1
  #else
    extern "C" void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *h, uint32_t) { if (h == &hfdcan) fdcan_drain_fifo0(); }
    extern "C" void FDCAN1_IT0_IRQHandler() { if (hfdcan.Instance == FDCAN1) HAL_FDCAN_IRQHandler(&hfdcan); }
    #ifdef FDCAN2
      extern "C" void FDCAN2_IT0_IRQHandler() { if (hfdcan.Instance == FDCAN2) HAL_FDCAN_IRQHandler(&hfdcan); }
    #endif
    #ifdef FDCAN3
      extern "C" void FDCAN3_IT0_IRQHandler() { if (hfdcan.Instance == FDCAN3) HAL_FDCAN_IRQHandler(&hfdcan); }
    #endif
  #endif

  void CANBus::poll() { TERN_(CAN_POLL_RX, fdcan_drain_fifo0()); }

  bool CANBus::begin(const uint32_t bitrate) {
    FDCAN_GlobalTypeDef * const inst = (FDCAN_GlobalTypeDef*)can_pins_peripheral();
    if (!inst) return false;

    __HAL_RCC_FDCAN_CLK_ENABLE();

    // Use the kernel clock as configured by the variant. The reset default is PCLK on G0/G4, HSE on H7.
    can_timing_t t;
    if (!compute_timing(HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_FDCAN), bitrate, 255, 128, t)) return false;

    hfdcan.Instance                  = inst;
    hfdcan.Init.FrameFormat          = FDCAN_FRAME_CLASSIC;
    hfdcan.Init.Mode                 = FDCAN_MODE_NORMAL;
    hfdcan.Init.AutoRetransmission   = ENABLE;
    hfdcan.Init.TransmitPause        = DISABLE;
    hfdcan.Init.ProtocolException    = DISABLE;
    hfdcan.Init.NominalPrescaler     = t.prescaler;
    hfdcan.Init.NominalSyncJumpWidth = t.sjw;
    hfdcan.Init.NominalTimeSeg1      = t.seg1;
    hfdcan.Init.NominalTimeSeg2      = t.seg2;
    hfdcan.Init.DataPrescaler        = 1;
    hfdcan.Init.DataSyncJumpWidth    = 1;
    hfdcan.Init.DataTimeSeg1         = 1;
    hfdcan.Init.DataTimeSeg2         = 1;
    hfdcan.Init.StdFiltersNbr        = 0;
    hfdcan.Init.ExtFiltersNbr        = 0;
    hfdcan.Init.TxFifoQueueMode      = FDCAN_TX_FIFO_OPERATION;
    #if defined(STM32H7xx)
      // The H7 message RAM (2560 words) is shared by the FDCAN instances. Give each its own part.
      hfdcan.Init.MessageRAMOffset    = 0;
      #ifdef FDCAN2
        if (inst == FDCAN2) hfdcan.Init.MessageRAMOffset = 850;
      #endif
      #ifdef FDCAN3
        if (inst == FDCAN3) hfdcan.Init.MessageRAMOffset = 1700;
      #endif
      hfdcan.Init.RxFifo0ElmtsNbr     = 32;
      hfdcan.Init.RxFifo0ElmtSize     = FDCAN_DATA_BYTES_8;
      hfdcan.Init.RxFifo1ElmtsNbr     = 0;
      hfdcan.Init.RxFifo1ElmtSize     = FDCAN_DATA_BYTES_8;
      hfdcan.Init.RxBuffersNbr        = 0;
      hfdcan.Init.RxBufferSize        = FDCAN_DATA_BYTES_8;
      hfdcan.Init.TxEventsNbr         = 0;
      hfdcan.Init.TxBuffersNbr        = 0;
      hfdcan.Init.TxFifoQueueElmtsNbr = 16;
      hfdcan.Init.TxElmtSize          = FDCAN_DATA_BYTES_8;
    #else
      hfdcan.Init.ClockDivider        = FDCAN_CLOCK_DIV1;
    #endif

    if (HAL_FDCAN_Init(&hfdcan) != HAL_OK) return false;

    // No ID filters. Accept every data frame into FIFO 0.
    HAL_FDCAN_ConfigGlobalFilter(&hfdcan, FDCAN_ACCEPT_IN_RX_FIFO0, FDCAN_ACCEPT_IN_RX_FIFO0, FDCAN_REJECT_REMOTE, FDCAN_REJECT_REMOTE);

    #if DISABLED(CAN_POLL_RX)
      if (HAL_FDCAN_ActivateNotification(&hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK) return false;
      IRQn_Type irq = FDCAN1_IT0_IRQn;
      #ifdef FDCAN2
        if (inst == FDCAN2) irq = FDCAN2_IT0_IRQn;
      #endif
      #ifdef FDCAN3
        if (inst == FDCAN3) irq = FDCAN3_IT0_IRQn;
      #endif
      HAL_NVIC_SetPriority(irq, CAN_IRQ_PRIO, 0);
      HAL_NVIC_EnableIRQ(irq);
    #endif

    if (HAL_FDCAN_Start(&hfdcan) != HAL_OK) return false;

    can_bitrate = bitrate;
    return true;
  }

  bool CANBus::send(const can_frame_t &frame) {
    if (!can_bitrate || frame.len > 8) return false;
    if (!HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan)) { can_stats.tx_full++; return false; }
    FDCAN_TxHeaderTypeDef hdr;
    const bool ext = TEST32(frame.id, 31);
    hdr.Identifier          = frame.id & (ext ? CAN_EXT_ID_MASK : CAN_STD_ID_MASK);
    hdr.IdType              = ext ? FDCAN_EXTENDED_ID : FDCAN_STANDARD_ID;
    hdr.TxFrameType         = FDCAN_DATA_FRAME;
    hdr.DataLength          = uint32_t(frame.len) << 16;  // FDCAN_DLC_BYTES_0 ... FDCAN_DLC_BYTES_8
    hdr.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    hdr.BitRateSwitch       = FDCAN_BRS_OFF;
    hdr.FDFormat            = FDCAN_CLASSIC_CAN;
    hdr.TxEventFifoControl  = FDCAN_NO_TX_EVENTS;
    hdr.MessageMarker       = 0;
    if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan, &hdr, const_cast<uint8_t*>(frame.data)) != HAL_OK) {
      can_stats.tx_full++;
      return false;
    }
    can_stats.tx_frames++;
    return true;
  }

  void CANBus::get_stats(can_stats_t &stats) {
    if (can_bitrate) {
      FDCAN_ErrorCountersTypeDef ec;
      FDCAN_ProtocolStatusTypeDef ps;
      HAL_FDCAN_GetErrorCounters(&hfdcan, &ec);
      HAL_FDCAN_GetProtocolStatus(&hfdcan, &ps);
      can_stats.tx_errors = ec.TxErrorCnt;
      can_stats.rx_errors = ec.RxErrorCnt;
      can_stats.bus_off = ps.BusOff;
      can_stats.error_passive = ps.ErrorPassive;
    }
    stats = can_stats;
  }

#elif CAN_USE_BXCAN

  /**
   * bxCAN (STM32F1, F2, F4, F7)
   */
  static CAN_HandleTypeDef hcan;

  static void bxcan_drain_fifo0() {
    CAN_RxHeaderTypeDef hdr;
    can_frame_t frame;
    while (HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0)) {
      if (HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &hdr, frame.data) != HAL_OK) break;
      if (hdr.RTR != CAN_RTR_DATA) continue;
      frame.id = hdr.IDE == CAN_ID_EXT ? (hdr.ExtId | CAN_FRAME_EXT) : hdr.StdId;
      frame.len = _MIN(hdr.DLC, 8U);
      queue_rx_frame(frame);
    }
  }

  extern "C" void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *h) { if (h == &hcan) bxcan_drain_fifo0(); }

  #ifdef STM32F1xx // The F1 CAN shares its vector (and packet RAM) with USB
    extern "C" void USB_LP_CAN1_RX0_IRQHandler() { if (hcan.Instance == CAN1) HAL_CAN_IRQHandler(&hcan); }
    #define CAN1_RX_IRQ USB_LP_CAN1_RX0_IRQn
  #else
    extern "C" void CAN1_RX0_IRQHandler() { if (hcan.Instance == CAN1) HAL_CAN_IRQHandler(&hcan); }
    #define CAN1_RX_IRQ CAN1_RX0_IRQn
  #endif
  #ifdef CAN2
    extern "C" void CAN2_RX0_IRQHandler() { if (hcan.Instance == CAN2) HAL_CAN_IRQHandler(&hcan); }
  #endif

  void CANBus::poll() {}

  bool CANBus::begin(const uint32_t bitrate) {
    CAN_TypeDef * const inst = (CAN_TypeDef*)can_pins_peripheral();
    if (!inst) return false;

    __HAL_RCC_CAN1_CLK_ENABLE();     // CAN2 needs CAN1 clocked too since CAN1 owns the filters
    #ifdef CAN2
      if (inst == CAN2) __HAL_RCC_CAN2_CLK_ENABLE();
    #endif

    can_timing_t t;
    if (!compute_timing(HAL_RCC_GetPCLK1Freq(), bitrate, 16, 8, t)) return false;

    hcan.Instance                 = inst;
    hcan.Init.Prescaler           = t.prescaler;
    hcan.Init.Mode                = CAN_MODE_NORMAL;
    hcan.Init.SyncJumpWidth       = uint32_t(t.sjw - 1) << CAN_BTR_SJW_Pos;
    hcan.Init.TimeSeg1            = uint32_t(t.seg1 - 1) << CAN_BTR_TS1_Pos;
    hcan.Init.TimeSeg2            = uint32_t(t.seg2 - 1) << CAN_BTR_TS2_Pos;
    hcan.Init.TimeTriggeredMode   = DISABLE;
    hcan.Init.AutoBusOff          = ENABLE;
    hcan.Init.AutoWakeUp          = DISABLE;
    hcan.Init.AutoRetransmission  = ENABLE;
    hcan.Init.ReceiveFifoLocked   = DISABLE;
    hcan.Init.TransmitFifoPriority = ENABLE;  // Send in the order queued
    if (HAL_CAN_Init(&hcan) != HAL_OK) return false;

    // One open filter: accept everything into FIFO 0
    CAN_FilterTypeDef filter = {};
    #ifdef CAN2
      filter.FilterBank         = inst == CAN2 ? 14 : 0;
    #else
      filter.FilterBank         = 0;
    #endif
    filter.SlaveStartFilterBank = 14;
    filter.FilterMode           = CAN_FILTERMODE_IDMASK;
    filter.FilterScale          = CAN_FILTERSCALE_32BIT;
    filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    filter.FilterActivation     = ENABLE;
    if (HAL_CAN_ConfigFilter(&hcan, &filter) != HAL_OK) return false;

    if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) return false;
    #ifdef CAN2
      const IRQn_Type irq = inst == CAN2 ? CAN2_RX0_IRQn : CAN1_RX_IRQ;
    #else
      constexpr IRQn_Type irq = CAN1_RX_IRQ;
    #endif
    HAL_NVIC_SetPriority(irq, CAN_IRQ_PRIO, 0);
    HAL_NVIC_EnableIRQ(irq);

    if (HAL_CAN_Start(&hcan) != HAL_OK) return false;

    can_bitrate = bitrate;
    return true;
  }

  bool CANBus::send(const can_frame_t &frame) {
    if (!can_bitrate || frame.len > 8) return false;
    if (!HAL_CAN_GetTxMailboxesFreeLevel(&hcan)) { can_stats.tx_full++; return false; }
    CAN_TxHeaderTypeDef hdr;
    const bool ext = TEST32(frame.id, 31);
    hdr.StdId = frame.id & CAN_STD_ID_MASK;
    hdr.ExtId = frame.id & CAN_EXT_ID_MASK;
    hdr.IDE   = ext ? CAN_ID_EXT : CAN_ID_STD;
    hdr.RTR   = CAN_RTR_DATA;
    hdr.DLC   = frame.len;
    hdr.TransmitGlobalTime = DISABLE;
    uint32_t mailbox;
    if (HAL_CAN_AddTxMessage(&hcan, &hdr, const_cast<uint8_t*>(frame.data), &mailbox) != HAL_OK) {
      can_stats.tx_full++;
      return false;
    }
    can_stats.tx_frames++;
    return true;
  }

  void CANBus::get_stats(can_stats_t &stats) {
    if (can_bitrate) {
      const uint32_t esr = hcan.Instance->ESR;
      can_stats.tx_errors = (esr & CAN_ESR_TEC) >> CAN_ESR_TEC_Pos;
      can_stats.rx_errors = (esr & CAN_ESR_REC) >> CAN_ESR_REC_Pos;
      can_stats.bus_off = TEST(esr, CAN_ESR_BOFF_Pos);
      can_stats.error_passive = TEST(esr, CAN_ESR_EPVF_Pos);
    }
    stats = can_stats;
  }

#endif // CAN_USE_BXCAN

#endif // CAN_BUS
#endif // HAL_STM32
