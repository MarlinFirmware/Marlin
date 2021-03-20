#include "../inc/MarlinConfig.h"
#include "W25Qxx.h"
#ifdef LONGER3D
#include "../../HAL/HAL_STM32F1/HAL_spi_STM32F1.h"
//uint16_t W25QXX_TYPE=W25Q64;
W25Qxx::W25Qxx(/* args */)
{
}

W25Qxx::~W25Qxx()
{
}
void W25Qxx::W25QXX_Init(void)
{
    pinMode(PC5,OUTPUT);
    W25QXX_CS(1);
    W25QXX_ReadID();
}
uint16_t W25Qxx::W25QXX_ReadID()
{
    uint16_t temp=0;
    W25QXX_CS(0);
    spiSend(0x90);
    spiSend(0x00);
    spiSend(0x00);
    spiSend(0x00);
    temp|=spiRec()<<8;
    temp|=spiRec();
    W25QXX_CS(1);
   //  if((int)temp==0XEF16)
   //       Serial1.println("OK");
   //  Serial1.println((int)temp);
    return temp;
}
//write spi falsh
//Writes the specified length of data at the specified address
//This function has erase function
//pBuffer：data storage area
//WriteAddr: address of start writing （24bit）
//NumByteToWrite:number of bytes to write （max:65535）
uint8_t W25QXX_BUFFER[4096];
void W25Qxx::W25QXX_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
   uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;	   
 	uint16_t i;    
	uint8_t* W25QXX_BUF;	  
   W25QXX_BUF=W25QXX_BUFFER;
   secpos=WriteAddr/4096;     //Sector address
   secoff=WriteAddr%4096;     //Offset within the sector(扇区内的偏移)
   secremain=4096-secoff;     //Sector remaining space size
   // Serial1.print("WriteAddr:");
   // Serial1.println(WriteAddr);
   // Serial1.print("NumByteToWrite:");
   // Serial1.println(NumByteToWrite);
   if(NumByteToWrite<=secremain)
      secremain=NumByteToWrite;  // <=4096 bytes
   while(1)
   {
      W25QXX_Read(W25QXX_BUF,secpos*4096,4096);  //Read the contents of the entire sector（读出整个扇区的内容）
      for(i=0;i<secremain;i++)     
		{
  //       Serial1.println(W25QXX_BUF[secoff+i]);
			if(W25QXX_BUF[secoff+i]!=0XFF)  //Need to erase
            break;
		}

      if(i<secremain) //Need to erase
		{
			W25QXX_Erase_Sector(secpos);		
			for(i=0;i<secremain;i++)	   	
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);

		}
      else
         W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain); 

      if(secremain==NumByteToWrite) 
         break;                    
      else   
      {
         secpos++;  
         secoff=0; 
         pBuffer+=secremain; 
         WriteAddr+=secremain; 
         NumByteToWrite-=secremain;			

         if(NumByteToWrite>4096) 
            secremain=4096;
         else          
            secremain=NumByteToWrite;		
      }
   }
}
void W25Qxx::W25QXX_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)
{
   uint16_t i;
   W25QXX_CS(0);
   spiSend(W25X_ReadData);             
   spiSend((uint8_t)(ReadAddr>>16));   
   spiSend((uint8_t)(ReadAddr>>8)); 
   spiSend((uint8_t)ReadAddr); 
   // for(i=0;i<NumByteToRead;i++)
   // {
   //    pBuffer[i]=spiRec();
   // }
   spiRead(pBuffer,NumByteToRead);
   W25QXX_CS(1);
}

 void W25Qxx::W25QXX_Erase_Sector(uint32_t Dst_Addr)
 {
    	Dst_Addr*=4096;
      W25QXX_Write_Enable();                  	//SET WEL 	 
      W25QXX_Wait_Busy();   
      W25QXX_CS(0);
      spiSend(W25X_SectorErase);     
      spiSend((uint8_t)(Dst_Addr>>16));  
      spiSend((uint8_t)(Dst_Addr>>8)); 
      spiSend((uint8_t)Dst_Addr); 
      W25QXX_CS(1);
      W25QXX_Wait_Busy();   				 
 }

void W25Qxx::W25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
   uint16_t pageremain;	   
   pageremain=256-WriteAddr%256; 
   if(NumByteToWrite<=pageremain)  
      pageremain=NumByteToWrite;
   while(1)
   {
      W25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
      if(NumByteToWrite==pageremain)  
         break;
      else
      {
         pBuffer+=pageremain;
			WriteAddr+=pageremain;	
         NumByteToWrite-=pageremain;			 
         if(NumByteToWrite>256)               
            pageremain=256; 
         else                               
            pageremain=NumByteToWrite; 	  
      }
      
   }
}
void W25Qxx::W25QXX_Write_Enable(void)
{
   W25QXX_CS(0);
   spiSend(W25X_WriteEnable);   
   W25QXX_CS(1);
}
void W25Qxx::W25QXX_Write_Disable(void)
{
   W25QXX_CS(0);
   spiSend(W25X_WriteDisable);   
   W25QXX_CS(1);
}

void W25Qxx::W25QXX_Wait_Busy(void)
{
   while((W25QXX_ReadSR()&0x01)==0x01);  		
}
uint8_t	W25Qxx::W25QXX_ReadSR(void)
{
      uint8_t byte=0;
      W25QXX_CS(0);
      spiSend(W25X_ReadStatusReg);   
      byte=spiRec();
      W25QXX_CS(1);
      return byte;
}
 void W25Qxx::W25QXX_Write_SR(uint8_t sr)
 {
    W25QXX_CS(0);
    spiSend(W25X_WriteStatusReg);
    spiSend(sr);
    W25QXX_CS(1);
 }
void W25Qxx::W25QXX_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
   uint16_t i=0;
   W25QXX_Write_Enable();
   W25QXX_CS(0);
   spiSend(W25X_PageProgram);  
   spiSend((uint8_t)(WriteAddr>>16));   
   spiSend((uint8_t)(WriteAddr>>8)); 
   spiSend((uint8_t)WriteAddr); 
   for(i=0;i<NumByteToWrite;i++)   
   {
      spiSend(pBuffer[i]);
   }
   W25QXX_CS(1);
   W25QXX_Wait_Busy();   		
}
 void W25Qxx::W25QXX_Erase_Chip(void)
 {
    W25QXX_Write_Enable();
    W25QXX_Wait_Busy();
    W25QXX_CS(0);
    spiSend(W25X_ChipErase);
    W25QXX_CS(1);
    W25QXX_Wait_Busy();
 }
  void W25Qxx::W25QXX_WAKEUP(void)
  {
     W25QXX_CS(0);
     spiSend(W25X_ReleasePowerDown);  //send W25X_PowerDown command 0xAB   
     W25QXX_CS(1);
     delay_us(3);
  }
  void W25Qxx::W25QXX_PowerDown(void)
  {
     W25QXX_CS(0);
     spiSend(W25X_PowerDown);
     W25QXX_CS(1);
     delay_us(3);
  }
#endif