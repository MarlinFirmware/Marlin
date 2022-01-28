//
// PNG Decoder
//
// written by Larry Bank
// bitbank@pobox.com
// Arduino port started 5/3/2021
// Original PNG code written 20+ years ago :)
// The goal of this code is to decode PNG images on embedded systems
//
// Copyright 2021 BitBank Software, Inc. All Rights Reserved.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//    http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//===========================================================================
//
#ifndef __PNGDEC__
#define __PNGDEC__
#if defined( __MACH__ ) || defined( __LINUX__ ) || defined( __MCUXPRESSO )
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#define memcpy_P memcpy
#define PROGMEM
#else

// #define uint32_t	unsigned int
// #define uint16_t	unsigned short
// #define uint8_t		unsigned char
// #define int32_t		signed int
// #define int16_t		signed short
// #define int8_t		signed char
// #define __builtin_bswap16(x) (x<<8 | x>>8)

//#include <Arduino.h>
#endif
#include "zutil.h"
#include "inftrees.h"
#include "inflate.h"
//
// PNG Decoder
// Written by Larry Bank
// Copyright (c) 2021 BitBank Software, Inc.
// 
// Designed to decode most PNG images (1-32 bpp)
// using less than 40K of RAM
//
#ifndef FALSE
#define FALSE 0
#define TRUE 1
#endif
/* Defines and variables */
#define PNG_FILE_BUF_SIZE 2048
// Number of bytes to reserve for current and previous lines
// Defaults to 640 32-bit pixels max width
#define PNG_MAX_BUFFERED_PIXELS (640*4 + 1)
// PNG filter type
enum {
    PNG_FILTER_NONE=0,
    PNG_FILTER_SUB,
    PNG_FILTER_UP,
    PNG_FILTER_AVG,
    PNG_FILTER_PAETH,
    PNG_FILTER_COUNT
};

// decode options
enum {
    PNG_CHECK_CRC = 1,
    PNG_FAST_PALETTE = 2
};

// source pixel type
enum {
  PNG_PIXEL_GRAYSCALE=0,
    PNG_PIXEL_TRUECOLOR=2,
    PNG_PIXEL_INDEXED=3,
    PNG_PIXEL_GRAY_ALPHA=4,
    PNG_PIXEL_TRUECOLOR_ALPHA=6
};
// RGB565 endianness
enum {
    PNG_RGB565_LITTLE_ENDIAN = 0,
    PNG_RGB565_BIG_ENDIAN
};

enum {
    PNG_MEM_RAM=0,
    PNG_MEM_FLASH
};

// Error codes returned by getLastError()
enum {
    PNG_SUCCESS = 0,
    PNG_INVALID_PARAMETER,
    PNG_DECODE_ERROR,
    PNG_MEM_ERROR,
    PNG_NO_BUFFER,
    PNG_UNSUPPORTED_FEATURE,
    PNG_INVALID_FILE,
    PNG_TOO_BIG
};

typedef struct png_draw_tag
{
    int y; // starting x,y of this line
    int iWidth; // size of this line
    int iPitch; // bytes per line
    int iPixelType; // PNG pixel type (0,2,3,4,6)
    int iBpp; // bits per color stimulus
    void *pUser; // user supplied pointer
    uint8_t *pPalette;
    uint16_t *pFastPalette;
    uint8_t *pPixels;
} PNGDRAW;

typedef struct png_file_tag
{
  int32_t iPos; // current file position
  int32_t iSize; // file size
  uint8_t *pData; // memory file pointer
  void * fHandle; // class pointer to File/SdFat or whatever you want
} PNGFILE;

// Callback function prototypes
typedef int32_t (PNG_READ_CALLBACK)(PNGFILE *pFile, uint8_t *pBuf, int32_t iLen);
typedef int32_t (PNG_SEEK_CALLBACK)(PNGFILE *pFile, int32_t iPosition);
typedef void * (PNG_OPEN_CALLBACK)(const char *szFilename, int32_t *pFileSize);
typedef void (PNG_DRAW_CALLBACK)(PNGDRAW *);
typedef void (PNG_CLOSE_CALLBACK)(void *pHandle);

//
// our private structure to hold a JPEG image decode state
//
typedef struct png_image_tag
{
    int iWidth, iHeight; // image size
    uint8_t ucBpp, ucPixelType;
    uint8_t ucMemType;
    uint8_t *pImage;
    int iPitch; // bytes per line
    int iHasAlpha;
    int iInterlaced;
    uint32_t iTransparent; // transparent color index/value
    int iError;
    PNG_READ_CALLBACK *pfnRead;
    PNG_SEEK_CALLBACK *pfnSeek;
    PNG_OPEN_CALLBACK *pfnOpen;
    PNG_DRAW_CALLBACK *pfnDraw;
    PNG_CLOSE_CALLBACK *pfnClose;
    PNGFILE PNGFile;
    uint8_t ucZLIB[32768 + sizeof(inflate_state)]; // put this here to avoid needing malloc/free
    uint8_t ucPalette[1024];
    uint8_t ucPixels[PNG_MAX_BUFFERED_PIXELS * 2];
    uint8_t ucFileBuf[PNG_FILE_BUF_SIZE]; // holds temp file data
} PNGIMAGE;

extern PNGIMAGE _png;
//PNGIMAGE __pngimg __attribute__ ((section (".ccmram")));

#ifdef __cplusplus
#define PNG_STATIC static
//
// The PNG class wraps portable C code which does the actual work
//
class PNG
{
  public:
    int openRAM(uint8_t *pData, int iDataSize, PNG_DRAW_CALLBACK *pfnDraw);
    int openFLASH(uint8_t *pData, int iDataSize, PNG_DRAW_CALLBACK *pfnDraw);
    int open(const char *szFilename, PNG_OPEN_CALLBACK *pfnOpen, PNG_CLOSE_CALLBACK *pfnClose, PNG_READ_CALLBACK *pfnRead, PNG_SEEK_CALLBACK *pfnSeek, PNG_DRAW_CALLBACK *pfnDraw);
    void close();
    int decode(void *pUser, int iOptions);
    int getWidth();
    int getHeight();
    int getBpp();
    int hasAlpha();
    uint32_t getTransparentColor();
    int isInterlaced();
    uint8_t * getPalette();
    int getPixelType();
    int getLastError();
    int getBufferSize();
    uint8_t *getBuffer();
    void setBuffer(uint8_t *pBuffer);
    uint8_t getAlphaMask(PNGDRAW *pDraw, uint8_t *pMask, uint8_t ucThreshold);
    void getLineAsRGB565(PNGDRAW *pDraw, uint16_t *pPixels, int iEndianness, uint32_t u32Bkgd);

  private:
//    static PNGIMAGE _png __attribute__ ((section (".ccmram")));
};
#else
#define PNG_STATIC
int PNG_openRAM(PNGIMAGE *pPNG, uint8_t *pData, int iDataSize);
int PNG_openFile(PNGIMAGE *pPNG, const char *szFilename);
int PNG_getWidth(PNGIMAGE *pPNG);
int PNG_getHeight(PNGIMAGE *pPNG);
int PNG_decode(PNGIMAGE *pPNG, void *pUser, int iOptions);
void PNG_close(PNGIMAGE *pPNG);
int PNG_getLastError(PNGIMAGE *pPNG);
int PNG_getBpp(PNGIMAGE *pPNG);
int PNG_getLastError(PNGIMAGE *pPNG);
int PNG_getBufferSize(PNGIMAGE *pPNG);
uint8_t *PNG_getPalette(PNGIMAGE *pPNG);
int PNG_getPixelType(PNGIMAGE *pPNG);
int PNG_hasAlpha(PNGIMAGE *pPNG);
int PNG_isInterlaced(PNGIMAGE *pPNG);
uint8_t *PNG_getBuffer(PNGIMAGE *pPNG);
void PNG_setBuffer(PNGIMAGE *pPNG, uint8_t *pBuffer);
#endif // __cplusplus

// Due to unaligned memory causing an exception, we have to do these macros the slow way
#define INTELSHORT(p) ((*p) + (*(p+1)<<8))
#define INTELLONG(p) ((*p) + (*(p+1)<<8) + (*(p+2)<<16) + (*(p+3)<<24))
#define MOTOSHORT(p) (((*(p))<<8) + (*(p+1)))
#define MOTOLONG(p) (((*p)<<24) + ((*(p+1))<<16) + ((*(p+2))<<8) + (*(p+3)))

// Must be a 32-bit target processor
#define REGISTER_WIDTH 32

#endif // __PNGDEC__
