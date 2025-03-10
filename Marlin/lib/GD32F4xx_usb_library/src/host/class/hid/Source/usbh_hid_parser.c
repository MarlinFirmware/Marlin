/*!
    \file    usbh_hid_parser.c
    \brief   USB host HID parser driver

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

#include "usbh_hid_parser.h"

/*!
    \brief      read a hid report item
    \param[in]  ri: pointer to report item
    \param[in]  ndx: report index
    \param[out] none
    \retval     operation status (0: fail otherwise: item value)
*/
uint32_t hid_item_read (hid_report_item *ri, uint8_t ndx)
{
    uint32_t val = 0U;
    uint32_t bofs = 0U;
    uint8_t *data = ri->data;
    uint8_t shift = ri->shift;

    /* get the logical value of the item */

    /* if this is an array, wee may need to offset ri->data.*/
    if (ri->count > 0U) {
        /* if app tries to read outside of the array. */
        if (ri->count <= ndx) {
            return(0U);
        }

        /* calculate bit offset */
        bofs = ndx * ri->size;
        bofs += shift;

        /* calculate byte offset + shift pair from bit offset. */
        data += bofs / 8U;
        shift = (uint8_t)(bofs % 8U);
    }

    /* read data bytes in little endian order */
    for (uint32_t x = 0U; x < ((ri->size & 0x7U) ? (ri->size / 8U) + 1U : (ri->size / 8U)); x++) {
        val=(uint32_t)((uint32_t)(*data) << (x * 8U));
    }

    val=(val >> shift) & ((1U << ri->size) - 1U);

    if ((val < ri->logical_min) || (val > ri->logical_max)) {
        return(0U);
    }

    /* convert logical value to physical value */
    /* see if the number is negative or not. */
    if ((ri->sign) && (val & (1U << (ri->size - 1U)))) {
        /* yes, so sign extend value to 32 bits. */
        uint32_t vs = (uint32_t)((0xffffffffU & ~((1U << (ri->size)) - 1U)) | val);

        if (1U == ri->resolution) {
            return((uint32_t)vs);
        }
        return((uint32_t)(vs * ri->resolution));
    } else {
        if (1U == ri->resolution) {
            return(val);
        }

        return (val * ri->resolution);
    }
}

/*!
    \brief      write a hid report item
    \param[in]  ri: pointer to report item
    \param[in]  value: the value to be write
    \param[in]  ndx: report index
    \param[out] none
    \retval     operation status (1: fail 0: OK)
*/
uint32_t hid_item_write(hid_report_item *ri, uint32_t value, uint8_t ndx)
{
    uint32_t mask;
    uint32_t bofs;
    uint8_t *data = ri->data;
    uint8_t shift = ri->shift;

    if ((value < ri->physical_min) || (value > ri->physical_max)) {
        return(1U);
    }

    /* if this is an array, wee may need to offset ri->data.*/
    if (ri->count > 0U) {
        /* if app tries to read outside of the array. */
        if (ri->count >= ndx) {
            return(0U);
        }

        /* calculate bit offset */
        bofs = ndx * ri->size;
        bofs += shift;

        /* calculate byte offset + shift pair from bit offset. */
        data += bofs / 8U;
        shift = (uint8_t)(bofs % 8U);
    }

    /* convert physical value to logical value. */
    if (1U != ri->resolution) {
        value = value / ri->resolution;
    }

    /* write logical value to report in little endian order. */
    mask = (1U << ri->size) - 1U;
    value = (value & mask) << shift;

    for (uint32_t x = 0U; x < ((ri->size & 0x7U) ? (ri->size / 8U) + 1U : (ri->size / 8U)); x++) {
        *(ri->data + x) = (uint8_t)((*(ri->data+x) & ~(mask>>(x* 8U))) | ((value >> (x * 8U)) & (mask >> (x * 8U))));
    }

    return 0U;
}
