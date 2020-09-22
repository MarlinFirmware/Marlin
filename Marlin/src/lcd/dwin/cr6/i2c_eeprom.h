#ifndef __I2C_EEPROM_H
#define __I2C_EEPROM_H

#include "../../inc/MarlinConfig.h"
#include "../../Marlin.h"

#include <libmaple/gpio.h>


/******************** IIC ********************/
// IO direction setting
#define SDA_IN()  {PIN_MAP[IIC_EEPROM_SDA].gpio_device->regs->CRH&=0XFFFF0FFF;PIN_MAP[IIC_EEPROM_SDA].gpio_device->regs->CRH|=8<<12;}
#define SDA_OUT() {PIN_MAP[IIC_EEPROM_SDA].gpio_device->regs->CRH&=0XFFFF0FFF;PIN_MAP[IIC_EEPROM_SDA].gpio_device->regs->CRH|=3<<12;}

// IO operation function
#define IIC_SCL_0()   digitalWrite(IIC_EEPROM_SCL,LOW)
#define IIC_SCL_1()   digitalWrite(IIC_EEPROM_SCL,HIGH)
#define IIC_SDA_0()   digitalWrite(IIC_EEPROM_SDA,LOW)
#define IIC_SDA_1()   digitalWrite(IIC_EEPROM_SDA,HIGH)
#define READ_SDA()    READ(IIC_EEPROM_SDA)  

// IIC all operation functions
void IIC_Init(void);                       // Initialize the IO port of IIC			 
void IIC_Start(void);				       // Send IIC start signal
void IIC_Stop(void);	  		      	   // Send IIC stop signal
void IIC_Send_Byte(uint8_t txd);		   // IIC send a byte
uint8_t IIC_Read_Byte(unsigned char ack);  // IIC read one byte
uint8_t IIC_Wait_Ack(void);                // IIC waits for ACK signal
void IIC_Ack(void);                        // IIC send ACK signal
void IIC_NAck(void);                       // IIC not ACK signal

void IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);	  


/******************** EEPROM ********************/
#define BL24C01		127
#define BL24C02		255
#define BL24C04		511
#define BL24C08		1023
#define BL24C16		2047
#define BL24C32		4095
#define BL24C64	  8191
#define BL24C128	16383
#define BL24C256	32767  
#define EE_TYPE   BL24C16
					  
uint8_t BL24CXX_ReadOneByte(uint16_t ReadAddr); // Read a byte at the specified address
void BL24CXX_WriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite); // Write a byte to the specified address
void BL24CXX_WriteLenByte(uint16_t WriteAddr,uint32_t DataToWrite,uint8_t Len); // The specified address starts to write the specified length of data
uint32_t BL24CXX_ReadLenByte(uint16_t ReadAddr,uint8_t Len);					// The specified address starts to read the specified length data
void BL24CXX_Write(uint16_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite);	// Write data of the specified length from the specified address
void BL24CXX_Read(uint16_t ReadAddr,uint8_t *pBuffer,uint16_t NumToRead);   	// Read the data of the specified length from the specified address

uint8_t BL24CXX_Check(void);  // Check the device
void BL24CXX_Init(void);      // Initialize IIC


#endif

