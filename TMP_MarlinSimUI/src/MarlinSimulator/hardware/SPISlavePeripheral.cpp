#include "SPISlavePeripheral.h"

SPISlavePeripheral::SPISlavePeripheral(SpiBus& spi_bus, pin_type cs) : VirtualPrinter::Component("SPISlavePeripheral"), spi_bus(spi_bus), cs_pin(cs) {
  Gpio::attach(cs_pin, [this](GpioEvent& event){ this->interrupt(event); });
  spi_bus.attach([this](SpiEvent& event){ this->interrupt(event); });
}

SPISlavePeripheral::~SPISlavePeripheral() {};

void SPISlavePeripheral::onBeginTransaction() {
  spi_bus.acquire();
  //printf("BUS Acquired by %s (cs: %d)\n", name.c_str(), cs_pin);
  insideTransaction = true;
  outgoing_byte = 0xFF;
}


void SPISlavePeripheral::onEndTransaction() {
  // check for pending data to receive
  if (requestedDataSize > 0) {
    onRequestedDataReceived(currentToken, requestedData, requestedDataIndex);
  }
  setRequestedDataSize(0xFF, 0);
  insideTransaction = false;
  spi_bus.release();
  //printf("BUS Released by %s (cs: %d)\n", name.c_str(), cs_pin);
}


void SPISlavePeripheral::onByteReceived(uint8_t _byte) {
  //printf("%s::onByteReceived: 0x%X\n", name.c_str(), _byte);
  if (requestedDataSize > 0) {
    requestedData[requestedDataIndex++] = _byte;
    if (requestedDataIndex == requestedDataSize) {
      requestedDataSize = 0;
      onRequestedDataReceived(currentToken, requestedData, requestedDataIndex);
    }
  }
}

void SPISlavePeripheral::onResponseSent() {
  // printf("SPISlavePeripheral::onResponseSent\n");
  hasDataToSend = false;
}

void SPISlavePeripheral::onRequestedDataReceived(uint8_t token, uint8_t* _data, size_t count) {
  //printf("%s::onRequestedDataReceived\n", name.c_str());
}

void SPISlavePeripheral::onByteSent(uint8_t _byte) {
  //printf("%s::onByteSent: %d\n", name.c_str(), _byte);
  if (responseDataSize > 0) {
    outgoing_byte = *responseData;
    responseData++;
    responseDataSize--;
  }
  else {
    if (hasDataToSend) onResponseSent();
    outgoing_byte = 0xFF;
  }
}

void SPISlavePeripheral::setResponse(uint8_t _data) {
  static uint8_t _response = 0;
  _response = _data;
  setResponse(&_response, 1);
}

void SPISlavePeripheral::setResponse16(uint16_t _data, bool msb) {
  static uint16_t _response = 0;
  if (msb) {
    _response = ((_data << 8) & 0xFF00) | (_data >> 8);
  }
  else {
    _response = _data;
  }
  setResponse((uint8_t*)&_response, 2);
  // printf("setResponse: %d, %d\n", _data, _response);
}

void SPISlavePeripheral::setResponse(uint8_t *_bytes, size_t count) {
  responseData = _bytes;
  responseDataSize = count;
  outgoing_byte = *responseData;
  responseData++;
  responseDataSize--;
  hasDataToSend = true;
}

void SPISlavePeripheral::setRequestedDataSize(uint8_t token, size_t _count) {
  currentToken = token;
  requestedDataSize = _count;
  requestedDataIndex = 0;
  delete[] requestedData;
  if (_count > 0) {
    requestedData = new uint8_t[_count];
  }
  else {
    requestedData = nullptr;
  }
}

void SPISlavePeripheral::interrupt(SpiEvent& ev) {
  //printf("SPI(%s): interrupt\n", name.c_str());
  if (Gpio::get_pin_value(cs_pin) != 0 || !insideTransaction) return;

  for (size_t i = 0; i < ev.length; i++) {
    if (ev.read_into != nullptr) {
      ev.read_into[i] = outgoing_byte;
      onByteSent(outgoing_byte);
      //printf("SPI(%s): data out\n", name.c_str());
    }
    if (ev.write_from != nullptr) {
      incoming_byte = ev.source_increment ? ev.write_from[i] : ev.write_from[i % ev.source_format] ;
      onByteReceived(incoming_byte);
      //printf("SPI(%s): data in\n", name.c_str());
    }
  }
}

void SPISlavePeripheral::interrupt(GpioEvent& ev) {
  if (ev.pin_id == cs_pin) {
    if (ev.event == GpioEvent::FALL) onBeginTransaction();
    else if (ev.event == GpioEvent::RISE && insideTransaction) onEndTransaction();
    return;
  }
}
