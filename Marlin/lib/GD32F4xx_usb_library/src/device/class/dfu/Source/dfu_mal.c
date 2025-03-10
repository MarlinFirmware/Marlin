/*!
    \file    dfu_mal.c
    \brief   USB DFU device media access layer functions

    \version 2020-08-01, V3.0.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "dfu_mal.h"
#include "flash_if.h"
#include "drv_usb_hw.h"
#include "usbd_transc.h"

extern usb_core_driver usb_dfu_dev;

extern struct {
    uint8_t buf[TRANSFER_SIZE];
    uint16_t data_len;
    uint16_t block_num;
    uint32_t base_addr;
} prog;

dfu_mal_prop* tMALTab[MAX_USED_MEMORY_MEDIA] = {
    &DFU_Flash_cb
};

/* The list of memory interface string descriptor pointers. This list
   can be updated whenever a memory has to be added or removed */
const uint8_t* USBD_DFU_StringDesc[MAX_USED_MEMORY_MEDIA] = 
{
    (const uint8_t *)FLASH_IF_STRING
};

static uint8_t dfu_mal_checkaddr (uint32_t addr);

/*!
    \brief      initialize the memory media on the GD32
    \param[in]  none
    \param[out] none
    \retval     MAL_OK
*/
uint8_t dfu_mal_init (void)
{
    uint32_t mem_index = 0U;

    /* initialize all supported memory medias */
    for (mem_index = 0U; mem_index < MAX_USED_MEMORY_MEDIA; mem_index++) {
        /* check if the memory media exists */
        if (NULL != tMALTab[mem_index]->mal_init) {
            tMALTab[mem_index]->mal_init();
        }
    }

    return MAL_OK;
}

/*!
    \brief      deinitialize the memory media on the GD32
    \param[in]  none
    \param[out] none
    \retval     MAL_OK
*/
uint8_t dfu_mal_deinit (void)
{
    uint32_t mem_index = 0U;

    /* deinitialize all supported memory medias */
    for (mem_index = 0U; mem_index < MAX_USED_MEMORY_MEDIA; mem_index++) {
        /* check if the memory media exists */
        if (NULL != tMALTab[mem_index]->mal_deinit) {
            tMALTab[mem_index]->mal_deinit();
        }
    }

    return MAL_OK;
}

/*!
    \brief      erase a memory sector
    \param[in]  addr: memory sector address/code
    \param[out] none
    \retval     MAL_OK
*/
uint8_t dfu_mal_erase (uint32_t addr)
{
    uint32_t mem_index = dfu_mal_checkaddr(addr);

    /* check if the address is in protected area */
    if (IS_PROTECTED_AREA(addr)) {
        return MAL_FAIL;
    }

    if (mem_index < MAX_USED_MEMORY_MEDIA) {
        /* check if the operation is supported */
        if (NULL != tMALTab[mem_index]->mal_erase) {
            return tMALTab[mem_index]->mal_erase(addr);
        } else {
            return MAL_FAIL;
        }
    } else {
        return MAL_FAIL;
    }
}

/*!
    \brief      write data to sectors of memory
    \param[in]  buf: the data buffer to be write
    \param[in]  addr: memory sector address/code
    \param[in]  len: data length
    \param[out] none
    \retval     MAL_OK
*/
uint8_t dfu_mal_write (uint8_t *buf, uint32_t addr, uint32_t len)
{
    uint32_t mem_index = dfu_mal_checkaddr(addr);

    /* check if the address is in protected area */
    if (IS_PROTECTED_AREA(addr)) {
        return MAL_FAIL;
    }

    if (mem_index < MAX_USED_MEMORY_MEDIA) {
        /* check if the operation is supported */
        if (NULL != tMALTab[mem_index]->mal_write) {
            return tMALTab[mem_index]->mal_write(buf, addr, len);
        } else {
            return MAL_FAIL;
        }
    } else {
        return MAL_FAIL;
    }
}

/*!
    \brief      read data from sectors of memory
    \param[in]  buf: the data buffer to be write
    \param[in]  addr: memory sector address/code
    \param[in]  len: data length
    \param[out] none
    \retval     pointer to buffer
*/
uint8_t* dfu_mal_read (uint8_t *buf, uint32_t addr, uint32_t len)
{
    uint32_t mem_index = 0U;

    if (OB_RDPT != addr) {
        mem_index = dfu_mal_checkaddr(addr);
    }

    if (mem_index < MAX_USED_MEMORY_MEDIA) {
        /* check if the operation is supported */
        if (NULL != tMALTab[mem_index]->mal_read) {
            return tMALTab[mem_index]->mal_read(buf, addr, len);
        } else {
            return buf;
        }
    } else {
        return buf;
    }
}

/*!
    \brief      get the status of a given memory and store in buffer
    \param[in]  addr: memory sector address/code
    \param[in]  cmd: 0 for erase and 1 for write
    \param[in]  buffer: pointer to the buffer where the status data will be stored
    \param[out] none
    \retval     MAL_OK if all operations are OK, MAL_FAIL else
*/
uint8_t dfu_mal_getstatus (uint32_t addr, uint8_t cmd, uint8_t *buffer)
{
    uint32_t mem_index = dfu_mal_checkaddr(addr);

    if (mem_index < MAX_USED_MEMORY_MEDIA) {
        if (cmd & 0x01U) {
            SET_POLLING_TIMEOUT(tMALTab[mem_index]->write_timeout);
        } else {
            SET_POLLING_TIMEOUT(tMALTab[mem_index]->erase_timeout);
        }

        return MAL_OK;
    } else {
        return MAL_FAIL;
    }
}

/*!
    \brief      check the address is supported
    \param[in]  addr: memory sector address/code
    \param[out] none
    \retval     index of the addressed memory
*/
static uint8_t dfu_mal_checkaddr (uint32_t addr)
{
    uint8_t mem_index = 0U;

    /* check with all supported memories */
    for (mem_index = 0U; mem_index < MAX_USED_MEMORY_MEDIA; mem_index++) {
        /* if the check address is supported, return the memory index */
        if (MAL_OK == tMALTab[mem_index]->mal_checkaddr(addr)) {
            return mem_index;
        }
    }

    /* if there is no memory found, return MAX_USED_MEMORY_MEDIA */
    return (MAX_USED_MEMORY_MEDIA);
}
