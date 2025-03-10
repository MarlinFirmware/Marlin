/*!
    \file    usbd_msc_mem.h
    \brief   header file for storage memory

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

#ifndef __USBD_MSC_MEM_H
#define __USBD_MSC_MEM_H

#include "usbd_conf.h"

#define USBD_STD_INQUIRY_LENGTH     36U

typedef struct
{
    int8_t (*mem_init)         (uint8_t lun);
    int8_t (*mem_ready)        (uint8_t lun);
    int8_t (*mem_protected)    (uint8_t lun);
    int8_t (*mem_read)         (uint8_t lun, uint8_t *buf, uint32_t block_addr, uint16_t block_len);
    int8_t (*mem_write)        (uint8_t lun, uint8_t *buf, uint32_t block_addr, uint16_t block_len);
    int8_t (*mem_maxlun)       (void);

    uint8_t *mem_toc_data;
    uint8_t *mem_inquiry_data[MEM_LUN_NUM];
    uint32_t mem_block_size[MEM_LUN_NUM];
    uint32_t mem_block_len[MEM_LUN_NUM];
}usbd_mem_cb;

extern usbd_mem_cb *usbd_mem_fops;

#endif /* __USBD_MSC_MEM_H */
