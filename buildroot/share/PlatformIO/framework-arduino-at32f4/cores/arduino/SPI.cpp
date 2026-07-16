/*
 * MIT License
 * Copyright (c) 2017 - 2022 _VIFEXTech
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// SPI Çevre Birimleri ve Pinler
// Bu kütüphanedeki başlatma kodu bize pin bağlantıları hakkında bilgi veriyor:

// SPI Birimi  SCK Pin MISO Pin MOSI Pin
// SPI1           PA5     PA6     PA7
// SPI2           PB13    PB14    PB15
// SPI3           PB3     PB4     PB5



#include "SPI.h"

#define SPI1_CLOCK                     (F_CPU)
#define SPI2_CLOCK                     (F_CPU)
#define SPI3_CLOCK                     (F_CPU)

SPIClass::SPIClass(spi_type* spix)
    : SPIx(spix)
    , SPI_Clock(0)
{
    memset(&spi_init_struct, 0, sizeof(spi_init_struct));
}

void SPIClass::SPI_Settings(
    spi_master_slave_mode_type master_slave_mode,
    spi_frame_bit_num_type frame_bit_num,
    uint16_t SPI_MODEx,
    spi_cs_mode_type cs_mode,
    spi_mclk_freq_div_type mclk_freq_div,
    spi_first_bit_type first_bit)
{
    spi_clock_polarity_type clock_polarity;
    spi_clock_phase_type clock_phase;
    spi_enable(SPIx, FALSE);

    switch(SPI_MODEx)
    {
    case 0:
        clock_polarity = SPI_CLOCK_POLARITY_LOW;
        clock_phase = SPI_CLOCK_PHASE_1EDGE;
        break;
    case 1:
        clock_polarity = SPI_CLOCK_POLARITY_LOW;
        clock_phase = SPI_CLOCK_PHASE_2EDGE;
        break;
    case 2:
        clock_polarity = SPI_CLOCK_POLARITY_HIGH;
        clock_phase = SPI_CLOCK_PHASE_1EDGE;
        break;
    case 3:
        clock_polarity = SPI_CLOCK_POLARITY_HIGH;
        clock_phase = SPI_CLOCK_PHASE_2EDGE;
        break;
    default:
        return;
    }

    spi_default_para_init(&spi_init_struct);
    spi_init_struct.transmission_mode = SPI_TRANSMIT_FULL_DUPLEX;
    spi_init_struct.master_slave_mode = master_slave_mode;
    spi_init_struct.frame_bit_num = frame_bit_num;
    spi_init_struct.clock_polarity = clock_polarity;
    spi_init_struct.clock_phase = clock_phase;
    spi_init_struct.cs_mode_selection = cs_mode;
    spi_init_struct.mclk_freq_division = mclk_freq_div;
    spi_init_struct.first_bit_transmission = first_bit;
    spi_init(SPIx, &spi_init_struct);

    spi_enable(SPIx, TRUE);
}

void SPIClass::begin(void)
{
    spi_i2s_reset(SPIx);
    if(SPIx == SPI1)
    {
        SPI_Clock = SPI1_CLOCK;
        crm_periph_clock_enable(CRM_SPI1_PERIPH_CLOCK, TRUE);
        pinMode(PA5, OUTPUT_AF_PP);
        pinMode(PA6, OUTPUT_AF_PP);
        pinMode(PA7, OUTPUT_AF_PP);
    }
    else if(SPIx == SPI2)
    {
        SPI_Clock = SPI2_CLOCK;
        crm_periph_clock_enable(CRM_SPI2_PERIPH_CLOCK, TRUE);
        pinMode(PB13, OUTPUT_AF_PP);
        pinMode(PB14, OUTPUT_AF_PP);
        pinMode(PB15, OUTPUT_AF_PP);

    }
    else if(SPIx == SPI3)
    {
        SPI_Clock = SPI3_CLOCK;
        crm_periph_clock_enable(CRM_SPI3_PERIPH_CLOCK, TRUE);
        pinMode(PB3, OUTPUT_AF_PP);
        pinMode(PB4, OUTPUT_AF_PP);
        pinMode(PB5, OUTPUT_AF_PP);

    }
    else
    {
        return;
    }

    SPI_Settings(
        SPI_MODE_MASTER,
        SPI_FRAME_8BIT,
        SPI_MODE0,
        SPI_CS_SOFTWARE_MODE,
        SPI_MCLK_DIV_8,
        SPI_FIRST_BIT_MSB
    );
}

void SPIClass::begin(uint32_t clock, uint16_t dataOrder, uint16_t dataMode)
{
    begin();
    setClock(clock);
    setBitOrder(dataOrder);
    setDataMode(dataMode);
    spi_enable(SPIx, TRUE);
}

void SPIClass::begin(SPISettings settings)
{
    begin();
    setClock(settings.clock);
    setBitOrder(settings.bitOrder);
    setDataMode(settings.dataMode);
    spi_enable(SPIx, TRUE);
}

void SPIClass::beginSlave(void)
{
    begin();
    SPI_Settings(
        SPI_MODE_SLAVE,
        SPI_FRAME_8BIT,
        SPI_MODE0,
        SPI_CS_SOFTWARE_MODE,
        SPI_MCLK_DIV_16,
        SPI_FIRST_BIT_MSB
    );
    spi_enable(SPIx, TRUE);
}

void SPIClass::end(void)
{
    spi_enable(SPIx, FALSE);
}

void SPIClass::setClock(uint32_t clock)
{
    if(clock == 0)
    {
        return;
    }

    static const spi_mclk_freq_div_type mclk_freq_div_map[] =
    {
        SPI_MCLK_DIV_2,
        SPI_MCLK_DIV_2,
        SPI_MCLK_DIV_4,
        SPI_MCLK_DIV_8,
        SPI_MCLK_DIV_16,
        SPI_MCLK_DIV_32,
        SPI_MCLK_DIV_64,
        SPI_MCLK_DIV_128,
        SPI_MCLK_DIV_256,
        SPI_MCLK_DIV_512,
        SPI_MCLK_DIV_1024,
    };
    const uint8_t mapSize = sizeof(mclk_freq_div_map) / sizeof(mclk_freq_div_map[0]);
    uint32_t clockDiv = SPI_Clock / clock;
    uint8_t mapIndex = 0;

    while(clockDiv > 1)
    {
        clockDiv = clockDiv >> 1;
        mapIndex++;
    }

    if(mapIndex >= mapSize)
    {
        mapIndex = mapSize - 1;
    }

    spi_init_struct.mclk_freq_division = mclk_freq_div_map[mapIndex];
    spi_init(SPIx, &spi_init_struct);
    spi_enable(SPIx, TRUE);
}

void SPIClass::setClockDivider(uint32_t Div)
{
    if(Div == 0)
    {
        Div = 1;
    }
#if SPI_CLASS_AVR_COMPATIBILITY_MODE
    setClock(16000000 / Div); // AVR:16MHz
#else
    setClock(SPI_Clock / Div);
#endif
}

void SPIClass::setBitOrder(uint16_t bitOrder)
{
    spi_init_struct.first_bit_transmission = (bitOrder == MSBFIRST) ? SPI_FIRST_BIT_MSB : SPI_FIRST_BIT_LSB;
    spi_init(SPIx, &spi_init_struct);
    spi_enable(SPIx, TRUE);
}

/*  Victor Perez. Added to test changing datasize from 8 to 16 bit modes on the fly.
*   Input parameter should be SPI_CR1_DFF set to 0 or 1 on a 32bit word.
*
*/
void SPIClass::setDataSize(uint32_t datasize)
{
    spi_init_struct.frame_bit_num = (spi_frame_bit_num_type)datasize;
    spi_init(SPIx, &spi_init_struct);
    spi_enable(SPIx, TRUE);
}

void SPIClass::setDataMode(uint8_t dataMode)
{
    /* Notes.  As far as I can tell, the AVR numbers for dataMode appear to match the numbers required by the STM32

    From the AVR doc http://www.atmel.com/images/doc2585.pdf section 2.4

    SPI Mode    CPOL    CPHA    Shift SCK-edge  Capture SCK-edge
    0           0       0       Falling         Rising
    1           0       1       Rising          Falling
    2           1       0       Rising          Falling
    3           1       1       Falling         Rising


    On the STM32 it appears to be

    bit 1 - CPOL : Clock polarity
        (This bit should not be changed when communication is ongoing)
        0 : CLK to 0 when idle
        1 : CLK to 1 when idle

    bit 0 - CPHA : Clock phase
        (This bit should not be changed when communication is ongoing)
        0 : The first clock transition is the first data capture edge
        1 : The second clock transition is the first data capture edge

    If someone finds this is not the case or sees a logic error with this let me know ;-)
     */
    spi_clock_polarity_type clock_polarity;
    spi_clock_phase_type clock_phase;
    spi_enable(SPIx, FALSE);

    switch(dataMode)
    {
    case 0:
        clock_polarity = SPI_CLOCK_POLARITY_LOW;
        clock_phase = SPI_CLOCK_PHASE_1EDGE;
        break;
    case 1:
        clock_polarity = SPI_CLOCK_POLARITY_LOW;
        clock_phase = SPI_CLOCK_PHASE_2EDGE;
        break;
    case 2:
        clock_polarity = SPI_CLOCK_POLARITY_HIGH;
        clock_phase = SPI_CLOCK_PHASE_1EDGE;
        break;
    case 3:
        clock_polarity = SPI_CLOCK_POLARITY_HIGH;
        clock_phase = SPI_CLOCK_PHASE_2EDGE;
        break;
    default:
        return;
    }

    spi_init_struct.clock_polarity = clock_polarity;
    spi_init_struct.clock_phase = clock_phase;
    spi_init(SPIx, &spi_init_struct);
    spi_enable(SPIx, TRUE);
}

void SPIClass::beginTransaction(SPISettings settings)
{
    SPISettings(settings.clock, settings.bitOrder, settings.dataMode);

    setClock(settings.clock);
    setBitOrder(settings.bitOrder);
    setDataMode(settings.dataMode);
    setDataSize(settings.dataSize);

    spi_enable(SPIx, TRUE);
}

void SPIClass::beginTransactionSlave(void)
{
    beginSlave();
}

void SPIClass::endTransaction(void)
{
    spi_enable(SPIx, FALSE);
}

uint16_t SPIClass::read(void)
{
    SPI_I2S_WAIT_RX(SPIx);
    return (uint16_t)(SPI_I2S_RXDATA(SPIx));
}

void SPIClass::read(uint8_t *buf, uint32_t len)
{
    if (len == 0)
        return;

    SPI_I2S_RXDATA_VOLATILE(SPIx);
    SPI_I2S_TXDATA(SPIx, 0x00FF);

    while((--len))
    {
        SPI_I2S_WAIT_TX(SPIx);
        noInterrupts();
        SPI_I2S_TXDATA(SPIx, 0x00FF);
        SPI_I2S_WAIT_RX(SPIx);
        *buf++ = (uint8_t)SPI_I2S_RXDATA(SPIx);
        interrupts();
    }
    SPI_I2S_WAIT_RX(SPIx);
    *buf++ = (uint8_t)SPI_I2S_RXDATA(SPIx);
}

void SPIClass::write(uint16_t data)
{
    SPI_I2S_TXDATA(SPIx, data);
    SPI_I2S_WAIT_TX(SPIx);
    SPI_I2S_WAIT_BUSY(SPIx);
}

void SPIClass::write(uint16_t data, uint32_t n)
{
    while ((n--) > 0)
    {
        SPI_I2S_TXDATA(SPIx, data); // write the data to be transmitted into the SPI_DR register (this clears the TXE flag)
        SPI_I2S_WAIT_TX(SPIx); // wait till Tx empty
    }

    SPI_I2S_WAIT_BUSY(SPIx); // wait until BSY=0 before returning
}

void SPIClass::write(const uint8_t *data, uint32_t length)
{
    while (length--)
    {
        SPI_I2S_WAIT_TX(SPIx);
        SPI_I2S_TXDATA(SPIx, *data++);
    }
    SPI_I2S_WAIT_TX(SPIx);
    SPI_I2S_WAIT_BUSY(SPIx);
}

void SPIClass::write(const uint16_t *data, uint32_t length)
{
    while (length--)
    {
        SPI_I2S_WAIT_TX(SPIx);
        SPI_I2S_TXDATA(SPIx, *data++);
    }
    SPI_I2S_WAIT_TX(SPIx);
    SPI_I2S_WAIT_BUSY(SPIx);
}

uint8_t SPIClass::transfer(uint8_t wr_data) const
{
    SPI_I2S_RXDATA_VOLATILE(SPIx);
    SPI_I2S_TXDATA(SPIx, wr_data);
    SPI_I2S_WAIT_TX(SPIx);
    SPI_I2S_WAIT_BUSY(SPIx);
    return (uint8_t)SPI_I2S_RXDATA(SPIx);
}

uint16_t SPIClass::transfer16(uint16_t wr_data) const
{
    SPI_I2S_RXDATA_VOLATILE(SPIx);
    SPI_I2S_TXDATA(SPIx, wr_data);
    SPI_I2S_WAIT_TX(SPIx);
    SPI_I2S_WAIT_BUSY(SPIx);
    return (uint16_t)SPI_I2S_RXDATA(SPIx);
}

uint8_t SPIClass::send(uint8_t data)
{
    this->write(data);
    return 1;
}

uint8_t SPIClass::send(uint8_t *buf, uint32_t len)
{
    this->write(buf, len);
    return len;
}

uint8_t SPIClass::recv(void)
{
    return this->read();
}



// #if SPI_CLASS_1_ENABLE
// SPIClass SPI(SPI_CLASS_1_SPI);
// #endif

// #if SPI_CLASS_2_ENABLE
// SPIClass SPI_2(SPI_CLASS_2_SPI);
// #endif

// #if SPI_CLASS_3_ENABLE
// SPIClass SPI_3(SPI_CLASS_3_SPI);
// #endif
