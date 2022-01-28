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
#include "PNGdec.h"

// forward references
PNG_STATIC int PNGInit(PNGIMAGE *pPNG);
PNG_STATIC int DecodePNG(PNGIMAGE *pImage, void *pUser, int iOptions);
PNG_STATIC uint8_t PNGMakeMask(PNGDRAW *pDraw, uint8_t *pMask, uint8_t ucThreshold);
// Include the C code which does the actual work
#include "png.inl"

//
// Memory initialization
//
int PNG::openRAM(uint8_t *pData, int iDataSize, PNG_DRAW_CALLBACK *pfnDraw)
{
    memset(&_png, 0, sizeof(PNGIMAGE));
    _png.ucMemType = PNG_MEM_RAM;
    _png.pfnRead = readRAM;
    _png.pfnSeek = seekMem;
    _png.pfnDraw = pfnDraw;
    _png.pfnOpen = NULL;
    _png.pfnClose = NULL;
    _png.PNGFile.iSize = iDataSize;
    _png.PNGFile.pData = pData;
    return PNGInit(&_png);
} /* openRAM() */
//
// It's necessary to separate out a FLASH version on Harvard architecture machines
//
int PNG::openFLASH(uint8_t *pData, int iDataSize, PNG_DRAW_CALLBACK *pfnDraw)
{
    memset(&_png, 0, sizeof(PNGIMAGE));
    _png.ucMemType = PNG_MEM_FLASH;
    _png.pfnRead = readFLASH;
    _png.pfnSeek = seekMem;
    _png.pfnDraw = pfnDraw;
    _png.pfnOpen = NULL;
    _png.pfnClose = NULL;
    _png.PNGFile.iSize = iDataSize;
    _png.PNGFile.pData = pData;
    return PNGInit(&_png);
} /* openRAM() */

//
// File (SD/MMC) based initialization
//
int PNG::open(const char *szFilename, PNG_OPEN_CALLBACK *pfnOpen, PNG_CLOSE_CALLBACK *pfnClose, PNG_READ_CALLBACK *pfnRead, PNG_SEEK_CALLBACK *pfnSeek, PNG_DRAW_CALLBACK *pfnDraw)
{
    memset(&_png, 0, sizeof(PNGIMAGE));
    _png.pfnRead = pfnRead;
    _png.pfnSeek = pfnSeek;
    _png.pfnDraw = pfnDraw;
    _png.pfnOpen = pfnOpen;
    _png.pfnClose = pfnClose;
    _png.PNGFile.fHandle = (*pfnOpen)(szFilename, &_png.PNGFile.iSize);
    if (_png.PNGFile.fHandle == NULL)
       return 0;
    return PNGInit(&_png);

} /* open() */
//
// return the last error (if any)
//
int PNG::getLastError()
{
    return _png.iError;
} /* getLastError() */
//
// Get the width of the image in pixels
// can be called after opening the file (before decoding)
//
int PNG::getWidth()
{
    return _png.iWidth;
} /* getWidth() */
//
// Get the height of the image in pixels
// can be called after opening the file (before decoding)
//
int PNG::getHeight()
{
    return _png.iHeight;
} /* getHeight() */
//
// For truecolor and palette images, it's possible to have a single
// transparent color defined. This call will return it if defined
//
uint32_t PNG::getTransparentColor()
{
    return _png.iTransparent;
} /* getTransparentColor() */
//
// Alpha information can be per pixel, per color or a single color
// depending on the PNG pixel type of the image
// This call simply tells you if there is alpha for the current pixel type
//
int PNG::hasAlpha()
{
    return _png.iHasAlpha;
} /* hasAlpha() */
//
// Returns true or false for the use of Adam7 interlacing
// This option is not supported by the decoder, but after opening the image
// you can determine if it's set
//
int PNG::isInterlaced()
{
    return _png.iInterlaced;
} /* isInterlaced() */

//
// Returns the number of bits per color stimulus
// values of 1,2,4, and 8 are supported
//
int PNG::getBpp()
{
    return (int)_png.ucBpp;
} /* getBpp() */
//
// Returns the PNG pixel type (see enum in PNGdec.h)
//
int PNG::getPixelType()
{
    return (int)_png.ucPixelType;
} /* getPixelType() */
//
// Set the image buffer to memory managed by the caller
// If set, decode() will not use the PNGDRAW callback function
// and instead write the image into this buffer in one shot
//
void PNG::setBuffer(uint8_t *pBuffer)
{
    _png.pImage = pBuffer;
} /* setBuffer() */
//
// Returns the previously set image buffer or NULL if there is none
//
uint8_t * PNG::getBuffer()
{
    return _png.pImage;
} /* getBuffer() */
//
// Returns the size in bytes of the buffer needed to hold the uncompressed image
//
int PNG::getBufferSize()
{
    return _png.iHeight * _png.iPitch;
} /* getBufferSize() */
//
// Returns a pointer to the palette
// If there is alpha info for the palette, it starts at pPalette[768]
//
uint8_t * PNG::getPalette()
{
    return _png.ucPalette;
} /* getPalette() */
//
// Close the file - not needed when decoding from memory
//
void PNG::close()
{
    if (_png.pfnClose)
        (*_png.pfnClose)(_png.PNGFile.fHandle);
} /* close() */

//
// Decode the image
// returns:
// 0 = PNG_SUCCESS
// non 0 = PNG enumerated error code
//
int PNG::decode(void *pUser, int iOptions)
{
    return DecodePNG(&_png, pUser, iOptions);
} /* decode() */
//
// Convert a line of native pixels (all supported formats) into RGB565
// can optionally mix in a background color - set to -1 to disable
// Background color is in the form of a uint32_t -> 00BBGGRR (MSB on left)
//
void PNG::getLineAsRGB565(PNGDRAW *pDraw, uint16_t *pPixels, int iEndianness, uint32_t u32Bkgd)
{
    PNGRGB565(pDraw, pPixels, iEndianness, u32Bkgd, hasAlpha());
} /* getLineAsRGB565() */

uint8_t PNG::getAlphaMask(PNGDRAW *pDraw, uint8_t *pMask, uint8_t ucThreshold)
{
    return PNGMakeMask(pDraw, pMask, ucThreshold);
} /* getAlphaMask() */
