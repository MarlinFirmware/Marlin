#include "Wire.h"
#include "/Users/davidbuezas/code/Marlin/Marlin/src/HAL/shared/Delay.h"
#include "/Users/davidbuezas/code/Marlin/Marlin/src/HAL/STM32/fastio.h"

#define DELAY_TICK DELAY_NS(500) // 1.36MHZ 

SoftI2C Wire(0,0);

/*
Modified SoftI2C file that:
- Uses fastiio
- Reduces delays to the mimimum, taking into account that high takes longer throught the pull-up
- Clock working at 1MHz on an STM32H723VG (SKR 3)
*/

#include "Wire.h"

// https://embeddedbucket.wordpress.com/2017/02/19/configuring-gpios-with-hal-and-cmsis-part-1/
inline void SoftI2C::sdaLow() {
  // OUT_WRITE_OD(sda, 0);
  PORT_SDA->BSRR = SDA_BSRR_SET_LOW;
  PORT_SDA->MODER |= SDA_MODDER_OUTPUT;
}

inline void SoftI2C::sdaHigh() {
  // SET_INPUT_PULLUP(sda);
  PORT_SDA->MODER &= SDA_MODDER_INPUT;
  PORT_SDA->PUPDR |= SDA_PUPDR_PULLUP;
}

inline void SoftI2C::sclLow() {
  // OUT_WRITE_OD(sda, 0);
  PORT_SCL->BSRR = SCL_BSRR_SET_LOW;
  PORT_SCL->MODER |= SCL_MODDER_OUTPUT;
}
inline void SoftI2C::sclHigh() {
  // SET_INPUT_PULLUP(scl);
  PORT_SCL->MODER &= SCL_MODDER_INPUT;
  PORT_SCL->PUPDR |= SCL_PUPDR_PULLUP;
}

// Read SDA (for data read)
inline uint8_t SoftI2C::readSda() {
  // return READ(sda);
  return (PORT_SDA->IDR & SDA_PIN_MASK) != 0;
}

// Read SCL (to detect clock-stretching)
inline uint8_t SoftI2C::readScl() {
  // return READ(scl);
  return (PORT_SCL->IDR & SCL_PIN_MASK) != 0;
}

void SoftI2C::pinsSetup(uint8_t sda, uint8_t scl){
  if (sda != 0 && scl != 0){    
    PORT_SDA = FastIOPortMap[STM_PORT(digitalPinToPinName(sda))];    
    uint32_t PIN_POS_SDA = STM_PIN(digitalPinToPinName(sda));
    SDA_PIN_MASK = _BV32(PIN_POS_SDA);
    SDA_MODDER_OUTPUT = (0b01 << (PIN_POS_SDA * 2U)); // 00 => input, 01 => output
    SDA_MODDER_INPUT = ~(0b11 << (PIN_POS_SDA * 2U)); // 00 => input, 01 => output
    SDA_BSRR_SET_LOW = (uint32_t) SDA_PIN_MASK << 16U; // bssr <<0 => set, <<16 => clear
    SDA_PUPDR_PULLUP = (0b01 << (PIN_POS_SDA * 2U)); // 00 => no pullup, 01 => pullup
    PORT_SDA->OSPEEDR |= (0b11 << (PIN_POS_SDA * 2U)); // 00 => low speed, ..., 11 => very high speed
    
    PORT_SCL = FastIOPortMap[STM_PORT(digitalPinToPinName(scl))];
    uint32_t PIN_POS_SCL = STM_PIN(digitalPinToPinName(scl));
    SCL_PIN_MASK = _BV32(PIN_POS_SCL);
    SCL_MODDER_INPUT = ~(0b11 << (PIN_POS_SCL * 2U)); // 00 => input, 01 => output
    SCL_MODDER_OUTPUT = (0b01 << (PIN_POS_SCL * 2U)); // 00 => input, 01 => output
    SCL_BSRR_SET_LOW = (uint32_t) SCL_PIN_MASK << 16U; // bssr <<0 => set, <<16 => clear
    SCL_PUPDR_PULLUP = (0b01 << (PIN_POS_SCL * 2U)); // 00 => no pullup, 01 => pullup
    PORT_SCL->OSPEEDR |= (0b11 << (PIN_POS_SCL * 2U)); // 00 => low speed, ..., 11 => very high speed
  }
}

SoftI2C::SoftI2C(uint8_t dataPin, uint8_t clockPin) {
  pinsSetup(dataPin, clockPin);
}

void SoftI2C::begin(uint8_t dataPin, uint8_t clockPin) {
  pinsSetup(dataPin, clockPin);
  begin();
}
void SoftI2C::begin(void) {
  rxBufferIndex = 0;
  rxBufferLength = 0;
  error = 0;
  isTransmitting = false;

  i2cInit();
}

void SoftI2C::end(void) {
}

void SoftI2C::beginTransmission(uint8_t address) {
  if (isTransmitting) {
    error = (i2cRepStart((address<<1)|I2C_WRITE) ? 0 : 2);
  } else {
    error = (i2cStart((address<<1)|I2C_WRITE) ? 0 : 2);
  }
    // indicate that we are isTransmitting
  isTransmitting = 1;
}

void SoftI2C::beginTransmission(int address) {
  beginTransmission((uint8_t)address);
}

uint8_t SoftI2C::endTransmission(uint8_t sendStop) {
  uint8_t transferError = error;
  if (sendStop) {
    i2cStop();
    isTransmitting = 0;
  }
  error = 0;
  return transferError;
}

uint8_t SoftI2C::endTransmission(void) {
  return endTransmission(true);
}

size_t SoftI2C::write(uint8_t data) {
  if (i2cWrite(data)) {
    return 1;
  } else {
    if (error == 0) error = 3;
    return 0;
  }
}

size_t SoftI2C::write(const uint8_t *data, size_t quantity) {
  size_t progress = 0;
  for(size_t i = 0; i < quantity; ++i){
    progress += write(data[i]);
  }
  return progress;
}

uint8_t SoftI2C::requestFrom(uint8_t address, uint8_t quantity,
            uint32_t iaddress, uint8_t isize, uint8_t sendStop) {
  error = 0;
  uint8_t localerror = 0;
    if (isize > 0) {
      beginTransmission(address);
      // the maximum size of internal address is 3 bytes
      if (isize > 3){
        isize = 3;
      }
      // write internal register address - most significant byte first
      while (isize-- > 0) {
        write((uint8_t)(iaddress >> (isize*8)));
      }
      endTransmission(false);
    }
    // clamp to buffer length
    if(quantity > I2C_BUFFER_LENGTH){
      quantity = I2C_BUFFER_LENGTH;
    }
    if (isTransmitting) {
      localerror = !i2cRepStart((address<<1) | I2C_READ);
    } else {
      localerror = !i2cStart((address<<1) | I2C_READ);
    }
    if (error == 0 && localerror) error = 2;
    // perform blocking read into buffer
    for (uint8_t count=0; count < quantity; count++) {
      rxBuffer[count] = i2cRead(count == quantity-1);
    }
    // set rx buffer iterator vars
    rxBufferIndex = 0;
    rxBufferLength = error ? 0 : quantity;
    if (sendStop) {
      isTransmitting = 0;
      i2cStop();
    }
    return rxBufferLength;
}

uint8_t SoftI2C::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop) {
  return requestFrom((uint8_t)address, (uint8_t)quantity, (uint32_t)0, (uint8_t)0, (uint8_t)sendStop);
}

uint8_t SoftI2C::requestFrom(int address, int quantity, int sendStop) {
  return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)sendStop);
}

uint8_t SoftI2C::requestFrom(uint8_t address, uint8_t quantity) {
  return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)true);
}

uint8_t SoftI2C::requestFrom(int address, int quantity) {
  return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)true);
}

int SoftI2C::available(void) {
  return rxBufferLength - rxBufferIndex;
}

int SoftI2C::read(void) {
  int value = -1;
  if(rxBufferIndex < rxBufferLength){
    value = rxBuffer[rxBufferIndex];
    ++rxBufferIndex;
  }
  return value;
}

int SoftI2C::peek(void) {
  int value = -1;

  if(rxBufferIndex < rxBufferLength){
    value = rxBuffer[rxBufferIndex];
  }
  return value;
}

void SoftI2C::flush(void) {
}

/**
 * @brief Initializes the SoftI2C module.
 *
 * This function needs to be called once in the beginning to initialize the SoftI2C module.
 * It returns false if either the SDA or SCL lines are low, which may indicate an I2C bus lockup
 * or that the lines are not properly pulled up.
 *
 * @return True if initialization is successful, false otherwise.
 */
bool SoftI2C::i2cInit(void) {
  sdaLow();
  sclLow();
  DELAY_TICK;
  sdaHigh();
  sclHigh();
  DELAY_TICK;
  return readSda() && readScl();
}

/**
 * @brief Start an I2C transfer.
 *
 * This function starts an I2C transfer by pulling the data line (SDA) low followed by the clock line (SCL).
 * The 8-bit I2C address (including the R/W bit) is passed as a parameter.
 *
 * @param addr The 8-bit I2C address to communicate with.
 * @return Returns true if the slave responds with an "acknowledge" signal, false otherwise.
 */
bool SoftI2C::i2cStart(uint8_t addr) {
  sdaLow();
  // delayMicroseconds(DELAY);
  sclLow();
  return i2cWrite(addr);
}

/**
 * @brief Try to start transfer until an ACK is returned.
 *
 * This function attempts to start an I2C transfer to the specified address and
 * waits for an acknowledgment (ACK) to be returned. If the ACK is not received
 * within a certain number of retries, the function gives up and returns false.
 *
 * @param addr The 7-bit I2C address of the target device.
 * @return Returns true if the I2C start and ACK were successful, false otherwise.
 */
bool SoftI2C::i2cStartWait(uint8_t addr) {
  long retry = I2C_MAXWAIT;
  while (!i2cStart(addr)) {
    i2cStop();
    if (--retry == 0) return false;
  }
  return true;
}

/**
 * @brief Repeated start function.
 *
 * After having claimed the bus with a start condition,
 * you can address another or the same chip again without an intervening 
 * stop condition.
 *
 * @param addr The 7-bit address of the slave device.
 *
 * @return True if the slave replies with an "acknowledge", false otherwise.
 */
bool SoftI2C::i2cRepStart(uint8_t addr) {
  sdaHigh();
  sclHigh();
  // delayMicroseconds(DELAY);
  DELAY_TICK;
  return i2cStart(addr);
}

/**
 * @brief Issue a stop condition, freeing the bus.
 *
 * This function generates a stop condition on the I2C bus,
 * releasing the bus for other devices to use.
 */
void SoftI2C::i2cStop(void) {
  sdaLow();
  // delayMicroseconds(DELAY);
  sclHigh();
  // delayMicroseconds(DELAY);
  DELAY_TICK;
  sdaHigh();
  // delayMicroseconds(DELAY);
  DELAY_TICK;
}

/**
 * @brief Write one byte to the slave chip that had been addressed by the previous start call.
 *
 * @param value The byte to be sent.
 * @return True if the slave replies with an "acknowledge," false otherwise.
 */
bool SoftI2C::i2cWrite(uint8_t value) {
  for (uint8_t curr = 0b10000000; curr != 0; curr >>= 1) {
    if ((curr & value) != 0) sdaHigh();
    else sdaLow();
    sclHigh();
    // delayMicroseconds(DELAY);
    DELAY_TICK;
    sclLow();
  }

  sdaHigh();
  sclHigh();
  // delayMicroseconds(DELAY/2);
  DELAY_TICK;
  uint8_t ack = readSda();
  sclLow();
  // delayMicroseconds(DELAY/2);  
  sdaLow();
  return ack == 0;
}

/**
 * @brief Read one byte from the I2C bus.
 * 
 * This function reads a single byte from the I2C bus. If the parameter `last` is true,
 * a NAK (Not Acknowledge) signal is sent after receiving the byte to terminate the
 * read sequence.
 * 
 * @param last Set to true if NAK should be sent after the byte read.
 * @return The byte read from the I2C bus.
 */
uint8_t SoftI2C::i2cRead(bool last) {
  uint8_t receivedByte  = 0;
  sdaHigh();
  for (uint8_t i = 0; i < 8; i++) {
    receivedByte <<= 1;
    // delayMicroseconds(DELAY);
    DELAY_TICK;
    sclHigh();
    if (readSda()) receivedByte  |= 1;
    sclLow();
  }
  if (last) sdaHigh();
  else sdaLow();
  sclHigh();
  // delayMicroseconds(DELAY/2);
  DELAY_TICK;
  sclLow();
  // delayMicroseconds(DELAY/2);
  sdaLow();
  return receivedByte ;
}
