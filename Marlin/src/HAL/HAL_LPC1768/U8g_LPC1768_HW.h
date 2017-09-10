  
  /*
  //#include "../../../Marlin/src/HAL/HAL_LPC1768/HAL_spi.cpp" 
  #include "HAL_spi.cpp" 
  switch(msg)
  {
    case U8G_COM_MSG_STOP:
      break;
    
    case U8G_COM_MSG_INIT:

      u8g_Delay(125);
      spiBegin();
      spiInit(3);  // run LCD at 1 MHz
        
      u8g_SetPILevel(u8g, U8G_PI_CS, 1);
      u8g_SetPILevel(u8g, U8G_PI_A0, 1);
      u8g_SetPILevel(u8g, U8G_PI_RESET, 1);
      u8g_SetPIOutput(u8g, U8G_PI_CS);
      u8g_SetPIOutput(u8g, U8G_PI_A0);
      u8g_SetPIOutput(u8g, U8G_PI_RESET);

      break;
    
    case U8G_COM_MSG_ADDRESS:                     // define cmd (arg_val = 0) or data mode (arg_val = 1) 
      u8g_SetPILevel(u8g, U8G_PI_A0, arg_val);
      break;

    case U8G_COM_MSG_CHIP_SELECT:
      
       u8g_SetPILevel(u8g, U8G_PI_CS, (arg_val ? 0 : 1));
      break;
      
    case U8G_COM_MSG_RESET:
      break;
    
    case U8G_COM_MSG_WRITE_BYTE:
      spiSend(arg_val & 0xFF);
      break;
    
    case U8G_COM_MSG_WRITE_SEQ:
      {
        register uint8_t *ptr = arg_ptr;
        while( arg_val > 0 )
        {
          spiSend(*ptr++);
          arg_val--;
        }
      }
      break;
    case U8G_COM_MSG_WRITE_SEQ_P:
      {
        register uint8_t *ptr = arg_ptr;
        while( arg_val > 0 )
        {
          spiSend(ptr);
          ptr++;
          arg_val--;
        }
      }
      break;
  }
  return 1;
  
  */