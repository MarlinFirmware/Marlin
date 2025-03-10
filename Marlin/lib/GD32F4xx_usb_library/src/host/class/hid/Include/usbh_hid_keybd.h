/*!
    \file    usbh_hid_keybd.h
    \brief   header file for usbh_hid_keybd.c

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

#ifndef __USBH_HID_KEYBD_H
#define __USBH_HID_KEYBD_H

#include "usb_conf.h"
#include "usbh_hid_core.h"

//#define AZERTY_KEYBOARD
#define QWERTY_KEYBOARD

#define KBD_LEFT_CTRL                          0x01U
#define KBD_LEFT_SHIFT                         0x02U
#define KBD_LEFT_ALT                           0x04U
#define KBD_LEFT_GUI                           0x08U
#define KBD_RIGHT_CTRL                         0x10U
#define KBD_RIGHT_SHIFT                        0x20U
#define KBD_RIGHT_ALT                          0x40U
#define KBD_RIGHT_GUI                          0x80U

#define KEY_NONE                               0x00U
#define KEY_ERRORROLLOVER                      0x01U
#define KEY_POSTFAIL                           0x02U
#define KEY_ERRORUNDEFINED                     0x03U
#define KEY_A                                  0x04U
#define KEY_B                                  0x05U
#define KEY_C                                  0x06U
#define KEY_D                                  0x07U
#define KEY_E                                  0x08U
#define KEY_F                                  0x09U
#define KEY_G                                  0x0AU
#define KEY_H                                  0x0BU
#define KEY_I                                  0x0CU
#define KEY_J                                  0x0DU
#define KEY_K                                  0x0EU
#define KEY_L                                  0x0FU
#define KEY_M                                  0x10U
#define KEY_N                                  0x11U
#define KEY_O                                  0x12U
#define KEY_P                                  0x13U
#define KEY_Q                                  0x14U
#define KEY_R                                  0x15U
#define KEY_S                                  0x16U
#define KEY_T                                  0x17U
#define KEY_U                                  0x18U
#define KEY_V                                  0x19U
#define KEY_W                                  0x1AU
#define KEY_X                                  0x1BU
#define KEY_Y                                  0x1CU
#define KEY_Z                                  0x1DU
#define KEY_1_EXCLAMATION_MARK                 0x1EU
#define KEY_2_AT                               0x1FU
#define KEY_3_NUMBER_SIGN                      0x20U
#define KEY_4_DOLLAR                           0x21U
#define KEY_5_PERCENT                          0x22U
#define KEY_6_CARET                            0x23U
#define KEY_7_AMPERSAND                        0x24U
#define KEY_8_ASTERISK                         0x25U
#define KEY_9_OPARENTHESIS                     0x26U
#define KEY_0_CPARENTHESIS                     0x27U
#define KEY_ENTER                              0x28U
#define KEY_ESCAPE                             0x29U
#define KEY_BACKSPACE                          0x2AU
#define KEY_TAB                                0x2BU
#define KEY_SPACEBAR                           0x2CU
#define KEY_MINUS_UNDERSCORE                   0x2DU
#define KEY_EQUAL_PLUS                         0x2EU
#define KEY_OBRACKET_AND_OBRACE                0x2FU
#define KEY_CBRACKET_AND_CBRACE                0x30U
#define KEY_BACKSLASH_VERTICAL_BAR             0x31U
#define KEY_NONUS_NUMBER_SIGN_TILDE            0x32U
#define KEY_SEMICOLON_COLON                    0x33U
#define KEY_SINGLE_AND_DOUBLE_QUOTE            0x34U
#define KEY_GRAVE ACCENT AND TILDE             0x35U
#define KEY_COMMA_AND_LESS                     0x36U
#define KEY_DOT_GREATER                        0x37U
#define KEY_SLASH_QUESTION                     0x38U
#define KEY_CAPS LOCK                          0x39U
#define KEY_F1                                 0x3AU
#define KEY_F2                                 0x3BU
#define KEY_F3                                 0x3CU
#define KEY_F4                                 0x3DU
#define KEY_F5                                 0x3EU
#define KEY_F6                                 0x3FU
#define KEY_F7                                 0x40U
#define KEY_F8                                 0x41U
#define KEY_F9                                 0x42U
#define KEY_F10                                0x43U
#define KEY_F11                                0x44U
#define KEY_F12                                0x45U
#define KEY_PRINTSCREEN                        0x46U
#define KEY_SCROLL LOCK                        0x47U
#define KEY_PAUSE                              0x48U
#define KEY_INSERT                             0x49U
#define KEY_HOME                               0x4AU
#define KEY_PAGEUP                             0x4BU
#define KEY_DELETE                             0x4CU
#define KEY_END1                               0x4DU
#define KEY_PAGEDOWN                           0x4EU
#define KEY_RIGHTARROW                         0x4FU
#define KEY_LEFTARROW                          0x50U
#define KEY_DOWNARROW                          0x51U
#define KEY_UPARROW                            0x52U
#define KEY_KEYPAD_NUM_LOCK_AND_CLEAR          0x53U
#define KEY_KEYPAD_SLASH                       0x54U
#define KEY_KEYPAD_ASTERIKS                    0x55U
#define KEY_KEYPAD_MINUS                       0x56U
#define KEY_KEYPAD_PLUS                        0x57U
#define KEY_KEYPAD_ENTER                       0x58U
#define KEY_KEYPAD_1_END                       0x59U
#define KEY_KEYPAD_2_DOWN_ARROW                0x5AU
#define KEY_KEYPAD_3_PAGEDN                    0x5BU
#define KEY_KEYPAD_4_LEFT_ARROW                0x5CU
#define KEY_KEYPAD_5                           0x5DU
#define KEY_KEYPAD_6_RIGHT_ARROW               0x5EU
#define KEY_KEYPAD_7_HOME                      0x5FU
#define KEY_KEYPAD_8_UP_ARROW                  0x60U
#define KEY_KEYPAD_9_PAGEUP                    0x61U
#define KEY_KEYPAD_0_INSERT                    0x62U
#define KEY_KEYPAD_DECIMAL_SEPARATOR_DELETE    0x63U
#define KEY_NONUS_BACK_SLASH_VERTICAL_BAR      0x64U
#define KEY_APPLICATION                        0x65U
#define KEY_POWER                              0x66U
#define KEY_KEYPAD_EQUAL                       0x67U
#define KEY_F13                                0x68U
#define KEY_F14                                0x69U
#define KEY_F15                                0x6AU
#define KEY_F16                                0x6BU
#define KEY_F17                                0x6CU
#define KEY_F18                                0x6DU
#define KEY_F19                                0x6EU
#define KEY_F20                                0x6FU
#define KEY_F21                                0x70U
#define KEY_F22                                0x71U
#define KEY_F23                                0x72U
#define KEY_F24                                0x73U
#define KEY_EXECUTE                            0x74U
#define KEY_HELP                               0x75U
#define KEY_MENU                               0x76U
#define KEY_SELECT                             0x77U
#define KEY_STOP                               0x78U
#define KEY_AGAIN                              0x79U
#define KEY_UNDO                               0x7AU
#define KEY_CUT                                0x7BU
#define KEY_COPY                               0x7CU
#define KEY_PASTE                              0x7DU
#define KEY_FIND                               0x7EU
#define KEY_MUTE                               0x7FU
#define KEY_VOLUME_UP                          0x80U
#define KEY_VOLUME_DOWN                        0x81U
#define KEY_LOCKING_CAPS_LOCK                  0x82U
#define KEY_LOCKING_NUM_LOCK                   0x83U
#define KEY_LOCKING_SCROLL_LOCK                0x84U
#define KEY_KEYPAD_COMMA                       0x85U
#define KEY_KEYPAD_EQUAL_SIGN                  0x86U
#define KEY_INTERNATIONAL1                     0x87U
#define KEY_INTERNATIONAL2                     0x88U
#define KEY_INTERNATIONAL3                     0x89U
#define KEY_INTERNATIONAL4                     0x8AU
#define KEY_INTERNATIONAL5                     0x8BU
#define KEY_INTERNATIONAL6                     0x8CU
#define KEY_INTERNATIONAL7                     0x8DU
#define KEY_INTERNATIONAL8                     0x8EU
#define KEY_INTERNATIONAL9                     0x8FU
#define KEY_LANG1                              0x90U
#define KEY_LANG2                              0x91U
#define KEY_LANG3                              0x92U
#define KEY_LANG4                              0x93U
#define KEY_LANG5                              0x94U
#define KEY_LANG6                              0x95U
#define KEY_LANG7                              0x96U
#define KEY_LANG8                              0x97U
#define KEY_LANG9                              0x98U
#define KEY_ALTERNATE_ERASE                    0x99U
#define KEY_SYSREQ                             0x9AU
#define KEY_CANCEL                             0x9BU
#define KEY_CLEAR                              0x9CU
#define KEY_PRIOR                              0x9DU
#define KEY_RETURN                             0x9EU
#define KEY_SEPARATOR                          0x9FU
#define KEY_OUT                                0xA0U
#define KEY_OPER                               0xA1U
#define KEY_CLEAR_AGAIN                        0xA2U
#define KEY_CRSEL                              0xA3U
#define KEY_EXSEL                              0xA4U
#define KEY_KEYPAD_00                          0xB0U
#define KEY_KEYPAD_000                         0xB1U
#define KEY_THOUSANDS_SEPARATOR                0xB2U
#define KEY_DECIMAL_SEPARATOR                  0xB3U
#define KEY_CURRENCY_UNIT                      0xB4U
#define KEY_CURRENCY_SUB_UNIT                  0xB5U
#define KEY_KEYPAD_OPARENTHESIS                0xB6U
#define KEY_KEYPAD_CPARENTHESIS                0xB7U
#define KEY_KEYPAD_OBRACE                      0xB8U
#define KEY_KEYPAD_CBRACE                      0xB9U
#define KEY_KEYPAD_TAB                         0xBAU
#define KEY_KEYPAD_BACKSPACE                   0xBBU
#define KEY_KEYPAD_A                           0xBCU
#define KEY_KEYPAD_B                           0xBDU
#define KEY_KEYPAD_C                           0xBEU
#define KEY_KEYPAD_D                           0xBFU
#define KEY_KEYPAD_E                           0xC0U
#define KEY_KEYPAD_F                           0xC1U
#define KEY_KEYPAD_XOR                         0xC2U
#define KEY_KEYPAD_CARET                       0xC3U
#define KEY_KEYPAD_PERCENT                     0xC4U
#define KEY_KEYPAD_LESS                        0xC5U
#define KEY_KEYPAD_GREATER                     0xC6U
#define KEY_KEYPAD_AMPERSAND                   0xC7U
#define KEY_KEYPAD_LOGICAL_AND                 0xC8U
#define KEY_KEYPAD_VERTICAL_BAR                0xC9U
#define KEY_KEYPAD_LOGIACL_OR                  0xCAU
#define KEY_KEYPAD_COLON                       0xCBU
#define KEY_KEYPAD_NUMBER_SIGN                 0xCCU
#define KEY_KEYPAD_SPACE                       0xCDU
#define KEY_KEYPAD_AT                          0xCEU
#define KEY_KEYPAD_EXCLAMATION_MARK            0xCFU
#define KEY_KEYPAD_MEMORY_STORE                0xD0U
#define KEY_KEYPAD_MEMORY_RECALL               0xD1U
#define KEY_KEYPAD_MEMORY_CLEAR                0xD2U
#define KEY_KEYPAD_MEMORY_ADD                  0xD3U
#define KEY_KEYPAD_MEMORY_SUBTRACT             0xD4U
#define KEY_KEYPAD_MEMORY_MULTIPLY             0xD5U
#define KEY_KEYPAD_MEMORY_DIVIDE               0xD6U
#define KEY_KEYPAD_PLUSMINUS                   0xD7U
#define KEY_KEYPAD_CLEAR                       0xD8U
#define KEY_KEYPAD_CLEAR_ENTRY                 0xD9U
#define KEY_KEYPAD_BINARY                      0xDAU
#define KEY_KEYPAD_OCTAL                       0xDBU
#define KEY_KEYPAD_DECIMAL                     0xDCU
#define KEY_KEYPAD_HEXADECIMAL                 0xDDU
#define KEY_LEFTCONTROL                        0xE0U
#define KEY_LEFTSHIFT                          0xE1U
#define KEY_LEFTALT                            0xE2U
#define KEY_LEFT_GUI                           0xE3U
#define KEY_RIGHTCONTROL                       0xE4U
#define KEY_RIGHTSHIFT                         0xE5U
#define KEY_RIGHTALT                           0xE6U
#define KEY_RIGHT_GUI                          0xE7U

#define KBR_MAX_NBR_PRESSED                    6U

typedef struct
{
    uint8_t state;
    uint8_t lctrl;
    uint8_t lshift;
    uint8_t lalt;
    uint8_t lgui;
    uint8_t rctrl;
    uint8_t rshift;
    uint8_t ralt;
    uint8_t rgui;
    uint8_t keys[6];
} hid_keybd_info;

/* function declarations */
/* initialize keyboard */
void usr_keybrd_init (void);
/* process keyboard data */
void usr_keybrd_process_data (uint8_t pbuf);
/* initialize the keyboard function */
usbh_status usbh_hid_keybd_init (usb_core_driver *udev, usbh_host *uhost);
/* get keyboard information */
hid_keybd_info *usbh_hid_keybd_info_get (usb_core_driver *udev, usbh_host *uhost);
/* get the ascii code of hid */
uint8_t usbh_hid_ascii_code_get (hid_keybd_info *info);
/* keyboard machine */
void usbh_hid_keybrd_machine (usb_core_driver *udev, usbh_host *uhost);

#endif /* __USBH_HID_KEYBD_H */
