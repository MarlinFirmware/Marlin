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
 * USB CDC-ACM device for Atmel SAM3U (AT91SAM3U4E). See usb_cdc.h.
 *
 * ---------------------------------------------------------------------------
 * Design notes
 * ---------------------------------------------------------------------------
 *
 * FULL SPEED, DELIBERATELY. UDPHS is high-speed capable, and the original 4pi
 * firmware enumerated at 480Mbit/s with 512-byte bulk endpoints. This driver
 * forces full speed instead (UDPHS_TST_SPEED_CFG_FULL_SPEED), because:
 *
 *   - 12Mbit/s is already orders of magnitude more than Marlin needs; the
 *     bottleneck is G-code parsing, not the wire.
 *   - A high-speed device must also publish a device_qualifier and an
 *     other_speed_configuration, and keep two sets of endpoint sizes in step.
 *     Full speed needs one descriptor set and cannot get that wrong.
 *   - 64-byte banks instead of 512 leave nearly all of the 4KB DPRAM free.
 *
 * To switch to high speed later: drop the UDPHS_TST write in usb_cdc_init(),
 * change EP_BULK_SIZE to 512, and add the two extra descriptors.
 *
 * NO DMA. UDPHS has a DMA controller per endpoint, but at 64 bytes a packet
 * the copy is trivial and the FIFO path has far fewer ways to go wrong. Each
 * endpoint's bank is visible to the CPU as a 64KB window at
 * UDPHS_RAM_ADDR + 0x10000 * endpoint (datasheet 38.6.10).
 *
 * BUFFERING. Marlin writes a byte at a time, so bytes are accumulated into a
 * transmit ring and pushed out in whole packets by usb_cdc_task() (called from
 * the idle task) or when a packet fills. Reads are drained from the bulk OUT
 * endpoint into a receive ring inside the ISR.
 */

#ifdef __SAM3U4E__

#include "../../../inc/MarlinConfig.h"
#include "usb_cdc.h"
#include "../clock.h"

// ---------------------------------------------------------------------------
// Endpoints
// ---------------------------------------------------------------------------

#define EP_CTRL         0     // Control, 64 bytes, 1 bank
#define EP_BULK_OUT     1     // Host -> device, 2 banks
#define EP_BULK_IN      2     // Device -> host, 2 banks
#define EP_NOTIFY       3     // Interrupt IN; required by CDC ACM, never used

#define EP0_SIZE        64
#define EP_BULK_SIZE    64    // Full speed bulk maximum
#define EP_NOTIFY_SIZE  16

// Each endpoint's FIFO is mapped into its own 64KB logical window
#define EP_FIFO(ep) ((volatile uint8_t *)(UDPHS_RAM_ADDR + 0x10000UL * (ep)))

// UDPHS_IEN / UDPHS_INTSTA place the per-endpoint interrupt flags from bit 8
#define EPT_INT(ep) (1UL << (8 + (ep)))

// ---------------------------------------------------------------------------
// USB protocol constants
// ---------------------------------------------------------------------------

// bmRequestType
#define REQ_DIR_IN          0x80
#define REQ_TYPE_MASK       0x60
#define REQ_TYPE_STANDARD   0x00
#define REQ_TYPE_CLASS      0x20
#define REQ_RECIP_MASK      0x1F
#define REQ_RECIP_DEVICE    0x00
#define REQ_RECIP_INTERFACE 0x01
#define REQ_RECIP_ENDPOINT  0x02

// Standard requests
#define REQ_GET_STATUS        0
#define REQ_CLEAR_FEATURE     1
#define REQ_SET_FEATURE       3
#define REQ_SET_ADDRESS       5
#define REQ_GET_DESCRIPTOR    6
#define REQ_SET_DESCRIPTOR    7
#define REQ_GET_CONFIGURATION 8
#define REQ_SET_CONFIGURATION 9
#define REQ_GET_INTERFACE     10
#define REQ_SET_INTERFACE     11

// CDC class requests
#define CDC_SET_LINE_CODING         0x20
#define CDC_GET_LINE_CODING         0x21
#define CDC_SET_CONTROL_LINE_STATE  0x22

// Descriptor types
#define DESC_DEVICE         1
#define DESC_CONFIGURATION  2
#define DESC_STRING         3
#define DESC_INTERFACE      4
#define DESC_ENDPOINT       5
#define DESC_DEVICE_QUALIFIER 6

struct SetupPacket {
  uint8_t  bmRequestType;
  uint8_t  bRequest;
  uint16_t wValue;
  uint16_t wIndex;
  uint16_t wLength;
} __attribute__((packed));

// ---------------------------------------------------------------------------
// Descriptors
// ---------------------------------------------------------------------------

/**
 * VID/PID are carried over from the original 4pi firmware (Openmoko's vendor
 * ID with the 4pi's allocated product ID) so existing udev rules and host-side
 * tooling keep working. Note this is NOT the 0x03EB/0x6119 the board JSON
 * lists - that pair is the Atmel SAM-BA bootloader's, a different device.
 */
#define USB_VID 0x1D50
#define USB_PID 0x6019

static const uint8_t device_descriptor[] = {
  18,                     // bLength
  DESC_DEVICE,            // bDescriptorType
  0x00, 0x02,             // bcdUSB 2.00
  0x02,                   // bDeviceClass: CDC
  0x00,                   // bDeviceSubClass
  0x00,                   // bDeviceProtocol
  EP0_SIZE,               // bMaxPacketSize0
  USB_VID & 0xFF, USB_VID >> 8,
  USB_PID & 0xFF, USB_PID >> 8,
  0x00, 0x01,             // bcdDevice 1.00
  1,                      // iManufacturer
  2,                      // iProduct
  0,                      // iSerialNumber (none)
  1                       // bNumConfigurations
};

#define CONFIG_TOTAL_LEN 67

static const uint8_t config_descriptor[] = {
  // Configuration
  9, DESC_CONFIGURATION,
  CONFIG_TOTAL_LEN & 0xFF, CONFIG_TOTAL_LEN >> 8,
  2,                      // bNumInterfaces
  1,                      // bConfigurationValue
  0,                      // iConfiguration
  0x80,                   // bmAttributes: bus powered
  250,                    // bMaxPower: 500mA

  // Interface 0: CDC Communication
  9, DESC_INTERFACE,
  0,                      // bInterfaceNumber
  0,                      // bAlternateSetting
  1,                      // bNumEndpoints
  0x02,                   // bInterfaceClass: Communication
  0x02,                   // bInterfaceSubClass: Abstract Control Model
  0x01,                   // bInterfaceProtocol: AT commands
  0,                      // iInterface

  // CDC Header functional descriptor
  5, 0x24, 0x00, 0x10, 0x01,

  // CDC Call Management functional descriptor
  5, 0x24, 0x01,
  0x00,                   // bmCapabilities: no call management
  1,                      // bDataInterface

  // CDC Abstract Control Management functional descriptor
  4, 0x24, 0x02,
  0x02,                   // bmCapabilities: supports Set/Get Line Coding

  // CDC Union functional descriptor
  5, 0x24, 0x06,
  0,                      // bMasterInterface
  1,                      // bSlaveInterface

  // Notification endpoint (required by the class, never actually used here)
  7, DESC_ENDPOINT,
  0x80 | EP_NOTIFY,       // bEndpointAddress: IN
  0x03,                   // bmAttributes: interrupt
  EP_NOTIFY_SIZE, 0x00,   // wMaxPacketSize
  0xFF,                   // bInterval

  // Interface 1: CDC Data
  9, DESC_INTERFACE,
  1,                      // bInterfaceNumber
  0,                      // bAlternateSetting
  2,                      // bNumEndpoints
  0x0A,                   // bInterfaceClass: CDC Data
  0x00, 0x00, 0,

  // Bulk OUT
  7, DESC_ENDPOINT,
  EP_BULK_OUT,            // bEndpointAddress: OUT
  0x02,                   // bmAttributes: bulk
  EP_BULK_SIZE, 0x00,
  0,                      // bInterval

  // Bulk IN
  7, DESC_ENDPOINT,
  0x80 | EP_BULK_IN,      // bEndpointAddress: IN
  0x02,                   // bmAttributes: bulk
  EP_BULK_SIZE, 0x00,
  0                       // bInterval
};

static_assert(sizeof(config_descriptor) == CONFIG_TOTAL_LEN,
              "CONFIG_TOTAL_LEN must match the actual configuration descriptor size.");
static_assert(sizeof(device_descriptor) == 18, "Device descriptor must be 18 bytes.");

// String descriptors, UTF-16LE
static const uint8_t string_langid[] = { 4, DESC_STRING, 0x09, 0x04 };  // en-US

static const uint8_t string_manufacturer[] = {
  14, DESC_STRING, 'M',0, 'a',0, 'r',0, 'l',0, 'i',0, 'n',0
};

static const uint8_t string_product[] = {
  8, DESC_STRING, '4',0, 'p',0, 'i',0
};

// ---------------------------------------------------------------------------
// State
// ---------------------------------------------------------------------------

static volatile bool usb_configured = false;
static volatile bool usb_dtr = false;      // Host has opened the port

// SET_ADDRESS only takes effect once its status stage has completed, so the
// address is parked here until the ZLP has actually gone out.
static volatile uint8_t pending_address = 0;

// In-flight control IN transfer
static const uint8_t *ctrl_in_data = nullptr;
static uint16_t ctrl_in_remaining = 0;
static bool ctrl_in_send_zlp = false;

// Set while a control transfer with a host->device data stage is in progress.
// The status stage may only be acknowledged once that data has arrived.
static bool ctrl_out_pending = false;

// The host's notion of the line settings. Nothing here acts on them - there is
// no real UART behind this - but GET_LINE_CODING must return what was set.
static uint8_t line_coding[7] = { 0x00, 0xC2, 0x01, 0x00, 0x00, 0x00, 0x08 }; // 115200 8N1

// Ring buffers. Sized to hold several packets without stalling the bus.
#define USB_RX_SIZE 256
#define USB_TX_SIZE 256

static volatile uint8_t rx_buf[USB_RX_SIZE];
static volatile uint16_t rx_head = 0, rx_tail = 0;

static volatile uint8_t tx_buf[USB_TX_SIZE];
static volatile uint16_t tx_head = 0, tx_tail = 0;

// True while a bulk IN packet is in flight and the bank is not free
static volatile bool tx_busy = false;

static inline uint16_t rx_count() { return (rx_head - rx_tail) & (USB_RX_SIZE - 1); }
static inline uint16_t tx_count() { return (tx_head - tx_tail) & (USB_TX_SIZE - 1); }

// ---------------------------------------------------------------------------
// Low-level endpoint helpers
// ---------------------------------------------------------------------------

// EPT_SIZE field: 0 = 8 bytes, and each step doubles
static uint32_t ept_size_field(const uint16_t size) {
  uint32_t f = 0;
  for (uint16_t s = 8; s < size && f < 7; s <<= 1) f++;
  return f;
}

/**
 * Configure one endpoint. Types are the USB values (0 control, 2 bulk,
 * 3 interrupt); dir_in is ignored for control endpoints.
 */
static void ep_configure(const uint8_t ep, const uint8_t type, const bool dir_in,
                         const uint16_t size, const uint8_t banks) {

  UDPHS->UDPHS_EPT[ep].UDPHS_EPTCTLDIS = UDPHS_EPTCTLDIS_EPT_DISABL;

  UDPHS->UDPHS_EPT[ep].UDPHS_EPTCLRSTA = UDPHS_EPTCLRSTA_TOGGLESQ | UDPHS_EPTCLRSTA_FRCESTALL;
  UDPHS->UDPHS_EPTRST = 1UL << ep;

  UDPHS->UDPHS_EPT[ep].UDPHS_EPTCFG = UDPHS_EPTCFG_EPT_SIZE(ept_size_field(size))
                                    | (dir_in ? UDPHS_EPTCFG_EPT_DIR : 0)
                                    | UDPHS_EPTCFG_EPT_TYPE(type)
                                    | UDPHS_EPTCFG_BK_NUMBER(banks);

  // EPT_MAPD reads back set only if the DPRAM allocation succeeded. If it
  // doesn't, the endpoint table asks for more than the 4KB of DPRAM and the
  // configuration is simply wrong - give up rather than half-enumerate.
  if (!(UDPHS->UDPHS_EPT[ep].UDPHS_EPTCFG & UDPHS_EPTCFG_EPT_MAPD)) return;

  if (type == 0) {  // Control: interrupt on SETUP and OUT
    UDPHS->UDPHS_EPT[ep].UDPHS_EPTCTLENB = UDPHS_EPTCTLENB_RXRDY_TXKL
                                         | UDPHS_EPTCTLENB_RX_SETUP
                                         | UDPHS_EPTCTLENB_EPT_ENABL;
    UDPHS->UDPHS_IEN |= EPT_INT(ep);
  }
  else if (!dir_in) {  // Bulk OUT: interrupt when a packet lands
    UDPHS->UDPHS_EPT[ep].UDPHS_EPTCTLENB = UDPHS_EPTCTLENB_RXRDY_TXKL
                                         | UDPHS_EPTCTLENB_EPT_ENABL;
    UDPHS->UDPHS_IEN |= EPT_INT(ep);
  }
  else {  // IN endpoints are driven from the transmit path
    UDPHS->UDPHS_EPT[ep].UDPHS_EPTCTLENB = UDPHS_EPTCTLENB_EPT_ENABL;
    UDPHS->UDPHS_IEN |= EPT_INT(ep);
  }
}

static void ep_stall(const uint8_t ep) {
  UDPHS->UDPHS_EPT[ep].UDPHS_EPTSETSTA = UDPHS_EPTSETSTA_FRCESTALL;
}

// ---------------------------------------------------------------------------
// Control endpoint
// ---------------------------------------------------------------------------

// Push the next chunk of an in-flight control IN transfer
static void ctrl_in_continue() {
  volatile uint8_t * const fifo = EP_FIFO(EP_CTRL);

  uint16_t n = ctrl_in_remaining;
  if (n > EP0_SIZE) n = EP0_SIZE;

  for (uint16_t i = 0; i < n; i++) fifo[i] = ctrl_in_data[i];

  ctrl_in_data += n;
  ctrl_in_remaining -= n;

  // A transfer that ends exactly on a packet boundary but is shorter than the
  // host asked for needs a zero-length packet to mark the end.
  if (!ctrl_in_remaining && n < EP0_SIZE) ctrl_in_send_zlp = false;

  UDPHS->UDPHS_EPT[EP_CTRL].UDPHS_EPTSETSTA = UDPHS_EPTSETSTA_TXRDY;
  UDPHS->UDPHS_EPT[EP_CTRL].UDPHS_EPTCTLENB = UDPHS_EPTCTLENB_TXRDY;
}

// Begin a control IN transfer of at most wLength bytes
static void ctrl_in(const void * const data, uint16_t len, const uint16_t wLength) {
  if (len > wLength) len = wLength;
  ctrl_in_data = (const uint8_t *)data;
  ctrl_in_remaining = len;
  ctrl_in_send_zlp = (len < wLength) && (len % EP0_SIZE) == 0 && len != 0;
  ctrl_in_continue();
}

// Acknowledge a control transfer that has no data stage
static void ctrl_status_ack() {
  ctrl_in_data = nullptr;
  ctrl_in_remaining = 0;
  ctrl_in_send_zlp = false;
  UDPHS->UDPHS_EPT[EP_CTRL].UDPHS_EPTSETSTA = UDPHS_EPTSETSTA_TXRDY;
  UDPHS->UDPHS_EPT[EP_CTRL].UDPHS_EPTCTLENB = UDPHS_EPTCTLENB_TXRDY;
}

static void set_configuration(const uint8_t cfg) {
  usb_configured = (cfg != 0);
  if (!usb_configured) return;

  ep_configure(EP_BULK_OUT, 2, false, EP_BULK_SIZE, 2);
  ep_configure(EP_BULK_IN,  2, true,  EP_BULK_SIZE, 2);
  ep_configure(EP_NOTIFY,   3, true,  EP_NOTIFY_SIZE, 1);

  tx_busy = false;
}

static void handle_standard_request(const SetupPacket &s) {
  switch (s.bRequest) {

    case REQ_GET_DESCRIPTOR:
      switch (s.wValue >> 8) {
        case DESC_DEVICE:
          ctrl_in(device_descriptor, sizeof(device_descriptor), s.wLength);
          return;
        case DESC_CONFIGURATION:
          ctrl_in(config_descriptor, sizeof(config_descriptor), s.wLength);
          return;
        case DESC_STRING:
          switch (s.wValue & 0xFF) {
            case 0: ctrl_in(string_langid, sizeof(string_langid), s.wLength); return;
            case 1: ctrl_in(string_manufacturer, sizeof(string_manufacturer), s.wLength); return;
            case 2: ctrl_in(string_product, sizeof(string_product), s.wLength); return;
          }
          break;
        // A full-speed-only device must not answer this one
        case DESC_DEVICE_QUALIFIER:
        default:
          break;
      }
      ep_stall(EP_CTRL);
      return;

    case REQ_SET_ADDRESS:
      // Park it; the address may only take effect after the status stage
      pending_address = s.wValue & 0x7F;
      ctrl_status_ack();
      return;

    case REQ_SET_CONFIGURATION:
      set_configuration(s.wValue & 0xFF);
      ctrl_status_ack();
      return;

    case REQ_GET_CONFIGURATION: {
      static uint8_t cfg;
      cfg = usb_configured ? 1 : 0;
      ctrl_in(&cfg, 1, s.wLength);
      return;
    }

    case REQ_GET_STATUS: {
      static uint8_t status[2];
      status[0] = status[1] = 0;
      ctrl_in(status, 2, s.wLength);
      return;
    }

    case REQ_CLEAR_FEATURE:
    case REQ_SET_FEATURE:
      // No remote wakeup and no halt support worth honouring here
      ctrl_status_ack();
      return;

    case REQ_GET_INTERFACE: {
      static uint8_t alt = 0;
      ctrl_in(&alt, 1, s.wLength);
      return;
    }

    case REQ_SET_INTERFACE:
      ctrl_status_ack();
      return;

    default:
      ep_stall(EP_CTRL);
      return;
  }
}

static void handle_class_request(const SetupPacket &s) {
  switch (s.bRequest) {

    case CDC_GET_LINE_CODING:
      ctrl_in(line_coding, sizeof(line_coding), s.wLength);
      return;

    case CDC_SET_LINE_CODING:
      // A 7-byte OUT data stage follows. Acknowledging now would put the
      // status stage ahead of the data, so wait for it to land (see
      // ctrl_out_pending in ep0_handler) and record it for GET_LINE_CODING.
      ctrl_out_pending = true;
      return;

    case CDC_SET_CONTROL_LINE_STATE:
      // bit 0 = DTR. Hosts drop DTR when the port is closed, which is how we
      // know to stop queueing output.
      usb_dtr = (s.wValue & 0x01);
      ctrl_status_ack();
      return;

    default:
      ep_stall(EP_CTRL);
      return;
  }
}

static void handle_setup() {
  /**
   * The 8 SETUP bytes sit at the start of EP0's bank and are read linearly.
   *
   * Worth knowing if enumeration ever misbehaves: the old at91lib driver in
   * the 4pi reference firmware instead reads the same 32-bit FIFO location
   * twice, which would only produce the right result if the hardware popped
   * on read. The datasheet (38.6.9.1, and 38.6.10 on the DPR being addressed
   * as a linear buffer) and Atmel's later ASF driver - which does exactly the
   * memcpy below - both say linear, so that is what this does.
   */
  SetupPacket s;
  const volatile uint8_t * const fifo = EP_FIFO(EP_CTRL);
  uint8_t raw[8];
  for (uint8_t i = 0; i < 8; i++) raw[i] = fifo[i];
  memcpy(&s, raw, sizeof(s));

  UDPHS->UDPHS_EPT[EP_CTRL].UDPHS_EPTCLRSTA = UDPHS_EPTCLRSTA_RX_SETUP;

  ctrl_in_data = nullptr;
  ctrl_in_remaining = 0;
  ctrl_out_pending = false;

  switch (s.bmRequestType & REQ_TYPE_MASK) {
    case REQ_TYPE_STANDARD: handle_standard_request(s); break;
    case REQ_TYPE_CLASS:    handle_class_request(s);    break;
    default:                ep_stall(EP_CTRL);          break;
  }
}

static void ep0_handler() {
  const uint32_t status = UDPHS->UDPHS_EPT[EP_CTRL].UDPHS_EPTSTA;

  if (status & UDPHS_EPTSTA_RX_SETUP) { handle_setup(); return; }

  // An IN packet went out
  if ((UDPHS->UDPHS_EPT[EP_CTRL].UDPHS_EPTCTL & UDPHS_EPTCTL_TXRDY)
      && !(status & UDPHS_EPTSTA_TXRDY)) {

    if (ctrl_in_remaining || ctrl_in_send_zlp) {
      if (!ctrl_in_remaining) ctrl_in_send_zlp = false;
      ctrl_in_continue();
    }
    else {
      UDPHS->UDPHS_EPT[EP_CTRL].UDPHS_EPTCTLDIS = UDPHS_EPTCTLDIS_TXRDY;

      // The status stage of SET_ADDRESS has now completed, so the new address
      // may finally be programmed (USB 2.0 section 9.4.6).
      if (pending_address) {
        UDPHS->UDPHS_CTRL = (UDPHS->UDPHS_CTRL & ~uint32_t(UDPHS_CTRL_DEV_ADDR_Msk))
                          | UDPHS_CTRL_DEV_ADDR(pending_address) | UDPHS_CTRL_FADDR_EN;
        pending_address = 0;
      }
    }
    return;
  }

  // OUT data, or the status stage of a control read
  if (status & UDPHS_EPTSTA_RXRDY_TXKL) {

    if (ctrl_out_pending) {
      // The data stage of a control write. The only one we accept is
      // SET_LINE_CODING, so stash it for GET_LINE_CODING to hand back.
      const uint16_t count = (status & UDPHS_EPTSTA_BYTE_COUNT_Msk) >> UDPHS_EPTSTA_BYTE_COUNT_Pos;
      const volatile uint8_t * const fifo = EP_FIFO(EP_CTRL);
      for (uint16_t i = 0; i < count && i < sizeof(line_coding); i++)
        line_coding[i] = fifo[i];

      ctrl_out_pending = false;
      UDPHS->UDPHS_EPT[EP_CTRL].UDPHS_EPTCLRSTA = UDPHS_EPTCLRSTA_RXRDY_TXKL;
      ctrl_status_ack();
      return;
    }

    UDPHS->UDPHS_EPT[EP_CTRL].UDPHS_EPTCLRSTA = UDPHS_EPTCLRSTA_RXRDY_TXKL;
  }
}

// ---------------------------------------------------------------------------
// Bulk endpoints
// ---------------------------------------------------------------------------

// Drain a received packet into the receive ring
static void bulk_out_handler() {
  const uint32_t status = UDPHS->UDPHS_EPT[EP_BULK_OUT].UDPHS_EPTSTA;
  if (!(status & UDPHS_EPTSTA_RXRDY_TXKL)) return;

  const uint16_t count = (status & UDPHS_EPTSTA_BYTE_COUNT_Msk) >> UDPHS_EPTSTA_BYTE_COUNT_Pos;
  const volatile uint8_t * const fifo = EP_FIFO(EP_BULK_OUT);

  for (uint16_t i = 0; i < count; i++) {
    const uint16_t next = (rx_head + 1) & (USB_RX_SIZE - 1);
    if (next == rx_tail) break;      // Buffer full - drop the rest
    rx_buf[rx_head] = fifo[i];
    rx_head = next;
  }

  UDPHS->UDPHS_EPT[EP_BULK_OUT].UDPHS_EPTCLRSTA = UDPHS_EPTCLRSTA_RXRDY_TXKL;
}

/**
 * Move up to one packet from the transmit ring onto the bus.
 *
 * This is reachable from two places - the bulk IN completion interrupt and the
 * writer/idle task - so the caller must ensure only one of them is in here at
 * a time or two contexts could both decide the bank is free and fill it twice.
 * Thread-context callers go through bulk_in_start(), which masks the UDPHS
 * interrupt; the ISR calls this directly since it already has exclusion.
 */
static void bulk_in_start_locked() {
  if (tx_busy || !usb_configured) return;

  const uint16_t pending = tx_count();
  if (!pending) return;

  uint16_t n = pending;
  if (n > EP_BULK_SIZE) n = EP_BULK_SIZE;

  volatile uint8_t * const fifo = EP_FIFO(EP_BULK_IN);
  for (uint16_t i = 0; i < n; i++) {
    fifo[i] = tx_buf[tx_tail];
    tx_tail = (tx_tail + 1) & (USB_TX_SIZE - 1);
  }

  tx_busy = true;
  UDPHS->UDPHS_EPT[EP_BULK_IN].UDPHS_EPTSETSTA = UDPHS_EPTSETSTA_TXRDY;
  UDPHS->UDPHS_EPT[EP_BULK_IN].UDPHS_EPTCTLENB = UDPHS_EPTCTLENB_TX_COMPLT;
}

// Thread-context entry point: keep the USB ISR out while we touch the bank.
// Only the UDPHS line is masked, so the stepper and temperature timers are
// unaffected.
static void bulk_in_start() {
  NVIC_DisableIRQ(UDPHS_IRQn);
  __DSB();
  __ISB();
  bulk_in_start_locked();
  NVIC_EnableIRQ(UDPHS_IRQn);
}

static void bulk_in_handler() {
  const uint32_t status = UDPHS->UDPHS_EPT[EP_BULK_IN].UDPHS_EPTSTA;
  if (!(status & UDPHS_EPTSTA_TX_COMPLT)) return;

  UDPHS->UDPHS_EPT[EP_BULK_IN].UDPHS_EPTCLRSTA = UDPHS_EPTCLRSTA_TX_COMPLT;
  tx_busy = false;

  if (tx_count())
    bulk_in_start_locked();
  else
    UDPHS->UDPHS_EPT[EP_BULK_IN].UDPHS_EPTCTLDIS = UDPHS_EPTCTLDIS_TX_COMPLT;
}

// ---------------------------------------------------------------------------
// Bus events
// ---------------------------------------------------------------------------

static void handle_reset() {
  usb_configured = false;
  usb_dtr = false;
  tx_busy = false;
  pending_address = 0;
  rx_head = rx_tail = 0;
  tx_head = tx_tail = 0;

  // Only EP0 survives a reset; the rest are reconfigured on SET_CONFIGURATION
  ep_configure(EP_CTRL, 0, false, EP0_SIZE, 1);

  UDPHS->UDPHS_CLRINT = UDPHS_CLRINT_ENDRESET;
}

extern "C" void UDPHS_Handler() {
  uint32_t status = UDPHS->UDPHS_INTSTA & UDPHS->UDPHS_IEN;

  while (status) {

    if (status & UDPHS_INTSTA_ENDRESET) {
      handle_reset();
    }
    else if (status & UDPHS_INTSTA_DET_SUSPD) {
      UDPHS->UDPHS_CLRINT = UDPHS_CLRINT_DET_SUSPD;
    }
    else if (status & (UDPHS_INTSTA_WAKE_UP | UDPHS_INTSTA_ENDOFRSM)) {
      UDPHS->UDPHS_CLRINT = UDPHS_CLRINT_WAKE_UP | UDPHS_CLRINT_ENDOFRSM;
    }
    else if (status & UDPHS_INTSTA_INT_SOF) {
      UDPHS->UDPHS_CLRINT = UDPHS_CLRINT_INT_SOF;
    }
    else {
      if (status & EPT_INT(EP_CTRL))     ep0_handler();
      if (status & EPT_INT(EP_BULK_OUT)) bulk_out_handler();
      if (status & EPT_INT(EP_BULK_IN))  bulk_in_handler();
      if (status & EPT_INT(EP_NOTIFY))
        UDPHS->UDPHS_EPT[EP_NOTIFY].UDPHS_EPTCLRSTA = UDPHS_EPTCLRSTA_TX_COMPLT;
    }

    status = UDPHS->UDPHS_INTSTA & UDPHS->UDPHS_IEN;
  }
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

void usb_cdc_init() {

  // The UDPHS needs its peripheral clock and the dedicated 480MHz UTMI PLL
  // (datasheet 38.5.1). The UTMI PLL is independent of PLLA, so the 96MHz
  // master clock choice does not constrain it.
  pmc_enable_periph_clk(ID_UDPHS);

  PMC->CKGR_UCKR |= CKGR_UCKR_UPLLCOUNT(0xF) | CKGR_UCKR_UPLLEN;
  uint32_t timeout = 0xFFFFF;
  while (!(PMC->PMC_SR & PMC_SR_LOCKU) && --timeout) { }

  // Detach while configuring, and turn off the pull-downs (device mode)
  UDPHS->UDPHS_CTRL = UDPHS_CTRL_DETACH | UDPHS_CTRL_PULLD_DIS;

  // Enable the controller, then pin the link to full speed. See the design
  // note at the top of this file.
  UDPHS->UDPHS_CTRL |= UDPHS_CTRL_EN_UDPHS;
  UDPHS->UDPHS_TST = (UDPHS->UDPHS_TST & ~uint32_t(UDPHS_TST_SPEED_CFG_Msk))
                   | UDPHS_TST_SPEED_CFG_FULL_SPEED;

  // Quiesce every endpoint before use
  for (uint8_t ep = 0; ep < 7; ep++) {
    UDPHS->UDPHS_EPT[ep].UDPHS_EPTCTLDIS = UDPHS_EPTCTLDIS_EPT_DISABL;
    UDPHS->UDPHS_EPT[ep].UDPHS_EPTCFG = 0;
  }

  ep_configure(EP_CTRL, 0, false, EP0_SIZE, 1);

  UDPHS->UDPHS_CLRINT = UDPHS_CLRINT_ENDRESET | UDPHS_CLRINT_DET_SUSPD
                      | UDPHS_CLRINT_WAKE_UP | UDPHS_CLRINT_ENDOFRSM
                      | UDPHS_CLRINT_INT_SOF;
  UDPHS->UDPHS_IEN |= UDPHS_IEN_ENDRESET | UDPHS_IEN_DET_SUSPD | UDPHS_IEN_WAKE_UP;

  // Below the stepper and temperature timers - USB can always wait
  NVIC_SetPriority(UDPHS_IRQn, 8);
  NVIC_EnableIRQ(UDPHS_IRQn);

  // Attach: pull D+ up and let the host see us
  UDPHS->UDPHS_CTRL &= ~uint32_t(UDPHS_CTRL_DETACH);
}

bool usb_cdc_connected() { return usb_configured && usb_dtr; }

uint16_t usb_cdc_available() { return rx_count(); }

int usb_cdc_peek() {
  if (rx_head == rx_tail) return -1;
  return rx_buf[rx_tail];
}

int usb_cdc_read() {
  if (rx_head == rx_tail) return -1;
  const uint8_t c = rx_buf[rx_tail];
  rx_tail = (rx_tail + 1) & (USB_RX_SIZE - 1);
  return c;
}

void usb_cdc_rx_flush() { rx_tail = rx_head; }

void usb_cdc_write(const uint8_t c) {
  // With no host listening, throw the byte away rather than blocking forever.
  // Marlin chatters on startup long before anyone opens the port.
  if (!usb_cdc_connected()) return;

  const uint16_t next = (tx_head + 1) & (USB_TX_SIZE - 1);

  // Buffer full: let the ISR drain it. Give up if the host stops reading, so
  // a disconnected cable cannot wedge the firmware.
  uint32_t guard = 200000;
  while (next == tx_tail && --guard) {
    if (!usb_cdc_connected()) return;
    bulk_in_start();
  }
  if (next == tx_tail) return;

  tx_buf[tx_head] = c;
  tx_head = next;

  // Push as soon as a full packet is available; short packets wait for
  // usb_cdc_flush() or the idle task so we are not sending 1-byte packets.
  if (tx_count() >= EP_BULK_SIZE) bulk_in_start();
}

void usb_cdc_flush() {
  if (!usb_cdc_connected()) return;
  bulk_in_start();
}

void usb_cdc_task() {
  if (usb_cdc_connected() && !tx_busy && tx_count()) bulk_in_start();
}

#endif // __SAM3U4E__
