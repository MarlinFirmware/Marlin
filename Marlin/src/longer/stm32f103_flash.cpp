//#pragma once
#include "../inc/MarlinConfig.h"
#ifdef LONGER3D
    #include "stm32f103_flash.h"

    volatile FLASH_Status FLASHStatus = FLASH_BUSY;  //Flash Operational State Variables
    /* Name:     Write_Flash_Data
    *  Function: Write data to internal Flash  
    *  Input:    WriteAddress: Target address of the data to be written  (Actual address in Flash:FLASH_STARTADDR+WriteAddress)
    *            data[]:       the header address of written data
    *            num:          the number of written data
    *  */
   void Write_Flash_Data(uint32_t WriteAddress, int16_t data[], uint32_t num)
   {
       uint32_t i = 0;
       int16_t temp = 0;
       FLASH_Unlock();  
       FLASHStatus=FLASH_BUSY;
      FLASHStatus =FLASH_ErasePage(FLASH_STARTADDR);
      if(FLASHStatus == FLASH_COMPLETE)//flash操作完成
      {
            FLASHStatus = FLASH_BUSY;    //清空状态指示标志位
            for(i=0; i<num; i++)
            {
                temp = (int16_t)data[i];
                FLASHStatus = FLASH_ProgramHalfWord(FLASH_STARTADDR+(WriteAddress+i*2), temp);//写入数据
                Serial1.println(temp);
            }
      }
      FLASHStatus=FLASH_BUSY;
      FLASH_Lock();
   }
   void Read_Flash_Data(uint32_t ReadAddress, int16_t *dest_Data, uint32_t num)
   {
       uint32_t i=0;
       ReadAddress=(uint32_t)FLASH_STARTADDR+ReadAddress;
       while(i<num)
       {
        *(dest_Data+i) = *(__IO int16_t*) ReadAddress;
        ReadAddress += 2;
        i++;
       }
    //     Serial1.println(dest_Data[0]);
    //    Serial1.println(dest_Data[1]);
    //    Serial1.println(dest_Data[2]);
    //     Serial1.println(dest_Data[3]);
   }
#endif