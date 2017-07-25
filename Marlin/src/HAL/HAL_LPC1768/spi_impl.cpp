#ifdef TARGET_LPC1768

#include "../spi_api.h"

#include <lpc17xx_ssp.h>
#include <lpc17xx_pinsel.h>
#include <lpc17xx_gpio.h>
#include "lpc17xx_clkpwr.h"

extern "C" void SSP0_IRQHandler(void);
extern "C" void SSP1_IRQHandler(void);

namespace HAL {
namespace SPI {

enum class SignalPolarity : uint8_t {
  ACTIVE_LOW = 0,
  ACTIVE_HIGH
};

/* Hardware channels :
 * 0: clk(0_7),  mosi(0_9),  miso(0_8),  SSP1
 * 1: clk(0_15), mosi(0_28), miso(0_17), SSP0

 * Logical channels:
 * 0: hwchannel: 1, ssel(0_6)
 * 1: hwchannel: 0, ssel(0_16)
 * 2: hwchannel: 0, ssel(1_23)
 */

/*
 * Defines the Hardware setup for an SPI channel
 * The pins and (if applicable) the Hardware Peripheral
 *
 */
struct LogicalChannel; // who doesn't like circular dependencies

struct HardwareChannel {
  LPC_SSP_TypeDef *peripheral;
  IRQn_Type IRQn;
  uint8_t clk_port;
  uint8_t clk_pin;
  uint8_t mosi_port;
  uint8_t mosi_pin;
  uint8_t miso_port;
  uint8_t miso_pin;
  SSP_DATA_SETUP_Type xfer_config;
  volatile FlagStatus xfer_complete;
  bool initialised;
  volatile bool in_use;
  LogicalChannel* active_channel;
} hardware_channels[2] = {
    {LPC_SSP0, SSP0_IRQn, 0, 15, 0, 18, 0, 17, { nullptr, 0, nullptr, 0, 0, SSP_STAT_DONE }, RESET, false, false, nullptr},
    {LPC_SSP1, SSP1_IRQn, 0, 7,  0, 9,  0, 8 , { nullptr, 0, nullptr, 0, 0, SSP_STAT_DONE }, RESET, false, false, nullptr}
};

/*
 * Define all available logical SPI ports
 */
struct LogicalChannel {
  HardwareChannel& hw_channel;
  uint8_t ssel_port;
  uint8_t ssel_pin;
  SignalPolarity ssel_polarity;
  bool ssel_override;
  SSP_CFG_Type config;
  uint32_t CR0;
  uint32_t CPSR;
} logical_channels[3] = {
    { hardware_channels[1], 0, 6,  SignalPolarity::ACTIVE_LOW, false, { SSP_DATABIT_8, SSP_CPHA_FIRST, SSP_CPOL_HI, SSP_MASTER_MODE, SSP_FRAME_SPI, 1000000 }, 0, 0 },
    { hardware_channels[0], 0, 16, SignalPolarity::ACTIVE_HIGH, true, { SSP_DATABIT_8, SSP_CPHA_FIRST, SSP_CPOL_HI, SSP_MASTER_MODE, SSP_FRAME_SPI, 1000000 }, 0, 0 },
    { hardware_channels[0], 1, 23, SignalPolarity::ACTIVE_LOW, true, { SSP_DATABIT_8, SSP_CPHA_FIRST, SSP_CPOL_HI, SSP_MASTER_MODE, SSP_FRAME_SPI, 1000000 }, 0, 0 }
};

//Internal functions
extern "C" void ssp_irq_handler(uint8_t hw_channel);
LogicalChannel* get_logical_channel(uint8_t channel);
bool set_ssel(LogicalChannel* logical_channel);
void clear_ssel(LogicalChannel* logical_channel);
void restore_frequency(LogicalChannel* logical_channel);

LogicalChannel* get_logical_channel(uint8_t channel) {
  if(channel > sizeof(logical_channels) - 1) {
    return nullptr;
  }
  return &logical_channels[channel];
}

bool set_ssel(LogicalChannel* logical_channel) {
  if(logical_channel->hw_channel.in_use == true) {
    return false;
  }

  if(logical_channel->ssel_polarity == SignalPolarity::ACTIVE_HIGH) {
    GPIO_SetValue(logical_channel->ssel_port, (1 << logical_channel->ssel_pin));
  } else {
    GPIO_ClearValue(logical_channel->ssel_port, (1 << logical_channel->ssel_pin));
  }
  logical_channel->hw_channel.in_use = true;

  return true;
}

void clear_ssel(LogicalChannel* logical_channel) {
  if(logical_channel->ssel_polarity == SignalPolarity::ACTIVE_HIGH) {
    GPIO_ClearValue(logical_channel->ssel_port, (1 << logical_channel->ssel_pin));
  } else {
    GPIO_SetValue(logical_channel->ssel_port, (1 << logical_channel->ssel_pin));
  }
  logical_channel->hw_channel.in_use = false;
}

void restore_frequency(LogicalChannel* logical_channel) {
  logical_channel->hw_channel.peripheral->CR0 = logical_channel->CR0;
  logical_channel->hw_channel.peripheral->CPSR = logical_channel->CPSR;
}

/*
 * SPI API Implementation
 */
bool initialise(uint8_t channel) {
  LogicalChannel* logical_channel = get_logical_channel(channel);
  if(logical_channel == nullptr) return false;
  HardwareChannel& hw_channel = logical_channel->hw_channel;

  PINSEL_CFG_Type pin_cfg;
  pin_cfg.OpenDrain = PINSEL_PINMODE_NORMAL;
  pin_cfg.Pinmode = PINSEL_PINMODE_PULLUP;

  if(hw_channel.initialised == false) {
    pin_cfg.Funcnum = 2; //ssp (spi) function
    pin_cfg.Portnum = hw_channel.clk_port;
    pin_cfg.Pinnum = hw_channel.clk_pin;
    PINSEL_ConfigPin(&pin_cfg); //clk

    pin_cfg.Portnum = hw_channel.miso_port;
    pin_cfg.Pinnum = hw_channel.miso_pin;
    PINSEL_ConfigPin(&pin_cfg); //miso

    pin_cfg.Portnum = hw_channel.mosi_port;
    pin_cfg.Pinnum = hw_channel.mosi_pin;
    PINSEL_ConfigPin(&pin_cfg); //mosi

    SSP_Init(hw_channel.peripheral, &logical_channel->config);
    logical_channel->CR0 = logical_channel->hw_channel.peripheral->CR0; // preserve for restore
    logical_channel->CPSR = logical_channel->hw_channel.peripheral->CPSR; // preserve for restore
    SSP_Cmd(hw_channel.peripheral, ENABLE);

    hw_channel.initialised = true;
    hw_channel.active_channel = logical_channel;

    //NVIC_SetPriority(hw_channel.IRQn, NVIC_EncodePriority(0, 3, 0)); //Very Low priority
    //NVIC_EnableIRQ(hw_channel.IRQn);
  }

  pin_cfg.Portnum = logical_channel->ssel_port;
  pin_cfg.Pinnum = logical_channel->ssel_pin;
  pin_cfg.Pinmode = logical_channel->ssel_polarity ==  SignalPolarity::ACTIVE_LOW ? PINSEL_PINMODE_PULLUP : PINSEL_PINMODE_PULLDOWN;
  pin_cfg.Funcnum = 0; //gpio function
  PINSEL_ConfigPin(&pin_cfg); //ssel

  GPIO_SetDir(logical_channel->ssel_port, (1 << logical_channel->ssel_pin), 1);
  GPIO_SetValue(logical_channel->ssel_port, (1 << logical_channel->ssel_pin));
  return true;
}

bool enable_cs(uint8_t channel) {
  LogicalChannel* logical_channel = get_logical_channel(channel);
  if(logical_channel == nullptr) return false;
  return set_ssel(logical_channel);
}

void disable_cs(uint8_t channel) {
  LogicalChannel* logical_channel = get_logical_channel(channel);
  if(logical_channel == nullptr) return;
  if(logical_channel->hw_channel.in_use && !logical_channel->ssel_override) return; //automatic SSel wasn't overridden

  clear_ssel(logical_channel);
}

void set_frequency(uint8_t channel, uint32_t frequency) {
  LogicalChannel* logical_channel = get_logical_channel(channel);
  if(logical_channel == nullptr) return;

  SSP_Cmd(logical_channel->hw_channel.peripheral, DISABLE);
  uint32_t prescale, cr0_div, cmp_clk, ssp_clk;

  if (logical_channel->hw_channel.peripheral == LPC_SSP0){
    ssp_clk = CLKPWR_GetPCLK (CLKPWR_PCLKSEL_SSP0);
  } else if (logical_channel->hw_channel.peripheral == LPC_SSP1) {
    ssp_clk = CLKPWR_GetPCLK (CLKPWR_PCLKSEL_SSP1);
  } else {
    return;
  }
  //find the closest clock divider / prescaler
  cr0_div = 0;
  cmp_clk = 0xFFFFFFFF;
  prescale = 2;
  while (cmp_clk > frequency) {
    cmp_clk = ssp_clk / ((cr0_div + 1) * prescale);
    if (cmp_clk > frequency) {
      cr0_div++;
      if (cr0_div > 0xFF) {
        cr0_div = 0;
        prescale += 2;
      }
    }
  }

  logical_channel->hw_channel.peripheral->CR0 &= (~SSP_CR0_SCR(0xFF)) & SSP_CR0_BITMASK;
  logical_channel->hw_channel.peripheral->CR0 |= (SSP_CR0_SCR(cr0_div)) & SSP_CR0_BITMASK;
  logical_channel->CR0 = logical_channel->hw_channel.peripheral->CR0; // preserve for restore

  logical_channel->hw_channel.peripheral->CPSR = prescale & SSP_CPSR_BITMASK;
  logical_channel->CPSR = logical_channel->hw_channel.peripheral->CPSR; // preserve for restore

  logical_channel->config.ClockRate = ssp_clk / ((cr0_div + 1) * prescale);

  SSP_Cmd(logical_channel->hw_channel.peripheral, ENABLE);
}

void read(uint8_t channel, uint8_t *buffer, uint32_t length) {
  transfer(channel, nullptr, buffer, length);
}

uint8_t read(uint8_t channel) {
  uint8_t buffer;
  transfer(channel, nullptr, &buffer, 1);
  return buffer;
}

void write(uint8_t channel, const uint8_t *buffer, uint32_t length) {
  transfer(channel, buffer, nullptr, length);
}

void write(uint8_t channel, uint8_t value) {
  transfer(channel, &value, nullptr, 1);
}

void transfer(uint8_t channel, const uint8_t *buffer_write, uint8_t *buffer_read, uint32_t length) {
  LogicalChannel* logical_channel = get_logical_channel(channel);
  if(logical_channel == nullptr) return;

  if((logical_channel->hw_channel.in_use && !logical_channel->ssel_override) || !logical_channel->hw_channel.initialised) return;
  if(!logical_channel->ssel_override) {
    if(!set_ssel(logical_channel)) return;
  }

  if(logical_channel != logical_channel->hw_channel.active_channel) {
    restore_frequency(logical_channel);
    logical_channel->hw_channel.active_channel = logical_channel;
  }

  logical_channel->hw_channel.xfer_config.tx_data = (void *)buffer_write;
  logical_channel->hw_channel.xfer_config.rx_data = (void *)buffer_read;
  logical_channel->hw_channel.xfer_config.length = length;

  (void)SSP_ReadWrite(logical_channel->hw_channel.peripheral, &logical_channel->hw_channel.xfer_config, SSP_TRANSFER_POLLING); //SSP_TRANSFER_INTERRUPT

  if(!logical_channel->ssel_override) {
    clear_ssel(logical_channel->hw_channel.active_channel);
  }
}

uint8_t transfer(uint8_t channel, uint8_t value) {
  uint8_t buffer;
  transfer(channel, &value, &buffer, 1);
  return buffer;
}

/*
 *  Interrupt Handlers
 */
extern "C" void ssp_irq_handler(uint8_t hw_channel) {

  SSP_DATA_SETUP_Type *xf_setup;
  uint32_t tmp;
  uint8_t dataword;

  // Disable all SSP interrupts
  SSP_IntConfig(hardware_channels[hw_channel].peripheral, SSP_INTCFG_ROR | SSP_INTCFG_RT | SSP_INTCFG_RX | SSP_INTCFG_TX, DISABLE);

  dataword = (SSP_GetDataSize(hardware_channels[hw_channel].peripheral) > 8) ? 1 : 0;

  xf_setup = &hardware_channels[hw_channel].xfer_config;
  // save status
  tmp = SSP_GetRawIntStatusReg(hardware_channels[hw_channel].peripheral);
  xf_setup->status = tmp;

  // Check overrun error
  if (tmp & SSP_RIS_ROR) {
    // Clear interrupt
    SSP_ClearIntPending(hardware_channels[hw_channel].peripheral, SSP_INTCLR_ROR);
    // update status
    xf_setup->status |= SSP_STAT_ERROR;
    // Set Complete Flag
    hardware_channels[hw_channel].xfer_complete = SET;
    if(!hardware_channels[hw_channel].active_channel->ssel_override) clear_ssel(hardware_channels[hw_channel].active_channel);
    return;
  }

  if ((xf_setup->tx_cnt != xf_setup->length) || (xf_setup->rx_cnt != xf_setup->length)) {
    /* check if RX FIFO contains data */
    while ((SSP_GetStatus(hardware_channels[hw_channel].peripheral, SSP_STAT_RXFIFO_NOTEMPTY)) && (xf_setup->rx_cnt != xf_setup->length)) {
      // Read data from SSP data
      tmp = SSP_ReceiveData(hardware_channels[hw_channel].peripheral);

      // Store data to destination
      if (xf_setup->rx_data != nullptr) {
        if (dataword == 0) {
          *(uint8_t *) ((uint32_t) xf_setup->rx_data + xf_setup->rx_cnt) = (uint8_t) tmp;
        } else {
          *(uint16_t *) ((uint32_t) xf_setup->rx_data + xf_setup->rx_cnt) = (uint16_t) tmp;
        }
      }
      // Increase counter
      if (dataword == 0) {
        xf_setup->rx_cnt++;
      } else {
        xf_setup->rx_cnt += 2;
      }
    }

    while ((SSP_GetStatus(hardware_channels[hw_channel].peripheral, SSP_STAT_TXFIFO_NOTFULL)) && (xf_setup->tx_cnt != xf_setup->length)) {
      // Write data to buffer
      if (xf_setup->tx_data == nullptr) {
        if (dataword == 0) {
          SSP_SendData(hardware_channels[hw_channel].peripheral, 0xFF);
          xf_setup->tx_cnt++;
        } else {
          SSP_SendData(hardware_channels[hw_channel].peripheral, 0xFFFF);
          xf_setup->tx_cnt += 2;
        }
      } else {
        if (dataword == 0) {
          SSP_SendData(hardware_channels[hw_channel].peripheral, (*(uint8_t *) ((uint32_t) xf_setup->tx_data + xf_setup->tx_cnt)));
          xf_setup->tx_cnt++;
        } else {
          SSP_SendData(hardware_channels[hw_channel].peripheral, (*(uint16_t *) ((uint32_t) xf_setup->tx_data + xf_setup->tx_cnt)));
          xf_setup->tx_cnt += 2;
        }
      }

      // Check overrun error
      if (SSP_GetRawIntStatus(hardware_channels[hw_channel].peripheral, SSP_INTSTAT_RAW_ROR)) {
        // update status
        xf_setup->status |= SSP_STAT_ERROR;
        // Set Complete Flag
        hardware_channels[hw_channel].xfer_complete = SET;
        if(!hardware_channels[hw_channel].active_channel->ssel_override) clear_ssel(hardware_channels[hw_channel].active_channel);
        return;
      }

      // Check for any data available in RX FIFO
      while ((SSP_GetStatus(hardware_channels[hw_channel].peripheral, SSP_STAT_RXFIFO_NOTEMPTY)) && (xf_setup->rx_cnt != xf_setup->length)) {
        // Read data from SSP data
        tmp = SSP_ReceiveData(hardware_channels[hw_channel].peripheral);

        // Store data to destination
        if (xf_setup->rx_data != nullptr) {
          if (dataword == 0) {
            *(uint8_t *) ((uint32_t) xf_setup->rx_data + xf_setup->rx_cnt) = (uint8_t) tmp;
          } else {
            *(uint16_t *) ((uint32_t) xf_setup->rx_data + xf_setup->rx_cnt) = (uint16_t) tmp;
          }
        }
        // Increase counter
        if (dataword == 0) {
          xf_setup->rx_cnt++;
        } else {
          xf_setup->rx_cnt += 2;
        }
      }
    }
  }

  // If there more data to sent or receive
  if ((xf_setup->rx_cnt != xf_setup->length) || (xf_setup->tx_cnt != xf_setup->length)) {
    // Enable all interrupt
    SSP_IntConfig(hardware_channels[hw_channel].peripheral, SSP_INTCFG_ROR | SSP_INTCFG_RT | SSP_INTCFG_RX | SSP_INTCFG_TX, ENABLE);
  } else {
    // Save status
    xf_setup->status = SSP_STAT_DONE;
    // Set Complete Flag
    hardware_channels[hw_channel].xfer_complete = SET;
    if(!hardware_channels[hw_channel].active_channel->ssel_override) clear_ssel(hardware_channels[hw_channel].active_channel);
  }
}

}
}

extern "C" void SSP0_IRQHandler(void) {
  HAL::SPI::ssp_irq_handler(0);
}

extern "C" void SSP1_IRQHandler(void) {
  HAL::SPI::ssp_irq_handler(1);
}


#endif
