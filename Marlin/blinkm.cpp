/*
  blinkm.cpp - Library for controlling a BlinkM over i2c
  Created by Tim Koster, August 21 2013.
*/
#include "Marlin.h"
#include "cardreader.h"
#include "temperature.h"
#include "cardreader.h"
#include "stepper.h"
#include "configuration_store.h"

#define  BLINKM

#if ENABLED(BLINKM)
#include "blinkm.h"

uint16_t blink_time = 0;
uint16_t blinkfeed_time = 0;
uint16_t blinkretract_time = 0;

uint8_t print_key_flag = 0;
float z_height_stop = 0;
float temperature_protect_last = 0;

volatile uint32_t protect_time = 0;

uint8_t print_pause = 0; //用来表示打印暂停

uint32_t sys_time = 0;

uint8_t cancel_print = 0;

uint8_t key_flag = 0; //0:没有按键动作,1:按键归零动作, 2:按键退料动作, 3:按键进料动作, 4:正在打印动作

void BlinkLed(void)
{
  static uint32_t blink_time_previous=0;
  static uint32_t blink_time_start=0;
  if(blink_time == 0)
  {
    WRITE(PRINT_LED,1);
    return;
  }
  if(blink_time > 3000)
  {
    WRITE(PRINT_LED,0);
    return;
  }
  if(blink_time_previous!=blink_time)
  {
    blink_time_previous = blink_time;
    blink_time_start = millis();
  }
  if(millis()<blink_time_start+blink_time)
  {
    WRITE(PRINT_LED,0);
  }
  else if(millis()<blink_time_start+2*blink_time)
  {
    WRITE(PRINT_LED,1);
  }
  else
  {
    blink_time_start = millis();
  }
}


void BlinkFeedLed(void)
{
  static uint32_t blink_time_previous=0;
  static uint32_t blink_time_start=0;
  if(blinkfeed_time == 0)
  {
    WRITE(FEED_LED,1);
    return;
  }
  if(blinkfeed_time > 3000)
  {
    WRITE(FEED_LED,0);
    return;
  }
  if(blink_time_previous!=blinkfeed_time)
  {
    blink_time_previous = blinkfeed_time;
    blink_time_start = millis();
  }
  if(millis()<blink_time_start+blinkfeed_time)
  {
    WRITE(FEED_LED,0);
  }
  else if(millis()<blink_time_start+2*blinkfeed_time)
  {
    WRITE(FEED_LED,1);
  }
  else
  {
    blink_time_start = millis();
  }
}

void BlinkRetractLed(void)
{
  static uint32_t blink_time_previous=0;
  static uint32_t blink_time_start=0;
  if(blinkretract_time == 0)
  {
    WRITE(RETRACT_LED,1);
    return;
  }
  if(blinkretract_time > 3000)
  {
    WRITE(RETRACT_LED,0);
    return;
  }
  if(blink_time_previous!=blinkretract_time)
  {
    blink_time_previous = blinkretract_time;
    blink_time_start = millis();
  }
  if(millis()<blink_time_start+blinkretract_time)
  {
    WRITE(RETRACT_LED,0);
  }
  else if(millis()<blink_time_start+2*blinkretract_time)
  {
    WRITE(RETRACT_LED,1);
  }
  else
  {
    blink_time_start = millis();
  }
}

void SendColors(byte red, byte grn, byte blu) {
  Wire.begin();
  Wire.beginTransmission(0x09);
  Wire.write('o');                    //to disable ongoing script, only needs to be used once
  Wire.write('n');
  Wire.write(red);
  Wire.write(grn);
  Wire.write(blu);
  Wire.endTransmission();
}

void PrintOneKey(void)
{
  static uint8_t key_status=0;
  static uint32_t key_time = 0;
  static uint8_t pause_flag = 0;

  if(key_flag==1||key_flag==2||key_flag==3)return;

  if(key_status == 0)  //(1)等待按键
  {
    if(!READ(PRINT_START_PIN))
    {
      key_time = millis();
      key_status = 1;
    }
  }
  else if(key_status == 1) //(2)按键的确认
  {
    if(key_time+30<millis())
    {
      if(!READ(PRINT_START_PIN)) 
      {
        key_time = millis();
        key_status = 2;
      }
      else
      {
        key_status = 0;
      }
    }
  }
  else if(key_status == 2) //(3)按键的处理
  {
    if(READ(PRINT_START_PIN)) //等待松开打印键
    {
      if(key_time + 1000 > millis()) //<一>1秒内是短按键的处理
      {
        if(print_key_flag == 0) //<1>第一次打印键是选择文件开始打印
        {
          card.initsd();
          if(card.cardOK==false)
          {
            BLINK_LED(LED_OFF);
            key_status = 0;
            key_time = 0;
            return;
          }
          uint16_t filecnt = card.getnrfilenames();
          card.getfilename(filecnt);
          while(card.filenameIsDir)
          {
            if(filecnt>1)
            {
              filecnt--;
              card.getfilename(filecnt);
            }
          }
          card.openFile(card.filename,true);
          card.startFileprint();
          BLINK_LED(LED_BLINK_0);
          print_key_flag = 1;
          key_flag = 4;
        }
        else if(print_key_flag == 1)//<2>第二次短按键是暂停打印 
        {
          //MYSERIAL.print("pause");
          BLINK_LED(LED_ON);
          card.pauseSDPrint();
          print_pause = 1;
          print_key_flag = 2;
        }
        else if(print_key_flag == 2) //<3>第三次按键是暂停后恢复打印
        {
          //MYSERIAL.print("print");
          if(temperature_protect_last > 60)
          {
            target_temperature[0]= temperature_protect_last;
            temperature_protect_last = 0;
          }
          BLINK_LED(LED_BLINK_0);
          card.startFileprint();
          print_pause = 0;
          print_key_flag = 1;
        }
        else
        {
          print_key_flag = 0;
        }
      }
      else //<二>长于1秒是长按键,长按键的处理
      {
        if(print_key_flag==0) //<1>没有打印的时候长按键是升Z轴10mm
        {
          if(IsRunning()) 
          {
            destination[Z_AXIS] += 10;
            feedrate = 10*60;
            prepare_move();
          }
        }
        else //在打印或者暂停的时候,长按键是取消打印
        {
          z_height_stop = get_absolute_position(Z_AXIS); 
          //MYSERIAL.print("cancel");

          card.sdprinting = false;
          cancel_heatup = true; 
          cancel_gohome_ms = millis()+500;

          quickStop();
          card.closefile();; // switch off all heaters.

          cancel_print = 1;
          key_flag = 0;
          BLINK_LED(LED_OFF);
          print_key_flag = 0;
          print_pause = 0;
        }
      }
      key_status = 0;
      key_time = 0;
    }
  }
  else
  {
    key_status = 0;
    key_time = 0;
    print_key_flag = 0;
  }
}

void feed_filament(void)
{
  if(IsRunning())
  {
  //    MYSERIAL.print("jin_liao");
      destination[E_AXIS] += 100;
      feedrate = 1.5*60;
      prepare_move();
  }
}

void retract_filament(void)
{
  if(IsRunning())
  {
      //  MYSERIAL.print("tui_liao");
            destination[E_AXIS] += 2;
          feedrate = 3*60;
          prepare_move();

    destination[E_AXIS] -= 300;
    feedrate =15.0*60;
    prepare_move();
  }
}

void LoadFilament(void)
{
  static uint8_t filament_status = 0;
  static uint8_t filament_flag = 0; //0:没有动作,1:进料，2:退料
  static uint8_t key = 0;
  static uint32_t key_time = 0;

  if(key_flag==1||key_flag==4)return;

  if(filament_status == 0) //(1)等待按键状态
  {
    if(!READ(RETRACT_PIN)) 
    {
      key = 0x01;
    };
    if(!READ(FEED_PIN))
    {
      key = 0x02;
    };
    if(key)
    {
      filament_status++;
      key_time = millis()+20;
    }
  }
  else if(filament_status == 1) //(2)按键的确定状态
  {
    if(key_time<=millis())
    {
      if(READ(RETRACT_PIN)){key &= ~0x01;}
      if(READ(FEED_PIN)){key &= ~0x02;   }
      if(key)
      {
        protect_time = millis();
        setTargetHotend0(220);
        if(key&0x01){BLINKRETRACT_LED(LED_BLINK_7);BLINKFEED_LED(LED_OFF);key_flag = 2;}
        if(key&0x02){BLINKFEED_LED(LED_BLINK_7);BLINKRETRACT_LED(LED_OFF);key_flag = 3;}
        filament_status++;
      }
      else
      {
        key_flag =0;
        filament_status = 0;
      }
    }
  }
  else if(filament_status == 2) //(3)等待松开键
  {
    if(READ(RETRACT_PIN)&&READ(FEED_PIN))
    {
      filament_status = 3;
    }
  }
  else if(filament_status == 3) //(4)按键的执行状态,等待加热
  {
    if(current_temperature[0] >= 210) //等待温度加热到210度
    {
      filament_status++;
    }
    else //在升温过程中,按键发生变换
    {
      if(!READ(RETRACT_PIN)) //退料键被按下
      {
        if(key==0x01) //取消加热
        {
          key=0;
          key_flag =0;
          filament_status = 6;
          setTargetHotend0(0);
        //  MYSERIAL.print("retract_hot0_cancel");
        }
        else
        {
          key=0x01;
          BLINKRETRACT_LED(LED_BLINK_7);
          BLINKFEED_LED(LED_OFF);
          key_flag = 2;
        //  MYSERIAL.print("retract_hot0");
          filament_status = 2;
        }
      }; 
      if(!READ(FEED_PIN)) //进料键被按下
      {
        if(key==0x02)//取消加热
        {
          key=0;
          key_flag =0;
          filament_status = 6;
          setTargetHotend0(0);
        //  MYSERIAL.print("feed_hot0_cancel");
        }
        else
        {
          key=0x02;
          BLINKFEED_LED(LED_BLINK_7);
          BLINKRETRACT_LED(LED_OFF);
          key_flag = 3;
          //MYSERIAL.print("feed_hot0");
          filament_status = 2;
        }
      };
    }
  }
  else if(filament_status == 4) //(4)等待进进退料
  {
    if(key&0x01)
    {
      retract_filament();
      BLINKRETRACT_LED(LED_BLINK_5);
      BLINKFEED_LED(LED_OFF);
    }; //电机转动开始进料
    if(key&0x02)
    {
      feed_filament();
      BLINKFEED_LED(LED_BLINK_5);
      BLINKRETRACT_LED(LED_OFF);
    };//电机转动开始退料
    filament_status++;
  }
  else if(filament_status==5) //(5)等待进退料结束
  {
    if(!blocks_queued()) //等待进退料完成后关闭LED
    {
      BLINKRETRACT_LED(LED_OFF);
      BLINKFEED_LED(LED_OFF);
      filament_status = 0;
      key_flag = 0;
      key = 0;
    }
    if(!READ(RETRACT_PIN))
    {
      if(key==0x01) //已经在进料，再次进料则是停止进料
      {
        cleaning_buffer_counter = 2;
        BLINKRETRACT_LED(LED_OFF);
        filament_status = 6;
        key_flag = 0;
      }
      else
      {
        cleaning_buffer_counter = 2;
        key=0x01;
        key_flag = 2;
        filament_status=7;
      }
    }
    if(!READ(FEED_PIN))
    {
      if(key==0x02)
      {
        cleaning_buffer_counter = 2;
        BLINKFEED_LED(LED_OFF);
        key_flag = 0;
        filament_status = 6;
      }
      else
      {
        cleaning_buffer_counter = 2;
        key=0x02;
        key_flag = 3;
        filament_status=7;
      }
    }
  }
  else if(filament_status == 8) //等待松键后重新加热
  {
    if(READ(RETRACT_PIN)&&READ(FEED_PIN)) //退料键被按下
    {
      filament_status = 3;
    }
  }
  else if(filament_status == 7) //等待松键后重新进退料
  {
    if(READ(RETRACT_PIN)&&READ(FEED_PIN)) //退料键被按下
    {
      filament_status = 4;
    }
  }
  else if(filament_status==6) //等待松键后重新开始
  {
    if(READ(RETRACT_PIN)&&READ(FEED_PIN)) //退料键被按下
    {
      BLINKRETRACT_LED(LED_OFF);
      BLINKFEED_LED(LED_OFF);
      filament_status = 0;
    }
  }
  else
  {
    filament_status = 0;
  }
}

void home_key(void)
{
  static uint8_t key_status = 0;
  static uint32_t key_time;

  if(key_flag==4||key_flag==2||key_flag==3)return;

  if(key_status==0)
  {
    if(!READ(Z_HOME_PIN))
    {
      key_time = millis()+50;
      key_status = 1;
    }
  }
  else if(key_status==1)
  {
    if(key_time<=millis())
    {
      if(!READ(Z_HOME_PIN))
      {
        key_status = 2;
      }
      else
      {
        key_status = 0;
      }
    }
  }
  else if(key_status==2)
  {
    WRITE(HOME_LED,0);
    disable_x();
    disable_y();
    enqueuecommand("G28 Z0");
    key_flag = 1;
    key_status = 4;
  }
  else if(key_status == 4) //等待归零完成
  {
    if(READ(Z_HOME_PIN))
    {
      key_status = 3;
    }
  }
  else if(key_status == 3) //等待归零完成
  {
    if(!blocks_queued())
    {
      key_status = 0;
      WRITE(HOME_LED,1);
      key_flag = 0;
    }
  }
}

void heat_protect(void)
{
  if(current_temperature[0] < 60)return;
  if(card.sdprinting==true)return;
  if(blocks_queued())return;
  if(commands_in_queue)return;
  if(key_flag!=0)return;

  if(card.sdprinting==false) //非SD卡打印,非联机打印
  {
    if(protect_time+600000l < millis())
    {
      protect_time = millis();
      temperature_protect_last = target_temperature[0];
      disable_all_heaters();
      MYSERIAL.print("extruder heating off!");
    }
  }
}

#endif
