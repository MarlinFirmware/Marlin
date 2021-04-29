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
#pragma once

#ifdef __cplusplus
  extern "C" { /* C-declarations for C++ */
#endif

#include "../../../../inc/MarlinConfigPre.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define UART_RX_BUFFER_SIZE   1024
#define UART_FIFO_BUFFER_SIZE 1024

#define WIFI_DECODE_TYPE      1

#define IP_DHCP_FLAG          1

#define WIFI_AP_NAME          "TP-LINK_MKS"
#define WIFI_KEY_CODE         "makerbase"

#define IP_ADDR               "192.168.3.100"
#define IP_MASK               "255.255.255.0"
#define IP_GATE               "192.168.3.1"
#define IP_DNS                "192.168.3.1"

#define AP_IP_DHCP_FLAG       1
#define AP_IP_ADDR            "192.168.3.100"
#define AP_IP_MASK            "255.255.255.0"
#define AP_IP_GATE            "192.168.3.1"
#define AP_IP_DNS             "192.168.3.1"
#define IP_START_IP           "192.168.3.1"
#define IP_END_IP             "192.168.3.255"

#define UDISKBUFLEN           1024

typedef enum{
  udisk_buf_empty = 0,
  udisk_buf_full,
} UDISK_DATA_BUFFER_STATE;

#define TRANS_RCV_FIFO_BLOCK_NUM  14

typedef struct {
  unsigned char *bufferAddr[TRANS_RCV_FIFO_BLOCK_NUM];
  unsigned char *p;
  UDISK_DATA_BUFFER_STATE state[TRANS_RCV_FIFO_BLOCK_NUM];
  unsigned char read_cur;
  unsigned char write_cur;
} WIFI_DMA_RCV_FIFO;

typedef struct {
  uint8_t flag; // 0x0: no error;  0x01: error
  uint32_t start_tick; //error start time
  uint32_t now_tick;
} WIFI_TRANS_ERROR;

extern volatile WIFI_TRANS_ERROR wifiTransError;

typedef struct {
  char ap_name[32];   //wifi-name
  char keyCode[64]; //wifi password
  int  decodeType;
  int  baud;
  int  mode;
} WIFI_PARA;

typedef struct {
  char state;
  char hostUrl[96];
  int  port;
  char id[21];
} CLOUD_PARA;

typedef struct {
  char  dhcp_flag;
  char  ip_addr[16];
  char  mask[16];
  char  gate[16];
  char  dns[16];

  char  dhcpd_flag;
  char  dhcpd_ip[16];
  char  dhcpd_mask[16];
  char  dhcpd_gate[16];
  char  dhcpd_dns[16];
  char  start_ip_addr[16];
  char  end_ip_addr[16];
} IP_PARA;

typedef enum {
  WIFI_NOT_CONFIG,
  WIFI_CONFIG_MODE,
  WIFI_CONFIG_DHCP,
  WIFI_CONFIG_AP,
  WIFI_CONFIG_IP_INF,
  WIFI_CONFIG_DNS,
  WIFI_CONFIG_TCP,
  WIFI_CONFIG_SERVER,
  WIFI_CONFIG_REMOTE_PORT,
  WIFI_CONFIG_BAUD,
  WIFI_CONFIG_COMMINT,
  WIFI_CONFIG_OK,
  WIFI_GET_IP_OK,
  WIFI_RECONN,
  WIFI_CONNECTED,
  WIFI_WAIT_TRANS_START,
  WIFI_TRANS_FILE,
  WIFI_CONFIG_DHCPD,
  WIFI_COFIG_DHCPD_IP,
  WIFI_COFIG_DHCPD_DNS,
  WIFI_EXCEPTION,
} WIFI_STATE;

typedef enum {
  TRANSFER_IDLE,
  TRANSFERING,
  TRANSFER_STORE,
} TRANSFER_STATE;
extern volatile TRANSFER_STATE esp_state;

typedef struct {
  char buf[20][80];
  int  rd_index;
  int  wt_index;
} QUEUE;

typedef enum {
  WIFI_PARA_SET,      // 0x0:net parameter
  WIFI_PRINT_INF,     // 0x1:print message
  WIFI_TRANS_INF,     // 0x2:Pass through information
  WIFI_EXCEP_INF,     // 0x3:Exception information
  WIFI_CLOUD_CFG,     // 0x4:cloud config
  WIFI_CLOUD_UNBIND,  // 0x5:Unbind ID
} WIFI_RET_TYPE;

typedef struct {
  uint32_t uart_read_point;
  uint32_t uart_write_point;
  //uint8_t uartTxBuffer[UART_FIFO_BUFFER_SIZE];
} SZ_USART_FIFO;

#define WIFI_GCODE_BUFFER_LEAST_SIZE    96
#define WIFI_GCODE_BUFFER_SIZE  (WIFI_GCODE_BUFFER_LEAST_SIZE * 3)
typedef struct {
  uint8_t wait_tick;
  uint8_t Buffer[WIFI_GCODE_BUFFER_SIZE];
  uint32_t r;
  uint32_t w;
} WIFI_GCODE_BUFFER;

extern volatile WIFI_STATE wifi_link_state;
extern WIFI_PARA wifiPara;
extern IP_PARA ipPara;
extern CLOUD_PARA cloud_para;

extern WIFI_GCODE_BUFFER espGcodeFifo;

uint32_t getWifiTick();
uint32_t getWifiTickDiff(int32_t lastTick, int32_t curTick);

void mks_esp_wifi_init();
extern int cfg_cloud_flag;
int send_to_wifi(uint8_t *buf, int len);
void wifi_looping();
int raw_send_to_wifi(uint8_t *buf, int len);
int package_to_wifi(WIFI_RET_TYPE type, uint8_t *buf, int len);
void get_wifi_list_command_send();
void get_wifi_commands();
int readWifiBuf(int8_t *buf, int32_t len);
void mks_wifi_firmware_update();

#ifdef __cplusplus
  } /* C-declarations for C++ */
#endif
