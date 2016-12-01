/*

  u8g_com_linux_ssd_i2c.c

  com interface for linux i2c-dev and the SSDxxxx chip (SOLOMON) variant
  I2C protocol


  Universal 8bit Graphics Library

  Copyright (c) 2012, olikraus@gmail.com
  Copyright (c) 2015, daniel@redfelineninja.org.uk
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list
    of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright notice, this
    list of conditions and the following disclaimer in the documentation and/or other
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include "u8g.h"

#if defined(U8G_LINUX)

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <linux/i2c-dev.h>


#define I2C_SLA		0x3c
#define I2C_CMD_MODE	0x80
#define I2C_DATA_MODE	0x40
#define MAX_PACKET      64

#ifndef U8G_WITH_PINLIST
#error U8G_WITH_PINLIST is mandatory for this driver
#endif

static void set_cmd_mode(u8g_t *u8g, bool cmd_mode)
{
  u8g->pin_list[U8G_PI_A0_STATE] = cmd_mode;
}

static bool get_cmd_mode(u8g_t *u8g)
{
  return u8g->pin_list[U8G_PI_A0_STATE];
}

static uint8_t send_data_burst(u8g_t *u8g, int fd, uint8_t *buf, size_t buflen)
{
  uint8_t i2cbuf[2*MAX_PACKET];
  uint8_t i2clen;
  int res;

  /* ignore bursts when there is no file open */
  if (fd < 0)
	  return 0;

  if (get_cmd_mode(u8g)) {
    i2clen = 0;
    while (buflen > 0) {
      i2cbuf[i2clen++] = I2C_CMD_MODE;
      i2cbuf[i2clen++] = *buf++;
      buflen--;
    }
  } else {
    i2cbuf[0] = I2C_DATA_MODE;
    memcpy(i2cbuf+1, buf, buflen);
    i2clen = buflen + 1;
  }

  res = write(fd, i2cbuf, i2clen);
  if (res < 0)
    fprintf(stderr, "I2C write failed (%s)\n", strerror(errno));
  else if (res != i2clen)
    fprintf(stderr, "Incomplete I2C write (%d of %d packet)\n", res, i2clen);

  return res == i2clen;
}

uint8_t u8g_com_linux_ssd_i2c_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  static int fd = -1;
  char dev[24];

  switch(msg)
  {
    case U8G_COM_MSG_INIT:
      sprintf(dev, "/dev/i2c-%d", u8g->pin_list[U8G_PI_I2C_OPTION]);
      fd = open(dev, O_RDWR);
      if (fd < 0) {
	fprintf(stderr, "cannot open %s (%s)\n", dev, strerror(errno));
	return 0;
      }

      if (ioctl(fd, I2C_SLAVE, I2C_SLA) < 0) {
	fprintf(stderr, "cannot set slave address (%s)\n", strerror(errno));
	return 0;
      }

      break;

    case U8G_COM_MSG_STOP:
      /* ignored - i2c-dev will automatically stop between writes */
      break;

    case U8G_COM_MSG_RESET:
      /* ignored - no obvious means to reset an SSD via I2C */
      break;

    case U8G_COM_MSG_CHIP_SELECT:
      set_cmd_mode(u8g, true);
      break;

    case U8G_COM_MSG_WRITE_BYTE:
      send_data_burst(u8g, fd, &arg_val, 1);
      break;

    case U8G_COM_MSG_WRITE_SEQ:
    case U8G_COM_MSG_WRITE_SEQ_P:	/* no progmem in Linux */
      while (arg_val > MAX_PACKET) {
	send_data_burst(u8g, fd, arg_ptr, MAX_PACKET);
	arg_ptr += MAX_PACKET;
	arg_val -= MAX_PACKET;
      }
      send_data_burst(u8g, fd, arg_ptr, arg_val);
      break;

    case U8G_COM_MSG_ADDRESS:
      /* choose cmd (arg_val = 0) or data mode (arg_val = 1) */
      set_cmd_mode(u8g, !arg_val);
      break;
  }

  return 1;
}

#endif /* U8G_LINUX */
