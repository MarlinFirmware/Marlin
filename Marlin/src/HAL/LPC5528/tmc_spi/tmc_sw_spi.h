#pragma once


// Transfer mode
enum SPITransferMode {
  SPI_CONTINUE, /* Transfer not finished: CS pin kept active */
  SPI_LAST      /* Transfer ended: CS pin released */
};


class TMC_SPI {
public:

};

extern TMC_SPI tmc_spi;
