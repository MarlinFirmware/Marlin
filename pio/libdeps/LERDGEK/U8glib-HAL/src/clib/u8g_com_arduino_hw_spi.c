/*

  u8g_com_arduino_hw_spi.c

  Universal 8bit Graphics Library

  Copyright (c) 2011, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list
    of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright notice, this
    list of conditions and the following disclaimer in the documentation and/or other
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

  SPI Clock Cycle Type

  SSD1351        50ns          20   MHz
  SSD1322       300ns           3.3 MHz
  SSD1327       300ns
  SSD1306       300ns
  ST7565        400ns           2.5 MHz
  ST7920        400ns

  Arduino DUE

  PA25  MISO
  PA26  MOSI    75
  PA27  SCLK    76


typedef struct {
  WoReg SPI_CR;        (Spi Offset: 0x00) Control Register
  RwReg SPI_MR;        (Spi Offset: 0x04) Mode Register
  RoReg SPI_RDR;       (Spi Offset: 0x08) Receive Data Register
  WoReg SPI_TDR;       (Spi Offset: 0x0C) Transmit Data Register
  RoReg SPI_SR;        (Spi Offset: 0x10) Status Register
  WoReg SPI_IER;       (Spi Offset: 0x14) Interrupt Enable Register
  WoReg SPI_IDR;       (Spi Offset: 0x18) Interrupt Disable Register
  RoReg SPI_IMR;       (Spi Offset: 0x1C) Interrupt Mask Register
  RoReg Reserved1[4];
  RwReg SPI_CSR[4];    (Spi Offset: 0x30) Chip Select Register
  RoReg Reserved2[41];
  RwReg SPI_WPMR;      (Spi Offset: 0xE4) Write Protection Control Register
  RoReg SPI_WPSR;      (Spi Offset: 0xE8) Write Protection Status Register
} Spi;

  Power Management Controller (PMC)
  arduino-1.5.2/hardware/arduino/sam/system/CMSIS/Device/ATMEL/sam3xa/include/instance/instance_pmc.h
    - enable PIO

      REG_PMC_PCER0 = 1UL << ID_PIOA
    - enable SPI
      REG_PMC_PCER0 = 1UL << ID_SPI0


    - enable PIOA and SPI0
      REG_PMC_PCER0 = (1UL << ID_PIOA) | (1UL << ID_SPI0);

  Parallel Input/Output Controller (PIO)
  arduino-1.5.2/hardware/arduino/sam/system/CMSIS/Device/ATMEL/sam3xa/include/instance/instance_pioa.h
    - enable special function of the pin: disable PIO on A26 and A27:
        REG_PIOA_PDR = 0x0c000000
        PIOA->PIO_PDR = 0x0c000000

  SPI
    SPI0->SPI_CR = SPI_CR_SPIDIS
    SPI0->SPI_CR = SPI_CR_SWRST ;
    SPI0->SPI_CR = SPI_CR_SWRST ;
    SPI0->SPI_CR = SPI_CR_SPIEN

    Bit 0: Master Mode = 1 (active)
    Bit 1: Peripheral Select = 0 (fixed)
    Bit 2: Chip Select Decode Mode = 1 (4 to 16)
    Bit 4: Mode Fault Detection = 1 (disabled)
    Bit 5: Wait Data Read = 0 (disabled)
    Bit 7: Loop Back Mode = 0 (disabled)
    Bit 16-19: Peripheral Chip Select = 0 (chip select 0)
    SPI0->SPI_MR = SPI_MR_MSTR | SPI_MR_PCSDEC | SPI_MR_MODFDIS

    Bit 0: Clock Polarity = 0
    Bit 1: Clock Phase = 0
    Bit 4-7: Bits = 0 (8 Bit)
    Bit 8-15: SCBR = 1
    SPI0->SPI_CSR[0] = SPI_CSR_SCBR(x)  Serial Baud Rate
        SCBR / 84000000 > 50 / 1000000000
        SCBR / 84 > 5 / 100
        SCBR  > 50 *84 / 1000 --> SCBR=5
        SCBR  > 300*84 / 1000 --> SCBR=26
        SCBR  > 400*84 / 1000 --> SCBR=34

  Arduino Due test code:
    REG_PMC_PCER0 = (1UL << ID_PIOA) | (1UL << ID_SPI0);
    REG_PIOA_PDR = 0x0c000000;
    SPI0->SPI_CR = SPI_CR_SPIDIS;
    SPI0->SPI_CR = SPI_CR_SWRST;
    SPI0->SPI_CR = SPI_CR_SWRST;
    SPI0->SPI_CR = SPI_CR_SPIEN;
    SPI0->SPI_MR = SPI_MR_MSTR | SPI_MR_PCSDEC | SPI_MR_MODFDIS;
    SPI0->SPI_CSR[0] = SPI_CSR_SCBR(30);

    for(;;)
    {
      while( (SPI0->SPI_SR & SPI_SR_TDRE) == 0 )
        ;
      SPI0->SPI_TDR = 0x050;
    }

*/

#include "u8g.h"

#ifdef ARDUINO

#ifdef __AVR__
#define U8G_ARDUINO_ATMEGA_HW_SPI
/* remove the definition for attiny */
#if __AVR_ARCH__ == 2
#undef U8G_ARDUINO_ATMEGA_HW_SPI
#endif
#if __AVR_ARCH__ == 25
#undef U8G_ARDUINO_ATMEGA_HW_SPI
#endif
#endif

#ifdef U8G_ARDUINO_ATMEGA_HW_SPI

#include <avr/interrupt.h>
#include <avr/io.h>

#if ARDUINO < 100
#include <WProgram.h>

/* fixed pins */
#if defined(__AVR_ATmega644P__) || defined(__AVR_ATmega1284P__) // Sanguino.cc board
#define PIN_SCK         7
#define PIN_MISO        6
#define PIN_MOSI        5
#define PIN_CS          4
#else                                   // Arduino Board
#define PIN_SCK 13
#define PIN_MISO  12
#define PIN_MOSI 11
#define PIN_CS 10
#endif // (__AVR_ATmega644P__) || defined(__AVR_ATmega1284P__)

#else

#include <Arduino.h>

/* use Arduino pin definitions */
#define PIN_SCK SCK
#define PIN_MISO  MISO
#define PIN_MOSI MOSI
#define PIN_CS SS

#endif



//static uint8_t u8g_spi_out(uint8_t data) U8G_NOINLINE;
static uint8_t u8g_spi_out(uint8_t data)
{
  /* unsigned char x = 100; */
  /* send data */
  SPDR = data;
  /* wait for transmission */
  while (!(SPSR & (1<<SPIF)))
    ;
  /* clear the SPIF flag by reading SPDR */
  return  SPDR;
}


uint8_t u8g_com_arduino_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  switch(msg)
  {
    case U8G_COM_MSG_STOP:
      break;

    case U8G_COM_MSG_INIT:
      u8g_com_arduino_assign_pin_output_high(u8g);
      pinMode(PIN_SCK, OUTPUT);
      digitalWrite(PIN_SCK, LOW);
      pinMode(PIN_MOSI, OUTPUT);
      digitalWrite(PIN_MOSI, LOW);
      /* pinMode(PIN_MISO, INPUT); */

      pinMode(PIN_CS, OUTPUT);                  /* system chip select for the atmega board */
      digitalWrite(PIN_CS, HIGH);



      /*
        SPR1 SPR0
            0   0               fclk/4
            0   1               fclk/16
            1   0               fclk/64
            1   1               fclk/128
      */
      SPCR = 0;
      SPCR =  (1<<SPE) | (1<<MSTR)|(0<<SPR1)|(0<<SPR0)|(0<<CPOL)|(0<<CPHA);
#ifdef U8G_HW_SPI_2X
      SPSR = (1 << SPI2X);  /* double speed, issue 89 */
#else
      if ( arg_val  <= U8G_SPI_CLK_CYCLE_50NS )
      {
        SPSR = (1 << SPI2X);  /* double speed, issue 89 */
      }
#endif


      break;

    case U8G_COM_MSG_ADDRESS:                     /* define cmd (arg_val = 0) or data mode (arg_val = 1) */
      u8g_com_arduino_digital_write(u8g, U8G_PI_A0, arg_val);
      break;

    case U8G_COM_MSG_CHIP_SELECT:
      if ( arg_val == 0 )
      {
        /* disable */
        u8g_com_arduino_digital_write(u8g, U8G_PI_CS, HIGH);
      }
      else
      {
        /* enable */
        u8g_com_arduino_digital_write(u8g, U8G_PI_SCK, LOW);
        u8g_com_arduino_digital_write(u8g, U8G_PI_CS, LOW);
      }
      break;

    case U8G_COM_MSG_RESET:
      if ( u8g->pin_list[U8G_PI_RESET] != U8G_PIN_NONE )
        u8g_com_arduino_digital_write(u8g, U8G_PI_RESET, arg_val);
      break;

    case U8G_COM_MSG_WRITE_BYTE:
      u8g_spi_out(arg_val);
      break;

    case U8G_COM_MSG_WRITE_SEQ:
      {
        register uint8_t *ptr = arg_ptr;
        while( arg_val > 0 )
        {
          u8g_spi_out(*ptr++);
          arg_val--;
        }
      }
      break;
    case U8G_COM_MSG_WRITE_SEQ_P:
      {
        register uint8_t *ptr = arg_ptr;
        while( arg_val > 0 )
        {
          u8g_spi_out(u8g_pgm_read(ptr));
          ptr++;
          arg_val--;
        }
      }
      break;
  }
  return 1;
}

/* #elif defined(__18CXX) || defined(__PIC32MX) */

#elif defined(__SAM3X8E__)              // Arduino Due, maybe we should better check for __SAM3X8E__

#include <Arduino.h>

/* use Arduino pin definitions */
#define PIN_SCK SCK
#define PIN_MISO  MISO
#define PIN_MOSI MOSI
#define PIN_CS SS


static uint8_t u8g_spi_out(uint8_t data)
{
  /* wait until tx register is empty */
  while( (SPI0->SPI_SR & SPI_SR_TDRE) == 0 )
    ;
  /* send data */
  SPI0->SPI_TDR = (uint32_t)data;
  return  data;
}


uint8_t u8g_com_arduino_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  switch(msg)
  {
    case U8G_COM_MSG_STOP:
      break;

    case U8G_COM_MSG_INIT:
      u8g_com_arduino_assign_pin_output_high(u8g);
      u8g_com_arduino_digital_write(u8g, U8G_PI_CS, HIGH);

      /* Arduino Due specific code */

      /* enable PIOA and SPI0 */
      REG_PMC_PCER0 = (1UL << ID_PIOA) | (1UL << ID_SPI0);

      /* disable PIO on A26 and A27 */
      REG_PIOA_PDR = 0x0c000000;

      /* reset SPI0 (from sam lib) */
      SPI0->SPI_CR = SPI_CR_SPIDIS;
      SPI0->SPI_CR = SPI_CR_SWRST;
      SPI0->SPI_CR = SPI_CR_SWRST;
      SPI0->SPI_CR = SPI_CR_SPIEN;
      u8g_MicroDelay();

      /* master mode, no fault detection, chip select 0 */
      SPI0->SPI_MR = SPI_MR_MSTR | SPI_MR_PCSDEC | SPI_MR_MODFDIS;

      /* Polarity, Phase, 8 Bit data transfer, baud rate */
      /* x * 1000 / 84 --> clock cycle in ns
        5 * 1000 / 84 = 58 ns
        SCBR  > 50 *84 / 1000 --> SCBR=5
        SCBR  > 300*84 / 1000 --> SCBR=26
        SCBR  > 400*84 / 1000 --> SCBR=34
      */

      if ( arg_val <= U8G_SPI_CLK_CYCLE_50NS )
      {
        SPI0->SPI_CSR[0] = SPI_CSR_SCBR(5) | 1;
      }
      else if ( arg_val <= U8G_SPI_CLK_CYCLE_300NS )
      {
        SPI0->SPI_CSR[0] = SPI_CSR_SCBR(26) | 1;
      }
      else if ( arg_val <= U8G_SPI_CLK_CYCLE_400NS )
      {
        SPI0->SPI_CSR[0] = SPI_CSR_SCBR(34) | 1;
      }
      else
      {
        SPI0->SPI_CSR[0] = SPI_CSR_SCBR(84) | 1;
      }

      u8g_MicroDelay();
      break;

    case U8G_COM_MSG_ADDRESS:                     /* define cmd (arg_val = 0) or data mode (arg_val = 1) */
      u8g_com_arduino_digital_write(u8g, U8G_PI_A0, arg_val);
      u8g_MicroDelay();
      break;

    case U8G_COM_MSG_CHIP_SELECT:
      if ( arg_val == 0 )
      {
        /* disable */
        u8g_MicroDelay();               /* this delay is required to avoid that the display is switched off too early --> DOGS102 with DUE */
        u8g_com_arduino_digital_write(u8g, U8G_PI_CS, HIGH);
        u8g_MicroDelay();
      }
      else
      {
        /* enable */
        //u8g_com_arduino_digital_write(u8g, U8G_PI_SCK, LOW);
        u8g_com_arduino_digital_write(u8g, U8G_PI_CS, LOW);
        u8g_MicroDelay();
      }
      break;

    case U8G_COM_MSG_RESET:
      if ( u8g->pin_list[U8G_PI_RESET] != U8G_PIN_NONE )
        u8g_com_arduino_digital_write(u8g, U8G_PI_RESET, arg_val);
      break;

    case U8G_COM_MSG_WRITE_BYTE:
      u8g_spi_out(arg_val);
      u8g_MicroDelay();
      break;

    case U8G_COM_MSG_WRITE_SEQ:
      {
        register uint8_t *ptr = arg_ptr;
        while( arg_val > 0 )
        {
          u8g_spi_out(*ptr++);
          arg_val--;
        }
      }
      break;
    case U8G_COM_MSG_WRITE_SEQ_P:
      {
        register uint8_t *ptr = arg_ptr;
        while( arg_val > 0 )
        {
          u8g_spi_out(u8g_pgm_read(ptr));
          ptr++;
          arg_val--;
        }
      }
      break;
  }
  return 1;
}



#else /* U8G_ARDUINO_ATMEGA_HW_SPI */

#endif /* U8G_ARDUINO_ATMEGA_HW_SPI */

#else /* ARDUINO */

uint8_t u8g_com_arduino_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  return 1;
}

#endif /* ARDUINO */
