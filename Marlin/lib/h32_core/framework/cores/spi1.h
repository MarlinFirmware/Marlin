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
 * @file libmaple/stm32f1/include/series/spi.h
 * @author Marti Bolivar <mbolivar@leaflabs.com>
 * @brief STM32F1 SPI/I2S series header.
 */

#ifndef _LIBMAPLE_STM32F1_SPI_H_
#define _LIBMAPLE_STM32F1_SPI_H_

#include "libmaple_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef USE_SPI

    /*
     * Register map base pointers
     */

    struct spi_reg_map;

#define SPI1_BASE ((struct spi_reg_map *)0x40013000)
#define SPI2_BASE ((struct spi_reg_map *)0x40003800)
#define SPI3_BASE ((struct spi_reg_map *)0x40003C00)

    /*
     * Device pointers
     */

    struct spi_dev;

    extern struct spi_dev *SPI1;
    extern struct spi_dev *SPI2;

    /*
     * Routines
     */

    /* spi_gpio_cfg(): Backwards compatibility shim to spi_config_gpios() */
    struct gpio_dev;
    extern void spi_config_gpios(struct spi_dev *, uint8,
                                 struct gpio_dev *, uint8,
                                 struct gpio_dev *, uint8, uint8, uint8);
    /**
     * @brief Deprecated. Use spi_config_gpios() instead.
     * @see spi_config_gpios()
     */
    static inline __always_inline void spi_gpio_cfg(uint8 as_master,
                                                    struct gpio_dev *nss_dev,
                                                    uint8 nss_bit,
                                                    struct gpio_dev *comm_dev,
                                                    uint8 sck_bit,
                                                    uint8 miso_bit,
                                                    uint8 mosi_bit)
    {
        /* We switched style globally to foo_config_gpios() and always
         * taking a foo_dev* argument (that last bit is the important
         * part) after this function was written.
         *
         * However, spi_config_gpios() just ignores the spi_dev* on F1, so
         * we can still keep this around for older code. */
        spi_config_gpios(NULL, as_master, nss_dev, nss_bit,
                         comm_dev, sck_bit, miso_bit, mosi_bit);
    }
#endif
#ifdef __cplusplus
}
#endif

#endif
