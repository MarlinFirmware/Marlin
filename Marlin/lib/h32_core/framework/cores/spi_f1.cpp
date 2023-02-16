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
 * @file libmaple/stm32f1/spi.c
 * @author Marti Bolivar <mbolivar@leaflabs.com>
 * @brief STM32F1 SPI/I2S.
 */

#ifdef USE_SPI
#include "spi.h"
#include "gpio.h"
#include "spi_private.h"

/*
 * Devices
 */

static spi_dev spi1 = SPI_DEV(1);
static spi_dev spi2 = SPI_DEV(2);

spi_dev *SPI1 = &spi1;
spi_dev *SPI2 = &spi2;

/*
 * Routines
 */

void spi_config_gpios(spi_dev *ignored __attribute__((unused)),
                      uint8 as_master,
                      gpio_dev *nss_dev,
                      uint8 nss_bit,
                      gpio_dev *comm_dev,
                      uint8 sck_bit,
                      uint8 miso_bit,
                      uint8 mosi_bit)
{
    if (as_master)
    {
        //   gpio_set_mode(nss_dev, nss_bit, GPIO_AF_OUTPUT_PP);// Roger Clark. Commented out, so that NSS can be driven as a normal GPIO pin during SPI use
        gpio_set_mode(comm_dev, sck_bit, GPIO_AF_OUTPUT_PP);
        gpio_set_mode(comm_dev, miso_bit, GPIO_INPUT_FLOATING);
        gpio_set_mode(comm_dev, mosi_bit, GPIO_AF_OUTPUT_PP);
    }
    else
    {
        gpio_set_mode(nss_dev, nss_bit, GPIO_INPUT_FLOATING);
        gpio_set_mode(comm_dev, sck_bit, GPIO_INPUT_FLOATING);
        gpio_set_mode(comm_dev, miso_bit, GPIO_AF_OUTPUT_PP);
        gpio_set_mode(comm_dev, mosi_bit, GPIO_INPUT_FLOATING);
    }
}

void spi_foreach(void (*fn)(spi_dev *))
{
    fn(SPI1);
    fn(SPI2);
}
#endif
