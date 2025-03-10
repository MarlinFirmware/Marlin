/*!
    \file    audio_out_itf.h
    \brief   audio OUT (playback) interface header file

    \version 2020-08-01, V3.0.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#ifndef __AUDIO_OUT_ITF_H
#define __AUDIO_OUT_ITF_H

#include "usbd_conf.h"

/* audio commands enumeration */
typedef enum
{
    AUDIO_CMD_PLAY = 1U,
    AUDIO_CMD_PAUSE,
    AUDIO_CMD_STOP,
}audio_cmd_enum;

/* mute commands */
#define AUDIO_MUTE                      0x01U
#define AUDIO_UNMUTE                    0x00U

/* functions return value */
#define AUDIO_OK                        0x00U
#define AUDIO_FAIL                      0xFFU

/* audio machine states */
#define AUDIO_STATE_INACTIVE            0x00U
#define AUDIO_STATE_ACTIVE              0x01U
#define AUDIO_STATE_PLAYING             0x02U
#define AUDIO_STATE_PAUSED              0x03U
#define AUDIO_STATE_STOPPED             0x04U
#define AUDIO_STATE_ERROR               0x05U

typedef struct {
    uint8_t  (*audio_init)        (uint32_t audio_freq, uint32_t volume, uint32_t options);
    uint8_t  (*audio_deinit)      (uint32_t options);
    uint8_t  (*audio_cmd)         (uint8_t* pbuf, uint32_t size, uint8_t cmd);
    uint8_t  (*audio_volume_ctl)  (uint8_t vol);
    uint8_t  (*audio_mute_ctl)    (uint8_t cmd);
    uint8_t  (*audio_periodic_tc) (uint8_t cmd);
    uint8_t  (*audio_state_get)   (void);
} audio_fops_struct;

extern audio_fops_struct audio_out_fops;

#endif /* __AUDIO_OUT_ITF_H */
