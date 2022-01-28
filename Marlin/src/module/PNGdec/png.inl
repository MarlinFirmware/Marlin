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
#include "zlib.h"
//
// Convert 8-bit grayscale into RGB565
//
static const uint16_t usGrayTo565[] = {0x0000,0x0000,0x0000,0x0000,0x0020,0x0020,0x0020,0x0020, // 0
    0x0841,0x0841,0x0841,0x0841,0x0861,0x0861,0x0861,0x0861,
    0x1082,0x1082,0x1082,0x1082,0x10a2,0x10a2,0x10a2,0x10a2,
    0x18c3,0x18c3,0x18c3,0x18c3,0x18e3,0x18e3,0x18e3,0x18e3,
    0x2104,0x2104,0x2104,0x2104,0x2124,0x2124,0x2124,0x2124,
    0x2945,0x2945,0x2945,0x2945,0x2965,0x2965,0x2965,0x2965,
    0x3186,0x3186,0x3186,0x3186,0x31a6,0x31a6,0x31a6,0x31a6,
    0x39c7,0x39c7,0x39c7,0x39c7,0x39e7,0x39e7,0x39e7,0x39e7,
    0x4208,0x4208,0x4208,0x4208,0x4228,0x4228,0x4228,0x4228,
    0x4a49,0x4a49,0x4a49,0x4a49,0x4a69,0x4a69,0x4a69,0x4a69,
    0x528a,0x528a,0x528a,0x528a,0x52aa,0x52aa,0x52aa,0x52aa,
    0x5acb,0x5acb,0x5acb,0x5acb,0x5aeb,0x5aeb,0x5aeb,0x5aeb,
    0x630c,0x630c,0x630c,0x630c,0x632c,0x632c,0x632c,0x632c,
    0x6b4d,0x6b4d,0x6b4d,0x6b4d,0x6b6d,0x6b6d,0x6b6d,0x6b6d,
    0x738e,0x738e,0x738e,0x738e,0x73ae,0x73ae,0x73ae,0x73ae,
    0x7bcf,0x7bcf,0x7bcf,0x7bcf,0x7bef,0x7bef,0x7bef,0x7bef,
    0x8410,0x8410,0x8410,0x8410,0x8430,0x8430,0x8430,0x8430,
    0x8c51,0x8c51,0x8c51,0x8c51,0x8c71,0x8c71,0x8c71,0x8c71,
    0x9492,0x9492,0x9492,0x9492,0x94b2,0x94b2,0x94b2,0x94b2,
    0x9cd3,0x9cd3,0x9cd3,0x9cd3,0x9cf3,0x9cf3,0x9cf3,0x9cf3,
    0xa514,0xa514,0xa514,0xa514,0xa534,0xa534,0xa534,0xa534,
    0xad55,0xad55,0xad55,0xad55,0xad75,0xad75,0xad75,0xad75,
    0xb596,0xb596,0xb596,0xb596,0xb5b6,0xb5b6,0xb5b6,0xb5b6,
    0xbdd7,0xbdd7,0xbdd7,0xbdd7,0xbdf7,0xbdf7,0xbdf7,0xbdf7,
    0xc618,0xc618,0xc618,0xc618,0xc638,0xc638,0xc638,0xc638,
    0xce59,0xce59,0xce59,0xce59,0xce79,0xce79,0xce79,0xce79,
    0xd69a,0xd69a,0xd69a,0xd69a,0xd6ba,0xd6ba,0xd6ba,0xd6ba,
    0xdedb,0xdedb,0xdedb,0xdedb,0xdefb,0xdefb,0xdefb,0xdefb,
    0xe71c,0xe71c,0xe71c,0xe71c,0xe73c,0xe73c,0xe73c,0xe73c,
    0xef5d,0xef5d,0xef5d,0xef5d,0xef7d,0xef7d,0xef7d,0xef7d,
    0xf79e,0xf79e,0xf79e,0xf79e,0xf7be,0xf7be,0xf7be,0xf7be,
    0xffdf,0xffdf,0xffdf,0xffdf,0xffff,0xffff,0xffff,0xffff};


PNGIMAGE _png __attribute__ ((section (".ccmram")));

//
// C interface
//
#ifndef __cplusplus
// C API
int PNG_openRAM(PNGIMAGE *pPNG, uint8_t *pData, int iDataSize, PNG_DRAW_CALLBACK *pfnDraw)
{
    pPNG->iError = PNG_SUCCESS;
    pPNG->pfnRead = readMem;
    pPNG->pfnSeek = seekMem;
    pPNG->pfnDraw = pfnDraw;
    pPNG->pfnOpen = NULL;
    pPNG->pfnClose = NULL;
    pPNG->PNGFile.iSize = iDataSize;
    pPNG->PNGFile.pData = pData;
    return PNGInit(pPNG);
} /* PNG_openRAM() */

#ifdef __LINUX__
int PNG_openFile(PNGIMAGE *pPNG, const char *szFilename, PNG_DRAW_CALLBACK *pfnDraw)
{
    pPNG->iError = PNG_SUCCESS;
    pPNG->pfnRead = readFile;
    pPNG->pfnSeek = seekFile;
    pPNG->pfnDraw = pfnDraw;
    pPNG->pfnOpen = NULL;
    pPNG->pfnClose = closeFile;
    pPNG->PNGFile.fHandle = fopen(szFilename, "r+b");
    if (pPNG->PNGFile.fHandle == NULL)
       return 0;
    fseek((FILE *)pPNG->PNGFile.fHandle, 0, SEEK_END);
    pPNG->PNGFile.iSize = (int)ftell((FILE *)pPNG->PNGFile.fHandle);
    fseek((FILE *)pPNG->PNGFile.fHandle, 0, SEEK_SET);
    return PNGInit(pPNG);
} /* PNG_openFile() */
#endif // __LINUX__
void PNG_close(PNGIMAGE *pPNG)
{
    if (pPNG->pfnClose)
        (*pPNG->pfnClose)(pPNG->PNGFile.fHandle);
} /* PNG_close() */

int PNG_getWidth(PNGIMAGE *pPNG)
{
    return pPNG->iWidth;
} /* PNG_getsWidth() */

int PNG_getHeight(PNGIMAGE *pPNG)
{
    return pPNG->iHeight;
} /* PNG_getHeight() */

int PNG_getLastError(PNGIMAGE *pPNG)
{
    return pPNG->iError;
} /* PNG_getLastError() */

uint8_t *PNG_getPalette(PNGIMAGE *pPNG)
{
    return pPNG->ucPalette;
} /* PNG_getPalette() */

int PNG_getBufferSize(PNGIMAGE *pPNG)
{
    return pPNG->iHeight * pPNG->iPitch;
} /* PNG_getBufferSize() */

uint8_t * PNG_getBuffer(PNGIMAGE *pPNG)
{
    return pPNG->pImage;
} /* PNG_getBuffer() */

#endif // !__cplusplus
PNG_STATIC uint8_t PNGMakeMask(PNGDRAW *pDraw, uint8_t *pMask, uint8_t ucThreshold)
{
    uint8_t alpha, c, *s, *d, *pPal;
    uint8_t cHasOpaque = 0;
    int i, x;
    
    switch (pDraw->iPixelType) {
        case PNG_PIXEL_TRUECOLOR_ALPHA: // truecolor + alpha
            s = pDraw->pPixels;
            d = pMask;
            for (x=0; x<pDraw->iWidth; x+=8) { // groups of 8 pixels in each byte of mask
                c = 0;
                for (i=0; i<8; i++) {
                    c <<= 1;
                    alpha = s[3];
                   if (alpha >= ucThreshold) // if opaque 'enough', set the bit
                       c |= 1;
                    s += 4;
                }
                *d++ = c;
                cHasOpaque |= c;
            }
            break;
        case PNG_PIXEL_GRAY_ALPHA:
            s = pDraw->pPixels;
            d = pMask;
            for (x=0; x<pDraw->iWidth; x+=8) { // groups of 8 pixels in each byte of mask
                c = 0;
                for (i=0; i<8; i++) {
                    c <<= 1;
                    alpha = s[1];
                   if (alpha >= ucThreshold) // if opaque 'enough', set the bit
                       c |= 1;
                    s += 2;
                }
                *d++ = c;
                cHasOpaque |= c;
            }
            break;
        case PNG_PIXEL_INDEXED:
            s = pDraw->pPixels;
            pPal = &pDraw->pPalette[768];
            d = pMask;
            for (x=0; x<pDraw->iWidth; x+=8) { // groups of 8 pixels in each byte of mask
                uint8_t ucPix = 0;
                c = 0;
                switch (pDraw->iBpp) {
                    case 2:
                        for (i=0; i<8; i++) {
                            if (i == 0 || i == 4)
                                ucPix = *s++;
                            c <<= 1;
                            alpha = pPal[ucPix >> 6]; // get palette alpha for this color
                           if (alpha >= ucThreshold) // if opaque 'enough', set the bit
                               c |= 1;
                            ucPix <<= 2;
                        }
                        break;
                    case 4:
                        for (i=0; i<8; i++) {
                            if ((i & 1) == 0)
                                ucPix = *s++;
                            c <<= 1;
                            alpha = pPal[ucPix >> 4]; // get palette alpha for this color
                           if (alpha >= ucThreshold) // if opaque 'enough', set the bit
                               c |= 1;
                            ucPix <<= 4;
                        }
                        break;
                    case 8:
                        for (i=0; i<8; i++) {
                            c <<= 1;
                            alpha = pPal[s[0]]; // get palette alpha for this color
                           if (alpha >= ucThreshold) // if opaque 'enough', set the bit
                               c |= 1;
                            s++;
                        }
                        break;
                } // switch on bit depth
                *d++ = c;
                cHasOpaque |= c;
            }
            break;
        default: // No alpha channel; make a mask of all 1's
            memset(pMask, 0xff, (pDraw->iWidth+7)>>3);
            cHasOpaque = 1;
            break;
    } // switch on pixel type
    return cHasOpaque; // let the caller know if any pixels are opaque
} /* PNGMakeMask() */
//
// Convert a line of native PNG pixels into RGB565
// handles all standard pixel types
// written for simplicity, not necessarily performance
//
PNG_STATIC void PNGRGB565(PNGDRAW *pDraw, uint16_t *pPixels, int iEndiannes, uint32_t u32Bkgd, int iHasAlpha)
{
    int x, j;
    uint16_t usPixel, *pDest = pPixels;
    uint8_t c, a, *pPal, *s = pDraw->pPixels;
    
    switch (pDraw->iPixelType) {
        case PNG_PIXEL_GRAY_ALPHA:
            for (x=0; x<pDraw->iWidth; x++) {
                c = *s++; // gray level
                a = *s++;
                j = (a * c) >> 8; // multiply by the alpha
                usPixel = usGrayTo565[j]; 
                if (iEndiannes == PNG_RGB565_BIG_ENDIAN)
                    usPixel = __builtin_bswap16(usPixel);
                *pDest++ = usPixel;
            }
            break;
        case PNG_PIXEL_GRAYSCALE:
            for (x=0; x<pDraw->iWidth; x++) {
                c = *s++;
                usPixel = (c >> 3); // blue
                usPixel |= ((c >> 2) << 5); // green
                usPixel |= ((c >> 3) << 11); // red
                if (iEndiannes == PNG_RGB565_BIG_ENDIAN)
                    usPixel = __builtin_bswap16(usPixel);
                *pDest++ = usPixel;
            }
            break;
        case PNG_PIXEL_TRUECOLOR:
            for (x=0; x<pDraw->iWidth; x++) {
                usPixel = (s[2] >> 3); // blue
                usPixel |= ((s[1] >> 2) << 5); // green
                usPixel |= ((s[0] >> 3) << 11); // red
                if (iEndiannes == PNG_RGB565_BIG_ENDIAN)
                    usPixel = __builtin_bswap16(usPixel);
                *pDest++ = usPixel;
                s += 3;
            }
            break;
        case PNG_PIXEL_INDEXED: // palette color (can be 1/2/4 or 8 bits per pixel)
            if (pDraw->pFastPalette) { // faster RGB565 palette exists
               switch (pDraw->iBpp) {
                   case 8:
                       for (x=0; x<pDraw->iWidth; x++) {
                           c = *s++;
                           usPixel = pDraw->pFastPalette[c];
                           if (iEndiannes == PNG_RGB565_BIG_ENDIAN)
                               usPixel = __builtin_bswap16(usPixel);
                           *pDest++ = usPixel;
                       }
                       break;
                   case 4:
                       for (x=0; x<pDraw->iWidth; x+=2) {
                           c = *s++;
                           usPixel = pDraw->pFastPalette[c >> 4];
                           if (iEndiannes == PNG_RGB565_BIG_ENDIAN)
                               usPixel = __builtin_bswap16(usPixel);
                           *pDest++ = usPixel;
                           usPixel = pDraw->pFastPalette[c & 0xf];
                           if (iEndiannes == PNG_RGB565_BIG_ENDIAN)
                               usPixel = __builtin_bswap16(usPixel);
                           *pDest++ = usPixel;
                       }
                       break;
                   case 2:
                       for (x=0; x<pDraw->iWidth; x+=4) {
                           c = *s++;
                           for (j=0; j<4; j++) { // work on pairs of bits
                               usPixel = pDraw->pFastPalette[c >> 6];
                               if (iEndiannes == PNG_RGB565_BIG_ENDIAN)
                                   usPixel = __builtin_bswap16(usPixel);
                               *pDest++ = usPixel;
                               c <<= 2;
                           }
                       }
                       break;
                   case 1:
                       for (x=0; x<pDraw->iWidth; x+=4) {
                           c = *s++;
                           for (j=0; j<8; j++) { // work on pairs of bits
                               usPixel = pDraw->pFastPalette[c >> 7];
                               if (iEndiannes == PNG_RGB565_BIG_ENDIAN)
                                   usPixel = __builtin_bswap16(usPixel);
                               *pDest++ = usPixel;
                               c <<= 1;
                           }
                       }
                       break;
               } // switch on bpp 
               return;
            }
            switch (pDraw->iBpp) {
                case 8:
                    for (x=0; x<pDraw->iWidth; x++) {
                        c = *s++;
                        pPal = &pDraw->pPalette[c * 3];
                        usPixel = (pPal[2] >> 3); // blue
                        usPixel |= ((pPal[1] >> 2) << 5); // green
                        usPixel |= ((pPal[0] >> 3) << 11); // red
                        if (iEndiannes == PNG_RGB565_BIG_ENDIAN)
                            usPixel = __builtin_bswap16(usPixel);
                        *pDest++ = usPixel;
                    }
                    break;
                case 4:
                    for (x=0; x<pDraw->iWidth; x+=2) {
                        c = *s++;
                        pPal = &pDraw->pPalette[(c >> 4) * 3];
                        usPixel = (pPal[2] >> 3); // blue
                        usPixel |= ((pPal[1] >> 2) << 5); // green
                        usPixel |= ((pPal[0] >> 3) << 11); // red
                        if (iEndiannes == PNG_RGB565_BIG_ENDIAN)
                            usPixel = __builtin_bswap16(usPixel);
                        *pDest++ = usPixel;
                        pPal = &pDraw->pPalette[(c & 0xf) * 3];
                        usPixel = (pPal[2] >> 3); // blue
                        usPixel |= ((pPal[1] >> 2) << 5); // green
                        usPixel |= ((pPal[0] >> 3) << 11); // red
                        if (iEndiannes == PNG_RGB565_BIG_ENDIAN)
                            usPixel = __builtin_bswap16(usPixel);
                        *pDest++ = usPixel;
                    }
                    break;
                case 2:
                    for (x=0; x<pDraw->iWidth; x+=4) {
                        c = *s++;
                        for (j=0; j<4; j++) { // work on pairs of bits
                            pPal = &pDraw->pPalette[(c >> 6) * 3];
                            usPixel = (pPal[2] >> 3); // blue
                            usPixel |= ((pPal[1] >> 2) << 5); // green
                            usPixel |= ((pPal[0] >> 3) << 11); // red
                            if (iEndiannes == PNG_RGB565_BIG_ENDIAN)
                                usPixel = __builtin_bswap16(usPixel);
                            *pDest++ = usPixel;
                            c <<= 2;
                        }
                    }
                    break;
                case 1:
                    for (x=0; x<pDraw->iWidth; x+=4) {
                        c = *s++;
                        for (j=0; j<8; j++) { // work on pairs of bits
                            pPal = &pDraw->pPalette[(c >> 7) * 3];
                            usPixel = (pPal[2] >> 3); // blue
                            usPixel |= ((pPal[1] >> 2) << 5); // green
                            usPixel |= ((pPal[0] >> 3) << 11); // red
                            if (iEndiannes == PNG_RGB565_BIG_ENDIAN)
                                usPixel = __builtin_bswap16(usPixel);
                            *pDest++ = usPixel;
                            c <<= 1;
                        }
                    }
                    break;
            } // switch on bits per pixel
            break;
        case PNG_PIXEL_TRUECOLOR_ALPHA: // truecolor + alpha
            if (u32Bkgd != 0xffffffff) { // user wants to blend it with a background color
                uint32_t r, g, b, a;
                uint32_t b_r, b_g, b_b;
                b_r = u32Bkgd & 0xff; b_g = (u32Bkgd & 0xff00) >> 8;
                b_b = (u32Bkgd >> 16) & 0xff;
                uint16_t u16Clr = (u32Bkgd & 0xf8) << 8;
                u16Clr |= ((u32Bkgd & 0xfc00) >> 5);
                u16Clr |= ((u32Bkgd & 0xf80000) >> 19);
                for (x=0; x<pDraw->iWidth; x++) {
                    r = s[0]; g = s[1]; b = s[2]; a = s[3];
                    if (a == 0)
                        usPixel = u16Clr;
                    else if (a == 255) { // fully opaque
                        usPixel = (s[2] >> 3); // blue
                        usPixel |= ((s[1] >> 2) << 5); // green
                        usPixel |= ((s[0] >> 3) << 11); // red
                    } else { // mix the colors
                        r = ((r * a) + (b_r * (255-a))) >> 8;
                        g = ((g * a) + (b_g * (255-a))) >> 8;
                        b = ((b * a) + (b_b * (255-a))) >> 8;
                        usPixel = (b >> 3); // blue
                        usPixel |= ((g >> 2) << 5); // green
                        usPixel |= ((r >> 3) << 11); // red
                    }
                    if (iEndiannes == PNG_RGB565_BIG_ENDIAN)
                        usPixel = __builtin_bswap16(usPixel);
                    *pDest++ = usPixel;
                    s += 4; // skip alpha
                }
            } else { // ignore alpha
                for (x=0; x<pDraw->iWidth; x++) {
                    usPixel = (s[2] >> 3); // blue
                    usPixel |= ((s[1] >> 2) << 5); // green
                    usPixel |= ((s[0] >> 3) << 11); // red
                    if (iEndiannes == PNG_RGB565_BIG_ENDIAN)
                        usPixel = __builtin_bswap16(usPixel);
                    *pDest++ = usPixel;
                    s += 4; // skip alpha
                }
            }
            break;
    }
} /* PNGRGB565() */
//
// Helper functions for memory based images
//
PNG_STATIC int32_t seekMem(PNGFILE *pFile, int32_t iPosition)
{
    if (iPosition < 0) iPosition = 0;
    else if (iPosition >= pFile->iSize) iPosition = pFile->iSize-1;
    pFile->iPos = iPosition;
    return iPosition;
} /* seekMem() */

PNG_STATIC int32_t readFLASH(PNGFILE *pFile, uint8_t *pBuf, int32_t iLen)
{
    int32_t iBytesRead;

    iBytesRead = iLen;
    if ((pFile->iSize - pFile->iPos) < iLen)
       iBytesRead = pFile->iSize - pFile->iPos;
    if (iBytesRead <= 0)
       return 0;
    memcpy(pBuf, &pFile->pData[pFile->iPos], iBytesRead);
    pFile->iPos += iBytesRead;
    return iBytesRead;
} /* readFLASH() */

PNG_STATIC int32_t readRAM(PNGFILE *pFile, uint8_t *pBuf, int32_t iLen)
{
    int32_t iBytesRead;

    iBytesRead = iLen;
    if ((pFile->iSize - pFile->iPos) < iLen)
       iBytesRead = pFile->iSize - pFile->iPos;
    if (iBytesRead <= 0)
       return 0;
    memcpy(pBuf, &pFile->pData[pFile->iPos], iBytesRead);
    pFile->iPos += iBytesRead;
    return iBytesRead;
} /* readRAM() */
//
// Verify it's a PNG file and then parse the IHDR chunk
// to get basic image size/type/etc
//
PNG_STATIC int PNGParseInfo(PNGIMAGE *pPage)
{
    uint8_t *s = pPage->ucFileBuf;
    int iBytesRead;
    
    pPage->iHasAlpha = pPage->iInterlaced = 0;
    // Read a few bytes to just parse the size/pixel info
    iBytesRead = (*pPage->pfnRead)(&pPage->PNGFile, s, 32);
    if (iBytesRead < 32) { // a PNG file this tiny? probably bad
        pPage->iError = PNG_INVALID_FILE;
        return pPage->iError;
    }

    if (MOTOLONG(s) != (int32_t)0x89504e47) { // check that it's a PNG file
        pPage->iError = PNG_INVALID_FILE;
        return pPage->iError;
    }
    if (MOTOLONG(&s[12]) == 0x49484452/*'IHDR'*/) {
        pPage->iWidth = MOTOLONG(&s[16]);
        pPage->iHeight = MOTOLONG(&s[20]);
        pPage->ucBpp = s[24]; // bits per pixel
        pPage->ucPixelType = s[25]; // pixel type
        pPage->iInterlaced = s[28];
        if (pPage->iInterlaced || pPage->ucBpp > 8) { // 16-bit pixels are not supported (yet)
            pPage->iError = PNG_UNSUPPORTED_FEATURE;
            return pPage->iError;
        }
        // calculate the number of bytes per line of pixels
        switch (pPage->ucPixelType) {
            case PNG_PIXEL_GRAYSCALE: // grayscale
            case PNG_PIXEL_INDEXED: // indexed
                pPage->iPitch = (pPage->iWidth * pPage->ucBpp + 7)/8; // bytes per pixel
                break;
            case PNG_PIXEL_TRUECOLOR: // truecolor
                pPage->iPitch = ((3 * pPage->ucBpp) * pPage->iWidth + 7)/8;
                break;
            case PNG_PIXEL_GRAY_ALPHA: // grayscale + alpha
                pPage->iPitch = ((2 * pPage->ucBpp) * pPage->iWidth + 7)/8;
                pPage->iHasAlpha = 1;
                break;
            case PNG_PIXEL_TRUECOLOR_ALPHA: // truecolor + alpha
                pPage->iPitch = ((4 * pPage->ucBpp) * pPage->iWidth + 7)/8;
                pPage->iHasAlpha = 1;
        } // switch
    }
    if (pPage->iPitch >= PNG_MAX_BUFFERED_PIXELS)
       return PNG_TOO_BIG;

    return PNG_SUCCESS;
} /* PNGParseInfo() */
//
// De-filter the current line of pixels
//
PNG_STATIC void DeFilter(uint8_t *pCurr, uint8_t *pPrev, int iWidth, int iPitch)
{
    uint8_t ucFilter = *pCurr++;
    int x, iBpp;
    if (iPitch <= iWidth)
        iBpp = 1;
    else
        iBpp = iPitch / iWidth;
    
    pPrev++; // skip filter of previous line
    switch (ucFilter) { // switch on filter type
        case PNG_FILTER_NONE:
            // nothing to do :)
            break;
        case PNG_FILTER_SUB:
            for (x=iBpp; x<iPitch; x++) {
                pCurr[x] += pCurr[x-iBpp];
            }
            break;
        case PNG_FILTER_UP:
            for (x = 0; x < iPitch; x++) {
               pCurr[x] += pPrev[x];
            }
            break;
        case PNG_FILTER_AVG:
            for (x = 0; x < iBpp; x++) {
               pCurr[x] = (pCurr[x] +
                  pPrev[x] / 2 );
            }
            for (x = iBpp; x < iPitch; x++) {
               pCurr[x] = pCurr[x] +
                  (pPrev[x] + pCurr[x-iBpp]) / 2;
            }
            break;
        case PNG_FILTER_PAETH:
            if (iBpp == 1) {
                int a, c;
                uint8_t *pEnd = &pCurr[iPitch];
                // First pixel/byte
                c = *pPrev++;
                a = *pCurr + c;
                *pCurr++ = (uint8_t)a;
                while (pCurr < pEnd) {
                   int b, pa, pb, pc, p;
                   a &= 0xff; // From previous iteration
                   b = *pPrev++;
                   p = b - c;
                   pc = a - c;
                   // assume no native ABS() instruction
                   pa = p < 0 ? -p : p;
                   pb = pc < 0 ? -pc : pc;
                   pc = (p + pc) < 0 ? -(p + pc) : p + pc;
                   // choose the best predictor
                   if (pb < pa) {
                      pa = pb; a = b;
                   }
                   if (pc < pa) a = c;
                   // Calculate current pixel
                   c = b;
                   a += *pCurr;
                   *pCurr++ = (uint8_t)a;
                }
            } else { // multi-byte
                uint8_t *pEnd = &pCurr[iBpp];
                // first pixel is treated the same as 'up'
                while (pCurr < pEnd) {
                   int a = *pCurr + *pPrev++;
                   *pCurr++ = (uint8_t)a;
                }
                pEnd = pEnd + (iPitch - iBpp);
                while (pCurr < pEnd) {
                   int a, b, c, pa, pb, pc, p;
                   c = pPrev[-iBpp];
                   a = pCurr[-iBpp];
                   b = *pPrev++;
                   p = b - c;
                   pc = a - c;
                    // asume no native ABS() instruction
                   pa = p < 0 ? -p : p;
                   pb = pc < 0 ? -pc : pc;
                   pc = (p + pc) < 0 ? -(p + pc) : p + pc;
                   if (pb < pa) {
                      pa = pb; a = b;
                   }
                   if (pc < pa) a = c;
                   a += *pCurr;
                   *pCurr++ = (uint8_t)a;
                }
            } // multi-byte
            break;
    } // switch on filter type
} /* DeFilter() */
//
// PNGInit
// Parse the PNG file header and confirm that it's a valid file
//
// returns 0 for success, 1 for failure
//
PNG_STATIC int PNGInit(PNGIMAGE *pPNG)
{
    return PNGParseInfo(pPNG); // gather info for image
} /* PNGInit() */
//
// Decode the PNG file
//
// You must call open() before calling decode()
// This function can be called repeatedly without having
// to close and re-open the file
//
PNG_STATIC int DecodePNG(PNGIMAGE *pPage, void *pUser, int iOptions)
{
    int err, y, iLen=0;
    int bDone, iOffset, iFileOffset, iBytesRead;
    int iMarker=0;
    uint8_t *tmp, *pCurr, *pPrev;
    z_stream d_stream; /* decompression stream */
    uint8_t *s = pPage->ucFileBuf;
    struct inflate_state *state;
    
    // Either the image buffer must be allocated or a draw callback must be set before entering
    if (pPage->pImage == NULL && pPage->pfnDraw == NULL) {
        pPage->iError = PNG_NO_BUFFER;
        return 0;
    }
    // Use internal buffer to maintain the current and previous lines
    pCurr = pPage->ucPixels;
    pPrev = &pPage->ucPixels[pPage->iPitch+1];
    pPage->iError = PNG_SUCCESS;
    // Start decoding the image
    bDone = FALSE;
    // Inflate the compressed image data
    // The allocation functions are disabled and zlib has been modified
    // to not use malloc/free and instead the buffer is part of the PNG class
    d_stream.zalloc = (alloc_func)0;
    d_stream.zfree = (free_func)0;
    d_stream.opaque = (voidpf)0;
    // Insert the memory pointer here to avoid having to use malloc() inside zlib
    state = (struct inflate_state FAR *)pPage->ucZLIB;
    d_stream.state = (struct internal_state FAR *)state;
    state->window = &pPage->ucZLIB[sizeof(inflate_state)]; // point to 32k dictionary buffer
    err = inflateInit(&d_stream);
#ifdef FUTURE
//    if (inpage->cCompression == PIL_COMP_IPHONE_FLATE)
//        err = mz_inflateInit2(&d_stream, -15); // undocumented option which ignores header and crcs
//    else
//        err = mz_inflateInit2(&d_stream, 15);
#endif // FUTURE
    
    iFileOffset = 8; // skip PNG file signature
    iOffset = 0; // internal buffer offset starts at 0
    // Read some data to start
    (*pPage->pfnSeek)(&pPage->PNGFile, iFileOffset);
    iBytesRead = (*pPage->pfnRead)(&pPage->PNGFile, s, PNG_FILE_BUF_SIZE);
    if (iBytesRead < 1)
    {
        pPage->iError = PNG_INVALID_FILE;
        return pPage->iError;
    }
    iFileOffset += iBytesRead;
    y = 0;
    d_stream.avail_out = 0;
    d_stream.next_out = pPage->pImage;

    while (y < pPage->iHeight) { // continue until fully decoded
        // parse the markers until the next data block
    while (!bDone)
    {
        iLen = MOTOLONG(&s[iOffset]); // chunk length
        if (iLen < 0 || iLen + (iFileOffset - iBytesRead) > pPage->PNGFile.iSize) // invalid data
        {
            pPage->iError = PNG_DECODE_ERROR;
            return 1;
        }
        iMarker = MOTOLONG(&s[iOffset+4]);
        iOffset += 8; // point to the marker data
        switch (iMarker)
        {
            case 0x44474b62: // 'bKGD' DEBUG
                break;
            case 0x67414d41: //'gAMA'
                break;
#ifdef FUTURE
            case 0x6663544C: //'fcTL' frame control block for animated PNG (need to get size of this partial image)
                pPage->iWidth = MOTOLONG(&pPage->pData[iOffset + 4]); // frame width
                pPage->iHeight = MOTOLONG(&pPage->pData[iOffset + 8]); // frame height
                bDone = TRUE;
                break;
#endif
            case 0x504c5445: //'PLTE' palette colors
                memset(&pPage->ucPalette[768], 0xff, 256); // assume all colors are opaque unless specified
                memcpy(pPage->ucPalette, &s[iOffset], iLen);
                if (iOptions & PNG_FAST_PALETTE) { // create a RGB565 palette
                    int i, iColors = 1 << pPage->ucBpp;
                    uint16_t usPixel, *d;
                    uint8_t *s = pPage->ucPalette;
                    d = (uint16_t *)&pPage->ucPixels[sizeof(pPage->ucPixels)-512];
                    for (i=0; i<iColors; i++) {
                    usPixel = (s[2] >> 3); // blue
                    usPixel |= ((s[1] >> 2) << 5); // green
                    usPixel |= ((s[0] >> 3) << 11); // red
                    *d++ = usPixel;
                    s += 3;
                    }
                }
                break;
            case 0x74524e53: //'tRNS' transparency info
                if (pPage->ucPixelType == PNG_PIXEL_INDEXED) // if palette exists
                {
                    memcpy(&pPage->ucPalette[768], &s[iOffset], iLen);
                    pPage->iHasAlpha = 1;
                }
                else if (iLen == 2) // for grayscale images
                {
                    pPage->iTransparent = s[iOffset + 1]; // lower part of 2-byte value is transparent color index
                    pPage->iHasAlpha = 1;
                }
                else if (iLen == 6) // transparent color for 24-bpp image
                {
                    pPage->iTransparent = s[iOffset + 5]; // lower part of 2-byte value is transparent color value
                    pPage->iTransparent |= (s[iOffset + 3] << 8);
                    pPage->iTransparent |= (s[iOffset + 1] << 16);
                    pPage->iHasAlpha = 1;
                }
                break;
            case 0x49444154: //'IDAT' image data block
                while (iLen) {
                    if (iOffset >= iBytesRead) {
                        // we ran out of data; get some more
                        iBytesRead = (*pPage->pfnRead)(&pPage->PNGFile, pPage->ucFileBuf, (iLen > PNG_FILE_BUF_SIZE) ? PNG_FILE_BUF_SIZE : iLen);
                        if (iBytesRead < 1)
                        {
                            pPage->iError = PNG_INVALID_FILE;
                            return pPage->iError;
                        }
                        iFileOffset += iBytesRead;
                        iOffset = 0;
                    } else {
                        // number of bytes remaining in buffer
                        iBytesRead -= iOffset;
                    }
                    d_stream.next_in  = &pPage->ucFileBuf[iOffset];
                    d_stream.avail_in = iBytesRead;
                    iLen -= iBytesRead;
                    if (iLen < 0) iLen = 0;
                    iOffset += iBytesRead;
            //        if (iMarker == 0x66644154) // data starts at offset 4 in APNG frame data block
            //        {
            //            d_stream.next_in += 4;
            //            d_stream.avail_in -= 4;
            //        }
                    err = 0;
                    while (err == Z_OK) {
                        if (d_stream.avail_out == 0) { // reset for next line
                            d_stream.avail_out = pPage->iPitch+1;
                            d_stream.next_out = pCurr;
                        } // otherwise it could be a continuation of an unfinished line
                        err = inflate(&d_stream, Z_NO_FLUSH, iOptions & PNG_CHECK_CRC);
                        if ((err == Z_OK || err == Z_STREAM_END) && d_stream.avail_out == 0) {// successfully decoded line
                            DeFilter(pCurr, pPrev, pPage->iWidth, pPage->iPitch);
                            if (pPage->pImage == NULL) { // no image buffer, send it line by line
                                PNGDRAW pngd;
                                pngd.pUser = pUser;
                                pngd.iPitch = pPage->iPitch;
                                pngd.iWidth = pPage->iWidth;
                                pngd.pPalette = pPage->ucPalette;
                                pngd.pFastPalette = (iOptions & PNG_FAST_PALETTE) ? (uint16_t *)&pPage->ucPixels[sizeof(pPage->ucPixels)-512] : NULL;
                                pngd.pPixels = pCurr+1;
                                pngd.iPixelType = pPage->ucPixelType;
                                pngd.iBpp = pPage->ucBpp;
                                pngd.y = y;
                                (*pPage->pfnDraw)(&pngd);
                            } else {
                                // copy to destination bitmap
                                memcpy(&pPage->pImage[y * pPage->iPitch], &pCurr[1], pPage->iPitch);
                            }
                            y++;
                        // swap current and previous lines
                        tmp = pCurr; pCurr = pPrev; pPrev = tmp;
                        } else { // some error
                            tmp = NULL;
                        }
                    }
                    if (err == Z_STREAM_END && d_stream.avail_out == 0) {
                        // successful decode, stop here
                        y = pPage->iHeight;
                        bDone = TRUE;
                    } else  if (err == Z_DATA_ERROR || err == Z_STREAM_ERROR) {
                        iLen = 0; // quit now
                        y = pPage->iHeight;
                        pPage->iError = PNG_DECODE_ERROR;
                        bDone = TRUE; // force loop to exit with error
                    } else if (err == Z_BUF_ERROR) {
                        y |= 0; // need more data
                    }
                } // while (iLen)
                if (y != pPage->iHeight && iFileOffset < pPage->PNGFile.iSize) {
                    // need to read more IDAT chunks
                    iBytesRead = (*pPage->pfnRead)(&pPage->PNGFile, pPage->ucFileBuf,  PNG_FILE_BUF_SIZE);
                    if (iBytesRead < 1)
                    {
                        pPage->iError = PNG_INVALID_FILE;
                        return pPage->iError;
                    }
                    iFileOffset += iBytesRead;
                    iOffset = 0;
                }
                break;
                //               case 0x69545874: //'iTXt'
                //               case 0x7a545874: //'zTXt'
#ifdef FUTURE
            case 0x74455874: //'tEXt'
            {
                char szTemp[256];
                char *pDest = NULL;
                memcpy(szTemp, &s[iOffset], 80); // get the label length (Title, Author, Description, Copyright, Creation Time, Software, Disclaimer, Warning, Source, Comment)
                i = (int)strlen(szTemp) + 1; // start of actual text
                if (strcmp(szTemp, "Comment") == 0 || strcmp(szTemp, "Description") == 0) pDest = &pPage->szComment[0];
                else if (strcmp(szTemp, "Software") == 0) pDest = &pPage->szSoftware[0];
                else if (strcmp(szTemp, "Author") == 0) pDest = &pPage->szArtist[0];
                if (pDest != NULL)
                {
                    if ((iLen - i) < 128)
                    {
                        memcpy(pPage->szComment, &pPage->pData[iOffset + i], iLen - i);
                        pPage->szComment[iLen - i + 1] = 0;
                    }
                    else
                    {
                        memcpy(pPage->szComment, &pPage->pData[iOffset + i], 127);
                        pPage->szComment[127] = '\0';
                    }
                }
            }
                break;
#endif
        } // switch
        iOffset += (iLen + 4); // skip data + CRC
        if (iOffset > iBytesRead-8) { // need to read more data
            iFileOffset += (iOffset - iBytesRead);
            (*pPage->pfnSeek)(&pPage->PNGFile, iFileOffset);
            iBytesRead = (*pPage->pfnRead)(&pPage->PNGFile, s, PNG_FILE_BUF_SIZE);
            if (iBytesRead < 1)
            {
                pPage->iError = PNG_INVALID_FILE;
                return pPage->iError;
            }
            iFileOffset += iBytesRead;
            iOffset = 0;
        }
    } // while !bDone
    } // while y < height
    err = inflateEnd(&d_stream);
    return pPage->iError;
} /* DecodePNG() */
