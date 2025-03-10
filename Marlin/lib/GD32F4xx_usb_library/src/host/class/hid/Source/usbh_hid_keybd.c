/*!
    \file    usbh_hid_keybd.c
    \brief   USB host HID keyboard driver

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

#include "usbh_hid_keybd.h"
#include "usbh_hid_parser.h"
#include <stdbool.h>

hid_keybd_info keybd_info;

uint32_t keybd_report_data[2];

static const hid_report_item imp_0_lctrl =
{
    (uint8_t*)(void *)keybd_report_data + 0, /* data */
    1,     /* size */
    0,     /* shift */
    0,     /* count (only for array items) */
    0,     /* signed */
    0,     /* min value read can return */
    1,     /* max value read can return */
    0,     /* min vale device can report */
    1,     /* max value device can report */
    1      /* resolution */
};

static const hid_report_item imp_0_lshift =
{
    (uint8_t*)(void *)keybd_report_data + 0, /* data */
    1,     /* size */
    1,     /* shift */
    0,     /* count (only for array items) */
    0,     /* signed */
    0,     /* min value read can return */
    1,     /* max value read can return */
    0,     /* min vale device can report */
    1,     /* max value device can report */
    1      /* resolution */
};

static const hid_report_item imp_0_lalt = 
{
    (uint8_t*)(void *)keybd_report_data + 0, /* data */
    1,     /* size */
    2,     /* shift */
    0,     /* count (only for array items) */
    0,     /* signed */
    0,     /* min value read can return */
    1,     /* max value read can return */
    0,     /* min vale device can report */
    1,     /* max value device can report */
    1      /* resolution */
};

static const hid_report_item imp_0_lgui =
{
    (uint8_t*)(void *)keybd_report_data + 0, /* data */
    1,     /* size */
    3,     /* shift */
    0,     /* count (only for array items) */
    0,     /* signed */
    0,     /* min value read can return */
    1,     /* max value read can return */
    0,     /* min vale device can report */
    1,     /* max value device can report */
    1      /* resolution */
};

static const hid_report_item imp_0_rctrl =
{
    (uint8_t*)(void *)keybd_report_data + 0, /* data */
    1,     /* size */
    4,     /* shift */
    0,     /* count (only for array items) */
    0,     /* signed */
    0,     /* min value read can return */
    1,     /* max value read can return */
    0,     /* min vale device can report */
    1,     /* max value device can report */
    1      /* resolution */
};

static const hid_report_item imp_0_rshift = 
{
    (uint8_t*)(void *)keybd_report_data + 0, /* data */
    1,     /* size */
    5,     /* shift */
    0,     /* count (only for array items) */
    0,     /* signed */
    0,     /* min value read can return */
    1,     /* max value read can return */
    0,     /* min vale device can report */
    1,     /* max value device can report */
    1      /* resolution */
};

static const hid_report_item imp_0_ralt = 
{
    (uint8_t*)(void *)keybd_report_data + 0, /* data */
    1,     /* size */
    6,     /* shift */
    0,     /* count (only for array items) */
    0,     /* signed */
    0,     /* min value read can return */
    1,     /* max value read can return */
    0,     /* min vale device can report */
    1,     /* max value device can report */
    1      /* resolution */
};

static const hid_report_item imp_0_rgui = 
{
    (uint8_t*)(void *)keybd_report_data + 0, /* data */
    1,     /* size */
    7,     /* shift */
    0,     /* count (only for array items) */
    0,     /* signed */
    0,     /* min value read can return */
    1,     /* max value read can return */
    0,     /* min vale device can report */
    1,     /* max value device can report */
    1      /* resolution */
};

static const hid_report_item imp_0_key_array = 
{
    (uint8_t*)(void *)keybd_report_data + 2, /* data */
    8,     /* size */
    0,     /* shift */
    6,     /* count (only for array items) */
    0,     /* signed */
    0,     /* min value read can return */
    101,   /* max value read can return */
    0,     /* min vale device can report */
    101,   /* max value device can report */
    1      /* resolution */
};

/* local constants */
static const uint8_t hid_keybrd_codes[] = 
{
    0,      0,      0,      0,      31,     50,     48,     33, 
    19,     34,     35,     36,     24,     37,     38,     39,         /* 0x00 - 0x0F */
    52,     51,     25,     26,     17,     20,     32,     21,
    23,     49,     18,     47,     22,     46,     2,      3,          /* 0x10 - 0x1F */
    4,      5,      6,      7,      8,      9,      10,     11, 
    43,     110,    15,     16,     61,     12,     13,     27,         /* 0x20 - 0x2F */
    28,     29,     42,     40,     41,     1,      53,     54,  
    55,     30,     112,    113,    114,    115,    116,    117,        /* 0x30 - 0x3F */
    118,    119,    120,    121,    122,    123,    124,    125,  
    126,    75,     80,     85,     76,     81,     86,     89,         /* 0x40 - 0x4F */
    79,     84,     83,     90,     95,     100,    105,    106,
    108,    93,     98,     103,    92,     97,     102,    91,         /* 0x50 - 0x5F */
    96,     101,    99,     104,    45,     129,    0,      0, 
    0,      0,      0,      0,      0,      0,      0,      0,          /* 0x60 - 0x6F */
    0,      0,      0,      0,      0,      0,      0,      0,
    0,      0,      0,      0,      0,      0,      0,      0,          /* 0x70 - 0x7F */
    0,      0,      0,      0,      0,      107,    0,      56,
    0,      0,      0,      0,      0,      0,      0,      0,          /* 0x80 - 0x8F */
    0,      0,      0,      0,      0,      0,      0,      0,
    0,      0,      0,      0,      0,      0,      0,      0,          /* 0x90 - 0x9F */
    0,      0,      0,      0,      0,      0,      0,      0,
    0,      0,      0,      0,      0,      0,      0,      0,          /* 0xA0 - 0xAF */
    0,      0,      0,      0,      0,      0,      0,      0, 
    0,      0,      0,      0,      0,      0,      0,      0,          /* 0xB0 - 0xBF */
    0,      0,      0,      0,      0,      0,      0,      0, 
    0,      0,      0,      0,      0,      0,      0,      0,          /* 0xC0 - 0xCF */
    0,      0,      0,      0,      0,      0,      0,      0, 
    0,      0,      0,      0,      0,      0,      0,      0,          /* 0xD0 - 0xDF */
    58,     44,     60,     127,    64,     57,     62,     128         /* 0xE0 - 0xE7 */
};

#ifdef QWERTY_KEYBOARD

static const int8_t hid_keybrd_key[] = 
{
    '\0',   '`',    '1',    '2',    '3',    '4',    '5',    '6',
    '7',    '8',    '9',    '0',    '-',    '=',    '\0',   '\r',
    '\t',   'q',    'w',    'e',    'r',    't',    'y',    'u', 
    'i',    'o',    'p',    '[',    ']',    '\\',
    '\0',   'a',    's',    'd',    'f',    'g',    'h',    'j',
    'k',    'l',    ';',    '\'',   '\0',   '\n',
    '\0',   '\0',   'z',    'x',    'c',    'v',    'b',    'n',
    'm',    ',',    '.',    '/',    '\0',   '\0',
    '\0',   '\0',   '\0',   ' ',    '\0',   '\0',   '\0',   '\0',
    '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',
    '\0',   '\0',   '\0',   '\0',   '\0',   '\r',   '\0',   '\0', 
    '\0',   '\0',   '\0',   '\0',   '\0',   '\0',
    '\0',   '\0',   '7',    '4',    '1',
    '\0',   '/',    '8',    '5',    '2',
    '0',    '*',    '9',    '6',    '3',
    '.',    '-',    '+',    '\0',   '\n',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',
    '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0', 
    '\0',   '\0',   '\0',   '\0'
};

static const int8_t hid_keybrd_shiftkey[] = {
    '\0',   '~',    '!',    '@',    '#',    '$',    '%',    '^',    '&',    '*',    '(',    ')',
    '_',    '+',    '\0',   '\0',   '\0',   'Q',    'W',    'E',    'R',    'T',    'Y',    'U', 
    'I',    'O',    'P',    '{',    '}',    '|',    '\0',   'A',    'S',    'D',    'F',    'G', 
    'H',    'J',    'K',    'L',    ':',    '"',    '\0',   '\n',   '\0',   '\0',   'Z',    'X', 
    'C',    'V',    'B',    'N',    'M',    '<',    '>',    '?',    '\0',   '\0',   '\0',   '\0',
    '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0', 
    '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',
    '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0', 
    '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',
    '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',
    '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0'
};

#else

static const int8_t hid_keybrd_key[] = {
    '\0',   '`',    '1',    '2',    '3',    '4',    '5',    '6',    '7',    '8',    '9',    '0',
    '-',    '=',    '\0',   '\r',   '\t',   'a',    'z',    'e',    'r',    't',    'y',    'u', 
    'i',    'o',    'p',    '[',    ']',    '\\',   '\0',   'q',    's',    'd',    'f',    'g', 
    'h',    'j',    'k',    'l',    'm',    '\0',   '\0',   '\n',   '\0',   '\0',   'w',    'x', 
    'c',    'v',    'b',    'n',    ',',    ';',    ':',    '!',    '\0',   '\0',   '\0',   '\0', 
    '\0',   ' ',    '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0', 
    '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\r',   '\0',   '\0',   '\0', 
    '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '7',    '4',    '1',    '\0',   '/', 
    '8',    '5',    '2',    '0',    '*',    '9',    '6',    '3',    '.',    '-',    '+',    '\0', 
    '\n',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',
    '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0'
};

static const int8_t hid_keybrd_shiftkey[] = {
    '\0',   '~',    '!',    '@',    '#',    '$',    '%',    '^',    '&',    '*',    '(',    ')',    '_',
    '+',    '\0',   '\0',   '\0',   'A',    'Z',    'E',    'R',    'T',    'Y',    'U',    'I',    'O',
    'P',    '{',    '}',    '*',    '\0',   'Q',    'S',    'D',    'F',    'G',    'H',    'J',    'K',
    'L',    'M',    '%',    '\0',   '\n',   '\0',   '\0',   'W',    'X',    'C',    'V',    'B',    'N',
    '?',    '.',    '/',    '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',
    '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',
    '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',
    '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',
    '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',
    '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0'
};

#endif

/* local function prototypes ('static') */
static usbh_status usbh_hid_keybrd_decode (usb_core_driver *udev, usbh_host *uhost);

/*!
    \brief      initialize the keyboard function
    \param[in]  udev: pointer to USB core instance
    \param[in]  uhost: pointer to USB host
    \param[out] none
    \retval     operation status
*/
usbh_status usbh_hid_keybd_init (usb_core_driver *udev, usbh_host *uhost)
{
    usbh_hid_handler *hid = (usbh_hid_handler *)uhost->active_class->class_data;

    keybd_info.lctrl = keybd_info.lshift = 0U;
    keybd_info.lalt  = keybd_info.lgui   = 0U;
    keybd_info.rctrl = keybd_info.rshift = 0U;
    keybd_info.ralt  = keybd_info.rgui   = 0U;

    for (uint32_t x = 0U; x < (sizeof(keybd_report_data) / sizeof(uint32_t)); x++) {
        keybd_report_data[x] = 0U;
    }

    if (hid->len > (sizeof(keybd_report_data) / sizeof(uint32_t))) {
        hid->len = (sizeof(keybd_report_data) / sizeof(uint32_t));
    }

    hid->pdata = (uint8_t*)(void *)keybd_report_data;

    usbh_hid_fifo_init (&hid->fifo, uhost->dev_prop.data, HID_QUEUE_SIZE * sizeof(keybd_report_data));

    /* call user initialization*/
    usr_keybrd_init();

    return USBH_OK;
}

/*!
    \brief      get keyboard information
    \param[in]  udev: pointer to USB core instance
    \param[in]  uhost: pointer to USB host handler
    \param[out] none
    \retval     keyboard information
*/
hid_keybd_info *usbh_hid_keybd_info_get (usb_core_driver *udev, usbh_host *uhost)
{
    if (USBH_OK == usbh_hid_keybrd_decode(udev, uhost)) {
        return &keybd_info;
    } else {
        return NULL;
    }
}

/*!
    \brief      get ascii code
    \param[in]  info: keyboard information
    \param[out] none
    \retval     output
*/
uint8_t usbh_hid_ascii_code_get (hid_keybd_info *info)
{
    uint8_t output;
    if ((1U == info->lshift) || (info->rshift)) {
        output = hid_keybrd_shiftkey[hid_keybrd_codes[info->keys[0]]];
    } else {
        output = hid_keybrd_key[hid_keybrd_codes[info->keys[0]]];
    }

    return output;
}

/*!
    \brief      decode the pressed keys
    \param[in]  udev: pointer to USB core instance
    \param[in]  uhost: pointer to USB host
    \param[out] none
    \retval     none
*/
void usbh_hid_keybrd_machine (usb_core_driver *udev, usbh_host *uhost)
{
    hid_keybd_info *k_pinfo;

    k_pinfo = usbh_hid_keybd_info_get(udev, uhost);

    if (k_pinfo != NULL) {
        char c = usbh_hid_ascii_code_get(k_pinfo);

        if (c != 0U) {
            usr_keybrd_process_data(c);
        }
    }
}

/*!
    \brief      decode keyboard information
    \param[in]  udev: pointer to USB core instance
    \param[in]  uhost: pointer to USB host
    \param[out] none
    \retval     operation status
*/
static usbh_status usbh_hid_keybrd_decode (usb_core_driver *udev, usbh_host *uhost)
{
    usbh_hid_handler *hid = (usbh_hid_handler *)uhost->active_class->class_data;

    if (hid->len == 0U) {
        return USBH_FAIL;
    }

    /* fill report */
    if (usbh_hid_fifo_read (&hid->fifo, &keybd_report_data, hid->len) == hid->len) {
        keybd_info.lctrl  = (uint8_t)hid_item_read((hid_report_item *)&imp_0_lctrl, 0U);
        keybd_info.lshift = (uint8_t)hid_item_read((hid_report_item *)&imp_0_lshift, 0U);
        keybd_info.lalt   = (uint8_t)hid_item_read((hid_report_item *)&imp_0_lalt, 0U);
        keybd_info.lgui   = (uint8_t)hid_item_read((hid_report_item *)&imp_0_lgui, 0U);
        keybd_info.rctrl  = (uint8_t)hid_item_read((hid_report_item *)&imp_0_rctrl, 0U);
        keybd_info.rshift = (uint8_t)hid_item_read((hid_report_item *)&imp_0_rshift, 0U);
        keybd_info.ralt   = (uint8_t)hid_item_read((hid_report_item *)&imp_0_ralt, 0U);
        keybd_info.rgui   = (uint8_t)hid_item_read((hid_report_item *)&imp_0_rgui, 0U);

        for (uint8_t x = 0U; x < sizeof(keybd_info.keys); x++) {
            keybd_info.keys[x] = (uint8_t)hid_item_read((hid_report_item *)&imp_0_key_array, x);
        }

        return USBH_OK;
    }

    return USBH_FAIL;
}
