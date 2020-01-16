/**
 * Copyright (C) 2011 Circuits At Home, LTD. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Contact information
 * -------------------
 *
 * Circuits At Home, LTD
 * Web      :  http://www.circuitsathome.com
 * e-mail   :  support@circuitsathome.com
 */
#pragma once

#ifndef _usb_h_
  #error "Never include parsetools.h directly; include Usb.h instead"
#endif

struct MultiValueBuffer {
  uint8_t valueSize;
  void *pValue;
} __attribute__((packed));

class MultiByteValueParser {
  uint8_t * pBuf;
  uint8_t countDown;
  uint8_t valueSize;

public:

  MultiByteValueParser() : pBuf(nullptr), countDown(0), valueSize(0) {
  };

  const uint8_t* GetBuffer() { return pBuf; }

  void Initialize(MultiValueBuffer * const pbuf) {
    pBuf = (uint8_t*)pbuf->pValue;
    countDown = valueSize = pbuf->valueSize;
  }

  bool Parse(uint8_t **pp, uint16_t *pcntdn);
};

class ByteSkipper {
  uint8_t *pBuf;
  uint8_t nStage;
  uint16_t countDown;

public:

  ByteSkipper() : pBuf(nullptr), nStage(0), countDown(0) {
  }

  void Initialize(MultiValueBuffer *pbuf) {
    pBuf = (uint8_t*)pbuf->pValue;
    countDown = 0;
  }

  bool Skip(uint8_t **pp, uint16_t *pcntdn, uint16_t bytes_to_skip) {
    switch (nStage) {
      case 0:
        countDown = bytes_to_skip;
        nStage++;
      case 1:
        for (; countDown && (*pcntdn); countDown--, (*pp)++, (*pcntdn)--);

        if (!countDown)
          nStage = 0;
    }
    return (!countDown);
  }
};

// Pointer to a callback function triggered for each element of PTP array when used with PTPArrayParser
typedef void (*PTP_ARRAY_EL_FUNC)(const MultiValueBuffer * const p, uint32_t count, const void *me);

class PTPListParser {
public:

  enum ParseMode {
    modeArray, modeRange/*, modeEnum*/
  };

private:
  uint8_t nStage;
  uint8_t enStage;

  uint32_t arLen;
  uint32_t arLenCntdn;

  uint8_t lenSize; // size of the array length field in bytes
  uint8_t valSize; // size of the array element in bytes

  MultiValueBuffer *pBuf;

  // The only parser for both size and array element parsing
  MultiByteValueParser theParser;

  uint8_t /*ParseMode*/ prsMode;

public:

  PTPListParser() :
    nStage(0),
    enStage(0),
    arLen(0),
    arLenCntdn(0),
    lenSize(0),
    valSize(0),
    pBuf(nullptr),
    prsMode(modeArray) {}
  ;

  void Initialize(const uint8_t len_size, const uint8_t val_size, MultiValueBuffer * const p, const uint8_t mode = modeArray) {
    pBuf = p;
    lenSize = len_size;
    valSize = val_size;
    prsMode = mode;

    if (prsMode == modeRange) {
      arLenCntdn = arLen = 3;
      nStage = 2;
    }
    else {
      arLenCntdn = arLen = 0;
      nStage = 0;
    }
    enStage = 0;
    theParser.Initialize(p);
  }

  bool Parse(uint8_t **pp, uint16_t *pcntdn, PTP_ARRAY_EL_FUNC pf, const void *me = nullptr);
};
