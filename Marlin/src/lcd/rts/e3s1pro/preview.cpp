/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "../../../inc/MarlinConfig.h"

#if ENABLED(E3S1PRO_RTS)

#include "preview.h"
#include "lcd_rts.h"
#include "base64.h"
#include "utf8_unicode.h"

#include <stdio.h>
#include <arduino.h>
#include <wstring.h>
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

#define BRIGHTNESS_PRINT_HIGH    300        // The total height of the progress bar
#define BRIGHTNESS_PRINT_WIDTH   300        // The total width of the progress bar
#define BRIGHTNESS_PRINT_LEFT_HIGH_X    186 // The upper left corner of the progress bar -x
#define BRIGHTNESS_PRINT_LEFT_HIGH_Y    70  // The upper left corner of the progress bar-y
#define BRIGHTNESS_PRINT    120             // Brightness value (0 is the darker)
#define FORMAT_JPG_HEADER "jpg begin"
#define FORMAT_PNG_HEADER "png begin"
#define FORMAT_JPG "jpg"
#define FORMAT_PNG "png"
DwinBrightness_t printBri; // Preview graph structure
#define JPG_BYTES_PER_FRAME 240 // The number of bytes sent each frame (picture data)
#define JPG_WORD_PER_FRAME  (JPG_BYTES_PER_FRAME / 2)// The number of words sent each frame (picture data)
#define SizeofDatabuf2    300
#define USER_LOGIC_DEBUG 1

#ifdef LCD_SERIAL_PORT
  #define LCDSERIAL LCD_SERIAL
#elif SERIAL_PORT_2
  #define LCDSERIAL MYSERIAL2
#endif

/**
 * Dimming
 * brightness addr
 * in range: 0x8800 ~ 0x8FFF
 */
#define BRIGHTNESS_ADDR_PRINT               0x8800
unsigned char databuf[SizeofDatabuf2];

// Write two bytes to the specified address space
void dwinWriteOneWord(unsigned long addr, unsigned int data) {
  rts.sendData(data, addr, VarAddr_W);
}

void dwin_uart_write(unsigned char *buf, int len) {
  for (uint8_t n = 0; n < len; ++n) LCDSERIAL.write(buf[n]);
}

// Send a frame of data for JPG pictures
void RTS_SendJpegData(const char *str, unsigned long addr, unsigned char cmd /*= VarAddr_W*/) {
  int len = JPG_BYTES_PER_FRAME;// strlen(str);
  if (len > 0) {
    databuf[0] = FHONE;
    databuf[1] = FHTWO;
    databuf[2] = 3 + len;
    databuf[3] = cmd;
    databuf[4] = addr >> 8;
    databuf[5] = addr & 0x00FF;
    for (int i = 0; i < len; i++) databuf[6 + i] = str[i];

    dwin_uart_write(databuf, len + 6);

    ZERO(databuf);
  }
}

// Display jpg pictures
void dwinDisplayJpeg(unsigned long addr, unsigned long vp) {
  unsigned char buf[10];
  buf[0] = 0x5A;
  buf[1] = 0xA5;
  buf[2] = 0x07;
  buf[3] = 0x82;
  buf[4] = addr >> 8;   // Control address
  buf[5] = addr & 0x00FF;
  buf[6] = 0x5A;
  buf[7] = 0xA5;
  buf[8] = vp >> 8;     // Image storage address
  buf[9] = vp & 0x00FF;

  dwin_uart_write(buf, 10);
}

// Send JPG pictures whole package data
void dwinSendJpegData(char *jpeg, unsigned long size, unsigned long jpgAddr) {
  int jpgSize = size;

  char buf[JPG_BYTES_PER_FRAME];
  int i, j;

  for (i = 0; i < jpgSize / JPG_BYTES_PER_FRAME; i++) {
    //delay(20);
    //memset(buf, 0, JPG_BYTES_PER_FRAME);
    memcpy(buf, &jpeg[i * JPG_BYTES_PER_FRAME], JPG_BYTES_PER_FRAME);
    hal.watchdog_refresh();

    // Send picture data to the specified address
    RTS_SendJpegData(buf, (jpgAddr + (JPG_WORD_PER_FRAME * i)), 0x82);

    #if ENABLED(DWIN_DEBUG)
      for (j = 0; j < JPG_BYTES_PER_FRAME; j++) {
        //SERIAL_ECHOPGM(" ", j,
        //               " = ", buf[j]);
        //if ((j + 1) % 8 == 0) SERIAL_EOL();
      }
    #endif

    // Dwin to the 7 -inch screen of Dwin. After sending a preview diagram a frame of data, there is no return value
    // So the abnormality will not be judged first,
    #define CMD_TAILA    0x03824F4B    // Frame tail (currently valid for the 4.3 -inch DWin screen)
    uint8_t receivedbyte = 0;
    uint8_t cmd_pos      = 0; // Current instruction pointer status
    uint32_t cmd_state   = 0; // Quest frame tail detection status
    char buffer[20]      = {0};
    const uint32_t ms = millis() + 25;
    while (1) {
      if (LCDSERIAL.available()) {
        // Take a data
        receivedbyte = LCDSERIAL.read();
        SERIAL_ECHOLNPGM("receivedbyte = ", receivedbyte);
        if (cmd_pos == 0 && receivedbyte != 0x5A)  // The first byte of the instruction must be a frame header
          continue;

        if (cmd_pos >= 20) break;

        buffer[cmd_pos++] = receivedbyte;   // Prevents overflow

        cmd_state = ((cmd_state << 8) | receivedbyte); // Stitch the last 4 bytes, form the last 32 -bit integer

        // Frame judgment
        if (cmd_state == CMD_TAILA) break;
      }
      if (ELAPSED(millis(), ms)) { // According to the data manual, the delay of 20ms, there is a phenomenon that there is a probability that it cannot be brushed out of the preview map !!!
        //PRINT_LOG("more than 25ms");
        break;
      }
    }

  }

  if (jpgSize % JPG_BYTES_PER_FRAME) {
    memset(buf, 0, JPG_BYTES_PER_FRAME);
    memcpy(buf, &jpeg[i * JPG_BYTES_PER_FRAME], (jpgSize - i * JPG_BYTES_PER_FRAME));
    hal.watchdog_refresh();

    // Send picture data to the specified address
    RTS_SendJpegData(buf, (jpgAddr + (JPG_WORD_PER_FRAME * i)), 0x82);

    #if ENABLED(DWIN_DEBUG)
      for (j = 0; j < JPG_BYTES_PER_FRAME; j++) {
        //SERIAL_ECHOPGM(" ", j,
        //               " = ", buf[j]);
        //if ((j + 1) % 8 == 0) SERIAL_EOL();
      }
    #endif

    delay(25); // According to the data manual, the delay of 20ms, there is a phenomenon that there is a probability that it cannot be brushed out of the preview map !!!
  }
}

/**
 * @F function reads preview data from the U disk and decodes
 * @Author Creality
 * @Time   2022-04-13
 * buf          : Used to save the decoding data
 * picLen       : Data length required
 * resetFlag    : Reset the data logo -Since the base64 decoding is the multiple of 3 (4 Base64 character decoding is 4 byte data), but the parameter ‘piclen’ is not necessarily a multiple of 3.
 *                So after a single call, the remaining byte data is stored in "Base64_OUT", and its length is "DecodeBase64CNT"
 *                When the first picture is displayed, the second picture is displayed, and you need to clear these two data to prevent the display of the display of the second picture.
 *                true - Clear historical data ("Base64_out", "DecodeBase64cnt"),
 *                false - No action
 */
bool gcodePicGetDataFormBase64(char * buf, unsigned long picLen, bool resetFlag) {
  char base64_in[4];                            // Save the base64 encoding array
  static unsigned char base64_out[3] = {'0'};   // Save the base64 decoding array
  int getBase64Cnt                   = 0;       // Data obtained from the USB flash drive, the data encoded by Base64
  static int deCodeBase64Cnt         = 0;       // I have decoded the data
  unsigned long deCodePicLenCnt      = 0;       // Save the picture data that has been obtained
  static char lCmdBuf[100];
  bool getPicEndFlag = false;


  #if ENABLED(USER_LOGIC_DEBUG)
    //SERIAL_ECHOLNPGM("\r\n gcodePicGetDataFormBase64(...), .deCodeBase64Cnt = ", deCodeBase64Cnt,
    //                 "\r\n gcodePicGetDataFormBase64(...), .deCodePicLenCnt = ", deCodePicLenCnt,
    //                 "\r\n gcodePicGetDataFormBase64(...), .picLen = ", picLen);
  #endif

  //  清除上次记录
  if (resetFlag) {
    for (unsigned int i = 0; i < sizeof(base64_out); i++)
      // base64_out[i] = '0x00';
      base64_out[i] = '\0';
    deCodeBase64Cnt = 0;
    return true;
  }

  if ((deCodeBase64Cnt > 0) && (deCodePicLenCnt < picLen)) {
    #if ENABLED(USER_LOGIC_DEBUG)
      SERIAL_ECHOLNPGM("\r\n There are parameters left last time ");
      ZERO(lCmdBuf);
      sprintf(lCmdBuf, "\r\n ------------------------------deCodeBase64Cnt = %d; base64_out[3 - deCodeBase64Cnt] = %x", deCodeBase64Cnt, base64_out[3 - deCodeBase64Cnt]);
      SERIAL_ECHOLNPGM(lCmdBuf);
    #endif

    for (int deCode = deCodeBase64Cnt; deCode > 0; deCode--) {
      if (deCodePicLenCnt >= picLen) break;
      buf[deCodePicLenCnt++] = base64_out[3 - deCode];
    }
  }

  #if ENABLED(USER_LOGIC_DEBUG)
    //SERIAL_ECHOLNPGM("\r\n gcodePicGetDataFormBase64(...), ..deCodeBase64Cnt = ", deCodeBase64Cnt,
    //                 "\r\n gcodePicGetDataFormBase64(...), ..deCodePicLenCnt = ", deCodePicLenCnt);
  #endif

  while (deCodePicLenCnt < picLen) {
    char j, ret;
    for (j = 0; j < 20; j++) {
      ret = card.get(); // Get a character from the USB flash drive
      if (ret == ';' || ret == ' ' || ret == '\r' || ret == '\n') continue;
      base64_in[getBase64Cnt++] = ret;
      if (getBase64Cnt >= 4) {
        getBase64Cnt = 0;
        break;
      }
    }

    ZERO(base64_out);
    deCodeBase64Cnt = base64_decode(base64_in, 4, base64_out);
    for (int i = deCodeBase64Cnt; i < 3; i++) base64_out[i] = 0;
    deCodeBase64Cnt = 3; // Here forced 3, because it is always 4--> 3 characters

    //#if ENABLED(USER_LOGIC_DEBUG)
    //  ZERO(lCmdBuf);
    //  sprintf(lCmdBuf, "\r\n deCodePicLenCnt = %d ;in = %s; ", deCodePicLenCnt, base64_in);
    //  SERIAL_ECHOLN(lCmdBuf);
    //#endif
    int test = deCodeBase64Cnt;
    for (int deCode = 0; deCode < test; deCode++) {
      if (deCodePicLenCnt >= picLen) break;

      // Special processing the end characters, after finding the FF D9, exit
      buf[deCodePicLenCnt++] = getPicEndFlag ? 0 : base64_out[deCode];

      if (deCodePicLenCnt > 2 && \
          ((buf[deCodePicLenCnt - 1] == 0xD9 && buf[deCodePicLenCnt - 2] == 0xFF) || (buf[deCodePicLenCnt - 1] == 0xd9 && buf[deCodePicLenCnt - 2] == 0xff)))
        getPicEndFlag = true;
        //#if ENABLED(USER_LOGIC_DEBUG)
        //  SERIAL_ECHOLNPGM("\r\n ---------------- deCodePicLenCnt = ", deCodePicLenCnt);
        //#endif

      deCodeBase64Cnt--;
    }

    hal.watchdog_refresh();
  }

  #if ENABLED(USER_LOGIC_DEBUG)
    //SERIAL_ECHOLNPGM("\r\n gcodePicGetDataFormBase64(...), ....deCodePicLenCnt = ", deCodePicLenCnt);
  #endif

  return true;
}

/**
 *@Function read JPEG pictures from GCODE: 1. Send to the screen display; 2. Let the pointer be skipped this picture, and then find the next picture
 * @Author Creality
 * @Time   2021-12-01
 * picLenth     : Picture length (base64 encoding length)
 * isDisplay    : Whether to display the picture
 * jpgAddr      : Display the address of the picture
 */
bool gcodePicDataRead(unsigned long picLenth, char isDisplay, unsigned long jpgAddr) {
  //          96*96-耗时-Ms  200*200-耗时-Ms
  //  * 2  :      1780        8900
  //  * 4  :      940         4490
  //  * 8  :      518         2010
  //  * 12 :      435         1300
  //  * 16 :      420         1130
  #define PIN_BUG_LEN_DACAI   2048
  #define PIN_BUG_LEN_DWIN    (JPG_BYTES_PER_FRAME * 12)
  #define PIN_DATA_LEN_DWIN   (PIN_BUG_LEN_DWIN / 2)

  static char picBuf[PIN_BUG_LEN_DWIN + 1]; // This take mxa (PIN_BUG_LEN_DACAI, PIN_BUG_LEN_DWIN))

  unsigned long picLen;                 // Picture length (the length after decoding)
  unsigned long j;

  picLen = picLenth;// (picLenth / 4) * 3;
  #if ENABLED(USER_LOGIC_DEBUG)
    //SERIAL_ECHOLNPGM("\r\n gcodePicDataRead(...), picLenth = ", picLenth,
    //                 "\r\n gcodePicDataRead(...), picLen = ", picLen);
  #endif

  gcodePicGetDataFormBase64(picBuf, 0, true);

  // Diwen
  // Write 0 to the first address first, otherwise Dwin will be stuck to death
  dwinWriteOneWord(jpgAddr, 0);

  // Start reading
  for (j = 0; j < (picLen / PIN_BUG_LEN_DWIN); j++) {
    ZERO(picBuf);
    //card.read(picBuf, PIN_BUG_LEN_DWIN);
    gcodePicGetDataFormBase64(picBuf, PIN_BUG_LEN_DWIN, false);
    rts.sendData((j % 8) + 1, DOWNLOAD_PREVIEW_VP); // Load pictures
    // Send picture data to the specified address
    if (isDisplay) dwinSendJpegData(picBuf, PIN_BUG_LEN_DWIN, (2 + jpgAddr + PIN_DATA_LEN_DWIN * j));
  }
  rts.sendData(0, DOWNLOAD_PREVIEW_VP);

  // Data processing of less than 240 characters remaining, according to Diwen processing content
  //watchdog_refresh();
  if (picLen % PIN_BUG_LEN_DWIN != 0) {
    ZERO(picBuf);
    // card.read(picBuf, (picLen - PIN_BUG_LEN_DWIN * j));
    gcodePicGetDataFormBase64(picBuf, (picLen - PIN_BUG_LEN_DWIN * j), false);
    // Send picture data to the specified address
    if (isDisplay)
      dwinSendJpegData(picBuf, (picLen - PIN_BUG_LEN_DWIN * j), (2 + jpgAddr + PIN_DATA_LEN_DWIN * j));
  }
  //delay(25);

  // Used to display jpg pictures
  if (isDisplay) dwinDisplayJpeg(jpgAddr, picLen);

  return true;
}

/**
 * @Function Gcode preview diagram There is judgment
 * @Author Creality
 * @Time   2021-12-01
 * fileName         Gcode file name
 * jpgAddr          Display address
 * jpgFormat        Image type (JPG. PNG)
 * jpgResolution    Image size
 */
static uint32_t msTest;
char gcodePicExistjudge(char *fileName, unsigned int targetPicAddr, const char targetPicFormat, const char targetPicResolution) {
  #define STRING_MAX_LEN      80

  // unsigned char picFormat = PIC_FORMAT_MAX;      // Image Format
  unsigned char picResolution = PIC_RESOLUTION_MAX; // Picture resolution
  unsigned char ret;

  unsigned char strBuf[STRING_MAX_LEN] = {0};
  unsigned char bufIndex               = 0;
  // char lCmdBuf[20];
  char *picMsgP;

  char lPicFormat[20];
  char lPicHeader[20];
  // char lPicResolution[20];
  unsigned long picLen      = 0; // Image data length
  unsigned int picStartLine = 0; // Starting line
  unsigned int picEndLine   = 0; // Ending line
  unsigned int picHigh      = 0; // High picture model

  // Read a string and separate it with a space
  #define GET_STRING_ON_GCODE() {                                                          \
    /* Read a line to separate the row symbol */                                           \
    ZERO(strBuf);                                                                          \
    int strLenMax;                                                                         \
    bool strStartFg = false;                                                               \
    uint8_t curBufLen = 0;                                                                 \
    uint8_t inquireYimes = 0;   /* Number */                                               \
    do {                                                                                   \
      for (strLenMax = 0; strLenMax < STRING_MAX_LEN; strLenMax++) {                       \
        ret = card.get();   /* Get a character from the USB flash drive */                 \
        if (ret != ';' && strStartFg == false) { /* Read the beginning of ';'*/            \
          SERIAL_ECHOLNPGM("preview.cpp GET_STRING_ON_GCODE continue card.get");           \
          continue;                                                                        \
        }                                                                                  \
        else {                                                                             \
          SERIAL_ECHOLNPGM("preview.cpp GET_STRING_ON_GCODE strStartFg = true;");          \
          strStartFg = true;                                                               \
        }                                                                                  \
        if ((ret == '\r' || ret == '\n') && bufIndex != 0) break; /* Read the change of lines, exit */ \
        strBuf[bufIndex++] = ret;                                                          \
      }                                                                                    \
      if (strLenMax >= STRING_MAX_LEN) {                                                   \
        SERIAL_ECHOLNPGM("strLenMax: ", strLenMax);                                        \
        SERIAL_ECHOLNPGM("STRING_MAX_LEN :", STRING_MAX_LEN);                              \
        SERIAL_ECHOLNPGM("current srting lenth more than STRING_MAX_LEN(60)");             \
        SERIAL_ECHOLNPGM("preview.cpp PIC_MISS_ERR STRING_MAX_LEN");                       \
        return PIC_MISS_ERR;                                                               \
      }                                                                                    \
      curBufLen = sizeof(strBuf);                                                          \
      if (inquireYimes++ >= 5) {                                                           \
        SERIAL_ECHOLNPGM("inquireYimes more than5 times");                                 \
        SERIAL_ECHOLNPGM("preview.cpp PIC_MISS_ERR inquireYimes more than5 times");        \
        return PIC_MISS_ERR;                                                               \
      }                                                                                    \
    } while (curBufLen < 20);                                                              \
                                                                                           \
    /* SERIAL_ECHOLNPGM("strBuf = ", strBuf); */                                           \
    /* SERIAL_ECHOLNPGM("curBufLen = ", curBufLen); */                                     \
  }

  // 1. Read a line of data
  GET_STRING_ON_GCODE();

  // 2. Determine the format of the picture (JPG. PNG), if it does not meet, exit directly
  if (targetPicFormat == PIC_FORMAT_JPG) {
    if (strstr((const char *)strBuf, FORMAT_JPG_HEADER ) == nullptr) {
      SERIAL_ECHOLNPGM("strbuf: ", (const char*)strBuf);
      SERIAL_ECHOLNPGM("preview.cpp PIC_MISS_ERR FORMAT_JPG_HEADER1 ");
      return PIC_MISS_ERR;
    }
  }
  else if (strstr((const char *)strBuf, FORMAT_JPG_HEADER ) == nullptr)   {
    SERIAL_ECHOLNPGM("preview.cpp PIC_MISS_ERR FORMAT_JPG_HEADER2 ");
    return PIC_MISS_ERR;
  }

  // 3. Get the picture format content of the string
  picMsgP = strtok((char *)strBuf, (const char *)" ");
  for (;;) {
    if (ENABLED(USER_LOGIC_DEBUG)) SERIAL_ECHOLNPGM("3.picMsgP = ", picMsgP);

    if (picMsgP == nullptr) {
      SERIAL_ECHOLNPGM("fine the lPicFormat err!");
      SERIAL_ECHOLNPGM("preview.cpp PIC_MISS_ERR ocMsgP NULL");
      return PIC_MISS_ERR;
    }

    if (picMsgP != nullptr && \
        (strstr((const char *)picMsgP, FORMAT_JPG) != nullptr || strstr((const char *)picMsgP, FORMAT_PNG) != nullptr)) break;

    picMsgP = strtok(nullptr, (const char *)" ");
  }

  // 4. Get the "Start" field
  picMsgP = strtok(nullptr, (const char *)" ");
  if (ENABLED(USER_LOGIC_DEBUG)) SERIAL_ECHOLNPGM("4.picMsgP = ", picMsgP,  " strlen(picMsgP) = ", strlen(picMsgP));
  if (picMsgP != nullptr) {
    ZERO(lPicHeader);
    memcpy(lPicHeader, picMsgP, strlen(picMsgP));
  }

  // 5. Get the picture size field 200*200 300*300 ...
  picMsgP = strtok(nullptr, (const char *)" ");
  if (ENABLED(USER_LOGIC_DEBUG)) SERIAL_ECHOLNPGM("5.picMsgP = ", picMsgP, " strlen(picMsgP) = ", strlen(picMsgP));
  if (picMsgP != nullptr) {
    picResolution = PIC_RESOLUTION_MAX;
    if (strcmp(picMsgP, RESOLUTION_36_36) == 0)
      picResolution = PIC_RESOLUTION_36_36;
    else if (strcmp(picMsgP, RESOLUTION_48_48) == 0)
      picResolution = PIC_RESOLUTION_48_48;
    else if (strcmp(picMsgP, RESOLUTION_64_64) == 0)
      picResolution = PIC_RESOLUTION_64_64;
    else if (strcmp(picMsgP, RESOLUTION_96_96) == 0)
      picResolution = PIC_RESOLUTION_96_96;
    else if (strcmp(picMsgP, RESOLUTION_144_144) == 0)
      picResolution = PIC_RESOLUTION_144_144;
    else if (strcmp(picMsgP, RESOLUTION_200_200) == 0)
      picResolution = PIC_RESOLUTION_200_200;
    else if (strcmp(picMsgP, RESOLUTION_300_300) == 0)
      picResolution = PIC_RESOLUTION_300_300;
    else if (strcmp(picMsgP, RESOLUTION_600_600) == 0)
      picResolution = PIC_RESOLUTION_600_600;
  }

  // 6. Get picture data length
  picMsgP = strtok(nullptr, (const char *)" ");
  if (ENABLED(USER_LOGIC_DEBUG)) SERIAL_ECHOLNPGM("6.picMsgP = ", picMsgP);
  if (picMsgP != nullptr) picLen = atoi(picMsgP);

  // 7. Get the beginning of the picture
  picMsgP = strtok(nullptr, (const char *)" ");
  if (ENABLED(USER_LOGIC_DEBUG)) SERIAL_ECHOLNPGM("7.picMsgP = ", picMsgP);
  if (picMsgP != nullptr) picStartLine = atoi(picMsgP);

  // 8. Get the end of the picture
  picMsgP = strtok(nullptr, (const char *)" ");
  if (ENABLED(USER_LOGIC_DEBUG)) SERIAL_ECHOLNPGM("8.picMsgP = ", picMsgP);
  if (picMsgP != nullptr) picEndLine = atoi(picMsgP);

  // 9. Get the height of the model
  picMsgP = strtok(nullptr, (const char *)" ");
  if (ENABLED(USER_LOGIC_DEBUG)) SERIAL_ECHOLNPGM("9.picMsgP = ", picMsgP);
  if (picMsgP != nullptr) picHigh = atoi(picMsgP);

  #if ENABLED(USER_LOGIC_DEBUG)
    SERIAL_ECHOLNPGM("lPicFormat = ", lPicFormat);
    SERIAL_ECHOLNPGM("lPicHeader = ", lPicHeader);
    SERIAL_ECHOLNPGM("picResolution = ", picResolution);
    SERIAL_ECHOLNPGM("picLen = ", picLen);
    SERIAL_ECHOLNPGM("picStartLine = ", picStartLine);
    SERIAL_ECHOLNPGM("picEndLine = ", picEndLine);
    SERIAL_ECHOLNPGM("picHigh = ", picHigh);
  #endif

  #if ENABLED(USER_LOGIC_DEBUG)
    //SERIAL_ECHOPGM("\r\n gcode pic time test 1 msTest = ", (millis() - msTest));
  #endif

  msTest = millis();

  // Read the picture data from GCODE, determine whether you need to send it
  // to the screen according to the selected format or the picture of the scheduled size.

  // Determine whether it is a required resolution
  if (picResolution == targetPicResolution) {
    gcodePicDataRead(picLen, true, targetPicAddr);
  }
  else {
    // Move the pointer directly, skip the invalid picture
    // The agreement stipulates a complete line of data: ' +' ' + "data" +' \ n '1 + 1 + 76 + 1 = 79 byte
    // The last line is "; PNG END \ R" or "; JPG END \ R",
    uint32_t index1 = card.getFileCurPosition();// card.getIndex();
    uint32_t targetPicLen = (picLen / 3 + (picLen % 3 == 0)) * 4;
    uint32_t indexAdd = (targetPicLen / 76) * 3 + targetPicLen + 10;
    if ((targetPicLen % 76 ) != 0)
      indexAdd += 3;

    card.setIndex((index1 + indexAdd));
    #if ENABLED(USER_LOGIC_DEBUG)
      //SERIAL_ECHOLNPGM("\r\n ...old_index1 = ", index1,
      //                 "\r\n ...indexAdd = ", indexAdd);
    #endif

    return picResolution != targetPicResolution ? PIC_RESOLUTION_ERR : PIC_FORMAT_ERR;
  }

  //card.closefile();
  #if ENABLED(USER_LOGIC_DEBUG)
    //SERIAL_ECHOLNPGM("\r\n gcode pic time test 3 msTest = ", (millis() - msTest));
  #endif

  msTest = millis();
  return PIC_OK;
}

/**
 * @Function GCODE preview map Send to Diwen
 * @Author Creality
 * @Time   2021-12-01
 * fileName      Gcode file name
 * jpgAddr       Display address
 * jpgFormat     Picture type (JPG, PNG)
 * jpgResolution Image size
 *
 * #define RESOLUTION_48_48    "48*48"
 * #define RESOLUTION_200_200  "200*200"
 *
 * #define RESOLUTION_96_96    "96*96"
 * #define RESOLUTION_300_300  "300*300"
 *
 */
char gcodePicDataSendToDwin(char *fileName, unsigned int jpgAddr, unsigned char jpgFormat, unsigned char jpgResolution) {
  char ret;
  char returyCnt = 0;
  card.openFileRead(fileName);
  msTest = millis();
  while (1) {
    ret = gcodePicExistjudge(fileName, jpgAddr, jpgFormat, jpgResolution);
    if (ret == PIC_MISS_ERR) { // When there is no pic in GCODE, return directly
      card.closefile();
      SERIAL_ECHOLNPGM("preview.cpp file closed");
      return PIC_MISS_ERR;
    }
    else if (ret == PIC_FORMAT_ERR || (et == PIC_RESOLUTION_ERR) { // When the format or size is wrong, continue to judge
      if (++returyCnt >= 3) {
        card.closefile();
        SERIAL_ECHOLNPGM("preview.cpp file closed pic format error");
        return PIC_MISS_ERR;
      }
      else {
        continue;
      }
    }
    else {
      card.closefile();
      SERIAL_ECHOLNPGM("preview.cpp file pic ok");
      return PIC_OK;
    }
  }

}

/**
 * @Function GCode Preview Display and Hide
 * @Author Creality
 * @Time   2021-0-27
 * jpgAddr      address
 * onoff        Display (onoff == TRUE), hidden (onoff == false)
 * Display address
 */
void gcodePicDisplayOnOff(unsigned int jpgAddr, bool onoff) {
  rts.sendData(onoff ? 1 : 0, jpgAddr);
}

// Brightness control function letter
void DWIN_BrightnessCtrl(DwinBrightness_t brightness) {
  unsigned int buf[10];

  buf[0] = brightness.LeftUp_X;   // brightness
  buf[1] = brightness.LeftUp_Y;
  buf[2] = brightness.RightDown_X;   // brightness
  buf[3] = brightness.RightDown_Y;

  // Display area modification
  dwinWriteOneWord(brightness.spAddr, buf[0]);
  dwinWriteOneWord(brightness.spAddr + 1, buf[1]);
  dwinWriteOneWord(brightness.spAddr + 2, buf[2]);
  dwinWriteOneWord(brightness.spAddr + 3, buf[3]);

  // Dimming
  dwinWriteOneWord(brightness.addr, brightness.brightness);
}

/**
 * [refreshBrightnessAtPrint : Refresh the printing, the gradient display of the GCode preview picture]
 * @Author Creality
 * @Time   2021-06-19
 */
void refreshBrightnessAtPrint(uint16_t percent) {
  printBri.brightness  = BRIGHTNESS_PRINT;
  printBri.addr        = BRIGHTNESS_ADDR_PRINT;
  printBri.spAddr      = SP_ADDR_BRIGHTNESS_PRINT + 1;
  printBri.LeftUp_X    = BRIGHTNESS_PRINT_LEFT_HIGH_X;
  printBri.LeftUp_Y    = BRIGHTNESS_PRINT_LEFT_HIGH_Y;
  printBri.RightDown_X = BRIGHTNESS_PRINT_LEFT_HIGH_X + BRIGHTNESS_PRINT_WIDTH;
  printBri.RightDown_Y = BRIGHTNESS_PRINT_LEFT_HIGH_Y + (100 - percent) * BRIGHTNESS_PRINT_HIGH / 100;

  DWIN_BrightnessCtrl(printBri);
}

#endif // E3S1PRO_RTS
