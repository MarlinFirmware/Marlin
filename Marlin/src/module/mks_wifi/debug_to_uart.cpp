#include "debug_to_uart.h"

#ifdef DEBUG_TO_UART

void debug_to_uart(char *fmt,...){

    char buffer[200];
    va_list ParamList;
    char *ptr = (char *)buffer;

    buffer[0] = ';';
    buffer[1] = ' ';
    
    va_start(ParamList, fmt);
    vsnprintf (buffer+2, 199, fmt, ParamList);
    va_end(ParamList);

#ifdef MARLIN_CONFIG_MY
    SERIAL_ECHOLN((char *)&buffer);	
#endif

    while(*ptr){
      while(MYSERIAL2.availableForWrite()==0){
        safe_delay(10);				
      }
		MYSERIAL2.write(*ptr++);
    }
   

}

#endif
