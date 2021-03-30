/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#include "../../../../inc/MarlinConfigPre.h"

#if BOTH(HAS_TFT_LVGL_UI, MKS_WIFI_MODULE)

#include "draw_ui.h"
#include "wifi_module.h"
#include "wifi_upload.h"
#include "SPI_TFT.h"

#include "../../../../MarlinCore.h"
#include "../../../../module/temperature.h"
#include "../../../../gcode/queue.h"
#include "../../../../gcode/gcode.h"
#include "../../../../lcd/marlinui.h"
#include "../../../../sd/cardreader.h"
#include "../../../../module/planner.h"
#include "../../../../module/servo.h"
#include "../../../../module/probe.h"

#if DISABLED(EMERGENCY_PARSER)
  #include "../../../../module/motion.h"
#endif
#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../../feature/powerloss.h"
#endif
#if ENABLED(PARK_HEAD_ON_PAUSE)
  #include "../../../../feature/pause.h"
#endif

#define WIFI_SET()        WRITE(WIFI_RESET_PIN, HIGH);
#define WIFI_RESET()      WRITE(WIFI_RESET_PIN, LOW);
#define WIFI_IO1_SET()      WRITE(WIFI_IO1_PIN, HIGH);
#define WIFI_IO1_RESET()    WRITE(WIFI_IO1_PIN, LOW);

extern uint8_t Explore_Disk (char *path , uint8_t recu_level);

extern uint8_t commands_in_queue;
extern uint8_t sel_id;

int usartFifoAvailable(SZ_USART_FIFO *fifo);
int readUsartFifo(SZ_USART_FIFO *fifo, int8_t *buf, int32_t len);
int writeUsartFifo(SZ_USART_FIFO * fifo, int8_t * buf, int32_t len);
extern unsigned int  getTickDiff(unsigned int curTick, unsigned int  lastTick);

volatile SZ_USART_FIFO  WifiRxFifo;

#define WAIT_ESP_TRANS_TIMEOUT_TICK 10500

int cfg_cloud_flag = 0;

extern PRINT_TIME print_time;

char wifi_firm_ver[20] = { 0 };
WIFI_GCODE_BUFFER espGcodeFifo;
extern uint8_t pause_resum;

uint8_t wifi_connect_flg = 0;
extern volatile uint8_t get_temp_flag;

#define WIFI_MODE 2
#define WIFI_AP_MODE  3

int upload_result = 0;

uint32_t upload_time = 0;
uint32_t upload_size = 0;

volatile WIFI_STATE wifi_link_state;
WIFI_PARA wifiPara;
IP_PARA ipPara;
CLOUD_PARA cloud_para;

char wifi_check_time = 0;

extern uint8_t gCurDir[100];

extern uint32_t wifi_loop_cycle;

volatile TRANSFER_STATE esp_state;

uint8_t left_to_send = 0;
uint8_t left_to_save[96] = { 0 };

volatile WIFI_DMA_RCV_FIFO wifiDmaRcvFifo;

volatile WIFI_TRANS_ERROR wifiTransError;

static bool need_ok_later = false;

extern volatile WIFI_STATE wifi_link_state;
extern WIFI_PARA wifiPara;
extern IP_PARA ipPara;
extern CLOUD_PARA cloud_para;

extern bool once_flag, flash_preview_begin, default_preview_flg, gcode_preview_over;
extern bool flash_dma_mode;

uint32_t getWifiTick() { return millis(); }

uint32_t getWifiTickDiff(int32_t lastTick, int32_t curTick) {
  if (lastTick <= curTick)
    return (curTick - lastTick) * TICK_CYCLE;
  else
    return (0xFFFFFFFF - lastTick + curTick) * TICK_CYCLE;
}

void wifi_delay(int n) {
  uint32_t begin = getWifiTick();
  uint32_t end = begin;
  while (getWifiTickDiff(begin, end) < (uint32_t)n) {
    watchdog_refresh();
    end = getWifiTick();
  }
}

void wifi_reset() {
  uint32_t start, now;
  start = getWifiTick();
  now = start;
  WIFI_RESET();
  while (getWifiTickDiff(start, now) < 500)
    now = getWifiTick();

  WIFI_SET();
}

void mount_file_sys(uint8_t disk_type) {
  if (disk_type == FILE_SYS_SD) {
    TERN_(SDSUPPORT, card.mount());
  }
  else if (disk_type == FILE_SYS_USB) {
  }
}

#include <libmaple/timer.h>
#include <libmaple/util.h>
#include <libmaple/rcc.h>

#include <boards.h>
#include <wirish.h>

#include <libmaple/dma.h>
#include <libmaple/bitband.h>

#include <libmaple/libmaple.h>
#include <libmaple/gpio.h>
#include <libmaple/usart.h>
#include <libmaple/ring_buffer.h>

void changeFlashMode(const bool dmaMode) {
  if (flash_dma_mode != dmaMode) {
    flash_dma_mode = dmaMode;
    if (!flash_dma_mode) {
      dma_disable(DMA1, DMA_CH5);
      dma_clear_isr_bits(DMA1, DMA_CH4);
    }
  }
}

static bool longName2DosName(const char *longName, uint8_t *dosName) {
  uint8_t i;
  for (i = FILENAME_LENGTH; i--;) dosName[i] = '\0';
  while (*longName) {
    uint8_t c = *longName++;
    if (c == '.') { // For a dot...
      if (i == 0) return false;
      strcat_P((char *)dosName, PSTR(".GCO"));
      break;
    }
    else {
      if (c < 0x21 || c == 0x7F) return false;                  // Check size, non-printable characters
      // Fail for illegal characters
      PGM_P p = PSTR("|<>^+=?/[];,*\"\\");
      while (const uint8_t b = pgm_read_byte(p++)) if (b == c) return false;
      dosName[i++] = c + (WITHIN(c, 'a', 'z') ? 'A' - 'a' : 0); // Uppercase required for 8.3 name
    }
    if (i >= 5) {
      strcat_P((char *)dosName, PSTR("~1.GCO"));
      break;
    }
  }
  return dosName[0] != '\0'; // Return true if any name was set
}

static int storeRcvData(volatile uint8_t *bufToCpy, int32_t len) {
  unsigned char tmpW = wifiDmaRcvFifo.write_cur;

  if (len > UDISKBUFLEN) return 0;

  if (wifiDmaRcvFifo.state[tmpW] == udisk_buf_empty) {
    memcpy((unsigned char *) wifiDmaRcvFifo.bufferAddr[tmpW], (uint8_t *)bufToCpy, len);
    wifiDmaRcvFifo.state[tmpW] = udisk_buf_full;
    wifiDmaRcvFifo.write_cur = (tmpW + 1) % TRANS_RCV_FIFO_BLOCK_NUM;
    return 1;
  }
  return 0;
}

static void esp_dma_pre() {
  dma_channel_reg_map *channel_regs = dma_tube_regs(DMA1, DMA_CH5);

  CBI32(channel_regs->CCR, 0);
  channel_regs->CMAR = (uint32_t)WIFISERIAL.usart_device->rb->buf;
  channel_regs->CNDTR = 0x0000;
  channel_regs->CNDTR = UART_RX_BUFFER_SIZE;
  DMA1->regs->IFCR = 0xF0000;
  SBI32(channel_regs->CCR, 0);
}

static void dma_ch5_irq_handle() {
    uint8 status_bits = dma_get_isr_bits(DMA1, DMA_CH5);
    dma_clear_isr_bits(DMA1, DMA_CH5);
    if (status_bits & 0x8) {
      // DMA transmit Error
    }
    else if (status_bits & 0x2) {
      // DMA transmit complete
      if (esp_state == TRANSFER_IDLE)
        esp_state = TRANSFERING;

      if (storeRcvData(WIFISERIAL.usart_device->rb->buf, UART_RX_BUFFER_SIZE)) {
        esp_dma_pre();
        if (wifiTransError.flag != 0x1)
          WIFI_IO1_RESET();
      }
      else {
        WIFI_IO1_SET();
        esp_state = TRANSFER_STORE;
      }
    }
    else if (status_bits & 0x4) {
      // DMA transmit half
      WIFI_IO1_SET();
    }
}

static void wifi_usart_dma_init() {
  dma_init(DMA1);
  uint32_t flags = ( DMA_MINC_MODE | DMA_TRNS_CMPLT | DMA_HALF_TRNS | DMA_TRNS_ERR);
  dma_xfer_size dma_bit_size = DMA_SIZE_8BITS;
  dma_setup_transfer(DMA1, DMA_CH5, &USART1_BASE->DR, dma_bit_size,
             (volatile void*)WIFISERIAL.usart_device->rb->buf, dma_bit_size, flags);// Transmit buffer DMA
  dma_set_priority(DMA1, DMA_CH5, DMA_PRIORITY_LOW);
  dma_attach_interrupt(DMA1, DMA_CH5, &dma_ch5_irq_handle);

  dma_clear_isr_bits(DMA1, DMA_CH5);
  dma_set_num_transfers(DMA1, DMA_CH5, UART_RX_BUFFER_SIZE);

  bb_peri_set_bit(&USART1_BASE->CR3, USART_CR3_DMAR_BIT, 1);
  dma_enable(DMA1, DMA_CH5);   // enable transmit

  for (uint8_t i = 0; i < TRANS_RCV_FIFO_BLOCK_NUM; i++) {
    wifiDmaRcvFifo.bufferAddr[i] = &bmp_public_buf[1024 * i];
    wifiDmaRcvFifo.state[i] = udisk_buf_empty;
  }

  memset(wifiDmaRcvFifo.bufferAddr[0], 0, 1024 * TRANS_RCV_FIFO_BLOCK_NUM);
  wifiDmaRcvFifo.read_cur = 0;
  wifiDmaRcvFifo.write_cur = 0;
}

void esp_port_begin(uint8_t interrupt) {
  WifiRxFifo.uart_read_point = 0;
  WifiRxFifo.uart_write_point = 0;

  #if 1

    #if ENABLED(MKS_WIFI_MODULE)
      WIFISERIAL.end();
      if (interrupt) {
        for (uint16_t i = 0; i < 65535; i++) { /*nada*/ }
        WIFISERIAL.begin(WIFI_BAUDRATE);
        millis_t serial_connect_timeout = millis() + 1000UL;
        while (PENDING(millis(), serial_connect_timeout)) { /*nada*/ }
      }
      else {
        WIFISERIAL.usart_device->regs->CR1 &= ~USART_CR1_RXNEIE;
        WIFISERIAL.begin(WIFI_UPLOAD_BAUDRATE);
        wifi_usart_dma_init();
      }
    #endif

  #else

    #if MKS_WIFI_MODULE
      WIFISERIAL.end();
      for (uint16_t i = 0; i < 65535; i++) { /*nada*/ }
      WIFISERIAL.begin(interrupt ? WIFI_BAUDRATE : WIFI_UPLOAD_BAUDRATE);
      millis_t serial_connect_timeout = millis() + 1000UL;
      while (PENDING(millis(), serial_connect_timeout)) { /*nada*/ }
    #endif
    if (!interrupt) wifi_usart_dma_init();

  #endif
}

#if ENABLED(MKS_WIFI_MODULE)

  int raw_send_to_wifi(uint8_t *buf, int len) {
    if (buf == 0 || len <= 0) return 0;
    for (int i = 0; i < len; i++)
      WIFISERIAL.write(*(buf + i));
    return len;
  }

#endif

void wifi_ret_ack() {}

uint8_t buf_to_wifi[256];
int index_to_wifi = 0;
int package_to_wifi(WIFI_RET_TYPE type, uint8_t *buf, int len) {
  uint8_t wifi_ret_head = 0xA5;
  uint8_t wifi_ret_tail = 0xFC;

  if (type == WIFI_PARA_SET) {
    int data_offset = 4;
    int apLen = strlen((const char *)uiCfg.wifi_name);
    int keyLen = strlen((const char *)uiCfg.wifi_key);

    ZERO(buf_to_wifi);
    index_to_wifi = 0;

    buf_to_wifi[data_offset] = gCfgItems.wifi_mode_sel;
    buf_to_wifi[data_offset + 1]  = apLen;
    memcpy(&buf_to_wifi[data_offset + 2], (const char *)uiCfg.wifi_name, apLen);
    buf_to_wifi[data_offset + apLen + 2]  = keyLen;
    memcpy(&buf_to_wifi[data_offset + apLen + 3], (const char *)uiCfg.wifi_key, keyLen);
    buf_to_wifi[data_offset + apLen + keyLen + 3] = wifi_ret_tail;

    index_to_wifi = apLen + keyLen + 3;

    buf_to_wifi[0] = wifi_ret_head;
    buf_to_wifi[1] = type;
    buf_to_wifi[2] = index_to_wifi & 0xFF;
    buf_to_wifi[3] = (index_to_wifi >> 8) & 0xFF;

    raw_send_to_wifi(buf_to_wifi, 5 + index_to_wifi);

    ZERO(buf_to_wifi);
    index_to_wifi = 0;
  }
  else if (type == WIFI_TRANS_INF) {
    if (len > (int)(sizeof(buf_to_wifi) - index_to_wifi - 5)) {
      ZERO(buf_to_wifi);
      index_to_wifi = 0;
      return 0;
    }

   if (len > 0) {
      memcpy(&buf_to_wifi[4 + index_to_wifi], buf, len);
      index_to_wifi += len;

      if (index_to_wifi < 1)
        return 0;

       if (buf_to_wifi[index_to_wifi + 3] == '\n') {
        // mask "wait" "busy" "X:"
        if (((buf_to_wifi[4] == 'w') && (buf_to_wifi[5] == 'a') && (buf_to_wifi[6] == 'i')  && (buf_to_wifi[7] == 't') )
          || ((buf_to_wifi[4] == 'b') && (buf_to_wifi[5] == 'u') && (buf_to_wifi[6] == 's')  && (buf_to_wifi[7] == 'y') )
          || ((buf_to_wifi[4] == 'X') && (buf_to_wifi[5] == ':') )
        ) {
          ZERO(buf_to_wifi);
          index_to_wifi = 0;
          return 0;
        }

        buf_to_wifi[0] = wifi_ret_head;
        buf_to_wifi[1] = type;
        buf_to_wifi[2] = index_to_wifi & 0xFF;
        buf_to_wifi[3] = (index_to_wifi >> 8) & 0xFF;
        buf_to_wifi[4 + index_to_wifi] = wifi_ret_tail;

        raw_send_to_wifi(buf_to_wifi, 5 + index_to_wifi);

        ZERO(buf_to_wifi);
        index_to_wifi = 0;
      }
    }
  }
  else if (type == WIFI_EXCEP_INF) {
    ZERO(buf_to_wifi);

    buf_to_wifi[0] = wifi_ret_head;
    buf_to_wifi[1] = type;
    buf_to_wifi[2] = 1;
    buf_to_wifi[3] = 0;
    buf_to_wifi[4] = *buf;
    buf_to_wifi[5] = wifi_ret_tail;

    raw_send_to_wifi(buf_to_wifi, 6);

    ZERO(buf_to_wifi);
    index_to_wifi = 0;
  }
  else if (type == WIFI_CLOUD_CFG) {
    int data_offset = 4;
    int urlLen = strlen((const char *)uiCfg.cloud_hostUrl);

    ZERO(buf_to_wifi);
    index_to_wifi = 0;

    buf_to_wifi[data_offset] = gCfgItems.cloud_enable ? 0x0A : 0x05;
    buf_to_wifi[data_offset + 1]  = urlLen;
    memcpy(&buf_to_wifi[data_offset + 2], (const char *)uiCfg.cloud_hostUrl, urlLen);
    buf_to_wifi[data_offset + urlLen + 2] = uiCfg.cloud_port & 0xFF;
    buf_to_wifi[data_offset + urlLen + 3] = (uiCfg.cloud_port >> 8) & 0xFF;
    buf_to_wifi[data_offset + urlLen + 4] = wifi_ret_tail;

    index_to_wifi = urlLen + 4;

    buf_to_wifi[0] = wifi_ret_head;
    buf_to_wifi[1] = type;
    buf_to_wifi[2] = index_to_wifi & 0xFF;
    buf_to_wifi[3] = (index_to_wifi >> 8) & 0xFF;

    raw_send_to_wifi(buf_to_wifi, 5 + index_to_wifi);

    ZERO(buf_to_wifi);
    index_to_wifi = 0;
  }
  else if (type == WIFI_CLOUD_UNBIND) {
    ZERO(buf_to_wifi);

    buf_to_wifi[0] = wifi_ret_head;
    buf_to_wifi[1] = type;
    buf_to_wifi[2] = 0;
    buf_to_wifi[3] = 0;
    buf_to_wifi[4] = wifi_ret_tail;

    raw_send_to_wifi(buf_to_wifi, 5);

    ZERO(buf_to_wifi);
    index_to_wifi = 0;
  }
  return 1;
}

#define SEND_OK_TO_WIFI send_to_wifi((uint8_t *)"ok\r\n", strlen("ok\r\n"))
int send_to_wifi(uint8_t *buf, int len) { return package_to_wifi(WIFI_TRANS_INF, buf, len); }

void set_cur_file_sys(int fileType) { gCfgItems.fileSysType = fileType; }

void get_file_list(char *path) {
  if (!path) return;

  if (gCfgItems.fileSysType == FILE_SYS_SD) {
    TERN_(SDSUPPORT, card.mount());
  }
  else if (gCfgItems.fileSysType == FILE_SYS_USB) {
    // udisk
  }
  Explore_Disk(path, 0);
}

char wait_ip_back_flag = 0;

typedef struct {
  int write_index;
  uint8_t saveFileName[30];
  uint8_t fileTransfer;
  uint32_t fileLen;
  uint32_t tick_begin;
  uint32_t tick_end;
} FILE_WRITER;

FILE_WRITER file_writer;

int32_t lastFragment = 0;

char saveFilePath[50];

static SdFile upload_file, *upload_curDir;
static filepos_t pos;

int write_to_file(char *buf, int len) {
  int i;
  int res = 0;

  for (i = 0; i < len; i++) {
    public_buf[file_writer.write_index++] = buf[i];
    if (file_writer.write_index >= 512) {
      res = upload_file.write(public_buf, file_writer.write_index);

      if (res == -1) {
        upload_file.close();
        const char * const fname = card.diveToFile(true, upload_curDir, saveFilePath);

        if (upload_file.open(upload_curDir, fname, O_WRITE)) {
          upload_file.setpos(&pos);
          res = upload_file.write(public_buf, file_writer.write_index);
        }
      }

      if (res == -1) return -1;

      upload_file.getpos(&pos);
      file_writer.write_index = 0;
    }
  }

  if (res == -1) {
    ZERO(public_buf);
    file_writer.write_index = 0;
    return  -1;
  }

  return 0;
}

#define ESP_PROTOC_HEAD (uint8_t)0xA5
#define ESP_PROTOC_TAIL   (uint8_t)0xFC

#define ESP_TYPE_NET        (uint8_t)0x0
#define ESP_TYPE_GCODE        (uint8_t)0x1
#define ESP_TYPE_FILE_FIRST     (uint8_t)0x2
#define ESP_TYPE_FILE_FRAGMENT    (uint8_t)0x3

#define ESP_TYPE_WIFI_LIST    (uint8_t)0x4

uint8_t esp_msg_buf[UART_RX_BUFFER_SIZE] = { 0 };
uint16_t esp_msg_index = 0;

typedef struct {
  uint8_t head;
  uint8_t type;
  uint16_t dataLen;
  uint8_t *data;
  uint8_t tail;
} ESP_PROTOC_FRAME;

static int cut_msg_head(uint8_t *msg, uint16_t msgLen, uint16_t cutLen) {
  if (msgLen < cutLen) return 0;

  else if (msgLen == cutLen) {
    memset(msg, 0, msgLen);
    return 0;
  }

  for (int i = 0; i < (msgLen - cutLen); i++)
    msg[i] = msg[cutLen + i];

  memset(&msg[msgLen - cutLen], 0, cutLen);

  return msgLen - cutLen;
}

uint8_t Explore_Disk(char *path , uint8_t recu_level) {
  char tmp[200];
  char Fstream[200];

  if (!path) return 0;

  const uint8_t fileCnt = card.get_num_Files();

  for (uint8_t i = 0; i < fileCnt; i++) {
    card.getfilename_sorted(SD_ORDER(i, fileCnt));
    ZERO(tmp);
    strcpy(tmp, card.filename);

    ZERO(Fstream);
    strcpy(Fstream, tmp);

    if (card.flag.filenameIsDir && recu_level <= 10)
      strcat_P(Fstream, PSTR(".DIR"));

    strcat_P(Fstream, PSTR("\r\n"));
    send_to_wifi((uint8_t*)Fstream, strlen(Fstream));
  }

  return fileCnt;
}

static void wifi_gcode_exec(uint8_t *cmd_line) {
  int8_t tempBuf[100] = { 0 };
  uint8_t *tmpStr = 0;
  int  cmd_value;
  volatile int print_rate;
  if (strchr((char *)cmd_line, '\n') && (strchr((char *)cmd_line, 'G') || strchr((char *)cmd_line, 'M') || strchr((char *)cmd_line, 'T'))) {
    tmpStr = (uint8_t *)strchr((char *)cmd_line, '\n');
    if (tmpStr) *tmpStr = '\0';

    tmpStr = (uint8_t *)strchr((char *)cmd_line, '\r');
    if (tmpStr) *tmpStr = '\0';

    tmpStr = (uint8_t *)strchr((char *)cmd_line, '*');
    if (tmpStr) *tmpStr = '\0';

    tmpStr = (uint8_t *)strchr((char *)cmd_line, 'M');
    if (tmpStr) {
      cmd_value = atoi((char *)(tmpStr + 1));
      tmpStr = (uint8_t *)strchr((char *)tmpStr, ' ');

      switch (cmd_value) {

        case 20: // M20: Print SD / µdisk file
          file_writer.fileTransfer = 0;
          if (uiCfg.print_state == IDLE) {
            int index = 0;

            if (tmpStr == 0) {
              gCfgItems.fileSysType = FILE_SYS_SD;
              send_to_wifi((uint8_t *)"Begin file list\r\n", strlen("Begin file list\r\n"));
              get_file_list((char *)"0:/");
              send_to_wifi((uint8_t *)"End file list\r\n", strlen("End file list\r\n"));
              SEND_OK_TO_WIFI;
              break;
            }

            while (tmpStr[index] == ' ') index++;

            if (gCfgItems.wifi_type == ESP_WIFI) {
              char *path = (char *)tempBuf;

              if (strlen((char *)&tmpStr[index]) < 80) {
                send_to_wifi((uint8_t *)"Begin file list\r\n", strlen("Begin file list\r\n"));

                if (strncmp((char *)&tmpStr[index], "1:", 2) == 0)
                  gCfgItems.fileSysType = FILE_SYS_SD;
                else if (strncmp((char *)&tmpStr[index], "0:", 2) == 0)
                  gCfgItems.fileSysType = FILE_SYS_USB;

                strcpy((char *)path, (char *)&tmpStr[index]);
                get_file_list(path);
                send_to_wifi((uint8_t *)"End file list\r\n", strlen("End file list\r\n"));
              }
              SEND_OK_TO_WIFI;
            }
          }
          break;

        case 21:
          /*init sd card*/
          SEND_OK_TO_WIFI;
          break;

        case 23:
          /*select the file*/
          if (uiCfg.print_state == IDLE) {
            int index = 0;
            while (tmpStr[index] == ' ') index++;

            if (strstr_P((char *)&tmpStr[index], PSTR(".g")) || strstr_P((char *)&tmpStr[index], PSTR(".G"))) {
              if (strlen((char *)&tmpStr[index]) < 80) {
                ZERO(list_file.file_name[sel_id]);
                ZERO(list_file.long_name[sel_id]);
                uint8_t has_path_selected = 0;

                if (gCfgItems.wifi_type == ESP_WIFI) {
                  if (strncmp_P((char *)&tmpStr[index], PSTR("1:"), 2) == 0) {
                    gCfgItems.fileSysType = FILE_SYS_SD;
                    has_path_selected = 1;
                  }
                  else if (strncmp_P((char *)&tmpStr[index], PSTR("0:"), 2) == 0) {
                    gCfgItems.fileSysType = FILE_SYS_USB;
                    has_path_selected = 1;
                  }
                  else if (tmpStr[index] != '/')
                    strcat_P((char *)list_file.file_name[sel_id], PSTR("/"));

                  if (file_writer.fileTransfer == 1) {
                    uint8_t dosName[FILENAME_LENGTH];
                    uint8_t fileName[sizeof(list_file.file_name[sel_id])];
                    fileName[0] = '\0';
                    if (has_path_selected == 1) {
                      strcat((char *)fileName, (char *)&tmpStr[index + 3]);
                      strcat_P((char *)list_file.file_name[sel_id], PSTR("/"));
                    }
                    else strcat((char *)fileName, (char *)&tmpStr[index]);
                    if (!longName2DosName((const char *)fileName, dosName))
                      strcpy_P(list_file.file_name[sel_id], PSTR("notValid"));
                    strcat((char *)list_file.file_name[sel_id], (char *)dosName);
                    strcat((char *)list_file.long_name[sel_id], (char *)dosName);
                  }
                  else {
                    strcat((char *)list_file.file_name[sel_id], (char *)&tmpStr[index]);
                    strcat((char *)list_file.long_name[sel_id], (char *)&tmpStr[index]);
                  }

                }
                else
                  strcpy(list_file.file_name[sel_id], (char *)&tmpStr[index]);

                char *cur_name=strrchr(list_file.file_name[sel_id],'/');

                card.openFileRead(cur_name);

                if (card.isFileOpen())
                  send_to_wifi((uint8_t *)"File selected\r\n", strlen("File selected\r\n"));
                else {
                  send_to_wifi((uint8_t *)"file.open failed\r\n", strlen("file.open failed\r\n"));
                  strcpy_P(list_file.file_name[sel_id], PSTR("notValid"));
                }
                SEND_OK_TO_WIFI;
              }
            }
          }
          break;

        case 24:
          if (strcmp_P(list_file.file_name[sel_id], PSTR("notValid")) != 0) {
            if (uiCfg.print_state == IDLE) {
              clear_cur_ui();
              reset_print_time();
              start_print_time();
              preview_gcode_prehandle(list_file.file_name[sel_id]);
              uiCfg.print_state = WORKING;
              lv_draw_printing();

              #if ENABLED(SDSUPPORT)
                if (!gcode_preview_over) {
                  char *cur_name = strrchr(list_file.file_name[sel_id], '/');

                  card.endFilePrint();

                  SdFile file;
                  SdFile *curDir;
                  card.endFilePrint();
                  const char * const fname = card.diveToFile(true, curDir, cur_name);
                  if (!fname) return;
                  if (file.open(curDir, fname, O_READ)) {
                    gCfgItems.curFilesize = file.fileSize();
                    file.close();
                    update_spi_flash();
                  }
                  card.openFileRead(cur_name);
                  if (card.isFileOpen()) {
                    //saved_feedrate_percentage = feedrate_percentage;
                    feedrate_percentage = 100;
                    planner.flow_percentage[0] = 100;
                    planner.e_factor[0] = planner.flow_percentage[0] * 0.01f;
                    #if EXTRUDERS == 2
                      planner.flow_percentage[1] = 100;
                      planner.e_factor[1] = planner.flow_percentage[1] * 0.01f;
                    #endif
                    card.startFileprint();
                    TERN_(POWER_LOSS_RECOVERY, recovery.prepare());
                    once_flag = false;
                  }
                }
              #endif
            }
            else if (uiCfg.print_state == PAUSED) {
              uiCfg.print_state = RESUMING;
              clear_cur_ui();
              start_print_time();

              if (gCfgItems.from_flash_pic)
                flash_preview_begin = true;
              else
                default_preview_flg = true;
              lv_draw_printing();
            }
            else if (uiCfg.print_state == REPRINTING) {
              uiCfg.print_state = REPRINTED;
              clear_cur_ui();
              start_print_time();
              if (gCfgItems.from_flash_pic)
                flash_preview_begin = true;
              else
                default_preview_flg = true;
              lv_draw_printing();
            }
          }
          SEND_OK_TO_WIFI;
          break;

        case 25:
          /*pause print file*/
          if (uiCfg.print_state == WORKING) {
            stop_print_time();

            clear_cur_ui();

            #if ENABLED(SDSUPPORT)
              card.pauseSDPrint();
              uiCfg.print_state = PAUSING;
            #endif
            if (gCfgItems.from_flash_pic)
              flash_preview_begin = true;
            else
              default_preview_flg = true;
            lv_draw_printing();
            SEND_OK_TO_WIFI;
          }
          break;

        case 26:
          /*stop print file*/
          if ((uiCfg.print_state == WORKING) || (uiCfg.print_state == PAUSED) || (uiCfg.print_state == REPRINTING)) {
            stop_print_time();

            clear_cur_ui();
            #if ENABLED(SDSUPPORT)
              uiCfg.print_state = IDLE;
              card.flag.abort_sd_printing = true;
            #endif

            lv_draw_ready_print();

            SEND_OK_TO_WIFI;
          }
          break;

        case 27:
          /*report print rate*/
          if ((uiCfg.print_state == WORKING) || (uiCfg.print_state == PAUSED)|| (uiCfg.print_state == REPRINTING)) {
            print_rate = uiCfg.totalSend;
            ZERO(tempBuf);
            sprintf_P((char *)tempBuf, PSTR("M27 %d\r\n"), print_rate);
            send_to_wifi((uint8_t *)tempBuf, strlen((char *)tempBuf));
          }
          break;

        case 28:
          /*begin to transfer file to filesys*/
          if (uiCfg.print_state == IDLE) {

            int index = 0;
            while (tmpStr[index] == ' ') index++;

            if (strstr_P((char *)&tmpStr[index], PSTR(".g")) || strstr_P((char *)&tmpStr[index], PSTR(".G"))) {
              strcpy((char *)file_writer.saveFileName, (char *)&tmpStr[index]);

              if (gCfgItems.fileSysType == FILE_SYS_SD) {
                ZERO(tempBuf);
                sprintf_P((char *)tempBuf, PSTR("%s"), file_writer.saveFileName);
              }
              else if (gCfgItems.fileSysType == FILE_SYS_USB) {
                ZERO(tempBuf);
                sprintf_P((char *)tempBuf, PSTR("%s"), (char *)file_writer.saveFileName);
              }
              mount_file_sys(gCfgItems.fileSysType);

              #if ENABLED(SDSUPPORT)
                char *cur_name = strrchr(list_file.file_name[sel_id], '/');
                card.openFileWrite(cur_name);
                if (card.isFileOpen()) {
                  ZERO(file_writer.saveFileName);
                  strcpy((char *)file_writer.saveFileName, (char *)&tmpStr[index]);
                  ZERO(tempBuf);
                  sprintf_P((char *)tempBuf, PSTR("Writing to file: %s\r\n"), (char *)file_writer.saveFileName);
                  wifi_ret_ack();
                  send_to_wifi((uint8_t *)tempBuf, strlen((char *)tempBuf));
                  wifi_link_state = WIFI_WAIT_TRANS_START;
                }
                else {
                  wifi_link_state = WIFI_CONNECTED;
                  clear_cur_ui();
                  lv_draw_dialog(DIALOG_TRANSFER_NO_DEVICE);
                }
              #endif
            }
          }
          break;

        case 105:
        case 991:
          ZERO(tempBuf);
          if (cmd_value == 105) {

            SEND_OK_TO_WIFI;

            char *outBuf = (char *)tempBuf;
            char str_1[16], tbuf[34];

            dtostrf(thermalManager.degHotend(0), 1, 1, tbuf);
            strcat_P(tbuf, PSTR(" /"));
            strcat(tbuf, dtostrf(thermalManager.degTargetHotend(0), 1, 1, str_1));

            const int tlen = strlen(tbuf);

            sprintf_P(outBuf, PSTR("T:%s"), tbuf);
            outBuf += 2 + tlen;

            strcpy_P(outBuf, PSTR(" B:"));
            outBuf += 3;
            #if HAS_HEATED_BED
              strcpy(outBuf, dtostrf(thermalManager.degBed(), 1, 1, str_1));
              strcat_P(outBuf, PSTR(" /"));
              strcat(outBuf, dtostrf(thermalManager.degTargetBed(), 1, 1, str_1));
            #else
              strcpy_P(outBuf, PSTR("0 /0"));
            #endif
            outBuf += 4;

            strcat_P(outBuf, PSTR(" T0:"));
            strcat(outBuf, tbuf);
            outBuf += 4 + tlen;

            strcat_P(outBuf, PSTR(" T1:"));
            outBuf += 4;
            #if HAS_MULTI_HOTEND
              strcat(outBuf, dtostrf(thermalManager.degHotend(1), 1, 1, str_1));
              strcat_P(outBuf, PSTR(" /"));
              strcat(outBuf, dtostrf(thermalManager.degTargetHotend(1), 1, 1, str_1));
            #else
              strcat_P(outBuf, PSTR("0 /0"));
            #endif
            outBuf += 4;

            strcat_P(outBuf, PSTR(" @:0 B@:0\r\n"));
          }
          else {
            sprintf_P((char *)tempBuf, PSTR("T:%d /%d B:%d /%d T0:%d /%d T1:%d /%d @:0 B@:0\r\n"),
              thermalManager.degHotend(0), thermalManager.degTargetHotend(0),
              #if HAS_HEATED_BED
                thermalManager.degBed(), thermalManager.degTargetBed(),
              #else
                0, 0,
              #endif
              thermalManager.degHotend(0), thermalManager.degTargetHotend(0),
              #if HAS_MULTI_HOTEND
                thermalManager.degHotend(1), thermalManager.degTargetHotend(1)
              #else
                0, 0
              #endif
            );
          }

          send_to_wifi((uint8_t *)tempBuf, strlen((char *)tempBuf));
          queue.enqueue_one_P(PSTR("M105"));
          break;

        case 992:
          if ((uiCfg.print_state == WORKING) || (uiCfg.print_state == PAUSED)) {
            ZERO(tempBuf);
            sprintf_P((char *)tempBuf, PSTR("M992 %d%d:%d%d:%d%d\r\n"), print_time.hours/10, print_time.hours%10, print_time.minutes/10, print_time.minutes%10, print_time.seconds/10, print_time.seconds%10);
            wifi_ret_ack();
            send_to_wifi((uint8_t *)tempBuf, strlen((char *)tempBuf));
          }
          break;

        case 994:
          if ((uiCfg.print_state == WORKING) || (uiCfg.print_state == PAUSED)) {
            ZERO(tempBuf);
            if (strlen((char *)list_file.file_name[sel_id]) > (100 - 1)) return;
            sprintf_P((char *)tempBuf, PSTR("M994 %s;%d\n"), list_file.file_name[sel_id], (int)gCfgItems.curFilesize);
            wifi_ret_ack();
            send_to_wifi((uint8_t *)tempBuf, strlen((char *)tempBuf));
          }
          break;

        case 997:
          if (uiCfg.print_state == IDLE) {
            wifi_ret_ack();
            send_to_wifi((uint8_t *)"M997 IDLE\r\n", strlen("M997 IDLE\r\n"));
          }
          else if (uiCfg.print_state == WORKING) {
            wifi_ret_ack();
            send_to_wifi((uint8_t *)"M997 PRINTING\r\n", strlen("M997 PRINTING\r\n"));
          }
          else if (uiCfg.print_state == PAUSED) {
            wifi_ret_ack();
            send_to_wifi((uint8_t *)"M997 PAUSE\r\n", strlen("M997 PAUSE\r\n"));
          }
          else if (uiCfg.print_state == REPRINTING) {
            wifi_ret_ack();
            send_to_wifi((uint8_t *)"M997 PAUSE\r\n", strlen("M997 PAUSE\r\n"));
          }
          if (!uiCfg.command_send) get_wifi_list_command_send();
          break;

        case 998:
          if (uiCfg.print_state == IDLE) {
            int v = atoi((char *)tmpStr);
            if (v == 0)
              set_cur_file_sys(0);
            else if (v == 1)
              set_cur_file_sys(1);
            wifi_ret_ack();
          }
          break;

        case 115:
          ZERO(tempBuf);
          SEND_OK_TO_WIFI;
          send_to_wifi((uint8_t *)"FIRMWARE_NAME:Robin_nano\r\n", strlen("FIRMWARE_NAME:Robin_nano\r\n"));
          break;

        default:
          strcat_P((char *)cmd_line, PSTR("\n"));

          if (espGcodeFifo.wait_tick > 5) {
            uint32_t left;
            if (espGcodeFifo.r > espGcodeFifo.w)
              left = espGcodeFifo.r - espGcodeFifo.w - 1;
            else
              left = WIFI_GCODE_BUFFER_SIZE + espGcodeFifo.r - espGcodeFifo.w - 1;

            if (left >= strlen((const char *)cmd_line)) {
              uint32_t index = 0;
              while (index < strlen((const char *)cmd_line)) {
                espGcodeFifo.Buffer[espGcodeFifo.w] = cmd_line[index] ;
                espGcodeFifo.w = (espGcodeFifo.w + 1) % WIFI_GCODE_BUFFER_SIZE;
                index++;
              }
              if (left - WIFI_GCODE_BUFFER_LEAST_SIZE >= strlen((const char *)cmd_line))
                SEND_OK_TO_WIFI;
              else
                need_ok_later = true;
            }
          }
          break;
      }
    }
    else {
      strcat_P((char *)cmd_line, PSTR("\n"));

      if (espGcodeFifo.wait_tick > 5) {
        uint32_t left_g;
        if (espGcodeFifo.r > espGcodeFifo.w)
          left_g = espGcodeFifo.r - espGcodeFifo.w - 1;
        else
          left_g = WIFI_GCODE_BUFFER_SIZE + espGcodeFifo.r - espGcodeFifo.w - 1;

        if (left_g >= strlen((const char *)cmd_line)) {
          uint32_t index = 0;
          while (index < strlen((const char *)cmd_line)) {
            espGcodeFifo.Buffer[espGcodeFifo.w] = cmd_line[index] ;
            espGcodeFifo.w = (espGcodeFifo.w + 1) % WIFI_GCODE_BUFFER_SIZE;
            index++;
          }
          if (left_g - WIFI_GCODE_BUFFER_LEAST_SIZE >= strlen((const char *)cmd_line))
            SEND_OK_TO_WIFI;
          else
            need_ok_later = true;
        }
      }
    }
  }
}

static int32_t charAtArray(const uint8_t *_array, uint32_t _arrayLen, uint8_t _char) {
  for (uint32_t i = 0; i < _arrayLen; i++)
    if (*(_array + i) == _char) return i;
  return -1;
}

void get_wifi_list_command_send() {
  uint8_t cmd_wifi_list[] = { 0xA5, 0x07, 0x00, 0x00, 0xFC };
  raw_send_to_wifi(cmd_wifi_list, COUNT(cmd_wifi_list));
}

static void net_msg_handle(uint8_t * msg, uint16_t msgLen) {
  int wifiNameLen, wifiKeyLen, hostLen, id_len, ver_len;

  if (msgLen <= 0) return;

  // ip
  sprintf_P(ipPara.ip_addr, PSTR("%d.%d.%d.%d"), msg[0], msg[1], msg[2], msg[3]);

  // port
  // connect state
  switch (msg[6]) {
    case 0x0A: wifi_link_state = WIFI_CONNECTED; break;
    case 0x0E: wifi_link_state = WIFI_EXCEPTION; break;
    default:   wifi_link_state = WIFI_NOT_CONFIG; break;
  }

  // mode
  wifiPara.mode = msg[7];

  // wifi name
  wifiNameLen = msg[8];
  wifiKeyLen = msg[9 + wifiNameLen];
  if (wifiNameLen < 32) {
    ZERO(wifiPara.ap_name);
    memcpy(wifiPara.ap_name, &msg[9], wifiNameLen);

    memset(&wifi_list.wifiConnectedName, 0, sizeof(wifi_list.wifiConnectedName));
    memcpy(&wifi_list.wifiConnectedName, &msg[9], wifiNameLen);

    // wifi key
    if (wifiKeyLen < 64) {
      ZERO(wifiPara.keyCode);
      memcpy(wifiPara.keyCode, &msg[10 + wifiNameLen], wifiKeyLen);
    }
  }

  cloud_para.state =msg[10 + wifiNameLen + wifiKeyLen];
  hostLen = msg[11 + wifiNameLen + wifiKeyLen];
  if (cloud_para.state) {
    if (hostLen < 96) {
      ZERO(cloud_para.hostUrl);
      memcpy(cloud_para.hostUrl, &msg[12 + wifiNameLen + wifiKeyLen], hostLen);
    }
    cloud_para.port = msg[12 + wifiNameLen + wifiKeyLen + hostLen] + (msg[13 + wifiNameLen + wifiKeyLen + hostLen] << 8);
  }

  // id
  id_len = msg[14 + wifiNameLen + wifiKeyLen + hostLen];
  if (id_len == 20) {
    ZERO(cloud_para.id);
    memcpy(cloud_para.id, (const char *)&msg[15 + wifiNameLen + wifiKeyLen + hostLen], id_len);
  }
  ver_len = msg[15 + wifiNameLen + wifiKeyLen + hostLen + id_len];
  if (ver_len < 20) {
    ZERO(wifi_firm_ver);
    memcpy(wifi_firm_ver, (const char *)&msg[16 + wifiNameLen + wifiKeyLen + hostLen + id_len], ver_len);
  }

  if (uiCfg.configWifi) {
    if ((wifiPara.mode != gCfgItems.wifi_mode_sel)
      || (strncmp(wifiPara.ap_name, (const char *)uiCfg.wifi_name, 32) != 0)
      || (strncmp(wifiPara.keyCode, (const char *)uiCfg.wifi_key, 64) != 0)) {
      package_to_wifi(WIFI_PARA_SET, (uint8_t *)0, 0);
    }
    else uiCfg.configWifi = false;
  }
  if (cfg_cloud_flag == 1) {
    if (((cloud_para.state >> 4) != (char)gCfgItems.cloud_enable)
      || (strncmp(cloud_para.hostUrl, (const char *)uiCfg.cloud_hostUrl, 96) != 0)
      || (cloud_para.port != uiCfg.cloud_port)) {
      package_to_wifi(WIFI_CLOUD_CFG, (uint8_t *)0, 0);
    }
    else cfg_cloud_flag = 0;
  }
}

static void wifi_list_msg_handle(uint8_t * msg, uint16_t msgLen) {
  int wifiNameLen,wifiMsgIdex = 1;
  int8_t wifi_name_is_same = 0;
  int8_t i, j;
  int8_t wifi_name_num = 0;
  uint8_t *str = 0;
  int8_t valid_name_num;

  if (msgLen <= 0) return;
  if (disp_state == KEY_BOARD_UI) return;

  wifi_list.getNameNum = msg[0];

  if (wifi_list.getNameNum < 20) {
    uiCfg.command_send = true;
    ZERO(wifi_list.wifiName);
    wifi_name_num = wifi_list.getNameNum;
    valid_name_num = 0;
    str = wifi_list.wifiName[0];

    if (wifi_list.getNameNum > 0) wifi_list.currentWifipage = 1;

    for (i = 0; i < wifi_list.getNameNum; i++) {
      wifiNameLen = msg[wifiMsgIdex++];
      if (wifiNameLen < 32) {
        memset(str, 0, WIFI_NAME_BUFFER_SIZE);
        memcpy(str, &msg[wifiMsgIdex], wifiNameLen);
        for (j = 0; j < valid_name_num; j++) {
          if (strcmp((const char *)str, (const char *)wifi_list.wifiName[j]) == 0) {
            wifi_name_is_same = 1;
            break;
          }
        }

        if (wifi_name_is_same != 1 && str[0] > 0x80)
          wifi_name_is_same = 1;

        if (wifi_name_is_same == 1) {
          wifi_name_is_same = 0;
          wifiMsgIdex  +=  wifiNameLen;
          wifiMsgIdex  +=  1;
          wifi_name_num--;
          //i--;
          continue;
        }
        if (i < WIFI_TOTAL_NUMBER - 1)
          str = wifi_list.wifiName[++valid_name_num];
      }
      wifiMsgIdex += wifiNameLen;
      wifi_list.RSSI[i] = msg[wifiMsgIdex++];
    }
    wifi_list.getNameNum = wifi_name_num;
    wifi_list.getPage = wifi_list.getNameNum / NUMBER_OF_PAGE + ((wifi_list.getNameNum % NUMBER_OF_PAGE) != 0);
    wifi_list.nameIndex = 0;

    if (disp_state == WIFI_LIST_UI) disp_wifi_list();
  }
}

static void gcode_msg_handle(uint8_t * msg, uint16_t msgLen) {
  uint8_t gcodeBuf[100] = { 0 };
  char *index_s, *index_e;

  if (msgLen <= 0) return;

  index_s = (char *)msg;
  index_e = (char *)strchr((char *)msg, '\n');
  if (*msg == 'N') {
    index_s = (char *)strchr((char *)msg, ' ');
    while (*index_s == ' ') index_s++;
  }
  while ((index_e != 0) && ((int)index_s < (int)index_e)) {
    if ((int)(index_e - index_s) < (int)sizeof(gcodeBuf)) {
      ZERO(gcodeBuf);
      memcpy(gcodeBuf, index_s, index_e - index_s + 1);
      wifi_gcode_exec(gcodeBuf);
    }
    while ((*index_e == '\r') || (*index_e == '\n')) index_e++;
    index_s = index_e;
    index_e = (char *)strchr(index_s, '\n');
  }
}

void utf8_2_unicode(uint8_t *source, uint8_t Len) {
  uint8_t i = 0, char_i = 0, char_byte_num = 0;
  uint16_t u16_h, u16_m, u16_l, u16_value;
  uint8_t FileName_unicode[30];

  ZERO(FileName_unicode);

  while (1) {
    char_byte_num = source[i] & 0xF0;
    if (source[i] < 0x80) {
      //ASCII --1byte
      FileName_unicode[char_i] = source[i];
      i += 1;
      char_i += 1;
    }
    else if (char_byte_num == 0xC0 || char_byte_num == 0xD0) {
      //--2byte
      u16_h = (((uint16_t)source[i] << 8) & 0x1F00) >> 2;
      u16_l = ((uint16_t)source[i + 1] & 0x003F);
      u16_value = (u16_h | u16_l);
      FileName_unicode[char_i] = (uint8_t)((u16_value & 0xFF00) >> 8);
      FileName_unicode[char_i + 1] = (uint8_t)(u16_value & 0x00FF);
      i += 2;
      char_i += 2;
    }
    else if (char_byte_num == 0xE0) {
      //--3byte
      u16_h = (((uint16_t)source[i] << 8) & 0x0F00) << 4;
      u16_m = (((uint16_t)source[i + 1] << 8) & 0x3F00) >> 2;
      u16_l = ((uint16_t)source[i + 2] & 0x003F);
      u16_value = (u16_h | u16_m | u16_l);
      FileName_unicode[char_i] = (uint8_t)((u16_value & 0xFF00) >> 8);
      FileName_unicode[char_i + 1] = (uint8_t)(u16_value & 0x00FF);
      i += 3;
      char_i += 2;
    }
    else if (char_byte_num == 0xF0) {
      //--4byte
      i += 4;
      //char_i += 3;
    }
    else
      break;

    if (i >= Len || i >= 255) break;
  }
  COPY(source, FileName_unicode);
}

static void file_first_msg_handle(uint8_t * msg, uint16_t msgLen) {
  uint8_t fileNameLen = *msg;

  if (msgLen != fileNameLen + 5) return;

  file_writer.fileLen = *((uint32_t *)(msg + 1));
  ZERO(file_writer.saveFileName);

  memcpy(file_writer.saveFileName, msg + 5, fileNameLen);

  utf8_2_unicode(file_writer.saveFileName,fileNameLen);

  ZERO(public_buf);

  if (strlen((const char *)file_writer.saveFileName) > sizeof(saveFilePath))
    return;

  ZERO(saveFilePath);

  if (gCfgItems.fileSysType == FILE_SYS_SD) {
    TERN_(SDSUPPORT, card.mount());
  }
  else if (gCfgItems.fileSysType == FILE_SYS_USB) {

  }
  file_writer.write_index = 0;
  lastFragment = -1;

  wifiTransError.flag = 0;
  wifiTransError.start_tick = 0;
  wifiTransError.now_tick = 0;

  TERN_(SDSUPPORT, card.closefile());

  wifi_delay(1000);

  #if ENABLED(SDSUPPORT)

    uint8_t dosName[FILENAME_LENGTH];

    if (!longName2DosName((const char *)file_writer.saveFileName,dosName)) {
      clear_cur_ui();
      upload_result = 2;
      wifiTransError.flag = 1;
      wifiTransError.start_tick = getWifiTick();
      lv_draw_dialog(DIALOG_TYPE_UPLOAD_FILE);
      return;
    }
    sprintf_P((char *)saveFilePath, PSTR("%s"), dosName);

    card.cdroot();
    upload_file.close();
    const char * const fname = card.diveToFile(true, upload_curDir, saveFilePath);

    if (!upload_file.open(upload_curDir, fname, O_CREAT | O_APPEND | O_WRITE | O_TRUNC)) {
      clear_cur_ui();
      upload_result = 2;

      wifiTransError.flag = 1;
      wifiTransError.start_tick = getWifiTick();

      lv_draw_dialog(DIALOG_TYPE_UPLOAD_FILE);
      return;
    }

  #endif // SDSUPPORT

  wifi_link_state = WIFI_TRANS_FILE;

  upload_result = 1;

  clear_cur_ui();
  lv_draw_dialog(DIALOG_TYPE_UPLOAD_FILE);

  lv_task_handler();

  file_writer.tick_begin = getWifiTick();

  file_writer.fileTransfer = 1;
}

#define FRAG_MASK ~_BV32(31)

static void file_fragment_msg_handle(uint8_t * msg, uint16_t msgLen) {
  uint32_t frag = *((uint32_t *)msg);
  if ((frag & FRAG_MASK) != (uint32_t)(lastFragment + 1)) {
    ZERO(public_buf);
    file_writer.write_index = 0;
    wifi_link_state = WIFI_CONNECTED;
    upload_result = 2;
  }
  else {
    if (write_to_file((char *)msg + 4, msgLen - 4) < 0) {
      ZERO(public_buf);
      file_writer.write_index = 0;
      wifi_link_state = WIFI_CONNECTED;
      upload_result = 2;
      return;
    }
    lastFragment = frag;

    if ((frag & (~FRAG_MASK)) != 0) {
      int res = upload_file.write(public_buf, file_writer.write_index);
      if (res == -1) {
        upload_file.close();
        const char * const fname = card.diveToFile(true, upload_curDir, saveFilePath);
        if (upload_file.open(upload_curDir, fname, O_WRITE)) {
          upload_file.setpos(&pos);
          res = upload_file.write(public_buf, file_writer.write_index);
        }
      }
      upload_file.close();
      SdFile file, *curDir;
      const char * const fname = card.diveToFile(true, curDir, saveFilePath);
      if (file.open(curDir, fname, O_RDWR)) {
        gCfgItems.curFilesize = file.fileSize();
        file.close();
      }
      else {
        ZERO(public_buf);
        file_writer.write_index = 0;
        wifi_link_state = WIFI_CONNECTED;
        upload_result = 2;
        return;
      }
      ZERO(public_buf);
      file_writer.write_index = 0;
      file_writer.tick_end = getWifiTick();
      upload_time = getWifiTickDiff(file_writer.tick_begin, file_writer.tick_end) / 1000;
      upload_size = gCfgItems.curFilesize;
      wifi_link_state = WIFI_CONNECTED;
      upload_result = 3;
    }
  }
}

void esp_data_parser(char *cmdRxBuf, int len) {
  int32_t head_pos, tail_pos;
  uint16_t cpyLen;
  int16_t leftLen = len;
  bool loop_again = false;

  ESP_PROTOC_FRAME esp_frame;

  while (leftLen > 0 || loop_again) {
    loop_again = false;

    if (esp_msg_index != 0) {
      head_pos = 0;
      cpyLen = (leftLen < (int16_t)((sizeof(esp_msg_buf) - esp_msg_index)) ? leftLen : sizeof(esp_msg_buf) - esp_msg_index);

      memcpy(&esp_msg_buf[esp_msg_index], cmdRxBuf + len - leftLen, cpyLen);

      esp_msg_index += cpyLen;

      leftLen = leftLen - cpyLen;
      tail_pos = charAtArray(esp_msg_buf, esp_msg_index, ESP_PROTOC_TAIL);

      if (tail_pos == -1) {
        if (esp_msg_index >= sizeof(esp_msg_buf)) {
          ZERO(esp_msg_buf);
          esp_msg_index = 0;
        }
        return;
      }
    }
    else {
      head_pos = charAtArray((uint8_t const *)&cmdRxBuf[len - leftLen], leftLen, ESP_PROTOC_HEAD);
      if (head_pos == -1) return;

      ZERO(esp_msg_buf);
      memcpy(esp_msg_buf, &cmdRxBuf[len - leftLen + head_pos], leftLen - head_pos);

      esp_msg_index = leftLen - head_pos;

      leftLen = 0;
      head_pos = 0;
      tail_pos = charAtArray(esp_msg_buf, esp_msg_index, ESP_PROTOC_TAIL);
      if (tail_pos == -1) {
        if (esp_msg_index >= sizeof(esp_msg_buf)) {
          ZERO(esp_msg_buf);
          esp_msg_index = 0;
        }
        return;
      }
    }

    esp_frame.type = esp_msg_buf[1];
    if (  esp_frame.type != ESP_TYPE_NET
       && esp_frame.type != ESP_TYPE_GCODE
       && esp_frame.type != ESP_TYPE_FILE_FIRST
       && esp_frame.type != ESP_TYPE_FILE_FRAGMENT
       && esp_frame.type != ESP_TYPE_WIFI_LIST
    ) {
      ZERO(esp_msg_buf);
      esp_msg_index = 0;
      return;
    }

    esp_frame.dataLen = esp_msg_buf[2] + (esp_msg_buf[3] << 8);

    if ((int)(4 + esp_frame.dataLen) > (int)(sizeof(esp_msg_buf))) {
      ZERO(esp_msg_buf);
      esp_msg_index = 0;
      return;
    }

    if (esp_msg_buf[4 + esp_frame.dataLen] != ESP_PROTOC_TAIL) {
      if (esp_msg_index >= sizeof(esp_msg_buf)) {
        ZERO(esp_msg_buf);
        esp_msg_index = 0;
      }
      return;
    }

    esp_frame.data = &esp_msg_buf[4];
    switch (esp_frame.type) {
      case ESP_TYPE_NET:
        net_msg_handle(esp_frame.data, esp_frame.dataLen);
        break;
      case ESP_TYPE_GCODE:
        gcode_msg_handle(esp_frame.data, esp_frame.dataLen);
        break;
      case ESP_TYPE_FILE_FIRST:
        file_first_msg_handle(esp_frame.data, esp_frame.dataLen);
        break;
      case ESP_TYPE_FILE_FRAGMENT:
        file_fragment_msg_handle(esp_frame.data, esp_frame.dataLen);
        break;
      case ESP_TYPE_WIFI_LIST:
        wifi_list_msg_handle(esp_frame.data, esp_frame.dataLen);
        break;
      default: break;
    }

    esp_msg_index = cut_msg_head(esp_msg_buf, esp_msg_index, esp_frame.dataLen  + 5);
    if (esp_msg_index > 0) {
      if (charAtArray(esp_msg_buf, esp_msg_index,  ESP_PROTOC_HEAD) == -1) {
        ZERO(esp_msg_buf);
        esp_msg_index = 0;
        return;
      }
      if ((charAtArray(esp_msg_buf, esp_msg_index,  ESP_PROTOC_HEAD) != -1) && (charAtArray(esp_msg_buf, esp_msg_index, ESP_PROTOC_TAIL) != -1))
        loop_again = true;
    }
  }
}

int32_t tick_net_time1, tick_net_time2;
int32_t readWifiFifo(uint8_t *retBuf, uint32_t bufLen) {
  unsigned char tmpR = wifiDmaRcvFifo.read_cur;
  if (bufLen >= UDISKBUFLEN && wifiDmaRcvFifo.state[tmpR] == udisk_buf_full) {
    memcpy(retBuf, (unsigned char *)wifiDmaRcvFifo.bufferAddr[tmpR], UDISKBUFLEN);
    wifiDmaRcvFifo.state[tmpR] = udisk_buf_empty;
    wifiDmaRcvFifo.read_cur = (tmpR + 1) % TRANS_RCV_FIFO_BLOCK_NUM;
    return UDISKBUFLEN;
  }
  return 0;
}

void stopEspTransfer() {
  if (wifi_link_state == WIFI_TRANS_FILE)
  wifi_link_state = WIFI_CONNECTED;

  TERN_(SDSUPPORT, card.closefile());

  if (upload_result != 3) {
    wifiTransError.flag = 1;
    wifiTransError.start_tick = getWifiTick();
    card.removeFile((const char *)saveFilePath);
  }

  wifi_delay(200);
  WIFI_IO1_SET();

  // disable dma
  dma_clear_isr_bits(DMA1, DMA_CH5);
  bb_peri_set_bit(&USART1_BASE->CR3, USART_CR3_DMAR_BIT, 0);
  dma_disable(DMA1, DMA_CH5);

  wifi_delay(200);
  changeFlashMode(true); // Set SPI flash to use DMA mode
  esp_port_begin(1);
  wifi_delay(200);

  W25QXX.init(SPI_QUARTER_SPEED);

  TERN_(HAS_TFT_LVGL_UI_SPI, SPI_TFT.spi_init(SPI_FULL_SPEED));
  TERN_(HAS_SERVOS, servo_init());
  TERN_(HAS_Z_SERVO_PROBE, probe.servo_probe_init());

  if (wifiTransError.flag != 0x1) WIFI_IO1_RESET();
}

void wifi_rcv_handle() {
  int32_t len = 0;
  uint8_t ucStr[(UART_RX_BUFFER_SIZE) + 1] = {0};
  int8_t getDataF = 0;
  if (wifi_link_state == WIFI_TRANS_FILE) {
    #if 0
      if (WIFISERIAL.available() == UART_RX_BUFFER_SIZE) {
        for (uint16_t i=0;i<UART_RX_BUFFER_SIZE;i++) {
          ucStr[i] = WIFISERIAL.read();
          len++;
        }
      }
    #else
      len = readWifiFifo(ucStr, UART_RX_BUFFER_SIZE);
    #endif
    if (len > 0) {
      esp_data_parser((char *)ucStr, len);
      if (wifi_link_state == WIFI_CONNECTED) {
        clear_cur_ui();
        lv_draw_dialog(DIALOG_TYPE_UPLOAD_FILE);
        stopEspTransfer();
      }
      getDataF = 1;
    }
    if (esp_state == TRANSFER_STORE) {
      if (storeRcvData(WIFISERIAL.usart_device->rb->buf, UART_RX_BUFFER_SIZE)) {
        esp_state = TRANSFERING;
        esp_dma_pre();
        if (wifiTransError.flag != 0x1) WIFI_IO1_RESET();
      }
      else
        WIFI_IO1_SET();
    }
  }
  else {
    len = readWifiBuf((int8_t *)ucStr, UART_RX_BUFFER_SIZE);
    if (len > 0) {
      esp_data_parser((char *)ucStr, len);
      if (wifi_link_state == WIFI_TRANS_FILE) {
        changeFlashMode(false); // Set SPI flash to use non-DMA mode
        wifi_delay(10);
        esp_port_begin(0);
        wifi_delay(10);
        tick_net_time1 = 0;
      }
      if (wifiTransError.flag != 0x1) WIFI_IO1_RESET();
      getDataF = 1;
    }
    if (need_ok_later && !queue.ring_buffer.full()) {
      need_ok_later = false;
      send_to_wifi((uint8_t *)"ok\r\n", strlen("ok\r\n"));
    }
  }

  if (getDataF == 1) {
    tick_net_time1 = getWifiTick();
  }
  else {
    tick_net_time2 = getWifiTick();

    if (wifi_link_state == WIFI_TRANS_FILE) {
      if (tick_net_time1 && getWifiTickDiff(tick_net_time1, tick_net_time2) > 8000) {
        wifi_link_state = WIFI_CONNECTED;
        upload_result = 2;
        clear_cur_ui();
        stopEspTransfer();
        lv_draw_dialog(DIALOG_TYPE_UPLOAD_FILE);
      }
    }
    if (tick_net_time1 && getWifiTickDiff(tick_net_time1, tick_net_time2) > 10000)
      wifi_link_state = WIFI_NOT_CONFIG;

    if (tick_net_time1 && getWifiTickDiff(tick_net_time1, tick_net_time2) > 120000) {
      wifi_link_state = WIFI_NOT_CONFIG;
      wifi_reset();
      tick_net_time1 = getWifiTick();
    }
  }

  if (wifiTransError.flag == 0x1) {
    wifiTransError.now_tick = getWifiTick();
    if (getWifiTickDiff(wifiTransError.start_tick, wifiTransError.now_tick) > WAIT_ESP_TRANS_TIMEOUT_TICK) {
      wifiTransError.flag = 0;
      WIFI_IO1_RESET();
    }
  }
}

void wifi_looping() {
  do {
    wifi_rcv_handle();
    watchdog_refresh();
  } while (wifi_link_state == WIFI_TRANS_FILE);
}

void mks_esp_wifi_init() {
  wifi_link_state = WIFI_NOT_CONFIG;

  SET_OUTPUT(WIFI_RESET_PIN);
  WIFI_SET();
  SET_OUTPUT(WIFI_IO1_PIN);
  SET_INPUT_PULLUP(WIFI_IO0_PIN);
  WIFI_IO1_SET();

  esp_state = TRANSFER_IDLE;
  esp_port_begin(1);

  wifi_reset();

  #if 0
  if (update_flag == 0) {
    res = f_open(&esp_upload.uploadFile, ESP_WEB_FIRMWARE_FILE,  FA_OPEN_EXISTING | FA_READ);

      if (res ==  FR_OK) {
        f_close(&esp_upload.uploadFile);

        wifi_delay(2000);

        if (usartFifoAvailable((SZ_USART_FIFO *)&WifiRxFifo) < 20) {
          return;
        }

        clear_cur_ui();

        draw_dialog(DIALOG_TYPE_UPDATE_ESP_FIRMWARE);
        if (wifi_upload(1) >= 0) {

          f_unlink("1:/MKS_WIFI_CUR");
          f_rename(ESP_WEB_FIRMWARE_FILE,"/MKS_WIFI_CUR");
        }
        draw_return_ui();
        update_flag = 1;
      }

    }
    if (update_flag == 0) {
      res = f_open(&esp_upload.uploadFile, ESP_WEB_FILE,  FA_OPEN_EXISTING | FA_READ);
      if (res ==  FR_OK) {
        f_close(&esp_upload.uploadFile);

        wifi_delay(2000);

        if (usartFifoAvailable((SZ_USART_FIFO *)&WifiRxFifo) < 20) {
          return;
        }

        clear_cur_ui();

        draw_dialog(DIALOG_TYPE_UPDATE_ESP_DATA);

        if (wifi_upload(2) >= 0) {

          f_unlink("1:/MKS_WEB_CONTROL_CUR");
          f_rename(ESP_WEB_FILE,"/MKS_WEB_CONTROL_CUR");
        }
        draw_return_ui();
      }
    }
  #endif
  wifiPara.decodeType = WIFI_DECODE_TYPE;
  wifiPara.baud = 115200;
  wifi_link_state = WIFI_NOT_CONFIG;
}

void mks_wifi_firmware_update() {
  card.openFileRead((char *)ESP_FIRMWARE_FILE);

  if (card.isFileOpen()) {
    card.closefile();

    wifi_delay(2000);

    if (usartFifoAvailable((SZ_USART_FIFO *)&WifiRxFifo) < 20)
      return;

    clear_cur_ui();

    lv_draw_dialog(DIALOG_TYPE_UPDATE_ESP_FIRMWARE);

    lv_task_handler();
    watchdog_refresh();

    if (wifi_upload(0) >= 0) {
      card.removeFile((char *)ESP_FIRMWARE_FILE_RENAME);
      SdFile file, *curDir;
      const char * const fname = card.diveToFile(true, curDir, ESP_FIRMWARE_FILE);
      if (file.open(curDir, fname, O_READ)) {
        file.rename(curDir, (char *)ESP_FIRMWARE_FILE_RENAME);
        file.close();
      }
    }
    clear_cur_ui();
  }
}

#define BUF_INC_POINTER(p)  ((p + 1 == UART_FIFO_BUFFER_SIZE) ? 0:(p + 1))

int usartFifoAvailable(SZ_USART_FIFO *fifo) {
  return WIFISERIAL.available();
}

void get_wifi_commands() {
  static char wifi_line_buffer[MAX_CMD_SIZE];
  static bool wifi_comment_mode = false;
  static int wifi_read_count = 0;

  if (espGcodeFifo.wait_tick > 5) {
    while (!queue.ring_buffer.full() && (espGcodeFifo.r != espGcodeFifo.w)) {

      espGcodeFifo.wait_tick = 0;

      char wifi_char = espGcodeFifo.Buffer[espGcodeFifo.r];

      espGcodeFifo.r = (espGcodeFifo.r + 1) % WIFI_GCODE_BUFFER_SIZE;

      /**
       * If the character ends the line
       */
      if (wifi_char == '\n' || wifi_char == '\r') {

        wifi_comment_mode = false; // end of line == end of comment

        if (!wifi_read_count) continue; // skip empty lines

        wifi_line_buffer[wifi_read_count] = 0; // terminate string
        wifi_read_count = 0; //reset buffer

        char* command = wifi_line_buffer;
        while (*command == ' ') command++; // skip any leading spaces

          // Movement commands alert when stopped
          if (IsStopped()) {
          char* gpos = strchr(command, 'G');
          if (gpos) {
            switch (strtol(gpos + 1, nullptr, 10)) {
              case 0 ... 1:
              #if ENABLED(ARC_SUPPORT)
                case 2 ... 3:
              #endif
              #if ENABLED(BEZIER_CURVE_SUPPORT)
                case 5:
              #endif
                SERIAL_ECHOLNPGM(STR_ERR_STOPPED);
                LCD_MESSAGEPGM(MSG_STOPPED);
                break;
            }
          }
        }

        #if DISABLED(EMERGENCY_PARSER)
          // Process critical commands early
          if (strcmp(command, "M108") == 0) {
            wait_for_heatup = false;
            TERN_(HAS_LCD_MENU, wait_for_user = false);
          }
          if (strcmp(command, "M112") == 0) kill(M112_KILL_STR, nullptr, true);
          if (strcmp(command, "M410") == 0) quickstop_stepper();
        #endif

        // Add the command to the queue
        queue.enqueue_one_P(wifi_line_buffer);
      }
      else if (wifi_read_count >= MAX_CMD_SIZE - 1) {

      }
      else { // it's not a newline, carriage return or escape char
        if (wifi_char == ';') wifi_comment_mode = true;
        if (!wifi_comment_mode) wifi_line_buffer[wifi_read_count++] = wifi_char;
      }
    }
  } // queue has space, serial has data
  else
    espGcodeFifo.wait_tick++;
}

int readWifiBuf(int8_t *buf, int32_t len) {
  int i = 0;
  while (i < len && WIFISERIAL.available())
    buf[i++] = WIFISERIAL.read();
  return i;
}

#endif // HAS_TFT_LVGL_UI && MKS_WIFI_MODULE
