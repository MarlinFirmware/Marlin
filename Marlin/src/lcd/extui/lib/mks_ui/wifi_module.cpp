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

#if HAS_TFT_LVGL_UI

#include "draw_ui.h"
#include "wifi_module.h"
#include "wifi_upload.h"

#if ENABLED(MKS_WIFI_MODULE)

#include "../../../../MarlinCore.h"
#include "../../../../module/temperature.h"
#include "../../../../gcode/queue.h"
#include "../../../../gcode/gcode.h"
#include "../../../../lcd/marlinui.h"
#include "../../../../sd/cardreader.h"
#include "../../../../module/planner.h"
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

extern uint8_t Explore_Disk (char* path , uint8_t recu_level);

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

extern uint8_t bmp_public_buf[17 * 1024];

uint32_t getWifiTick() {
  return millis();
}

uint32_t getWifiTickDiff(int32_t lastTick, int32_t curTick) {
  if (lastTick <= curTick)
    return (curTick - lastTick) * TICK_CYCLE;
  else
    return (0xFFFFFFFF - lastTick + curTick) * TICK_CYCLE;
}

void wifi_delay(int n) {
  uint32_t begin = getWifiTick();
  uint32_t end = begin;
  while (getWifiTickDiff(begin, end) < (uint32_t)n)
    end = getWifiTick();
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

static void dma_init() {
  #if 0
  __HAL_RCC_DMA1_CLK_ENABLE();

  //HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_0);
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 4, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

  hdma_usart1_rx.Instance = DMA1_Channel5;
  //hdma_usart1_rx.Init.Channel = DMA_CHANNEL_4;
  hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_usart1_rx.Init.MemDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_usart1_rx.Init.Mode = DMA_NORMAL;
  hdma_usart1_rx.Init.Priority = DMA_PRIORITY_VERY_HIGH;
  if (HAL_DMA_Init((DMA_HandleTypeDef *)&hdma_usart1_rx) != HAL_OK) {
    Error_Handler();
  }


  HAL_DMA_Start_IT((DMA_HandleTypeDef *)&hdma_usart1_rx,
      (uint32_t)&huart1.Instance->DR,
      (uint32_t)(&WifiRxFifo.uartTxBuffer[0]),
      UART_RX_BUFFER_SIZE);

    //HAL_UART_Receive_DMA(&huart1,(uint8_t*)&WifiRxFifo.uartTxBuffer[0], UART_RX_BUFFER_SIZE);

      /* Enable the DMA transfer for the receiver request by setting the DMAR bit
       in the UART CR3 register */
      SET_BIT(huart1.Instance->CR3, USART_CR3_DMAR);

  #endif
  for (uint8_t i = 0; i < TRANS_RCV_FIFO_BLOCK_NUM; i++) {
    wifiDmaRcvFifo.bufferAddr[i] = &bmp_public_buf[1024 * i];
    wifiDmaRcvFifo.state[i] = udisk_buf_empty;
  }

  memset(wifiDmaRcvFifo.bufferAddr[0], 0, 1024 * TRANS_RCV_FIFO_BLOCK_NUM);
  wifiDmaRcvFifo.read_cur = 0;
  wifiDmaRcvFifo.write_cur = 0;
}

static void wifi_deInit() {
  #if 0
  HAL_DMA_Abort((DMA_HandleTypeDef *)&hdma_usart1_rx);
  HAL_DMA_DeInit((DMA_HandleTypeDef *)&hdma_usart1_rx);
  __HAL_DMA_DISABLE((DMA_HandleTypeDef *)&hdma_usart1_rx);
  #endif
}

extern uint8_t mksUsart1Rx;

void esp_port_begin(uint8_t interrupt) {
  WifiRxFifo.uart_read_point = 0;
  WifiRxFifo.uart_write_point = 0;
  #if 0
  NVIC_InitTypeDef NVIC_InitStructure;

  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStruct;

  WifiRxFifo.uart_read_point = 0;
  WifiRxFifo.uart_write_point = 0;
  memset((uint8_t*)WifiRxFifo.uartTxBuffer, 0, sizeof(WifiRxFifo.uartTxBuffer));

  if (interrupt) {
  #if TAN
    wifi_deInit ();

    //SZ_STM32_COMInit(COM1, 115200);
    __HAL_UART_ENABLE_IT(USART1, USART_IT_RXNE);

    USART_InitStructure.USART_BaudRate = 115200;        //���ڵĲ����ʣ�����115200 ��ߴ�4.5Mbits/s
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //�����ֳ���(8λ��9λ)
    USART_InitStructure.USART_StopBits = USART_StopBits_1;    //�����õ�ֹͣλ-֧��1��2��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;     //����żУ��
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ��������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //˫��ģʽ��ʹ�ܷ��ͺͽ���

    __HAL_RCC_USART1_CLK_ENABLE();

    GPIO_InitStruct.Pin = TFT_WIFI_TX_Pin|TFT_WIFI_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
      GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
      GPIO_InitStruct.Pin = TFT_WIFI_RX_Pin;
      HAL_GPIO_Init(GPIOA,&GPIO_InitStruct);

    USART_Init(USART1, &USART_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  //  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  //  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
    #else
    HAL_UART_DeInit(&huart1);
    MX_USART1_UART_Init(3);
    //__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
    HAL_UART_Receive_IT(&huart1,&mksUsart1Rx,1);
    #endif
  }
  else{
    #if 0
    NVIC_DisableIRQ(SZ_STM32_COM1_IRQn);

    USART_Cmd(SZ_STM32_COM1, DISABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, DISABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    SZ_STM32_COMInit(COM1, 1958400);

    USART_Cmd(SZ_STM32_COM1, ENABLE);

    wifi_delay(10);

    dma_init();
    #endif
    HAL_UART_DeInit(&huart1);
    MX_USART1_UART_Init(5);
        //dma1_5_IRQ_sel = 1;
    dma_init();
  }
  #endif
  if (interrupt) {
    #if ENABLED(MKS_WIFI_MODULE)
      WIFISERIAL.end();
      for (uint16_t i = 0; i < 65535; i++);
      WIFISERIAL.begin(WIFI_BAUDRATE);
      uint32_t serial_connect_timeout = millis() + 1000UL;
        while (/*!WIFISERIAL && */PENDING(millis(), serial_connect_timeout)) { /*nada*/ }
      //for (uint8_t i=0;i<100;i++)WIFISERIAL.write(0x33);
    #endif
  }
  else {
    #if ENABLED(MKS_WIFI_MODULE)
      WIFISERIAL.end();
      for (uint16_t i = 0; i < 65535; i++);
      WIFISERIAL.begin(WIFI_UPLOAD_BAUDRATE);
      uint32_t serial_connect_timeout = millis() + 1000UL;
        while (/*!WIFISERIAL && */PENDING(millis(), serial_connect_timeout)) { /*nada*/ }
      //for (uint16_t i=0;i<65535;i++);//WIFISERIAL.write(0x33);
    #endif
    dma_init();
  }
}

#if ENABLED(MKS_WIFI_MODULE)

  int raw_send_to_wifi(char *buf, int len) {
    if (buf == 0 || len <= 0) return 0;
    for (int i = 0; i < len; i++)
      WIFISERIAL.write(*(buf + i));
    return len;
  }

#endif

void wifi_ret_ack() {}

char buf_to_wifi[256];
int index_to_wifi = 0;
int package_to_wifi(WIFI_RET_TYPE type,char *buf, int len) {
  char wifi_ret_head = 0xA5;
  char wifi_ret_tail = 0xFC;

  if (type == WIFI_PARA_SET) {
    int data_offset = 4;
    int apLen = strlen((const char *)uiCfg.wifi_name);
    int keyLen = strlen((const char *)uiCfg.wifi_key);

    ZERO(buf_to_wifi);
    index_to_wifi = 0;

    buf_to_wifi[data_offset] = gCfgItems.wifi_mode_sel;
    buf_to_wifi[data_offset + 1]  = apLen;
    strncpy(&buf_to_wifi[data_offset + 2], (const char *)uiCfg.wifi_name, apLen);
    buf_to_wifi[data_offset + apLen + 2]  = keyLen;
    strncpy(&buf_to_wifi[data_offset + apLen + 3], (const char *)uiCfg.wifi_key, keyLen);
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
    strncpy(&buf_to_wifi[data_offset + 2], (const char *)uiCfg.cloud_hostUrl, urlLen);
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
}


int send_to_wifi(char *buf, int len) { return package_to_wifi(WIFI_TRANS_INF, buf, len); }

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
  char write_buf[513];
  int write_index;
  uint8_t saveFileName[30];
  uint32_t fileLen;
  uint32_t tick_begin;
  uint32_t tick_end;
} FILE_WRITER;

FILE_WRITER file_writer;

int32_t lastFragment = 0;

char lastBinaryCmd[50] = { 0 };

int total_write = 0;
char binary_head[2] = { 0, 0 };
unsigned char binary_data_len = 0;

int write_to_file(char *buf, int len) {
  for (int i = 0; i < len; i++) {
    file_writer.write_buf[file_writer.write_index++] = buf[i];
    if (file_writer.write_index >= 512) {
      int res = card.write(file_writer.write_buf, file_writer.write_index);
      if (res == -1) return -1;
      ZERO(file_writer.write_buf);
      file_writer.write_index = 0;
    }
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

uint8_t Explore_Disk(char* path , uint8_t recu_level) {
  char tmp[200];
  char Fstream[200];

  if (!path) return 0;

  const uint8_t fileCnt = card.get_num_Files();

  for (uint8_t i = 0; i < fileCnt; i++) {
    card.getfilename_sorted(SD_ORDER(i, fileCnt));

    //if (card.longFilename[0] == 0)
      strcpy(tmp, card.filename);
    //else
    //  strcpy(tmp, card.longFilename);

    ZERO(Fstream);
    strcpy(Fstream, tmp);

    if (card.flag.filenameIsDir && recu_level <= 10)
      strcat(Fstream, ".DIR");

    strcat(Fstream, "\r\n");
    send_to_wifi(Fstream, strlen(Fstream));
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
          if (uiCfg.print_state == IDLE) {
            int index = 0;

            if (tmpStr == 0) {
              gCfgItems.fileSysType = FILE_SYS_SD;
              send_to_wifi((char *)"Begin file list\r\n", strlen("Begin file list\r\n"));
              get_file_list((char *)"0:/");
              send_to_wifi((char *)"End file list\r\n", strlen("End file list\r\n"));
              send_to_wifi((char *)"ok\r\n", strlen("ok\r\n"));
              break;
            }

            while (tmpStr[index] == ' ') index++;

            if (gCfgItems.wifi_type == ESP_WIFI) {
              char *path = (char *)tempBuf;

              if (strlen((char *)&tmpStr[index]) < 80) {
                send_to_wifi((char *)"Begin file list\r\n", strlen("Begin file list\r\n"));

                if (strncmp((char *)&tmpStr[index], "1:", 2) == 0)
                  gCfgItems.fileSysType = FILE_SYS_SD;
                else if (strncmp((char *)&tmpStr[index], "0:", 2) == 0)
                  gCfgItems.fileSysType = FILE_SYS_USB;

                strcpy((char *)path, (char *)&tmpStr[index]);
                get_file_list(path);
                send_to_wifi((char *)"End file list\r\n", strlen("End file list\r\n"));
              }
              send_to_wifi((char *)"ok\r\n", strlen("ok\r\n"));
            }
          }
          break;

        case 21:
          /*init sd card*/
          send_to_wifi((char *)"ok\r\n", strlen("ok\r\n"));
          break;

        case 23:
          /*select the file*/
          if (uiCfg.print_state == IDLE) {
            int index = 0;
            while (tmpStr[index] == ' ') index++;

            if (strstr((char *)&tmpStr[index], ".g") || strstr((char *)&tmpStr[index], ".G")) {
              if (strlen((char *)&tmpStr[index]) < 80) {
                ZERO(list_file.file_name[sel_id]);

                if (gCfgItems.wifi_type == ESP_WIFI) {
                  if (strncmp((char *)&tmpStr[index], "1:", 2) == 0)
                    gCfgItems.fileSysType = FILE_SYS_SD;
                  else if (strncmp((char *)&tmpStr[index], "0:", 2) == 0)
                    gCfgItems.fileSysType = FILE_SYS_USB;
                  else if (tmpStr[index] != '/')
                    strcat((char *)list_file.file_name[0], "/");
                  strcat((char *)list_file.file_name[sel_id], (char *)&tmpStr[index]);

                }
                else
                  strcpy(list_file.file_name[sel_id], (char *)&tmpStr[index]);

                char *cur_name=strrchr(list_file.file_name[sel_id],'/');

                card.openFileRead(cur_name);

                if (card.isFileOpen())
                  send_to_wifi((char *)"File selected\r\n", strlen("File selected\r\n"));
                else {
                  send_to_wifi((char *)"file.open failed\r\n", strlen("file.open failed\r\n"));
                  strcpy(list_file.file_name[sel_id], "notValid");
                }
                send_to_wifi((char *)"ok\r\n", strlen("ok\r\n"));
              }
            }
          }
          break;

        case 24:
          if (strcmp(list_file.file_name[sel_id], "notValid") != 0) {
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
          send_to_wifi((char *)"ok\r\n", strlen("ok\r\n"));
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
            send_to_wifi((char *)"ok\r\n", strlen("ok\r\n"));
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

            send_to_wifi((char *)"ok\r\n", strlen("ok\r\n"));
          }
          break;

        case 27:
          /*report print rate*/
          if ((uiCfg.print_state == WORKING) || (uiCfg.print_state == PAUSED)|| (uiCfg.print_state == REPRINTING)) {
            print_rate = uiCfg.totalSend;
            ZERO(tempBuf);
            sprintf((char *)tempBuf, "M27 %d\r\n", print_rate);
            send_to_wifi((char *)tempBuf, strlen((char *)tempBuf));
          }
          break;

        case 28:
          /*begin to transfer file to filesys*/
          if (uiCfg.print_state == IDLE) {

            int index = 0;
            while (tmpStr[index] == ' ') index++;

            if (strstr((char *)&tmpStr[index], ".g") || strstr((char *)&tmpStr[index], ".G")) {
              strcpy((char *)file_writer.saveFileName, (char *)&tmpStr[index]);

              if (gCfgItems.fileSysType == FILE_SYS_SD) {
                ZERO(tempBuf);
                sprintf((char *)tempBuf, "%s", file_writer.saveFileName);
              }
              else if (gCfgItems.fileSysType == FILE_SYS_USB) {
                ZERO(tempBuf);
                sprintf((char *)tempBuf, "%s", (char *)file_writer.saveFileName);
              }
              mount_file_sys(gCfgItems.fileSysType);

              #if ENABLED(SDSUPPORT)
                char *cur_name = strrchr(list_file.file_name[sel_id], '/');
                card.openFileWrite(cur_name);
                if (card.isFileOpen()) {
                  ZERO(file_writer.saveFileName);
                  strcpy((char *)file_writer.saveFileName, (char *)&tmpStr[index]);
                  ZERO(tempBuf);
                  sprintf((char *)tempBuf, "Writing to file: %s\r\n", (char *)file_writer.saveFileName);
                  wifi_ret_ack();
                  send_to_wifi((char *)tempBuf, strlen((char *)tempBuf));
                  total_write = 0;
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
            send_to_wifi((char *)"ok\r\n", strlen("ok\r\n"));
            sprintf((char *)tempBuf,"T:%.1f /%.1f B:%.1f /%.1f T0:%.1f /%.1f T1:%.1f /%.1f @:0 B@:0\r\n",

            (float)thermalManager.temp_hotend[0].celsius, (float)thermalManager.temp_hotend[0].target,
            #if HAS_HEATED_BED
              (float)thermalManager.temp_bed.celsius, (float)thermalManager.temp_bed.target,
            #else
              0.0f, 0.0f,
            #endif
            (float)thermalManager.temp_hotend[0].celsius, (float)thermalManager.temp_hotend[0].target,
            #if !defined(SINGLENOZZLE) && HAS_MULTI_EXTRUDER
              (float)thermalManager.temp_hotend[1].celsius, (float)thermalManager.temp_hotend[1].target
            #else
              0.0f, 0.0f
            #endif
            );
          }
          else {
            sprintf((char *)tempBuf,"T:%d /%d B:%d /%d T0:%d /%d T1:%d /%d @:0 B@:0\r\n",

            (int)thermalManager.temp_hotend[0].celsius, (int)thermalManager.temp_hotend[0].target,
            #if HAS_HEATED_BED
              (int)thermalManager.temp_bed.celsius, (int)thermalManager.temp_bed.target,
            #else
              0, 0,
            #endif
            (int)thermalManager.temp_hotend[0].celsius, (int)thermalManager.temp_hotend[0].target,
              #if !defined(SINGLENOZZLE) && HAS_MULTI_EXTRUDER
                (int)thermalManager.temp_hotend[1].celsius, (int)thermalManager.temp_hotend[1].target
              #else
                0, 0
              #endif
            );
          }

          send_to_wifi((char *)tempBuf, strlen((char *)tempBuf));
          queue.enqueue_one_P(PSTR("M105"));
          break;

        case 992:
          if ((uiCfg.print_state == WORKING) || (uiCfg.print_state == PAUSED)) {
            ZERO(tempBuf);
            sprintf((char *)tempBuf, "M992 %d%d:%d%d:%d%d\r\n", print_time.hours/10, print_time.hours%10, print_time.minutes/10, print_time.minutes%10, print_time.seconds/10, print_time.seconds%10);
            wifi_ret_ack();
            send_to_wifi((char *)tempBuf, strlen((char *)tempBuf));
          }
          break;

        case 994:
          if ((uiCfg.print_state == WORKING) || (uiCfg.print_state == PAUSED)) {
            ZERO(tempBuf);
            if (strlen((char *)list_file.file_name[sel_id]) > (100 - 1)) return;
            sprintf((char *)tempBuf, "M994 %s;%d\n", list_file.file_name[sel_id],(int)gCfgItems.curFilesize);
            wifi_ret_ack();
            send_to_wifi((char *)tempBuf, strlen((char *)tempBuf));
          }
          break;

        case 997:
          if (uiCfg.print_state == IDLE) {
            wifi_ret_ack();
            send_to_wifi((char *)"M997 IDLE\r\n", strlen("M997 IDLE\r\n"));
          }
          else if (uiCfg.print_state == WORKING) {
            wifi_ret_ack();
            send_to_wifi((char *)"M997 PRINTING\r\n", strlen("M997 PRINTING\r\n"));
          }
          else if (uiCfg.print_state == PAUSED) {
            wifi_ret_ack();
            send_to_wifi((char *)"M997 PAUSE\r\n", strlen("M997 PAUSE\r\n"));
          }
          else if (uiCfg.print_state == REPRINTING) {
            wifi_ret_ack();
            send_to_wifi((char *)"M997 PAUSE\r\n", strlen("M997 PAUSE\r\n"));
          }
          if (uiCfg.command_send == 0) get_wifi_list_command_send();
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
          send_to_wifi((char *)"ok\r\n", strlen("ok\r\n"));
          send_to_wifi((char *)"FIRMWARE_NAME:Robin_nano\r\n", strlen("FIRMWARE_NAME:Robin_nano\r\n"));
          break;

        default:
          strcat((char *)cmd_line, "\n");

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
                send_to_wifi((char *)"ok\r\n", strlen("ok\r\n"));
              else
                need_ok_later = true;
            }
          }
          break;
      }
    }
    else {
      strcat((char *)cmd_line, "\n");

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
            send_to_wifi((char *)"ok\r\n", strlen("ok\r\n"));
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
  char buf[] = { 0xA5, 0x07, 0x00, 0x00, 0xFC };
  raw_send_to_wifi(buf, 5);
}

static void net_msg_handle(uint8_t * msg, uint16_t msgLen) {
  int wifiNameLen, wifiKeyLen, hostLen, id_len, ver_len;

  if (msgLen <= 0) return;

  // ip
  sprintf(ipPara.ip_addr, "%d.%d.%d.%d", msg[0], msg[1], msg[2], msg[3]);

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

  if (uiCfg.configWifi == 1) {
    if ((wifiPara.mode != gCfgItems.wifi_mode_sel)
      || (strncmp(wifiPara.ap_name, (const char *)uiCfg.wifi_name, 32) != 0)
      || (strncmp(wifiPara.keyCode, (const char *)uiCfg.wifi_key, 64) != 0)) {
      package_to_wifi(WIFI_PARA_SET, (char *)0, 0);
    }
    else uiCfg.configWifi = 0;
  }
  if (cfg_cloud_flag == 1) {
    if (((cloud_para.state >> 4) != (char)gCfgItems.cloud_enable)
      || (strncmp(cloud_para.hostUrl, (const char *)uiCfg.cloud_hostUrl, 96) != 0)
      || (cloud_para.port != uiCfg.cloud_port)) {
      package_to_wifi(WIFI_CLOUD_CFG, (char *)0, 0);
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
    uiCfg.command_send = 1;
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
          if (strcmp((const char *)str,(const char *)wifi_list.wifiName[j]) == 0) {
            wifi_name_is_same = 1;
            break;
          }
        }
        if (wifi_name_is_same != 1) {
          //for (j=0;j<wifiNameLen;j++) {
            if (str[0] > 0x80) {
              wifi_name_is_same = 1;
              //break;
            }
          //}
        }
        if (wifi_name_is_same == 1) {
          wifi_name_is_same = 0;
          wifiMsgIdex  +=  wifiNameLen;
          //wifi_list.RSSI[i] = msg[wifiMsgIdex];
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

void utf8_2_unicode(uint8_t *source,uint8_t Len) {
  uint8_t i = 0, char_i = 0, char_byte_num = 0;
  uint16_t u16_h, u16_m, u16_l, u16_value;
  uint8_t FileName_unicode[30];

  ZERO(FileName_unicode);

  while (1) {
    char_byte_num = source[i] & 0xF0;
    if (source[i] < 0X80) {
      //ASCII --1byte
      FileName_unicode[char_i] = source[i];
      i += 1;
      char_i += 1;
    }
    else if (char_byte_num == 0XC0 || char_byte_num == 0XD0) {
      //--2byte

      u16_h = (((uint16_t)source[i] << 8) & 0x1F00) >> 2;
      u16_l = ((uint16_t)source[i + 1] & 0x003F);
      u16_value = (u16_h | u16_l);
      FileName_unicode[char_i] = (uint8_t)((u16_value & 0xFF00) >> 8);
      FileName_unicode[char_i + 1] = (uint8_t)(u16_value & 0x00FF);
      i += 2;
      char_i += 2;
    }
    else if (char_byte_num == 0XE0) {
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
    else if (char_byte_num == 0XF0) {
      //--4byte
      i += 4;
      //char_i += 3;
    }
    else {
      break;
    }
    if (i >= Len || i >= 255) break;
  }
  COPY(source, FileName_unicode);
}

char saveFilePath[50];

static bool longName2DosName(const char* longName, uint8_t* dosName) {
  uint8_t i = 11;
  while (i--) dosName[i] = '\0';
  while (*longName) {
    uint8_t c = *longName++;
    if (c == '.') {                   // For a dot...
      if (i == 0) return false;
      else { strcat((char *)dosName,".GCO"); return dosName[0] != '\0'; }
    }
    else {
      // Fail for illegal characters
      PGM_P p = PSTR("|<>^+=?/[];,*\"\\");
      while (uint8_t b = pgm_read_byte(p++)) if (b == c) return false;
      if (c < 0x21 || c == 0x7F) return false;           // Check size, non-printable characters
      dosName[i++] = (c < 'a' || c > 'z') ? (c) : (c + ('A' - 'a')); // Uppercase required for 8.3 name
    }
    if (i >= 5) strcat((char *)dosName,"~1.GCO");
  }
  return dosName[0] != '\0';              // Return true if any name was set
}

static void file_first_msg_handle(uint8_t * msg, uint16_t msgLen) {
  uint8_t fileNameLen = *msg;

  if (msgLen != fileNameLen + 5) return;

  file_writer.fileLen = *((uint32_t *)(msg + 1));
  ZERO(file_writer.saveFileName);

  memcpy(file_writer.saveFileName, msg + 5, fileNameLen);

  utf8_2_unicode(file_writer.saveFileName,fileNameLen);

  ZERO(file_writer.write_buf);

  if (strlen((const char *)file_writer.saveFileName) > sizeof(saveFilePath))
    return;

  ZERO(saveFilePath);

  if (gCfgItems.fileSysType == FILE_SYS_SD) {
    //sprintf((char *)saveFilePath, "/%s", file_writer.saveFileName);
    card.mount();

    //ZERO(list_file.long_name[sel_id]);
    //memcpy(list_file.long_name[sel_id],file_writer.saveFileName,sizeof(list_file.long_name[sel_id]));
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
    sprintf((char *)saveFilePath, "/%s", dosName);

    ZERO(list_file.long_name[sel_id]);
    memcpy(list_file.long_name[sel_id], dosName, sizeof(dosName));

    char *cur_name=strrchr((const char *)saveFilePath,'/');

    SdFile file;
    SdFile *curDir;
    card.endFilePrint();
    const char * const fname = card.diveToFile(true, curDir, cur_name);
    if (!fname) return;
    if (file.open(curDir, fname, O_CREAT | O_APPEND | O_WRITE | O_TRUNC)) {
      gCfgItems.curFilesize = file.fileSize();
    }
    else {
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
}

#define FRAG_MASK _BV32(31)

static void file_fragment_msg_handle(uint8_t * msg, uint16_t msgLen) {
  uint32_t frag = *((uint32_t *)msg);

  if ((frag & FRAG_MASK) != (uint32_t)(lastFragment + 1)) {
    ZERO(file_writer.write_buf);
    file_writer.write_index = 0;
    wifi_link_state = WIFI_CONNECTED;
    upload_result = 2;
  }
  else {
    if (write_to_file((char *)msg + 4, msgLen - 4) < 0) {
      ZERO(file_writer.write_buf);
      file_writer.write_index = 0;
      wifi_link_state = WIFI_CONNECTED;
      upload_result = 2;
      return;
    }
    lastFragment = frag;

    if ((frag & (~FRAG_MASK))) {
      int res = card.write(file_writer.write_buf, file_writer.write_index);
      if (res == -1) {
        ZERO(file_writer.write_buf);
        file_writer.write_index = 0;
        wifi_link_state = WIFI_CONNECTED;
        upload_result = 2;
        return;
      }
      ZERO(file_writer.write_buf);
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
  int32_t head_pos;
  int32_t tail_pos;
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

int storeRcvData(int32_t len) {
  unsigned char tmpW = wifiDmaRcvFifo.write_cur;
  if (len <= UDISKBUFLEN && wifiDmaRcvFifo.state[tmpW] == udisk_buf_empty) {
    for (uint16_t i = 0; i < len; i++)
      wifiDmaRcvFifo.bufferAddr[tmpW][i] = WIFISERIAL.read();
    wifiDmaRcvFifo.state[tmpW] = udisk_buf_full;
    wifiDmaRcvFifo.write_cur = (tmpW + 1) % TRANS_RCV_FIFO_BLOCK_NUM;
    return 1;
  }
  return 0;
}

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
  else {
  }
  wifi_delay(200);
  WIFI_IO1_SET();
  //exchangeFlashMode(1);  //change spi flash to use dma mode
  esp_port_begin(1);
  if (wifiTransError.flag != 0x1) WIFI_IO1_RESET();
}

void wifi_rcv_handle() {
  int32_t len = 0;
  uint8_t ucStr[(UART_RX_BUFFER_SIZE) + 1] = { 0 };
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
      if (storeRcvData(UART_RX_BUFFER_SIZE)) {
        esp_state = TRANSFERING;
        //esp_dma_pre();
        if (wifiTransError.flag != 0x1) WIFI_IO1_RESET();
      }
      else
        WIFI_IO1_SET();
    }
  }
  else {
    //len = readUsartFifo((SZ_USART_FIFO *)&WifiRxFifo, (int8_t *)ucStr, UART_RX_BUFFER_SIZE);
    len = readWifiBuf((int8_t *)ucStr, UART_RX_BUFFER_SIZE);
    if (len > 0) {
      esp_data_parser((char *)ucStr, len);

      if (wifi_link_state == WIFI_TRANS_FILE) {
        //exchangeFlashMode(0);  //change spi flash not use dma mode
        wifi_delay(10);
        esp_port_begin(0);
        wifi_delay(10);
        tick_net_time1 = 0;
      }
      if (wifiTransError.flag != 0x1) WIFI_IO1_RESET();
      getDataF = 1;
    }
    if (need_ok_later &&  (queue.length < BUFSIZE)) {
      need_ok_later = false;
      send_to_wifi((char *)"ok\r\n", strlen("ok\r\n"));
    }
  }

  if (getDataF == 1) {
    tick_net_time1 = getWifiTick();
  }
  else {
    tick_net_time2 = getWifiTick();

    if (wifi_link_state == WIFI_TRANS_FILE) {
      if ((tick_net_time1 != 0) && (getWifiTickDiff(tick_net_time1, tick_net_time2) > 4500)) {
        wifi_link_state = WIFI_CONNECTED;
        upload_result = 2;
        clear_cur_ui();
        stopEspTransfer();
        lv_draw_dialog(DIALOG_TYPE_UPLOAD_FILE);
      }
    }

    if ((tick_net_time1 != 0) && (getWifiTickDiff(tick_net_time1, tick_net_time2) > 10000))
      wifi_link_state = WIFI_NOT_CONFIG;

    if ((tick_net_time1 != 0) && (getWifiTickDiff(tick_net_time1, tick_net_time2) > 120000)) {
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
  do { wifi_rcv_handle(); } while (wifi_link_state == WIFI_TRANS_FILE);
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
    res = f_open(&esp_upload.uploadFile, ESP_FIRMWARE_FILE,  FA_OPEN_EXISTING | FA_READ);

    if (res ==  FR_OK) {
      f_close(&esp_upload.uploadFile);

      wifi_delay(2000);

      if (usartFifoAvailable((SZ_USART_FIFO *)&WifiRxFifo) < 20) {
        return;
      }

      clear_cur_ui();

      draw_dialog(DIALOG_TYPE_UPDATE_ESP_FIRMARE);

      if (wifi_upload(0) >= 0) {

        f_unlink("1:/MKS_WIFI_CUR");
        f_rename(ESP_FIRMWARE_FILE,"/MKS_WIFI_CUR");
      }
      draw_return_ui();

      update_flag = 1;
    }
    if (update_flag == 0) {
      res = f_open(&esp_upload.uploadFile, ESP_WEB_FIRMWARE_FILE,  FA_OPEN_EXISTING | FA_READ);

      if (res ==  FR_OK) {
        f_close(&esp_upload.uploadFile);

        wifi_delay(2000);

        if (usartFifoAvailable((SZ_USART_FIFO *)&WifiRxFifo) < 20) {
          return;
        }

        clear_cur_ui();

        draw_dialog(DIALOG_TYPE_UPDATE_ESP_FIRMARE);
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

#define BUF_INC_POINTER(p)  ((p + 1 == UART_FIFO_BUFFER_SIZE) ? 0 : (p + 1))

int usartFifoAvailable(SZ_USART_FIFO *fifo) {
  int diff = fifo->uart_write_point - fifo->uart_read_point;
  if (diff < 0) diff += UART_FIFO_BUFFER_SIZE;
  return diff;
}

int readUsartFifo(SZ_USART_FIFO *fifo, int8_t *buf, int32_t len) {
  int i = 0 ;
  while (i < len && fifo->uart_read_point != fifo->uart_write_point) {
    buf[i++] = fifo->uartTxBuffer[fifo->uart_read_point];
    fifo->uart_read_point = BUF_INC_POINTER(fifo->uart_read_point);
  }
  return i;
}

int writeUsartFifo(SZ_USART_FIFO *fifo, int8_t *buf, int32_t len) {
  if (buf == 0 || len <= 0) return -1;

  int i = 0 ;
  while (i < len && fifo->uart_read_point != BUF_INC_POINTER(fifo->uart_write_point)) {
    fifo->uartTxBuffer[fifo->uart_write_point] = buf[i++];
    fifo->uart_write_point = BUF_INC_POINTER(fifo->uart_write_point);
  }
  return i;
}

void get_wifi_commands() {
  static char wifi_line_buffer[MAX_CMD_SIZE];
  static bool wifi_comment_mode = false;
  static int wifi_read_count = 0;

  if (espGcodeFifo.wait_tick > 5) {
    while ((queue.length < BUFSIZE) && (espGcodeFifo.r != espGcodeFifo.w)) {

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
  }// queue has space, serial has data
  else {
    espGcodeFifo.wait_tick++;
  }
}

int readWifiBuf(int8_t *buf, int32_t len) {
  int i = 0;
  while (i < len && WIFISERIAL.available())
    buf[i++] = WIFISERIAL.read();
  return i;
}

#endif // MKS_WIFI_MODULE
#endif // HAS_TFT_LVGL_UI
