#include "stdio.h"
#include <stdio.h>
#include <arduino.h>
#include <wstring.h>
#include "lcd_rts.h"
#include "../../../inc/MarlinConfig.h"
#include "../../../sd/cardreader.h"
#include "../../../gcode/queue.h"
#include "../../../libs/duration_t.h"
#include "../../../module/settings.h"
#include "../../../core/serial.h"
#include "../../../module/temperature.h"
#include "../../../module/motion.h"
#include "../../../module/planner.h"
#include "../../../module/printcounter.h"
#include "../../../module/probe.h"
#include "base64.h"
#include "utf8_unicode.h"
#include "lcd_rts.h"
#include "preview.h"
#define BRIGHTNESS_PRINT_HIGH    300        // 进度条的总高度
#define BRIGHTNESS_PRINT_WIDTH   300        // 进度条的总宽度
#define BRIGHTNESS_PRINT_LEFT_HIGH_X    186  // 进度条的左上角-X
#define BRIGHTNESS_PRINT_LEFT_HIGH_Y    70  // 进度条的左上角-Y
#define BRIGHTNESS_PRINT    120             // 亮度值（0最暗）
#define FORMAT_JPG_HEADER "jpg begin"
#define FORMAT_PNG_HEADER "png begin"
#define FORMAT_JPG "jpg"
#define FORMAT_PNG "png"
DwinBrightness_t printBri; // 预览图结构体
#define JPG_BYTES_PER_FRAME 240   // 每一帧发送的字节数（图片数据）
#define JPG_WORD_PER_FRAME  (JPG_BYTES_PER_FRAME/2)   // 每一帧发送的字数（图片数据）
#define SizeofDatabuf2		300
#define USER_LOGIC_DEUBG 1

#ifdef LCD_SERIAL_PORT
  #define LCDSERIAL LCD_SERIAL
#elif SERIAL_PORT_2
  #define LCDSERIAL MYSERIAL2
#endif

/**
 * 亮度调节
 * brightless addr
 * in range: 0x8800 ~ 0x8FFF
 */
#define BRIGHTNESS_ADDR_PRINT               0x8800
unsigned char databuf[SizeofDatabuf2];

  // 向指定地址空间写两个字节
  void DWIN_WriteOneWord(unsigned long addr, unsigned int data)
  {
    rtscheck.RTS_SndData(data, addr, VarAddr_W);
  }

void dwin_uart_write(unsigned char *buf, int len)
{
        for (uint8_t n = 0; n < len; ++n) { LCDSERIAL.write(buf[n]); }
}

// 发送jpg图片的一帧数据
void RTS_SendJpegDate(const char *str, unsigned long addr, unsigned char cmd/*= VarAddr_W*/)
  {
    int len = JPG_BYTES_PER_FRAME;//strlen(str);
    if( len > 0)
    {
      databuf[0] = FHONE;
      databuf[1] = FHTWO;
      databuf[2] = 3+len;
      databuf[3] = cmd;
      databuf[4] = addr >> 8;
      databuf[5] = addr & 0x00FF;
      for(int i =0;i <len ;i++)
      {
        databuf[6 + i] = str[i];
      }

      dwin_uart_write(databuf, len + 6);

      memset(databuf,0, sizeof(databuf));
    }
  }
 // 显示jpg图片
 void DWIN_DisplayJpeg(unsigned long addr, unsigned long vp)
  {
    unsigned char buf[10];
    buf[0] = 0x5A;
    buf[1] = 0xA5;
    buf[2] = 0x07;
    buf[3] = 0x82;
    buf[4] = addr >> 8;     //控件地址
    buf[5] = addr & 0x00FF;
    buf[6] = 0x5A;
    buf[7] = 0xA5;
    buf[8] = vp >> 8;       //图片存储地址
    buf[9] = vp & 0x00FF;

    dwin_uart_write(buf, 10);
  }
    // 发送jpg图片整包数据
void DWIN_SendJpegDate(char *jpeg, unsigned long size, unsigned long jpgAddr)
{
    uint32_t MS = millis();

    int jpgSize = size;

    char buf[JPG_BYTES_PER_FRAME];
    int i,j;
   
    for (i = 0; i < jpgSize / JPG_BYTES_PER_FRAME; i++) {
      // delay(20);
      //memset(buf, 0, JPG_BYTES_PER_FRAME);
      memcpy(buf, &jpeg[i * JPG_BYTES_PER_FRAME], JPG_BYTES_PER_FRAME);
      hal.watchdog_refresh();

      // 向指定地址发送图片数据
      RTS_SendJpegDate(buf, (jpgAddr + (JPG_WORD_PER_FRAME * i)), 0x82);

      if (ENABLED(DWIN_DEBUG)) {
        for (j = 0; j < JPG_BYTES_PER_FRAME; j++) {
            //SERIAL_ECHOPAIR(" ", j,
            //                " = ", buf[j]);
            //if ((j+1) % 8 == 0) SERIAL_ECHO("\r\n");
        }
      }
      
      // dwin转大彩、dwin的7寸屏，发送预览图一帧数据后，没有返回值
      // 所以异常先不进行判断，
      #define CMD_TAILA    0x03824F4B      // 帧尾(当前对4.3寸的dwin屏有效)
      uint8_t receivedbyte = 0;
      uint8_t cmd_pos = 0;     // 当前指令指针状态
      uint32_t cmd_state = 0;  // 队列帧尾检测状态
      char buffer[20] = {0};
      MS = millis();
      while(1)
      {
        if (LCDSERIAL.available())
        {
          // 取一个数据
          receivedbyte = LCDSERIAL.read();
          SERIAL_ECHO_MSG("receivedbyte = ", receivedbyte);
          if(cmd_pos == 0 && receivedbyte != 0x5A)      //指令的第一个字节必须是帧头
          {
              continue;
          }

          if(cmd_pos < 20) 
              buffer[cmd_pos++] = receivedbyte;           //防止溢出
          else
              break;

          cmd_state = ((cmd_state << 8) | receivedbyte);  //拼接最后4字节，组成最后一个32位整数

          // 帧尾判断
          if(cmd_state==CMD_TAILA)
          {
              break;
          }
        }
        if (millis() - MS >= 25) {	// 根据数据手册，延时20ms，有概率出现刷不出预览图的现象!!!
        //   PRINT_LOG("more than 25ms");
          break;
        } 
      }

    }

    if (jpgSize % JPG_BYTES_PER_FRAME)
    {
      memset(buf, 0, JPG_BYTES_PER_FRAME);
      memcpy(buf, &jpeg[i * JPG_BYTES_PER_FRAME], (jpgSize - i * JPG_BYTES_PER_FRAME));
      hal.watchdog_refresh();

      // 向指定地址发送图片数据
      RTS_SendJpegDate(buf, (jpgAddr + (JPG_WORD_PER_FRAME * i)), 0x82);

      if (ENABLED(DWIN_DEBUG)) {
        for (j = 0; j < JPG_BYTES_PER_FRAME; j++) {
            //SERIAL_ECHOPAIR(" ", j,
            //                " = ", buf[j]);
            //if ((j+1) % 8 == 0) SERIAL_ECHO("\r\n");
        }
      }
	  delay(25);	// 根据数据手册，延时20ms，有概率出现刷不出预览图的现象!!!
    }
}



/**
 * @功能   从U盘中读取预览图数据并解码
 * @Author Creality
 * @Time   2022-04-13
 * buf          : 用于保存解码后的数据
 * picLen       : 需要的数据长度
 * resetFlag    : 重置数据标志 -- 由于Base64解码后是3的倍数（4个Base64字符解码后是4个字节数据），但是入参‘picLen’不一定是3的倍数。
 *                所以单次调用后，剩余的没有使用到的字节数据保存在“base64_out”，其长度为“deCodeBase64Cnt”
 *                当显示完第一张图片后，显示第二张图时，需要清除一下这两个数据，防止影响第二张图片的显示
 *                true -- 清除历史数据 （“base64_out”，“deCodeBase64Cnt”），
 *                false -- 不动作
 */
bool gcodePicGetDataFormBase64(char * buf, unsigned long picLen, bool resetFlag)
{
    char base64_in[4];          // 保存base64编码的数组
    static unsigned char base64_out[3] = {'0'};   // 保存base64解码的数组
    int getBase64Cnt = 0;                      // 从U盘获取的，base64编码的数据
    static int deCodeBase64Cnt = 0;     // 已经解码得了数据
    unsigned long deCodePicLenCnt = 0;          // 保存已经获取的图片数据
    static char lCmdBuf[100];
    bool getPicEndFlag = false;


    if (ENABLED(USER_LOGIC_DEUBG)) 
        //SERIAL_ECHOLNPAIR("\r\n gcodePicGetDataFormBase64(...), .deCodeBase64Cnt = ", deCodeBase64Cnt,
        //                    "\r\n gcodePicGetDataFormBase64(...), .deCodePicLenCnt = ", deCodePicLenCnt,
        //                    "\r\n gcodePicGetDataFormBase64(...), .picLen = ", picLen);

    //  清除上次记录
    if (resetFlag)
    {
        for (unsigned int i = 0; i < sizeof(base64_out); i++)
            //base64_out[i] = '0x00';
            base64_out[i] = '\0';
        deCodeBase64Cnt = 0;
        return true;
    }


    if ((deCodeBase64Cnt > 0) && (deCodePicLenCnt < picLen))
    {
        if (ENABLED(USER_LOGIC_DEUBG)) 
        {
            SERIAL_ECHO("\r\n There are parameters left last time ");
            memset(lCmdBuf, 0, sizeof(lCmdBuf));
            sprintf(lCmdBuf, "\r\n ------------------------------deCodeBase64Cnt = %d; base64_out[3 - deCodeBase64Cnt] = %x",deCodeBase64Cnt,base64_out[3 - deCodeBase64Cnt]);
            SERIAL_ECHO(lCmdBuf);
        }

        for (int deCode = deCodeBase64Cnt; deCode > 0; deCode--)
        {
            if (deCodePicLenCnt < picLen)
            {
                buf[deCodePicLenCnt++] = base64_out[3 - deCode];
            } else {
                break;
            }
        }

    }

    if (ENABLED(USER_LOGIC_DEUBG)) 
        //SERIAL_ECHOLNPAIR("\r\n gcodePicGetDataFormBase64(...), ..deCodeBase64Cnt = ", deCodeBase64Cnt,
        //                    "\r\n gcodePicGetDataFormBase64(...), ..deCodePicLenCnt = ", deCodePicLenCnt);

    
    while(deCodePicLenCnt < picLen)
    {
        char j, ret;
        for ( j = 0; j < 20; j++)
        {
            ret = card.get();   // 从U盘中获取一个字符 
            
            if (ret == ';' || ret == ' ' || ret == '\r' || ret == '\n') 
                continue;

            base64_in[getBase64Cnt++] = ret;
            if (getBase64Cnt >= 4)  {
                getBase64Cnt = 0;
                break;
            }
            
        }
        
        memset(base64_out, 0, sizeof(base64_out));
        deCodeBase64Cnt = base64_decode(base64_in, 4, base64_out);
        for(int i = deCodeBase64Cnt; i < 3; i++)
            base64_out[i] = 0;
        deCodeBase64Cnt = 3;  // 这里强制给3，因为始终是4 --> 3 字符

        // if (ENABLED(USER_LOGIC_DEUBG)) {
        //     memset(lCmdBuf, 0, sizeof(lCmdBuf));
        //     sprintf(lCmdBuf, "\r\n deCodePicLenCnt = %d ;in = %s; ", deCodePicLenCnt, base64_in);
        //     SERIAL_ECHO(lCmdBuf);
        // }
        int test = deCodeBase64Cnt;
        for (int deCode = 0; deCode < test; deCode++)
        {
            if (deCodePicLenCnt < picLen)
            {
                // 特殊处理一下末尾字符，找到了FF D9后退出
                if (getPicEndFlag)
                    buf[deCodePicLenCnt++] = 0;
                else
                    buf[deCodePicLenCnt++] = base64_out[deCode];

                if (deCodePicLenCnt > 2 && \
                    ((buf[deCodePicLenCnt-1] == 0xD9 && buf[deCodePicLenCnt-2] == 0xFF) || (buf[deCodePicLenCnt-1] == 0xd9 && buf[deCodePicLenCnt-2] == 0xff)))
                    {
                        getPicEndFlag = true;
                        //if (ENABLED(USER_LOGIC_DEUBG)) 
                            //SERIAL_ECHOLNPAIR("\r\n ---------------- deCodePicLenCnt = ", deCodePicLenCnt);
                    }

                deCodeBase64Cnt--;
            } else {
                break;
            }
        }

        hal.watchdog_refresh();
    }

    if (ENABLED(USER_LOGIC_DEUBG)) 
        //SERIAL_ECHOLNPAIR("\r\n gcodePicGetDataFormBase64(...), ....deCodePicLenCnt = ", deCodePicLenCnt);
        
    return true;
}

/**
 * @功能   从gcode里面读取jpeg图片显示：1、发送到屏显示；2、让指针跳过这段图片，再去寻找下一张图片
 * @Author Creality
 * @Time   2021-12-01
 * picLenth     : 图片长度(base64编码的长度)
 * isDisplay    : 是否显示该图片
 * jpgAddr      : 显示图片的地址
 */
bool gcodePicDataRead(unsigned long picLenth, char isDisplay, unsigned long jpgAddr)
{
    //          96*96-耗时-Ms  200*200-耗时-Ms
    //  * 2  :      1780        8900 
    //  * 4  :      940         4490 
    //  * 8  :      518         2010 
    //  * 12 :      435         1300 
    //  * 16 :      420         1130 
    #define PIN_BUG_LEN_DACAI   2048 
    #define PIN_BUG_LEN_DWIN    (JPG_BYTES_PER_FRAME * 12)
    #define PIN_DATA_LEN_DWIN   (PIN_BUG_LEN_DWIN / 2)

    static char picBuf[PIN_BUG_LEN_DWIN+1]; // 这个取 MXA(PIN_BUG_LEN_DACAI, PIN_BUG_LEN_DWIN)
    
    unsigned long picLen;                   // 图片长度(解码后的长度)
    unsigned long j;

    picLen = picLenth;//(picLenth / 4) * 3; 
    if (ENABLED(USER_LOGIC_DEUBG)) 
        //SERIAL_ECHOLNPAIR("\r\n gcodePicDataRead(...), picLenth = ", picLenth,
        //                    "\r\n gcodePicDataRead(...), picLen = ", picLen);

    gcodePicGetDataFormBase64(picBuf, 0, true);

    // 迪文
    // 先给首地址写0，否则dwin会卡死
    DWIN_WriteOneWord(jpgAddr, 0);

    //开始读取
    for (j = 0; j < (picLen / PIN_BUG_LEN_DWIN); j++)
    {
        memset(picBuf, 0, sizeof(picBuf));
        // card.read(picBuf, PIN_BUG_LEN_DWIN);
        gcodePicGetDataFormBase64(picBuf, PIN_BUG_LEN_DWIN, false);
        rtscheck.RTS_SndData((j % 8) + 1, DOWNLOAD_PREVIEW_VP); //出现加载图片
        // 发送图片数据到指定地址
        if (isDisplay) {
            DWIN_SendJpegDate(picBuf, PIN_BUG_LEN_DWIN, (2 + jpgAddr + PIN_DATA_LEN_DWIN * j));
        }
    }
    rtscheck.RTS_SndData(0, DOWNLOAD_PREVIEW_VP);
    // 剩下的不足240字符的数据处理，根据迪文处理内容
    // watchdog_refresh();
    if (picLen % PIN_BUG_LEN_DWIN != 0)
    {
        memset(picBuf, 0, sizeof(picBuf));
        // card.read(picBuf, (picLen - PIN_BUG_LEN_DWIN * j));  
        gcodePicGetDataFormBase64(picBuf, (picLen - PIN_BUG_LEN_DWIN * j), false);
        // 发送图片数据到指定地址
        if (isDisplay) {
            DWIN_SendJpegDate(picBuf, (picLen - PIN_BUG_LEN_DWIN * j), (2 + jpgAddr + PIN_DATA_LEN_DWIN * j));
        }
    }
    // delay(25);
    // 用于显示jpg图片
    if (isDisplay)
        DWIN_DisplayJpeg(jpgAddr, picLen);

    return true;
}

/**
 * @功能   gcode预览图存在判断
 * @Author Creality
 * @Time   2021-12-01
 * fileName         gcode文件名
 * jpgAddr          显示地址
 * jpgFormat        图片类型（jpg、png）
 * jpgResolution    图片大小
 */
static uint32_t msTest;
char gcodePicExistjudge(char *fileName, unsigned int targitPicAddr, const char targitPicFormat, const char targitPicResolution)
{
    #define STRING_MAX_LEN      80
					
	// unsigned char picFormat = PIC_FORMAT_MAX;		// 图片格式
	unsigned char picResolution = PIC_RESOLITION_MAX;// 图片分辨率
	unsigned char ret;

    unsigned char strBuf[STRING_MAX_LEN] = {0};
    unsigned char bufIndex = 0;
    // char lCmdBuf[20];
    char *picMsgP; 

    char lPicFormar[20];
    char lPicHeder[20];
    // char lPicResolution[20];
    unsigned long picLen = 0;		// 图片数据长度
    unsigned int picStartLine = 0;	// 图片起始行
    unsigned int picEndLine = 0;	// 图片结束行
    unsigned int picHigh = 0;	    // 图片模型高度
    

    // 读取一个字符串，以空格隔开
    #define GET_STRING_ON_GCODE()
        {
            // 读取一行，以换行符隔开
            memset(strBuf, 0, sizeof(strBuf));
            int strLenMax;
            bool strStartFg = false;
            uint8_t curBufLen = 0;
            uint8_t inquireYimes = 0;   // 查找次数
            do {
                for (strLenMax = 0; strLenMax < STRING_MAX_LEN; strLenMax++)
                {
                    ret = card.get();   // 从U盘中获取一个字符
                    if (ret != ';' && strStartFg == false) { // 读到';'为一行的开始
                    SERIAL_ECHO("preview.cpp GET_STRING_ON_GCODE continue card.get");                      
                        continue;
                    } else{
                    SERIAL_ECHO("preview.cpp GET_STRING_ON_GCODE strStartFg = true;");                                              
                        strStartFg = true;
                    }                        
                    if ((ret == '\r' || ret == '\n') && bufIndex != 0) break;   // 读到换行符，退出
                    strBuf[bufIndex++] = ret;
                }
                if (strLenMax >= STRING_MAX_LEN) {
                    SERIAL_ECHO_MSG("strLenMax: ", strLenMax);  
                    SERIAL_ECHO_MSG("STRING_MAX_LEN :", STRING_MAX_LEN);
                    SERIAL_ECHO_MSG("current srting lenth more than STRING_MAX_LEN(60)");
                    SERIAL_ECHO("preview.cpp PIC_MISS_ERR STRING_MAX_LEN");                    
                    return PIC_MISS_ERR;
                }
                curBufLen = sizeof(strBuf);
                if (inquireYimes++ >= 5)
                {
                    SERIAL_ECHO_MSG("inquireYimes more than5 times");
                    SERIAL_ECHO("preview.cpp PIC_MISS_ERR inquireYimes more than5 times");                                        
                    return PIC_MISS_ERR;
                }
            }while(curBufLen < 20);

            //SERIAL_ECHO_MSG("strBuf = ", strBuf);
            //SERIAL_ECHO_MSG("curBufLen = ", curBufLen);
        }

    // 1、读出一行数据
    GET_STRING_ON_GCODE();

    // 2、进行图片的格式判断（jpg、png），如果不符合，直接退出
    if ( targitPicFormat == PIC_FORMAT_JPG)
    {
        if ( strstr((const char *)strBuf, FORMAT_JPG_HEADER ) == NULL){
            SERIAL_ECHO_MSG("strbuf: ", (const char*)strBuf);         
            SERIAL_ECHO("preview.cpp PIC_MISS_ERR FORMAT_JPG_HEADER1 ");                                                
            return PIC_MISS_ERR;
        }
    }
    else
    {
        if ( strstr((const char *)strBuf, FORMAT_JPG_HEADER ) == NULL){
                    SERIAL_ECHO("preview.cpp PIC_MISS_ERR FORMAT_JPG_HEADER2 ");         
            return PIC_MISS_ERR;
        }
    }

    // 3、获取字符串的图片格式内容
    picMsgP = strtok((char *)strBuf, (const char *)" ");
    do {
        if ( ENABLED(USER_LOGIC_DEUBG) )  SERIAL_ECHO_MSG("3.picMsgP = ", picMsgP);

        if ( picMsgP == NULL )
        {
            SERIAL_ECHO_MSG("fine the lPicFormar err!");
            SERIAL_ECHO("preview.cpp PIC_MISS_ERR ocMsgP NULL");             
            return PIC_MISS_ERR;
        }

        if (picMsgP != NULL && \
           (strstr((const char *)picMsgP, FORMAT_JPG) != NULL || strstr((const char *)picMsgP, FORMAT_PNG) != NULL)) break;

        picMsgP = strtok(NULL, (const char *)" ");
    }while(1);

    // 4、获取“start”字段
    picMsgP = strtok(NULL, (const char *)" ");
    if ( ENABLED(USER_LOGIC_DEUBG) )  SERIAL_ECHO_MSG("4.picMsgP = ", picMsgP,  " strlen(picMsgP) = ", strlen(picMsgP));
    if ( picMsgP != NULL )
    {
        memset(lPicHeder, 0, sizeof(lPicHeder));
        memcpy(lPicHeder, picMsgP, strlen(picMsgP));
    }

    // 5、获取图片大小字段 200*200 300*300 ……
    picMsgP = strtok(NULL, (const char *)" ");
    if ( ENABLED(USER_LOGIC_DEUBG) )  SERIAL_ECHO_MSG("5.picMsgP = ", picMsgP, " strlen(picMsgP) = ", strlen(picMsgP));
    if ( picMsgP != NULL )
    {
        picResolution = PIC_RESOLITION_MAX;
        if (strcmp(picMsgP, RESOLITION_36_36) == 0) {
            picResolution = PIC_RESOLITION_36_36;
        } else if (strcmp(picMsgP, RESOLITION_48_48) == 0) {
            picResolution = PIC_RESOLITION_48_48;
        } else if (strcmp(picMsgP, RESOLITION_64_64) == 0) {
            picResolution = PIC_RESOLITION_64_64;
        } else if (strcmp(picMsgP, RESOLITION_96_96) == 0) {
            picResolution = PIC_RESOLITION_96_96;
        } else if (strcmp(picMsgP, RESOLITION_144_144) == 0) {
            picResolution = PIC_RESOLITION_144_144;
        } else if (strcmp(picMsgP, RESOLITION_200_200) == 0) {
            picResolution = PIC_RESOLITION_200_200;
        } else if (strcmp(picMsgP, RESOLITION_300_300) == 0) {
            picResolution = PIC_RESOLITION_300_300;
        } else if (strcmp(picMsgP, RESOLITION_600_600) == 0) {
            picResolution = PIC_RESOLITION_600_600;
        }
    }
    

    // 6、获取图片数据长度
    picMsgP = strtok(NULL, (const char *)" ");
    if ( ENABLED(USER_LOGIC_DEUBG) )  SERIAL_ECHO_MSG("6.picMsgP = ", picMsgP);
    if ( picMsgP != NULL )
    {
        picLen = atoi(picMsgP);
    }

    // 7、获取图片的起始行
    picMsgP = strtok(NULL, (const char *)" ");
    if ( ENABLED(USER_LOGIC_DEUBG) )  SERIAL_ECHO_MSG("7.picMsgP = ", picMsgP);
    if ( picMsgP != NULL )
    {
        picStartLine = atoi(picMsgP);
    }

    // 8、获取图片的结束行
    picMsgP = strtok(NULL, (const char *)" ");
    if ( ENABLED(USER_LOGIC_DEUBG) )  SERIAL_ECHO_MSG("8.picMsgP = ", picMsgP);
    if ( picMsgP != NULL )
    {
        picEndLine = atoi(picMsgP);
    }

    // 9、获取模型的高度
    picMsgP = strtok(NULL, (const char *)" ");
    if ( ENABLED(USER_LOGIC_DEUBG) )  SERIAL_ECHO_MSG("9.picMsgP = ", picMsgP);
    if ( picMsgP != NULL )
    {
        picHigh = atoi(picMsgP);
    }

    if (ENABLED(USER_LOGIC_DEUBG)) 
    {
        SERIAL_ECHO_MSG("lPicFormar = ", lPicFormar);
        SERIAL_ECHO_MSG("lPicHeder = ", lPicHeder);
        SERIAL_ECHO_MSG("picResolution = ", picResolution);
        SERIAL_ECHO_MSG("picLen = ", picLen);
        SERIAL_ECHO_MSG("picStartLine = ", picStartLine);
        SERIAL_ECHO_MSG("picEndLine = ", picEndLine);
        SERIAL_ECHO_MSG("picHigh = ", picHigh);
    }
    

    if (ENABLED(USER_LOGIC_DEUBG)) 
    {
        //SERIAL_ECHOPAIR("\r\n gcode pic time test 1 msTest = ", (millis() - msTest));
        msTest = millis();
    }

    // 从gcode里面读出图片数据，根据选择的是不是预定格式或预定大小图片来判断是否需要发送到屏上
    
    // 判断是否是需要的 分辨率
    if ( picResolution == targitPicResolution )
    {
        gcodePicDataRead(picLen, true, targitPicAddr);
    }
    else
    {
        // 直接移动指针，跳过无效的图片
        // 协议规定完整一行数据：';' + ' ' + "数据" + '\n'  1+1+76+1 = 79字节
        // 最后一行为“; png end\r” 或 “; jpg end\r”,
        uint32_t index1 = card.getFileCurPosition();//card.getIndex();
        uint32_t targitPicpicLen = 0;
        if ( picLen % 3 == 0 ) {
            targitPicpicLen = picLen / 3 * 4;
        } else {
            targitPicpicLen = (picLen / 3 + 1) * 4; 
        }
        uint32_t indexAdd = (targitPicpicLen / 76) * 3 + targitPicpicLen + 10;
        if ( (targitPicpicLen % 76 ) != 0) {
            indexAdd += 3;
        }

        card.setIndex((index1 + indexAdd));
        if ( ENABLED(USER_LOGIC_DEUBG) ) 
        {
            //SERIAL_ECHOLNPAIR("\r\n ...old_index1 = ", index1,
            //                  "\r\n ...indexAdd = ", indexAdd);
        }

        if ( picResolution != targitPicResolution )
            return PIC_RESOLITION_ERR;
        else
            return PIC_FORMAT_ERR;
    }

    //card.closefile();
    if ( ENABLED(USER_LOGIC_DEUBG) ) 
	    //SERIAL_ECHOPAIR("\r\n gcode pic time test 3 msTest = ", (millis() - msTest));
    msTest = millis();
	return PIC_OK;
}

/**
 * @功能   gcode预览图发送到迪文
 * @Author Creality
 * @Time   2021-12-01
 * fileName     gcode文件名
 * jpgAddr      显示地址
 * jpgFormat    图片类型（jpg、png）
 * jpgResolution     图片大小

		#define RESOLITION_48_48    "48*48"
		#define RESOLITION_200_200  "200*200"
		
		#define RESOLITION_96_96    "96*96"
		#define RESOLITION_300_300  "300*300"

 */
char gcodePicDataSendToDwin(char *fileName, unsigned int jpgAddr, unsigned char jpgFormat, unsigned char jpgResolution)
{
    char ret;
    char returyCnt = 0;
    card.openFileRead(fileName);
    msTest = millis();
    while (1)
    {
        ret = gcodePicExistjudge(fileName, jpgAddr, jpgFormat, jpgResolution);
        if (ret == PIC_MISS_ERR) // 当gcode中没有pic时，直接返回
        {
            card.closefile();
            SERIAL_ECHO("preview.cpp file closed");
            return PIC_MISS_ERR;
        }
        else if ((ret == PIC_FORMAT_ERR) || (ret == PIC_RESOLITION_ERR)) // 当格式或大小错误，继续往下判断
        {
            if (++returyCnt >= 3)
            {
                card.closefile();
            SERIAL_ECHO("preview.cpp file closed pic format error");                
                return PIC_MISS_ERR;
            }
            else
                continue;
        }
        else 
        {
            card.closefile();
            SERIAL_ECHO("preview.cpp file pic ok");                            
            return PIC_OK;
        }
    }

}

/**
 * @功能   gcode预览图显示、隐藏
 * @Author Creality
 * @Time   2021-0-27
 * jpgAddr      地址
 * onoff        显示(onoff == true)，隐藏(onoff == false)
 * 显示地址
 */
void gcodePicDispalyOnOff(unsigned int jpgAddr, bool onoff)
{
    if (onoff) {
        rtscheck.RTS_SndData(1, jpgAddr);  
    } else {
        rtscheck.RTS_SndData(0, jpgAddr);
    }
}
  // 亮度控制功能函
  void DWIN_BrightnessCtrl(DwinBrightness_t brightness)
  {

    unsigned int buf[10];

    buf[0] = brightness.LeftUp_X;     // 亮度
    buf[1] = brightness.LeftUp_Y;
    buf[2] = brightness.RightDown_X;     // 亮度
    buf[3] = brightness.RightDown_Y;

    // 显示区域修改
    DWIN_WriteOneWord(brightness.spAddr, buf[0]);
    DWIN_WriteOneWord(brightness.spAddr + 1, buf[1]);
    DWIN_WriteOneWord(brightness.spAddr + 2, buf[2]);
    DWIN_WriteOneWord(brightness.spAddr + 3, buf[3]);

    // 亮度调节
    DWIN_WriteOneWord(brightness.addr, brightness.brightness);
  }
/*
 * [RefreshBrightnessAtPrint :刷新打印中，gcode预览图片的渐变显示]
 * @Author Creality
 * @Time   2021-06-19
 */
void RefreshBrightnessAtPrint(uint16_t persent)
{
    printBri.brightness = BRIGHTNESS_PRINT;
    printBri.addr = BRIGHTNESS_ADDR_PRINT;
    printBri.spAddr = SP_ADDR_BRIGHTNESS_PRINT + 1;
    printBri.LeftUp_X = BRIGHTNESS_PRINT_LEFT_HIGH_X;
    printBri.LeftUp_Y = BRIGHTNESS_PRINT_LEFT_HIGH_Y;
    printBri.RightDown_X = BRIGHTNESS_PRINT_LEFT_HIGH_X + BRIGHTNESS_PRINT_WIDTH;
    printBri.RightDown_Y = BRIGHTNESS_PRINT_LEFT_HIGH_Y + (100 - persent) * BRIGHTNESS_PRINT_HIGH / 100;

    DWIN_BrightnessCtrl(printBri);
    
}