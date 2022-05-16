

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(BTT_TFT)
#include "btt_tft_api.h"
#include "../marlinui.h"

btt_tft_gcode_request bttgocde;

static uint8_t command_len;
static char nextion_command[MAX_CMND_LEN];
char delimiter[] = " ";

unsigned long previousMillis = 0;
unsigned long interval = 3000;
bool command_ready = false;

// PauseMode oldpause = PAUSE_MODE_SAME;

void M(){
if (nextion_command[0] == 'G' || nextion_command[0] == 'M' || nextion_command[0] == 'T')
    {
      if (strcmp(nextion_command, "M105") == 0)
      {
        command_ready = false;
        bttgocde.M105_report();
      }
      else if (strcmp(nextion_command, "M92") == 0)
      {
        command_ready = false;
        
        bttgocde.M92_report();
        bttgocde.ok_send();
      }
      else if (strcmp(nextion_command, "M211") == 0)
      {
        //command_ready = false;
        
        //bttgocde.M211_report();
      }
      else if (strcmp(nextion_command, "M569") == 0)
      {
        //command_ready = false;
        
        //bttgocde.M569_report();
      }
      else if (strcmp(nextion_command, "M114") == 0)
      {
        //command_ready = false;
        
        //bttgocde.M114_report();
      }
      else if (strcmp(nextion_command, "M503") == 0 || strcmp(nextion_command, "M503 S0") == 0)
      {
        command_ready = false;
        
        while(bttgocde.M503_report(nextion_command));
        
      }
      else if (strcmp(nextion_command, "M115") == 0)
      {
        //command_ready = false;
        //bttgocde.ok_send();
        //bttgocde.M115_report();
      }
    }
}
void G()
{

}
void T()
{

}

void command_gcode()
{
  if (command_ready)
  {
    nextion_command[command_len] = 0x00;

    switch (nextion_command[0])
    {
    case 'M':
        M();
      break;
    case 'G':
        G();
      break;
    case 'T':
        T();
      break;
    default:
      command_ready = false;
      break;
    }
    
    
      if (!queue.ring_buffer.full() && command_ready == true)
      {
        queue.enqueue_one(nextion_command);
        bttgocde.ok_send();
      }
      command_len = 0;
    
  } 
}
bool ReadTFTCommand()
{
  command_ready = false;
  while ((BTT_TFT_SERIAL.available()) && (command_len < MAX_CMND_LEN))
  {
    nextion_command[command_len] = BTT_TFT_SERIAL.read();
    if (nextion_command[command_len] == 10)
    {
      command_ready = true;
      break;
    }
    command_len++;
  }
  return command_ready;
}

void btt_tft_api::update()
{
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval)
    {
    previousMillis = currentMillis;

    bttgocde.temp_report();
    //bttgcode.postion();
    }
    ReadTFTCommand();
    command_gcode();
}

void btt_tft_api::init()
{
    BTT_TFT_SERIAL.begin(BTT_BAUDRATE);
#ifdef LCD_SERIAL_PORT
    const millis_t serial_connect_timeout = millis() + 1000UL;
    while (!LCD_SERIAL.connected() && PENDING(millis(), serial_connect_timeout))
    { /*nada*/
    }
#endif
    command_len = 0;
    nextion_command[0] = '\0';
}



#ifndef EXTENSIBLE_UI

MarlinUI::init(){btt_tft_api::init();}
MarlinUI::update(){btt_tft_api::update();}
MarlinUI::killsreen(FSTR_P const lcd_error, FSTR_P const lcd_component);
#endif // !1(EXTENSIBLE_UI)

#endif








