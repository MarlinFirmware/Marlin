/**
 * @file    fontutils.h
 * @brief   help functions for font and char
 * @author  Yunhui Fu (yhfudev@gmail.com)
 * @version 1.0
 * @date    2016-08-19
 * @copyright GPL/BSD
 */
#pragma once

#include <stdlib.h>
#include <stddef.h> // wchar_t
#include <stdint.h> // uint32_t

#include "../HAL/shared/Marduino.h"
#include "../core/macros.h"

// read a byte from ROM or RAM
typedef uint8_t (*read_byte_cb_t)(uint8_t * str);

uint8_t read_byte_ram(uint8_t * str);
uint8_t read_byte_rom(uint8_t * str);

// there's overflow of the wchar_t due to the 2-byte size in Arduino
// sizeof(wchar_t)=2; sizeof(size_t)=2; sizeof(uint32_t)=4;
// sizeof(int)=2; sizeof(long)=4; sizeof(unsigned)=2;
//#undef wchar_t
#define wchar_t uint32_t
//typedef uint32_t wchar_t;

typedef uint16_t pixel_len_t;
#define PIXEL_LEN_NOLIMIT ((pixel_len_t)(-1))

/* Perform binary search */
typedef int (* pf_bsearch_cb_comp_t)(void *userdata, size_t idx, void * data_pin); /*"data_list[idx] - *data_pin"*/
int pf_bsearch_r(void *userdata, size_t num_data, pf_bsearch_cb_comp_t cb_comp, void *data_pinpoint, size_t *ret_idx);

/* Get the character, decoding multibyte UTF8 characters and returning a pointer to the start of the next UTF8 character */
uint8_t* get_utf8_value_cb(uint8_t *pstart, read_byte_cb_t cb_read_byte, wchar_t *pval);

/* Returns lenght of string in CHARACTERS, NOT BYTES */
uint8_t utf8_strlen(const char *pstart);
uint8_t utf8_strlen_P(PGM_P pstart);
