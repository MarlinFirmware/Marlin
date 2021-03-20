#pragma once
#include "../inc/MarlinConfig.h"
#ifdef LONGER3D
    #include "flash_stm32.h"
    #define FLASH_STARTADDR  0x807D000 //500k     // //0x0807FFFF              //the end address of stm32f103VET6 512k flash 
     void Write_Flash_Data(uint32_t WriteAddress, int16_t data[], uint32_t num);
     void Read_Flash_Data(uint32_t ReadAddress, int16_t *dest_Data, uint32_t num);
#endif