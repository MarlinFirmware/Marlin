#pragma once

#include "../virtual_printer.h"
#include "Gpio.h"
#include "bus/spi.h"

/**
 * Class to Easily Handle SPI Slave communication
 */
class SPISlavePeripheral : public VirtualPrinter::Component {
public:
  SPISlavePeripheral(SpiBus &spi_bus, pin_type cs);
  virtual ~SPISlavePeripheral();

  // Callbacks
  virtual void onBeginTransaction();
  virtual void onEndTransaction();

  virtual void onByteReceived(uint8_t _byte);
  virtual void onRequestedDataReceived(uint8_t token, uint8_t* _data, size_t count);

  virtual void onByteSent(uint8_t _byte);
  virtual void onResponseSent();


  void setResponse(uint8_t _data);
  void setResponse16(uint16_t _data, bool msb = true);
  void setResponse(uint8_t *_bytes, size_t count);

  void setRequestedDataSize(uint8_t token, size_t _count);

  uint8_t getCurrentToken() { return currentToken; }
  void clearCurrentToken() { currentToken = 0xFF; }

protected:
  void interrupt(GpioEvent& ev);
  void interrupt(SpiEvent& ev);
  SpiBus &spi_bus;
  pin_type cs_pin;

private:
  uint8_t incoming_byte = 0;
  uint8_t incoming_byte_count = 0;
  uint8_t outgoing_byte = 0xFF;

  uint8_t *responseData = nullptr;
  size_t responseDataSize = 0;
  bool insideTransaction = false;
  bool hasDataToSend = false;
  uint8_t currentToken = 0xFF;
  uint8_t *requestedData = nullptr;
  size_t requestedDataSize = 0;
  size_t requestedDataIndex = 0;
};
