/*

  u8g_state.c
  
  backup and restore hardware state

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
  

  state callback: backup env U8G_STATE_MSG_BACKUP_ENV
  device callback: DEV_MSG_INIT
  state callback: backup u8g U8G_STATE_MSG_BACKUP_U8G
  state callback: restore env U8G_STATE_MSG_RESTORE_ENV

  state callback: backup env U8G_STATE_MSG_BACKUP_ENV
  state callback: retore u8g U8G_STATE_MSG_RESTORE_U8G
  DEV_MSG_PAGE_FIRST or DEV_MSG_PAGE_NEXT
  state callback: restore env U8G_STATE_MSG_RESTORE_ENV

*/

#include <stddef.h>
#include "u8g.h"

void u8g_state_dummy_cb(uint8_t msg)
{
  /* the dummy procedure does nothing */
}

void u8g_SetHardwareBackup(u8g_t *u8g, u8g_state_cb backup_cb)
{
  u8g->state_cb = backup_cb;
  /* in most cases the init message was already sent, so this will backup the */
  /* current u8g state */
  backup_cb(U8G_STATE_MSG_BACKUP_U8G);
}


/*===============================================================*/
/* register variable for restoring interrupt state */

#if defined(__AVR__)
uint8_t global_SREG_backup;
#endif



/*===============================================================*/
/* AVR */

#if defined(__AVR__)
#define U8G_ATMEGA_HW_SPI

/* remove the definition for attiny */
#if __AVR_ARCH__ == 2
#undef U8G_ATMEGA_HW_SPI
#endif
#if __AVR_ARCH__ == 25
#undef U8G_ATMEGA_HW_SPI
#endif
#endif

#if defined(U8G_ATMEGA_HW_SPI)
#include <avr/interrupt.h>
static uint8_t u8g_state_avr_spi_memory[2];

void u8g_backup_spi(uint8_t msg)
{
  if ( U8G_STATE_MSG_IS_BACKUP(msg) )
  {
    u8g_state_avr_spi_memory[U8G_STATE_MSG_GET_IDX(msg)] = SPCR;
  }
  else
  {
    uint8_t tmp = SREG;
    cli();
    SPCR = 0;
    SPCR = u8g_state_avr_spi_memory[U8G_STATE_MSG_GET_IDX(msg)];
    SREG = tmp;
  }
}

#elif defined(ARDUINO) && defined(__SAM3X8E__)		// Arduino Due, maybe we should better check for __SAM3X8E__

#include "sam.h"

struct sam_backup_struct
{
  uint32_t mr;
  uint32_t sr;
  uint32_t csr[4];  
} sam_backup[2];

void u8g_backup_spi(uint8_t msg)
{
  uint8_t idx = U8G_STATE_MSG_GET_IDX(msg);
  if ( U8G_STATE_MSG_IS_BACKUP(msg) )
  {
    sam_backup[idx].mr = SPI0->SPI_MR;
    sam_backup[idx].sr = SPI0->SPI_SR;
    sam_backup[idx].csr[0] = SPI0->SPI_CSR[0];
    sam_backup[idx].csr[1] = SPI0->SPI_CSR[1];
    sam_backup[idx].csr[2] = SPI0->SPI_CSR[2];
    sam_backup[idx].csr[3] = SPI0->SPI_CSR[3];
  }
  else
  {
    SPI0->SPI_MR = sam_backup[idx].mr;
    SPI0->SPI_CSR[0] = sam_backup[idx].csr[0];
    SPI0->SPI_CSR[1] = sam_backup[idx].csr[1];
    SPI0->SPI_CSR[2] = sam_backup[idx].csr[2];
    SPI0->SPI_CSR[3] = sam_backup[idx].csr[3];
  }
}

#else

void u8g_backup_spi(uint8_t msg)
{
}

#endif

