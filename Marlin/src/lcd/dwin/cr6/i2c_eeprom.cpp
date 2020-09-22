#include "i2c_eeprom.h"
#include <stdlib.h>

#include "../../inc/MarlinConfigPre.h"
#include "../../MarlinCore.h"


/******************** IIC ********************/

// Initialize IIC
void IIC_Init(void)
{					     
  SET_OUTPUT(IIC_EEPROM_SDA);
  SET_OUTPUT(IIC_EEPROM_SCL);
 
	IIC_SCL_1();
	IIC_SDA_1();
}

// Generate IIC start signal
void IIC_Start(void)
{
	SDA_OUT();     // sda line output
	IIC_SDA_1();	  	  
	IIC_SCL_1();
	delay_us(4);
 	IIC_SDA_0();// START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL_0();// Clamp the I2C bus, ready to send or receive data
}	  

// Generate IIC stop signal
void IIC_Stop(void)
{
	SDA_OUT();   // sda line output
	IIC_SCL_0();
	IIC_SDA_0(); //STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL_1(); 
	IIC_SDA_1(); // Send I2C bus end signal
	delay_us(4);							   	
}

// Waiting for the response signal
// Return value: 1, failed to receive response
//               0, successfully received the response
uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SDA_IN();      // SDA set as input
	IIC_SDA_1();delay_us(1);	   
	IIC_SCL_1();delay_us(1);	 
	while(READ_SDA())
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL_0(); // Clock output 0 
	return 0;  
} 

// Generate ACK response
void IIC_Ack(void)
{
	IIC_SCL_0();
	SDA_OUT();
	IIC_SDA_0();
	delay_us(2);
	IIC_SCL_1();
	delay_us(2);
	IIC_SCL_0();
}

// No ACK response
void IIC_NAck(void)
{
	IIC_SCL_0();
	SDA_OUT();
	IIC_SDA_1();
	delay_us(2);
	IIC_SCL_1();
	delay_us(2);
	IIC_SCL_0();
}					 

// IIC sends a byte
// Return whether the slave responds
//   1. There is an answer
//   0. No response	  
void IIC_Send_Byte(uint8_t txd)
{                        
	uint8_t t;   
	SDA_OUT(); 	    
	IIC_SCL_0(); // Pull down the clock to start data transmission
	for(t=0; t<8; t++)
	{              
		// IIC_SDA=(txd&0x80)>>7;
		if(txd & 0x80) {IIC_SDA_1();}
		else IIC_SDA_0();
		txd <<= 1; 	  
		delay_us(2);   // All three delays are necessary for TEA5767
		IIC_SCL_1();
		delay_us(2); 
		IIC_SCL_0();	
		delay_us(2);
	}
} 	   

// Read 1 byte, when ack=1, send ACK, ack=0, send nACK
uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i, receive = 0;
	SDA_IN(); // SDA set as input
	for(i=0; i<8; i++)
	{
		IIC_SCL_0(); 
		delay_us(2);
		IIC_SCL_1();
		receive <<= 1;
		if(READ_SDA()) receive++;   
		delay_us(1); 
	}		
	
	if (!ack) {
		IIC_NAck(); // Send nACK
	} else {
		IIC_Ack();  // Send ACK
	}
	
	return receive;
}


/******************** EEPROM ********************/

// Initialize the IIC interface
void BL24CXX_Init(void)
{
	IIC_Init();
}

// Read a data at the designated address of BL24CXX
//  ReadAddr: the address to start reading
//  Return value: the data read
uint8_t BL24CXX_ReadOneByte(uint16_t ReadAddr)
{				  
	uint8_t temp = 0;		  	    																 
  IIC_Start();  
	if(EE_TYPE>BL24C16)
	{
		IIC_Send_Byte(0XA0);	   // Send write command
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr >> 8);// Send high address
		IIC_Wait_Ack();		 
	} 
	else 
	{
		IIC_Send_Byte(0XA0+((ReadAddr/256) << 1));   // Send device address 0XA0, write data
	}

	IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr%256);   // Send low address
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte(0XA1);           // Enter receive mode
	IIC_Wait_Ack();	 
    temp = IIC_Read_Byte(0);		   
    IIC_Stop(); // Generate a stop condition
	return temp;
}

// Write a data at the designated address of BL24CXX
//   WriteAddr  : Destination address of data written
//   DataToWrite: Data to be written
void BL24CXX_WriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite)
{				   	  	    																 
    IIC_Start();  
	if(EE_TYPE>BL24C16)
	{
		IIC_Send_Byte(0XA0);	    // Send write command
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);// Send high address
 	}
	else
	{
		IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   // Send device address 0XA0, write data
	}	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr%256);   // Send low address
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     // Send byte						   
	IIC_Wait_Ack();  		    	   
    IIC_Stop(); // Generate a stop condition
	
	delay(2);
}

// Start writing data of length Len at the specified address in BL24CXX
// This function is used to write 16bit or 32bit data.
//   WriteAddr  : Start address
//   DataToWrite: The first address of the data array
//   Len        : Length of data to be written 2,4
void BL24CXX_WriteLenByte(uint16_t WriteAddr,uint32_t DataToWrite,uint8_t Len)
{  	
	uint8_t t;
	
	for(t=0;t<Len;t++)
	{
		BL24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}
	
	delay(10);
}

// Start reading data of length Len from the specified address in BL24CXX
// This function is used to read 16bit or 32bit data.
//   ReadAddr   : Start address
//   Returns    : data
//   Len        : The length of the data to be read 2,4
uint32_t BL24CXX_ReadLenByte(uint16_t ReadAddr,uint8_t Len)
{  	
	uint8_t t;
	uint32_t temp=0;
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=BL24CXX_ReadOneByte(ReadAddr+Len-t-1); 	 				   
	}
	return temp;												    
}

// Check if BL24CXX is normal
// The last address (255) of 24XX is used here to store the flag word.
// If you use other 24C series, this address needs to be modified
//   Return 1: Detection failed
//   Return 0: Successful detection
uint8_t BL24CXX_Check(void)
{
	uint8_t temp;
	temp=BL24CXX_ReadOneByte(255);//避免每次开机都写BL24CXX			   
	if(temp==0X55)return 0;		   
	else//排除第一次初始化的情况
	{
		BL24CXX_WriteOneByte(255,0X55);
    temp=BL24CXX_ReadOneByte(255);	  
		if(temp==0X55)return 0;
	}
	return 1;											  
}

// Start reading the specified number of data at the specified address in BL24CXX
//ReadAddr : The address to start reading is 0~255 for 24c02
//pBuffer  : The first address of the data array
//NumToRead: The number of data to be read
void BL24CXX_Read(uint16_t ReadAddr,uint8_t *pBuffer,uint16_t NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=BL24CXX_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
}  

// Start writing the specified number of data at the specified address in BL24CXX
//   WriteAddr : The address to start writing is 0~255 for 24c02
//   pBuffer   : The first address of the data array
//   NumToWrite: The number of data to be written
void BL24CXX_Write(uint16_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite)
{
	while(NumToWrite--)
	{
		BL24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
	delay(10);
}

