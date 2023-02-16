/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2011, 2012 LeafLabs, LLC.
 * Copyright (c) 2010 Perry Hung.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

/**
 * @file libmaple/spi.c
 * @author Marti Bolivar <mbolivar@leaflabs.com>
 * @brief Serial Peripheral Interface (SPI) support.
 *        Currently, there is no Integrated Interchip Sound (I2S) support.
 */

#ifdef USE_SPI

#include "spi.h"
#include "bitband.h"

static void spi_reconfigure(spi_dev *dev, uint32 cr1_config);

/*
 * SPI convenience routines
 */

/**
 * @brief Initialize and reset a SPI device.
 * @param dev Device to initialize and reset.
 */
void spi_init(spi_dev *dev)
{
    rcc_clk_enable(dev->clk_id);
    rcc_reset_dev(dev->clk_id);
}

/**
 * @brief Configure and enable a SPI device as bus master.
 *
 * The device's peripheral will be disabled before being reconfigured.
 *
 * @param dev Device to configure as bus master
 * @param baud Bus baud rate
 * @param mode SPI mode
 * @param flags Logical OR of spi_cfg_flag values.
 * @see spi_cfg_flag
 */
void spi_master_enable(spi_dev *dev,
                       spi_baud_rate baud,
                       spi_mode mode,
                       uint32 flags)
{
    spi_reconfigure(dev, baud | flags | SPI_CR1_MSTR | mode);
}

/**
 * @brief Configure and enable a SPI device as a bus slave.
 *
 * The device's peripheral will be disabled before being reconfigured.
 *
 * @param dev Device to configure as a bus slave
 * @param mode SPI mode
 * @param flags Logical OR of spi_cfg_flag values.
 * @see spi_cfg_flag
 */
void spi_slave_enable(spi_dev *dev, spi_mode mode, uint32 flags)
{
    spi_reconfigure(dev, flags | mode);
}

/**
 * @brief Blocking SPI transmit.
 * @param dev SPI port to use for transmission
 * @param buf Buffer to transmit.  The sizeof buf's elements are
 *            inferred from dev's data frame format (i.e., are
 *            correctly treated as 8-bit or 16-bit quantities).
 * @param len Maximum number of elements to transmit.
 * @return Number of elements transmitted.
 */
uint32 spi_tx(spi_dev *dev, const void *buf, uint32 len)
{
    uint32 txed = len;
    spi_reg_map *regs = dev->regs;
    if (spi_dff(dev) == SPI_DFF_8_BIT)
    {
        const uint8 *dp8 = (const uint8 *)buf;
        while (len--)
        {
            while ((regs->SR & SPI_SR_TXE) == 0)
                ; // while ( spi_is_tx_empty(dev)==0 ); // wait Tx to be empty
            regs->DR = *dp8++;
        }
    }
    else
    {
        const uint16 *dp16 = (const uint16 *)buf;
        while (len--)
        {
            while ((regs->SR & SPI_SR_TXE) == 0)
                ; // while ( spi_is_tx_empty(dev)==0 ); // wait Tx to be empty
            regs->DR = *dp16++;
        }
    }
    return txed;
}

/**
 * @brief Enable a SPI peripheral
 * @param dev Device to enable
 */
void spi_peripheral_enable(spi_dev *dev)
{
    bb_peri_set_bit(&dev->regs->CR1, SPI_CR1_SPE_BIT, 1);
}

/**
 * @brief Disable a SPI peripheral
 * @param dev Device to disable
 */
void spi_peripheral_disable(spi_dev *dev)
{
    bb_peri_set_bit(&dev->regs->CR1, SPI_CR1_SPE_BIT, 0);
}

/**
 * @brief Enable DMA requests whenever the transmit buffer is empty
 * @param dev SPI device on which to enable TX DMA requests
 */
void spi_tx_dma_enable(spi_dev *dev)
{
    bb_peri_set_bit(&dev->regs->CR2, SPI_CR2_TXDMAEN_BIT, 1);
}

/**
 * @brief Disable DMA requests whenever the transmit buffer is empty
 * @param dev SPI device on which to disable TX DMA requests
 */
void spi_tx_dma_disable(spi_dev *dev)
{
    bb_peri_set_bit(&dev->regs->CR2, SPI_CR2_TXDMAEN_BIT, 0);
}

/**
 * @brief Enable DMA requests whenever the receive buffer is empty
 * @param dev SPI device on which to enable RX DMA requests
 */
void spi_rx_dma_enable(spi_dev *dev)
{
    bb_peri_set_bit(&dev->regs->CR2, SPI_CR2_RXDMAEN_BIT, 1);
}

/**
 * @brief Disable DMA requests whenever the receive buffer is empty
 * @param dev SPI device on which to disable RX DMA requests
 */
void spi_rx_dma_disable(spi_dev *dev)
{
    bb_peri_set_bit(&dev->regs->CR2, SPI_CR2_RXDMAEN_BIT, 0);
}

/*
 * SPI auxiliary routines
 */

static void spi_reconfigure(spi_dev *dev, uint32 cr1_config)
{
#define MASK (SPI_CR1_CRCEN | SPI_CR1_DFF)
    spi_irq_disable(dev, SPI_INTERRUPTS_ALL);
    if ((dev->regs->CR1 & MASK) != (cr1_config & MASK))
        spi_peripheral_disable(dev);
    dev->regs->CR1 = cr1_config;
    spi_peripheral_enable(dev);
}
#endif
