/*
  I2C.cpp - I2C library
  Copyright (c) 2011-2012 Wayne Truchsess.  All right reserved.
  Rev 5.0 - January 24th, 2012
          - Removed the use of interrupts completely from the library
            so TWI state changes are now polled. 
          - Added calls to lockup() function in most functions 
            to combat arbitration problems 
          - Fixed scan() procedure which left timeouts enabled 
            and set to 80msec after exiting procedure
          - Changed scan() address range back to 0 - 0x7F
          - Removed all Wire legacy functions from library
          - A big thanks to Richard Baldwin for all the testing
            and feedback with debugging bus lockups!
  Rev 4.0 - January 14th, 2012
          - Updated to make compatible with 8MHz clock frequency
  Rev 3.0 - January 9th, 2012
          - Modified library to be compatible with Arduino 1.0
          - Changed argument type from boolean to uint8_t in pullUp(), 
            setSpeed() and receiveByte() functions for 1.0 compatability
          - Modified return values for timeout feature to report
            back where in the transmission the timeout occured.
          - added function scan() to perform a bus scan to find devices
            attached to the I2C bus.  Similar to work done by Todbot
            and Nick Gammon
  Rev 2.0 - September 19th, 2011
          - Added support for timeout function to prevent 
            and recover from bus lockup (thanks to PaulS
            and CrossRoads on the Arduino forum)
          - Changed return type for stop() from void to
            uint8_t to handle timeOut function 
  Rev 1.0 - August 8th, 2011
  
  This is a modified version of the Arduino Wire/TWI 
  library.  Functions were rewritten to provide more functionality
  and also the use of Repeated Start.  Some I2C devices will not
  function correctly without the use of a Repeated Start.  The 
  initial version of this library only supports the Master.


  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#if(ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <inttypes.h>
#include "I2C.h"
#include "Marlin.h"


uint8_t I2C::bytesAvailable = 0;
uint8_t I2C::bufferIndex = 0;
uint8_t I2C::totalBytes = 0;
uint16_t I2C::timeOutDelay = 0;

I2C::I2C()
{
}


////////////// Public Methods ////////////////////////////////////////



void I2C::begin()
{
  #if defined(__AVR_ATmega168__) || defined(__AVR_ATmega8__) || defined(__AVR_ATmega328P__)
    // activate internal pull-ups for twi
    // as per note from atmega8 manual pg167
    sbi(PORTC, 4);
    sbi(PORTC, 5);
  #else
    // activate internal pull-ups for twi
    // as per note from atmega128 manual pg204
    sbi(PORTD, 0);
    sbi(PORTD, 1);
  #endif
  // initialize twi prescaler and bit rate
  cbi(TWSR, TWPS0);
  cbi(TWSR, TWPS1);
  TWBR = ((F_CPU / 100000) - 16) / 2;
  // enable twi module and acks
  TWCR = _BV(TWEN) | _BV(TWEA); 
}

void I2C::end()
{
  TWCR = 0;
}

void I2C::timeOut(uint16_t _timeOut)
{
  timeOutDelay = _timeOut;
}

void I2C::setSpeed(uint8_t _fast)
{
  if(!_fast)
  {
    TWBR = ((F_CPU / 100000) - 16) / 2;
  }
  else
  {
    TWBR = ((F_CPU / 400000) - 16) / 2;
  }
}
  
void I2C::pullup(uint8_t activate)
{
  if(activate)
  {
    #if defined(__AVR_ATmega168__) || defined(__AVR_ATmega8__) || defined(__AVR_ATmega328P__)
      // activate internal pull-ups for twi
      // as per note from atmega8 manual pg167
      sbi(PORTC, 4);
      sbi(PORTC, 5);
    #else
      // activate internal pull-ups for twi
      // as per note from atmega128 manual pg204
      sbi(PORTD, 0);
      sbi(PORTD, 1);
    #endif
  }
  else
  {
    #if defined(__AVR_ATmega168__) || defined(__AVR_ATmega8__) || defined(__AVR_ATmega328P__)
      // deactivate internal pull-ups for twi
      // as per note from atmega8 manual pg167
      cbi(PORTC, 4);
      cbi(PORTC, 5);
    #else
      // deactivate internal pull-ups for twi
      // as per note from atmega128 manual pg204
      cbi(PORTD, 0);
      cbi(PORTD, 1);
    #endif
  }
}

void I2C::scan()
{
  uint16_t tempTime = timeOutDelay;
  timeOut(80);
  uint8_t totalDevicesFound = 0;
  MYSERIAL.println("Scanning for devices...please wait");
  MYSERIAL.println();
  for(uint8_t s = 0; s <= 0x7F; s++)
  {
    returnStatus = 0;
    returnStatus = start();
    if(!returnStatus)
    { 
      returnStatus = sendAddress(SLA_W(s));
    }
    if(returnStatus)
    {
      if(returnStatus == 1)
      {
        MYSERIAL.println("There is a problem with the bus, could not complete scan");
        timeOutDelay = tempTime;
        return;
      }
    }
    else
    {
      MYSERIAL.print("Found device at address - ");
      MYSERIAL.print(" 0x");
      MYSERIAL.println(s,HEX);
      totalDevicesFound++;
    }
    stop();
  }
  if(!totalDevicesFound){MYSERIAL.println("No devices found");}
  timeOutDelay = tempTime;
}


uint8_t I2C::available()
{
  return(bytesAvailable);
}

uint8_t I2C::receive()
{
  bufferIndex = totalBytes - bytesAvailable;
  if(!bytesAvailable)
  {
    bufferIndex = 0;
    return(0);
  }
  bytesAvailable--;
  return(data[bufferIndex]);
}

  
/*return values for new functions that use the timeOut feature 
  will now return at what point in the transmission the timeout
  occurred. Looking at a full communication sequence between a 
  master and slave (transmit data and then readback data) there
  a total of 7 points in the sequence where a timeout can occur.
  These are listed below and correspond to the returned value:
  1 - Waiting for successful completion of a Start bit
  2 - Waiting for ACK/NACK while addressing slave in transmit mode (MT)
  3 - Waiting for ACK/NACK while sending data to the slave
  4 - Waiting for successful completion of a Repeated Start
  5 - Waiting for ACK/NACK while addressing slave in receiver mode (MR)
  6 - Waiting for ACK/NACK while receiving data from the slave
  7 - Waiting for successful completion of the Stop bit

  All possible return values:
  0           Function executed with no errors
  1 - 7       Timeout occurred, see above list
  8 - 0xFF    See datasheet for exact meaning */ 


/////////////////////////////////////////////////////

uint8_t I2C::write(uint8_t address, uint8_t registerAddress)
{
  returnStatus = 0;
  returnStatus = start();
  if(returnStatus){return(returnStatus);}
  returnStatus = sendAddress(SLA_W(address));
  if(returnStatus)
  {
    if(returnStatus == 1){return(2);}
    return(returnStatus);
  }
  returnStatus = sendByte(registerAddress);
  if(returnStatus)
  {
    if(returnStatus == 1){return(3);}
    return(returnStatus);
  }
  returnStatus = stop();
  if(returnStatus)
  {
    if(returnStatus == 1){return(7);}
    return(returnStatus);
  }
  return(returnStatus);
}

uint8_t I2C::write(int address, int registerAddress)
{
  return(write((uint8_t) address, (uint8_t) registerAddress));
}

uint8_t I2C::write(uint8_t address, uint8_t registerAddress, uint8_t data)
{
  returnStatus = 0;
  returnStatus = start(); 
  if(returnStatus){return(returnStatus);}
  returnStatus = sendAddress(SLA_W(address));
  if(returnStatus)
  {
    if(returnStatus == 1){return(2);}
    return(returnStatus);
  }
  returnStatus = sendByte(registerAddress);
  if(returnStatus)
  {
    if(returnStatus == 1){return(3);}
    return(returnStatus);
  }
  returnStatus = sendByte(data);
  if(returnStatus)
  {
    if(returnStatus == 1){return(3);}
    return(returnStatus);
  }
  returnStatus = stop();
  if(returnStatus)
  {
    if(returnStatus == 1){return(7);}
    return(returnStatus);
  }
  return(returnStatus);
}

uint8_t I2C::write(int address, int registerAddress, int data)
{
  return(write((uint8_t) address, (uint8_t) registerAddress, (uint8_t) data));
}

uint8_t I2C::write(uint8_t address, uint8_t registerAddress, char *data)
{
  uint8_t bufferLength = strlen(data);
  returnStatus = 0;
  returnStatus = write(address, registerAddress, (uint8_t*)data, bufferLength);
  return(returnStatus);
}

uint8_t I2C::write(uint8_t address, uint8_t registerAddress, uint8_t *data, uint8_t numberBytes)
{
  returnStatus = 0;
  returnStatus = start();
  if(returnStatus){return(returnStatus);}
  returnStatus = sendAddress(SLA_W(address));
  if(returnStatus)
  {
    if(returnStatus == 1){return(2);}
    return(returnStatus);
  }
  returnStatus = sendByte(registerAddress);
  if(returnStatus)
  {
    if(returnStatus == 1){return(3);}
    return(returnStatus);
  }
  for (uint8_t i = 0; i < numberBytes; i++)
  {
    returnStatus = sendByte(data[i]);
    if(returnStatus)
      {
        if(returnStatus == 1){return(3);}
        return(returnStatus);
      }
  }
  returnStatus = stop();
  if(returnStatus)
  {
    if(returnStatus == 1){return(7);}
    return(returnStatus);
  }
  return(returnStatus);
}

uint8_t I2C::read(int address, int numberBytes)
{
  return(read((uint8_t) address, (uint8_t) numberBytes));
}

uint8_t I2C::read(uint8_t address, uint8_t numberBytes)
{
  bytesAvailable = 0;
  bufferIndex = 0;
  if(numberBytes == 0){numberBytes++;}
  nack = numberBytes - 1;
  returnStatus = 0;
  returnStatus = start();
  if(returnStatus){return(returnStatus);}
  returnStatus = sendAddress(SLA_R(address));
  if(returnStatus)
  {
    if(returnStatus == 1){return(5);}
    return(returnStatus);
  }
  for(uint8_t i = 0; i < numberBytes; i++)
  {
    if( i == nack )
    {
      returnStatus = receiveByte(0);
      if(returnStatus == 1){return(6);}

      if(returnStatus != MR_DATA_NACK){return(returnStatus);}
    }
    else
    {
      returnStatus = receiveByte(1);
      if(returnStatus == 1){return(6);}
      if(returnStatus != MR_DATA_ACK){return(returnStatus);}
    }
    data[i] = TWDR;
    bytesAvailable = i+1;
    totalBytes = i+1;
  }
  returnStatus = stop();
  if(returnStatus)
  {
    if(returnStatus == 1){return(7);}
    return(returnStatus);
  }
  return(returnStatus);
}

uint8_t I2C::read(int address, int registerAddress, int numberBytes)
{
  return(read((uint8_t) address, (uint8_t) registerAddress, (uint8_t) numberBytes));
}

uint8_t I2C::read(uint8_t address, uint8_t registerAddress, uint8_t numberBytes)
{
  bytesAvailable = 0;
  bufferIndex = 0;
  if(numberBytes == 0){numberBytes++;}
  nack = numberBytes - 1;
  returnStatus = 0;
  returnStatus = start();
  if(returnStatus){return(returnStatus);}
  returnStatus = sendAddress(SLA_W(address));
  if(returnStatus)
  {
    if(returnStatus == 1){return(2);}
    return(returnStatus);
  }
  returnStatus = sendByte(registerAddress);
  if(returnStatus)
  {
    if(returnStatus == 1){return(3);}
    return(returnStatus);
  }
  returnStatus = start();
  if(returnStatus)
  {
    if(returnStatus == 1){return(4);}
    return(returnStatus);
  }
  returnStatus = sendAddress(SLA_R(address));
  if(returnStatus)
  {
    if(returnStatus == 1){return(5);}
    return(returnStatus);
  }
  for(uint8_t i = 0; i < numberBytes; i++)
  {
    if( i == nack )
    {
      returnStatus = receiveByte(0);
      if(returnStatus == 1){return(6);}
      if(returnStatus != MR_DATA_NACK){return(returnStatus);}
    }
    else
    {
      returnStatus = receiveByte(1);
      if(returnStatus == 1){return(6);}
      if(returnStatus != MR_DATA_ACK){return(returnStatus);}
    }
    data[i] = TWDR;
    bytesAvailable = i+1;
    totalBytes = i+1;
  }
  returnStatus = stop();
  if(returnStatus)
  {
    if(returnStatus == 1){return(7);}
    return(returnStatus);
  }
  return(returnStatus);
}

uint8_t I2C::read(uint8_t address, uint8_t numberBytes, uint8_t *dataBuffer)
{
  bytesAvailable = 0;
  bufferIndex = 0;
  if(numberBytes == 0){numberBytes++;}
  nack = numberBytes - 1;
  returnStatus = 0;
  returnStatus = start();
  if(returnStatus){return(returnStatus);}
  returnStatus = sendAddress(SLA_R(address));
  if(returnStatus)
  {
    if(returnStatus == 1){return(5);}
    return(returnStatus);
  }
  for(uint8_t i = 0; i < numberBytes; i++)
  {
    if( i == nack )
    {
      returnStatus = receiveByte(0);
      if(returnStatus == 1){return(6);}
      if(returnStatus != MR_DATA_NACK){return(returnStatus);}
    }
    else
    {
      returnStatus = receiveByte(1);
      if(returnStatus == 1){return(6);}
      if(returnStatus != MR_DATA_ACK){return(returnStatus);}
    }
    dataBuffer[i] = TWDR;
    bytesAvailable = i+1;
    totalBytes = i+1;
  }
  returnStatus = stop();
  if(returnStatus)
  {
    if(returnStatus == 1){return(7);}
    return(returnStatus);
  }
  return(returnStatus);
}

uint8_t I2C::read(uint8_t address, uint8_t registerAddress, uint8_t numberBytes, uint8_t *dataBuffer)
{
  bytesAvailable = 0;
  bufferIndex = 0;
  if(numberBytes == 0){numberBytes++;}
  nack = numberBytes - 1;
  returnStatus = 0;
  returnStatus = start();
  if(returnStatus){return(returnStatus);}
  returnStatus = sendAddress(SLA_W(address));
  if(returnStatus)
  {
    if(returnStatus == 1){return(2);}
    return(returnStatus);
  }
  returnStatus = sendByte(registerAddress);
  if(returnStatus)
  {
    if(returnStatus == 1){return(3);}
    return(returnStatus);
  }
  returnStatus = start();
  if(returnStatus)
  {
    if(returnStatus == 1){return(4);}
    return(returnStatus);
  }
  returnStatus = sendAddress(SLA_R(address));
  if(returnStatus)
  {
    if(returnStatus == 1){return(5);}
    return(returnStatus);
  }
  for(uint8_t i = 0; i < numberBytes; i++)
  {
    if( i == nack )
    {
      returnStatus = receiveByte(0);
      if(returnStatus == 1){return(6);}
      if(returnStatus != MR_DATA_NACK){return(returnStatus);}
    }
    else
    {
      returnStatus = receiveByte(1);
      if(returnStatus == 1){return(6);}
      if(returnStatus != MR_DATA_ACK){return(returnStatus);}
    }
    dataBuffer[i] = TWDR;
    bytesAvailable = i+1;
    totalBytes = i+1;
  }
  returnStatus = stop();
  if(returnStatus)
  {
    if(returnStatus == 1){return(7);}
    return(returnStatus);
  }
  return(returnStatus);
}


/////////////// Private Methods ////////////////////////////////////////


uint8_t I2C::start()
{
  unsigned long startingTime = millis();
  TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
  while (!(TWCR & (1<<TWINT)))
  {
    if(!timeOutDelay){continue;}
    if((millis() - startingTime) >= timeOutDelay)
    {
      lockUp();
      return(1);
    }
       
  }
  if ((TWI_STATUS == START) || (TWI_STATUS == REPEATED_START))
  {
    return(0);
  }
  if (TWI_STATUS == LOST_ARBTRTN)
  {
    uint8_t bufferedStatus = TWI_STATUS;
    lockUp();
    return(bufferedStatus);
  }
  return(TWI_STATUS);
}

uint8_t I2C::sendAddress(uint8_t i2cAddress)
{
  TWDR = i2cAddress;
  unsigned long startingTime = millis();
  TWCR = (1<<TWINT) | (1<<TWEN);
  while (!(TWCR & (1<<TWINT)))
  {
    if(!timeOutDelay){continue;}
    if((millis() - startingTime) >= timeOutDelay)
    {
      lockUp();
      return(1);
    }
       
  }
  if ((TWI_STATUS == MT_SLA_ACK) || (TWI_STATUS == MR_SLA_ACK))
  {
    return(0);
  }
  uint8_t bufferedStatus = TWI_STATUS;
  if ((TWI_STATUS == MT_SLA_NACK) || (TWI_STATUS == MR_SLA_NACK))
  {
    stop();
    return(bufferedStatus);
  }
  else
  {
    lockUp();
    return(bufferedStatus);
  } 
}

uint8_t I2C::sendByte(uint8_t i2cData)
{
  TWDR = i2cData;
  unsigned long startingTime = millis();
  TWCR = (1<<TWINT) | (1<<TWEN);
  while (!(TWCR & (1<<TWINT)))
  {
    if(!timeOutDelay){continue;}
    if((millis() - startingTime) >= timeOutDelay)
    {
      lockUp();
      return(1);
    }
       
  }
  if (TWI_STATUS == MT_DATA_ACK)
  {
    return(0);
  }
  uint8_t bufferedStatus = TWI_STATUS;
  if (TWI_STATUS == MT_DATA_NACK)
  {
    stop();
    return(bufferedStatus);
  }
  else
  {
    lockUp();
    return(bufferedStatus);
  } 
}

uint8_t I2C::receiveByte(uint8_t ack)
{
  unsigned long startingTime = millis();
  if(ack)
  {
    TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);

  }
  else
  {
    TWCR = (1<<TWINT) | (1<<TWEN);
  }
  while (!(TWCR & (1<<TWINT)))
  {
    if(!timeOutDelay){continue;}
    if((millis() - startingTime) >= timeOutDelay)
    {
      lockUp();
      return(1);
    }
  }
  if (TWI_STATUS == LOST_ARBTRTN)
  {
    uint8_t bufferedStatus = TWI_STATUS;
    lockUp();
    return(bufferedStatus);
  }
  return(TWI_STATUS); 
}

uint8_t I2C::stop()
{
  unsigned long startingTime = millis();
  TWCR = (1<<TWINT)|(1<<TWEN)| (1<<TWSTO);
  while ((TWCR & (1<<TWSTO)))
  {
    if(!timeOutDelay){continue;}
    if((millis() - startingTime) >= timeOutDelay)
    {
      lockUp();
      return(1);
    }
       
  }
  return(0);
}

void I2C::lockUp()
{
  TWCR = 0; //releases SDA and SCL lines to high impedance
  TWCR = _BV(TWEN) | _BV(TWEA); //reinitialize TWI 
}

I2C I2c = I2C();

