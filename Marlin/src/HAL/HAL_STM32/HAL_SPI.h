#pragma once

#include <SPI.h>
#include "SoftSPI.h" //TODO: improve the added one when needed. for now let's just go hardware

#define GET_BUS(X) ((HW_SPI(X)) ? ((SPIClass*)spiBus[X]) : ((/*SoftSPI*/SPIClass*)spiBus[X]))
