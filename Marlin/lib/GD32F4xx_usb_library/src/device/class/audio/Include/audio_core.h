/*!
    \file    audio_core.h
    \brief   the header file of USB audio device class core functions

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

#ifndef __AUDIO_CORE_H
#define __AUDIO_CORE_H

#include "usbd_enum.h"

#define FORMAT_24BIT(x)                              (uint8_t)(x);(uint8_t)(x >> 8U);(uint8_t)(x >> 16U)

/* number of sub-packets in the audio transfer buffer. you can modify this value but always make sure
   that it is an even number and higher than 3 */
#define OUT_PACKET_NUM                               4U

/* total size of the audio transfer buffer */
#define OUT_BUF_MARGIN                               4U
#define TOTAL_OUT_BUF_SIZE                           ((uint32_t)((SPEAKER_OUT_PACKET + OUT_BUF_MARGIN) * OUT_PACKET_NUM))

#define AUDIO_CONFIG_DESC_SET_LEN                    (sizeof(usb_desc_config_set))
#define AUDIO_INTERFACE_DESC_SIZE                    9U

#define USB_AUDIO_DESC_SIZ                           0x09U
#define AUDIO_STANDARD_EP_DESC_SIZE                  0x09U
#define AUDIO_STREAMING_EP_DESC_SIZE                 0x07U

/* audio interface class code */
#define USB_CLASS_AUDIO                              0x01U

/* audio interface subclass codes */
#define AUDIO_SUBCLASS_CONTROL                       0x01U
#define AUDIO_SUBCLASS_AUDIOSTREAMING                0x02U
#define AUDIO_SUBCLASS_MIDISTREAMING                 0x03U

/* audio interface protocol codes */
#define AUDIO_PROTOCOL_UNDEFINED                     0x00U
#define AUDIO_STREAMING_GENERAL                      0x01U
#define AUDIO_STREAMING_FORMAT_TYPE                  0x02U

/* audio class-specific descriptor types */
#define AUDIO_DESCTYPE_UNDEFINED                     0x20U
#define AUDIO_DESCTYPE_DEVICE                        0x21U
#define AUDIO_DESCTYPE_CONFIGURATION                 0x22U
#define AUDIO_DESCTYPE_STRING                        0x23U
#define AUDIO_DESCTYPE_INTERFACE                     0x24U
#define AUDIO_DESCTYPE_ENDPOINT                      0x25U

/* audio control interface descriptor subtypes */
#define AUDIO_CONTROL_HEADER                         0x01U
#define AUDIO_CONTROL_INPUT_TERMINAL                 0x02U
#define AUDIO_CONTROL_OUTPUT_TERMINAL                0x03U
#define AUDIO_CONTROL_MIXER_UNIT                     0x04U
#define AUDIO_CONTROL_SELECTOR_UNIT                  0x05U
#define AUDIO_CONTROL_FEATURE_UNIT                   0x06U
#define AUDIO_CONTROL_PROCESSING_UNIT                0x07U
#define AUDIO_CONTROL_EXTENSION_UNIT                 0x08U

#define AUDIO_INPUT_TERMINAL_DESC_SIZE               0x0CU
#define AUDIO_OUTPUT_TERMINAL_DESC_SIZE              0x09U
#define AUDIO_STREAMING_INTERFACE_DESC_SIZE          0x07U

#define AUDIO_CONTROL_MUTE                           0x01U
#define AUDIO_CONTROL_VOLUME                         0x02U

#define AUDIO_FORMAT_TYPE_I                          0x01U
#define AUDIO_FORMAT_TYPE_III                        0x03U

#define USB_ENDPOINT_TYPE_ISOCHRONOUS                0x01U
#define AUDIO_ENDPOINT_GENERAL                       0x01U

#define AUDIO_REQ_UNDEFINED                          0x00U
#define AUDIO_REQ_SET_CUR                            0x01U
#define AUDIO_REQ_GET_CUR                            0x81U
#define AUDIO_REQ_SET_MIN                            0x02U
#define AUDIO_REQ_GET_MIN                            0x82U
#define AUDIO_REQ_SET_MAX                            0x03U
#define AUDIO_REQ_GET_MAX                            0x83U
#define AUDIO_REQ_SET_RES                            0x04U
#define AUDIO_REQ_GET_RES                            0x84U
#define AUDIO_REQ_SET_MEM                            0x05U
#define AUDIO_REQ_GET_MEM                            0x85U
#define AUDIO_REQ_GET_STAT                           0xFFU

#define AUDIO_OUT_STREAMING_CTRL                     0x05U
#define AUDIO_IN_STREAMING_CTRL                      0x02U

/* audio stream interface number */
enum
{
#ifdef USE_USB_AUDIO_MICPHONE
    MIC_INTERFACE_COUNT,
#endif
#ifdef USE_USB_AUDIO_SPEAKER
    SPEAK_INTERFACE_COUNT,
#endif
    CONFIG_DESC_AS_ITF_COUNT,
};

#define AC_ITF_TOTAL_LEN                             (sizeof(usb_desc_AC_itf) + CONFIG_DESC_AS_ITF_COUNT*(sizeof(usb_desc_input_terminal) + \
                                                      sizeof(usb_desc_mono_feature_unit) + sizeof(usb_desc_output_terminal)))

#pragma pack(1)

typedef struct
{
    usb_desc_header header;           /*!< descriptor header, including type and size */
    uint8_t  bDescriptorSubtype;      /*!< header descriptor subtype */
    uint16_t bcdADC;                  /*!< audio device class specification release number in binary-coded decimal */
    uint16_t wTotalLength;            /*!< total number of bytes */
    uint8_t  bInCollection;           /*!< the number of the streaming interfaces */
#ifdef USE_USB_AUDIO_MICPHONE
    uint8_t  baInterfaceNr0;          /*!< interface number of the streaming interfaces */
#endif

#ifdef USE_USB_AUDIO_SPEAKER
    uint8_t  baInterfaceNr1;          /*!< interface number of the streaming interfaces */
#endif
} usb_desc_AC_itf;

typedef struct
{
    usb_desc_header header;           /*!< descriptor header, including type and size */
    uint8_t  bDescriptorSubtype;      /*!< AS_GENERAL descriptor subtype */
    uint8_t  bTerminalLink;           /*!< the terminal ID */
    uint8_t  bDelay;                  /*!< delay introduced by the data path */
    uint16_t wFormatTag;              /*!< the audio data format */
} usb_desc_AS_itf;

typedef struct
{
    usb_desc_header header;           /*!< descriptor header, including type and size */
    uint8_t  bDescriptorSubtype;      /*!< INPUT_TERMINAL descriptor subtype. */
    uint8_t  bTerminalID;             /*!< constant uniquely identifying the terminal within the audio function */
    uint16_t wTerminalType;           /*!< constant characterizing the type of terminal */
    uint8_t  bAssocTerminal;          /*!< ID of the output terminal */
    uint8_t  bNrChannels;             /*!< number of logical output channels */
    uint16_t wChannelConfig;          /*!< describes the spatial location of the logical channels */
    uint8_t  iChannelNames;           /*!< index of a string descriptor */
    uint8_t  iTerminal;               /*!< index of a string descriptor */
} usb_desc_input_terminal;

typedef struct
{
    usb_desc_header header;           /*!< descriptor header, including type and size */
    uint8_t  bDescriptorSubtype;      /*!< OUTPUT_TERMINAL descriptor subtype */
    uint8_t  bTerminalID;             /*!< constant uniquely identifying the terminal within the audio function */
    uint16_t wTerminalType;           /*!< constant characterizing the type of terminal */
    uint8_t  bAssocTerminal;          /*!< constant, identifying the input terminal to which this output terminal is associated */
    uint8_t  bSourceID;               /*!< ID of the unit or terminal */
    uint8_t  iTerminal;               /*!< index of a string descriptor */
} usb_desc_output_terminal;

typedef struct
{
    usb_desc_header header;           /*!< descriptor header, including type and size */
    uint8_t  bDescriptorSubtype;      /*!< FEATURE_UNIT descriptor subtype */
    uint8_t  bUnitID;                 /*!< constant uniquely identifying the unit within the audio function */
    uint8_t  bSourceID;               /*!< ID of the unit or terminal */
    uint8_t  bControlSize;            /*!< size in bytes of an element of the bmaControls() array */
    uint8_t  bmaControls0;            /*!< a bit set to 1 indicates that the mentioned control is supported for master channel 0 */
    uint8_t  bmaControls1;            /*!< a bit set to 1 indicates that the mentioned control is supported for logical channel 1 */
    uint8_t  iFeature;                /*!< index of a string descriptor */
} usb_desc_mono_feature_unit;

typedef struct
{
    usb_desc_header header;           /*!< descriptor header, including type and size */
    uint8_t  bDescriptorSubtype;      /*!< FEATURE_UNIT descriptor subtype */
    uint8_t  bUnitID;                 /*!< constant uniquely identifying the unit within the audio function */
    uint8_t  bSourceID;               /*!< ID of the unit or terminal */
    uint8_t  bControlSize;            /*!< size in bytes of an element of the bmaControls() array */
    uint16_t bmaControls0;            /*!< a bit set to 1 indicates that the mentioned control is supported for master channel 0 */
    uint16_t bmaControls1;            /*!< a bit set to 1 indicates that the mentioned control is supported for logical channel 1 */
    uint16_t bmaControls2;            /*!< a bit set to 1 indicates that the mentioned control is supported for logical channel 2 */
    uint8_t  iFeature;                /*!< index of a string descriptor */
} usb_desc_stereo_feature_unit;

typedef struct
{
    usb_desc_header header;           /*!< descriptor header, including type and size */
    uint8_t  bDescriptorSubtype;      /*!< FORMAT_TYPE descriptor subtype */
    uint8_t  bFormatType;             /*!< constant identifying the format type */
    uint8_t  bNrChannels;             /*!< indicates the number of physical channels in the audio data stream */
    uint8_t  bSubFrameSize;           /*!< the number of bytes occupied by one audio subframe */
    uint8_t  bBitResolution;          /*!< the number of effectively used bits from the available bits in an audio subframe */
    uint8_t  bSamFreqType;            /*!< indicates how the sampling frequency can be programmed */
    uint8_t  bSamFreq[3];             /*!< sampling frequency ns in Hz for this isochronous data endpoint */
} usb_desc_format_type;

typedef struct
{
    usb_desc_header header;           /*!< descriptor header, including type and size */
    uint8_t  bEndpointAddress;        /*!< the address of the endpoint */
    uint8_t  bmAttributes;            /*!< transfer type and synchronization type */
    uint16_t wMaxPacketSize;          /*!< maximum packet size this endpoint is capable of sending or receiving */
    uint8_t  bInterval;               /*!< left to the designer's discretion */
    uint8_t  bRefresh;                /*!< reset to 0 */
    uint8_t  bSynchAddress;           /*!< reset to 0 */
} usb_desc_std_ep;

typedef struct
{
    usb_desc_header header;           /*!< descriptor header, including type and size */
    uint8_t  bDescriptorSubtype;      /*!< EP_GENERAL descriptor subtype */
    uint8_t  bmAttributes;            /*!< transfer type and synchronization type */
    uint8_t  bLockDelayUnits;         /*!< indicates the units used for the wLockDelay field */
    uint16_t wLockDelay;              /*!< indicates the time it takes this endpoint to reliably lock its internal clock recovery circuitry */
} usb_desc_AS_ep;

#pragma pack()

/* USB configuration descriptor structure */
typedef struct
{
    usb_desc_config             config;
    usb_desc_itf                std_itf;
    usb_desc_AC_itf             ac_itf;

#ifdef USE_USB_AUDIO_MICPHONE
    usb_desc_input_terminal       mic_in_terminal;
    usb_desc_mono_feature_unit    mic_feature_unit;
    usb_desc_output_terminal      mic_out_terminal;
#endif

#ifdef USE_USB_AUDIO_SPEAKER
    usb_desc_input_terminal       speak_in_terminal;
    usb_desc_mono_feature_unit    speak_feature_unit;
    usb_desc_output_terminal      speak_out_terminal;
#endif

#ifdef USE_USB_AUDIO_MICPHONE
    usb_desc_itf                mic_std_as_itf_zeroband;
    usb_desc_itf                mic_std_as_itf_opera;
    usb_desc_AS_itf             mic_as_itf;
    usb_desc_format_type        mic_format_typeI;
    usb_desc_std_ep             mic_std_endpoint;
    usb_desc_AS_ep              mic_as_endpoint;
#endif

#ifdef USE_USB_AUDIO_SPEAKER
    usb_desc_itf                speak_std_as_itf_zeroband;
    usb_desc_itf                speak_std_as_itf_opera;
    usb_desc_AS_itf             speak_as_itf;
    usb_desc_format_type        speak_format_typeI;
    usb_desc_std_ep             speak_std_endpoint;
    usb_desc_AS_ep              speak_as_endpoint;
#endif
} usb_desc_config_set;

typedef struct
{
    /* main buffer for audio data out transfers and its relative pointers */
    uint8_t  isoc_out_buff[TOTAL_OUT_BUF_SIZE * 2U];
    uint8_t* isoc_out_wrptr;
    uint8_t* isoc_out_rdptr;

    /* main buffer for audio control requests transfers and its relative variables */
    uint8_t  audioctl[64];
    uint8_t  audioctl_unit;
    uint32_t audioctl_len;

#ifdef USE_USB_AUDIO_SPEAKER
    uint32_t play_flag;
#endif /* USE_USB_AUDIO_SPEAKER */
} usbd_audio_handler;

extern usb_desc audio_desc;
extern usb_class_core usbd_audio_cb;

#endif /* __AUDIO_CORE_H */
