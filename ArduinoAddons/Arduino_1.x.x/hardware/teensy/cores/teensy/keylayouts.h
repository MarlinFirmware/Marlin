/* Teensyduino Core Library
 * http://www.pjrc.com/teensy/
 * Copyright (c) 2013 PJRC.COM, LLC.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * 1. The above copyright notice and this permission notice shall be 
 * included in all copies or substantial portions of the Software.
 *
 * 2. If the Software is incorporated into a build system that allows 
 * selection among a list of target devices, then similar target
 * devices manufactured by PJRC.COM must be included in the list of
 * target devices and selectable in the same manner.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef KEYLAYOUTS_H__
#define KEYLAYOUTS_H__

#include <stdint.h>
#include <avr/pgmspace.h>

#ifdef __cplusplus
extern "C"{
#endif

//#define LAYOUT_US_ENGLISH
//#define LAYOUT_CANADIAN_FRENCH
//#define LAYOUT_CANADIAN_MULTILINGUAL
//#define LAYOUT_DANISH
//#define LAYOUT_FINNISH
//#define LAYOUT_FRENCH
//#define LAYOUT_FRENCH_BELGIAN
//#define LAYOUT_FRENCH_SWISS
//#define LAYOUT_GERMAN
//#define LAYOUT_GERMAN_MAC
//#define LAYOUT_GERMAN_SWISS
//#define LAYOUT_ICELANDIC
//#define LAYOUT_IRISH
//#define LAYOUT_ITALIAN
//#define LAYOUT_NORWEGIAN
//#define LAYOUT_PORTUGUESE
//#define LAYOUT_PORTUGUESE_BRAZILIAN
//#define LAYOUT_SPANISH
//#define LAYOUT_SPANISH_LATIN_AMERICA
//#define LAYOUT_SWEDISH
//#define LAYOUT_TURKISH
//#define LAYOUT_UNITED_KINGDOM	
//#define LAYOUT_US_INTERNATIONAL



// http://en.wikipedia.org/wiki/Keyboard_layout


#define MODIFIERKEY_CTRL        ( 0x01 | 0x8000 )
#define MODIFIERKEY_SHIFT       ( 0x02 | 0x8000 )
#define MODIFIERKEY_ALT         ( 0x04 | 0x8000 )
#define MODIFIERKEY_GUI         ( 0x08 | 0x8000 )
#define MODIFIERKEY_LEFT_CTRL   ( 0x01 | 0x8000 )
#define MODIFIERKEY_LEFT_SHIFT  ( 0x02 | 0x8000 )
#define MODIFIERKEY_LEFT_ALT    ( 0x04 | 0x8000 )
#define MODIFIERKEY_LEFT_GUI    ( 0x08 | 0x8000 )
#define MODIFIERKEY_RIGHT_CTRL  ( 0x10 | 0x8000 )
#define MODIFIERKEY_RIGHT_SHIFT ( 0x20 | 0x8000 )
#define MODIFIERKEY_RIGHT_ALT   ( 0x40 | 0x8000 )
#define MODIFIERKEY_RIGHT_GUI   ( 0x80 | 0x8000 )

#define KEY_MEDIA_VOLUME_INC    0x01
#define KEY_MEDIA_VOLUME_DEC    0x02
#define KEY_MEDIA_MUTE          0x04
#define KEY_MEDIA_PLAY_PAUSE    0x08
#define KEY_MEDIA_NEXT_TRACK    0x10
#define KEY_MEDIA_PREV_TRACK    0x20
#define KEY_MEDIA_STOP          0x40
#define KEY_MEDIA_EJECT         0x80

#define KEY_A           ( 4   | 0x4000 )
#define KEY_B           ( 5   | 0x4000 )
#define KEY_C           ( 6   | 0x4000 )
#define KEY_D           ( 7   | 0x4000 )
#define KEY_E           ( 8   | 0x4000 )
#define KEY_F           ( 9   | 0x4000 )
#define KEY_G           ( 10  | 0x4000 )
#define KEY_H           ( 11  | 0x4000 )
#define KEY_I           ( 12  | 0x4000 )
#define KEY_J           ( 13  | 0x4000 )
#define KEY_K           ( 14  | 0x4000 )
#define KEY_L           ( 15  | 0x4000 )
#define KEY_M           ( 16  | 0x4000 )
#define KEY_N           ( 17  | 0x4000 )
#define KEY_O           ( 18  | 0x4000 )
#define KEY_P           ( 19  | 0x4000 )
#define KEY_Q           ( 20  | 0x4000 )
#define KEY_R           ( 21  | 0x4000 )
#define KEY_S           ( 22  | 0x4000 )
#define KEY_T           ( 23  | 0x4000 )
#define KEY_U           ( 24  | 0x4000 )
#define KEY_V           ( 25  | 0x4000 )
#define KEY_W           ( 26  | 0x4000 )
#define KEY_X           ( 27  | 0x4000 )
#define KEY_Y           ( 28  | 0x4000 )
#define KEY_Z           ( 29  | 0x4000 )
#define KEY_1           ( 30  | 0x4000 )
#define KEY_2           ( 31  | 0x4000 )
#define KEY_3           ( 32  | 0x4000 )
#define KEY_4           ( 33  | 0x4000 )
#define KEY_5           ( 34  | 0x4000 )
#define KEY_6           ( 35  | 0x4000 )
#define KEY_7           ( 36  | 0x4000 )
#define KEY_8           ( 37  | 0x4000 )
#define KEY_9           ( 38  | 0x4000 )
#define KEY_0           ( 39  | 0x4000 )
#define KEY_ENTER       ( 40  | 0x4000 )
#define KEY_ESC         ( 41  | 0x4000 )
#define KEY_BACKSPACE   ( 42  | 0x4000 )
#define KEY_TAB         ( 43  | 0x4000 )
#define KEY_SPACE       ( 44  | 0x4000 )
#define KEY_MINUS       ( 45  | 0x4000 )
#define KEY_EQUAL       ( 46  | 0x4000 )
#define KEY_LEFT_BRACE  ( 47  | 0x4000 )
#define KEY_RIGHT_BRACE ( 48  | 0x4000 )
#define KEY_BACKSLASH   ( 49  | 0x4000 )
#define KEY_NON_US_NUM  ( 50  | 0x4000 )
#define KEY_SEMICOLON   ( 51  | 0x4000 )
#define KEY_QUOTE       ( 52  | 0x4000 )
#define KEY_TILDE       ( 53  | 0x4000 )
#define KEY_COMMA       ( 54  | 0x4000 )
#define KEY_PERIOD      ( 55  | 0x4000 )
#define KEY_SLASH       ( 56  | 0x4000 )
#define KEY_CAPS_LOCK   ( 57  | 0x4000 )
#define KEY_F1          ( 58  | 0x4000 )
#define KEY_F2          ( 59  | 0x4000 )
#define KEY_F3          ( 60  | 0x4000 )
#define KEY_F4          ( 61  | 0x4000 )
#define KEY_F5          ( 62  | 0x4000 )
#define KEY_F6          ( 63  | 0x4000 )
#define KEY_F7          ( 64  | 0x4000 )
#define KEY_F8          ( 65  | 0x4000 )
#define KEY_F9          ( 66  | 0x4000 )
#define KEY_F10         ( 67  | 0x4000 )
#define KEY_F11         ( 68  | 0x4000 )
#define KEY_F12         ( 69  | 0x4000 )
#define KEY_PRINTSCREEN ( 70  | 0x4000 )
#define KEY_SCROLL_LOCK ( 71  | 0x4000 )
#define KEY_PAUSE       ( 72  | 0x4000 )
#define KEY_INSERT      ( 73  | 0x4000 )
#define KEY_HOME        ( 74  | 0x4000 )
#define KEY_PAGE_UP     ( 75  | 0x4000 )
#define KEY_DELETE      ( 76  | 0x4000 )
#define KEY_END         ( 77  | 0x4000 )
#define KEY_PAGE_DOWN   ( 78  | 0x4000 )
#define KEY_RIGHT       ( 79  | 0x4000 )
#define KEY_LEFT        ( 80  | 0x4000 )
#define KEY_DOWN        ( 81  | 0x4000 )
#define KEY_UP          ( 82  | 0x4000 )
#define KEY_NUM_LOCK    ( 83  | 0x4000 )
#define KEYPAD_SLASH    ( 84  | 0x4000 )
#define KEYPAD_ASTERIX  ( 85  | 0x4000 )
#define KEYPAD_MINUS    ( 86  | 0x4000 )
#define KEYPAD_PLUS     ( 87  | 0x4000 )
#define KEYPAD_ENTER    ( 88  | 0x4000 )
#define KEYPAD_1        ( 89  | 0x4000 )
#define KEYPAD_2        ( 90  | 0x4000 )
#define KEYPAD_3        ( 91  | 0x4000 )
#define KEYPAD_4        ( 92  | 0x4000 )
#define KEYPAD_5        ( 93  | 0x4000 )
#define KEYPAD_6        ( 94  | 0x4000 )
#define KEYPAD_7        ( 95  | 0x4000 )
#define KEYPAD_8        ( 96  | 0x4000 )
#define KEYPAD_9        ( 97  | 0x4000 )
#define KEYPAD_0        ( 98  | 0x4000 )
#define KEYPAD_PERIOD   ( 99  | 0x4000 )
#define KEY_MENU	( 101 | 0x4000 )
#define KEY_F13         ( 104 | 0x4000 )
#define KEY_F14         ( 105 | 0x4000 )
#define KEY_F15         ( 106 | 0x4000 )
#define KEY_F16         ( 107 | 0x4000 )
#define KEY_F17         ( 108 | 0x4000 )
#define KEY_F18         ( 109 | 0x4000 )
#define KEY_F19         ( 110 | 0x4000 )
#define KEY_F20         ( 111 | 0x4000 )
#define KEY_F21         ( 112 | 0x4000 )
#define KEY_F22         ( 113 | 0x4000 )
#define KEY_F23         ( 114 | 0x4000 )
#define KEY_F24         ( 115 | 0x4000 )


// for compatibility with Leonardo's slightly different names
#define KEY_UP_ARROW	KEY_UP
#define KEY_DOWN_ARROW	KEY_DOWN
#define KEY_LEFT_ARROW	KEY_LEFT
#define KEY_RIGHT_ARROW	KEY_RIGHT
#define KEY_RETURN	KEY_ENTER
#define KEY_LEFT_CTRL	MODIFIERKEY_LEFT_CTRL
#define KEY_LEFT_SHIFT	MODIFIERKEY_LEFT_SHIFT
#define KEY_LEFT_ALT	MODIFIERKEY_LEFT_ALT
#define KEY_LEFT_GUI	MODIFIERKEY_LEFT_GUI
#define KEY_RIGHT_CTRL	MODIFIERKEY_RIGHT_CTRL
#define KEY_RIGHT_SHIFT	MODIFIERKEY_RIGHT_SHIFT
#define KEY_RIGHT_ALT	MODIFIERKEY_RIGHT_ALT
#define KEY_RIGHT_GUI	MODIFIERKEY_RIGHT_GUI


// Everything below this line is not intended for use in "normal" programs.
// These private symbols populate lookup tables, which are used to translate
// ascii and UTF8 unicode into keystroke sequences.


#ifdef LAYOUT_US_ENGLISH

#define SHIFT_MASK		0x40
#define KEYCODE_TYPE		uint8_t

#define ASCII_20	KEY_SPACE				// 32  
#define ASCII_21	KEY_1 + SHIFT_MASK			// 33 !
#define ASCII_22	KEY_QUOTE + SHIFT_MASK			// 34 "
#define ASCII_23	KEY_3 + SHIFT_MASK			// 35 #
#define ASCII_24	KEY_4 + SHIFT_MASK			// 36 $
#define ASCII_25	KEY_5 + SHIFT_MASK			// 37 %
#define ASCII_26	KEY_7 + SHIFT_MASK			// 38 &
#define ASCII_27	KEY_QUOTE				// 39 '  
#define ASCII_28	KEY_9 + SHIFT_MASK			// 40 ( 
#define ASCII_29	KEY_0 + SHIFT_MASK			// 41 )
#define ASCII_2A	KEY_8 + SHIFT_MASK			// 42 *
#define ASCII_2B	KEY_EQUAL + SHIFT_MASK			// 43 +
#define ASCII_2C	KEY_COMMA				// 44 ,
#define ASCII_2D	KEY_MINUS				// 45 -
#define ASCII_2E	KEY_PERIOD				// 46 .
#define ASCII_2F	KEY_SLASH				// 47 /
#define ASCII_30	KEY_0					// 48 0
#define ASCII_31	KEY_1					// 49 1
#define ASCII_32	KEY_2					// 50 2
#define ASCII_33	KEY_3					// 51 3
#define ASCII_34	KEY_4					// 52 4
#define ASCII_35	KEY_5					// 53 5
#define ASCII_36	KEY_6					// 54 6
#define ASCII_37	KEY_7					// 55 7
#define ASCII_38	KEY_8					// 55 8
#define ASCII_39	KEY_9					// 57 9
#define ASCII_3A	KEY_SEMICOLON + SHIFT_MASK		// 58 :
#define ASCII_3B	KEY_SEMICOLON				// 59 ;
#define ASCII_3C	KEY_COMMA + SHIFT_MASK			// 60 <
#define ASCII_3D	KEY_EQUAL				// 61 =
#define ASCII_3E	KEY_PERIOD + SHIFT_MASK			// 62 >
#define ASCII_3F	KEY_SLASH + SHIFT_MASK			// 63 ?
#define ASCII_40	KEY_2 + SHIFT_MASK			// 64 @
#define ASCII_41	KEY_A + SHIFT_MASK			// 65 A
#define ASCII_42	KEY_B + SHIFT_MASK			// 66 B
#define ASCII_43	KEY_C + SHIFT_MASK			// 67 C
#define ASCII_44	KEY_D + SHIFT_MASK			// 68 D
#define ASCII_45	KEY_E + SHIFT_MASK			// 69 E
#define ASCII_46	KEY_F + SHIFT_MASK			// 70 F
#define ASCII_47	KEY_G + SHIFT_MASK			// 71 G
#define ASCII_48	KEY_H + SHIFT_MASK			// 72 H
#define ASCII_49	KEY_I + SHIFT_MASK			// 73 I
#define ASCII_4A	KEY_J + SHIFT_MASK			// 74 J
#define ASCII_4B	KEY_K + SHIFT_MASK			// 75 K
#define ASCII_4C	KEY_L + SHIFT_MASK			// 76 L
#define ASCII_4D	KEY_M + SHIFT_MASK			// 77 M
#define ASCII_4E	KEY_N + SHIFT_MASK			// 78 N
#define ASCII_4F	KEY_O + SHIFT_MASK			// 79 O
#define ASCII_50	KEY_P + SHIFT_MASK			// 80 P
#define ASCII_51	KEY_Q + SHIFT_MASK			// 81 Q
#define ASCII_52	KEY_R + SHIFT_MASK			// 82 R
#define ASCII_53	KEY_S + SHIFT_MASK			// 83 S
#define ASCII_54	KEY_T + SHIFT_MASK			// 84 T
#define ASCII_55	KEY_U + SHIFT_MASK			// 85 U
#define ASCII_56	KEY_V + SHIFT_MASK			// 86 V
#define ASCII_57	KEY_W + SHIFT_MASK			// 87 W
#define ASCII_58	KEY_X + SHIFT_MASK			// 88 X
#define ASCII_59	KEY_Y + SHIFT_MASK			// 89 Y
#define ASCII_5A	KEY_Z + SHIFT_MASK			// 90 Z
#define ASCII_5B	KEY_LEFT_BRACE				// 91 [
#define ASCII_5C	KEY_BACKSLASH				// 92 
#define ASCII_5D	KEY_RIGHT_BRACE				// 93 ]
#define ASCII_5E	KEY_6 + SHIFT_MASK			// 94 ^
#define ASCII_5F	KEY_MINUS + SHIFT_MASK			// 95 _
#define ASCII_60	KEY_TILDE				// 96 `
#define ASCII_61	KEY_A					// 97 a
#define ASCII_62	KEY_B					// 98 b
#define ASCII_63	KEY_C					// 99 c
#define ASCII_64	KEY_D					// 100 d
#define ASCII_65	KEY_E					// 101 e
#define ASCII_66	KEY_F					// 102 f
#define ASCII_67	KEY_G					// 103 g
#define ASCII_68	KEY_H					// 104 h
#define ASCII_69	KEY_I					// 105 i
#define ASCII_6A	KEY_J					// 106 j
#define ASCII_6B	KEY_K					// 107 k
#define ASCII_6C	KEY_L					// 108 l
#define ASCII_6D	KEY_M					// 109 m
#define ASCII_6E	KEY_N					// 110 n
#define ASCII_6F	KEY_O					// 111 o
#define ASCII_70	KEY_P					// 112 p
#define ASCII_71	KEY_Q					// 113 q
#define ASCII_72	KEY_R					// 114 r
#define ASCII_73	KEY_S					// 115 s
#define ASCII_74	KEY_T					// 116 t
#define ASCII_75	KEY_U					// 117 u
#define ASCII_76	KEY_V					// 118 v
#define ASCII_77	KEY_W					// 119 w
#define ASCII_78	KEY_X					// 120 x
#define ASCII_79	KEY_Y					// 121 y
#define ASCII_7A	KEY_Z					// 122 z
#define ASCII_7B	KEY_LEFT_BRACE + SHIFT_MASK		// 123 {
#define ASCII_7C	KEY_BACKSLASH + SHIFT_MASK		// 124 |
#define ASCII_7D	KEY_RIGHT_BRACE + SHIFT_MASK		// 125 }
#define ASCII_7E	KEY_TILDE + SHIFT_MASK			// 126 ~
#define ASCII_7F	KEY_BACKSPACE				// 127

#endif // LAYOUT_US_ENGLISH




#ifdef LAYOUT_US_INTERNATIONAL

#define SHIFT_MASK		0x0040
#define ALTGR_MASK		0x0080
#define DEADKEYS_MASK		0x0700
#define	CIRCUMFLEX_BITS		0x0100
#define ACUTE_ACCENT_BITS	0x0200
#define GRAVE_ACCENT_BITS	0x0300
#define TILDE_BITS		0x0400
#define DIAERESIS_BITS		0x0500
#define KEYCODE_TYPE		uint16_t
#define DEADKEY_CIRCUMFLEX	KEY_6 + SHIFT_MASK
#define DEADKEY_ACUTE_ACCENT	KEY_QUOTE
#define DEADKEY_GRAVE_ACCENT	KEY_TILDE
#define DEADKEY_TILDE		KEY_TILDE + SHIFT_MASK
#define DEADKEY_DIAERESIS	KEY_QUOTE + SHIFT_MASK

#define ASCII_20	KEY_SPACE				// 32  
#define ASCII_21	KEY_1 + SHIFT_MASK			// 33 !
#define ASCII_22	DIAERESIS_BITS + KEY_SPACE		// 34 "
#define ASCII_23	KEY_3 + SHIFT_MASK			// 35 #
#define ASCII_24	KEY_4 + SHIFT_MASK			// 36 $
#define ASCII_25	KEY_5 + SHIFT_MASK			// 37 %
#define ASCII_26	KEY_7 + SHIFT_MASK			// 38 &
#define ASCII_27	ACUTE_ACCENT_BITS + KEY_SPACE		// 39 '  
#define ASCII_28	KEY_9 + SHIFT_MASK			// 40 ( 
#define ASCII_29	KEY_0 + SHIFT_MASK			// 41 )
#define ASCII_2A	KEY_8 + SHIFT_MASK			// 42 *
#define ASCII_2B	KEY_EQUAL + SHIFT_MASK			// 43 +
#define ASCII_2C	KEY_COMMA				// 44 ,
#define ASCII_2D	KEY_MINUS				// 45 -
#define ASCII_2E	KEY_PERIOD				// 46 .
#define ASCII_2F	KEY_SLASH				// 47 /
#define ASCII_30	KEY_0					// 48 0
#define ASCII_31	KEY_1					// 49 1
#define ASCII_32	KEY_2					// 50 2
#define ASCII_33	KEY_3					// 51 3
#define ASCII_34	KEY_4					// 52 4
#define ASCII_35	KEY_5					// 53 5
#define ASCII_36	KEY_6					// 54 6
#define ASCII_37	KEY_7					// 55 7
#define ASCII_38	KEY_8					// 55 8
#define ASCII_39	KEY_9					// 57 9
#define ASCII_3A	KEY_SEMICOLON + SHIFT_MASK		// 58 :
#define ASCII_3B	KEY_SEMICOLON				// 59 ;
#define ASCII_3C	KEY_COMMA + SHIFT_MASK			// 60 <
#define ASCII_3D	KEY_EQUAL				// 61 =
#define ASCII_3E	KEY_PERIOD + SHIFT_MASK			// 62 >
#define ASCII_3F	KEY_SLASH + SHIFT_MASK			// 63 ?
#define ASCII_40	KEY_2 + SHIFT_MASK			// 64 @
#define ASCII_41	KEY_A + SHIFT_MASK			// 65 A
#define ASCII_42	KEY_B + SHIFT_MASK			// 66 B
#define ASCII_43	KEY_C + SHIFT_MASK			// 67 C
#define ASCII_44	KEY_D + SHIFT_MASK			// 68 D
#define ASCII_45	KEY_E + SHIFT_MASK			// 69 E
#define ASCII_46	KEY_F + SHIFT_MASK			// 70 F
#define ASCII_47	KEY_G + SHIFT_MASK			// 71 G
#define ASCII_48	KEY_H + SHIFT_MASK			// 72 H
#define ASCII_49	KEY_I + SHIFT_MASK			// 73 I
#define ASCII_4A	KEY_J + SHIFT_MASK			// 74 J
#define ASCII_4B	KEY_K + SHIFT_MASK			// 75 K
#define ASCII_4C	KEY_L + SHIFT_MASK			// 76 L
#define ASCII_4D	KEY_M + SHIFT_MASK			// 77 M
#define ASCII_4E	KEY_N + SHIFT_MASK			// 78 N
#define ASCII_4F	KEY_O + SHIFT_MASK			// 79 O
#define ASCII_50	KEY_P + SHIFT_MASK			// 80 P
#define ASCII_51	KEY_Q + SHIFT_MASK			// 81 Q
#define ASCII_52	KEY_R + SHIFT_MASK			// 82 R
#define ASCII_53	KEY_S + SHIFT_MASK			// 83 S
#define ASCII_54	KEY_T + SHIFT_MASK			// 84 T
#define ASCII_55	KEY_U + SHIFT_MASK			// 85 U
#define ASCII_56	KEY_V + SHIFT_MASK			// 86 V
#define ASCII_57	KEY_W + SHIFT_MASK			// 87 W
#define ASCII_58	KEY_X + SHIFT_MASK			// 88 X
#define ASCII_59	KEY_Y + SHIFT_MASK			// 89 Y
#define ASCII_5A	KEY_Z + SHIFT_MASK			// 90 Z
#define ASCII_5B	KEY_LEFT_BRACE				// 91 [
#define ASCII_5C	KEY_BACKSLASH				// 92 
#define ASCII_5D	KEY_RIGHT_BRACE				// 93 ]
#define ASCII_5E	CIRCUMFLEX_BITS + KEY_SPACE		// 94 ^
#define ASCII_5F	KEY_MINUS + SHIFT_MASK			// 95 _
#define ASCII_60	GRAVE_ACCENT_BITS + KEY_SPACE		// 96 `
#define ASCII_61	KEY_A					// 97 a
#define ASCII_62	KEY_B					// 98 b
#define ASCII_63	KEY_C					// 99 c
#define ASCII_64	KEY_D					// 100 d
#define ASCII_65	KEY_E					// 101 e
#define ASCII_66	KEY_F					// 102 f
#define ASCII_67	KEY_G					// 103 g
#define ASCII_68	KEY_H					// 104 h
#define ASCII_69	KEY_I					// 105 i
#define ASCII_6A	KEY_J					// 106 j
#define ASCII_6B	KEY_K					// 107 k
#define ASCII_6C	KEY_L					// 108 l
#define ASCII_6D	KEY_M					// 109 m
#define ASCII_6E	KEY_N					// 110 n
#define ASCII_6F	KEY_O					// 111 o
#define ASCII_70	KEY_P					// 112 p
#define ASCII_71	KEY_Q					// 113 q
#define ASCII_72	KEY_R					// 114 r
#define ASCII_73	KEY_S					// 115 s
#define ASCII_74	KEY_T					// 116 t
#define ASCII_75	KEY_U					// 117 u
#define ASCII_76	KEY_V					// 118 v
#define ASCII_77	KEY_W					// 119 w
#define ASCII_78	KEY_X					// 120 x
#define ASCII_79	KEY_Y					// 121 y
#define ASCII_7A	KEY_Z					// 122 z
#define ASCII_7B	KEY_LEFT_BRACE + SHIFT_MASK		// 123 {
#define ASCII_7C	KEY_BACKSLASH + SHIFT_MASK		// 124 |
#define ASCII_7D	KEY_RIGHT_BRACE + SHIFT_MASK		// 125 }
#define ASCII_7E	TILDE_BITS + KEY_SPACE			// 126 ~
#define ASCII_7F	KEY_BACKSPACE				// 127
#define ISO_8859_1_A0	KEY_SPACE				// 160       Nonbreakng Space
#define ISO_8859_1_A1	KEY_1 + ALTGR_MASK			// 161 ¡     Inverted Exclamation
#define ISO_8859_1_A2	KEY_C + ALTGR_MASK + SHIFT_MASK		// 162 ¢     Cent SIGN
#define ISO_8859_1_A3	KEY_4 + ALTGR_MASK + SHIFT_MASK		// 163 £     Pound Sign
#define ISO_8859_1_A4	KEY_4 + ALTGR_MASK			// 164 ¤     Currency or Euro Sign
#define ISO_8859_1_A5	KEY_MINUS + ALTGR_MASK			// 165 ¥     YEN SIGN
#define ISO_8859_1_A6	KEY_BACKSLASH + ALTGR_MASK + SHIFT_MASK	// 166 ¦     BROKEN BAR			??
#define ISO_8859_1_A7	KEY_S + ALTGR_MASK + SHIFT_MASK		// 167 §     SECTION SIGN
#define ISO_8859_1_A8	KEY_QUOTE + ALTGR_MASK + SHIFT_MASK	// 168 ¨     DIAERESIS
#define ISO_8859_1_A9	KEY_C + ALTGR_MASK			// 169 ©     COPYRIGHT SIGN
#define ISO_8859_1_AA	0					// 170 ª     FEMININE ORDINAL
#define ISO_8859_1_AB	KEY_LEFT_BRACE + ALTGR_MASK		// 171 «     LEFT DOUBLE ANGLE QUOTE
#define ISO_8859_1_AC	KEY_BACKSLASH + ALTGR_MASK		// 172 ¬     NOT SIGN			??
#define ISO_8859_1_AD	0					// 173       SOFT HYPHEN
#define ISO_8859_1_AE	KEY_R + ALTGR_MASK			// 174 ®     REGISTERED SIGN
#define ISO_8859_1_AF	0					// 175 ¯     MACRON
#define ISO_8859_1_B0	KEY_SEMICOLON + ALTGR_MASK + SHIFT_MASK	// 176 °     DEGREE SIGN
#define ISO_8859_1_B1	0					// 177 ±     PLUS-MINUS SIGN
#define ISO_8859_1_B2	KEY_2 + ALTGR_MASK			// 178 ²     SUPERSCRIPT TWO
#define ISO_8859_1_B3	KEY_3 + ALTGR_MASK			// 179 ³     SUPERSCRIPT THREE
#define ISO_8859_1_B4	KEY_QUOTE + ALTGR_MASK			// 180 ´     ACUTE ACCENT
#define ISO_8859_1_B5	KEY_M + ALTGR_MASK			// 181 µ     MICRO SIGN
#define ISO_8859_1_B6	KEY_SEMICOLON + ALTGR_MASK		// 182 ¶     PILCROW SIGN
#define ISO_8859_1_B7	0					// 183 ·     MIDDLE DOT
#define ISO_8859_1_B8	0					// 184 ¸     CEDILLA
#define ISO_8859_1_B9	KEY_1 + ALTGR_MASK + SHIFT_MASK		// 185 ¹     SUPERSCRIPT ONE
#define ISO_8859_1_BA	0					// 186 º     MASCULINE ORDINAL
#define ISO_8859_1_BB	KEY_RIGHT_BRACE + ALTGR_MASK		// 187 »     RIGHT DOUBLE ANGLE QUOTE
#define ISO_8859_1_BC	KEY_6 + ALTGR_MASK			// 188 ¼     FRACTION ONE QUARTER
#define ISO_8859_1_BD	KEY_7 + ALTGR_MASK			// 189 ½     FRACTION ONE HALF
#define ISO_8859_1_BE	KEY_8 + ALTGR_MASK			// 190 ¾     FRACTION THREE QUARTERS
#define ISO_8859_1_BF	KEY_SLASH + ALTGR_MASK			// 191 ¿     INVERTED QUESTION MARK
#define ISO_8859_1_C0	GRAVE_ACCENT_BITS + KEY_A + SHIFT_MASK	// 192 À     A GRAVE
#define ISO_8859_1_C1	KEY_A + ALTGR_MASK + SHIFT_MASK		// 193 Á     A ACUTE
#define ISO_8859_1_C2	CIRCUMFLEX_BITS	+ KEY_A + SHIFT_MASK	// 194 Â     A CIRCUMFLEX
#define ISO_8859_1_C3	TILDE_BITS + KEY_A + SHIFT_MASK		// 195 Ã     A TILDE
#define ISO_8859_1_C4	KEY_Q + ALTGR_MASK + SHIFT_MASK		// 196 Ä     A DIAERESIS
#define ISO_8859_1_C5	KEY_W + ALTGR_MASK + SHIFT_MASK		// 197 Å     A RING ABOVE
#define ISO_8859_1_C6	KEY_Z + ALTGR_MASK + SHIFT_MASK		// 198 Æ     AE
#define ISO_8859_1_C7	KEY_COMMA + ALTGR_MASK + SHIFT_MASK	// 199 Ç     C CEDILLA
#define ISO_8859_1_C8	GRAVE_ACCENT_BITS + KEY_E + SHIFT_MASK	// 200 È     E GRAVE
#define ISO_8859_1_C9	KEY_E + ALTGR_MASK + SHIFT_MASK		// 201 É     E ACUTE
#define ISO_8859_1_CA	CIRCUMFLEX_BITS + KEY_E + SHIFT_MASK	// 202 Ê     E CIRCUMFLEX
#define ISO_8859_1_CB	DIAERESIS_BITS + KEY_E + SHIFT_MASK	// 203 Ë     E DIAERESIS
#define ISO_8859_1_CC	GRAVE_ACCENT_BITS + KEY_I + SHIFT_MASK	// 204 Ì     I GRAVE
#define ISO_8859_1_CD	KEY_I + ALTGR_MASK + SHIFT_MASK		// 205 Í     I ACUTE
#define ISO_8859_1_CE	CIRCUMFLEX_BITS + KEY_I + SHIFT_MASK	// 206 Î     I CIRCUMFLEX
#define ISO_8859_1_CF	DIAERESIS_BITS + KEY_I + SHIFT_MASK	// 207 Ï     I DIAERESIS
#define ISO_8859_1_D0	KEY_D + ALTGR_MASK + SHIFT_MASK		// 208 Ð     ETH
#define ISO_8859_1_D1	KEY_N + ALTGR_MASK + SHIFT_MASK		// 209 Ñ     N TILDE
#define ISO_8859_1_D2	GRAVE_ACCENT_BITS + KEY_O + SHIFT_MASK	// 210 Ò     O GRAVE
#define ISO_8859_1_D3	KEY_O + ALTGR_MASK + SHIFT_MASK		// 211 Ó     O ACUTE
#define ISO_8859_1_D4	CIRCUMFLEX_BITS + KEY_O + SHIFT_MASK	// 212 Ô     O CIRCUMFLEX
#define ISO_8859_1_D5	TILDE_BITS + KEY_O + SHIFT_MASK		// 213 Õ     O TILDE
#define ISO_8859_1_D6	KEY_P + ALTGR_MASK + SHIFT_MASK		// 214 Ö     O DIAERESIS
#define ISO_8859_1_D7	KEY_EQUAL + ALTGR_MASK			// 215 ×     MULTIPLICATION
#define ISO_8859_1_D8	KEY_L + ALTGR_MASK + SHIFT_MASK		// 216 Ø     O STROKE
#define ISO_8859_1_D9	GRAVE_ACCENT_BITS + KEY_U + SHIFT_MASK	// 217 Ù     U GRAVE
#define ISO_8859_1_DA	KEY_U + ALTGR_MASK + SHIFT_MASK		// 218 Ú     U ACUTE
#define ISO_8859_1_DB	CIRCUMFLEX_BITS + KEY_U + SHIFT_MASK	// 219 Û     U CIRCUMFLEX
#define ISO_8859_1_DC	KEY_Y + ALTGR_MASK + SHIFT_MASK		// 220 Ü     U DIAERESIS
#define ISO_8859_1_DD	ACUTE_ACCENT_BITS + KEY_Y + SHIFT_MASK	// 221 Ý     Y ACUTE
#define ISO_8859_1_DE	KEY_T + ALTGR_MASK + SHIFT_MASK		// 222 Þ     THORN
#define ISO_8859_1_DF	KEY_S + ALTGR_MASK			// 223 ß     SHARP S
#define ISO_8859_1_E0	GRAVE_ACCENT_BITS + KEY_A		// 224 à     a GRAVE
#define ISO_8859_1_E1	KEY_A + ALTGR_MASK			// 225 á     a ACUTE
#define ISO_8859_1_E2	CIRCUMFLEX_BITS + KEY_A			// 226 â     a CIRCUMFLEX
#define ISO_8859_1_E3	TILDE_BITS + KEY_A			// 227 ã     a TILDE
#define ISO_8859_1_E4	DIAERESIS_BITS + KEY_A			// 228 ä     a DIAERESIS
#define ISO_8859_1_E5	KEY_W + ALTGR_MASK			// 229 å     a RING ABOVE
#define ISO_8859_1_E6	KEY_Z + ALTGR_MASK			// 230 æ     ae
#define ISO_8859_1_E7	KEY_COMMA + ALTGR_MASK			// 231 ç     c CEDILLA
#define ISO_8859_1_E8	GRAVE_ACCENT_BITS + KEY_E		// 232 è     e GRAVE
#define ISO_8859_1_E9	ACUTE_ACCENT_BITS + KEY_E		// 233 é     e ACUTE
#define ISO_8859_1_EA	CIRCUMFLEX_BITS + KEY_E			// 234 ê     e CIRCUMFLEX
#define ISO_8859_1_EB	DIAERESIS_BITS + KEY_E			// 235 ë     e DIAERESIS
#define ISO_8859_1_EC	GRAVE_ACCENT_BITS + KEY_I		// 236 ì     i GRAVE
#define ISO_8859_1_ED	KEY_I + ALTGR_MASK			// 237 í     i ACUTE
#define ISO_8859_1_EE	CIRCUMFLEX_BITS + KEY_I			// 238 î     i CIRCUMFLEX
#define ISO_8859_1_EF	DIAERESIS_BITS + KEY_I			// 239 ï     i DIAERESIS
#define ISO_8859_1_F0	KEY_D + ALTGR_MASK			// 240 ð     ETH
#define ISO_8859_1_F1	KEY_N + ALTGR_MASK			// 241 ñ     n TILDE
#define ISO_8859_1_F2	GRAVE_ACCENT_BITS + KEY_O		// 242 ò     o GRAVE
#define ISO_8859_1_F3	KEY_O + ALTGR_MASK			// 243 ó     o ACUTE
#define ISO_8859_1_F4	CIRCUMFLEX_BITS + KEY_O			// 244 ô     o CIRCUMFLEX
#define ISO_8859_1_F5	TILDE_BITS + KEY_O 			// 245 õ     o TILDE
#define ISO_8859_1_F6	KEY_P + ALTGR_MASK			// 246 ö     o DIAERESIS
#define ISO_8859_1_F7	KEY_EQUAL + ALTGR_MASK + SHIFT_MASK	// 247 ÷     DIVISION
#define ISO_8859_1_F8	KEY_L + ALTGR_MASK			// 248 ø     o STROKE
#define ISO_8859_1_F9	GRAVE_ACCENT_BITS + KEY_U		// 249 ù     u GRAVE
#define ISO_8859_1_FA	KEY_U + ALTGR_MASK			// 250 ú     u ACUTE
#define ISO_8859_1_FB	CIRCUMFLEX_BITS + KEY_U			// 251 û     u CIRCUMFLEX
#define ISO_8859_1_FC	KEY_Y + ALTGR_MASK			// 252 ü     u DIAERESIS
#define ISO_8859_1_FD	ACUTE_ACCENT_BITS + KEY_Y		// 253 ý     y ACUTE
#define ISO_8859_1_FE	KEY_T + ALTGR_MASK			// 254 þ     THORN
#define ISO_8859_1_FF	DIAERESIS_BITS + KEY_Y			// 255 ÿ     y DIAERESIS
#define UNICODE_20AC	KEY_5 + ALTGR_MASK			//     €     Euro Sign
// not yet implemented
#define UNICODE_EXTRA00	0x20AC
#define KEYCODE_EXTRA00	KEY_5 + ALTGR_MASK			// 20AC €    Euro Sign

#endif // LAYOUT_US_INTERNATIONAL



#ifdef LAYOUT_GERMAN

#define SHIFT_MASK		0x0040
#define ALTGR_MASK		0x0080
#define DEADKEYS_MASK		0x0700
#define SHIFT_MASK		0x0100
#define ALTGR_MASK		0x0200
#define	CIRCUMFLEX_BITS		0x0300
#define ACUTE_ACCENT_BITS	0x0400
#define GRAVE_ACCENT_BITS	0x0500
#define KEYCODE_TYPE		uint16_t
#define DEADKEY_CIRCUMFLEX	KEY_TILDE
//#define DEADKEY_CIRCUMFLEX	KEY_NON_US_100
#define DEADKEY_ACUTE_ACCENT	KEY_EQUAL
#define DEADKEY_GRAVE_ACCENT	KEY_EQUAL + SHIFT_MASK
#define KEY_NON_US_100		63

#define ASCII_20	KEY_SPACE				// 32  
#define ASCII_21	KEY_1 + SHIFT_MASK			// 33 !
#define ASCII_22	KEY_2 + SHIFT_MASK			// 34 "
#define ASCII_23	KEY_BACKSLASH				// 35 #		??
#define ASCII_24	KEY_4 + SHIFT_MASK			// 36 $
#define ASCII_25	KEY_5 + SHIFT_MASK			// 37 %
#define ASCII_26	KEY_6 + SHIFT_MASK			// 38 &
#define ASCII_27	KEY_BACKSLASH + SHIFT_MASK		// 39 '  
#define ASCII_28	KEY_8 + SHIFT_MASK			// 40 ( 
#define ASCII_29	KEY_9 + SHIFT_MASK			// 41 )
#define ASCII_2A	KEY_RIGHT_BRACE + SHIFT_MASK		// 42 *
#define ASCII_2B	KEY_RIGHT_BRACE				// 43 +
#define ASCII_2C	KEY_COMMA				// 44 ,
#define ASCII_2D	KEY_SLASH				// 45 -
#define ASCII_2E	KEY_PERIOD				// 46 .
#define ASCII_2F	KEY_7 + SHIFT_MASK			// 47 /
#define ASCII_30	KEY_0					// 48 0
#define ASCII_31	KEY_1					// 49 1
#define ASCII_32	KEY_2					// 50 2
#define ASCII_33	KEY_3					// 51 3
#define ASCII_34	KEY_4					// 52 4
#define ASCII_35	KEY_5					// 53 5
#define ASCII_36	KEY_6					// 54 6
#define ASCII_37	KEY_7					// 55 7
#define ASCII_38	KEY_8					// 55 8
#define ASCII_39	KEY_9					// 57 9
#define ASCII_3A	KEY_PERIOD + SHIFT_MASK			// 58 :
#define ASCII_3B	KEY_COMMA + SHIFT_MASK			// 59 ;
#define ASCII_3C	KEY_NON_US_100				// 60 <
#define ASCII_3D	KEY_0 + SHIFT_MASK			// 61 =
#define ASCII_3E	KEY_NON_US_100 + SHIFT_MASK		// 62 >
#define ASCII_3F	KEY_MINUS + SHIFT_MASK			// 63 ?
#define ASCII_40	KEY_Q + ALTGR_MASK			// 64 @
#define ASCII_41	KEY_A + SHIFT_MASK			// 65 A
#define ASCII_42	KEY_B + SHIFT_MASK			// 66 B
#define ASCII_43	KEY_C + SHIFT_MASK			// 67 C
#define ASCII_44	KEY_D + SHIFT_MASK			// 68 D
#define ASCII_45	KEY_E + SHIFT_MASK			// 69 E
#define ASCII_46	KEY_F + SHIFT_MASK			// 70 F
#define ASCII_47	KEY_G + SHIFT_MASK			// 71 G
#define ASCII_48	KEY_H + SHIFT_MASK			// 72 H
#define ASCII_49	KEY_I + SHIFT_MASK			// 73 I
#define ASCII_4A	KEY_J + SHIFT_MASK			// 74 J
#define ASCII_4B	KEY_K + SHIFT_MASK			// 75 K
#define ASCII_4C	KEY_L + SHIFT_MASK			// 76 L
#define ASCII_4D	KEY_M + SHIFT_MASK			// 77 M
#define ASCII_4E	KEY_N + SHIFT_MASK			// 78 N
#define ASCII_4F	KEY_O + SHIFT_MASK			// 79 O
#define ASCII_50	KEY_P + SHIFT_MASK			// 80 P
#define ASCII_51	KEY_Q + SHIFT_MASK			// 81 Q
#define ASCII_52	KEY_R + SHIFT_MASK			// 82 R
#define ASCII_53	KEY_S + SHIFT_MASK			// 83 S
#define ASCII_54	KEY_T + SHIFT_MASK			// 84 T
#define ASCII_55	KEY_U + SHIFT_MASK			// 85 U
#define ASCII_56	KEY_V + SHIFT_MASK			// 86 V
#define ASCII_57	KEY_W + SHIFT_MASK			// 87 W
#define ASCII_58	KEY_X + SHIFT_MASK			// 88 X
#define ASCII_59	KEY_Z + SHIFT_MASK			// 89 Y
#define ASCII_5A	KEY_Y + SHIFT_MASK			// 90 Z
#define ASCII_5B	KEY_8 + ALTGR_MASK			// 91 [
#define ASCII_5C	KEY_MINUS + ALTGR_MASK			// 92 
#define ASCII_5D	KEY_9 + ALTGR_MASK			// 93 ]
#define ASCII_5E	CIRCUMFLEX_BITS + KEY_SPACE		// 94 ^
#define ASCII_5F	KEY_SLASH + SHIFT_MASK			// 95 _
#define ASCII_60	GRAVE_ACCENT_BITS + KEY_SPACE		// 96 `
#define ASCII_61	KEY_A					// 97 a
#define ASCII_62	KEY_B					// 98 b
#define ASCII_63	KEY_C					// 99 c
#define ASCII_64	KEY_D					// 100 d
#define ASCII_65	KEY_E					// 101 e
#define ASCII_66	KEY_F					// 102 f
#define ASCII_67	KEY_G					// 103 g
#define ASCII_68	KEY_H					// 104 h
#define ASCII_69	KEY_I					// 105 i
#define ASCII_6A	KEY_J					// 106 j
#define ASCII_6B	KEY_K					// 107 k
#define ASCII_6C	KEY_L					// 108 l
#define ASCII_6D	KEY_M					// 109 m
#define ASCII_6E	KEY_N					// 110 n
#define ASCII_6F	KEY_O					// 111 o
#define ASCII_70	KEY_P					// 112 p
#define ASCII_71	KEY_Q					// 113 q
#define ASCII_72	KEY_R					// 114 r
#define ASCII_73	KEY_S					// 115 s
#define ASCII_74	KEY_T					// 116 t
#define ASCII_75	KEY_U					// 117 u
#define ASCII_76	KEY_V					// 118 v
#define ASCII_77	KEY_W					// 119 w
#define ASCII_78	KEY_X					// 120 x
#define ASCII_79	KEY_Z					// 121 y
#define ASCII_7A	KEY_Y					// 122 z
#define ASCII_7B	KEY_7 + ALTGR_MASK			// 123 {
#define ASCII_7C	KEY_NON_US_100 + ALTGR_MASK		// 124 |
#define ASCII_7D	KEY_0 + ALTGR_MASK			// 125 }
#define ASCII_7E	KEY_RIGHT_BRACE + ALTGR_MASK		// 126 ~
#define ASCII_7F	KEY_BACKSPACE				// 127
#define ISO_8859_1_A0	KEY_SPACE				// 160       Nonbreakng Space
#define ISO_8859_1_A1	0					// 161 ¡     Inverted Exclamation
#define ISO_8859_1_A2	0					// 162 ¢     Cent Sign
#define ISO_8859_1_A3	0					// 163 £     Pound Sign
#define ISO_8859_1_A4	KEY_E + ALTGR_MASK			// 164 ¤     Currency Sign
#define ISO_8859_1_A5	0					// 165 ¥     YEN SIGN
#define ISO_8859_1_A6	0					// 166 ¦     BROKEN BAR
#define ISO_8859_1_A7	KEY_3 + SHIFT_MASK			// 167 §     SECTION SIGN
#define ISO_8859_1_A8	0					// 168 ¨     DIAERESIS
#define ISO_8859_1_A9	0					// 169 ©     COPYRIGHT SIGN
#define ISO_8859_1_AA	0					// 170 ª     FEMININE ORDINAL
#define ISO_8859_1_AB	0					// 171 «     LEFT DOUBLE ANGLE QUOTE
#define ISO_8859_1_AC	0					// 172 ¬     NOT SIGN
#define ISO_8859_1_AD	0					// 173       SOFT HYPHEN
#define ISO_8859_1_AE	0					// 174 ®     REGISTERED SIGN
#define ISO_8859_1_AF	0					// 175 ¯     MACRON
#define ISO_8859_1_B0	KEY_TILDE + SHIFT_MASK			// 176 °     DEGREE SIGN
#define ISO_8859_1_B1	0					// 177 ±     PLUS-MINUS SIGN
#define ISO_8859_1_B2	KEY_2 + ALTGR_MASK			// 178 ²     SUPERSCRIPT TWO
#define ISO_8859_1_B3	KEY_3 + ALTGR_MASK			// 179 ³     SUPERSCRIPT THREE
#define ISO_8859_1_B4	0					// 180 ´     ACUTE ACCENT
#define ISO_8859_1_B5	KEY_M + ALTGR_MASK			// 181 µ     MICRO SIGN
#define ISO_8859_1_B6	0					// 182 ¶     PILCROW SIGN
#define ISO_8859_1_B7	0					// 183 ·     MIDDLE DOT
#define ISO_8859_1_B8	0					// 184 ¸     CEDILLA
#define ISO_8859_1_B9	0					// 185 ¹     SUPERSCRIPT ONE
#define ISO_8859_1_BA	0					// 186 º     MASCULINE ORDINAL
#define ISO_8859_1_BB	0					// 187 »     RIGHT DOUBLE ANGLE QUOTE
#define ISO_8859_1_BC	0					// 188 ¼     FRACTION ONE QUARTER
#define ISO_8859_1_BD	0					// 189 ½     FRACTION ONE HALF
#define ISO_8859_1_BE	0					// 190 ¾     FRACTION THREE QUARTERS
#define ISO_8859_1_BF	0					// 191 ¿     INVERTED QUESTION MARK
#define ISO_8859_1_C0	GRAVE_ACCENT_BITS + KEY_A + SHIFT_MASK	// 192 À     A GRAVE
#define ISO_8859_1_C1	ACUTE_ACCENT_BITS + KEY_A + SHIFT_MASK	// 193 Á     A ACUTE
#define ISO_8859_1_C2	CIRCUMFLEX_BITS	+ KEY_A + SHIFT_MASK	// 194 Â     A CIRCUMFLEX
#define ISO_8859_1_C3	0					// 195 Ã     A TILDE
#define ISO_8859_1_C4	KEY_QUOTE + SHIFT_MASK			// 196 Ä     A DIAERESIS
#define ISO_8859_1_C5	0					// 197 Å     A RING ABOVE
#define ISO_8859_1_C6	0					// 198 Æ     AE
#define ISO_8859_1_C7	0					// 199 Ç     C CEDILLA
#define ISO_8859_1_C8	GRAVE_ACCENT_BITS + KEY_E + SHIFT_MASK	// 200 È     E GRAVE
#define ISO_8859_1_C9	ACUTE_ACCENT_BITS + KEY_E + SHIFT_MASK	// 201 É     E ACUTE
#define ISO_8859_1_CA	CIRCUMFLEX_BITS + KEY_E + SHIFT_MASK	// 202 Ê     E CIRCUMFLEX
#define ISO_8859_1_CB	0					// 203 Ë     E DIAERESIS
#define ISO_8859_1_CC	GRAVE_ACCENT_BITS + KEY_I + SHIFT_MASK	// 204 Ì     I GRAVE
#define ISO_8859_1_CD	ACUTE_ACCENT_BITS + KEY_I + SHIFT_MASK	// 205 Í     I ACUTE
#define ISO_8859_1_CE	CIRCUMFLEX_BITS + KEY_I + SHIFT_MASK	// 206 Î     I CIRCUMFLEX
#define ISO_8859_1_CF	0					// 207 Ï     I DIAERESIS
#define ISO_8859_1_D0	0					// 208 Ð     ETH
#define ISO_8859_1_D1	0					// 209 Ñ     N TILDE
#define ISO_8859_1_D2	GRAVE_ACCENT_BITS + KEY_O + SHIFT_MASK	// 210 Ò     O GRAVE
#define ISO_8859_1_D3	ACUTE_ACCENT_BITS + KEY_O + SHIFT_MASK	// 211 Ó     O ACUTE
#define ISO_8859_1_D4	CIRCUMFLEX_BITS + KEY_O + SHIFT_MASK	// 212 Ô     O CIRCUMFLEX
#define ISO_8859_1_D5	0					// 213 Õ     O TILDE
#define ISO_8859_1_D6	KEY_SEMICOLON + SHIFT_MASK		// 214 Ö     O DIAERESIS
#define ISO_8859_1_D7	0					// 215 ×     MULTIPLICATION
#define ISO_8859_1_D8	0					// 216 Ø     O STROKE
#define ISO_8859_1_D9	GRAVE_ACCENT_BITS + KEY_U + SHIFT_MASK	// 217 Ù     U GRAVE
#define ISO_8859_1_DA	ACUTE_ACCENT_BITS + KEY_U + SHIFT_MASK	// 218 Ú     U ACUTE
#define ISO_8859_1_DB	CIRCUMFLEX_BITS + KEY_U + SHIFT_MASK	// 219 Û     U CIRCUMFLEX
#define ISO_8859_1_DC	KEY_LEFT_BRACE + SHIFT_MASK		// 220 Ü     U DIAERESIS
#define ISO_8859_1_DD	ACUTE_ACCENT_BITS + KEY_Z + SHIFT_MASK	// 221 Ý     Y ACUTE
#define ISO_8859_1_DE	0					// 222 Þ     THORN
#define ISO_8859_1_DF	KEY_MINUS				// 223 ß     SHARP S
#define ISO_8859_1_E0	GRAVE_ACCENT_BITS + KEY_A		// 224 à     a GRAVE
#define ISO_8859_1_E1	ACUTE_ACCENT_BITS + KEY_A		// 225 á     a ACUTE
#define ISO_8859_1_E2	CIRCUMFLEX_BITS + KEY_A			// 226 â     a CIRCUMFLEX
#define ISO_8859_1_E3	0					// 227 ã     a TILDE
#define ISO_8859_1_E4	KEY_QUOTE				// 228 ä     a DIAERESIS
#define ISO_8859_1_E5	0					// 229 å     a RING ABOVE
#define ISO_8859_1_E6	0					// 230 æ     ae
#define ISO_8859_1_E7	0					// 231 ç     c CEDILLA
#define ISO_8859_1_E8	GRAVE_ACCENT_BITS + KEY_E		// 232 è     e GRAVE
#define ISO_8859_1_E9	ACUTE_ACCENT_BITS + KEY_E		// 233 é     e ACUTE
#define ISO_8859_1_EA	CIRCUMFLEX_BITS + KEY_E			// 234 ê     e CIRCUMFLEX
#define ISO_8859_1_EB	0					// 235 ë     e DIAERESIS
#define ISO_8859_1_EC	GRAVE_ACCENT_BITS + KEY_I		// 236 ì     i GRAVE
#define ISO_8859_1_ED	ACUTE_ACCENT_BITS + KEY_I		// 237 í     i ACUTE
#define ISO_8859_1_EE	CIRCUMFLEX_BITS + KEY_I			// 238 î     i CIRCUMFLEX
#define ISO_8859_1_EF	0					// 239 ï     i DIAERESIS
#define ISO_8859_1_F0	0					// 240 ð     ETH
#define ISO_8859_1_F1	0					// 241 ñ     n TILDE
#define ISO_8859_1_F2	GRAVE_ACCENT_BITS + KEY_O		// 242 ò     o GRAVE
#define ISO_8859_1_F3	ACUTE_ACCENT_BITS + KEY_O		// 243 ó     o ACUTE
#define ISO_8859_1_F4	CIRCUMFLEX_BITS + KEY_O			// 244 ô     o CIRCUMFLEX
#define ISO_8859_1_F5	0					// 245 õ     o TILDE
#define ISO_8859_1_F6	KEY_SEMICOLON				// 246 ö     o DIAERESIS
#define ISO_8859_1_F7	0					// 247 ÷     DIVISION
#define ISO_8859_1_F8	0					// 248 ø     o STROKE
#define ISO_8859_1_F9	GRAVE_ACCENT_BITS + KEY_U		// 249 ù     u GRAVE
#define ISO_8859_1_FA	ACUTE_ACCENT_BITS + KEY_U		// 250 ú     u ACUTE
#define ISO_8859_1_FB	CIRCUMFLEX_BITS + KEY_U			// 251 û     u CIRCUMFLEX
#define ISO_8859_1_FC	KEY_LEFT_BRACE				// 252 ü     u DIAERESIS
#define ISO_8859_1_FD	ACUTE_ACCENT_BITS + KEY_Z		// 253 ý     y ACUTE
#define ISO_8859_1_FE	0					// 254 þ     THORN
#define ISO_8859_1_FF	0					// 255 ÿ     y DIAERESIS
#define UNICODE_20AC	KEY_E + ALTGR_MASK			//     €     Euro Sign
// not yet implemented
#define UNICODE_EXTRA00	0x20AC
#define KEYCODE_EXTRA00	KEY_E + ALTGR_MASK			// 20AC €    Euro Sign

#endif // LAYOUT_GERMAN



#ifdef LAYOUT_GERMAN_MAC

#define SHIFT_MASK		0x0040
#define ALTGR_MASK		0x0080
#define DEADKEYS_MASK		0x0700
#define ACUTE_ACCENT_BITS	0x0100
#define GRAVE_ACCENT_BITS	0x0200
#define	CIRCUMFLEX_BITS		0x0300
#define	DIAERESIS_BITS		0x0400
#define	TILDE_BITS		0x0500
#define KEYCODE_TYPE		uint16_t
#define DEADKEY_ACUTE_ACCENT	KEY_EQUAL
#define DEADKEY_GRAVE_ACCENT	KEY_EQUAL + SHIFT_MASK
#define DEADKEY_CIRCUMFLEX	KEY_6 + SHIFT_MASK + ALTGR_MASK
#define DEADKEY_DIAERESIS	KEY_U + ALTGR_MASK
#define DEADKEY_TILDE		KEY_N + ALTGR_MASK
#define KEY_NON_US_100		63

#define ASCII_20	KEY_SPACE				// 32  
#define ASCII_21	KEY_1 + SHIFT_MASK			// 33 !
#define ASCII_22	KEY_2 + SHIFT_MASK			// 34 "
#define ASCII_23	KEY_BACKSLASH				// 35 #		??
#define ASCII_24	KEY_4 + SHIFT_MASK			// 36 $
#define ASCII_25	KEY_5 + SHIFT_MASK			// 37 %
#define ASCII_26	KEY_6 + SHIFT_MASK			// 38 &
#define ASCII_27	KEY_BACKSLASH + SHIFT_MASK		// 39 '  
#define ASCII_28	KEY_8 + SHIFT_MASK			// 40 ( 
#define ASCII_29	KEY_9 + SHIFT_MASK			// 41 )
#define ASCII_2A	KEY_RIGHT_BRACE + SHIFT_MASK		// 42 *
#define ASCII_2B	KEY_RIGHT_BRACE				// 43 +
#define ASCII_2C	KEY_COMMA				// 44 ,
#define ASCII_2D	KEY_SLASH				// 45 -
#define ASCII_2E	KEY_PERIOD				// 46 .
#define ASCII_2F	KEY_7 + SHIFT_MASK			// 47 /
#define ASCII_30	KEY_0					// 48 0
#define ASCII_31	KEY_1					// 49 1
#define ASCII_32	KEY_2					// 50 2
#define ASCII_33	KEY_3					// 51 3
#define ASCII_34	KEY_4					// 52 4
#define ASCII_35	KEY_5					// 53 5
#define ASCII_36	KEY_6					// 54 6
#define ASCII_37	KEY_7					// 55 7
#define ASCII_38	KEY_8					// 55 8
#define ASCII_39	KEY_9					// 57 9
#define ASCII_3A	KEY_PERIOD + SHIFT_MASK			// 58 :
#define ASCII_3B	KEY_COMMA + SHIFT_MASK			// 59 ;
#define ASCII_3C	KEY_TILDE				// 60 <
#define ASCII_3D	KEY_0 + SHIFT_MASK			// 61 =
#define ASCII_3E	KEY_TILDE + SHIFT_MASK			// 62 >
#define ASCII_3F	KEY_MINUS + SHIFT_MASK			// 63 ?
#define ASCII_40	KEY_Q + ALTGR_MASK			// 64 @
#define ASCII_41	KEY_A + SHIFT_MASK			// 65 A
#define ASCII_42	KEY_B + SHIFT_MASK			// 66 B
#define ASCII_43	KEY_C + SHIFT_MASK			// 67 C
#define ASCII_44	KEY_D + SHIFT_MASK			// 68 D
#define ASCII_45	KEY_E + SHIFT_MASK			// 69 E
#define ASCII_46	KEY_F + SHIFT_MASK			// 70 F
#define ASCII_47	KEY_G + SHIFT_MASK			// 71 G
#define ASCII_48	KEY_H + SHIFT_MASK			// 72 H
#define ASCII_49	KEY_I + SHIFT_MASK			// 73 I
#define ASCII_4A	KEY_J + SHIFT_MASK			// 74 J
#define ASCII_4B	KEY_K + SHIFT_MASK			// 75 K
#define ASCII_4C	KEY_L + SHIFT_MASK			// 76 L
#define ASCII_4D	KEY_M + SHIFT_MASK			// 77 M
#define ASCII_4E	KEY_N + SHIFT_MASK			// 78 N
#define ASCII_4F	KEY_O + SHIFT_MASK			// 79 O
#define ASCII_50	KEY_P + SHIFT_MASK			// 80 P
#define ASCII_51	KEY_Q + SHIFT_MASK			// 81 Q
#define ASCII_52	KEY_R + SHIFT_MASK			// 82 R
#define ASCII_53	KEY_S + SHIFT_MASK			// 83 S
#define ASCII_54	KEY_T + SHIFT_MASK			// 84 T
#define ASCII_55	KEY_U + SHIFT_MASK			// 85 U
#define ASCII_56	KEY_V + SHIFT_MASK			// 86 V
#define ASCII_57	KEY_W + SHIFT_MASK			// 87 W
#define ASCII_58	KEY_X + SHIFT_MASK			// 88 X
#define ASCII_59	KEY_Z + SHIFT_MASK			// 89 Y
#define ASCII_5A	KEY_Y + SHIFT_MASK			// 90 Z
#define ASCII_5B	KEY_5 + ALTGR_MASK			// 91 [
#define ASCII_5C	KEY_7 + ALTGR_MASK + SHIFT_MASK		// 92 
#define ASCII_5D	KEY_6 + ALTGR_MASK			// 93 ]
#define ASCII_5E	CIRCUMFLEX_BITS + KEY_SPACE		// 94 ^
#define ASCII_5F	KEY_SLASH + SHIFT_MASK			// 95 _
#define ASCII_60	GRAVE_ACCENT_BITS + KEY_SPACE		// 96 `
#define ASCII_61	KEY_A					// 97 a
#define ASCII_62	KEY_B					// 98 b
#define ASCII_63	KEY_C					// 99 c
#define ASCII_64	KEY_D					// 100 d
#define ASCII_65	KEY_E					// 101 e
#define ASCII_66	KEY_F					// 102 f
#define ASCII_67	KEY_G					// 103 g
#define ASCII_68	KEY_H					// 104 h
#define ASCII_69	KEY_I					// 105 i
#define ASCII_6A	KEY_J					// 106 j
#define ASCII_6B	KEY_K					// 107 k
#define ASCII_6C	KEY_L					// 108 l
#define ASCII_6D	KEY_M					// 109 m
#define ASCII_6E	KEY_N					// 110 n
#define ASCII_6F	KEY_O					// 111 o
#define ASCII_70	KEY_P					// 112 p
#define ASCII_71	KEY_Q					// 113 q
#define ASCII_72	KEY_R					// 114 r
#define ASCII_73	KEY_S					// 115 s
#define ASCII_74	KEY_T					// 116 t
#define ASCII_75	KEY_U					// 117 u
#define ASCII_76	KEY_V					// 118 v
#define ASCII_77	KEY_W					// 119 w
#define ASCII_78	KEY_X					// 120 x
#define ASCII_79	KEY_Z					// 121 y
#define ASCII_7A	KEY_Y					// 122 z
#define ASCII_7B	KEY_8 + ALTGR_MASK			// 123 {
#define ASCII_7C	KEY_7 + ALTGR_MASK			// 124 |
#define ASCII_7D	KEY_9 + ALTGR_MASK			// 125 }
#define ASCII_7E	TILDE_BITS + KEY_SPACE			// 126 ~
#define ASCII_7F	KEY_BACKSPACE				// 127
#define ISO_8859_1_A0	KEY_SPACE				// 160       Nonbreakng Space
#define ISO_8859_1_A1	KEY_1 + ALTGR_MASK			// 161 ¡     Inverted Exclamation
#define ISO_8859_1_A2	KEY_4 + ALTGR_MASK			// 162 ¢     Cent Sign
#define ISO_8859_1_A3	KEY_4 + SHIFT_MASK + ALTGR_MASK		// 163 £     Pound Sign
#define ISO_8859_1_A4	KEY_E + ALTGR_MASK			// 164 ¤     Currency Sign
#define ISO_8859_1_A5	KEY_Z + ALTGR_MASK			// 165 ¥     YEN SIGN
#define ISO_8859_1_A6	0					// 166 ¦     BROKEN BAR
#define ISO_8859_1_A7	KEY_3 + SHIFT_MASK			// 167 §     SECTION SIGN
#define ISO_8859_1_A8	DIAERESIS_BITS + KEY_SPACE		// 168 ¨     DIAERESIS
#define ISO_8859_1_A9	KEY_G + ALTGR_MASK			// 169 ©     COPYRIGHT SIGN
#define ISO_8859_1_AA	KEY_H + ALTGR_MASK			// 170 ª     FEMININE ORDINAL
#define ISO_8859_1_AB	KEY_Q + ALTGR_MASK			// 171 «     LEFT DOUBLE ANGLE QUOTE
#define ISO_8859_1_AC	0					// 172 ¬     NOT SIGN
#define ISO_8859_1_AD	0					// 173       SOFT HYPHEN
#define ISO_8859_1_AE	KEY_R + ALTGR_MASK			// 174 ®     REGISTERED SIGN
#define ISO_8859_1_AF	KEY_0 + SHIFT_MASK + ALTGR_MASK		// 175 ¯     MACRON
#define ISO_8859_1_B0	KEY_LEFT_BRACE + ALTGR_MASK + SHIFT_MASK// 176 °     DEGREE SIGN
#define ISO_8859_1_B1	KEY_RIGHT_BRACE + ALTGR_MASK		// 177 ±     PLUS-MINUS SIGN
#define ISO_8859_1_B2	0					// 178 ²     SUPERSCRIPT TWO
#define ISO_8859_1_B3	0					// 179 ³     SUPERSCRIPT THREE
#define ISO_8859_1_B4	ACUTE_ACCENT_BITS + KEY_SPACE		// 180 ´     ACUTE ACCENT
#define ISO_8859_1_B5	KEY_M + ALTGR_MASK			// 181 µ     MICRO SIGN
#define ISO_8859_1_B6	KEY_3 + ALTGR_MASK			// 182 ¶     PILCROW SIGN
#define ISO_8859_1_B7	KEY_9 + SHIFT_MASK + ALTGR_MASK		// 183 ·     MIDDLE DOT
#define ISO_8859_1_B8	0					// 184 ¸     CEDILLA
#define ISO_8859_1_B9	0					// 185 ¹     SUPERSCRIPT ONE
#define ISO_8859_1_BA	KEY_J + ALTGR_MASK			// 186 º     MASCULINE ORDINAL
#define ISO_8859_1_BB	KEY_Q + SHIFT_MASK + ALTGR_MASK		// 187 »     RIGHT DOUBLE ANGLE QUOTE
#define ISO_8859_1_BC	0					// 188 ¼     FRACTION ONE QUARTER
#define ISO_8859_1_BD	0					// 189 ½     FRACTION ONE HALF
#define ISO_8859_1_BE	0					// 190 ¾     FRACTION THREE QUARTERS
#define ISO_8859_1_BF	KEY_MINUS + ALTGR_MASK			// 191 ¿     INVERTED QUESTION MARK
#define ISO_8859_1_C0	GRAVE_ACCENT_BITS + KEY_A + SHIFT_MASK	// 192 À     A GRAVE
#define ISO_8859_1_C1	ACUTE_ACCENT_BITS + KEY_A + SHIFT_MASK	// 193 Á     A ACUTE
#define ISO_8859_1_C2	CIRCUMFLEX_BITS	+ KEY_A + SHIFT_MASK	// 194 Â     A CIRCUMFLEX
#define ISO_8859_1_C3	TILDE_BITS + KEY_A + SHIFT_MASK		// 195 Ã     A TILDE
#define ISO_8859_1_C4	KEY_QUOTE + SHIFT_MASK			// 196 Ä     A DIAERESIS
#define ISO_8859_1_C5	KEY_A + ALTGR_MASK + SHIFT_MASK		// 197 Å     A RING ABOVE
#define ISO_8859_1_C6	KEY_QUOTE + ALTGR_MASK + SHIFT_MASK	// 198 Æ     AE
#define ISO_8859_1_C7	KEY_C + ALTGR_MASK + SHIFT_MASK		// 199 Ç     C CEDILLA
#define ISO_8859_1_C8	GRAVE_ACCENT_BITS + KEY_E + SHIFT_MASK	// 200 È     E GRAVE
#define ISO_8859_1_C9	ACUTE_ACCENT_BITS + KEY_E + SHIFT_MASK	// 201 É     E ACUTE
#define ISO_8859_1_CA	CIRCUMFLEX_BITS + KEY_E + SHIFT_MASK	// 202 Ê     E CIRCUMFLEX
#define ISO_8859_1_CB	DIAERESIS_BITS + KEY_E + SHIFT_MASK	// 203 Ë     E DIAERESIS
#define ISO_8859_1_CC	GRAVE_ACCENT_BITS + KEY_I + SHIFT_MASK	// 204 Ì     I GRAVE
#define ISO_8859_1_CD	ACUTE_ACCENT_BITS + KEY_I + SHIFT_MASK	// 205 Í     I ACUTE
#define ISO_8859_1_CE	CIRCUMFLEX_BITS + KEY_I + SHIFT_MASK	// 206 Î     I CIRCUMFLEX
#define ISO_8859_1_CF	DIAERESIS_BITS + KEY_I + SHIFT_MASK	// 207 Ï     I DIAERESIS
#define ISO_8859_1_D0	0					// 208 Ð     ETH
#define ISO_8859_1_D1	TILDE_BITS + KEY_N + SHIFT_MASK		// 209 Ñ     N TILDE
#define ISO_8859_1_D2	GRAVE_ACCENT_BITS + KEY_O + SHIFT_MASK	// 210 Ò     O GRAVE
#define ISO_8859_1_D3	ACUTE_ACCENT_BITS + KEY_O + SHIFT_MASK	// 211 Ó     O ACUTE
#define ISO_8859_1_D4	CIRCUMFLEX_BITS + KEY_O + SHIFT_MASK	// 212 Ô     O CIRCUMFLEX
#define ISO_8859_1_D5	TILDE_BITS + KEY_O + SHIFT_MASK		// 213 Õ     O TILDE
#define ISO_8859_1_D6	KEY_SEMICOLON + SHIFT_MASK		// 214 Ö     O DIAERESIS
#define ISO_8859_1_D7	0					// 215 ×     MULTIPLICATION
#define ISO_8859_1_D8	KEY_O + ALTGR_MASK + SHIFT_MASK		// 216 Ø     O STROKE
#define ISO_8859_1_D9	GRAVE_ACCENT_BITS + KEY_U + SHIFT_MASK	// 217 Ù     U GRAVE
#define ISO_8859_1_DA	ACUTE_ACCENT_BITS + KEY_U + SHIFT_MASK	// 218 Ú     U ACUTE
#define ISO_8859_1_DB	CIRCUMFLEX_BITS + KEY_U + SHIFT_MASK	// 219 Û     U CIRCUMFLEX
#define ISO_8859_1_DC	KEY_LEFT_BRACE + SHIFT_MASK		// 220 Ü     U DIAERESIS
#define ISO_8859_1_DD	ACUTE_ACCENT_BITS + KEY_Z + SHIFT_MASK	// 221 Ý     Y ACUTE
#define ISO_8859_1_DE	0					// 222 Þ     THORN
#define ISO_8859_1_DF	KEY_MINUS				// 223 ß     SHARP S
#define ISO_8859_1_E0	GRAVE_ACCENT_BITS + KEY_A		// 224 à     a GRAVE
#define ISO_8859_1_E1	ACUTE_ACCENT_BITS + KEY_A		// 225 á     a ACUTE
#define ISO_8859_1_E2	CIRCUMFLEX_BITS + KEY_A			// 226 â     a CIRCUMFLEX
#define ISO_8859_1_E3	TILDE_BITS + KEY_A			// 227 ã     a TILDE
#define ISO_8859_1_E4	KEY_QUOTE				// 228 ä     a DIAERESIS
#define ISO_8859_1_E5	KEY_A + ALTGR_MASK			// 229 å     a RING ABOVE
#define ISO_8859_1_E6	KEY_QUOTE + ALTGR_MASK			// 230 æ     ae
#define ISO_8859_1_E7	KEY_C + ALTGR_MASK			// 231 ç     c CEDILLA
#define ISO_8859_1_E8	GRAVE_ACCENT_BITS + KEY_E		// 232 è     e GRAVE
#define ISO_8859_1_E9	ACUTE_ACCENT_BITS + KEY_E		// 233 é     e ACUTE
#define ISO_8859_1_EA	CIRCUMFLEX_BITS + KEY_E			// 234 ê     e CIRCUMFLEX
#define ISO_8859_1_EB	DIAERESIS_BITS + KEY_E			// 235 ë     e DIAERESIS
#define ISO_8859_1_EC	GRAVE_ACCENT_BITS + KEY_I		// 236 ì     i GRAVE
#define ISO_8859_1_ED	ACUTE_ACCENT_BITS + KEY_I		// 237 í     i ACUTE
#define ISO_8859_1_EE	CIRCUMFLEX_BITS + KEY_I			// 238 î     i CIRCUMFLEX
#define ISO_8859_1_EF	DIAERESIS_BITS + KEY_I			// 239 ï     i DIAERESIS
#define ISO_8859_1_F0	0					// 240 ð     ETH
#define ISO_8859_1_F1	TILDE_BITS + KEY_N			// 241 ñ     n TILDE
#define ISO_8859_1_F2	GRAVE_ACCENT_BITS + KEY_O		// 242 ò     o GRAVE
#define ISO_8859_1_F3	ACUTE_ACCENT_BITS + KEY_O		// 243 ó     o ACUTE
#define ISO_8859_1_F4	CIRCUMFLEX_BITS + KEY_O			// 244 ô     o CIRCUMFLEX
#define ISO_8859_1_F5	TILDE_BITS + KEY_O			// 245 õ     o TILDE
#define ISO_8859_1_F6	KEY_SEMICOLON				// 246 ö     o DIAERESIS
#define ISO_8859_1_F7	0					// 247 ÷     DIVISION
#define ISO_8859_1_F8	KEY_O + ALTGR_MASK			// 248 ø     o STROKE
#define ISO_8859_1_F9	GRAVE_ACCENT_BITS + KEY_U		// 249 ù     u GRAVE
#define ISO_8859_1_FA	ACUTE_ACCENT_BITS + KEY_U		// 250 ú     u ACUTE
#define ISO_8859_1_FB	CIRCUMFLEX_BITS + KEY_U			// 251 û     u CIRCUMFLEX
#define ISO_8859_1_FC	KEY_LEFT_BRACE				// 252 ü     u DIAERESIS
#define ISO_8859_1_FD	ACUTE_ACCENT_BITS + KEY_Z		// 253 ý     y ACUTE
#define ISO_8859_1_FE	0					// 254 þ     THORN
#define ISO_8859_1_FF	DIAERESIS_BITS + KEY_Z			// 255 ÿ     y DIAERESIS
#define UNICODE_20AC	KEY_E + ALTGR_MASK			//     €     Euro Sign
// not yet implemented
#define UNICODE_EXTRA00	0x20AC
#define KEYCODE_EXTRA00	KEY_E + ALTGR_MASK			// 20AC €    Euro Sign

#endif // LAYOUT_GERMAN_MAC






#ifdef LAYOUT_CANADIAN_FRENCH

#define SHIFT_MASK		0x0040
#define ALTGR_MASK		0x0080
#define DEADKEYS_MASK		0x0700
#define	CIRCUMFLEX_BITS		0x0100
#define ACUTE_ACCENT_BITS	0x0200
#define GRAVE_ACCENT_BITS	0x0300
#define DIAERESIS_BITS		0x0400
#define CEDILLA_BITS		0x0500

#define KEYCODE_TYPE		uint16_t
#define DEADKEY_CIRCUMFLEX	KEY_LEFT_BRACE
#define DEADKEY_ACUTE_ACCENT	KEY_SLASH + ALTGR_MASK
#define DEADKEY_GRAVE_ACCENT	KEY_QUOTE
#define DEADKEY_DIAERESIS	KEY_RIGHT_BRACE + SHIFT_MASK
#define DEADKEY_CEDILLA		KEY_RIGHT_BRACE
#define KEY_NON_US_100		63

#define ASCII_20	KEY_SPACE				// 32  
#define ASCII_21	KEY_1 + SHIFT_MASK			// 33 !
#define ASCII_22	KEY_2 + SHIFT_MASK			// 34 "
#define ASCII_23	KEY_TILDE + SHIFT_MASK			// 35 #
#define ASCII_24	KEY_4 + SHIFT_MASK			// 36 $
#define ASCII_25	KEY_5 + SHIFT_MASK			// 37 %
#define ASCII_26	KEY_7 + SHIFT_MASK			// 38 &
#define ASCII_27	KEY_COMMA + SHIFT_MASK			// 39 '  
#define ASCII_27	ACUTE_ACCENT_BITS + KEY_SPACE		// 39 '  
#define ASCII_28	KEY_9 + SHIFT_MASK			// 40 ( 
#define ASCII_29	KEY_0 + SHIFT_MASK			// 41 )
#define ASCII_2A	KEY_8 + SHIFT_MASK			// 42 *
#define ASCII_2B	KEY_EQUAL + SHIFT_MASK			// 43 +
#define ASCII_2C	KEY_COMMA				// 44 ,
#define ASCII_2D	KEY_MINUS				// 45 -
#define ASCII_2E	KEY_PERIOD				// 46 .
#define ASCII_2F	KEY_3 + SHIFT_MASK			// 47 /
#define ASCII_30	KEY_0					// 48 0
#define ASCII_31	KEY_1					// 49 1
#define ASCII_32	KEY_2					// 50 2
#define ASCII_33	KEY_3					// 51 3
#define ASCII_34	KEY_4					// 52 4
#define ASCII_35	KEY_5					// 53 5
#define ASCII_36	KEY_6					// 54 6
#define ASCII_37	KEY_7					// 55 7
#define ASCII_38	KEY_8					// 55 8
#define ASCII_39	KEY_9					// 57 9
#define ASCII_3A	KEY_SEMICOLON + SHIFT_MASK		// 58 :
#define ASCII_3B	KEY_SEMICOLON				// 59 ;
#define ASCII_3C	KEY_BACKSLASH				// 60 <
#define ASCII_3D	KEY_EQUAL				// 61 =
#define ASCII_3E	KEY_BACKSLASH + SHIFT_MASK		// 62 >
#define ASCII_3F	KEY_6 + SHIFT_MASK			// 63 ?
#define ASCII_40	KEY_2 + ALTGR_MASK			// 64 @
#define ASCII_41	KEY_A + SHIFT_MASK			// 65 A
#define ASCII_42	KEY_B + SHIFT_MASK			// 66 B
#define ASCII_43	KEY_C + SHIFT_MASK			// 67 C
#define ASCII_44	KEY_D + SHIFT_MASK			// 68 D
#define ASCII_45	KEY_E + SHIFT_MASK			// 69 E
#define ASCII_46	KEY_F + SHIFT_MASK			// 70 F
#define ASCII_47	KEY_G + SHIFT_MASK			// 71 G
#define ASCII_48	KEY_H + SHIFT_MASK			// 72 H
#define ASCII_49	KEY_I + SHIFT_MASK			// 73 I
#define ASCII_4A	KEY_J + SHIFT_MASK			// 74 J
#define ASCII_4B	KEY_K + SHIFT_MASK			// 75 K
#define ASCII_4C	KEY_L + SHIFT_MASK			// 76 L
#define ASCII_4D	KEY_M + SHIFT_MASK			// 77 M
#define ASCII_4E	KEY_N + SHIFT_MASK			// 78 N
#define ASCII_4F	KEY_O + SHIFT_MASK			// 79 O
#define ASCII_50	KEY_P + SHIFT_MASK			// 80 P
#define ASCII_51	KEY_Q + SHIFT_MASK			// 81 Q
#define ASCII_52	KEY_R + SHIFT_MASK			// 82 R
#define ASCII_53	KEY_S + SHIFT_MASK			// 83 S
#define ASCII_54	KEY_T + SHIFT_MASK			// 84 T
#define ASCII_55	KEY_U + SHIFT_MASK			// 85 U
#define ASCII_56	KEY_V + SHIFT_MASK			// 86 V
#define ASCII_57	KEY_W + SHIFT_MASK			// 87 W
#define ASCII_58	KEY_X + SHIFT_MASK			// 88 X
#define ASCII_59	KEY_Y + SHIFT_MASK			// 89 Y
#define ASCII_5A	KEY_Z + SHIFT_MASK			// 90 Z
#define ASCII_5B	KEY_LEFT_BRACE + ALTGR_MASK		// 91 [
#define ASCII_5C	KEY_TILDE + ALTGR_MASK			// 92 
#define ASCII_5D	KEY_RIGHT_BRACE + ALTGR_MASK		// 93 ]
#define ASCII_5E	CIRCUMFLEX_BITS + KEY_SPACE		// 94 ^
#define ASCII_5F	KEY_MINUS + SHIFT_MASK			// 95 _
#define ASCII_60	GRAVE_ACCENT_BITS + KEY_SPACE		// 96 `
#define ASCII_61	KEY_A					// 97 a
#define ASCII_62	KEY_B					// 98 b
#define ASCII_63	KEY_C					// 99 c
#define ASCII_64	KEY_D					// 100 d
#define ASCII_65	KEY_E					// 101 e
#define ASCII_66	KEY_F					// 102 f
#define ASCII_67	KEY_G					// 103 g
#define ASCII_68	KEY_H					// 104 h
#define ASCII_69	KEY_I					// 105 i
#define ASCII_6A	KEY_J					// 106 j
#define ASCII_6B	KEY_K					// 107 k
#define ASCII_6C	KEY_L					// 108 l
#define ASCII_6D	KEY_M					// 109 m
#define ASCII_6E	KEY_N					// 110 n
#define ASCII_6F	KEY_O					// 111 o
#define ASCII_70	KEY_P					// 112 p
#define ASCII_71	KEY_Q					// 113 q
#define ASCII_72	KEY_R					// 114 r
#define ASCII_73	KEY_S					// 115 s
#define ASCII_74	KEY_T					// 116 t
#define ASCII_75	KEY_U					// 117 u
#define ASCII_76	KEY_V					// 118 v
#define ASCII_77	KEY_W					// 119 w
#define ASCII_78	KEY_X					// 120 x
#define ASCII_79	KEY_Y					// 121 y
#define ASCII_7A	KEY_Z					// 122 z
#define ASCII_7B	KEY_QUOTE + ALTGR_MASK			// 123 {
#define ASCII_7C	KEY_TILDE + SHIFT_MASK			// 124 |
#define ASCII_7D	KEY_BACKSLASH + ALTGR_MASK		// 125 }
#define ASCII_7E	KEY_SEMICOLON + ALTGR_MASK		// 126 ~
#define ASCII_7F	KEY_BACKSPACE				// 127
#define ISO_8859_1_A0	KEY_SPACE				// 160       Nonbreakng Space
#define ISO_8859_1_A1	0					// 161 ¡     Inverted Exclamation
#define ISO_8859_1_A2	KEY_4 + ALTGR_MASK			// 162 ¢     Cent SIGN
#define ISO_8859_1_A3	KEY_3 + ALTGR_MASK			// 163 £     Pound Sign
#define ISO_8859_1_A4	KEY_5 + ALTGR_MASK			// 164 ¤     Currency or Euro Sign
#define ISO_8859_1_A5	0					// 165 ¥     YEN SIGN
#define ISO_8859_1_A6	KEY_7 + ALTGR_MASK			// 166 ¦     BROKEN BAR
#define ISO_8859_1_A7	KEY_O + ALTGR_MASK			// 167 §     SECTION SIGN
#define ISO_8859_1_A8	0					// 168 ¨     DIAERESIS
#define ISO_8859_1_A9	0					// 169 ©     COPYRIGHT SIGN
#define ISO_8859_1_AA	0					// 170 ª     FEMININE ORDINAL
#define ISO_8859_1_AB	KEY_NON_US_100				// 171 «     LEFT DOUBLE ANGLE QUOTE
#define ISO_8859_1_AC	KEY_6 + ALTGR_MASK			// 172 ¬     NOT SIGN
#define ISO_8859_1_AD	KEY_PERIOD + ALTGR_MASK			// 173       SOFT HYPHEN
#define ISO_8859_1_AE	0					// 174 ®     REGISTERED SIGN
#define ISO_8859_1_AF	KEY_COMMA + ALTGR_MASK			// 175 ¯     MACRON
#define ISO_8859_1_B0	KEY_NON_US_100 + ALTGR_MASK		// 176 °     DEGREE SIGN
#define ISO_8859_1_B1	KEY_1 + ALTGR_MASK			// 177 ±     PLUS-MINUS SIGN
#define ISO_8859_1_B2	KEY_8 + ALTGR_MASK			// 178 ²     SUPERSCRIPT TWO
#define ISO_8859_1_B3	KEY_9 + ALTGR_MASK			// 179 ³     SUPERSCRIPT THREE
#define ISO_8859_1_B4	0					// 180 ´     ACUTE ACCENT
#define ISO_8859_1_B5	KEY_M + ALTGR_MASK			// 181 µ     MICRO SIGN
#define ISO_8859_1_B6	KEY_P + ALTGR_MASK			// 182 ¶     PILCROW SIGN
#define ISO_8859_1_B7	0					// 183 ·     MIDDLE DOT
#define ISO_8859_1_B8	0					// 184 ¸     CEDILLA
#define ISO_8859_1_B9	0					// 185 ¹     SUPERSCRIPT ONE
#define ISO_8859_1_BA	0					// 186 º     MASCULINE ORDINAL
#define ISO_8859_1_BB	KEY_NON_US_100 + SHIFT_MASK		// 187 »     RIGHT DOUBLE ANGLE QUOTE
#define ISO_8859_1_BC	KEY_0 + ALTGR_MASK			// 188 ¼     FRACTION ONE QUARTER
#define ISO_8859_1_BD	KEY_MINUS + ALTGR_MASK			// 189 ½     FRACTION ONE HALF
#define ISO_8859_1_BE	KEY_EQUAL + ALTGR_MASK			// 190 ¾     FRACTION THREE QUARTERS
#define ISO_8859_1_BF	0					// 191 ¿     INVERTED QUESTION MARK
#define ISO_8859_1_C0	GRAVE_ACCENT_BITS + KEY_A + SHIFT_MASK	// 192 À     A GRAVE
#define ISO_8859_1_C1	ACUTE_ACCENT_BITS + KEY_A + SHIFT_MASK	// 193 Á     A ACUTE
#define ISO_8859_1_C2	CIRCUMFLEX_BITS	+ KEY_A + SHIFT_MASK	// 194 Â     A CIRCUMFLEX
#define ISO_8859_1_C3	0					// 195 Ã     A TILDE
#define ISO_8859_1_C4	DIAERESIS_BITS + KEY_A + SHIFT_MASK	// 196 Ä     A DIAERESIS
#define ISO_8859_1_C5	0					// 197 Å     A RING ABOVE
#define ISO_8859_1_C6	0					// 198 Æ     AE
#define ISO_8859_1_C7	CEDILLA_BITS + KEY_C + SHIFT_MASK	// 199 Ç     C CEDILLA
#define ISO_8859_1_C8	GRAVE_ACCENT_BITS + KEY_E + SHIFT_MASK	// 200 È     E GRAVE
#define ISO_8859_1_C9	KEY_SLASH + SHIFT_MASK			// 201 É     E ACUTE
#define ISO_8859_1_CA	CIRCUMFLEX_BITS + KEY_E + SHIFT_MASK	// 202 Ê     E CIRCUMFLEX
#define ISO_8859_1_CB	DIAERESIS_BITS + KEY_E + SHIFT_MASK	// 203 Ë     E DIAERESIS
#define ISO_8859_1_CC	GRAVE_ACCENT_BITS + KEY_I + SHIFT_MASK	// 204 Ì     I GRAVE
#define ISO_8859_1_CD	ACUTE_ACCENT_BITS + KEY_I + SHIFT_MASK	// 205 Í     I ACUTE
#define ISO_8859_1_CE	CIRCUMFLEX_BITS + KEY_I + SHIFT_MASK	// 206 Î     I CIRCUMFLEX
#define ISO_8859_1_CF	DIAERESIS_BITS + KEY_I + SHIFT_MASK	// 207 Ï     I DIAERESIS
#define ISO_8859_1_D0	0					// 208 Ð     ETH
#define ISO_8859_1_D1	0					// 209 Ñ     N TILDE
#define ISO_8859_1_D2	GRAVE_ACCENT_BITS + KEY_O + SHIFT_MASK	// 210 Ò     O GRAVE
#define ISO_8859_1_D3	ACUTE_ACCENT_BITS + KEY_O + SHIFT_MASK	// 211 Ó     O ACUTE
#define ISO_8859_1_D4	CIRCUMFLEX_BITS + KEY_O + SHIFT_MASK	// 212 Ô     O CIRCUMFLEX
#define ISO_8859_1_D5	0					// 213 Õ     O TILDE
#define ISO_8859_1_D6	DIAERESIS_BITS + KEY_O + SHIFT_MASK	// 214 Ö     O DIAERESIS
#define ISO_8859_1_D7	0					// 215 ×     MULTIPLICATION
#define ISO_8859_1_D8	0					// 216 Ø     O STROKE
#define ISO_8859_1_D9	GRAVE_ACCENT_BITS + KEY_U + SHIFT_MASK	// 217 Ù     U GRAVE
#define ISO_8859_1_DA	ACUTE_ACCENT_BITS + KEY_U + SHIFT_MASK	// 218 Ú     U ACUTE
#define ISO_8859_1_DB	CIRCUMFLEX_BITS + KEY_U + SHIFT_MASK	// 219 Û     U CIRCUMFLEX
#define ISO_8859_1_DC	DIAERESIS_BITS + KEY_U			// 220 Ü     U DIAERESIS
#define ISO_8859_1_DD	ACUTE_ACCENT_BITS + KEY_Y + SHIFT_MASK	// 221 Ý     Y ACUTE
#define ISO_8859_1_DE	0					// 222 Þ     THORN
#define ISO_8859_1_DF	0					// 223 ß     SHARP S
#define ISO_8859_1_E0	GRAVE_ACCENT_BITS + KEY_A		// 224 à     a GRAVE
#define ISO_8859_1_E1	ACUTE_ACCENT_BITS + KEY_A		// 225 á     a ACUTE
#define ISO_8859_1_E2	CIRCUMFLEX_BITS + KEY_A			// 226 â     a CIRCUMFLEX
#define ISO_8859_1_E3	0					// 227 ã     a TILDE
#define ISO_8859_1_E4	DIAERESIS_BITS + KEY_A			// 228 ä     a DIAERESIS
#define ISO_8859_1_E5	0					// 229 å     a RING ABOVE
#define ISO_8859_1_E6	0					// 230 æ     ae
#define ISO_8859_1_E7	CEDILLA_BITS + KEY_C			// 231 ç     c CEDILLA
#define ISO_8859_1_E8	GRAVE_ACCENT_BITS + KEY_E		// 232 è     e GRAVE
#define ISO_8859_1_E9	KEY_SLASH				// 233 é     e ACUTE
#define ISO_8859_1_EA	CIRCUMFLEX_BITS + KEY_E			// 234 ê     e CIRCUMFLEX
#define ISO_8859_1_EB	DIAERESIS_BITS + KEY_E			// 235 ë     e DIAERESIS
#define ISO_8859_1_EC	GRAVE_ACCENT_BITS + KEY_I		// 236 ì     i GRAVE
#define ISO_8859_1_ED	ACUTE_ACCENT_BITS + KEY_I		// 237 í     i ACUTE
#define ISO_8859_1_EE	CIRCUMFLEX_BITS + KEY_I			// 238 î     i CIRCUMFLEX
#define ISO_8859_1_EF	DIAERESIS_BITS + KEY_I			// 239 ï     i DIAERESIS
#define ISO_8859_1_F0	0					// 240 ð     ETH
#define ISO_8859_1_F1	0					// 241 ñ     n TILDE
#define ISO_8859_1_F2	GRAVE_ACCENT_BITS + KEY_O		// 242 ò     o GRAVE
#define ISO_8859_1_F3	ACUTE_ACCENT_BITS + KEY_O		// 243 ó     o ACUTE
#define ISO_8859_1_F4	CIRCUMFLEX_BITS + KEY_O			// 244 ô     o CIRCUMFLEX
#define ISO_8859_1_F5	0					// 245 õ     o TILDE
#define ISO_8859_1_F6	DIAERESIS_BITS + KEY_O			// 246 ö     o DIAERESIS
#define ISO_8859_1_F7	0					// 247 ÷     DIVISION
#define ISO_8859_1_F8	0					// 248 ø     o STROKE
#define ISO_8859_1_F9	GRAVE_ACCENT_BITS + KEY_U		// 249 ù     u GRAVE
#define ISO_8859_1_FA	ACUTE_ACCENT_BITS + KEY_U		// 250 ú     u ACUTE
#define ISO_8859_1_FB	CIRCUMFLEX_BITS + KEY_U			// 251 û     u CIRCUMFLEX
#define ISO_8859_1_FC	DIAERESIS_BITS + KEY_U			// 252 ü     u DIAERESIS
#define ISO_8859_1_FD	ACUTE_ACCENT_BITS + KEY_Y		// 253 ý     y ACUTE
#define ISO_8859_1_FE	0					// 254 þ     THORN
#define ISO_8859_1_FF	DIAERESIS_BITS + KEY_Y			// 255 ÿ     y DIAERESIS

#endif // LAYOUT_CANADIAN_FRENCH



#ifdef LAYOUT_CANADIAN_MULTILINGUAL

#define SHIFT_MASK		0x0040
#define ALTGR_MASK		0x0080
#define RCTRL_MASK		0x0800
#define DEADKEYS_MASK		0x0700
#define	CIRCUMFLEX_BITS		0x0100
#define DIAERESIS_BITS		0x0200
#define ACUTE_ACCENT_BITS	0x0300
#define CEDILLA_BITS		0x0400
#define GRAVE_ACCENT_BITS	0x0500
#define TILDE_BITS		0x0600
#define RING_ABOVE_BITS	 	0x0700
#define KEYCODE_TYPE		uint16_t
#define DEADKEY_CIRCUMFLEX	KEY_LEFT_BRACE
#define DEADKEY_DIAERESIS	KEY_LEFT_BRACE + SHIFT_MASK
#define DEADKEY_ACUTE_ACCENT	KEY_SEMICOLON + RCTRL_MASK
#define DEADKEY_CEDILLA		KEY_EQUAL + RCTRL_MASK
#define DEADKEY_GRAVE_ACCENT	KEY_LEFT_BRACE + ALTGR_MASK
#define DEADKEY_TILDE		KEY_RIGHT_BRACE + ALTGR_MASK
#define DEADKEY_RING_ABOVE	KEY_LEFT_BRACE + SHIFT_MASK + RCTRL_MASK
#define KEY_NON_US_100		63

#define ASCII_20	KEY_SPACE				// 32  
#define ASCII_21	KEY_1 + SHIFT_MASK			// 33 !
#define ASCII_22	KEY_PERIOD + SHIFT_MASK			// 34 "
#define ASCII_23	KEY_3 + SHIFT_MASK			// 35 #
#define ASCII_24	KEY_4 + SHIFT_MASK			// 36 $
#define ASCII_25	KEY_5 + SHIFT_MASK			// 37 %
#define ASCII_26	KEY_7 + SHIFT_MASK			// 38 &
#define ASCII_27	KEY_COMMA + SHIFT_MASK			// 39 '  
#define ASCII_28	KEY_9 + SHIFT_MASK			// 40 ( 
#define ASCII_29	KEY_0 + SHIFT_MASK			// 41 )
#define ASCII_2A	KEY_8 + SHIFT_MASK			// 42 *
#define ASCII_2B	KEY_EQUAL + SHIFT_MASK			// 43 +
#define ASCII_2C	KEY_COMMA				// 44 ,
#define ASCII_2D	KEY_MINUS				// 45 -
#define ASCII_2E	KEY_PERIOD				// 46 .
#define ASCII_2F	KEY_TILDE				// 47 /
#define ASCII_30	KEY_0					// 48 0
#define ASCII_31	KEY_1					// 49 1
#define ASCII_32	KEY_2					// 50 2
#define ASCII_33	KEY_3					// 51 3
#define ASCII_34	KEY_4					// 52 4
#define ASCII_35	KEY_5					// 53 5
#define ASCII_36	KEY_6					// 54 6
#define ASCII_37	KEY_7					// 55 7
#define ASCII_38	KEY_8					// 55 8
#define ASCII_39	KEY_9					// 57 9
#define ASCII_3A	KEY_SEMICOLON + SHIFT_MASK		// 58 :
#define ASCII_3B	KEY_SEMICOLON				// 59 ;
#define ASCII_3C	KEY_COMMA + ALTGR_MASK			// 60 <
#define ASCII_3D	KEY_EQUAL				// 61 =
#define ASCII_3E	KEY_PERIOD + ALTGR_MASK			// 62 >
#define ASCII_3F	KEY_6 + SHIFT_MASK			// 63 ?
#define ASCII_40	KEY_2 + SHIFT_MASK			// 64 @
#define ASCII_41	KEY_A + SHIFT_MASK			// 65 A
#define ASCII_42	KEY_B + SHIFT_MASK			// 66 B
#define ASCII_43	KEY_C + SHIFT_MASK			// 67 C
#define ASCII_44	KEY_D + SHIFT_MASK			// 68 D
#define ASCII_45	KEY_E + SHIFT_MASK			// 69 E
#define ASCII_46	KEY_F + SHIFT_MASK			// 70 F
#define ASCII_47	KEY_G + SHIFT_MASK			// 71 G
#define ASCII_48	KEY_H + SHIFT_MASK			// 72 H
#define ASCII_49	KEY_I + SHIFT_MASK			// 73 I
#define ASCII_4A	KEY_J + SHIFT_MASK			// 74 J
#define ASCII_4B	KEY_K + SHIFT_MASK			// 75 K
#define ASCII_4C	KEY_L + SHIFT_MASK			// 76 L
#define ASCII_4D	KEY_M + SHIFT_MASK			// 77 M
#define ASCII_4E	KEY_N + SHIFT_MASK			// 78 N
#define ASCII_4F	KEY_O + SHIFT_MASK			// 79 O
#define ASCII_50	KEY_P + SHIFT_MASK			// 80 P
#define ASCII_51	KEY_Q + SHIFT_MASK			// 81 Q
#define ASCII_52	KEY_R + SHIFT_MASK			// 82 R
#define ASCII_53	KEY_S + SHIFT_MASK			// 83 S
#define ASCII_54	KEY_T + SHIFT_MASK			// 84 T
#define ASCII_55	KEY_U + SHIFT_MASK			// 85 U
#define ASCII_56	KEY_V + SHIFT_MASK			// 86 V
#define ASCII_57	KEY_W + SHIFT_MASK			// 87 W
#define ASCII_58	KEY_X + SHIFT_MASK			// 88 X
#define ASCII_59	KEY_Y + SHIFT_MASK			// 89 Y
#define ASCII_5A	KEY_Z + SHIFT_MASK			// 90 Z
#define ASCII_5B	KEY_9 + ALTGR_MASK			// 91 [
#define ASCII_5C	KEY_TILDE + SHIFT_MASK			// 92 
#define ASCII_5D	KEY_0 + ALTGR_MASK			// 93 ]
#define ASCII_5E	CIRCUMFLEX_BITS + KEY_SPACE		// 94 ^
#define ASCII_5F	KEY_MINUS + SHIFT_MASK			// 95 _
#define ASCII_60	GRAVE_ACCENT_BITS + KEY_SPACE		// 96 `
#define ASCII_61	KEY_A					// 97 a
#define ASCII_62	KEY_B					// 98 b
#define ASCII_63	KEY_C					// 99 c
#define ASCII_64	KEY_D					// 100 d
#define ASCII_65	KEY_E					// 101 e
#define ASCII_66	KEY_F					// 102 f
#define ASCII_67	KEY_G					// 103 g
#define ASCII_68	KEY_H					// 104 h
#define ASCII_69	KEY_I					// 105 i
#define ASCII_6A	KEY_J					// 106 j
#define ASCII_6B	KEY_K					// 107 k
#define ASCII_6C	KEY_L					// 108 l
#define ASCII_6D	KEY_M					// 109 m
#define ASCII_6E	KEY_N					// 110 n
#define ASCII_6F	KEY_O					// 111 o
#define ASCII_70	KEY_P					// 112 p
#define ASCII_71	KEY_Q					// 113 q
#define ASCII_72	KEY_R					// 114 r
#define ASCII_73	KEY_S					// 115 s
#define ASCII_74	KEY_T					// 116 t
#define ASCII_75	KEY_U					// 117 u
#define ASCII_76	KEY_V					// 118 v
#define ASCII_77	KEY_W					// 119 w
#define ASCII_78	KEY_X					// 120 x
#define ASCII_79	KEY_Y					// 121 y
#define ASCII_7A	KEY_Z					// 122 z
#define ASCII_7B	KEY_7 + ALTGR_MASK			// 123 {
#define ASCII_7C	KEY_TILDE + ALTGR_MASK			// 124 |
#define ASCII_7D	KEY_8 + ALTGR_MASK			// 125 }
#define ASCII_7E	KEY_RIGHT_BRACE + RCTRL_MASK		// 126 ~
#define ASCII_7F	KEY_BACKSPACE				// 127
#define ISO_8859_1_A0	KEY_SPACE + ALTGR_MASK			// 160       Nonbreakng Space
#define ISO_8859_1_A1	KEY_1 + SHIFT_MASK + RCTRL_MASK		// 161 ¡     Inverted Exclamation
#define ISO_8859_1_A2	KEY_C + RCTRL_MASK			// 162 ¢     Cent SIGN
#define ISO_8859_1_A3	KEY_3 + SHIFT_MASK + RCTRL_MASK		// 163 £     Pound Sign
#define ISO_8859_1_A4	KEY_4 + SHIFT_MASK + RCTRL_MASK		// 164 ¤     Currency or Euro Sign
#define ISO_8859_1_A5	KEY_Y + SHIFT_MASK + RCTRL_MASK		// 165 ¥     YEN SIGN
#define ISO_8859_1_A6	KEY_NON_US_100 + SHIFT_MASK + RCTRL_MASK// 166 ¦     BROKEN BAR
#define ISO_8859_1_A7	KEY_S + SHIFT_MASK + RCTRL_MASK		// 167 §     SECTION SIGN
#define ISO_8859_1_A8	DIAERESIS_BITS + KEY_SPACE		// 168 ¨     DIAERESIS
#define ISO_8859_1_A9	KEY_C + SHIFT_MASK + RCTRL_MASK		// 169 ©     COPYRIGHT SIGN
#define ISO_8859_1_AA	KEY_F + SHIFT_MASK + RCTRL_MASK		// 170 ª     FEMININE ORDINAL
#define ISO_8859_1_AB	KEY_Z + ALTGR_MASK			// 171 «     LEFT DOUBLE ANGLE QUOTE
#define ISO_8859_1_AC	KEY_EQUAL + ALTGR_MASK			// 172 ¬     NOT SIGN
#define ISO_8859_1_AD	KEY_TILDE + SHIFT_MASK + RCTRL_MASK	// 173       SOFT HYPHEN
#define ISO_8859_1_AE	KEY_R + SHIFT_MASK + RCTRL_MASK		// 174 ®     REGISTERED SIGN
#define ISO_8859_1_AF	0  // special dead key - no implemented	// 175 ¯     MACRON
#define ISO_8859_1_B0	KEY_SEMICOLON + ALTGR_MASK		// 176 °     DEGREE SIGN
#define ISO_8859_1_B1	KEY_9 + SHIFT_MASK + RCTRL_MASK		// 177 ±     PLUS-MINUS SIGN
#define ISO_8859_1_B2	KEY_2 + RCTRL_MASK			// 178 ²     SUPERSCRIPT TWO
#define ISO_8859_1_B3	KEY_3 + RCTRL_MASK			// 179 ³     SUPERSCRIPT THREE
#define ISO_8859_1_B4	ACUTE_ACCENT_BITS + KEY_SPACE		// 180 ´     ACUTE ACCENT
#define ISO_8859_1_B5	KEY_M + RCTRL_MASK			// 181 µ     MICRO SIGN
#define ISO_8859_1_B6	KEY_R + RCTRL_MASK			// 182 ¶     PILCROW SIGN
#define ISO_8859_1_B7	0					// 183 ·     MIDDLE DOT
#define ISO_8859_1_B8	CEDILLA_BITS + KEY_SPACE		// 184 ¸     CEDILLA
#define ISO_8859_1_B9	KEY_1 + RCTRL_MASK			// 185 ¹     SUPERSCRIPT ONE
#define ISO_8859_1_BA	KEY_M + SHIFT_MASK + RCTRL_MASK		// 186 º     MASCULINE ORDINAL
#define ISO_8859_1_BB	KEY_X + ALTGR_MASK			// 187 »     RIGHT DOUBLE ANGLE QUOTE
#define ISO_8859_1_BC	KEY_4 + RCTRL_MASK			// 188 ¼     FRACTION ONE QUARTER
#define ISO_8859_1_BD	KEY_5 + RCTRL_MASK			// 189 ½     FRACTION ONE HALF
#define ISO_8859_1_BE	KEY_6 + RCTRL_MASK			// 190 ¾     FRACTION THREE QUARTERS
#define ISO_8859_1_BF	KEY_MINUS + SHIFT_MASK + RCTRL_MASK	// 191 ¿     INVERTED QUESTION MARK
#define ISO_8859_1_C0	KEY_BACKSLASH + SHIFT_MASK		// 192 À     A GRAVE
#define ISO_8859_1_C1	ACUTE_ACCENT_BITS + KEY_A + SHIFT_MASK	// 193 Á     A ACUTE
#define ISO_8859_1_C2	CIRCUMFLEX_BITS	+ KEY_A + SHIFT_MASK	// 194 Â     A CIRCUMFLEX
#define ISO_8859_1_C3	TILDE_BITS + KEY_A + SHIFT_MASK		// 195 Ã     A TILDE
#define ISO_8859_1_C4	DIAERESIS_BITS + KEY_A + SHIFT_MASK	// 196 Ä     A DIAERESIS
#define ISO_8859_1_C5	RING_ABOVE_BITS + KEY_A + SHIFT_MASK	// 197 Å     A RING ABOVE
#define ISO_8859_1_C6	KEY_A + SHIFT_MASK + RCTRL_MASK		// 198 Æ     AE
#define ISO_8859_1_C7	KEY_RIGHT_BRACE + SHIFT_MASK		// 199 Ç     C CEDILLA
#define ISO_8859_1_C8	KEY_QUOTE + SHIFT_MASK			// 200 È     E GRAVE
#define ISO_8859_1_C9	KEY_SLASH + SHIFT_MASK			// 201 É     E ACUTE
#define ISO_8859_1_CA	CIRCUMFLEX_BITS + KEY_E + SHIFT_MASK	// 202 Ê     E CIRCUMFLEX
#define ISO_8859_1_CB	DIAERESIS_BITS + KEY_E + SHIFT_MASK	// 203 Ë     E DIAERESIS
#define ISO_8859_1_CC	GRAVE_ACCENT_BITS + KEY_I + SHIFT_MASK	// 204 Ì     I GRAVE
#define ISO_8859_1_CD	ACUTE_ACCENT_BITS + KEY_I + SHIFT_MASK	// 205 Í     I ACUTE
#define ISO_8859_1_CE	CIRCUMFLEX_BITS + KEY_I + SHIFT_MASK	// 206 Î     I CIRCUMFLEX
#define ISO_8859_1_CF	DIAERESIS_BITS + KEY_I + SHIFT_MASK	// 207 Ï     I DIAERESIS
#define ISO_8859_1_D0	KEY_D + SHIFT_MASK + RCTRL_MASK		// 208 Ð     ETH
#define ISO_8859_1_D1	TILDE_BITS + KEY_N + SHIFT_MASK		// 209 Ñ     N TILDE
#define ISO_8859_1_D2	GRAVE_ACCENT_BITS + KEY_O + SHIFT_MASK	// 210 Ò     O GRAVE
#define ISO_8859_1_D3	ACUTE_ACCENT_BITS + KEY_O + SHIFT_MASK	// 211 Ó     O ACUTE
#define ISO_8859_1_D4	CIRCUMFLEX_BITS + KEY_O + SHIFT_MASK	// 212 Ô     O CIRCUMFLEX
#define ISO_8859_1_D5	TILDE_BITS + KEY_O + SHIFT_MASK		// 213 Õ     O TILDE
#define ISO_8859_1_D6	DIAERESIS_BITS + KEY_O + SHIFT_MASK	// 214 Ö     O DIAERESIS
#define ISO_8859_1_D7	KEY_COMMA + SHIFT_MASK + RCTRL_MASK	// 215 ×     MULTIPLICATION
#define ISO_8859_1_D8	KEY_O + SHIFT_MASK + RCTRL_MASK		// 216 Ø     O STROKE
#define ISO_8859_1_D9	KEY_NON_US_100 + SHIFT_MASK		// 217 Ù     U GRAVE
#define ISO_8859_1_DA	ACUTE_ACCENT_BITS + KEY_U + SHIFT_MASK	// 218 Ú     U ACUTE
#define ISO_8859_1_DB	CIRCUMFLEX_BITS + KEY_U + SHIFT_MASK	// 219 Û     U CIRCUMFLEX
#define ISO_8859_1_DC	DIAERESIS_BITS + KEY_U			// 220 Ü     U DIAERESIS
#define ISO_8859_1_DD	ACUTE_ACCENT_BITS + KEY_Y + SHIFT_MASK	// 221 Ý     Y ACUTE
#define ISO_8859_1_DE	KEY_P + RCTRL_MASK + SHIFT_MASK		// 222 Þ     THORN
#define ISO_8859_1_DF	KEY_S + RCTRL_MASK			// 223 ß     SHARP S
#define ISO_8859_1_E0	KEY_BACKSLASH				// 224 à     a GRAVE
#define ISO_8859_1_E1	ACUTE_ACCENT_BITS + KEY_A		// 225 á     a ACUTE
#define ISO_8859_1_E2	CIRCUMFLEX_BITS + KEY_A			// 226 â     a CIRCUMFLEX
#define ISO_8859_1_E3	TILDE_BITS + KEY_A			// 227 ã     a TILDE
#define ISO_8859_1_E4	DIAERESIS_BITS + KEY_A			// 228 ä     a DIAERESIS
#define ISO_8859_1_E5	RING_ABOVE_BITS + KEY_A			// 229 å     a RING ABOVE
#define ISO_8859_1_E6	KEY_A + RCTRL_MASK			// 230 æ     ae
#define ISO_8859_1_E7	KEY_RIGHT_BRACE				// 231 ç     c CEDILLA
#define ISO_8859_1_E8	KEY_QUOTE				// 232 è     e GRAVE
#define ISO_8859_1_E9	KEY_SLASH				// 233 é     e ACUTE
#define ISO_8859_1_EA	CIRCUMFLEX_BITS + KEY_E			// 234 ê     e CIRCUMFLEX
#define ISO_8859_1_EB	DIAERESIS_BITS + KEY_E			// 235 ë     e DIAERESIS
#define ISO_8859_1_EC	GRAVE_ACCENT_BITS + KEY_I		// 236 ì     i GRAVE
#define ISO_8859_1_ED	ACUTE_ACCENT_BITS + KEY_I		// 237 í     i ACUTE
#define ISO_8859_1_EE	CIRCUMFLEX_BITS + KEY_I			// 238 î     i CIRCUMFLEX
#define ISO_8859_1_EF	DIAERESIS_BITS + KEY_I			// 239 ï     i DIAERESIS
#define ISO_8859_1_F0	KEY_D + RCTRL_MASK			// 240 ð     ETH
#define ISO_8859_1_F1	TILDE_BITS + KEY_N			// 241 ñ     n TILDE
#define ISO_8859_1_F2	GRAVE_ACCENT_BITS + KEY_O		// 242 ò     o GRAVE
#define ISO_8859_1_F3	ACUTE_ACCENT_BITS + KEY_O		// 243 ó     o ACUTE
#define ISO_8859_1_F4	CIRCUMFLEX_BITS + KEY_O			// 244 ô     o CIRCUMFLEX
#define ISO_8859_1_F5	TILDE_BITS + KEY_O			// 245 õ     o TILDE
#define ISO_8859_1_F6	DIAERESIS_BITS + KEY_O			// 246 ö     o DIAERESIS
#define ISO_8859_1_F7	KEY_PERIOD + SHIFT_MASK + RCTRL_MASK	// 247 ÷     DIVISION
#define ISO_8859_1_F8	KEY_O + RCTRL_MASK			// 248 ø     o STROKE
#define ISO_8859_1_F9	KEY_NON_US_100				// 249 ù     u GRAVE
#define ISO_8859_1_FA	ACUTE_ACCENT_BITS + KEY_U		// 250 ú     u ACUTE
#define ISO_8859_1_FB	CIRCUMFLEX_BITS + KEY_U			// 251 û     u CIRCUMFLEX
#define ISO_8859_1_FC	DIAERESIS_BITS + KEY_U			// 252 ü     u DIAERESIS
#define ISO_8859_1_FD	ACUTE_ACCENT_BITS + KEY_Y		// 253 ý     y ACUTE
#define ISO_8859_1_FE	KEY_P + RCTRL_MASK			// 254 þ     THORN
#define ISO_8859_1_FF	DIAERESIS_BITS + KEY_Y			// 255 ÿ     y DIAERESIS

#endif // LAYOUT_CANADIAN_MULTILINGUAL






#ifdef LAYOUT_UNITED_KINGDOM

#define SHIFT_MASK		0x0040
#define ALTGR_MASK		0x0080
#define KEYCODE_TYPE		uint8_t
#define KEY_NON_US_100		63

#define ASCII_20	KEY_SPACE				// 32  
#define ASCII_21	KEY_1 + SHIFT_MASK			// 33 !
#define ASCII_22	KEY_2 + SHIFT_MASK			// 34 "
#define ASCII_23	KEY_BACKSPACE				// 35 #
#define ASCII_24	KEY_4 + SHIFT_MASK			// 36 $
#define ASCII_25	KEY_5 + SHIFT_MASK			// 37 %
#define ASCII_26	KEY_7 + SHIFT_MASK			// 38 &
#define ASCII_27	KEY_QUOTE				// 39 '  
#define ASCII_28	KEY_9 + SHIFT_MASK			// 40 ( 
#define ASCII_29	KEY_0 + SHIFT_MASK			// 41 )
#define ASCII_2A	KEY_8 + SHIFT_MASK			// 42 *
#define ASCII_2B	KEY_EQUAL + SHIFT_MASK			// 43 +
#define ASCII_2C	KEY_COMMA				// 44 ,
#define ASCII_2D	KEY_MINUS				// 45 -
#define ASCII_2E	KEY_PERIOD				// 46 .
#define ASCII_2F	KEY_SLASH				// 47 /
#define ASCII_30	KEY_0					// 48 0
#define ASCII_31	KEY_1					// 49 1
#define ASCII_32	KEY_2					// 50 2
#define ASCII_33	KEY_3					// 51 3
#define ASCII_34	KEY_4					// 52 4
#define ASCII_35	KEY_5					// 53 5
#define ASCII_36	KEY_6					// 54 6
#define ASCII_37	KEY_7					// 55 7
#define ASCII_38	KEY_8					// 55 8
#define ASCII_39	KEY_9					// 57 9
#define ASCII_3A	KEY_SEMICOLON + SHIFT_MASK		// 58 :
#define ASCII_3B	KEY_SEMICOLON				// 59 ;
#define ASCII_3C	KEY_COMMA + SHIFT_MASK			// 60 <
#define ASCII_3D	KEY_EQUAL				// 61 =
#define ASCII_3E	KEY_PERIOD + SHIFT_MASK			// 62 >
#define ASCII_3F	KEY_SLASH + SHIFT_MASK			// 63 ?
#define ASCII_40	KEY_QUOTE + SHIFT_MASK			// 64 @
#define ASCII_41	KEY_A + SHIFT_MASK			// 65 A
#define ASCII_42	KEY_B + SHIFT_MASK			// 66 B
#define ASCII_43	KEY_C + SHIFT_MASK			// 67 C
#define ASCII_44	KEY_D + SHIFT_MASK			// 68 D
#define ASCII_45	KEY_E + SHIFT_MASK			// 69 E
#define ASCII_46	KEY_F + SHIFT_MASK			// 70 F
#define ASCII_47	KEY_G + SHIFT_MASK			// 71 G
#define ASCII_48	KEY_H + SHIFT_MASK			// 72 H
#define ASCII_49	KEY_I + SHIFT_MASK			// 73 I
#define ASCII_4A	KEY_J + SHIFT_MASK			// 74 J
#define ASCII_4B	KEY_K + SHIFT_MASK			// 75 K
#define ASCII_4C	KEY_L + SHIFT_MASK			// 76 L
#define ASCII_4D	KEY_M + SHIFT_MASK			// 77 M
#define ASCII_4E	KEY_N + SHIFT_MASK			// 78 N
#define ASCII_4F	KEY_O + SHIFT_MASK			// 79 O
#define ASCII_50	KEY_P + SHIFT_MASK			// 80 P
#define ASCII_51	KEY_Q + SHIFT_MASK			// 81 Q
#define ASCII_52	KEY_R + SHIFT_MASK			// 82 R
#define ASCII_53	KEY_S + SHIFT_MASK			// 83 S
#define ASCII_54	KEY_T + SHIFT_MASK			// 84 T
#define ASCII_55	KEY_U + SHIFT_MASK			// 85 U
#define ASCII_56	KEY_V + SHIFT_MASK			// 86 V
#define ASCII_57	KEY_W + SHIFT_MASK			// 87 W
#define ASCII_58	KEY_X + SHIFT_MASK			// 88 X
#define ASCII_59	KEY_Y + SHIFT_MASK			// 89 Y
#define ASCII_5A	KEY_Z + SHIFT_MASK			// 90 Z
#define ASCII_5B	KEY_LEFT_BRACE				// 91 [
#define ASCII_5C	KEY_NON_US_100				// 92 
#define ASCII_5D	KEY_RIGHT_BRACE				// 93 ]
#define ASCII_5E	KEY_6 + SHIFT_MASK			// 94 ^
#define ASCII_5F	KEY_MINUS + SHIFT_MASK			// 95 _
#define ASCII_60	KEY_TILDE				// 96 `
#define ASCII_61	KEY_A					// 97 a
#define ASCII_62	KEY_B					// 98 b
#define ASCII_63	KEY_C					// 99 c
#define ASCII_64	KEY_D					// 100 d
#define ASCII_65	KEY_E					// 101 e
#define ASCII_66	KEY_F					// 102 f
#define ASCII_67	KEY_G					// 103 g
#define ASCII_68	KEY_H					// 104 h
#define ASCII_69	KEY_I					// 105 i
#define ASCII_6A	KEY_J					// 106 j
#define ASCII_6B	KEY_K					// 107 k
#define ASCII_6C	KEY_L					// 108 l
#define ASCII_6D	KEY_M					// 109 m
#define ASCII_6E	KEY_N					// 110 n
#define ASCII_6F	KEY_O					// 111 o
#define ASCII_70	KEY_P					// 112 p
#define ASCII_71	KEY_Q					// 113 q
#define ASCII_72	KEY_R					// 114 r
#define ASCII_73	KEY_S					// 115 s
#define ASCII_74	KEY_T					// 116 t
#define ASCII_75	KEY_U					// 117 u
#define ASCII_76	KEY_V					// 118 v
#define ASCII_77	KEY_W					// 119 w
#define ASCII_78	KEY_X					// 120 x
#define ASCII_79	KEY_Y					// 121 y
#define ASCII_7A	KEY_Z					// 122 z
#define ASCII_7B	KEY_LEFT_BRACE + SHIFT_MASK		// 123 {
#define ASCII_7C	KEY_NON_US_100 + SHIFT_MASK		// 124 |
#define ASCII_7D	KEY_RIGHT_BRACE + SHIFT_MASK		// 125 }
#define ASCII_7E	KEY_BACKSLASH + SHIFT_MASK		// 126 ~
#define ASCII_7F	KEY_BACKSPACE				// 127

#define ISO_8859_1_A0	KEY_SPACE				// 160       Nonbreakng Space
#define ISO_8859_1_A1	0					// 161 ¡     Inverted Exclamation
#define ISO_8859_1_A2	0					// 162 ¢     Cent SIGN
#define ISO_8859_1_A3	KEY_3 + SHIFT_MASK			// 163 £     Pound Sign
#define ISO_8859_1_A4	0					// 164 ¤     Currency or Euro Sign
#define ISO_8859_1_A5	0					// 165 ¥     YEN SIGN
#define ISO_8859_1_A6	KEY_TILDE + ALTGR_MASK			// 166 ¦     BROKEN BAR
#define ISO_8859_1_A7	0					// 167 §     SECTION SIGN
#define ISO_8859_1_A8	0					// 168 ¨     DIAERESIS
#define ISO_8859_1_A9	0					// 169 ©     COPYRIGHT SIGN
#define ISO_8859_1_AA	0					// 170 ª     FEMININE ORDINAL
#define ISO_8859_1_AB	0					// 171 «     LEFT DOUBLE ANGLE QUOTE
#define ISO_8859_1_AC	KEY_TILDE + SHIFT_MASK			// 172 ¬     NOT SIGN
#define ISO_8859_1_AD	0					// 173       SOFT HYPHEN
#define ISO_8859_1_AE	0					// 174 ®     REGISTERED SIGN
#define ISO_8859_1_AF	0					// 175 ¯     MACRON
#define ISO_8859_1_B0	0					// 176 °     DEGREE SIGN
#define ISO_8859_1_B1	0					// 177 ±     PLUS-MINUS SIGN
#define ISO_8859_1_B2	0					// 178 ²     SUPERSCRIPT TWO
#define ISO_8859_1_B3	0					// 179 ³     SUPERSCRIPT THREE
#define ISO_8859_1_B4	0					// 180 ´     ACUTE ACCENT
#define ISO_8859_1_B5	0					// 181 µ     MICRO SIGN
#define ISO_8859_1_B6	0					// 182 ¶     PILCROW SIGN
#define ISO_8859_1_B7	0					// 183 ·     MIDDLE DOT
#define ISO_8859_1_B8	0					// 184 ¸     CEDILLA
#define ISO_8859_1_B9	0					// 185 ¹     SUPERSCRIPT ONE
#define ISO_8859_1_BA	0					// 186 º     MASCULINE ORDINAL
#define ISO_8859_1_BB	0					// 187 »     RIGHT DOUBLE ANGLE QUOTE
#define ISO_8859_1_BC	0					// 188 ¼     FRACTION ONE QUARTER
#define ISO_8859_1_BD	0					// 189 ½     FRACTION ONE HALF
#define ISO_8859_1_BE	0					// 190 ¾     FRACTION THREE QUARTERS
#define ISO_8859_1_BF	0					// 191 ¿     INVERTED QUESTION MARK
#define ISO_8859_1_C0	0					// 192 À     A GRAVE
#define ISO_8859_1_C1	KEY_A + ALTGR_MASK + SHIFT_MASK		// 193 Á     A ACUTE
#define ISO_8859_1_C2	0					// 194 Â     A CIRCUMFLEX
#define ISO_8859_1_C3	0					// 195 Ã     A TILDE
#define ISO_8859_1_C4	0					// 196 Ä     A DIAERESIS
#define ISO_8859_1_C5	0					// 197 Å     A RING ABOVE
#define ISO_8859_1_C6	0					// 198 Æ     AE
#define ISO_8859_1_C7	0					// 199 Ç     C CEDILLA
#define ISO_8859_1_C8	0					// 200 È     E GRAVE
#define ISO_8859_1_C9	KEY_E + ALTGR_MASK + SHIFT_MASK		// 201 É     E ACUTE
#define ISO_8859_1_CA	0					// 202 Ê     E CIRCUMFLEX
#define ISO_8859_1_CB	0					// 203 Ë     E DIAERESIS
#define ISO_8859_1_CC	0					// 204 Ì     I GRAVE
#define ISO_8859_1_CD	KEY_I + ALTGR_MASK + SHIFT_MASK		// 205 Í     I ACUTE
#define ISO_8859_1_CE	0					// 206 Î     I CIRCUMFLEX
#define ISO_8859_1_CF	0					// 207 Ï     I DIAERESIS
#define ISO_8859_1_D0	0					// 208 Ð     ETH
#define ISO_8859_1_D1	0					// 209 Ñ     N TILDE
#define ISO_8859_1_D2	0					// 210 Ò     O GRAVE
#define ISO_8859_1_D3	KEY_O + ALTGR_MASK + SHIFT_MASK		// 211 Ó     O ACUTE
#define ISO_8859_1_D4	0					// 212 Ô     O CIRCUMFLEX
#define ISO_8859_1_D5	0					// 213 Õ     O TILDE
#define ISO_8859_1_D6	0					// 214 Ö     O DIAERESIS
#define ISO_8859_1_D7	0					// 215 ×     MULTIPLICATION
#define ISO_8859_1_D8	0					// 216 Ø     O STROKE
#define ISO_8859_1_D9	0					// 217 Ù     U GRAVE
#define ISO_8859_1_DA	KEY_U + ALTGR_MASK + SHIFT_MASK		// 218 Ú     U ACUTE
#define ISO_8859_1_DB	0					// 219 Û     U CIRCUMFLEX
#define ISO_8859_1_DC	0					// 220 Ü     U DIAERESIS
#define ISO_8859_1_DD	0					// 221 Ý     Y ACUTE
#define ISO_8859_1_DE	0					// 222 Þ     THORN
#define ISO_8859_1_DF	0					// 223 ß     SHARP S
#define ISO_8859_1_E0	0					// 224 à     a GRAVE
#define ISO_8859_1_E1	KEY_A + ALTGR_MASK			// 225 á     a ACUTE
#define ISO_8859_1_E2	0					// 226 â     a CIRCUMFLEX
#define ISO_8859_1_E3	0					// 227 ã     a TILDE
#define ISO_8859_1_E4	0					// 228 ä     a DIAERESIS
#define ISO_8859_1_E5	0					// 229 å     a RING ABOVE
#define ISO_8859_1_E6	0					// 230 æ     ae
#define ISO_8859_1_E7	0					// 231 ç     c CEDILLA
#define ISO_8859_1_E8	0					// 232 è     e GRAVE
#define ISO_8859_1_E9	KEY_E + ALTGR_MASK			// 233 é     e ACUTE
#define ISO_8859_1_EA	0					// 234 ê     e CIRCUMFLEX
#define ISO_8859_1_EB	0					// 235 ë     e DIAERESIS
#define ISO_8859_1_EC	0					// 236 ì     i GRAVE
#define ISO_8859_1_ED	KEY_I + ALTGR_MASK			// 237 í     i ACUTE
#define ISO_8859_1_EE	0					// 238 î     i CIRCUMFLEX
#define ISO_8859_1_EF	0					// 239 ï     i DIAERESIS
#define ISO_8859_1_F0	0					// 240 ð     ETH
#define ISO_8859_1_F1	0					// 241 ñ     n TILDE
#define ISO_8859_1_F2	0					// 242 ò     o GRAVE
#define ISO_8859_1_F3	KEY_O + ALTGR_MASK			// 243 ó     o ACUTE
#define ISO_8859_1_F4	0					// 244 ô     o CIRCUMFLEX
#define ISO_8859_1_F5	0					// 245 õ     o TILDE
#define ISO_8859_1_F6	0					// 246 ö     o DIAERESIS
#define ISO_8859_1_F7	0					// 247 ÷     DIVISION
#define ISO_8859_1_F8	0					// 248 ø     o STROKE
#define ISO_8859_1_F9	0					// 249 ù     u GRAVE
#define ISO_8859_1_FA	KEY_U + ALTGR_MASK			// 250 ú     u ACUTE
#define ISO_8859_1_FB	0					// 251 û     u CIRCUMFLEX
#define ISO_8859_1_FC	0					// 252 ü     u DIAERESIS
#define ISO_8859_1_FD	0					// 253 ý     y ACUTE
#define ISO_8859_1_FE	0					// 254 þ     THORN
#define ISO_8859_1_FF	0					// 255 ÿ     y DIAERESIS
#define UNICODE_20AC	KEY_4 + ALTGR_MASK			//     €     Euro Sign
// not yet implemented
#define UNICODE_EXTRA00	0x20AC
#define KEYCODE_EXTRA00	KEY_4 + ALTGR_MASK			// 20AC €    Euro Sign

#endif // LAYOUT_UNITED_KINGDOM



#ifdef LAYOUT_FINNISH

#define SHIFT_MASK		0x0040
#define ALTGR_MASK		0x0080
#define DEADKEYS_MASK		0x0700
#define	CIRCUMFLEX_BITS		0x0100
#define ACUTE_ACCENT_BITS	0x0200
#define GRAVE_ACCENT_BITS	0x0300
#define TILDE_BITS		0x0400
#define DIAERESIS_BITS		0x0500
#define KEYCODE_TYPE		uint16_t
#define DEADKEY_CIRCUMFLEX	KEY_RIGHT_BRACE + SHIFT_MASK
#define DEADKEY_ACUTE_ACCENT	KEY_EQUAL
#define DEADKEY_GRAVE_ACCENT	KEY_EQUAL + SHIFT_MASK
#define DEADKEY_TILDE		KEY_RIGHT_BRACE + ALTGR_MASK
#define DEADKEY_DIAERESIS	KEY_RIGHT_BRACE
#define KEY_NON_US_100		63

#define ASCII_20	KEY_SPACE				// 32  
#define ASCII_21	KEY_1 + SHIFT_MASK			// 33 !
#define ASCII_22	KEY_2 + SHIFT_MASK			// 34 "
#define ASCII_23	KEY_3 + SHIFT_MASK			// 35 #
#define ASCII_24	KEY_4 + ALTGR_MASK			// 36 $
#define ASCII_25	KEY_5 + SHIFT_MASK			// 37 %
#define ASCII_26	KEY_6 + SHIFT_MASK			// 38 &
#define ASCII_27	KEY_BACKSLASH				// 39 '  
#define ASCII_28	KEY_8 + SHIFT_MASK			// 40 ( 
#define ASCII_29	KEY_9 + SHIFT_MASK			// 41 )
#define ASCII_2A	KEY_BACKSLASH + SHIFT_MASK		// 42 *
#define ASCII_2B	KEY_MINUS				// 43 +
#define ASCII_2C	KEY_COMMA				// 44 ,
#define ASCII_2D	KEY_SLASH				// 45 -
#define ASCII_2E	KEY_PERIOD				// 46 .
#define ASCII_2F	KEY_7 + SHIFT_MASK			// 47 /
#define ASCII_30	KEY_0					// 48 0
#define ASCII_31	KEY_1					// 49 1
#define ASCII_32	KEY_2					// 50 2
#define ASCII_33	KEY_3					// 51 3
#define ASCII_34	KEY_4					// 52 4
#define ASCII_35	KEY_5					// 53 5
#define ASCII_36	KEY_6					// 54 6
#define ASCII_37	KEY_7					// 55 7
#define ASCII_38	KEY_8					// 55 8
#define ASCII_39	KEY_9					// 57 9
#define ASCII_3A	KEY_PERIOD + SHIFT_MASK			// 58 :
#define ASCII_3B	KEY_COMMA + SHIFT_MASK			// 59 ;
#define ASCII_3C	KEY_NON_US_100				// 60 <
#define ASCII_3D	KEY_0 + SHIFT_MASK			// 61 =
#define ASCII_3E	KEY_NON_US_100 + SHIFT_MASK		// 62 >
#define ASCII_3F	KEY_MINUS + SHIFT_MASK			// 63 ?
#define ASCII_40	KEY_2 + ALTGR_MASK			// 64 @
#define ASCII_41	KEY_A + SHIFT_MASK			// 65 A
#define ASCII_42	KEY_B + SHIFT_MASK			// 66 B
#define ASCII_43	KEY_C + SHIFT_MASK			// 67 C
#define ASCII_44	KEY_D + SHIFT_MASK			// 68 D
#define ASCII_45	KEY_E + SHIFT_MASK			// 69 E
#define ASCII_46	KEY_F + SHIFT_MASK			// 70 F
#define ASCII_47	KEY_G + SHIFT_MASK			// 71 G
#define ASCII_48	KEY_H + SHIFT_MASK			// 72 H
#define ASCII_49	KEY_I + SHIFT_MASK			// 73 I
#define ASCII_4A	KEY_J + SHIFT_MASK			// 74 J
#define ASCII_4B	KEY_K + SHIFT_MASK			// 75 K
#define ASCII_4C	KEY_L + SHIFT_MASK			// 76 L
#define ASCII_4D	KEY_M + SHIFT_MASK			// 77 M
#define ASCII_4E	KEY_N + SHIFT_MASK			// 78 N
#define ASCII_4F	KEY_O + SHIFT_MASK			// 79 O
#define ASCII_50	KEY_P + SHIFT_MASK			// 80 P
#define ASCII_51	KEY_Q + SHIFT_MASK			// 81 Q
#define ASCII_52	KEY_R + SHIFT_MASK			// 82 R
#define ASCII_53	KEY_S + SHIFT_MASK			// 83 S
#define ASCII_54	KEY_T + SHIFT_MASK			// 84 T
#define ASCII_55	KEY_U + SHIFT_MASK			// 85 U
#define ASCII_56	KEY_V + SHIFT_MASK			// 86 V
#define ASCII_57	KEY_W + SHIFT_MASK			// 87 W
#define ASCII_58	KEY_X + SHIFT_MASK			// 88 X
#define ASCII_59	KEY_Y + SHIFT_MASK			// 89 Y
#define ASCII_5A	KEY_Z + SHIFT_MASK			// 90 Z
#define ASCII_5B	KEY_8 + ALTGR_MASK			// 91 [
#define ASCII_5C	KEY_MINUS + ALTGR_MASK			// 92 
#define ASCII_5D	KEY_9 + ALTGR_MASK			// 93 ]
#define ASCII_5E	CIRCUMFLEX_BITS + KEY_SPACE		// 94 ^
#define ASCII_5F	KEY_SLASH + SHIFT_MASK			// 95 _
#define ASCII_60	GRAVE_ACCENT_BITS + KEY_SPACE		// 96 `
#define ASCII_61	KEY_A					// 97 a
#define ASCII_62	KEY_B					// 98 b
#define ASCII_63	KEY_C					// 99 c
#define ASCII_64	KEY_D					// 100 d
#define ASCII_65	KEY_E					// 101 e
#define ASCII_66	KEY_F					// 102 f
#define ASCII_67	KEY_G					// 103 g
#define ASCII_68	KEY_H					// 104 h
#define ASCII_69	KEY_I					// 105 i
#define ASCII_6A	KEY_J					// 106 j
#define ASCII_6B	KEY_K					// 107 k
#define ASCII_6C	KEY_L					// 108 l
#define ASCII_6D	KEY_M					// 109 m
#define ASCII_6E	KEY_N					// 110 n
#define ASCII_6F	KEY_O					// 111 o
#define ASCII_70	KEY_P					// 112 p
#define ASCII_71	KEY_Q					// 113 q
#define ASCII_72	KEY_R					// 114 r
#define ASCII_73	KEY_S					// 115 s
#define ASCII_74	KEY_T					// 116 t
#define ASCII_75	KEY_U					// 117 u
#define ASCII_76	KEY_V					// 118 v
#define ASCII_77	KEY_W					// 119 w
#define ASCII_78	KEY_X					// 120 x
#define ASCII_79	KEY_Y					// 121 y
#define ASCII_7A	KEY_Z					// 122 z
#define ASCII_7B	KEY_7 + ALTGR_MASK			// 123 {
#define ASCII_7C	KEY_NON_US_100 + ALTGR_MASK		// 124 |
#define ASCII_7D	KEY_0 + ALTGR_MASK			// 125 }
#define ASCII_7E	TILDE_BITS + KEY_SPACE			// 126 ~
#define ASCII_7F	KEY_BACKSPACE				// 127
#define ISO_8859_1_A0	KEY_SPACE + ALTGR_MASK			// 160       Nonbreakng Space
#define ISO_8859_1_A1	KEY_1 + SHIFT_MASK + ALTGR_MASK		// 161 ¡     Inverted Exclamation
#define ISO_8859_1_A2	0					// 162 ¢     Cent SIGN
#define ISO_8859_1_A3	KEY_3 + ALTGR_MASK			// 163 £     Pound Sign
#define ISO_8859_1_A4	KEY_4 + SHIFT_MASK			// 164 ¤     Currency or Euro Sign
#define ISO_8859_1_A5	0					// 165 ¥     YEN SIGN
#define ISO_8859_1_A6	0					// 166 ¦     BROKEN BAR
#define ISO_8859_1_A7	KEY_TILDE				// 167 §     SECTION SIGN
#define ISO_8859_1_A8	DIAERESIS_BITS + KEY_SPACE		// 168 ¨     DIAERESIS
#define ISO_8859_1_A9	0					// 169 ©     COPYRIGHT SIGN
#define ISO_8859_1_AA	0					// 170 ª     FEMININE ORDINAL
#define ISO_8859_1_AB	KEY_4 + ALTGR_MASK + SHIFT_MASK		// 171 «     LEFT DOUBLE ANGLE QUOTE
#define ISO_8859_1_AC	0					// 172 ¬     NOT SIGN
#define ISO_8859_1_AD	KEY_SLASH + ALTGR_MASK			// 173       SOFT HYPHEN
#define ISO_8859_1_AE	0					// 174 ®     REGISTERED SIGN
#define ISO_8859_1_AF	0					// 175 ¯     MACRON
#define ISO_8859_1_B0	KEY_0 + ALTGR_MASK + SHIFT_MASK		// 176 °     DEGREE SIGN
#define ISO_8859_1_B1	0					// 177 ±     PLUS-MINUS SIGN
#define ISO_8859_1_B2	0					// 178 ²     SUPERSCRIPT TWO
#define ISO_8859_1_B3	0					// 179 ³     SUPERSCRIPT THREE
#define ISO_8859_1_B4	ACUTE_ACCENT_BITS + KEY_SPACE		// 180 ´     ACUTE ACCENT
#define ISO_8859_1_B5	KEY_M + ALTGR_MASK			// 181 µ     MICRO SIGN
#define ISO_8859_1_B6	0					// 182 ¶     PILCROW SIGN
#define ISO_8859_1_B7	KEY_X + ALTGR_MASK + SHIFT_MASK		// 183 ·     MIDDLE DOT
#define ISO_8859_1_B8	0					// 184 ¸     CEDILLA
#define ISO_8859_1_B9	0					// 185 ¹     SUPERSCRIPT ONE
#define ISO_8859_1_BA	0					// 186 º     MASCULINE ORDINAL
#define ISO_8859_1_BB	KEY_3 + ALTGR_MASK + SHIFT_MASK		// 187 »     RIGHT DOUBLE ANGLE QUOTE
#define ISO_8859_1_BC	0					// 188 ¼     FRACTION ONE QUARTER
#define ISO_8859_1_BD	KEY_TILDE + SHIFT_MASK			// 189 ½     FRACTION ONE HALF
#define ISO_8859_1_BE	0					// 190 ¾     FRACTION THREE QUARTERS
#define ISO_8859_1_BF	KEY_MINUS + ALTGR_MASK + SHIFT_MASK	// 191 ¿     INVERTED QUESTION MARK
#define ISO_8859_1_C0	GRAVE_ACCENT_BITS + KEY_A + SHIFT_MASK	// 192 À     A GRAVE
#define ISO_8859_1_C1	ACUTE_ACCENT_BITS + KEY_A + SHIFT_MASK	// 193 Á     A ACUTE
#define ISO_8859_1_C2	CIRCUMFLEX_BITS + KEY_A + SHIFT_MASK	// 194 Â     A CIRCUMFLEX
#define ISO_8859_1_C3	TILDE_BITS + KEY_A + SHIFT_MASK		// 195 Ã     A TILDE
#define ISO_8859_1_C4	KEY_QUOTE + SHIFT_MASK			// 196 Ä     A DIAERESIS
#define ISO_8859_1_C5	KEY_LEFT_BRACE + SHIFT_MASK		// 197 Å     A RING ABOVE
#define ISO_8859_1_C6	KEY_QUOTE + ALTGR_MASK + SHIFT_MASK	// 198 Æ     AE
#define ISO_8859_1_C7	0					// 199 Ç     C CEDILLA
#define ISO_8859_1_C8	GRAVE_ACCENT_BITS + KEY_E + SHIFT_MASK	// 200 È     E GRAVE
#define ISO_8859_1_C9	ACUTE_ACCENT_BITS + KEY_E + SHIFT_MASK	// 201 É     E ACUTE
#define ISO_8859_1_CA	CIRCUMFLEX_BITS + KEY_E + SHIFT_MASK	// 202 Ê     E CIRCUMFLEX
#define ISO_8859_1_CB	DIAERESIS_BITS + KEY_E + SHIFT_MASK	// 203 Ë     E DIAERESIS
#define ISO_8859_1_CC	GRAVE_ACCENT_BITS + KEY_I + SHIFT_MASK	// 204 Ì     I GRAVE
#define ISO_8859_1_CD	ACUTE_ACCENT_BITS + KEY_I + SHIFT_MASK	// 205 Í     I ACUTE
#define ISO_8859_1_CE	CIRCUMFLEX_BITS + KEY_I + SHIFT_MASK	// 206 Î     I CIRCUMFLEX
#define ISO_8859_1_CF	DIAERESIS_BITS + KEY_I + SHIFT_MASK	// 207 Ï     I DIAERESIS
#define ISO_8859_1_D0	KEY_D + ALTGR_MASK + SHIFT_MASK		// 208 Ð     ETH
#define ISO_8859_1_D1	TILDE_BITS + KEY_N + SHIFT_MASK		// 209 Ñ     N TILDE
#define ISO_8859_1_D2	GRAVE_ACCENT_BITS + KEY_O + SHIFT_MASK	// 210 Ò     O GRAVE
#define ISO_8859_1_D3	ACUTE_ACCENT_BITS + KEY_O + SHIFT_MASK	// 211 Ó     O ACUTE
#define ISO_8859_1_D4	CIRCUMFLEX_BITS + KEY_O + SHIFT_MASK	// 212 Ô     O CIRCUMFLEX
#define ISO_8859_1_D5	TILDE_BITS + KEY_O + SHIFT_MASK		// 213 Õ     O TILDE
#define ISO_8859_1_D6	KEY_SEMICOLON + SHIFT_MASK		// 214 Ö     O DIAERESIS
#define ISO_8859_1_D7	KEY_X + ALTGR_MASK			// 215 ×     MULTIPLICATION
#define ISO_8859_1_D8	KEY_SEMICOLON + ALTGR_MASK + SHIFT_MASK	// 216 Ø     O STROKE
#define ISO_8859_1_D9	GRAVE_ACCENT_BITS + KEY_U + SHIFT_MASK	// 217 Ù     U GRAVE
#define ISO_8859_1_DA	ACUTE_ACCENT_BITS + KEY_U + SHIFT_MASK	// 218 Ú     U ACUTE
#define ISO_8859_1_DB	CIRCUMFLEX_BITS + KEY_U + SHIFT_MASK	// 219 Û     U CIRCUMFLEX
#define ISO_8859_1_DC	DIAERESIS_BITS + KEY_U + SHIFT_MASK	// 220 Ü     U DIAERESIS
#define ISO_8859_1_DD	ACUTE_ACCENT_BITS + KEY_Y + SHIFT_MASK	// 221 Ý     Y ACUTE
#define ISO_8859_1_DE	KEY_T + ALTGR_MASK + SHIFT_MASK		// 222 Þ     THORN
#define ISO_8859_1_DF	KEY_S + ALTGR_MASK			// 223 ß     SHARP S
#define ISO_8859_1_E0	GRAVE_ACCENT_BITS + KEY_A		// 224 à     a GRAVE
#define ISO_8859_1_E1	ACUTE_ACCENT_BITS + KEY_A		// 225 á     a ACUTE
#define ISO_8859_1_E2	CIRCUMFLEX_BITS + KEY_A			// 226 â     a CIRCUMFLEX
#define ISO_8859_1_E3	TILDE_BITS + KEY_A			// 227 ã     a TILDE
#define ISO_8859_1_E4	KEY_QUOTE				// 228 ä     a DIAERESIS
#define ISO_8859_1_E5	KEY_LEFT_BRACE				// 229 å     a RING ABOVE
#define ISO_8859_1_E6	KEY_QUOTE + ALTGR_MASK			// 230 æ     ae
#define ISO_8859_1_E7	0					// 231 ç     c CEDILLA
#define ISO_8859_1_E8	GRAVE_ACCENT_BITS + KEY_E		// 232 è     e GRAVE
#define ISO_8859_1_E9	ACUTE_ACCENT_BITS + KEY_E		// 233 é     e ACUTE
#define ISO_8859_1_EA	CIRCUMFLEX_BITS + KEY_E			// 234 ê     e CIRCUMFLEX
#define ISO_8859_1_EB	DIAERESIS_BITS + KEY_E			// 235 ë     e DIAERESIS
#define ISO_8859_1_EC	GRAVE_ACCENT_BITS + KEY_I		// 236 ì     i GRAVE
#define ISO_8859_1_ED	ACUTE_ACCENT_BITS + KEY_I		// 237 í     i ACUTE
#define ISO_8859_1_EE	CIRCUMFLEX_BITS + KEY_I			// 238 î     i CIRCUMFLEX
#define ISO_8859_1_EF	DIAERESIS_BITS + KEY_I			// 239 ï     i DIAERESIS
#define ISO_8859_1_F0	KEY_D + ALTGR_MASK			// 240 ð     ETH
#define ISO_8859_1_F1	TILDE_BITS + KEY_N			// 241 ñ     n TILDE
#define ISO_8859_1_F2	GRAVE_ACCENT_BITS + KEY_O		// 242 ò     o GRAVE
#define ISO_8859_1_F3	ACUTE_ACCENT_BITS + KEY_O		// 243 ó     o ACUTE
#define ISO_8859_1_F4	CIRCUMFLEX_BITS + KEY_O			// 244 ô     o CIRCUMFLEX
#define ISO_8859_1_F5	TILDE_BITS + KEY_O			// 245 õ     o TILDE
#define ISO_8859_1_F6	KEY_SEMICOLON				// 246 ö     o DIAERESIS
#define ISO_8859_1_F7	0					// 247 ÷     DIVISION
#define ISO_8859_1_F8	KEY_SEMICOLON + ALTGR_MASK		// 248 ø     o STROKE
#define ISO_8859_1_F9	GRAVE_ACCENT_BITS + KEY_U		// 249 ù     u GRAVE
#define ISO_8859_1_FA	ACUTE_ACCENT_BITS + KEY_U		// 250 ú     u ACUTE
#define ISO_8859_1_FB	CIRCUMFLEX_BITS + KEY_U			// 251 û     u CIRCUMFLEX
#define ISO_8859_1_FC	DIAERESIS_BITS + KEY_U			// 252 ü     u DIAERESIS
#define ISO_8859_1_FD	ACUTE_ACCENT_BITS + KEY_Y		// 253 ý     y ACUTE
#define ISO_8859_1_FE	KEY_T + ALTGR_MASK			// 254 þ     THORN
#define ISO_8859_1_FF	DIAERESIS_BITS + KEY_Y			// 255 ÿ     y DIAERESIS
#define UNICODE_20AC	KEY_E + ALTGR_MASK			//     €     Euro Sign
// not yet implemented
#define UNICODE_EXTRA00	0x20AC
#define KEYCODE_EXTRA00	KEY_E + ALTGR_MASK			// 20AC €    Euro Sign
// TODO: Finnish Multilingual layout can type many more glyphs
// but we currently don't have tables tables to store them...

#endif // LAYOUT_FINNISH







#ifdef LAYOUT_FRENCH

#define SHIFT_MASK		0x0040
#define ALTGR_MASK		0x0080
#define DEADKEYS_MASK		0x0700
#define	CIRCUMFLEX_BITS		0x0100
#define GRAVE_ACCENT_BITS	0x0200
#define DIAERESIS_BITS		0x0300
#define TILDE_BITS		0x0400
#define KEYCODE_TYPE		uint16_t
#define DEADKEY_CIRCUMFLEX	KEY_LEFT_BRACE
#define DEADKEY_GRAVE_ACCENT	KEY_7 + ALTGR_MASK
#define DEADKEY_DIAERESIS	KEY_LEFT_BRACE + SHIFT_MASK
#define DEADKEY_TILDE		KEY_2 + ALTGR_MASK
#define KEY_NON_US_100		63

#define ASCII_20	KEY_SPACE				// 32  
#define ASCII_21	KEY_SLASH				// 33 !
#define ASCII_22	KEY_3					// 34 "
#define ASCII_23	KEY_3 + ALTGR_MASK			// 35 #
#define ASCII_24	KEY_RIGHT_BRACE				// 36 $
#define ASCII_25	KEY_QUOTE + SHIFT_MASK			// 37 %
#define ASCII_26	KEY_1					// 38 &
#define ASCII_27	KEY_4					// 39 '  
#define ASCII_28	KEY_5					// 40 ( 
#define ASCII_29	KEY_MINUS				// 41 )
#define ASCII_2A	KEY_BACKSLASH				// 42 *
#define ASCII_2B	KEY_EQUAL + SHIFT_MASK			// 43 +
#define ASCII_2C	KEY_M					// 44 ,
#define ASCII_2D	KEY_6					// 45 -
#define ASCII_2E	KEY_COMMA + SHIFT_MASK			// 46 .
#define ASCII_2F	KEY_PERIOD + SHIFT_MASK			// 47 /
#define ASCII_30	KEY_0 + SHIFT_MASK			// 48 0
#define ASCII_31	KEY_1 + SHIFT_MASK			// 49 1
#define ASCII_32	KEY_2 + SHIFT_MASK			// 50 2
#define ASCII_33	KEY_3 + SHIFT_MASK			// 51 3
#define ASCII_34	KEY_4 + SHIFT_MASK			// 52 4
#define ASCII_35	KEY_5 + SHIFT_MASK			// 53 5
#define ASCII_36	KEY_6 + SHIFT_MASK			// 54 6
#define ASCII_37	KEY_7 + SHIFT_MASK			// 55 7
#define ASCII_38	KEY_8 + SHIFT_MASK			// 55 8
#define ASCII_39	KEY_9 + SHIFT_MASK			// 57 9
#define ASCII_3A	KEY_PERIOD				// 58 :
#define ASCII_3B	KEY_COMMA				// 59 ;
#define ASCII_3C	KEY_NON_US_100				// 60 <
#define ASCII_3D	KEY_EQUAL				// 61 =
#define ASCII_3E	KEY_NON_US_100 + SHIFT_MASK		// 62 >
#define ASCII_3F	KEY_M + SHIFT_MASK			// 63 ?
#define ASCII_40	KEY_0 + ALTGR_MASK			// 64 @
#define ASCII_41	KEY_Q + SHIFT_MASK			// 65 A
#define ASCII_42	KEY_B + SHIFT_MASK			// 66 B
#define ASCII_43	KEY_C + SHIFT_MASK			// 67 C
#define ASCII_44	KEY_D + SHIFT_MASK			// 68 D
#define ASCII_45	KEY_E + SHIFT_MASK			// 69 E
#define ASCII_46	KEY_F + SHIFT_MASK			// 70 F
#define ASCII_47	KEY_G + SHIFT_MASK			// 71 G
#define ASCII_48	KEY_H + SHIFT_MASK			// 72 H
#define ASCII_49	KEY_I + SHIFT_MASK			// 73 I
#define ASCII_4A	KEY_J + SHIFT_MASK			// 74 J
#define ASCII_4B	KEY_K + SHIFT_MASK			// 75 K
#define ASCII_4C	KEY_L + SHIFT_MASK			// 76 L
#define ASCII_4D	KEY_SEMICOLON + SHIFT_MASK		// 77 M
#define ASCII_4E	KEY_N + SHIFT_MASK			// 78 N
#define ASCII_4F	KEY_O + SHIFT_MASK			// 79 O
#define ASCII_50	KEY_P + SHIFT_MASK			// 80 P
#define ASCII_51	KEY_A + SHIFT_MASK			// 81 Q
#define ASCII_52	KEY_R + SHIFT_MASK			// 82 R
#define ASCII_53	KEY_S + SHIFT_MASK			// 83 S
#define ASCII_54	KEY_T + SHIFT_MASK			// 84 T
#define ASCII_55	KEY_U + SHIFT_MASK			// 85 U
#define ASCII_56	KEY_V + SHIFT_MASK			// 86 V
#define ASCII_57	KEY_Z + SHIFT_MASK			// 87 W
#define ASCII_58	KEY_X + SHIFT_MASK			// 88 X
#define ASCII_59	KEY_Y + SHIFT_MASK			// 89 Y
#define ASCII_5A	KEY_W + SHIFT_MASK			// 90 Z
#define ASCII_5B	KEY_5 + ALTGR_MASK			// 91 [
#define ASCII_5C	KEY_8 + ALTGR_MASK			// 92 
#define ASCII_5D	KEY_MINUS + ALTGR_MASK			// 93 ]
#define ASCII_5E	KEY_9 + ALTGR_MASK			// 94 ^
#define ASCII_5F	KEY_8					// 95 _
#define ASCII_60	GRAVE_ACCENT_BITS + KEY_SPACE		// 96 `
#define ASCII_61	KEY_Q					// 97 a
#define ASCII_62	KEY_B					// 98 b
#define ASCII_63	KEY_C					// 99 c
#define ASCII_64	KEY_D					// 100 d
#define ASCII_65	KEY_E					// 101 e
#define ASCII_66	KEY_F					// 102 f
#define ASCII_67	KEY_G					// 103 g
#define ASCII_68	KEY_H					// 104 h
#define ASCII_69	KEY_I					// 105 i
#define ASCII_6A	KEY_J					// 106 j
#define ASCII_6B	KEY_K					// 107 k
#define ASCII_6C	KEY_L					// 108 l
#define ASCII_6D	KEY_SEMICOLON				// 109 m
#define ASCII_6E	KEY_N					// 110 n
#define ASCII_6F	KEY_O					// 111 o
#define ASCII_70	KEY_P					// 112 p
#define ASCII_71	KEY_A					// 113 q
#define ASCII_72	KEY_R					// 114 r
#define ASCII_73	KEY_S					// 115 s
#define ASCII_74	KEY_T					// 116 t
#define ASCII_75	KEY_U					// 117 u
#define ASCII_76	KEY_V					// 118 v
#define ASCII_77	KEY_Z					// 119 w
#define ASCII_78	KEY_X					// 120 x
#define ASCII_79	KEY_Y					// 121 y
#define ASCII_7A	KEY_W					// 122 z
#define ASCII_7B	KEY_4 + ALTGR_MASK			// 123 {
#define ASCII_7C	KEY_6 + ALTGR_MASK			// 124 |
#define ASCII_7D	KEY_EQUAL + ALTGR_MASK			// 125 }
#define ASCII_7E	TILDE_BITS + KEY_SPACE			// 126 ~
#define ASCII_7F	KEY_BACKSPACE				// 127
#define ISO_8859_1_A0	KEY_SPACE				// 160       Nonbreakng Space
#define ISO_8859_1_A1	0					// 161 ¡     Inverted Exclamation
#define ISO_8859_1_A2	0					// 162 ¢     Cent SIGN
#define ISO_8859_1_A3	KEY_RIGHT_BRACE + SHIFT_MASK		// 163 £     Pound Sign
#define ISO_8859_1_A4	KEY_RIGHT_BRACE + ALTGR_MASK		// 164 ¤     Currency or Euro Sign
#define ISO_8859_1_A5	0					// 165 ¥     YEN SIGN
#define ISO_8859_1_A6	0					// 166 ¦     BROKEN BAR
#define ISO_8859_1_A7	KEY_SLASH + SHIFT_MASK			// 167 §     SECTION SIGN
#define ISO_8859_1_A8	DIAERESIS_BITS + KEY_SPACE		// 168 ¨     DIAERESIS
#define ISO_8859_1_A9	0					// 169 ©     COPYRIGHT SIGN
#define ISO_8859_1_AA	0					// 170 ª     FEMININE ORDINAL
#define ISO_8859_1_AB	0					// 171 «     LEFT DOUBLE ANGLE QUOTE
#define ISO_8859_1_AC	0					// 172 ¬     NOT SIGN
#define ISO_8859_1_AD	0					// 173       SOFT HYPHEN
#define ISO_8859_1_AE	0					// 174 ®     REGISTERED SIGN
#define ISO_8859_1_AF	0					// 175 ¯     MACRON
#define ISO_8859_1_B0	KEY_MINUS + SHIFT_MASK			// 176 °     DEGREE SIGN
#define ISO_8859_1_B1	0					// 177 ±     PLUS-MINUS SIGN
#define ISO_8859_1_B2	KEY_TILDE				// 178 ²     SUPERSCRIPT TWO
#define ISO_8859_1_B3	0					// 179 ³     SUPERSCRIPT THREE
#define ISO_8859_1_B4	0					// 180 ´     ACUTE ACCENT
#define ISO_8859_1_B5	KEY_BACKSLASH + SHIFT_MASK		// 181 µ     MICRO SIGN
#define ISO_8859_1_B6	0					// 182 ¶     PILCROW SIGN
#define ISO_8859_1_B7	0					// 183 ·     MIDDLE DOT
#define ISO_8859_1_B8	0					// 184 ¸     CEDILLA
#define ISO_8859_1_B9	0					// 185 ¹     SUPERSCRIPT ONE
#define ISO_8859_1_BA	0					// 186 º     MASCULINE ORDINAL
#define ISO_8859_1_BB	0					// 187 »     RIGHT DOUBLE ANGLE QUOTE
#define ISO_8859_1_BC	0					// 188 ¼     FRACTION ONE QUARTER
#define ISO_8859_1_BD	0					// 189 ½     FRACTION ONE HALF
#define ISO_8859_1_BE	0					// 190 ¾     FRACTION THREE QUARTERS
#define ISO_8859_1_BF	0					// 191 ¿     INVERTED QUESTION MARK
#define ISO_8859_1_C0	GRAVE_ACCENT_BITS + KEY_Q + SHIFT_MASK	// 192 À     A GRAVE
#define ISO_8859_1_C1	0					// 193 Á     A ACUTE
#define ISO_8859_1_C2	CIRCUMFLEX_BITS	+ KEY_Q + SHIFT_MASK	// 194 Â     A CIRCUMFLEX
#define ISO_8859_1_C3	TILDE_BITS + KEY_Q + SHIFT_MASK		// 195 Ã     A TILDE
#define ISO_8859_1_C4	DIAERESIS_BITS + KEY_Q + SHIFT_MASK	// 196 Ä     A DIAERESIS
#define ISO_8859_1_C5	0					// 197 Å     A RING ABOVE
#define ISO_8859_1_C6	0					// 198 Æ     AE
#define ISO_8859_1_C7	0					// 199 Ç     C CEDILLA
#define ISO_8859_1_C8	GRAVE_ACCENT_BITS + KEY_E + SHIFT_MASK	// 200 È     E GRAVE
#define ISO_8859_1_C9	0					// 201 É     E ACUTE
#define ISO_8859_1_CA	CIRCUMFLEX_BITS + KEY_E + SHIFT_MASK	// 202 Ê     E CIRCUMFLEX
#define ISO_8859_1_CB	DIAERESIS_BITS + KEY_E + SHIFT_MASK	// 203 Ë     E DIAERESIS
#define ISO_8859_1_CC	GRAVE_ACCENT_BITS + KEY_I + SHIFT_MASK	// 204 Ì     I GRAVE
#define ISO_8859_1_CD	0					// 205 Í     I ACUTE
#define ISO_8859_1_CE	CIRCUMFLEX_BITS + KEY_I + SHIFT_MASK	// 206 Î     I CIRCUMFLEX
#define ISO_8859_1_CF	DIAERESIS_BITS + KEY_I + SHIFT_MASK	// 207 Ï     I DIAERESIS
#define ISO_8859_1_D0	0					// 208 Ð     ETH
#define ISO_8859_1_D1	TILDE_BITS + KEY_N + SHIFT_MASK		// 209 Ñ     N TILDE
#define ISO_8859_1_D2	GRAVE_ACCENT_BITS + KEY_O + SHIFT_MASK	// 210 Ò     O GRAVE
#define ISO_8859_1_D3	0					// 211 Ó     O ACUTE
#define ISO_8859_1_D4	CIRCUMFLEX_BITS + KEY_O + SHIFT_MASK	// 212 Ô     O CIRCUMFLEX
#define ISO_8859_1_D5	TILDE_BITS + KEY_O + SHIFT_MASK		// 213 Õ     O TILDE
#define ISO_8859_1_D6	DIAERESIS_BITS + KEY_O + SHIFT_MASK	// 214 Ö     O DIAERESIS
#define ISO_8859_1_D7	0					// 215 ×     MULTIPLICATION
#define ISO_8859_1_D8	0					// 216 Ø     O STROKE
#define ISO_8859_1_D9	GRAVE_ACCENT_BITS + KEY_U + SHIFT_MASK	// 217 Ù     U GRAVE
#define ISO_8859_1_DA	0					// 218 Ú     U ACUTE
#define ISO_8859_1_DB	CIRCUMFLEX_BITS + KEY_U + SHIFT_MASK	// 219 Û     U CIRCUMFLEX
#define ISO_8859_1_DC	DIAERESIS_BITS + KEY_U			// 220 Ü     U DIAERESIS
#define ISO_8859_1_DD	0					// 221 Ý     Y ACUTE
#define ISO_8859_1_DE	0					// 222 Þ     THORN
#define ISO_8859_1_DF	0					// 223 ß     SHARP S
#define ISO_8859_1_E0	KEY_0					// 224 à     a GRAVE
#define ISO_8859_1_E1	0					// 225 á     a ACUTE
#define ISO_8859_1_E2	CIRCUMFLEX_BITS + KEY_Q			// 226 â     a CIRCUMFLEX
#define ISO_8859_1_E3	TILDE_BITS + KEY_Q			// 227 ã     a TILDE
#define ISO_8859_1_E4	DIAERESIS_BITS + KEY_Q			// 228 ä     a DIAERESIS
#define ISO_8859_1_E5	0					// 229 å     a RING ABOVE
#define ISO_8859_1_E6	0					// 230 æ     ae
#define ISO_8859_1_E7	KEY_9					// 231 ç     c CEDILLA
#define ISO_8859_1_E8	KEY_7					// 232 è     e GRAVE
#define ISO_8859_1_E9	KEY_2					// 233 é     e ACUTE
#define ISO_8859_1_EA	CIRCUMFLEX_BITS + KEY_E			// 234 ê     e CIRCUMFLEX
#define ISO_8859_1_EB	DIAERESIS_BITS + KEY_E			// 235 ë     e DIAERESIS
#define ISO_8859_1_EC	GRAVE_ACCENT_BITS + KEY_I		// 236 ì     i GRAVE
#define ISO_8859_1_ED	0					// 237 í     i ACUTE
#define ISO_8859_1_EE	CIRCUMFLEX_BITS + KEY_I			// 238 î     i CIRCUMFLEX
#define ISO_8859_1_EF	DIAERESIS_BITS + KEY_I			// 239 ï     i DIAERESIS
#define ISO_8859_1_F0	0					// 240 ð     ETH
#define ISO_8859_1_F1	TILDE_BITS + KEY_N			// 241 ñ     n TILDE
#define ISO_8859_1_F2	GRAVE_ACCENT_BITS + KEY_O		// 242 ò     o GRAVE
#define ISO_8859_1_F3	0					// 243 ó     o ACUTE
#define ISO_8859_1_F4	CIRCUMFLEX_BITS + KEY_O			// 244 ô     o CIRCUMFLEX
#define ISO_8859_1_F5	TILDE_BITS + KEY_O			// 245 õ     o TILDE
#define ISO_8859_1_F6	DIAERESIS_BITS + KEY_O			// 246 ö     o DIAERESIS
#define ISO_8859_1_F7	0					// 247 ÷     DIVISION
#define ISO_8859_1_F8	0					// 248 ø     o STROKE
#define ISO_8859_1_F9	KEY_SEMICOLON				// 249 ù     u GRAVE
#define ISO_8859_1_FA	0					// 250 ú     u ACUTE
#define ISO_8859_1_FB	CIRCUMFLEX_BITS + KEY_U			// 251 û     u CIRCUMFLEX
#define ISO_8859_1_FC	DIAERESIS_BITS + KEY_U			// 252 ü     u DIAERESIS
#define ISO_8859_1_FD	0					// 253 ý     y ACUTE
#define ISO_8859_1_FE	0					// 254 þ     THORN
#define ISO_8859_1_FF	DIAERESIS_BITS + KEY_Y			// 255 ÿ     y DIAERESIS
#define UNICODE_20AC	KEY_E + ALTGR_MASK			//     €     Euro Sign
// not yet implemented
#define UNICODE_EXTRA00	0x20AC
#define KEYCODE_EXTRA00	KEY_E + ALTGR_MASK			// 20AC €    Euro Sign

#endif // LAYOUT_FRENCH





#ifdef LAYOUT_DANISH

#define SHIFT_MASK		0x0040
#define ALTGR_MASK		0x0080
#define DEADKEYS_MASK		0x0700
#define	CIRCUMFLEX_BITS		0x0100
#define ACUTE_ACCENT_BITS	0x0200
#define GRAVE_ACCENT_BITS	0x0300
#define TILDE_BITS		0x0400
#define DIAERESIS_BITS		0x0500
#define KEYCODE_TYPE		uint16_t
#define DEADKEY_CIRCUMFLEX	KEY_RIGHT_BRACE + SHIFT_MASK
#define DEADKEY_ACUTE_ACCENT	KEY_EQUAL
#define DEADKEY_GRAVE_ACCENT	KEY_EQUAL + SHIFT_MASK
#define DEADKEY_TILDE		KEY_RIGHT_BRACE + ALTGR_MASK
#define DEADKEY_DIAERESIS	KEY_RIGHT_BRACE
#define KEY_NON_US_100		63

#define ASCII_20	KEY_SPACE				// 32  
#define ASCII_21	KEY_1 + SHIFT_MASK			// 33 !
#define ASCII_22	KEY_2 + SHIFT_MASK			// 34 "
#define ASCII_23	KEY_3 + SHIFT_MASK			// 35 #
#define ASCII_24	KEY_4 + ALTGR_MASK			// 36 $
#define ASCII_25	KEY_5 + SHIFT_MASK			// 37 %
#define ASCII_26	KEY_6 + SHIFT_MASK			// 38 &
#define ASCII_27	KEY_BACKSLASH				// 39 '  
#define ASCII_28	KEY_8 + SHIFT_MASK			// 40 ( 
#define ASCII_29	KEY_9 + SHIFT_MASK			// 41 )
#define ASCII_2A	KEY_BACKSLASH + SHIFT_MASK		// 42 *
#define ASCII_2B	KEY_MINUS				// 43 +
#define ASCII_2C	KEY_COMMA				// 44 ,
#define ASCII_2D	KEY_SLASH				// 45 -
#define ASCII_2E	KEY_PERIOD				// 46 .
#define ASCII_2F	KEY_7 + SHIFT_MASK			// 47 /
#define ASCII_30	KEY_0					// 48 0
#define ASCII_31	KEY_1					// 49 1
#define ASCII_32	KEY_2					// 50 2
#define ASCII_33	KEY_3					// 51 3
#define ASCII_34	KEY_4					// 52 4
#define ASCII_35	KEY_5					// 53 5
#define ASCII_36	KEY_6					// 54 6
#define ASCII_37	KEY_7					// 55 7
#define ASCII_38	KEY_8					// 55 8
#define ASCII_39	KEY_9					// 57 9
#define ASCII_3A	KEY_PERIOD + SHIFT_MASK			// 58 :
#define ASCII_3B	KEY_COMMA + SHIFT_MASK			// 59 ;
#define ASCII_3C	KEY_NON_US_100				// 60 <
#define ASCII_3D	KEY_0 + SHIFT_MASK			// 61 =
#define ASCII_3E	KEY_NON_US_100 + SHIFT_MASK		// 62 >
#define ASCII_3F	KEY_MINUS + SHIFT_MASK			// 63 ?
#define ASCII_40	KEY_2 + ALTGR_MASK			// 64 @
#define ASCII_41	KEY_A + SHIFT_MASK			// 65 A
#define ASCII_42	KEY_B + SHIFT_MASK			// 66 B
#define ASCII_43	KEY_C + SHIFT_MASK			// 67 C
#define ASCII_44	KEY_D + SHIFT_MASK			// 68 D
#define ASCII_45	KEY_E + SHIFT_MASK			// 69 E
#define ASCII_46	KEY_F + SHIFT_MASK			// 70 F
#define ASCII_47	KEY_G + SHIFT_MASK			// 71 G
#define ASCII_48	KEY_H + SHIFT_MASK			// 72 H
#define ASCII_49	KEY_I + SHIFT_MASK			// 73 I
#define ASCII_4A	KEY_J + SHIFT_MASK			// 74 J
#define ASCII_4B	KEY_K + SHIFT_MASK			// 75 K
#define ASCII_4C	KEY_L + SHIFT_MASK			// 76 L
#define ASCII_4D	KEY_M + SHIFT_MASK			// 77 M
#define ASCII_4E	KEY_N + SHIFT_MASK			// 78 N
#define ASCII_4F	KEY_O + SHIFT_MASK			// 79 O
#define ASCII_50	KEY_P + SHIFT_MASK			// 80 P
#define ASCII_51	KEY_Q + SHIFT_MASK			// 81 Q
#define ASCII_52	KEY_R + SHIFT_MASK			// 82 R
#define ASCII_53	KEY_S + SHIFT_MASK			// 83 S
#define ASCII_54	KEY_T + SHIFT_MASK			// 84 T
#define ASCII_55	KEY_U + SHIFT_MASK			// 85 U
#define ASCII_56	KEY_V + SHIFT_MASK			// 86 V
#define ASCII_57	KEY_W + SHIFT_MASK			// 87 W
#define ASCII_58	KEY_X + SHIFT_MASK			// 88 X
#define ASCII_59	KEY_Y + SHIFT_MASK			// 89 Y
#define ASCII_5A	KEY_Z + SHIFT_MASK			// 90 Z
#define ASCII_5B	KEY_8 + ALTGR_MASK			// 91 [
#define ASCII_5C	KEY_NON_US_100 + ALTGR_MASK		// 92 
#define ASCII_5D	KEY_9 + ALTGR_MASK			// 93 ]
#define ASCII_5E	CIRCUMFLEX_BITS + KEY_SPACE		// 94 ^
#define ASCII_5F	KEY_SLASH + SHIFT_MASK			// 95 _
#define ASCII_60	GRAVE_ACCENT_BITS + KEY_SPACE		// 96 `
#define ASCII_61	KEY_A					// 97 a
#define ASCII_62	KEY_B					// 98 b
#define ASCII_63	KEY_C					// 99 c
#define ASCII_64	KEY_D					// 100 d
#define ASCII_65	KEY_E					// 101 e
#define ASCII_66	KEY_F					// 102 f
#define ASCII_67	KEY_G					// 103 g
#define ASCII_68	KEY_H					// 104 h
#define ASCII_69	KEY_I					// 105 i
#define ASCII_6A	KEY_J					// 106 j
#define ASCII_6B	KEY_K					// 107 k
#define ASCII_6C	KEY_L					// 108 l
#define ASCII_6D	KEY_M					// 109 m
#define ASCII_6E	KEY_N					// 110 n
#define ASCII_6F	KEY_O					// 111 o
#define ASCII_70	KEY_P					// 112 p
#define ASCII_71	KEY_Q					// 113 q
#define ASCII_72	KEY_R					// 114 r
#define ASCII_73	KEY_S					// 115 s
#define ASCII_74	KEY_T					// 116 t
#define ASCII_75	KEY_U					// 117 u
#define ASCII_76	KEY_V					// 118 v
#define ASCII_77	KEY_W					// 119 w
#define ASCII_78	KEY_X					// 120 x
#define ASCII_79	KEY_Y					// 121 y
#define ASCII_7A	KEY_Z					// 122 z
#define ASCII_7B	KEY_7 + ALTGR_MASK			// 123 {
#define ASCII_7C	KEY_EQUAL + ALTGR_MASK			// 124 |
#define ASCII_7D	KEY_0 + ALTGR_MASK			// 125 }
#define ASCII_7E	TILDE_BITS + KEY_SPACE			// 126 ~
#define ASCII_7F	KEY_BACKSPACE				// 127

#define ISO_8859_1_A0	KEY_SPACE				// 160       Nonbreakng Space
#define ISO_8859_1_A1	0					// 161 ¡     Inverted Exclamation
#define ISO_8859_1_A2	0					// 162 ¢     Cent SIGN
#define ISO_8859_1_A3	KEY_3 + ALTGR_MASK			// 163 £     Pound Sign
#define ISO_8859_1_A4	KEY_4 + SHIFT_MASK			// 164 ¤     Currency Sign
#define ISO_8859_1_A5	0					// 165 ¥     YEN SIGN
#define ISO_8859_1_A6	0					// 166 ¦     BROKEN BAR
#define ISO_8859_1_A7	KEY_TILDE + SHIFT_MASK			// 167 §     SECTION SIGN
#define ISO_8859_1_A8	DIAERESIS_BITS + KEY_SPACE		// 168 ¨     DIAERESIS
#define ISO_8859_1_A9	0					// 169 ©     COPYRIGHT SIGN
#define ISO_8859_1_AA	0					// 170 ª     FEMININE ORDINAL
#define ISO_8859_1_AB	KEY_4					// 171 «     LEFT DOUBLE ANGLE QUOTE
#define ISO_8859_1_AC	0					// 172 ¬     NOT SIGN
#define ISO_8859_1_AD	0					// 173       SOFT HYPHEN
#define ISO_8859_1_AE	0					// 174 ®     REGISTERED SIGN
#define ISO_8859_1_AF	0					// 175 ¯     MACRON
#define ISO_8859_1_B0	0					// 176 °     DEGREE SIGN
#define ISO_8859_1_B1	0					// 177 ±     PLUS-MINUS SIGN
#define ISO_8859_1_B2	0					// 178 ²     SUPERSCRIPT TWO
#define ISO_8859_1_B3	0					// 179 ³     SUPERSCRIPT THREE
#define ISO_8859_1_B4	ACUTE_ACCENT_BITS + KEY_SPACE		// 180 ´     ACUTE ACCENT
#define ISO_8859_1_B5	KEY_M + ALTGR_MASK			// 181 µ     MICRO SIGN
#define ISO_8859_1_B6	0					// 182 ¶     PILCROW SIGN
#define ISO_8859_1_B7	0					// 183 ·     MIDDLE DOT
#define ISO_8859_1_B8	0					// 184 ¸     CEDILLA
#define ISO_8859_1_B9	0					// 185 ¹     SUPERSCRIPT ONE
#define ISO_8859_1_BA	0					// 186 º     MASCULINE ORDINAL
#define ISO_8859_1_BB	0					// 187 »     RIGHT DOUBLE ANGLE QUOTE
#define ISO_8859_1_BC	0					// 188 ¼     FRACTION ONE QUARTER
#define ISO_8859_1_BD	KEY_TILDE				// 189 ½     FRACTION ONE HALF
#define ISO_8859_1_BE	0					// 190 ¾     FRACTION THREE QUARTERS
#define ISO_8859_1_BF	0					// 191 ¿     INVERTED QUESTION MARK
#define ISO_8859_1_C0	GRAVE_ACCENT_BITS + KEY_A + SHIFT_MASK	// 192 À     A GRAVE
#define ISO_8859_1_C1	ACUTE_ACCENT_BITS + KEY_A + SHIFT_MASK	// 193 Á     A ACUTE
#define ISO_8859_1_C2	CIRCUMFLEX_BITS + KEY_A + SHIFT_MASK	// 194 Â     A CIRCUMFLEX
#define ISO_8859_1_C3	TILDE_BITS + KEY_A + SHIFT_MASK		// 195 Ã     A TILDE
#define ISO_8859_1_C4	DIAERESIS_BITS + KEY_A + SHIFT_MASK	// 196 Ä     A DIAERESIS
#define ISO_8859_1_C5	KEY_LEFT_BRACE + SHIFT_MASK		// 197 Å     A RING ABOVE
#define ISO_8859_1_C6	KEY_SEMICOLON + SHIFT_MASK		// 198 Æ     AE
#define ISO_8859_1_C7	0					// 199 Ç     C CEDILLA
#define ISO_8859_1_C8	GRAVE_ACCENT_BITS + KEY_E + SHIFT_MASK	// 200 È     E GRAVE
#define ISO_8859_1_C9	ACUTE_ACCENT_BITS + KEY_E + SHIFT_MASK	// 201 É     E ACUTE
#define ISO_8859_1_CA	CIRCUMFLEX_BITS + KEY_E + SHIFT_MASK	// 202 Ê     E CIRCUMFLEX
#define ISO_8859_1_CB	DIAERESIS_BITS + KEY_E + SHIFT_MASK	// 203 Ë     E DIAERESIS
#define ISO_8859_1_CC	GRAVE_ACCENT_BITS + KEY_I + SHIFT_MASK	// 204 Ì     I GRAVE
#define ISO_8859_1_CD	ACUTE_ACCENT_BITS + KEY_I + SHIFT_MASK	// 205 Í     I ACUTE
#define ISO_8859_1_CE	CIRCUMFLEX_BITS + KEY_I + SHIFT_MASK	// 206 Î     I CIRCUMFLEX
#define ISO_8859_1_CF	DIAERESIS_BITS + KEY_I + SHIFT_MASK	// 207 Ï     I DIAERESIS
#define ISO_8859_1_D0	KEY_D + ALTGR_MASK + SHIFT_MASK		// 208 Ð     ETH
#define ISO_8859_1_D1	TILDE_BITS + KEY_N + SHIFT_MASK		// 209 Ñ     N TILDE
#define ISO_8859_1_D2	GRAVE_ACCENT_BITS + KEY_O + SHIFT_MASK	// 210 Ò     O GRAVE
#define ISO_8859_1_D3	ACUTE_ACCENT_BITS + KEY_O + SHIFT_MASK	// 211 Ó     O ACUTE
#define ISO_8859_1_D4	CIRCUMFLEX_BITS + KEY_O + SHIFT_MASK	// 212 Ô     O CIRCUMFLEX
#define ISO_8859_1_D5	TILDE_BITS + KEY_O + SHIFT_MASK		// 213 Õ     O TILDE
#define ISO_8859_1_D6	DIAERESIS_BITS + KEY_O + SHIFT_MASK	// 214 Ö     O DIAERESIS
#define ISO_8859_1_D7	0					// 215 ×     MULTIPLICATION
#define ISO_8859_1_D8	KEY_QUOTE + SHIFT_MASK			// 216 Ø     O STROKE
#define ISO_8859_1_D9	GRAVE_ACCENT_BITS + KEY_U + SHIFT_MASK	// 217 Ù     U GRAVE
#define ISO_8859_1_DA	ACUTE_ACCENT_BITS + KEY_U + SHIFT_MASK	// 218 Ú     U ACUTE
#define ISO_8859_1_DB	CIRCUMFLEX_BITS + KEY_U + SHIFT_MASK	// 219 Û     U CIRCUMFLEX
#define ISO_8859_1_DC	DIAERESIS_BITS + KEY_U + SHIFT_MASK	// 220 Ü     U DIAERESIS
#define ISO_8859_1_DD	ACUTE_ACCENT_BITS + KEY_Y + SHIFT_MASK	// 221 Ý     Y ACUTE
#define ISO_8859_1_DE	KEY_T + ALTGR_MASK + SHIFT_MASK		// 222 Þ     THORN
#define ISO_8859_1_DF	KEY_S + ALTGR_MASK			// 223 ß     SHARP S
#define ISO_8859_1_E0	GRAVE_ACCENT_BITS + KEY_A		// 224 à     a GRAVE
#define ISO_8859_1_E1	ACUTE_ACCENT_BITS + KEY_A		// 225 á     a ACUTE
#define ISO_8859_1_E2	CIRCUMFLEX_BITS + KEY_A			// 226 â     a CIRCUMFLEX
#define ISO_8859_1_E3	TILDE_BITS + KEY_A			// 227 ã     a TILDE
#define ISO_8859_1_E4	DIAERESIS_BITS + KEY_A			// 228 ä     a DIAERESIS
#define ISO_8859_1_E5	KEY_LEFT_BRACE				// 229 å     a RING ABOVE
#define ISO_8859_1_E6	KEY_SEMICOLON				// 230 æ     ae
#define ISO_8859_1_E7	0					// 231 ç     c CEDILLA
#define ISO_8859_1_E8	GRAVE_ACCENT_BITS + KEY_E		// 232 è     e GRAVE
#define ISO_8859_1_E9	ACUTE_ACCENT_BITS + KEY_E		// 233 é     e ACUTE
#define ISO_8859_1_EA	CIRCUMFLEX_BITS + KEY_E			// 234 ê     e CIRCUMFLEX
#define ISO_8859_1_EB	DIAERESIS_BITS + KEY_E			// 235 ë     e DIAERESIS
#define ISO_8859_1_EC	GRAVE_ACCENT_BITS + KEY_I		// 236 ì     i GRAVE
#define ISO_8859_1_ED	ACUTE_ACCENT_BITS + KEY_I		// 237 í     i ACUTE
#define ISO_8859_1_EE	CIRCUMFLEX_BITS + KEY_I			// 238 î     i CIRCUMFLEX
#define ISO_8859_1_EF	DIAERESIS_BITS + KEY_I			// 239 ï     i DIAERESIS
#define ISO_8859_1_F0	KEY_D + ALTGR_MASK			// 240 ð     ETH
#define ISO_8859_1_F1	TILDE_BITS + KEY_N			// 241 ñ     n TILDE
#define ISO_8859_1_F2	GRAVE_ACCENT_BITS + KEY_O		// 242 ò     o GRAVE
#define ISO_8859_1_F3	ACUTE_ACCENT_BITS + KEY_O		// 243 ó     o ACUTE
#define ISO_8859_1_F4	CIRCUMFLEX_BITS + KEY_O			// 244 ô     o CIRCUMFLEX
#define ISO_8859_1_F5	TILDE_BITS + KEY_O			// 245 õ     o TILDE
#define ISO_8859_1_F6	DIAERESIS_BITS + KEY_O			// 246 ö     o DIAERESIS
#define ISO_8859_1_F7	0					// 247 ÷     DIVISION
#define ISO_8859_1_F8	KEY_QUOTE				// 248 ø     o STROKE
#define ISO_8859_1_F9	GRAVE_ACCENT_BITS + KEY_U		// 249 ù     u GRAVE
#define ISO_8859_1_FA	ACUTE_ACCENT_BITS + KEY_U		// 250 ú     u ACUTE
#define ISO_8859_1_FB	CIRCUMFLEX_BITS + KEY_U			// 251 û     u CIRCUMFLEX
#define ISO_8859_1_FC	DIAERESIS_BITS + KEY_U			// 252 ü     u DIAERESIS
#define ISO_8859_1_FD	ACUTE_ACCENT_BITS + KEY_Y		// 253 ý     y ACUTE
#define ISO_8859_1_FE	KEY_T + ALTGR_MASK			// 254 þ     THORN
#define ISO_8859_1_FF	DIAERESIS_BITS + KEY_Y			// 255 ÿ     y DIAERESIS
#define UNICODE_20AC	KEY_E + ALTGR_MASK			//     €     Euro Sign
// not yet implemented
#define UNICODE_EXTRA00	0x20AC
#define KEYCODE_EXTRA00	KEY_E + ALTGR_MASK			// 20AC €    Euro Sign

#endif // LAYOUT_DANISH





#ifdef LAYOUT_NORWEGIAN

#define SHIFT_MASK		0x0040
#define ALTGR_MASK		0x0080
#define DEADKEYS_MASK		0x0700
#define	CIRCUMFLEX_BITS		0x0100
#define ACUTE_ACCENT_BITS	0x0200
#define GRAVE_ACCENT_BITS	0x0300
#define TILDE_BITS		0x0400
#define DIAERESIS_BITS		0x0500
#define KEYCODE_TYPE		uint16_t
#define DEADKEY_CIRCUMFLEX	KEY_RIGHT_BRACE + SHIFT_MASK
#define DEADKEY_ACUTE_ACCENT	KEY_EQUAL + ALTGR_MASK
#define DEADKEY_GRAVE_ACCENT	KEY_EQUAL + SHIFT_MASK
#define DEADKEY_TILDE		KEY_RIGHT_BRACE + ALTGR_MASK
#define DEADKEY_DIAERESIS	KEY_RIGHT_BRACE
#define KEY_NON_US_100		63

#define ASCII_20	KEY_SPACE				// 32  
#define ASCII_21	KEY_1 + SHIFT_MASK			// 33 !
#define ASCII_22	KEY_2 + SHIFT_MASK			// 34 "
#define ASCII_23	KEY_3 + SHIFT_MASK			// 35 #
#define ASCII_24	KEY_4 + ALTGR_MASK			// 36 $
#define ASCII_25	KEY_5 + SHIFT_MASK			// 37 %
#define ASCII_26	KEY_6 + SHIFT_MASK			// 38 &
#define ASCII_27	KEY_BACKSLASH				// 39 '  
#define ASCII_28	KEY_8 + SHIFT_MASK			// 40 ( 
#define ASCII_29	KEY_9 + SHIFT_MASK			// 41 )
#define ASCII_2A	KEY_BACKSLASH + SHIFT_MASK		// 42 *
#define ASCII_2B	KEY_MINUS				// 43 +
#define ASCII_2C	KEY_COMMA				// 44 ,
#define ASCII_2D	KEY_SLASH				// 45 -
#define ASCII_2E	KEY_PERIOD				// 46 .
#define ASCII_2F	KEY_7 + SHIFT_MASK			// 47 /
#define ASCII_30	KEY_0					// 48 0
#define ASCII_31	KEY_1					// 49 1
#define ASCII_32	KEY_2					// 50 2
#define ASCII_33	KEY_3					// 51 3
#define ASCII_34	KEY_4					// 52 4
#define ASCII_35	KEY_5					// 53 5
#define ASCII_36	KEY_6					// 54 6
#define ASCII_37	KEY_7					// 55 7
#define ASCII_38	KEY_8					// 55 8
#define ASCII_39	KEY_9					// 57 9
#define ASCII_3A	KEY_PERIOD + SHIFT_MASK			// 58 :
#define ASCII_3B	KEY_COMMA + SHIFT_MASK			// 59 ;
#define ASCII_3C	KEY_NON_US_100				// 60 <
#define ASCII_3D	KEY_0 + SHIFT_MASK			// 61 =
#define ASCII_3E	KEY_NON_US_100 + SHIFT_MASK		// 62 >
#define ASCII_3F	KEY_MINUS + SHIFT_MASK			// 63 ?
#define ASCII_40	KEY_2 + ALTGR_MASK			// 64 @
#define ASCII_41	KEY_A + SHIFT_MASK			// 65 A
#define ASCII_42	KEY_B + SHIFT_MASK			// 66 B
#define ASCII_43	KEY_C + SHIFT_MASK			// 67 C
#define ASCII_44	KEY_D + SHIFT_MASK			// 68 D
#define ASCII_45	KEY_E + SHIFT_MASK			// 69 E
#define ASCII_46	KEY_F + SHIFT_MASK			// 70 F
#define ASCII_47	KEY_G + SHIFT_MASK			// 71 G
#define ASCII_48	KEY_H + SHIFT_MASK			// 72 H
#define ASCII_49	KEY_I + SHIFT_MASK			// 73 I
#define ASCII_4A	KEY_J + SHIFT_MASK			// 74 J
#define ASCII_4B	KEY_K + SHIFT_MASK			// 75 K
#define ASCII_4C	KEY_L + SHIFT_MASK			// 76 L
#define ASCII_4D	KEY_M + SHIFT_MASK			// 77 M
#define ASCII_4E	KEY_N + SHIFT_MASK			// 78 N
#define ASCII_4F	KEY_O + SHIFT_MASK			// 79 O
#define ASCII_50	KEY_P + SHIFT_MASK			// 80 P
#define ASCII_51	KEY_Q + SHIFT_MASK			// 81 Q
#define ASCII_52	KEY_R + SHIFT_MASK			// 82 R
#define ASCII_53	KEY_S + SHIFT_MASK			// 83 S
#define ASCII_54	KEY_T + SHIFT_MASK			// 84 T
#define ASCII_55	KEY_U + SHIFT_MASK			// 85 U
#define ASCII_56	KEY_V + SHIFT_MASK			// 86 V
#define ASCII_57	KEY_W + SHIFT_MASK			// 87 W
#define ASCII_58	KEY_X + SHIFT_MASK			// 88 X
#define ASCII_59	KEY_Y + SHIFT_MASK			// 89 Y
#define ASCII_5A	KEY_Z + SHIFT_MASK			// 90 Z
#define ASCII_5B	KEY_8 + ALTGR_MASK			// 91 [
#define ASCII_5C	KEY_EQUAL				// 92 
#define ASCII_5D	KEY_9 + ALTGR_MASK			// 93 ]
#define ASCII_5E	CIRCUMFLEX_BITS + KEY_SPACE		// 94 ^
#define ASCII_5F	KEY_SLASH + SHIFT_MASK			// 95 _
#define ASCII_60	GRAVE_ACCENT_BITS + KEY_SPACE		// 96 `
#define ASCII_61	KEY_A					// 97 a
#define ASCII_62	KEY_B					// 98 b
#define ASCII_63	KEY_C					// 99 c
#define ASCII_64	KEY_D					// 100 d
#define ASCII_65	KEY_E					// 101 e
#define ASCII_66	KEY_F					// 102 f
#define ASCII_67	KEY_G					// 103 g
#define ASCII_68	KEY_H					// 104 h
#define ASCII_69	KEY_I					// 105 i
#define ASCII_6A	KEY_J					// 106 j
#define ASCII_6B	KEY_K					// 107 k
#define ASCII_6C	KEY_L					// 108 l
#define ASCII_6D	KEY_M					// 109 m
#define ASCII_6E	KEY_N					// 110 n
#define ASCII_6F	KEY_O					// 111 o
#define ASCII_70	KEY_P					// 112 p
#define ASCII_71	KEY_Q					// 113 q
#define ASCII_72	KEY_R					// 114 r
#define ASCII_73	KEY_S					// 115 s
#define ASCII_74	KEY_T					// 116 t
#define ASCII_75	KEY_U					// 117 u
#define ASCII_76	KEY_V					// 118 v
#define ASCII_77	KEY_W					// 119 w
#define ASCII_78	KEY_X					// 120 x
#define ASCII_79	KEY_Y					// 121 y
#define ASCII_7A	KEY_Z					// 122 z
#define ASCII_7B	KEY_7 + ALTGR_MASK			// 123 {
#define ASCII_7C	KEY_TILDE				// 124 |
#define ASCII_7D	KEY_0 + ALTGR_MASK			// 125 }
#define ASCII_7E	TILDE_BITS + KEY_SPACE			// 126 ~
#define ASCII_7F	KEY_BACKSPACE				// 127

#define ISO_8859_1_A0	KEY_SPACE				// 160       Nonbreakng Space
#define ISO_8859_1_A1	0					// 161 ¡     Inverted Exclamation
#define ISO_8859_1_A2	0					// 162 ¢     Cent SIGN
#define ISO_8859_1_A3	KEY_3 + ALTGR_MASK			// 163 £     Pound Sign
#define ISO_8859_1_A4	KEY_4 + SHIFT_MASK			// 164 ¤     Currency Sign
#define ISO_8859_1_A5	0					// 165 ¥     YEN SIGN
#define ISO_8859_1_A6	0					// 166 ¦     BROKEN BAR
#define ISO_8859_1_A7	KEY_TILDE + SHIFT_MASK			// 167 §     SECTION SIGN
#define ISO_8859_1_A8	DIAERESIS_BITS + KEY_SPACE		// 168 ¨     DIAERESIS
#define ISO_8859_1_A9	0					// 169 ©     COPYRIGHT SIGN
#define ISO_8859_1_AA	0					// 170 ª     FEMININE ORDINAL
#define ISO_8859_1_AB	KEY_4					// 171 «     LEFT DOUBLE ANGLE QUOTE
#define ISO_8859_1_AC	0					// 172 ¬     NOT SIGN
#define ISO_8859_1_AD	0					// 173       SOFT HYPHEN
#define ISO_8859_1_AE	0					// 174 ®     REGISTERED SIGN
#define ISO_8859_1_AF	0					// 175 ¯     MACRON
#define ISO_8859_1_B0	0					// 176 °     DEGREE SIGN
#define ISO_8859_1_B1	0					// 177 ±     PLUS-MINUS SIGN
#define ISO_8859_1_B2	0					// 178 ²     SUPERSCRIPT TWO
#define ISO_8859_1_B3	0					// 179 ³     SUPERSCRIPT THREE
#define ISO_8859_1_B4	ACUTE_ACCENT_BITS + KEY_SPACE		// 180 ´     ACUTE ACCENT
#define ISO_8859_1_B5	KEY_M + ALTGR_MASK			// 181 µ     MICRO SIGN
#define ISO_8859_1_B6	0					// 182 ¶     PILCROW SIGN
#define ISO_8859_1_B7	0					// 183 ·     MIDDLE DOT
#define ISO_8859_1_B8	0					// 184 ¸     CEDILLA
#define ISO_8859_1_B9	0					// 185 ¹     SUPERSCRIPT ONE
#define ISO_8859_1_BA	0					// 186 º     MASCULINE ORDINAL
#define ISO_8859_1_BB	0					// 187 »     RIGHT DOUBLE ANGLE QUOTE
#define ISO_8859_1_BC	0					// 188 ¼     FRACTION ONE QUARTER
#define ISO_8859_1_BD	KEY_TILDE				// 189 ½     FRACTION ONE HALF
#define ISO_8859_1_BE	0					// 190 ¾     FRACTION THREE QUARTERS
#define ISO_8859_1_BF	0					// 191 ¿     INVERTED QUESTION MARK
#define ISO_8859_1_C0	GRAVE_ACCENT_BITS + KEY_A + SHIFT_MASK	// 192 À     A GRAVE
#define ISO_8859_1_C1	ACUTE_ACCENT_BITS + KEY_A + SHIFT_MASK	// 193 Á     A ACUTE
#define ISO_8859_1_C2	CIRCUMFLEX_BITS + KEY_A + SHIFT_MASK	// 194 Â     A CIRCUMFLEX
#define ISO_8859_1_C3	TILDE_BITS + KEY_A + SHIFT_MASK		// 195 Ã     A TILDE
#define ISO_8859_1_C4	DIAERESIS_BITS + KEY_A + SHIFT_MASK	// 196 Ä     A DIAERESIS
#define ISO_8859_1_C5	KEY_LEFT_BRACE + SHIFT_MASK		// 197 Å     A RING ABOVE
#define ISO_8859_1_C6	KEY_QUOTE + SHIFT_MASK			// 198 Æ     AE
#define ISO_8859_1_C7	0					// 199 Ç     C CEDILLA
#define ISO_8859_1_C8	GRAVE_ACCENT_BITS + KEY_E + SHIFT_MASK	// 200 È     E GRAVE
#define ISO_8859_1_C9	ACUTE_ACCENT_BITS + KEY_E + SHIFT_MASK	// 201 É     E ACUTE
#define ISO_8859_1_CA	CIRCUMFLEX_BITS + KEY_E + SHIFT_MASK	// 202 Ê     E CIRCUMFLEX
#define ISO_8859_1_CB	DIAERESIS_BITS + KEY_E + SHIFT_MASK	// 203 Ë     E DIAERESIS
#define ISO_8859_1_CC	GRAVE_ACCENT_BITS + KEY_I + SHIFT_MASK	// 204 Ì     I GRAVE
#define ISO_8859_1_CD	ACUTE_ACCENT_BITS + KEY_I + SHIFT_MASK	// 205 Í     I ACUTE
#define ISO_8859_1_CE	CIRCUMFLEX_BITS + KEY_I + SHIFT_MASK	// 206 Î     I CIRCUMFLEX
#define ISO_8859_1_CF	DIAERESIS_BITS + KEY_I + SHIFT_MASK	// 207 Ï     I DIAERESIS
#define ISO_8859_1_D0	KEY_D + ALTGR_MASK + SHIFT_MASK		// 208 Ð     ETH
#define ISO_8859_1_D1	TILDE_BITS + KEY_N + SHIFT_MASK		// 209 Ñ     N TILDE
#define ISO_8859_1_D2	GRAVE_ACCENT_BITS + KEY_O + SHIFT_MASK	// 210 Ò     O GRAVE
#define ISO_8859_1_D3	ACUTE_ACCENT_BITS + KEY_O + SHIFT_MASK	// 211 Ó     O ACUTE
#define ISO_8859_1_D4	CIRCUMFLEX_BITS + KEY_O + SHIFT_MASK	// 212 Ô     O CIRCUMFLEX
#define ISO_8859_1_D5	TILDE_BITS + KEY_O + SHIFT_MASK		// 213 Õ     O TILDE
#define ISO_8859_1_D6	DIAERESIS_BITS + KEY_O + SHIFT_MASK	// 214 Ö     O DIAERESIS
#define ISO_8859_1_D7	0					// 215 ×     MULTIPLICATION
#define ISO_8859_1_D8	KEY_SEMICOLON + SHIFT_MASK		// 216 Ø     O STROKE
#define ISO_8859_1_D9	GRAVE_ACCENT_BITS + KEY_U + SHIFT_MASK	// 217 Ù     U GRAVE
#define ISO_8859_1_DA	ACUTE_ACCENT_BITS + KEY_U + SHIFT_MASK	// 218 Ú     U ACUTE
#define ISO_8859_1_DB	CIRCUMFLEX_BITS + KEY_U + SHIFT_MASK	// 219 Û     U CIRCUMFLEX
#define ISO_8859_1_DC	DIAERESIS_BITS + KEY_U + SHIFT_MASK	// 220 Ü     U DIAERESIS
#define ISO_8859_1_DD	ACUTE_ACCENT_BITS + KEY_Y + SHIFT_MASK	// 221 Ý     Y ACUTE
#define ISO_8859_1_DE	KEY_T + ALTGR_MASK + SHIFT_MASK		// 222 Þ     THORN
#define ISO_8859_1_DF	KEY_S + ALTGR_MASK			// 223 ß     SHARP S
#define ISO_8859_1_E0	GRAVE_ACCENT_BITS + KEY_A		// 224 à     a GRAVE
#define ISO_8859_1_E1	ACUTE_ACCENT_BITS + KEY_A		// 225 á     a ACUTE
#define ISO_8859_1_E2	CIRCUMFLEX_BITS + KEY_A			// 226 â     a CIRCUMFLEX
#define ISO_8859_1_E3	TILDE_BITS + KEY_A			// 227 ã     a TILDE
#define ISO_8859_1_E4	DIAERESIS_BITS + KEY_A			// 228 ä     a DIAERESIS
#define ISO_8859_1_E5	KEY_LEFT_BRACE				// 229 å     a RING ABOVE
#define ISO_8859_1_E6	KEY_QUOTE				// 230 æ     ae
#define ISO_8859_1_E7	0					// 231 ç     c CEDILLA
#define ISO_8859_1_E8	GRAVE_ACCENT_BITS + KEY_E		// 232 è     e GRAVE
#define ISO_8859_1_E9	ACUTE_ACCENT_BITS + KEY_E		// 233 é     e ACUTE
#define ISO_8859_1_EA	CIRCUMFLEX_BITS + KEY_E			// 234 ê     e CIRCUMFLEX
#define ISO_8859_1_EB	DIAERESIS_BITS + KEY_E			// 235 ë     e DIAERESIS
#define ISO_8859_1_EC	GRAVE_ACCENT_BITS + KEY_I		// 236 ì     i GRAVE
#define ISO_8859_1_ED	ACUTE_ACCENT_BITS + KEY_I		// 237 í     i ACUTE
#define ISO_8859_1_EE	CIRCUMFLEX_BITS + KEY_I			// 238 î     i CIRCUMFLEX
#define ISO_8859_1_EF	DIAERESIS_BITS + KEY_I			// 239 ï     i DIAERESIS
#define ISO_8859_1_F0	KEY_D + ALTGR_MASK			// 240 ð     ETH
#define ISO_8859_1_F1	TILDE_BITS + KEY_N			// 241 ñ     n TILDE
#define ISO_8859_1_F2	GRAVE_ACCENT_BITS + KEY_O		// 242 ò     o GRAVE
#define ISO_8859_1_F3	ACUTE_ACCENT_BITS + KEY_O		// 243 ó     o ACUTE
#define ISO_8859_1_F4	CIRCUMFLEX_BITS + KEY_O			// 244 ô     o CIRCUMFLEX
#define ISO_8859_1_F5	TILDE_BITS + KEY_O			// 245 õ     o TILDE
#define ISO_8859_1_F6	DIAERESIS_BITS + KEY_O			// 246 ö     o DIAERESIS
#define ISO_8859_1_F7	0					// 247 ÷     DIVISION
#define ISO_8859_1_F8	KEY_SEMICOLON				// 248 ø     o STROKE
#define ISO_8859_1_F9	GRAVE_ACCENT_BITS + KEY_U		// 249 ù     u GRAVE
#define ISO_8859_1_FA	ACUTE_ACCENT_BITS + KEY_U		// 250 ú     u ACUTE
#define ISO_8859_1_FB	CIRCUMFLEX_BITS + KEY_U			// 251 û     u CIRCUMFLEX
#define ISO_8859_1_FC	DIAERESIS_BITS + KEY_U			// 252 ü     u DIAERESIS
#define ISO_8859_1_FD	ACUTE_ACCENT_BITS + KEY_Y		// 253 ý     y ACUTE
#define ISO_8859_1_FE	KEY_T + ALTGR_MASK			// 254 þ     THORN
#define ISO_8859_1_FF	DIAERESIS_BITS + KEY_Y			// 255 ÿ     y DIAERESIS
#define UNICODE_20AC	KEY_E + ALTGR_MASK			//     €     Euro Sign
// not yet implemented
#define UNICODE_EXTRA00	0x20AC
#define KEYCODE_EXTRA00	KEY_E + ALTGR_MASK			// 20AC €    Euro Sign

#endif // LAYOUT_NORWEGIAN






#ifdef LAYOUT_SWEDISH

#define SHIFT_MASK		0x0040
#define ALTGR_MASK		0x0080
#define DEADKEYS_MASK		0x0700
#define	CIRCUMFLEX_BITS		0x0100
#define ACUTE_ACCENT_BITS	0x0200
#define GRAVE_ACCENT_BITS	0x0300
#define TILDE_BITS		0x0400
#define DIAERESIS_BITS		0x0500
#define KEYCODE_TYPE		uint16_t
#define DEADKEY_CIRCUMFLEX	KEY_RIGHT_BRACE + SHIFT_MASK
#define DEADKEY_ACUTE_ACCENT	KEY_EQUAL
#define DEADKEY_GRAVE_ACCENT	KEY_EQUAL + SHIFT_MASK
#define DEADKEY_TILDE		KEY_RIGHT_BRACE + ALTGR_MASK
#define DEADKEY_DIAERESIS	KEY_RIGHT_BRACE
#define KEY_NON_US_100		63

#define ASCII_20	KEY_SPACE				// 32  
#define ASCII_21	KEY_1 + SHIFT_MASK			// 33 !
#define ASCII_22	KEY_2 + SHIFT_MASK			// 34 "
#define ASCII_23	KEY_3 + SHIFT_MASK			// 35 #
#define ASCII_24	KEY_4 + ALTGR_MASK			// 36 $
#define ASCII_25	KEY_5 + SHIFT_MASK			// 37 %
#define ASCII_26	KEY_6 + SHIFT_MASK			// 38 &
#define ASCII_27	KEY_BACKSLASH				// 39 '  
#define ASCII_28	KEY_8 + SHIFT_MASK			// 40 ( 
#define ASCII_29	KEY_9 + SHIFT_MASK			// 41 )
#define ASCII_2A	KEY_BACKSLASH + SHIFT_MASK		// 42 *
#define ASCII_2B	KEY_MINUS				// 43 +
#define ASCII_2C	KEY_COMMA				// 44 ,
#define ASCII_2D	KEY_SLASH				// 45 -
#define ASCII_2E	KEY_PERIOD				// 46 .
#define ASCII_2F	KEY_7 + SHIFT_MASK			// 47 /
#define ASCII_30	KEY_0					// 48 0
#define ASCII_31	KEY_1					// 49 1
#define ASCII_32	KEY_2					// 50 2
#define ASCII_33	KEY_3					// 51 3
#define ASCII_34	KEY_4					// 52 4
#define ASCII_35	KEY_5					// 53 5
#define ASCII_36	KEY_6					// 54 6
#define ASCII_37	KEY_7					// 55 7
#define ASCII_38	KEY_8					// 55 8
#define ASCII_39	KEY_9					// 57 9
#define ASCII_3A	KEY_PERIOD + SHIFT_MASK			// 58 :
#define ASCII_3B	KEY_COMMA + SHIFT_MASK			// 59 ;
#define ASCII_3C	KEY_NON_US_100				// 60 <
#define ASCII_3D	KEY_0 + SHIFT_MASK			// 61 =
#define ASCII_3E	KEY_NON_US_100 + SHIFT_MASK		// 62 >
#define ASCII_3F	KEY_MINUS + SHIFT_MASK			// 63 ?
#define ASCII_40	KEY_2 + ALTGR_MASK			// 64 @
#define ASCII_41	KEY_A + SHIFT_MASK			// 65 A
#define ASCII_42	KEY_B + SHIFT_MASK			// 66 B
#define ASCII_43	KEY_C + SHIFT_MASK			// 67 C
#define ASCII_44	KEY_D + SHIFT_MASK			// 68 D
#define ASCII_45	KEY_E + SHIFT_MASK			// 69 E
#define ASCII_46	KEY_F + SHIFT_MASK			// 70 F
#define ASCII_47	KEY_G + SHIFT_MASK			// 71 G
#define ASCII_48	KEY_H + SHIFT_MASK			// 72 H
#define ASCII_49	KEY_I + SHIFT_MASK			// 73 I
#define ASCII_4A	KEY_J + SHIFT_MASK			// 74 J
#define ASCII_4B	KEY_K + SHIFT_MASK			// 75 K
#define ASCII_4C	KEY_L + SHIFT_MASK			// 76 L
#define ASCII_4D	KEY_M + SHIFT_MASK			// 77 M
#define ASCII_4E	KEY_N + SHIFT_MASK			// 78 N
#define ASCII_4F	KEY_O + SHIFT_MASK			// 79 O
#define ASCII_50	KEY_P + SHIFT_MASK			// 80 P
#define ASCII_51	KEY_Q + SHIFT_MASK			// 81 Q
#define ASCII_52	KEY_R + SHIFT_MASK			// 82 R
#define ASCII_53	KEY_S + SHIFT_MASK			// 83 S
#define ASCII_54	KEY_T + SHIFT_MASK			// 84 T
#define ASCII_55	KEY_U + SHIFT_MASK			// 85 U
#define ASCII_56	KEY_V + SHIFT_MASK			// 86 V
#define ASCII_57	KEY_W + SHIFT_MASK			// 87 W
#define ASCII_58	KEY_X + SHIFT_MASK			// 88 X
#define ASCII_59	KEY_Y + SHIFT_MASK			// 89 Y
#define ASCII_5A	KEY_Z + SHIFT_MASK			// 90 Z
#define ASCII_5B	KEY_8 + ALTGR_MASK			// 91 [
#define ASCII_5C	KEY_MINUS + ALTGR_MASK			// 92 
#define ASCII_5D	KEY_9 + ALTGR_MASK			// 93 ]
#define ASCII_5E	CIRCUMFLEX_BITS + KEY_SPACE		// 94 ^
#define ASCII_5F	KEY_SLASH + SHIFT_MASK			// 95 _
#define ASCII_60	GRAVE_ACCENT_BITS + KEY_SPACE		// 96 `
#define ASCII_61	KEY_A					// 97 a
#define ASCII_62	KEY_B					// 98 b
#define ASCII_63	KEY_C					// 99 c
#define ASCII_64	KEY_D					// 100 d
#define ASCII_65	KEY_E					// 101 e
#define ASCII_66	KEY_F					// 102 f
#define ASCII_67	KEY_G					// 103 g
#define ASCII_68	KEY_H					// 104 h
#define ASCII_69	KEY_I					// 105 i
#define ASCII_6A	KEY_J					// 106 j
#define ASCII_6B	KEY_K					// 107 k
#define ASCII_6C	KEY_L					// 108 l
#define ASCII_6D	KEY_M					// 109 m
#define ASCII_6E	KEY_N					// 110 n
#define ASCII_6F	KEY_O					// 111 o
#define ASCII_70	KEY_P					// 112 p
#define ASCII_71	KEY_Q					// 113 q
#define ASCII_72	KEY_R					// 114 r
#define ASCII_73	KEY_S					// 115 s
#define ASCII_74	KEY_T					// 116 t
#define ASCII_75	KEY_U					// 117 u
#define ASCII_76	KEY_V					// 118 v
#define ASCII_77	KEY_W					// 119 w
#define ASCII_78	KEY_X					// 120 x
#define ASCII_79	KEY_Y					// 121 y
#define ASCII_7A	KEY_Z					// 122 z
#define ASCII_7B	KEY_7 + ALTGR_MASK			// 123 {
#define ASCII_7C	KEY_NON_US_100 + ALTGR_MASK		// 124 |
#define ASCII_7D	KEY_0 + ALTGR_MASK			// 125 }
#define ASCII_7E	TILDE_BITS + KEY_SPACE			// 126 ~
#define ASCII_7F	KEY_BACKSPACE				// 127

#define ISO_8859_1_A0	KEY_SPACE				// 160       Nonbreakng Space
#define ISO_8859_1_A1	0					// 161 ¡     Inverted Exclamation
#define ISO_8859_1_A2	0					// 162 ¢     Cent SIGN
#define ISO_8859_1_A3	KEY_3 + ALTGR_MASK			// 163 £     Pound Sign
#define ISO_8859_1_A4	KEY_4 + SHIFT_MASK			// 164 ¤     Currency Sign
#define ISO_8859_1_A5	0					// 165 ¥     YEN SIGN
#define ISO_8859_1_A6	0					// 166 ¦     BROKEN BAR
#define ISO_8859_1_A7	KEY_TILDE				// 167 §     SECTION SIGN
#define ISO_8859_1_A8	DIAERESIS_BITS + KEY_SPACE		// 168 ¨     DIAERESIS
#define ISO_8859_1_A9	0					// 169 ©     COPYRIGHT SIGN
#define ISO_8859_1_AA	0					// 170 ª     FEMININE ORDINAL
#define ISO_8859_1_AB	KEY_4					// 171 «     LEFT DOUBLE ANGLE QUOTE
#define ISO_8859_1_AC	0					// 172 ¬     NOT SIGN
#define ISO_8859_1_AD	0					// 173       SOFT HYPHEN
#define ISO_8859_1_AE	0					// 174 ®     REGISTERED SIGN
#define ISO_8859_1_AF	0					// 175 ¯     MACRON
#define ISO_8859_1_B0	0					// 176 °     DEGREE SIGN
#define ISO_8859_1_B1	0					// 177 ±     PLUS-MINUS SIGN
#define ISO_8859_1_B2	0					// 178 ²     SUPERSCRIPT TWO
#define ISO_8859_1_B3	0					// 179 ³     SUPERSCRIPT THREE
#define ISO_8859_1_B4	ACUTE_ACCENT_BITS + KEY_SPACE		// 180 ´     ACUTE ACCENT
#define ISO_8859_1_B5	KEY_M + ALTGR_MASK			// 181 µ     MICRO SIGN
#define ISO_8859_1_B6	0					// 182 ¶     PILCROW SIGN
#define ISO_8859_1_B7	0					// 183 ·     MIDDLE DOT
#define ISO_8859_1_B8	0					// 184 ¸     CEDILLA
#define ISO_8859_1_B9	0					// 185 ¹     SUPERSCRIPT ONE
#define ISO_8859_1_BA	0					// 186 º     MASCULINE ORDINAL
#define ISO_8859_1_BB	0					// 187 »     RIGHT DOUBLE ANGLE QUOTE
#define ISO_8859_1_BC	0					// 188 ¼     FRACTION ONE QUARTER
#define ISO_8859_1_BD	KEY_TILDE + SHIFT_MASK			// 189 ½     FRACTION ONE HALF
#define ISO_8859_1_BE	0					// 190 ¾     FRACTION THREE QUARTERS
#define ISO_8859_1_BF	0					// 191 ¿     INVERTED QUESTION MARK
#define ISO_8859_1_C0	GRAVE_ACCENT_BITS + KEY_A + SHIFT_MASK	// 192 À     A GRAVE
#define ISO_8859_1_C1	ACUTE_ACCENT_BITS + KEY_A + SHIFT_MASK	// 193 Á     A ACUTE
#define ISO_8859_1_C2	CIRCUMFLEX_BITS + KEY_A + SHIFT_MASK	// 194 Â     A CIRCUMFLEX
#define ISO_8859_1_C3	TILDE_BITS + KEY_A + SHIFT_MASK		// 195 Ã     A TILDE
#define ISO_8859_1_C4	KEY_QUOTE + SHIFT_MASK			// 196 Ä     A DIAERESIS
#define ISO_8859_1_C5	KEY_LEFT_BRACE + SHIFT_MASK		// 197 Å     A RING ABOVE
#define ISO_8859_1_C6	0					// 198 Æ     AE
#define ISO_8859_1_C7	0					// 199 Ç     C CEDILLA
#define ISO_8859_1_C8	GRAVE_ACCENT_BITS + KEY_E + SHIFT_MASK	// 200 È     E GRAVE
#define ISO_8859_1_C9	ACUTE_ACCENT_BITS + KEY_E + SHIFT_MASK	// 201 É     E ACUTE
#define ISO_8859_1_CA	CIRCUMFLEX_BITS + KEY_E + SHIFT_MASK	// 202 Ê     E CIRCUMFLEX
#define ISO_8859_1_CB	DIAERESIS_BITS + KEY_E + SHIFT_MASK	// 203 Ë     E DIAERESIS
#define ISO_8859_1_CC	GRAVE_ACCENT_BITS + KEY_I + SHIFT_MASK	// 204 Ì     I GRAVE
#define ISO_8859_1_CD	ACUTE_ACCENT_BITS + KEY_I + SHIFT_MASK	// 205 Í     I ACUTE
#define ISO_8859_1_CE	CIRCUMFLEX_BITS + KEY_I + SHIFT_MASK	// 206 Î     I CIRCUMFLEX
#define ISO_8859_1_CF	DIAERESIS_BITS + KEY_I + SHIFT_MASK	// 207 Ï     I DIAERESIS
#define ISO_8859_1_D0	KEY_D + ALTGR_MASK + SHIFT_MASK		// 208 Ð     ETH
#define ISO_8859_1_D1	TILDE_BITS + KEY_N + SHIFT_MASK		// 209 Ñ     N TILDE
#define ISO_8859_1_D2	GRAVE_ACCENT_BITS + KEY_O + SHIFT_MASK	// 210 Ò     O GRAVE
#define ISO_8859_1_D3	ACUTE_ACCENT_BITS + KEY_O + SHIFT_MASK	// 211 Ó     O ACUTE
#define ISO_8859_1_D4	CIRCUMFLEX_BITS + KEY_O + SHIFT_MASK	// 212 Ô     O CIRCUMFLEX
#define ISO_8859_1_D5	TILDE_BITS + KEY_O + SHIFT_MASK		// 213 Õ     O TILDE
#define ISO_8859_1_D6	KEY_SEMICOLON + SHIFT_MASK		// 214 Ö     O DIAERESIS
#define ISO_8859_1_D7	0					// 215 ×     MULTIPLICATION
#define ISO_8859_1_D8	0					// 216 Ø     O STROKE
#define ISO_8859_1_D9	GRAVE_ACCENT_BITS + KEY_U + SHIFT_MASK	// 217 Ù     U GRAVE
#define ISO_8859_1_DA	ACUTE_ACCENT_BITS + KEY_U + SHIFT_MASK	// 218 Ú     U ACUTE
#define ISO_8859_1_DB	CIRCUMFLEX_BITS + KEY_U + SHIFT_MASK	// 219 Û     U CIRCUMFLEX
#define ISO_8859_1_DC	DIAERESIS_BITS + KEY_U + SHIFT_MASK	// 220 Ü     U DIAERESIS
#define ISO_8859_1_DD	ACUTE_ACCENT_BITS + KEY_Y + SHIFT_MASK	// 221 Ý     Y ACUTE
#define ISO_8859_1_DE	KEY_T + ALTGR_MASK + SHIFT_MASK		// 222 Þ     THORN
#define ISO_8859_1_DF	KEY_S + ALTGR_MASK			// 223 ß     SHARP S
#define ISO_8859_1_E0	GRAVE_ACCENT_BITS + KEY_A		// 224 à     a GRAVE
#define ISO_8859_1_E1	ACUTE_ACCENT_BITS + KEY_A		// 225 á     a ACUTE
#define ISO_8859_1_E2	CIRCUMFLEX_BITS + KEY_A			// 226 â     a CIRCUMFLEX
#define ISO_8859_1_E3	TILDE_BITS + KEY_A			// 227 ã     a TILDE
#define ISO_8859_1_E4	KEY_QUOTE				// 228 ä     a DIAERESIS
#define ISO_8859_1_E5	KEY_LEFT_BRACE				// 229 å     a RING ABOVE
#define ISO_8859_1_E6	0					// 230 æ     ae
#define ISO_8859_1_E7	0					// 231 ç     c CEDILLA
#define ISO_8859_1_E8	GRAVE_ACCENT_BITS + KEY_E		// 232 è     e GRAVE
#define ISO_8859_1_E9	ACUTE_ACCENT_BITS + KEY_E		// 233 é     e ACUTE
#define ISO_8859_1_EA	CIRCUMFLEX_BITS + KEY_E			// 234 ê     e CIRCUMFLEX
#define ISO_8859_1_EB	DIAERESIS_BITS + KEY_E			// 235 ë     e DIAERESIS
#define ISO_8859_1_EC	GRAVE_ACCENT_BITS + KEY_I		// 236 ì     i GRAVE
#define ISO_8859_1_ED	ACUTE_ACCENT_BITS + KEY_I		// 237 í     i ACUTE
#define ISO_8859_1_EE	CIRCUMFLEX_BITS + KEY_I			// 238 î     i CIRCUMFLEX
#define ISO_8859_1_EF	DIAERESIS_BITS + KEY_I			// 239 ï     i DIAERESIS
#define ISO_8859_1_F0	KEY_D + ALTGR_MASK			// 240 ð     ETH
#define ISO_8859_1_F1	TILDE_BITS + KEY_N			// 241 ñ     n TILDE
#define ISO_8859_1_F2	GRAVE_ACCENT_BITS + KEY_O		// 242 ò     o GRAVE
#define ISO_8859_1_F3	ACUTE_ACCENT_BITS + KEY_O		// 243 ó     o ACUTE
#define ISO_8859_1_F4	CIRCUMFLEX_BITS + KEY_O			// 244 ô     o CIRCUMFLEX
#define ISO_8859_1_F5	TILDE_BITS + KEY_O			// 245 õ     o TILDE
#define ISO_8859_1_F6	KEY_SEMICOLON				// 246 ö     o DIAERESIS
#define ISO_8859_1_F7	0					// 247 ÷     DIVISION
#define ISO_8859_1_F8	0					// 248 ø     o STROKE
#define ISO_8859_1_F9	GRAVE_ACCENT_BITS + KEY_U		// 249 ù     u GRAVE
#define ISO_8859_1_FA	ACUTE_ACCENT_BITS + KEY_U		// 250 ú     u ACUTE
#define ISO_8859_1_FB	CIRCUMFLEX_BITS + KEY_U			// 251 û     u CIRCUMFLEX
#define ISO_8859_1_FC	DIAERESIS_BITS + KEY_U			// 252 ü     u DIAERESIS
#define ISO_8859_1_FD	ACUTE_ACCENT_BITS + KEY_Y		// 253 ý     y ACUTE
#define ISO_8859_1_FE	KEY_T + ALTGR_MASK			// 254 þ     THORN
#define ISO_8859_1_FF	DIAERESIS_BITS + KEY_Y			// 255 ÿ     y DIAERESIS
#define UNICODE_20AC	KEY_E + ALTGR_MASK			//     €     Euro Sign
// not yet implemented
#define UNICODE_EXTRA00	0x20AC
#define KEYCODE_EXTRA00	KEY_E + ALTGR_MASK			// 20AC €    Euro Sign

#endif // LAYOUT_SWEDISH






#ifdef LAYOUT_SPANISH

#define SHIFT_MASK		0x0040
#define ALTGR_MASK		0x0080
#define DEADKEYS_MASK		0x0700
#define	CIRCUMFLEX_BITS		0x0100
#define ACUTE_ACCENT_BITS	0x0200
#define GRAVE_ACCENT_BITS	0x0300
#define TILDE_BITS		0x0400
#define DIAERESIS_BITS		0x0500
#define KEYCODE_TYPE		uint16_t
#define DEADKEY_CIRCUMFLEX	KEY_LEFT_BRACE + SHIFT_MASK
#define DEADKEY_ACUTE_ACCENT	KEY_QUOTE
#define DEADKEY_GRAVE_ACCENT	KEY_LEFT_BRACE
#define DEADKEY_TILDE		KEY_4 + ALTGR_MASK
#define DEADKEY_DIAERESIS	KEY_QUOTE + SHIFT_MASK
#define KEY_NON_US_100		63

#define ASCII_20	KEY_SPACE				// 32  
#define ASCII_21	KEY_1 + SHIFT_MASK			// 33 !
#define ASCII_22	KEY_2 + SHIFT_MASK			// 34 "
#define ASCII_23	KEY_3 + ALTGR_MASK			// 35 #
#define ASCII_24	KEY_4 + SHIFT_MASK			// 36 $
#define ASCII_25	KEY_5 + SHIFT_MASK			// 37 %
#define ASCII_26	KEY_6 + SHIFT_MASK			// 38 &
#define ASCII_27	KEY_MINUS				// 39 '  
#define ASCII_28	KEY_8 + SHIFT_MASK			// 40 ( 
#define ASCII_29	KEY_9 + SHIFT_MASK			// 41 )
#define ASCII_2A	KEY_RIGHT_BRACE + SHIFT_MASK		// 42 *
#define ASCII_2B	KEY_RIGHT_BRACE				// 43 +
#define ASCII_2C	KEY_COMMA				// 44 ,
#define ASCII_2D	KEY_SLASH				// 45 -
#define ASCII_2E	KEY_PERIOD				// 46 .
#define ASCII_2F	KEY_7 + SHIFT_MASK			// 47 /
#define ASCII_30	KEY_0					// 48 0
#define ASCII_31	KEY_1					// 49 1
#define ASCII_32	KEY_2					// 50 2
#define ASCII_33	KEY_3					// 51 3
#define ASCII_34	KEY_4					// 52 4
#define ASCII_35	KEY_5					// 53 5
#define ASCII_36	KEY_6					// 54 6
#define ASCII_37	KEY_7					// 55 7
#define ASCII_38	KEY_8					// 55 8
#define ASCII_39	KEY_9					// 57 9
#define ASCII_3A	KEY_PERIOD + SHIFT_MASK			// 58 :
#define ASCII_3B	KEY_COMMA + SHIFT_MASK			// 59 ;
#define ASCII_3C	KEY_NON_US_100				// 60 <
#define ASCII_3D	KEY_0 + SHIFT_MASK			// 61 =
#define ASCII_3E	KEY_NON_US_100 + SHIFT_MASK		// 62 >
#define ASCII_3F	KEY_MINUS + SHIFT_MASK			// 63 ?
#define ASCII_40	KEY_2 + ALTGR_MASK			// 64 @
#define ASCII_41	KEY_A + SHIFT_MASK			// 65 A
#define ASCII_42	KEY_B + SHIFT_MASK			// 66 B
#define ASCII_43	KEY_C + SHIFT_MASK			// 67 C
#define ASCII_44	KEY_D + SHIFT_MASK			// 68 D
#define ASCII_45	KEY_E + SHIFT_MASK			// 69 E
#define ASCII_46	KEY_F + SHIFT_MASK			// 70 F
#define ASCII_47	KEY_G + SHIFT_MASK			// 71 G
#define ASCII_48	KEY_H + SHIFT_MASK			// 72 H
#define ASCII_49	KEY_I + SHIFT_MASK			// 73 I
#define ASCII_4A	KEY_J + SHIFT_MASK			// 74 J
#define ASCII_4B	KEY_K + SHIFT_MASK			// 75 K
#define ASCII_4C	KEY_L + SHIFT_MASK			// 76 L
#define ASCII_4D	KEY_M + SHIFT_MASK			// 77 M
#define ASCII_4E	KEY_N + SHIFT_MASK			// 78 N
#define ASCII_4F	KEY_O + SHIFT_MASK			// 79 O
#define ASCII_50	KEY_P + SHIFT_MASK			// 80 P
#define ASCII_51	KEY_Q + SHIFT_MASK			// 81 Q
#define ASCII_52	KEY_R + SHIFT_MASK			// 82 R
#define ASCII_53	KEY_S + SHIFT_MASK			// 83 S
#define ASCII_54	KEY_T + SHIFT_MASK			// 84 T
#define ASCII_55	KEY_U + SHIFT_MASK			// 85 U
#define ASCII_56	KEY_V + SHIFT_MASK			// 86 V
#define ASCII_57	KEY_W + SHIFT_MASK			// 87 W
#define ASCII_58	KEY_X + SHIFT_MASK			// 88 X
#define ASCII_59	KEY_Y + SHIFT_MASK			// 89 Y
#define ASCII_5A	KEY_Z + SHIFT_MASK			// 90 Z
#define ASCII_5B	KEY_LEFT_BRACE + ALTGR_MASK		// 91 [
#define ASCII_5C	KEY_TILDE + ALTGR_MASK			// 92 
#define ASCII_5D	KEY_RIGHT_BRACE + ALTGR_MASK		// 93 ]
#define ASCII_5E	CIRCUMFLEX_BITS + KEY_SPACE		// 94 ^
#define ASCII_5F	KEY_SLASH + SHIFT_MASK			// 95 _
#define ASCII_60	GRAVE_ACCENT_BITS + KEY_SPACE		// 96 `
#define ASCII_61	KEY_A					// 97 a
#define ASCII_62	KEY_B					// 98 b
#define ASCII_63	KEY_C					// 99 c
#define ASCII_64	KEY_D					// 100 d
#define ASCII_65	KEY_E					// 101 e
#define ASCII_66	KEY_F					// 102 f
#define ASCII_67	KEY_G					// 103 g
#define ASCII_68	KEY_H					// 104 h
#define ASCII_69	KEY_I					// 105 i
#define ASCII_6A	KEY_J					// 106 j
#define ASCII_6B	KEY_K					// 107 k
#define ASCII_6C	KEY_L					// 108 l
#define ASCII_6D	KEY_M					// 109 m
#define ASCII_6E	KEY_N					// 110 n
#define ASCII_6F	KEY_O					// 111 o
#define ASCII_70	KEY_P					// 112 p
#define ASCII_71	KEY_Q					// 113 q
#define ASCII_72	KEY_R					// 114 r
#define ASCII_73	KEY_S					// 115 s
#define ASCII_74	KEY_T					// 116 t
#define ASCII_75	KEY_U					// 117 u
#define ASCII_76	KEY_V					// 118 v
#define ASCII_77	KEY_W					// 119 w
#define ASCII_78	KEY_X					// 120 x
#define ASCII_79	KEY_Y					// 121 y
#define ASCII_7A	KEY_Z					// 122 z
#define ASCII_7B	KEY_QUOTE + ALTGR_MASK			// 123 {
#define ASCII_7C	KEY_1 + ALTGR_MASK			// 124 |
#define ASCII_7D	KEY_BACKSLASH + ALTGR_MASK		// 125 }
#define ASCII_7E	TILDE_BITS + KEY_SPACE			// 126 ~
#define ASCII_7F	KEY_BACKSPACE				// 127

#define ISO_8859_1_A0	KEY_SPACE				// 160       Nonbreakng Space
#define ISO_8859_1_A1	KEY_EQUAL				// 161 ¡     Inverted Exclamation
#define ISO_8859_1_A2	0					// 162 ¢     Cent SIGN
#define ISO_8859_1_A3	0					// 163 £     Pound Sign
#define ISO_8859_1_A4	0					// 164 ¤     Currency Sign
#define ISO_8859_1_A5	0					// 165 ¥     YEN SIGN
#define ISO_8859_1_A6	0					// 166 ¦     BROKEN BAR
#define ISO_8859_1_A7	0					// 167 §     SECTION SIGN
#define ISO_8859_1_A8	DIAERESIS_BITS + KEY_SPACE		// 168 ¨     DIAERESIS
#define ISO_8859_1_A9	0					// 169 ©     COPYRIGHT SIGN
#define ISO_8859_1_AA	KEY_TILDE + SHIFT_MASK			// 170 ª     FEMININE ORDINAL
#define ISO_8859_1_AB	0					// 171 «     LEFT DOUBLE ANGLE QUOTE
#define ISO_8859_1_AC	KEY_6 + ALTGR_MASK			// 172 ¬     NOT SIGN
#define ISO_8859_1_AD	0					// 173       SOFT HYPHEN
#define ISO_8859_1_AE	0					// 174 ®     REGISTERED SIGN
#define ISO_8859_1_AF	0					// 175 ¯     MACRON
#define ISO_8859_1_B0	0					// 176 °     DEGREE SIGN
#define ISO_8859_1_B1	0					// 177 ±     PLUS-MINUS SIGN
#define ISO_8859_1_B2	0					// 178 ²     SUPERSCRIPT TWO
#define ISO_8859_1_B3	0					// 179 ³     SUPERSCRIPT THREE
#define ISO_8859_1_B4	ACUTE_ACCENT_BITS + KEY_SPACE		// 180 ´     ACUTE ACCENT
#define ISO_8859_1_B5	0					// 181 µ     MICRO SIGN
#define ISO_8859_1_B6	0					// 182 ¶     PILCROW SIGN
#define ISO_8859_1_B7	KEY_3 + SHIFT_MASK			// 183 ·     MIDDLE DOT
#define ISO_8859_1_B8	0					// 184 ¸     CEDILLA
#define ISO_8859_1_B9	0					// 185 ¹     SUPERSCRIPT ONE
#define ISO_8859_1_BA	KEY_TILDE				// 186 º     MASCULINE ORDINAL
#define ISO_8859_1_BB	0					// 187 »     RIGHT DOUBLE ANGLE QUOTE
#define ISO_8859_1_BC	0					// 188 ¼     FRACTION ONE QUARTER
#define ISO_8859_1_BD	0					// 189 ½     FRACTION ONE HALF
#define ISO_8859_1_BE	0					// 190 ¾     FRACTION THREE QUARTERS
#define ISO_8859_1_BF	KEY_EQUAL				// 191 ¿     INVERTED QUESTION MARK
#define ISO_8859_1_C0	GRAVE_ACCENT_BITS + KEY_A + SHIFT_MASK	// 192 À     A GRAVE
#define ISO_8859_1_C1	ACUTE_ACCENT_BITS + KEY_A + SHIFT_MASK	// 193 Á     A ACUTE
#define ISO_8859_1_C2	CIRCUMFLEX_BITS + KEY_A + SHIFT_MASK	// 194 Â     A CIRCUMFLEX
#define ISO_8859_1_C3	TILDE_BITS + KEY_A + SHIFT_MASK		// 195 Ã     A TILDE
#define ISO_8859_1_C4	DIAERESIS_BITS + KEY_A + SHIFT_MASK	// 196 Ä     A DIAERESIS
#define ISO_8859_1_C5	0					// 197 Å     A RING ABOVE
#define ISO_8859_1_C6	0					// 198 Æ     AE
#define ISO_8859_1_C7	KEY_BACKSLASH + SHIFT_MASK		// 199 Ç     C CEDILLA
#define ISO_8859_1_C8	GRAVE_ACCENT_BITS + KEY_E + SHIFT_MASK	// 200 È     E GRAVE
#define ISO_8859_1_C9	ACUTE_ACCENT_BITS + KEY_E + SHIFT_MASK	// 201 É     E ACUTE
#define ISO_8859_1_CA	CIRCUMFLEX_BITS + KEY_E + SHIFT_MASK	// 202 Ê     E CIRCUMFLEX
#define ISO_8859_1_CB	DIAERESIS_BITS + KEY_E + SHIFT_MASK	// 203 Ë     E DIAERESIS
#define ISO_8859_1_CC	GRAVE_ACCENT_BITS + KEY_I + SHIFT_MASK	// 204 Ì     I GRAVE
#define ISO_8859_1_CD	ACUTE_ACCENT_BITS + KEY_I + SHIFT_MASK	// 205 Í     I ACUTE
#define ISO_8859_1_CE	CIRCUMFLEX_BITS + KEY_I + SHIFT_MASK	// 206 Î     I CIRCUMFLEX
#define ISO_8859_1_CF	DIAERESIS_BITS + KEY_I + SHIFT_MASK	// 207 Ï     I DIAERESIS
#define ISO_8859_1_D0	0					// 208 Ð     ETH
#define ISO_8859_1_D1	KEY_SEMICOLON + SHIFT_MASK		// 209 Ñ     N TILDE
#define ISO_8859_1_D2	GRAVE_ACCENT_BITS + KEY_O + SHIFT_MASK	// 210 Ò     O GRAVE
#define ISO_8859_1_D3	ACUTE_ACCENT_BITS + KEY_O + SHIFT_MASK	// 211 Ó     O ACUTE
#define ISO_8859_1_D4	CIRCUMFLEX_BITS + KEY_O + SHIFT_MASK	// 212 Ô     O CIRCUMFLEX
#define ISO_8859_1_D5	TILDE_BITS + KEY_O + SHIFT_MASK		// 213 Õ     O TILDE
#define ISO_8859_1_D6	DIAERESIS_BITS + KEY_O + SHIFT_MASK	// 214 Ö     O DIAERESIS
#define ISO_8859_1_D7	0					// 215 ×     MULTIPLICATION
#define ISO_8859_1_D8	0					// 216 Ø     O STROKE
#define ISO_8859_1_D9	GRAVE_ACCENT_BITS + KEY_U + SHIFT_MASK	// 217 Ù     U GRAVE
#define ISO_8859_1_DA	ACUTE_ACCENT_BITS + KEY_U + SHIFT_MASK	// 218 Ú     U ACUTE
#define ISO_8859_1_DB	CIRCUMFLEX_BITS + KEY_U + SHIFT_MASK	// 219 Û     U CIRCUMFLEX
#define ISO_8859_1_DC	DIAERESIS_BITS + KEY_U + SHIFT_MASK	// 220 Ü     U DIAERESIS
#define ISO_8859_1_DD	ACUTE_ACCENT_BITS + KEY_Y + SHIFT_MASK	// 221 Ý     Y ACUTE
#define ISO_8859_1_DE	0					// 222 Þ     THORN
#define ISO_8859_1_DF	0					// 223 ß     SHARP S
#define ISO_8859_1_E0	GRAVE_ACCENT_BITS + KEY_A		// 224 à     a GRAVE
#define ISO_8859_1_E1	ACUTE_ACCENT_BITS + KEY_A		// 225 á     a ACUTE
#define ISO_8859_1_E2	CIRCUMFLEX_BITS + KEY_A			// 226 â     a CIRCUMFLEX
#define ISO_8859_1_E3	TILDE_BITS + KEY_A			// 227 ã     a TILDE
#define ISO_8859_1_E4	DIAERESIS_BITS + KEY_A			// 228 ä     a DIAERESIS
#define ISO_8859_1_E5	0					// 229 å     a RING ABOVE
#define ISO_8859_1_E6	0					// 230 æ     ae
#define ISO_8859_1_E7	KEY_BACKSLASH				// 231 ç     c CEDILLA
#define ISO_8859_1_E8	GRAVE_ACCENT_BITS + KEY_E		// 232 è     e GRAVE
#define ISO_8859_1_E9	ACUTE_ACCENT_BITS + KEY_E		// 233 é     e ACUTE
#define ISO_8859_1_EA	CIRCUMFLEX_BITS + KEY_E			// 234 ê     e CIRCUMFLEX
#define ISO_8859_1_EB	DIAERESIS_BITS + KEY_E			// 235 ë     e DIAERESIS
#define ISO_8859_1_EC	GRAVE_ACCENT_BITS + KEY_I		// 236 ì     i GRAVE
#define ISO_8859_1_ED	ACUTE_ACCENT_BITS + KEY_I		// 237 í     i ACUTE
#define ISO_8859_1_EE	CIRCUMFLEX_BITS + KEY_I			// 238 î     i CIRCUMFLEX
#define ISO_8859_1_EF	DIAERESIS_BITS + KEY_I			// 239 ï     i DIAERESIS
#define ISO_8859_1_F0	0					// 240 ð     ETH
#define ISO_8859_1_F1	KEY_SEMICOLON				// 241 ñ     n TILDE
#define ISO_8859_1_F2	GRAVE_ACCENT_BITS + KEY_O		// 242 ò     o GRAVE
#define ISO_8859_1_F3	ACUTE_ACCENT_BITS + KEY_O		// 243 ó     o ACUTE
#define ISO_8859_1_F4	CIRCUMFLEX_BITS + KEY_O			// 244 ô     o CIRCUMFLEX
#define ISO_8859_1_F5	TILDE_BITS + KEY_O			// 245 õ     o TILDE
#define ISO_8859_1_F6	DIAERESIS_BITS + KEY_O			// 246 ö     o DIAERESIS
#define ISO_8859_1_F7	0					// 247 ÷     DIVISION
#define ISO_8859_1_F8	0					// 248 ø     o STROKE
#define ISO_8859_1_F9	GRAVE_ACCENT_BITS + KEY_U		// 249 ù     u GRAVE
#define ISO_8859_1_FA	ACUTE_ACCENT_BITS + KEY_U		// 250 ú     u ACUTE
#define ISO_8859_1_FB	CIRCUMFLEX_BITS + KEY_U			// 251 û     u CIRCUMFLEX
#define ISO_8859_1_FC	DIAERESIS_BITS + KEY_U			// 252 ü     u DIAERESIS
#define ISO_8859_1_FD	ACUTE_ACCENT_BITS + KEY_Y		// 253 ý     y ACUTE
#define ISO_8859_1_FE	0					// 254 þ     THORN
#define ISO_8859_1_FF	DIAERESIS_BITS + KEY_Y			// 255 ÿ     y DIAERESIS
#define UNICODE_20AC	KEY_5 + ALTGR_MASK			//     €     Euro Sign
// not yet implemented
#define UNICODE_EXTRA00	0x20AC
#define KEYCODE_EXTRA00	KEY_5 + ALTGR_MASK			// 20AC €    Euro Sign

#endif // LAYOUT_SPANISH




#ifdef LAYOUT_PORTUGUESE

#define SHIFT_MASK		0x0040
#define ALTGR_MASK		0x0080
#define DEADKEYS_MASK		0x0700
#define	CIRCUMFLEX_BITS		0x0100
#define ACUTE_ACCENT_BITS	0x0200
#define GRAVE_ACCENT_BITS	0x0300
#define TILDE_BITS		0x0400
#define DIAERESIS_BITS		0x0500
#define KEYCODE_TYPE		uint16_t
#define DEADKEY_CIRCUMFLEX	KEY_BACKSLASH
#define DEADKEY_ACUTE_ACCENT	KEY_RIGHT_BRACE + SHIFT_MASK
#define DEADKEY_GRAVE_ACCENT	KEY_RIGHT_BRACE + SHIFT_MASK
#define DEADKEY_TILDE		KEY_BACKSLASH
#define DEADKEY_DIAERESIS	KEY_LEFT_BRACE + SHIFT_MASK
#define KEY_NON_US_100		63

#define ASCII_20	KEY_SPACE				// 32  
#define ASCII_21	KEY_1 + SHIFT_MASK			// 33 !
#define ASCII_22	KEY_2 + SHIFT_MASK			// 34 "
#define ASCII_23	KEY_3 + ALTGR_MASK			// 35 #
#define ASCII_24	KEY_4 + SHIFT_MASK			// 36 $
#define ASCII_25	KEY_5 + SHIFT_MASK			// 37 %
#define ASCII_26	KEY_6 + SHIFT_MASK			// 38 &
#define ASCII_27	KEY_MINUS				// 39 '
#define ASCII_28	KEY_8 + SHIFT_MASK			// 40 (
#define ASCII_29	KEY_9 + SHIFT_MASK			// 41 )
#define ASCII_2A	KEY_LEFT_BRACE + SHIFT_MASK		// 42 *
#define ASCII_2B	KEY_LEFT_BRACE				// 43 +
#define ASCII_2C	KEY_COMMA				// 44 ,
#define ASCII_2D	KEY_SLASH				// 45 -
#define ASCII_2E	KEY_PERIOD				// 46 .
#define ASCII_2F	KEY_7 + SHIFT_MASK			// 47 /
#define ASCII_30	KEY_0					// 48 0
#define ASCII_31	KEY_1					// 49 1
#define ASCII_32	KEY_2					// 50 2
#define ASCII_33	KEY_3					// 51 3
#define ASCII_34	KEY_4					// 52 4
#define ASCII_35	KEY_5					// 53 5
#define ASCII_36	KEY_6					// 54 6
#define ASCII_37	KEY_7					// 55 7
#define ASCII_38	KEY_8					// 55 8
#define ASCII_39	KEY_9					// 57 9
#define ASCII_3A	KEY_PERIOD + SHIFT_MASK			// 58 :
#define ASCII_3B	KEY_COMMA + SHIFT_MASK			// 59 ;
#define ASCII_3C	KEY_NON_US_100				// 60 <
#define ASCII_3D	KEY_0 + SHIFT_MASK			// 61 =
#define ASCII_3E	KEY_NON_US_100 + SHIFT_MASK		// 62 >
#define ASCII_3F	KEY_MINUS + SHIFT_MASK			// 63 ?
#define ASCII_40	KEY_2 + ALTGR_MASK			// 64 @
#define ASCII_41	KEY_A + SHIFT_MASK			// 65 A
#define ASCII_42	KEY_B + SHIFT_MASK			// 66 B
#define ASCII_43	KEY_C + SHIFT_MASK			// 67 C
#define ASCII_44	KEY_D + SHIFT_MASK			// 68 D
#define ASCII_45	KEY_E + SHIFT_MASK			// 69 E
#define ASCII_46	KEY_F + SHIFT_MASK			// 70 F
#define ASCII_47	KEY_G + SHIFT_MASK			// 71 G
#define ASCII_48	KEY_H + SHIFT_MASK			// 72 H
#define ASCII_49	KEY_I + SHIFT_MASK			// 73 I
#define ASCII_4A	KEY_J + SHIFT_MASK			// 74 J
#define ASCII_4B	KEY_K + SHIFT_MASK			// 75 K
#define ASCII_4C	KEY_L + SHIFT_MASK			// 76 L
#define ASCII_4D	KEY_M + SHIFT_MASK			// 77 M
#define ASCII_4E	KEY_N + SHIFT_MASK			// 78 N
#define ASCII_4F	KEY_O + SHIFT_MASK			// 79 O
#define ASCII_50	KEY_P + SHIFT_MASK			// 80 P
#define ASCII_51	KEY_Q + SHIFT_MASK			// 81 Q
#define ASCII_52	KEY_R + SHIFT_MASK			// 82 R
#define ASCII_53	KEY_S + SHIFT_MASK			// 83 S
#define ASCII_54	KEY_T + SHIFT_MASK			// 84 T
#define ASCII_55	KEY_U + SHIFT_MASK			// 85 U
#define ASCII_56	KEY_V + SHIFT_MASK			// 86 V
#define ASCII_57	KEY_W + SHIFT_MASK			// 87 W
#define ASCII_58	KEY_X + SHIFT_MASK			// 88 X
#define ASCII_59	KEY_Y + SHIFT_MASK			// 89 Y
#define ASCII_5A	KEY_Z + SHIFT_MASK			// 90 Z
#define ASCII_5B	KEY_8 + ALTGR_MASK			// 91 [
#define ASCII_5C	KEY_TILDE + ALTGR_MASK			// 92 
#define ASCII_5D	KEY_9 + ALTGR_MASK			// 93 ]
#define ASCII_5E	CIRCUMFLEX_BITS + KEY_SPACE		// 94 ^
#define ASCII_5F	KEY_SLASH + SHIFT_MASK			// 95 _
#define ASCII_60	GRAVE_ACCENT_BITS + KEY_SPACE		// 96 `
#define ASCII_61	KEY_A					// 97 a
#define ASCII_62	KEY_B					// 98 b
#define ASCII_63	KEY_C					// 99 c
#define ASCII_64	KEY_D					// 100 d
#define ASCII_65	KEY_E					// 101 e
#define ASCII_66	KEY_F					// 102 f
#define ASCII_67	KEY_G					// 103 g
#define ASCII_68	KEY_H					// 104 h
#define ASCII_69	KEY_I					// 105 i
#define ASCII_6A	KEY_J					// 106 j
#define ASCII_6B	KEY_K					// 107 k
#define ASCII_6C	KEY_L					// 108 l
#define ASCII_6D	KEY_M					// 109 m
#define ASCII_6E	KEY_N					// 110 n
#define ASCII_6F	KEY_O					// 111 o
#define ASCII_70	KEY_P					// 112 p
#define ASCII_71	KEY_Q					// 113 q
#define ASCII_72	KEY_R					// 114 r
#define ASCII_73	KEY_S					// 115 s
#define ASCII_74	KEY_T					// 116 t
#define ASCII_75	KEY_U					// 117 u
#define ASCII_76	KEY_V					// 118 v
#define ASCII_77	KEY_W					// 119 w
#define ASCII_78	KEY_X					// 120 x
#define ASCII_79	KEY_Y					// 121 y
#define ASCII_7A	KEY_Z					// 122 z
#define ASCII_7B	KEY_7 + ALTGR_MASK			// 123 {
#define ASCII_7C	KEY_TILDE + SHIFT_MASK			// 124 |
#define ASCII_7D	KEY_0 + ALTGR_MASK			// 125 }
#define ASCII_7E	TILDE_BITS + KEY_SPACE			// 126 ~
#define ASCII_7F	KEY_BACKSPACE				// 127

#define ISO_8859_1_A0	KEY_SPACE				// 160       Nonbreakng Space
#define ISO_8859_1_A1	0					// 161 ¡     Inverted Exclamation
#define ISO_8859_1_A2	0					// 162 ¢     Cent SIGN
#define ISO_8859_1_A3	KEY_3 + ALTGR_MASK			// 163 £     Pound Sign
#define ISO_8859_1_A4	0					// 164 ¤     Currency Sign
#define ISO_8859_1_A5	0					// 165 ¥     YEN SIGN
#define ISO_8859_1_A6	0					// 166 ¦     BROKEN BAR
#define ISO_8859_1_A7	KEY_4 + ALTGR_MASK			// 167 §     SECTION SIGN
#define ISO_8859_1_A8	DIAERESIS_BITS + KEY_SPACE		// 168 ¨     DIAERESIS
#define ISO_8859_1_A9	0					// 169 ©     COPYRIGHT SIGN
#define ISO_8859_1_AA	KEY_QUOTE + SHIFT_MASK			// 170 ª     FEMININE ORDINAL
#define ISO_8859_1_AB	KEY_EQUAL				// 171 «     LEFT DOUBLE ANGLE QUOTE
#define ISO_8859_1_AC	0					// 172 ¬     NOT SIGN
#define ISO_8859_1_AD	0					// 173       SOFT HYPHEN
#define ISO_8859_1_AE	0					// 174 ®     REGISTERED SIGN
#define ISO_8859_1_AF	0					// 175 ¯     MACRON
#define ISO_8859_1_B0	0					// 176 °     DEGREE SIGN
#define ISO_8859_1_B1	0					// 177 ±     PLUS-MINUS SIGN
#define ISO_8859_1_B2	0					// 178 ²     SUPERSCRIPT TWO
#define ISO_8859_1_B3	0					// 179 ³     SUPERSCRIPT THREE
#define ISO_8859_1_B4	ACUTE_ACCENT_BITS + KEY_SPACE		// 180 ´     ACUTE ACCENT
#define ISO_8859_1_B5	0					// 181 µ     MICRO SIGN
#define ISO_8859_1_B6	0					// 182 ¶     PILCROW SIGN
#define ISO_8859_1_B7	0					// 183 ·     MIDDLE DOT
#define ISO_8859_1_B8	0					// 184 ¸     CEDILLA
#define ISO_8859_1_B9	0					// 185 ¹     SUPERSCRIPT ONE
#define ISO_8859_1_BA	KEY_QUOTE				// 186 º     MASCULINE ORDINAL
#define ISO_8859_1_BB	KEY_EQUAL + SHIFT_MASK			// 187 »     RIGHT DOUBLE ANGLE QUOTE
#define ISO_8859_1_BC	0					// 188 ¼     FRACTION ONE QUARTER
#define ISO_8859_1_BD	0					// 189 ½     FRACTION ONE HALF
#define ISO_8859_1_BE	0					// 190 ¾     FRACTION THREE QUARTERS
#define ISO_8859_1_BF	0					// 191 ¿     INVERTED QUESTION MARK
#define ISO_8859_1_C0	GRAVE_ACCENT_BITS + KEY_A + SHIFT_MASK	// 192 À     A GRAVE
#define ISO_8859_1_C1	ACUTE_ACCENT_BITS + KEY_A + SHIFT_MASK	// 193 Á     A ACUTE
#define ISO_8859_1_C2	CIRCUMFLEX_BITS + KEY_A + SHIFT_MASK	// 194 Â     A CIRCUMFLEX
#define ISO_8859_1_C3	TILDE_BITS + KEY_A + SHIFT_MASK		// 195 Ã     A TILDE
#define ISO_8859_1_C4	DIAERESIS_BITS + KEY_A + SHIFT_MASK	// 196 Ä     A DIAERESIS
#define ISO_8859_1_C5	0					// 197 Å     A RING ABOVE
#define ISO_8859_1_C6	0					// 198 Æ     AE
#define ISO_8859_1_C7	KEY_SEMICOLON + SHIFT_MASK		// 199 Ç     C CEDILLA
#define ISO_8859_1_C8	GRAVE_ACCENT_BITS + KEY_E + SHIFT_MASK	// 200 È     E GRAVE
#define ISO_8859_1_C9	ACUTE_ACCENT_BITS + KEY_E + SHIFT_MASK	// 201 É     E ACUTE
#define ISO_8859_1_CA	CIRCUMFLEX_BITS + KEY_E + SHIFT_MASK	// 202 Ê     E CIRCUMFLEX
#define ISO_8859_1_CB	DIAERESIS_BITS + KEY_E + SHIFT_MASK	// 203 Ë     E DIAERESIS
#define ISO_8859_1_CC	GRAVE_ACCENT_BITS + KEY_I + SHIFT_MASK	// 204 Ì     I GRAVE
#define ISO_8859_1_CD	ACUTE_ACCENT_BITS + KEY_I + SHIFT_MASK	// 205 Í     I ACUTE
#define ISO_8859_1_CE	CIRCUMFLEX_BITS + KEY_I + SHIFT_MASK	// 206 Î     I CIRCUMFLEX
#define ISO_8859_1_CF	DIAERESIS_BITS + KEY_I + SHIFT_MASK	// 207 Ï     I DIAERESIS
#define ISO_8859_1_D0	0					// 208 Ð     ETH
#define ISO_8859_1_D1	TILDE_BITS + KEY_N + SHIFT_MASK		// 209 Ñ     N TILDE
#define ISO_8859_1_D2	GRAVE_ACCENT_BITS + KEY_O + SHIFT_MASK	// 210 Ò     O GRAVE
#define ISO_8859_1_D3	ACUTE_ACCENT_BITS + KEY_O + SHIFT_MASK	// 211 Ó     O ACUTE
#define ISO_8859_1_D4	CIRCUMFLEX_BITS + KEY_O + SHIFT_MASK	// 212 Ô     O CIRCUMFLEX
#define ISO_8859_1_D5	TILDE_BITS + KEY_O + SHIFT_MASK		// 213 Õ     O TILDE
#define ISO_8859_1_D6	DIAERESIS_BITS + KEY_O + SHIFT_MASK	// 214 Ö     O DIAERESIS
#define ISO_8859_1_D7	0					// 215 ×     MULTIPLICATION
#define ISO_8859_1_D8	0					// 216 Ø     O STROKE
#define ISO_8859_1_D9	GRAVE_ACCENT_BITS + KEY_U + SHIFT_MASK	// 217 Ù     U GRAVE
#define ISO_8859_1_DA	ACUTE_ACCENT_BITS + KEY_U + SHIFT_MASK	// 218 Ú     U ACUTE
#define ISO_8859_1_DB	CIRCUMFLEX_BITS + KEY_U + SHIFT_MASK	// 219 Û     U CIRCUMFLEX
#define ISO_8859_1_DC	DIAERESIS_BITS + KEY_U + SHIFT_MASK	// 220 Ü     U DIAERESIS
#define ISO_8859_1_DD	ACUTE_ACCENT_BITS + KEY_Y + SHIFT_MASK	// 221 Ý     Y ACUTE
#define ISO_8859_1_DE	0					// 222 Þ     THORN
#define ISO_8859_1_DF	0					// 223 ß     SHARP S
#define ISO_8859_1_E0	GRAVE_ACCENT_BITS + KEY_A		// 224 à     a GRAVE
#define ISO_8859_1_E1	ACUTE_ACCENT_BITS + KEY_A		// 225 á     a ACUTE
#define ISO_8859_1_E2	CIRCUMFLEX_BITS + KEY_A			// 226 â     a CIRCUMFLEX
#define ISO_8859_1_E3	TILDE_BITS + KEY_A			// 227 ã     a TILDE
#define ISO_8859_1_E4	DIAERESIS_BITS + KEY_A			// 228 ä     a DIAERESIS
#define ISO_8859_1_E5	0					// 229 å     a RING ABOVE
#define ISO_8859_1_E6	0					// 230 æ     ae
#define ISO_8859_1_E7	KEY_SEMICOLON				// 231 ç     c CEDILLA
#define ISO_8859_1_E8	GRAVE_ACCENT_BITS + KEY_E		// 232 è     e GRAVE
#define ISO_8859_1_E9	ACUTE_ACCENT_BITS + KEY_E		// 233 é     e ACUTE
#define ISO_8859_1_EA	CIRCUMFLEX_BITS + KEY_E			// 234 ê     e CIRCUMFLEX
#define ISO_8859_1_EB	DIAERESIS_BITS + KEY_E			// 235 ë     e DIAERESIS
#define ISO_8859_1_EC	GRAVE_ACCENT_BITS + KEY_I		// 236 ì     i GRAVE
#define ISO_8859_1_ED	ACUTE_ACCENT_BITS + KEY_I		// 237 í     i ACUTE
#define ISO_8859_1_EE	CIRCUMFLEX_BITS + KEY_I			// 238 î     i CIRCUMFLEX
#define ISO_8859_1_EF	DIAERESIS_BITS + KEY_I			// 239 ï     i DIAERESIS
#define ISO_8859_1_F0	0					// 240 ð     ETH
#define ISO_8859_1_F1	TILDE_BITS + KEY_N			// 241 ñ     n TILDE
#define ISO_8859_1_F2	GRAVE_ACCENT_BITS + KEY_O		// 242 ò     o GRAVE
#define ISO_8859_1_F3	ACUTE_ACCENT_BITS + KEY_O		// 243 ó     o ACUTE
#define ISO_8859_1_F4	CIRCUMFLEX_BITS + KEY_O			// 244 ô     o CIRCUMFLEX
#define ISO_8859_1_F5	TILDE_BITS + KEY_O			// 245 õ     o TILDE
#define ISO_8859_1_F6	DIAERESIS_BITS + KEY_O			// 246 ö     o DIAERESIS
#define ISO_8859_1_F7	0					// 247 ÷     DIVISION
#define ISO_8859_1_F8	0					// 248 ø     o STROKE
#define ISO_8859_1_F9	GRAVE_ACCENT_BITS + KEY_U		// 249 ù     u GRAVE
#define ISO_8859_1_FA	ACUTE_ACCENT_BITS + KEY_U		// 250 ú     u ACUTE
#define ISO_8859_1_FB	CIRCUMFLEX_BITS + KEY_U			// 251 û     u CIRCUMFLEX
#define ISO_8859_1_FC	DIAERESIS_BITS + KEY_U			// 252 ü     u DIAERESIS
#define ISO_8859_1_FD	ACUTE_ACCENT_BITS + KEY_Y		// 253 ý     y ACUTE
#define ISO_8859_1_FE	0					// 254 þ     THORN
#define ISO_8859_1_FF	DIAERESIS_BITS + KEY_Y			// 255 ÿ     y DIAERESIS
#define UNICODE_20AC	KEY_E + ALTGR_MASK			//     €     Euro Sign
// not yet implemented
#define UNICODE_EXTRA00	0x20AC
#define KEYCODE_EXTRA00	KEY_E + ALTGR_MASK			// 20AC €    Euro Sign

#endif // LAYOUT_PORTUGUESE







#ifdef LAYOUT_ITALIAN

#define SHIFT_MASK		0x40
#define ALTGR_MASK		0x80
#define KEYCODE_TYPE		uint8_t
#define KEY_NON_US_100		63

#define ASCII_20	KEY_SPACE				// 32  
#define ASCII_21	KEY_1 + SHIFT_MASK			// 33 !
#define ASCII_22	KEY_2 + SHIFT_MASK			// 34 "
#define ASCII_23	KEY_QUOTE + ALTGR_MASK			// 35 #
#define ASCII_24	KEY_4 + SHIFT_MASK			// 36 $
#define ASCII_25	KEY_5 + SHIFT_MASK			// 37 %
#define ASCII_26	KEY_6 + SHIFT_MASK			// 38 &
#define ASCII_27	KEY_MINUS				// 39 '  
#define ASCII_28	KEY_8 + SHIFT_MASK			// 40 ( 
#define ASCII_29	KEY_9 + SHIFT_MASK			// 41 )
#define ASCII_2A	KEY_RIGHT_BRACE + SHIFT_MASK		// 42 *
#define ASCII_2B	KEY_RIGHT_BRACE				// 43 +
#define ASCII_2C	KEY_COMMA				// 44 ,
#define ASCII_2D	KEY_SLASH				// 45 -
#define ASCII_2E	KEY_PERIOD				// 46 .
#define ASCII_2F	KEY_7 + SHIFT_MASK			// 47 /
#define ASCII_30	KEY_0					// 48 0
#define ASCII_31	KEY_1					// 49 1
#define ASCII_32	KEY_2					// 50 2
#define ASCII_33	KEY_3					// 51 3
#define ASCII_34	KEY_4					// 52 4
#define ASCII_35	KEY_5					// 53 5
#define ASCII_36	KEY_6					// 54 6
#define ASCII_37	KEY_7					// 55 7
#define ASCII_38	KEY_8					// 55 8
#define ASCII_39	KEY_9					// 57 9
#define ASCII_3A	KEY_PERIOD + SHIFT_MASK			// 58 :
#define ASCII_3B	KEY_COMMA + SHIFT_MASK			// 59 ;
#define ASCII_3C	KEY_NON_US_100				// 60 <
#define ASCII_3D	KEY_0 + SHIFT_MASK			// 61 =
#define ASCII_3E	KEY_NON_US_100 + SHIFT_MASK		// 62 >
#define ASCII_3F	KEY_MINUS + SHIFT_MASK			// 63 ?
#define ASCII_40	KEY_SEMICOLON + ALTGR_MASK		// 64 @
#define ASCII_41	KEY_A + SHIFT_MASK			// 65 A
#define ASCII_42	KEY_B + SHIFT_MASK			// 66 B
#define ASCII_43	KEY_C + SHIFT_MASK			// 67 C
#define ASCII_44	KEY_D + SHIFT_MASK			// 68 D
#define ASCII_45	KEY_E + SHIFT_MASK			// 69 E
#define ASCII_46	KEY_F + SHIFT_MASK			// 70 F
#define ASCII_47	KEY_G + SHIFT_MASK			// 71 G
#define ASCII_48	KEY_H + SHIFT_MASK			// 72 H
#define ASCII_49	KEY_I + SHIFT_MASK			// 73 I
#define ASCII_4A	KEY_J + SHIFT_MASK			// 74 J
#define ASCII_4B	KEY_K + SHIFT_MASK			// 75 K
#define ASCII_4C	KEY_L + SHIFT_MASK			// 76 L
#define ASCII_4D	KEY_M + SHIFT_MASK			// 77 M
#define ASCII_4E	KEY_N + SHIFT_MASK			// 78 N
#define ASCII_4F	KEY_O + SHIFT_MASK			// 79 O
#define ASCII_50	KEY_P + SHIFT_MASK			// 80 P
#define ASCII_51	KEY_Q + SHIFT_MASK			// 81 Q
#define ASCII_52	KEY_R + SHIFT_MASK			// 82 R
#define ASCII_53	KEY_S + SHIFT_MASK			// 83 S
#define ASCII_54	KEY_T + SHIFT_MASK			// 84 T
#define ASCII_55	KEY_U + SHIFT_MASK			// 85 U
#define ASCII_56	KEY_V + SHIFT_MASK			// 86 V
#define ASCII_57	KEY_W + SHIFT_MASK			// 87 W
#define ASCII_58	KEY_X + SHIFT_MASK			// 88 X
#define ASCII_59	KEY_Y + SHIFT_MASK			// 89 Y
#define ASCII_5A	KEY_Z + SHIFT_MASK			// 90 Z
#define ASCII_5B	KEY_LEFT_BRACE + ALTGR_MASK		// 91 [
#define ASCII_5C	KEY_TILDE				// 92 
#define ASCII_5D	KEY_RIGHT_BRACE + ALTGR_MASK		// 93 ]
#define ASCII_5E	KEY_EQUAL + SHIFT_MASK			// 94 ^
#define ASCII_5F	KEY_SLASH + SHIFT_MASK			// 95 _
#define ASCII_60	0					// 96 `  (how to type this on Italian?)
#define ASCII_61	KEY_A					// 97 a
#define ASCII_62	KEY_B					// 98 b
#define ASCII_63	KEY_C					// 99 c
#define ASCII_64	KEY_D					// 100 d
#define ASCII_65	KEY_E					// 101 e
#define ASCII_66	KEY_F					// 102 f
#define ASCII_67	KEY_G					// 103 g
#define ASCII_68	KEY_H					// 104 h
#define ASCII_69	KEY_I					// 105 i
#define ASCII_6A	KEY_J					// 106 j
#define ASCII_6B	KEY_K					// 107 k
#define ASCII_6C	KEY_L					// 108 l
#define ASCII_6D	KEY_M					// 109 m
#define ASCII_6E	KEY_N					// 110 n
#define ASCII_6F	KEY_O					// 111 o
#define ASCII_70	KEY_P					// 112 p
#define ASCII_71	KEY_Q					// 113 q
#define ASCII_72	KEY_R					// 114 r
#define ASCII_73	KEY_S					// 115 s
#define ASCII_74	KEY_T					// 116 t
#define ASCII_75	KEY_U					// 117 u
#define ASCII_76	KEY_V					// 118 v
#define ASCII_77	KEY_W					// 119 w
#define ASCII_78	KEY_X					// 120 x
#define ASCII_79	KEY_Y					// 121 y
#define ASCII_7A	KEY_Z					// 122 z
#define ASCII_7B	KEY_LEFT_BRACE + SHIFT_MASK + ALTGR_MASK // 123 {
#define ASCII_7C	KEY_TILDE + SHIFT_MASK			// 124 |
#define ASCII_7D	KEY_RIGHT_BRACE + SHIFT_MASK + ALTGR_MASK // 125 }
#define ASCII_7E	0					// 126 ~   (how to type this on Italian?)
#define ASCII_7F	KEY_BACKSPACE				// 127

#define ISO_8859_1_A0	KEY_SPACE				// 160       Nonbreakng Space
#define ISO_8859_1_A1	0					// 161 ¡     Inverted Exclamation
#define ISO_8859_1_A2	0					// 162 ¢     Cent Sign
#define ISO_8859_1_A3	KEY_3 + SHIFT_MASK			// 163 £     Pound Sign
#define ISO_8859_1_A4	0					// 164 ¤     Currency Sign
#define ISO_8859_1_A5	0					// 165 ¥     YEN SIGN
#define ISO_8859_1_A6	0					// 166 ¦     BROKEN BAR
#define ISO_8859_1_A7	KEY_BACKSLASH + SHIFT_MASK		// 167 §     SECTION SIGN
#define ISO_8859_1_A8	0					// 168 ¨     DIAERESIS
#define ISO_8859_1_A9	0					// 169 ©     COPYRIGHT SIGN
#define ISO_8859_1_AA	0					// 170 ª     FEMININE ORDINAL
#define ISO_8859_1_AB	0					// 171 «     LEFT DOUBLE ANGLE QUOTE
#define ISO_8859_1_AC	0					// 172 ¬     NOT SIGN
#define ISO_8859_1_AD	0					// 173       SOFT HYPHEN
#define ISO_8859_1_AE	0					// 174 ®     REGISTERED SIGN
#define ISO_8859_1_AF	0					// 175 ¯     MACRON
#define ISO_8859_1_B0	KEY_QUOTE + SHIFT_MASK			// 176 °     DEGREE SIGN
#define ISO_8859_1_B1	0					// 177 ±     PLUS-MINUS SIGN
#define ISO_8859_1_B2	0					// 178 ²     SUPERSCRIPT TWO
#define ISO_8859_1_B3	0					// 179 ³     SUPERSCRIPT THREE
#define ISO_8859_1_B4	0					// 180 ´     ACUTE ACCENT
#define ISO_8859_1_B5	0					// 181 µ     MICRO SIGN
#define ISO_8859_1_B6	0					// 182 ¶     PILCROW SIGN
#define ISO_8859_1_B7	0					// 183 ·     MIDDLE DOT
#define ISO_8859_1_B8	0					// 184 ¸     CEDILLA
#define ISO_8859_1_B9	0					// 185 ¹     SUPERSCRIPT ONE
#define ISO_8859_1_BA	0					// 186 º     MASCULINE ORDINAL
#define ISO_8859_1_BB	0					// 187 »     RIGHT DOUBLE ANGLE QUOTE
#define ISO_8859_1_BC	0					// 188 ¼     FRACTION ONE QUARTER
#define ISO_8859_1_BD	0					// 189 ½     FRACTION ONE HALF
#define ISO_8859_1_BE	0					// 190 ¾     FRACTION THREE QUARTERS
#define ISO_8859_1_BF	0					// 191 ¿     INVERTED QUESTION MARK
#define ISO_8859_1_C0	0					// 192 À     A GRAVE
#define ISO_8859_1_C1	0					// 193 Á     A ACUTE
#define ISO_8859_1_C2	0					// 194 Â     A CIRCUMFLEX
#define ISO_8859_1_C3	0					// 195 Ã     A TILDE
#define ISO_8859_1_C4	0					// 196 Ä     A DIAERESIS
#define ISO_8859_1_C5	0					// 197 Å     A RING ABOVE
#define ISO_8859_1_C6	0					// 198 Æ     AE
#define ISO_8859_1_C7	0					// 199 Ç     C CEDILLA
#define ISO_8859_1_C8	0					// 200 È     E GRAVE
#define ISO_8859_1_C9	0					// 201 É     E ACUTE
#define ISO_8859_1_CA	0					// 202 Ê     E CIRCUMFLEX
#define ISO_8859_1_CB	0					// 203 Ë     E DIAERESIS
#define ISO_8859_1_CC	0					// 204 Ì     I GRAVE
#define ISO_8859_1_CD	0					// 205 Í     I ACUTE
#define ISO_8859_1_CE	0					// 206 Î     I CIRCUMFLEX
#define ISO_8859_1_CF	0					// 207 Ï     I DIAERESIS
#define ISO_8859_1_D0	0					// 208 Ð     ETH
#define ISO_8859_1_D1	0					// 209 Ñ     N TILDE
#define ISO_8859_1_D2	0					// 210 Ò     O GRAVE
#define ISO_8859_1_D3	0					// 211 Ó     O ACUTE
#define ISO_8859_1_D4	0					// 212 Ô     O CIRCUMFLEX
#define ISO_8859_1_D5	0					// 213 Õ     O TILDE
#define ISO_8859_1_D6	0					// 214 Ö     O DIAERESIS
#define ISO_8859_1_D7	0					// 215 ×     MULTIPLICATION
#define ISO_8859_1_D8	0					// 216 Ø     O STROKE
#define ISO_8859_1_D9	0					// 217 Ù     U GRAVE
#define ISO_8859_1_DA	0					// 218 Ú     U ACUTE
#define ISO_8859_1_DB	0					// 219 Û     U CIRCUMFLEX
#define ISO_8859_1_DC	0					// 220 Ü     U DIAERESIS
#define ISO_8859_1_DD	0					// 221 Ý     Y ACUTE
#define ISO_8859_1_DE	0					// 222 Þ     THORN
#define ISO_8859_1_DF	0					// 223 ß     SHARP S
#define ISO_8859_1_E0	KEY_QUOTE				// 224 à     a GRAVE
#define ISO_8859_1_E1	0					// 225 á     a ACUTE
#define ISO_8859_1_E2	0					// 226 â     a CIRCUMFLEX
#define ISO_8859_1_E3	0					// 227 ã     a TILDE
#define ISO_8859_1_E4	0					// 228 ä     a DIAERESIS
#define ISO_8859_1_E5	0					// 229 å     a RING ABOVE
#define ISO_8859_1_E6	0					// 230 æ     ae
#define ISO_8859_1_E7	KEY_SEMICOLON + SHIFT_MASK		// 231 ç     c CEDILLA
#define ISO_8859_1_E8	KEY_LEFT_BRACE				// 232 è     e GRAVE
#define ISO_8859_1_E9	KEY_LEFT_BRACE + SHIFT_MASK		// 233 é     e ACUTE
#define ISO_8859_1_EA	0					// 234 ê     e CIRCUMFLEX
#define ISO_8859_1_EB	0					// 235 ë     e DIAERESIS
#define ISO_8859_1_EC	KEY_EQUAL				// 236 ì     i GRAVE
#define ISO_8859_1_ED	0					// 237 í     i ACUTE
#define ISO_8859_1_EE	0					// 238 î     i CIRCUMFLEX
#define ISO_8859_1_EF	0					// 239 ï     i DIAERESIS
#define ISO_8859_1_F0	0					// 240 ð     ETH
#define ISO_8859_1_F1	0					// 241 ñ     n TILDE
#define ISO_8859_1_F2	KEY_SEMICOLON				// 242 ò     o GRAVE
#define ISO_8859_1_F3	0					// 243 ó     o ACUTE
#define ISO_8859_1_F4	0					// 244 ô     o CIRCUMFLEX
#define ISO_8859_1_F5	0					// 245 õ     o TILDE
#define ISO_8859_1_F6	0					// 246 ö     o DIAERESIS
#define ISO_8859_1_F7	0					// 247 ÷     DIVISION
#define ISO_8859_1_F8	0					// 248 ø     o STROKE
#define ISO_8859_1_F9	KEY_BACKSLASH				// 249 ù     u GRAVE
#define ISO_8859_1_FA	0					// 250 ú     u ACUTE
#define ISO_8859_1_FB	0					// 251 û     u CIRCUMFLEX
#define ISO_8859_1_FC	0					// 252 ü     u DIAERESIS
#define ISO_8859_1_FD	0					// 253 ý     y ACUTE
#define ISO_8859_1_FE	0					// 254 þ     THORN
#define ISO_8859_1_FF	0					// 255 ÿ     y DIAERESIS

#endif // LAYOUT_ITALIAN




#ifdef LAYOUT_PORTUGUESE_BRAZILIAN

#define SHIFT_MASK		0x0040
#define ALTGR_MASK		0x0080
#define DEADKEYS_MASK		0x0700
#define	CIRCUMFLEX_BITS		0x0100
#define ACUTE_ACCENT_BITS	0x0200
#define GRAVE_ACCENT_BITS	0x0300
#define TILDE_BITS		0x0400
#define DIAERESIS_BITS		0x0500
#define KEYCODE_TYPE		uint16_t
#define DEADKEY_CIRCUMFLEX	KEY_QUOTE + SHIFT_MASK
#define DEADKEY_ACUTE_ACCENT	KEY_LEFT_BRACE
#define DEADKEY_GRAVE_ACCENT	KEY_LEFT_BRACE + SHIFT_MASK
#define DEADKEY_TILDE		KEY_QUOTE
#define DEADKEY_DIAERESIS	KEY_6 + SHIFT_MASK
#define KEY_NON_US_100		63

#define ASCII_20	KEY_SPACE				// 32  
#define ASCII_21	KEY_1 + SHIFT_MASK			// 33 !
#define ASCII_22	KEY_TILDE + SHIFT_MASK			// 34 "
#define ASCII_23	KEY_3 + ALTGR_MASK			// 35 #
#define ASCII_24	KEY_4 + SHIFT_MASK			// 36 $
#define ASCII_25	KEY_5 + SHIFT_MASK			// 37 %
#define ASCII_26	KEY_7 + SHIFT_MASK			// 38 &
#define ASCII_27	KEY_TILDE				// 39 '
#define ASCII_28	KEY_9 + SHIFT_MASK			// 40 (
#define ASCII_29	KEY_0 + SHIFT_MASK			// 41 )
#define ASCII_2A	KEY_7 + SHIFT_MASK			// 42 *
#define ASCII_2B	KEY_EQUAL + SHIFT_MASK			// 43 +
#define ASCII_2C	KEY_COMMA				// 44 ,
#define ASCII_2D	KEY_MINUS				// 45 -
#define ASCII_2E	KEY_PERIOD				// 46 .
#define ASCII_2F	KEY_Q + ALTGR_MASK			// 47 /
#define ASCII_30	KEY_0					// 48 0
#define ASCII_31	KEY_1					// 49 1
#define ASCII_32	KEY_2					// 50 2
#define ASCII_33	KEY_3					// 51 3
#define ASCII_34	KEY_4					// 52 4
#define ASCII_35	KEY_5					// 53 5
#define ASCII_36	KEY_6					// 54 6
#define ASCII_37	KEY_7					// 55 7
#define ASCII_38	KEY_8					// 55 8
#define ASCII_39	KEY_9					// 57 9
#define ASCII_3A	KEY_SLASH + SHIFT_MASK			// 58 :
#define ASCII_3B	KEY_SLASH				// 59 ;
#define ASCII_3C	KEY_COMMA + SHIFT_MASK			// 60 <
#define ASCII_3D	KEY_EQUAL				// 61 =
#define ASCII_3E	KEY_PERIOD + SHIFT_MASK			// 62 >
#define ASCII_3F	KEY_W + ALTGR_MASK			// 63 ?
#define ASCII_40	KEY_2 + SHIFT_MASK			// 64 @
#define ASCII_41	KEY_A + SHIFT_MASK			// 65 A
#define ASCII_42	KEY_B + SHIFT_MASK			// 66 B
#define ASCII_43	KEY_C + SHIFT_MASK			// 67 C
#define ASCII_44	KEY_D + SHIFT_MASK			// 68 D
#define ASCII_45	KEY_E + SHIFT_MASK			// 69 E
#define ASCII_46	KEY_F + SHIFT_MASK			// 70 F
#define ASCII_47	KEY_G + SHIFT_MASK			// 71 G
#define ASCII_48	KEY_H + SHIFT_MASK			// 72 H
#define ASCII_49	KEY_I + SHIFT_MASK			// 73 I
#define ASCII_4A	KEY_J + SHIFT_MASK			// 74 J
#define ASCII_4B	KEY_K + SHIFT_MASK			// 75 K
#define ASCII_4C	KEY_L + SHIFT_MASK			// 76 L
#define ASCII_4D	KEY_M + SHIFT_MASK			// 77 M
#define ASCII_4E	KEY_N + SHIFT_MASK			// 78 N
#define ASCII_4F	KEY_O + SHIFT_MASK			// 79 O
#define ASCII_50	KEY_P + SHIFT_MASK			// 80 P
#define ASCII_51	KEY_Q + SHIFT_MASK			// 81 Q
#define ASCII_52	KEY_R + SHIFT_MASK			// 82 R
#define ASCII_53	KEY_S + SHIFT_MASK			// 83 S
#define ASCII_54	KEY_T + SHIFT_MASK			// 84 T
#define ASCII_55	KEY_U + SHIFT_MASK			// 85 U
#define ASCII_56	KEY_V + SHIFT_MASK			// 86 V
#define ASCII_57	KEY_W + SHIFT_MASK			// 87 W
#define ASCII_58	KEY_X + SHIFT_MASK			// 88 X
#define ASCII_59	KEY_Y + SHIFT_MASK			// 89 Y
#define ASCII_5A	KEY_Z + SHIFT_MASK			// 90 Z
#define ASCII_5B	KEY_RIGHT_BRACE				// 91 [
#define ASCII_5C	KEY_NON_US_100				// 92
#define ASCII_5D	KEY_BACKSLASH				// 93 ]
#define ASCII_5E	CIRCUMFLEX_BITS + KEY_SPACE		// 94 ^
#define ASCII_5F	KEY_MINUS + SHIFT_MASK			// 95 _
#define ASCII_60	GRAVE_ACCENT_BITS + KEY_SPACE		// 96 `
#define ASCII_61	KEY_A					// 97 a
#define ASCII_62	KEY_B					// 98 b
#define ASCII_63	KEY_C					// 99 c
#define ASCII_64	KEY_D					// 100 d
#define ASCII_65	KEY_E					// 101 e
#define ASCII_66	KEY_F					// 102 f
#define ASCII_67	KEY_G					// 103 g
#define ASCII_68	KEY_H					// 104 h
#define ASCII_69	KEY_I					// 105 i
#define ASCII_6A	KEY_J					// 106 j
#define ASCII_6B	KEY_K					// 107 k
#define ASCII_6C	KEY_L					// 108 l
#define ASCII_6D	KEY_M					// 109 m
#define ASCII_6E	KEY_N					// 110 n
#define ASCII_6F	KEY_O					// 111 o
#define ASCII_70	KEY_P					// 112 p
#define ASCII_71	KEY_Q					// 113 q
#define ASCII_72	KEY_R					// 114 r
#define ASCII_73	KEY_S					// 115 s
#define ASCII_74	KEY_T					// 116 t
#define ASCII_75	KEY_U					// 117 u
#define ASCII_76	KEY_V					// 118 v
#define ASCII_77	KEY_W					// 119 w
#define ASCII_78	KEY_X					// 120 x
#define ASCII_79	KEY_Y					// 121 y
#define ASCII_7A	KEY_Z					// 122 z
#define ASCII_7B	KEY_RIGHT_BRACE + SHIFT_MASK		// 123 {
#define ASCII_7C	KEY_NON_US_100 + SHIFT_MASK		// 124 |
#define ASCII_7D	KEY_BACKSLASH + SHIFT_MASK		// 125 }
#define ASCII_7E	TILDE_BITS + KEY_SPACE			// 126 ~
#define ASCII_7F	KEY_BACKSPACE				// 127

#define ISO_8859_1_A0	KEY_SPACE				// 160       Nonbreakng Space
#define ISO_8859_1_A1	0					// 161 ¡     Inverted Exclamation
#define ISO_8859_1_A2	KEY_5 + ALTGR_MASK			// 162 ¢     Cent SIGN
#define ISO_8859_1_A3	KEY_4 + ALTGR_MASK			// 163 £     Pound Sign
#define ISO_8859_1_A4	0					// 164 ¤     Currency Sign
#define ISO_8859_1_A5	0					// 165 ¥     YEN SIGN
#define ISO_8859_1_A6	0					// 166 ¦     BROKEN BAR
#define ISO_8859_1_A7	KEY_EQUAL + ALTGR_MASK			// 167 §     SECTION SIGN
#define ISO_8859_1_A8	DIAERESIS_BITS + KEY_SPACE		// 168 ¨     DIAERESIS
#define ISO_8859_1_A9	0					// 169 ©     COPYRIGHT SIGN
#define ISO_8859_1_AA	KEY_RIGHT_BRACE + ALTGR_MASK		// 170 ª     FEMININE ORDINAL
#define ISO_8859_1_AB	0					// 171 «     LEFT DOUBLE ANGLE QUOTE
#define ISO_8859_1_AC	KEY_6 + ALTGR_MASK			// 172 ¬     NOT SIGN
#define ISO_8859_1_AD	0					// 173       SOFT HYPHEN
#define ISO_8859_1_AE	0					// 174 ®     REGISTERED SIGN
#define ISO_8859_1_AF	0					// 175 ¯     MACRON
#define ISO_8859_1_B0	KEY_E + ALTGR_MASK			// 176 °     DEGREE SIGN
#define ISO_8859_1_B1	0					// 177 ±     PLUS-MINUS SIGN
#define ISO_8859_1_B2	KEY_2 + ALTGR_MASK			// 178 ²     SUPERSCRIPT TWO
#define ISO_8859_1_B3	KEY_2 + ALTGR_MASK			// 179 ³     SUPERSCRIPT THREE
#define ISO_8859_1_B4	ACUTE_ACCENT_BITS + KEY_SPACE		// 180 ´     ACUTE ACCENT
#define ISO_8859_1_B5	0					// 181 µ     MICRO SIGN
#define ISO_8859_1_B6	0					// 182 ¶     PILCROW SIGN
#define ISO_8859_1_B7	0					// 183 ·     MIDDLE DOT
#define ISO_8859_1_B8	0					// 184 ¸     CEDILLA
#define ISO_8859_1_B9	KEY_1 + ALTGR_MASK			// 185 ¹     SUPERSCRIPT ONE
#define ISO_8859_1_BA	KEY_BACKSLASH + ALTGR_MASK		// 186 º     MASCULINE ORDINAL
#define ISO_8859_1_BB	0					// 187 »     RIGHT DOUBLE ANGLE QUOTE
#define ISO_8859_1_BC	0					// 188 ¼     FRACTION ONE QUARTER
#define ISO_8859_1_BD	0					// 189 ½     FRACTION ONE HALF
#define ISO_8859_1_BE	0					// 190 ¾     FRACTION THREE QUARTERS
#define ISO_8859_1_BF	0					// 191 ¿     INVERTED QUESTION MARK
#define ISO_8859_1_C0	GRAVE_ACCENT_BITS + KEY_A + SHIFT_MASK	// 192 À     A GRAVE
#define ISO_8859_1_C1	ACUTE_ACCENT_BITS + KEY_A + SHIFT_MASK	// 193 Á     A ACUTE
#define ISO_8859_1_C2	CIRCUMFLEX_BITS + KEY_A + SHIFT_MASK	// 194 Â     A CIRCUMFLEX
#define ISO_8859_1_C3	TILDE_BITS + KEY_A + SHIFT_MASK		// 195 Ã     A TILDE
#define ISO_8859_1_C4	DIAERESIS_BITS + KEY_A + SHIFT_MASK	// 196 Ä     A DIAERESIS
#define ISO_8859_1_C5	0					// 197 Å     A RING ABOVE
#define ISO_8859_1_C6	0					// 198 Æ     AE
#define ISO_8859_1_C7	0					// 199 Ç     C CEDILLA
#define ISO_8859_1_C8	GRAVE_ACCENT_BITS + KEY_E + SHIFT_MASK	// 200 È     E GRAVE
#define ISO_8859_1_C9	ACUTE_ACCENT_BITS + KEY_E + SHIFT_MASK	// 201 É     E ACUTE
#define ISO_8859_1_CA	CIRCUMFLEX_BITS + KEY_E + SHIFT_MASK	// 202 Ê     E CIRCUMFLEX
#define ISO_8859_1_CB	DIAERESIS_BITS + KEY_E + SHIFT_MASK	// 203 Ë     E DIAERESIS
#define ISO_8859_1_CC	GRAVE_ACCENT_BITS + KEY_I + SHIFT_MASK	// 204 Ì     I GRAVE
#define ISO_8859_1_CD	ACUTE_ACCENT_BITS + KEY_I + SHIFT_MASK	// 205 Í     I ACUTE
#define ISO_8859_1_CE	CIRCUMFLEX_BITS + KEY_I + SHIFT_MASK	// 206 Î     I CIRCUMFLEX
#define ISO_8859_1_CF	DIAERESIS_BITS + KEY_I + SHIFT_MASK	// 207 Ï     I DIAERESIS
#define ISO_8859_1_D0	0					// 208 Ð     ETH
#define ISO_8859_1_D1	TILDE_BITS + KEY_N + SHIFT_MASK		// 209 Ñ     N TILDE
#define ISO_8859_1_D2	GRAVE_ACCENT_BITS + KEY_O + SHIFT_MASK	// 210 Ò     O GRAVE
#define ISO_8859_1_D3	ACUTE_ACCENT_BITS + KEY_O + SHIFT_MASK	// 211 Ó     O ACUTE
#define ISO_8859_1_D4	CIRCUMFLEX_BITS + KEY_O + SHIFT_MASK	// 212 Ô     O CIRCUMFLEX
#define ISO_8859_1_D5	TILDE_BITS + KEY_O + SHIFT_MASK		// 213 Õ     O TILDE
#define ISO_8859_1_D6	DIAERESIS_BITS + KEY_O + SHIFT_MASK	// 214 Ö     O DIAERESIS
#define ISO_8859_1_D7	0					// 215 ×     MULTIPLICATION
#define ISO_8859_1_D8	0					// 216 Ø     O STROKE
#define ISO_8859_1_D9	GRAVE_ACCENT_BITS + KEY_U + SHIFT_MASK	// 217 Ù     U GRAVE
#define ISO_8859_1_DA	ACUTE_ACCENT_BITS + KEY_U + SHIFT_MASK	// 218 Ú     U ACUTE
#define ISO_8859_1_DB	CIRCUMFLEX_BITS + KEY_U + SHIFT_MASK	// 219 Û     U CIRCUMFLEX
#define ISO_8859_1_DC	DIAERESIS_BITS + KEY_U + SHIFT_MASK	// 220 Ü     U DIAERESIS
#define ISO_8859_1_DD	ACUTE_ACCENT_BITS + KEY_Y + SHIFT_MASK	// 221 Ý     Y ACUTE
#define ISO_8859_1_DE	0					// 222 Þ     THORN
#define ISO_8859_1_DF	0					// 223 ß     SHARP S
#define ISO_8859_1_E0	GRAVE_ACCENT_BITS + KEY_A		// 224 à     a GRAVE
#define ISO_8859_1_E1	ACUTE_ACCENT_BITS + KEY_A		// 225 á     a ACUTE
#define ISO_8859_1_E2	CIRCUMFLEX_BITS + KEY_A			// 226 â     a CIRCUMFLEX
#define ISO_8859_1_E3	TILDE_BITS + KEY_A			// 227 ã     a TILDE
#define ISO_8859_1_E4	DIAERESIS_BITS + KEY_A			// 228 ä     a DIAERESIS
#define ISO_8859_1_E5	0					// 229 å     a RING ABOVE
#define ISO_8859_1_E6	0					// 230 æ     ae
#define ISO_8859_1_E7	0					// 231 ç     c CEDILLA
#define ISO_8859_1_E8	GRAVE_ACCENT_BITS + KEY_E		// 232 è     e GRAVE
#define ISO_8859_1_E9	ACUTE_ACCENT_BITS + KEY_E		// 233 é     e ACUTE
#define ISO_8859_1_EA	CIRCUMFLEX_BITS + KEY_E			// 234 ê     e CIRCUMFLEX
#define ISO_8859_1_EB	DIAERESIS_BITS + KEY_E			// 235 ë     e DIAERESIS
#define ISO_8859_1_EC	GRAVE_ACCENT_BITS + KEY_I		// 236 ì     i GRAVE
#define ISO_8859_1_ED	ACUTE_ACCENT_BITS + KEY_I		// 237 í     i ACUTE
#define ISO_8859_1_EE	CIRCUMFLEX_BITS + KEY_I			// 238 î     i CIRCUMFLEX
#define ISO_8859_1_EF	DIAERESIS_BITS + KEY_I			// 239 ï     i DIAERESIS
#define ISO_8859_1_F0	0					// 240 ð     ETH
#define ISO_8859_1_F1	TILDE_BITS + KEY_N			// 241 ñ     n TILDE
#define ISO_8859_1_F2	GRAVE_ACCENT_BITS + KEY_O		// 242 ò     o GRAVE
#define ISO_8859_1_F3	ACUTE_ACCENT_BITS + KEY_O		// 243 ó     o ACUTE
#define ISO_8859_1_F4	CIRCUMFLEX_BITS + KEY_O			// 244 ô     o CIRCUMFLEX
#define ISO_8859_1_F5	TILDE_BITS + KEY_O			// 245 õ     o TILDE
#define ISO_8859_1_F6	DIAERESIS_BITS + KEY_O			// 246 ö     o DIAERESIS
#define ISO_8859_1_F7	0					// 247 ÷     DIVISION
#define ISO_8859_1_F8	0					// 248 ø     o STROKE
#define ISO_8859_1_F9	GRAVE_ACCENT_BITS + KEY_U		// 249 ù     u GRAVE
#define ISO_8859_1_FA	ACUTE_ACCENT_BITS + KEY_U		// 250 ú     u ACUTE
#define ISO_8859_1_FB	CIRCUMFLEX_BITS + KEY_U			// 251 û     u CIRCUMFLEX
#define ISO_8859_1_FC	DIAERESIS_BITS + KEY_U			// 252 ü     u DIAERESIS
#define ISO_8859_1_FD	ACUTE_ACCENT_BITS + KEY_Y		// 253 ý     y ACUTE
#define ISO_8859_1_FE	0					// 254 þ     THORN
#define ISO_8859_1_FF	DIAERESIS_BITS + KEY_Y			// 255 ÿ     y DIAERESIS

#endif // LAYOUT_PORTUGUESE_BRAZILIAN



#ifdef LAYOUT_FRENCH_BELGIAN

#define SHIFT_MASK		0x0040
#define ALTGR_MASK		0x0080
#define DEADKEYS_MASK		0x0700
#define	CIRCUMFLEX_BITS		0x0100
#define GRAVE_ACCENT_BITS	0x0200
#define DIAERESIS_BITS		0x0300
#define TILDE_BITS		0x0400
#define ACUTE_ACCENT_BITS	0x0500
#define KEYCODE_TYPE		uint16_t
#define DEADKEY_CIRCUMFLEX	KEY_LEFT_BRACE
#define DEADKEY_ACUTE_ACCENT	KEY_QUOTE + ALTGR_MASK
#define DEADKEY_GRAVE_ACCENT	KEY_BACKSLASH + ALTGR_MASK
#define DEADKEY_DIAERESIS	KEY_LEFT_BRACE + SHIFT_MASK
#define DEADKEY_TILDE		KEY_SLASH + ALTGR_MASK
#define KEY_NON_US_100		63

#define ASCII_20	KEY_SPACE				// 32  
#define ASCII_21	KEY_8					// 33 !
#define ASCII_22	KEY_3					// 34 "
#define ASCII_23	KEY_3 + ALTGR_MASK			// 35 #
#define ASCII_24	KEY_RIGHT_BRACE				// 36 $
#define ASCII_25	KEY_QUOTE + SHIFT_MASK			// 37 %
#define ASCII_26	KEY_1					// 38 &
#define ASCII_27	KEY_4					// 39 '  
#define ASCII_28	KEY_5					// 40 ( 
#define ASCII_29	KEY_MINUS				// 41 )
#define ASCII_2A	KEY_RIGHT_BRACE + SHIFT_MASK		// 42 *
#define ASCII_2B	KEY_SLASH + SHIFT_MASK			// 43 +
#define ASCII_2C	KEY_M					// 44 ,
#define ASCII_2D	KEY_EQUAL				// 45 -
#define ASCII_2E	KEY_COMMA + SHIFT_MASK			// 46 .
#define ASCII_2F	KEY_PERIOD + SHIFT_MASK			// 47 /
#define ASCII_30	KEY_0 + SHIFT_MASK			// 48 0
#define ASCII_31	KEY_1 + SHIFT_MASK			// 49 1
#define ASCII_32	KEY_2 + SHIFT_MASK			// 50 2
#define ASCII_33	KEY_3 + SHIFT_MASK			// 51 3
#define ASCII_34	KEY_4 + SHIFT_MASK			// 52 4
#define ASCII_35	KEY_5 + SHIFT_MASK			// 53 5
#define ASCII_36	KEY_6 + SHIFT_MASK			// 54 6
#define ASCII_37	KEY_7 + SHIFT_MASK			// 55 7
#define ASCII_38	KEY_8 + SHIFT_MASK			// 55 8
#define ASCII_39	KEY_9 + SHIFT_MASK			// 57 9
#define ASCII_3A	KEY_PERIOD				// 58 :
#define ASCII_3B	KEY_COMMA				// 59 ;
#define ASCII_3C	KEY_NON_US_100				// 60 <
#define ASCII_3D	KEY_SLASH				// 61 =
#define ASCII_3E	KEY_NON_US_100 + SHIFT_MASK		// 62 >
#define ASCII_3F	KEY_M + SHIFT_MASK			// 63 ?
#define ASCII_40	KEY_2 + ALTGR_MASK			// 64 @
#define ASCII_41	KEY_Q + SHIFT_MASK			// 65 A
#define ASCII_42	KEY_B + SHIFT_MASK			// 66 B
#define ASCII_43	KEY_C + SHIFT_MASK			// 67 C
#define ASCII_44	KEY_D + SHIFT_MASK			// 68 D
#define ASCII_45	KEY_E + SHIFT_MASK			// 69 E
#define ASCII_46	KEY_F + SHIFT_MASK			// 70 F
#define ASCII_47	KEY_G + SHIFT_MASK			// 71 G
#define ASCII_48	KEY_H + SHIFT_MASK			// 72 H
#define ASCII_49	KEY_I + SHIFT_MASK			// 73 I
#define ASCII_4A	KEY_J + SHIFT_MASK			// 74 J
#define ASCII_4B	KEY_K + SHIFT_MASK			// 75 K
#define ASCII_4C	KEY_L + SHIFT_MASK			// 76 L
#define ASCII_4D	KEY_SEMICOLON + SHIFT_MASK		// 77 M
#define ASCII_4E	KEY_N + SHIFT_MASK			// 78 N
#define ASCII_4F	KEY_O + SHIFT_MASK			// 79 O
#define ASCII_50	KEY_P + SHIFT_MASK			// 80 P
#define ASCII_51	KEY_A + SHIFT_MASK			// 81 Q
#define ASCII_52	KEY_R + SHIFT_MASK			// 82 R
#define ASCII_53	KEY_S + SHIFT_MASK			// 83 S
#define ASCII_54	KEY_T + SHIFT_MASK			// 84 T
#define ASCII_55	KEY_U + SHIFT_MASK			// 85 U
#define ASCII_56	KEY_V + SHIFT_MASK			// 86 V
#define ASCII_57	KEY_Z + SHIFT_MASK			// 87 W
#define ASCII_58	KEY_X + SHIFT_MASK			// 88 X
#define ASCII_59	KEY_Y + SHIFT_MASK			// 89 Y
#define ASCII_5A	KEY_W + SHIFT_MASK			// 90 Z
#define ASCII_5B	KEY_RIGHT_BRACE + ALTGR_MASK		// 91 [
#define ASCII_5C	KEY_NON_US_100 + ALTGR_MASK		// 92 
#define ASCII_5D	KEY_LEFT_BRACE + ALTGR_MASK		// 93 ]
#define ASCII_5E	KEY_6 + ALTGR_MASK			// 94 ^
#define ASCII_5F	KEY_EQUAL + SHIFT_MASK			// 95 _
#define ASCII_60	GRAVE_ACCENT_BITS + KEY_SPACE		// 96 `
#define ASCII_61	KEY_Q					// 97 a
#define ASCII_62	KEY_B					// 98 b
#define ASCII_63	KEY_C					// 99 c
#define ASCII_64	KEY_D					// 100 d
#define ASCII_65	KEY_E					// 101 e
#define ASCII_66	KEY_F					// 102 f
#define ASCII_67	KEY_G					// 103 g
#define ASCII_68	KEY_H					// 104 h
#define ASCII_69	KEY_I					// 105 i
#define ASCII_6A	KEY_J					// 106 j
#define ASCII_6B	KEY_K					// 107 k
#define ASCII_6C	KEY_L					// 108 l
#define ASCII_6D	KEY_SEMICOLON				// 109 m
#define ASCII_6E	KEY_N					// 110 n
#define ASCII_6F	KEY_O					// 111 o
#define ASCII_70	KEY_P					// 112 p
#define ASCII_71	KEY_A					// 113 q
#define ASCII_72	KEY_R					// 114 r
#define ASCII_73	KEY_S					// 115 s
#define ASCII_74	KEY_T					// 116 t
#define ASCII_75	KEY_U					// 117 u
#define ASCII_76	KEY_V					// 118 v
#define ASCII_77	KEY_Z					// 119 w
#define ASCII_78	KEY_X					// 120 x
#define ASCII_79	KEY_Y					// 121 y
#define ASCII_7A	KEY_W					// 122 z
#define ASCII_7B	KEY_9 + ALTGR_MASK			// 123 {
#define ASCII_7C	KEY_1 + ALTGR_MASK			// 124 |
#define ASCII_7D	KEY_0 + ALTGR_MASK			// 125 }
#define ASCII_7E	TILDE_BITS + KEY_SPACE			// 126 ~
#define ASCII_7F	KEY_BACKSPACE				// 127
#define ISO_8859_1_A0	KEY_SPACE				// 160       Nonbreakng Space
#define ISO_8859_1_A1	0					// 161 ¡     Inverted Exclamation
#define ISO_8859_1_A2	0					// 162 ¢     Cent SIGN
#define ISO_8859_1_A3	KEY_BACKSLASH + SHIFT_MASK		// 163 £     Pound Sign
#define ISO_8859_1_A4	0					// 164 ¤     Currency or Euro Sign
#define ISO_8859_1_A5	0					// 165 ¥     YEN SIGN
#define ISO_8859_1_A6	0					// 166 ¦     BROKEN BAR
#define ISO_8859_1_A7	KEY_6					// 167 §     SECTION SIGN
#define ISO_8859_1_A8	DIAERESIS_BITS + KEY_SPACE		// 168 ¨     DIAERESIS
#define ISO_8859_1_A9	0					// 169 ©     COPYRIGHT SIGN
#define ISO_8859_1_AA	0					// 170 ª     FEMININE ORDINAL
#define ISO_8859_1_AB	0					// 171 «     LEFT DOUBLE ANGLE QUOTE
#define ISO_8859_1_AC	0					// 172 ¬     NOT SIGN
#define ISO_8859_1_AD	0					// 173       SOFT HYPHEN
#define ISO_8859_1_AE	0					// 174 ®     REGISTERED SIGN
#define ISO_8859_1_AF	0					// 175 ¯     MACRON
#define ISO_8859_1_B0	KEY_MINUS + SHIFT_MASK			// 176 °     DEGREE SIGN
#define ISO_8859_1_B1	0					// 177 ±     PLUS-MINUS SIGN
#define ISO_8859_1_B2	KEY_TILDE				// 178 ²     SUPERSCRIPT TWO
#define ISO_8859_1_B3	KEY_TILDE + SHIFT_MASK			// 179 ³     SUPERSCRIPT THREE
#define ISO_8859_1_B4	ACUTE_ACCENT_BITS + KEY_SPACE		// 180 ´     ACUTE ACCENT
#define ISO_8859_1_B5	KEY_BACKSLASH				// 181 µ     MICRO SIGN
#define ISO_8859_1_B6	0					// 182 ¶     PILCROW SIGN
#define ISO_8859_1_B7	0					// 183 ·     MIDDLE DOT
#define ISO_8859_1_B8	0					// 184 ¸     CEDILLA
#define ISO_8859_1_B9	0					// 185 ¹     SUPERSCRIPT ONE
#define ISO_8859_1_BA	0					// 186 º     MASCULINE ORDINAL
#define ISO_8859_1_BB	0					// 187 »     RIGHT DOUBLE ANGLE QUOTE
#define ISO_8859_1_BC	0					// 188 ¼     FRACTION ONE QUARTER
#define ISO_8859_1_BD	0					// 189 ½     FRACTION ONE HALF
#define ISO_8859_1_BE	0					// 190 ¾     FRACTION THREE QUARTERS
#define ISO_8859_1_BF	0					// 191 ¿     INVERTED QUESTION MARK
#define ISO_8859_1_C0	GRAVE_ACCENT_BITS + KEY_Q + SHIFT_MASK	// 192 À     A GRAVE
#define ISO_8859_1_C1	ACUTE_ACCENT_BITS + KEY_Q + SHIFT_MASK	// 193 Á     A ACUTE
#define ISO_8859_1_C2	CIRCUMFLEX_BITS	+ KEY_Q + SHIFT_MASK	// 194 Â     A CIRCUMFLEX
#define ISO_8859_1_C3	TILDE_BITS + KEY_Q + SHIFT_MASK		// 195 Ã     A TILDE
#define ISO_8859_1_C4	DIAERESIS_BITS + KEY_Q + SHIFT_MASK	// 196 Ä     A DIAERESIS
#define ISO_8859_1_C5	0					// 197 Å     A RING ABOVE
#define ISO_8859_1_C6	0					// 198 Æ     AE
#define ISO_8859_1_C7	0					// 199 Ç     C CEDILLA
#define ISO_8859_1_C8	GRAVE_ACCENT_BITS + KEY_E + SHIFT_MASK	// 200 È     E GRAVE
#define ISO_8859_1_C9	ACUTE_ACCENT_BITS + KEY_E + SHIFT_MASK	// 201 É     E ACUTE
#define ISO_8859_1_CA	CIRCUMFLEX_BITS + KEY_E + SHIFT_MASK	// 202 Ê     E CIRCUMFLEX
#define ISO_8859_1_CB	DIAERESIS_BITS + KEY_E + SHIFT_MASK	// 203 Ë     E DIAERESIS
#define ISO_8859_1_CC	GRAVE_ACCENT_BITS + KEY_I + SHIFT_MASK	// 204 Ì     I GRAVE
#define ISO_8859_1_CD	ACUTE_ACCENT_BITS + KEY_I + SHIFT_MASK	// 205 Í     I ACUTE
#define ISO_8859_1_CE	CIRCUMFLEX_BITS + KEY_I + SHIFT_MASK	// 206 Î     I CIRCUMFLEX
#define ISO_8859_1_CF	DIAERESIS_BITS + KEY_I + SHIFT_MASK	// 207 Ï     I DIAERESIS
#define ISO_8859_1_D0	0					// 208 Ð     ETH
#define ISO_8859_1_D1	TILDE_BITS + KEY_N + SHIFT_MASK		// 209 Ñ     N TILDE
#define ISO_8859_1_D2	GRAVE_ACCENT_BITS + KEY_O + SHIFT_MASK	// 210 Ò     O GRAVE
#define ISO_8859_1_D3	ACUTE_ACCENT_BITS + KEY_O + SHIFT_MASK	// 211 Ó     O ACUTE
#define ISO_8859_1_D4	CIRCUMFLEX_BITS + KEY_O + SHIFT_MASK	// 212 Ô     O CIRCUMFLEX
#define ISO_8859_1_D5	TILDE_BITS + KEY_O + SHIFT_MASK		// 213 Õ     O TILDE
#define ISO_8859_1_D6	DIAERESIS_BITS + KEY_O + SHIFT_MASK	// 214 Ö     O DIAERESIS
#define ISO_8859_1_D7	0					// 215 ×     MULTIPLICATION
#define ISO_8859_1_D8	0					// 216 Ø     O STROKE
#define ISO_8859_1_D9	GRAVE_ACCENT_BITS + KEY_U + SHIFT_MASK	// 217 Ù     U GRAVE
#define ISO_8859_1_DA	ACUTE_ACCENT_BITS + KEY_U + SHIFT_MASK	// 218 Ú     U ACUTE
#define ISO_8859_1_DB	CIRCUMFLEX_BITS + KEY_U + SHIFT_MASK	// 219 Û     U CIRCUMFLEX
#define ISO_8859_1_DC	DIAERESIS_BITS + KEY_U			// 220 Ü     U DIAERESIS
#define ISO_8859_1_DD	ACUTE_ACCENT_BITS + KEY_Y + SHIFT_MASK	// 221 Ý     Y ACUTE
#define ISO_8859_1_DE	0					// 222 Þ     THORN
#define ISO_8859_1_DF	0					// 223 ß     SHARP S
#define ISO_8859_1_E0	KEY_0					// 224 à     a GRAVE
#define ISO_8859_1_E1	ACUTE_ACCENT_BITS + KEY_Q		// 225 á     a ACUTE
#define ISO_8859_1_E2	CIRCUMFLEX_BITS + KEY_Q			// 226 â     a CIRCUMFLEX
#define ISO_8859_1_E3	TILDE_BITS + KEY_Q			// 227 ã     a TILDE
#define ISO_8859_1_E4	DIAERESIS_BITS + KEY_Q			// 228 ä     a DIAERESIS
#define ISO_8859_1_E5	0					// 229 å     a RING ABOVE
#define ISO_8859_1_E6	0					// 230 æ     ae
#define ISO_8859_1_E7	KEY_9					// 231 ç     c CEDILLA
#define ISO_8859_1_E8	KEY_7					// 232 è     e GRAVE
#define ISO_8859_1_E9	KEY_2					// 233 é     e ACUTE
#define ISO_8859_1_EA	CIRCUMFLEX_BITS + KEY_E			// 234 ê     e CIRCUMFLEX
#define ISO_8859_1_EB	DIAERESIS_BITS + KEY_E			// 235 ë     e DIAERESIS
#define ISO_8859_1_EC	GRAVE_ACCENT_BITS + KEY_I		// 236 ì     i GRAVE
#define ISO_8859_1_ED	ACUTE_ACCENT_BITS + KEY_I		// 237 í     i ACUTE
#define ISO_8859_1_EE	CIRCUMFLEX_BITS + KEY_I			// 238 î     i CIRCUMFLEX
#define ISO_8859_1_EF	DIAERESIS_BITS + KEY_I			// 239 ï     i DIAERESIS
#define ISO_8859_1_F0	0					// 240 ð     ETH
#define ISO_8859_1_F1	TILDE_BITS + KEY_N			// 241 ñ     n TILDE
#define ISO_8859_1_F2	GRAVE_ACCENT_BITS + KEY_O		// 242 ò     o GRAVE
#define ISO_8859_1_F3	ACUTE_ACCENT_BITS + KEY_O		// 243 ó     o ACUTE
#define ISO_8859_1_F4	CIRCUMFLEX_BITS + KEY_O			// 244 ô     o CIRCUMFLEX
#define ISO_8859_1_F5	TILDE_BITS + KEY_O			// 245 õ     o TILDE
#define ISO_8859_1_F6	DIAERESIS_BITS + KEY_O			// 246 ö     o DIAERESIS
#define ISO_8859_1_F7	0					// 247 ÷     DIVISION
#define ISO_8859_1_F8	0					// 248 ø     o STROKE
#define ISO_8859_1_F9	KEY_QUOTE				// 249 ù     u GRAVE - TODO; check FRENCH
#define ISO_8859_1_FA	ACUTE_ACCENT_BITS + KEY_U		// 250 ú     u ACUTE
#define ISO_8859_1_FB	CIRCUMFLEX_BITS + KEY_U			// 251 û     u CIRCUMFLEX
#define ISO_8859_1_FC	DIAERESIS_BITS + KEY_U			// 252 ü     u DIAERESIS
#define ISO_8859_1_FD	ACUTE_ACCENT_BITS + KEY_Y		// 253 ý     y ACUTE
#define ISO_8859_1_FE	0					// 254 þ     THORN
#define ISO_8859_1_FF	DIAERESIS_BITS + KEY_Y			// 255 ÿ     y DIAERESIS
#define UNICODE_20AC	KEY_E + ALTGR_MASK			//     €     Euro Sign
// not yet implemented
#define UNICODE_EXTRA00	0x20AC
#define KEYCODE_EXTRA00	KEY_E + ALTGR_MASK			// 20AC €    Euro Sign

#endif // LAYOUT_FRENCH_BELGIAN




#ifdef LAYOUT_GERMAN_SWISS

#define SHIFT_MASK		0x0040
#define ALTGR_MASK		0x0080
#define DEADKEYS_MASK		0x0700
#define	CIRCUMFLEX_BITS		0x0100
#define GRAVE_ACCENT_BITS	0x0200
#define DIAERESIS_BITS		0x0300
#define TILDE_BITS		0x0400
#define ACUTE_ACCENT_BITS	0x0500
#define KEYCODE_TYPE		uint16_t
#define DEADKEY_CIRCUMFLEX	KEY_EQUAL
#define DEADKEY_ACUTE_ACCENT	KEY_MINUS + ALTGR_MASK
#define DEADKEY_GRAVE_ACCENT	KEY_EQUAL + SHIFT_MASK
#define DEADKEY_DIAERESIS	KEY_RIGHT_BRACE
#define DEADKEY_TILDE		KEY_EQUAL + ALTGR_MASK
#define KEY_NON_US_100		63

#define ASCII_20	KEY_SPACE				// 32  
#define ASCII_21	KEY_RIGHT_BRACE + SHIFT_MASK		// 33 !
#define ASCII_22	KEY_2 + SHIFT_MASK			// 34 "
#define ASCII_23	KEY_3 + ALTGR_MASK			// 35 #
#define ASCII_24	KEY_BACKSLASH				// 36 $
#define ASCII_25	KEY_5 + SHIFT_MASK			// 37 %
#define ASCII_26	KEY_6 + SHIFT_MASK			// 38 &
#define ASCII_27	KEY_MINUS				// 39 '  
#define ASCII_28	KEY_8 + SHIFT_MASK			// 40 ( 
#define ASCII_29	KEY_9 + SHIFT_MASK			// 41 )
#define ASCII_2A	KEY_3 + SHIFT_MASK			// 42 *
#define ASCII_2B	KEY_1 + SHIFT_MASK			// 43 +
#define ASCII_2C	KEY_COMMA				// 44 ,
#define ASCII_2D	KEY_SLASH				// 45 -
#define ASCII_2E	KEY_PERIOD				// 46 .
#define ASCII_2F	KEY_7 + SHIFT_MASK			// 47 /
#define ASCII_30	KEY_0					// 48 0
#define ASCII_31	KEY_1					// 49 1
#define ASCII_32	KEY_2					// 50 2
#define ASCII_33	KEY_3					// 51 3
#define ASCII_34	KEY_4					// 52 4
#define ASCII_35	KEY_5					// 53 5
#define ASCII_36	KEY_6					// 54 6
#define ASCII_37	KEY_7					// 55 7
#define ASCII_38	KEY_8					// 55 8
#define ASCII_39	KEY_9					// 57 9
#define ASCII_3A	KEY_PERIOD + SHIFT_MASK			// 58 :
#define ASCII_3B	KEY_COMMA + SHIFT_MASK			// 59 ;
#define ASCII_3C	KEY_NON_US_100				// 60 <
#define ASCII_3D	KEY_0 + SHIFT_MASK			// 61 =
#define ASCII_3E	KEY_NON_US_100 + SHIFT_MASK		// 62 >
#define ASCII_3F	KEY_MINUS + SHIFT_MASK			// 63 ?
#define ASCII_40	KEY_2 + ALTGR_MASK			// 64 @
#define ASCII_41	KEY_A + SHIFT_MASK			// 65 A
#define ASCII_42	KEY_B + SHIFT_MASK			// 66 B
#define ASCII_43	KEY_C + SHIFT_MASK			// 67 C
#define ASCII_44	KEY_D + SHIFT_MASK			// 68 D
#define ASCII_45	KEY_E + SHIFT_MASK			// 69 E
#define ASCII_46	KEY_F + SHIFT_MASK			// 70 F
#define ASCII_47	KEY_G + SHIFT_MASK			// 71 G
#define ASCII_48	KEY_H + SHIFT_MASK			// 72 H
#define ASCII_49	KEY_I + SHIFT_MASK			// 73 I
#define ASCII_4A	KEY_J + SHIFT_MASK			// 74 J
#define ASCII_4B	KEY_K + SHIFT_MASK			// 75 K
#define ASCII_4C	KEY_L + SHIFT_MASK			// 76 L
#define ASCII_4D	KEY_M + SHIFT_MASK			// 77 M
#define ASCII_4E	KEY_N + SHIFT_MASK			// 78 N
#define ASCII_4F	KEY_O + SHIFT_MASK			// 79 O
#define ASCII_50	KEY_P + SHIFT_MASK			// 80 P
#define ASCII_51	KEY_Q + SHIFT_MASK			// 81 Q
#define ASCII_52	KEY_R + SHIFT_MASK			// 82 R
#define ASCII_53	KEY_S + SHIFT_MASK			// 83 S
#define ASCII_54	KEY_T + SHIFT_MASK			// 84 T
#define ASCII_55	KEY_U + SHIFT_MASK			// 85 U
#define ASCII_56	KEY_V + SHIFT_MASK			// 86 V
#define ASCII_57	KEY_W + SHIFT_MASK			// 87 W
#define ASCII_58	KEY_X + SHIFT_MASK			// 88 X
#define ASCII_59	KEY_Z + SHIFT_MASK			// 89 Y
#define ASCII_5A	KEY_Y + SHIFT_MASK			// 90 Z
#define ASCII_5B	KEY_LEFT_BRACE + ALTGR_MASK		// 91 [
#define ASCII_5C	KEY_NON_US_100 + ALTGR_MASK		// 92 
#define ASCII_5D	KEY_RIGHT_BRACE + ALTGR_MASK		// 93 ]
#define ASCII_5E	CIRCUMFLEX_BITS + KEY_SPACE		// 94 ^
#define ASCII_5F	KEY_SLASH + SHIFT_MASK			// 95 _
#define ASCII_60	GRAVE_ACCENT_BITS + KEY_SPACE		// 96 `
#define ASCII_61	KEY_A					// 97 a
#define ASCII_62	KEY_B					// 98 b
#define ASCII_63	KEY_C					// 99 c
#define ASCII_64	KEY_D					// 100 d
#define ASCII_65	KEY_E					// 101 e
#define ASCII_66	KEY_F					// 102 f
#define ASCII_67	KEY_G					// 103 g
#define ASCII_68	KEY_H					// 104 h
#define ASCII_69	KEY_I					// 105 i
#define ASCII_6A	KEY_J					// 106 j
#define ASCII_6B	KEY_K					// 107 k
#define ASCII_6C	KEY_L					// 108 l
#define ASCII_6D	KEY_M					// 109 m
#define ASCII_6E	KEY_N					// 110 n
#define ASCII_6F	KEY_O					// 111 o
#define ASCII_70	KEY_P					// 112 p
#define ASCII_71	KEY_Q					// 113 q
#define ASCII_72	KEY_R					// 114 r
#define ASCII_73	KEY_S					// 115 s
#define ASCII_74	KEY_T					// 116 t
#define ASCII_75	KEY_U					// 117 u
#define ASCII_76	KEY_V					// 118 v
#define ASCII_77	KEY_W					// 119 w
#define ASCII_78	KEY_X					// 120 x
#define ASCII_79	KEY_Z					// 121 y
#define ASCII_7A	KEY_Y					// 122 z
#define ASCII_7B	KEY_QUOTE + ALTGR_MASK			// 123 {
#define ASCII_7C	KEY_7 + ALTGR_MASK			// 124 |
#define ASCII_7D	KEY_BACKSLASH + ALTGR_MASK		// 125 }
#define ASCII_7E	TILDE_BITS + KEY_SPACE			// 126 ~
#define ASCII_7F	KEY_BACKSPACE				// 127
#define ISO_8859_1_A0	KEY_SPACE				// 160       Nonbreakng Space
#define ISO_8859_1_A1	0					// 161 ¡     Inverted Exclamation
#define ISO_8859_1_A2	KEY_8 + ALTGR_MASK			// 162 ¢     Cent Sign
#define ISO_8859_1_A3	KEY_BACKSLASH + SHIFT_MASK		// 163 £     Pound Sign
#define ISO_8859_1_A4	0					// 164 ¤     Currency Sign
#define ISO_8859_1_A5	0					// 165 ¥     YEN SIGN
#define ISO_8859_1_A6	KEY_1 + ALTGR_MASK			// 166 ¦     BROKEN BAR
#define ISO_8859_1_A7	KEY_5 + ALTGR_MASK			// 167 §     SECTION SIGN
#define ISO_8859_1_A8	DIAERESIS_BITS + KEY_SPACE		// 168 ¨     DIAERESIS
#define ISO_8859_1_A9	0					// 169 ©     COPYRIGHT SIGN
#define ISO_8859_1_AA	0					// 170 ª     FEMININE ORDINAL
#define ISO_8859_1_AB	0					// 171 «     LEFT DOUBLE ANGLE QUOTE
#define ISO_8859_1_AC	KEY_6 + ALTGR_MASK			// 172 ¬     NOT SIGN
#define ISO_8859_1_AD	0					// 173       SOFT HYPHEN
#define ISO_8859_1_AE	0					// 174 ®     REGISTERED SIGN
#define ISO_8859_1_AF	0					// 175 ¯     MACRON
#define ISO_8859_1_B0	KEY_4 + ALTGR_MASK			// 176 °     DEGREE SIGN
#define ISO_8859_1_B1	0					// 177 ±     PLUS-MINUS SIGN
#define ISO_8859_1_B2	0					// 178 ²     SUPERSCRIPT TWO
#define ISO_8859_1_B3	0					// 179 ³     SUPERSCRIPT THREE
#define ISO_8859_1_B4	ACUTE_ACCENT_BITS + KEY_SPACE		// 180 ´     ACUTE ACCENT
#define ISO_8859_1_B5	0					// 181 µ     MICRO SIGN
#define ISO_8859_1_B6	0					// 182 ¶     PILCROW SIGN
#define ISO_8859_1_B7	0					// 183 ·     MIDDLE DOT
#define ISO_8859_1_B8	0					// 184 ¸     CEDILLA
#define ISO_8859_1_B9	0					// 185 ¹     SUPERSCRIPT ONE
#define ISO_8859_1_BA	0					// 186 º     MASCULINE ORDINAL
#define ISO_8859_1_BB	0					// 187 »     RIGHT DOUBLE ANGLE QUOTE
#define ISO_8859_1_BC	0					// 188 ¼     FRACTION ONE QUARTER
#define ISO_8859_1_BD	0					// 189 ½     FRACTION ONE HALF
#define ISO_8859_1_BE	0					// 190 ¾     FRACTION THREE QUARTERS
#define ISO_8859_1_BF	0					// 191 ¿     INVERTED QUESTION MARK
#define ISO_8859_1_C0	GRAVE_ACCENT_BITS + KEY_A + SHIFT_MASK	// 192 À     A GRAVE
#define ISO_8859_1_C1	ACUTE_ACCENT_BITS + KEY_A + SHIFT_MASK	// 193 Á     A ACUTE
#define ISO_8859_1_C2	CIRCUMFLEX_BITS	+ KEY_A + SHIFT_MASK	// 194 Â     A CIRCUMFLEX
#define ISO_8859_1_C3	TILDE_BITS + KEY_A + SHIFT_MASK		// 195 Ã     A TILDE
#define ISO_8859_1_C4	DIAERESIS_BITS + KEY_A + SHIFT_MASK	// 196 Ä     A DIAERESIS
#define ISO_8859_1_C5	0					// 197 Å     A RING ABOVE
#define ISO_8859_1_C6	0					// 198 Æ     AE
#define ISO_8859_1_C7	0					// 199 Ç     C CEDILLA
#define ISO_8859_1_C8	GRAVE_ACCENT_BITS + KEY_E + SHIFT_MASK	// 200 È     E GRAVE
#define ISO_8859_1_C9	ACUTE_ACCENT_BITS + KEY_E + SHIFT_MASK	// 201 É     E ACUTE
#define ISO_8859_1_CA	CIRCUMFLEX_BITS + KEY_E + SHIFT_MASK	// 202 Ê     E CIRCUMFLEX
#define ISO_8859_1_CB	DIAERESIS_BITS + KEY_E + SHIFT_MASK	// 203 Ë     E DIAERESIS
#define ISO_8859_1_CC	GRAVE_ACCENT_BITS + KEY_I + SHIFT_MASK	// 204 Ì     I GRAVE
#define ISO_8859_1_CD	ACUTE_ACCENT_BITS + KEY_I + SHIFT_MASK	// 205 Í     I ACUTE
#define ISO_8859_1_CE	CIRCUMFLEX_BITS + KEY_I + SHIFT_MASK	// 206 Î     I CIRCUMFLEX
#define ISO_8859_1_CF	DIAERESIS_BITS + KEY_I + SHIFT_MASK	// 207 Ï     I DIAERESIS
#define ISO_8859_1_D0	0					// 208 Ð     ETH
#define ISO_8859_1_D1	TILDE_BITS + KEY_N + SHIFT_MASK		// 209 Ñ     N TILDE
#define ISO_8859_1_D2	GRAVE_ACCENT_BITS + KEY_O + SHIFT_MASK	// 210 Ò     O GRAVE
#define ISO_8859_1_D3	ACUTE_ACCENT_BITS + KEY_O + SHIFT_MASK	// 211 Ó     O ACUTE
#define ISO_8859_1_D4	CIRCUMFLEX_BITS + KEY_O + SHIFT_MASK	// 212 Ô     O CIRCUMFLEX
#define ISO_8859_1_D5	TILDE_BITS + KEY_O + SHIFT_MASK		// 213 Õ     O TILDE
#define ISO_8859_1_D6	DIAERESIS_BITS + KEY_O + SHIFT_MASK	// 214 Ö     O DIAERESIS
#define ISO_8859_1_D7	0					// 215 ×     MULTIPLICATION
#define ISO_8859_1_D8	0					// 216 Ø     O STROKE
#define ISO_8859_1_D9	GRAVE_ACCENT_BITS + KEY_U + SHIFT_MASK	// 217 Ù     U GRAVE
#define ISO_8859_1_DA	ACUTE_ACCENT_BITS + KEY_U + SHIFT_MASK	// 218 Ú     U ACUTE
#define ISO_8859_1_DB	CIRCUMFLEX_BITS + KEY_U + SHIFT_MASK	// 219 Û     U CIRCUMFLEX
#define ISO_8859_1_DC	DIAERESIS_BITS + KEY_U + SHIFT_MASK	// 220 Ü     U DIAERESIS
#define ISO_8859_1_DD	ACUTE_ACCENT_BITS + KEY_Y + SHIFT_MASK	// 221 Ý     Y ACUTE
#define ISO_8859_1_DE	0					// 222 Þ     THORN
#define ISO_8859_1_DF	0					// 223 ß     SHARP S
#define ISO_8859_1_E0	KEY_QUOTE + SHIFT_MASK			// 224 à     a GRAVE
#define ISO_8859_1_E1	ACUTE_ACCENT_BITS + KEY_A		// 225 á     a ACUTE
#define ISO_8859_1_E2	CIRCUMFLEX_BITS + KEY_A			// 226 â     a CIRCUMFLEX
#define ISO_8859_1_E3	TILDE_BITS + KEY_A			// 227 ã     a TILDE
#define ISO_8859_1_E4	KEY_QUOTE				// 228 ä     a DIAERESIS
#define ISO_8859_1_E5	0					// 229 å     a RING ABOVE
#define ISO_8859_1_E6	0					// 230 æ     ae
#define ISO_8859_1_E7	KEY_4 + SHIFT_MASK			// 231 ç     c CEDILLA
#define ISO_8859_1_E8	KEY_LEFT_BRACE + SHIFT_MASK		// 232 è     e GRAVE
#define ISO_8859_1_E9	KEY_SEMICOLON + SHIFT_MASK		// 233 é     e ACUTE
#define ISO_8859_1_EA	CIRCUMFLEX_BITS + KEY_E			// 234 ê     e CIRCUMFLEX
#define ISO_8859_1_EB	DIAERESIS_BITS + KEY_E			// 235 ë     e DIAERESIS  TODO: check this
#define ISO_8859_1_EC	GRAVE_ACCENT_BITS + KEY_I		// 236 ì     i GRAVE
#define ISO_8859_1_ED	ACUTE_ACCENT_BITS + KEY_I		// 237 í     i ACUTE
#define ISO_8859_1_EE	CIRCUMFLEX_BITS + KEY_I			// 238 î     i CIRCUMFLEX
#define ISO_8859_1_EF	DIAERESIS_BITS + KEY_I			// 239 ï     i DIAERESIS
#define ISO_8859_1_F0	0					// 240 ð     ETH
#define ISO_8859_1_F1	TILDE_BITS + KEY_N			// 241 ñ     n TILDE
#define ISO_8859_1_F2	GRAVE_ACCENT_BITS + KEY_O		// 242 ò     o GRAVE
#define ISO_8859_1_F3	ACUTE_ACCENT_BITS + KEY_O		// 243 ó     o ACUTE
#define ISO_8859_1_F4	CIRCUMFLEX_BITS + KEY_O			// 244 ô     o CIRCUMFLEX
#define ISO_8859_1_F5	TILDE_BITS + KEY_O			// 245 õ     o TILDE
#define ISO_8859_1_F6	KEY_SEMICOLON				// 246 ö     o DIAERESIS
#define ISO_8859_1_F7	0					// 247 ÷     DIVISION
#define ISO_8859_1_F8	0					// 248 ø     o STROKE
#define ISO_8859_1_F9	GRAVE_ACCENT_BITS + KEY_U		// 249 ù     u GRAVE
#define ISO_8859_1_FA	ACUTE_ACCENT_BITS + KEY_U		// 250 ú     u ACUTE
#define ISO_8859_1_FB	CIRCUMFLEX_BITS + KEY_U			// 251 û     u CIRCUMFLEX
#define ISO_8859_1_FC	KEY_LEFT_BRACE				// 252 ü     u DIAERESIS
#define ISO_8859_1_FD	ACUTE_ACCENT_BITS + KEY_Y		// 253 ý     y ACUTE
#define ISO_8859_1_FE	0					// 254 þ     THORN
#define ISO_8859_1_FF	DIAERESIS_BITS + KEY_Y			// 255 ÿ     y DIAERESIS
#define UNICODE_20AC	KEY_E + ALTGR_MASK			//     €     Euro Sign
// not yet implemented
#define UNICODE_EXTRA00	0x20AC
#define KEYCODE_EXTRA00	KEY_E + ALTGR_MASK			// 20AC €    Euro Sign

#endif // LAYOUT_GERMAN_SWISS




#ifdef LAYOUT_FRENCH_SWISS

#define SHIFT_MASK		0x0040
#define ALTGR_MASK		0x0080
#define DEADKEYS_MASK		0x0700
#define	CIRCUMFLEX_BITS		0x0100
#define GRAVE_ACCENT_BITS	0x0200
#define DIAERESIS_BITS		0x0300
#define TILDE_BITS		0x0400
#define ACUTE_ACCENT_BITS	0x0500
#define KEYCODE_TYPE		uint16_t
#define DEADKEY_CIRCUMFLEX	KEY_EQUAL
#define DEADKEY_ACUTE_ACCENT	KEY_MINUS + ALTGR_MASK
#define DEADKEY_GRAVE_ACCENT	KEY_EQUAL + SHIFT_MASK
#define DEADKEY_DIAERESIS	KEY_RIGHT_BRACE
#define DEADKEY_TILDE		KEY_EQUAL + ALTGR_MASK
#define KEY_NON_US_100		63

#define ASCII_20	KEY_SPACE				// 32  
#define ASCII_21	KEY_RIGHT_BRACE + SHIFT_MASK		// 33 !
#define ASCII_22	KEY_2 + SHIFT_MASK			// 34 "
#define ASCII_23	KEY_3 + ALTGR_MASK			// 35 #
#define ASCII_24	KEY_BACKSLASH				// 36 $
#define ASCII_25	KEY_5 + SHIFT_MASK			// 37 %
#define ASCII_26	KEY_6 + SHIFT_MASK			// 38 &
#define ASCII_27	KEY_MINUS				// 39 '  
#define ASCII_28	KEY_8 + SHIFT_MASK			// 40 ( 
#define ASCII_29	KEY_9 + SHIFT_MASK			// 41 )
#define ASCII_2A	KEY_3 + SHIFT_MASK			// 42 *
#define ASCII_2B	KEY_1 + SHIFT_MASK			// 43 +
#define ASCII_2C	KEY_COMMA				// 44 ,
#define ASCII_2D	KEY_SLASH				// 45 -
#define ASCII_2E	KEY_PERIOD				// 46 .
#define ASCII_2F	KEY_7 + SHIFT_MASK			// 47 /
#define ASCII_30	KEY_0					// 48 0
#define ASCII_31	KEY_1					// 49 1
#define ASCII_32	KEY_2					// 50 2
#define ASCII_33	KEY_3					// 51 3
#define ASCII_34	KEY_4					// 52 4
#define ASCII_35	KEY_5					// 53 5
#define ASCII_36	KEY_6					// 54 6
#define ASCII_37	KEY_7					// 55 7
#define ASCII_38	KEY_8					// 55 8
#define ASCII_39	KEY_9					// 57 9
#define ASCII_3A	KEY_PERIOD + SHIFT_MASK			// 58 :
#define ASCII_3B	KEY_COMMA + SHIFT_MASK			// 59 ;
#define ASCII_3C	KEY_NON_US_100				// 60 <
#define ASCII_3D	KEY_0 + SHIFT_MASK			// 61 =
#define ASCII_3E	KEY_NON_US_100 + SHIFT_MASK		// 62 >
#define ASCII_3F	KEY_MINUS + SHIFT_MASK			// 63 ?
#define ASCII_40	KEY_2 + ALTGR_MASK			// 64 @
#define ASCII_41	KEY_A + SHIFT_MASK			// 65 A
#define ASCII_42	KEY_B + SHIFT_MASK			// 66 B
#define ASCII_43	KEY_C + SHIFT_MASK			// 67 C
#define ASCII_44	KEY_D + SHIFT_MASK			// 68 D
#define ASCII_45	KEY_E + SHIFT_MASK			// 69 E
#define ASCII_46	KEY_F + SHIFT_MASK			// 70 F
#define ASCII_47	KEY_G + SHIFT_MASK			// 71 G
#define ASCII_48	KEY_H + SHIFT_MASK			// 72 H
#define ASCII_49	KEY_I + SHIFT_MASK			// 73 I
#define ASCII_4A	KEY_J + SHIFT_MASK			// 74 J
#define ASCII_4B	KEY_K + SHIFT_MASK			// 75 K
#define ASCII_4C	KEY_L + SHIFT_MASK			// 76 L
#define ASCII_4D	KEY_M + SHIFT_MASK			// 77 M
#define ASCII_4E	KEY_N + SHIFT_MASK			// 78 N
#define ASCII_4F	KEY_O + SHIFT_MASK			// 79 O
#define ASCII_50	KEY_P + SHIFT_MASK			// 80 P
#define ASCII_51	KEY_Q + SHIFT_MASK			// 81 Q
#define ASCII_52	KEY_R + SHIFT_MASK			// 82 R
#define ASCII_53	KEY_S + SHIFT_MASK			// 83 S
#define ASCII_54	KEY_T + SHIFT_MASK			// 84 T
#define ASCII_55	KEY_U + SHIFT_MASK			// 85 U
#define ASCII_56	KEY_V + SHIFT_MASK			// 86 V
#define ASCII_57	KEY_W + SHIFT_MASK			// 87 W
#define ASCII_58	KEY_X + SHIFT_MASK			// 88 X
#define ASCII_59	KEY_Z + SHIFT_MASK			// 89 Y
#define ASCII_5A	KEY_Y + SHIFT_MASK			// 90 Z
#define ASCII_5B	KEY_LEFT_BRACE + ALTGR_MASK		// 91 [
#define ASCII_5C	KEY_NON_US_100 + ALTGR_MASK		// 92 
#define ASCII_5D	KEY_RIGHT_BRACE + ALTGR_MASK		// 93 ]
#define ASCII_5E	CIRCUMFLEX_BITS + KEY_SPACE		// 94 ^
#define ASCII_5F	KEY_SLASH + SHIFT_MASK			// 95 _
#define ASCII_60	GRAVE_ACCENT_BITS + KEY_SPACE		// 96 `
#define ASCII_61	KEY_A					// 97 a
#define ASCII_62	KEY_B					// 98 b
#define ASCII_63	KEY_C					// 99 c
#define ASCII_64	KEY_D					// 100 d
#define ASCII_65	KEY_E					// 101 e
#define ASCII_66	KEY_F					// 102 f
#define ASCII_67	KEY_G					// 103 g
#define ASCII_68	KEY_H					// 104 h
#define ASCII_69	KEY_I					// 105 i
#define ASCII_6A	KEY_J					// 106 j
#define ASCII_6B	KEY_K					// 107 k
#define ASCII_6C	KEY_L					// 108 l
#define ASCII_6D	KEY_M					// 109 m
#define ASCII_6E	KEY_N					// 110 n
#define ASCII_6F	KEY_O					// 111 o
#define ASCII_70	KEY_P					// 112 p
#define ASCII_71	KEY_Q					// 113 q
#define ASCII_72	KEY_R					// 114 r
#define ASCII_73	KEY_S					// 115 s
#define ASCII_74	KEY_T					// 116 t
#define ASCII_75	KEY_U					// 117 u
#define ASCII_76	KEY_V					// 118 v
#define ASCII_77	KEY_W					// 119 w
#define ASCII_78	KEY_X					// 120 x
#define ASCII_79	KEY_Z					// 121 y
#define ASCII_7A	KEY_Y					// 122 z
#define ASCII_7B	KEY_QUOTE + ALTGR_MASK			// 123 {
#define ASCII_7C	KEY_7 + ALTGR_MASK			// 124 |
#define ASCII_7D	KEY_BACKSLASH + ALTGR_MASK		// 125 }
#define ASCII_7E	TILDE_BITS + KEY_SPACE			// 126 ~
#define ASCII_7F	KEY_BACKSPACE				// 127
#define ISO_8859_1_A0	KEY_SPACE				// 160       Nonbreakng Space
#define ISO_8859_1_A1	0					// 161 ¡     Inverted Exclamation
#define ISO_8859_1_A2	KEY_8 + ALTGR_MASK			// 162 ¢     Cent Sign
#define ISO_8859_1_A3	KEY_BACKSLASH + SHIFT_MASK		// 163 £     Pound Sign
#define ISO_8859_1_A4	0					// 164 ¤     Currency Sign
#define ISO_8859_1_A5	0					// 165 ¥     YEN SIGN
#define ISO_8859_1_A6	KEY_1 + ALTGR_MASK			// 166 ¦     BROKEN BAR
#define ISO_8859_1_A7	KEY_5 + ALTGR_MASK			// 167 §     SECTION SIGN
#define ISO_8859_1_A8	DIAERESIS_BITS + KEY_SPACE		// 168 ¨     DIAERESIS
#define ISO_8859_1_A9	0					// 169 ©     COPYRIGHT SIGN
#define ISO_8859_1_AA	0					// 170 ª     FEMININE ORDINAL
#define ISO_8859_1_AB	0					// 171 «     LEFT DOUBLE ANGLE QUOTE
#define ISO_8859_1_AC	KEY_6 + ALTGR_MASK			// 172 ¬     NOT SIGN
#define ISO_8859_1_AD	0					// 173       SOFT HYPHEN
#define ISO_8859_1_AE	0					// 174 ®     REGISTERED SIGN
#define ISO_8859_1_AF	0					// 175 ¯     MACRON
#define ISO_8859_1_B0	KEY_4 + ALTGR_MASK			// 176 °     DEGREE SIGN
#define ISO_8859_1_B1	0					// 177 ±     PLUS-MINUS SIGN
#define ISO_8859_1_B2	0					// 178 ²     SUPERSCRIPT TWO
#define ISO_8859_1_B3	0					// 179 ³     SUPERSCRIPT THREE
#define ISO_8859_1_B4	ACUTE_ACCENT_BITS + KEY_SPACE		// 180 ´     ACUTE ACCENT
#define ISO_8859_1_B5	0					// 181 µ     MICRO SIGN
#define ISO_8859_1_B6	0					// 182 ¶     PILCROW SIGN
#define ISO_8859_1_B7	0					// 183 ·     MIDDLE DOT
#define ISO_8859_1_B8	0					// 184 ¸     CEDILLA
#define ISO_8859_1_B9	0					// 185 ¹     SUPERSCRIPT ONE
#define ISO_8859_1_BA	0					// 186 º     MASCULINE ORDINAL
#define ISO_8859_1_BB	0					// 187 »     RIGHT DOUBLE ANGLE QUOTE
#define ISO_8859_1_BC	0					// 188 ¼     FRACTION ONE QUARTER
#define ISO_8859_1_BD	0					// 189 ½     FRACTION ONE HALF
#define ISO_8859_1_BE	0					// 190 ¾     FRACTION THREE QUARTERS
#define ISO_8859_1_BF	0					// 191 ¿     INVERTED QUESTION MARK
#define ISO_8859_1_C0	GRAVE_ACCENT_BITS + KEY_A + SHIFT_MASK	// 192 À     A GRAVE
#define ISO_8859_1_C1	ACUTE_ACCENT_BITS + KEY_A + SHIFT_MASK	// 193 Á     A ACUTE
#define ISO_8859_1_C2	CIRCUMFLEX_BITS	+ KEY_A + SHIFT_MASK	// 194 Â     A CIRCUMFLEX
#define ISO_8859_1_C3	TILDE_BITS + KEY_A + SHIFT_MASK		// 195 Ã     A TILDE
#define ISO_8859_1_C4	DIAERESIS_BITS + KEY_A + SHIFT_MASK	// 196 Ä     A DIAERESIS
#define ISO_8859_1_C5	0					// 197 Å     A RING ABOVE
#define ISO_8859_1_C6	0					// 198 Æ     AE
#define ISO_8859_1_C7	0					// 199 Ç     C CEDILLA
#define ISO_8859_1_C8	GRAVE_ACCENT_BITS + KEY_E + SHIFT_MASK	// 200 È     E GRAVE
#define ISO_8859_1_C9	ACUTE_ACCENT_BITS + KEY_E + SHIFT_MASK	// 201 É     E ACUTE
#define ISO_8859_1_CA	CIRCUMFLEX_BITS + KEY_E + SHIFT_MASK	// 202 Ê     E CIRCUMFLEX
#define ISO_8859_1_CB	DIAERESIS_BITS + KEY_E + SHIFT_MASK	// 203 Ë     E DIAERESIS
#define ISO_8859_1_CC	GRAVE_ACCENT_BITS + KEY_I + SHIFT_MASK	// 204 Ì     I GRAVE
#define ISO_8859_1_CD	ACUTE_ACCENT_BITS + KEY_I + SHIFT_MASK	// 205 Í     I ACUTE
#define ISO_8859_1_CE	CIRCUMFLEX_BITS + KEY_I + SHIFT_MASK	// 206 Î     I CIRCUMFLEX
#define ISO_8859_1_CF	DIAERESIS_BITS + KEY_I + SHIFT_MASK	// 207 Ï     I DIAERESIS
#define ISO_8859_1_D0	0					// 208 Ð     ETH
#define ISO_8859_1_D1	TILDE_BITS + KEY_N + SHIFT_MASK		// 209 Ñ     N TILDE
#define ISO_8859_1_D2	GRAVE_ACCENT_BITS + KEY_O + SHIFT_MASK	// 210 Ò     O GRAVE
#define ISO_8859_1_D3	ACUTE_ACCENT_BITS + KEY_O + SHIFT_MASK	// 211 Ó     O ACUTE
#define ISO_8859_1_D4	CIRCUMFLEX_BITS + KEY_O + SHIFT_MASK	// 212 Ô     O CIRCUMFLEX
#define ISO_8859_1_D5	TILDE_BITS + KEY_O + SHIFT_MASK		// 213 Õ     O TILDE
#define ISO_8859_1_D6	DIAERESIS_BITS + KEY_O + SHIFT_MASK	// 214 Ö     O DIAERESIS
#define ISO_8859_1_D7	0					// 215 ×     MULTIPLICATION
#define ISO_8859_1_D8	0					// 216 Ø     O STROKE
#define ISO_8859_1_D9	GRAVE_ACCENT_BITS + KEY_U + SHIFT_MASK	// 217 Ù     U GRAVE
#define ISO_8859_1_DA	ACUTE_ACCENT_BITS + KEY_U + SHIFT_MASK	// 218 Ú     U ACUTE
#define ISO_8859_1_DB	CIRCUMFLEX_BITS + KEY_U + SHIFT_MASK	// 219 Û     U CIRCUMFLEX
#define ISO_8859_1_DC	DIAERESIS_BITS + KEY_U + SHIFT_MASK	// 220 Ü     U DIAERESIS
#define ISO_8859_1_DD	ACUTE_ACCENT_BITS + KEY_Y + SHIFT_MASK	// 221 Ý     Y ACUTE
#define ISO_8859_1_DE	0					// 222 Þ     THORN
#define ISO_8859_1_DF	0					// 223 ß     SHARP S
#define ISO_8859_1_E0	KEY_QUOTE				// 224 à     a GRAVE
#define ISO_8859_1_E1	ACUTE_ACCENT_BITS + KEY_A		// 225 á     a ACUTE
#define ISO_8859_1_E2	CIRCUMFLEX_BITS + KEY_A			// 226 â     a CIRCUMFLEX
#define ISO_8859_1_E3	TILDE_BITS + KEY_A			// 227 ã     a TILDE
#define ISO_8859_1_E4	KEY_QUOTE + SHIFT_MASK			// 228 ä     a DIAERESIS
#define ISO_8859_1_E5	0					// 229 å     a RING ABOVE
#define ISO_8859_1_E6	0					// 230 æ     ae
#define ISO_8859_1_E7	KEY_4 + SHIFT_MASK			// 231 ç     c CEDILLA
#define ISO_8859_1_E8	KEY_LEFT_BRACE				// 232 è     e GRAVE
#define ISO_8859_1_E9	KEY_SEMICOLON				// 233 é     e ACUTE
#define ISO_8859_1_EA	CIRCUMFLEX_BITS + KEY_E			// 234 ê     e CIRCUMFLEX
#define ISO_8859_1_EB	DIAERESIS_BITS + KEY_E			// 235 ë     e DIAERESIS
#define ISO_8859_1_EC	GRAVE_ACCENT_BITS + KEY_I		// 236 ì     i GRAVE
#define ISO_8859_1_ED	ACUTE_ACCENT_BITS + KEY_I		// 237 í     i ACUTE
#define ISO_8859_1_EE	CIRCUMFLEX_BITS + KEY_I			// 238 î     i CIRCUMFLEX
#define ISO_8859_1_EF	DIAERESIS_BITS + KEY_I			// 239 ï     i DIAERESIS
#define ISO_8859_1_F0	0					// 240 ð     ETH
#define ISO_8859_1_F1	TILDE_BITS + KEY_N			// 241 ñ     n TILDE
#define ISO_8859_1_F2	GRAVE_ACCENT_BITS + KEY_O		// 242 ò     o GRAVE
#define ISO_8859_1_F3	ACUTE_ACCENT_BITS + KEY_O		// 243 ó     o ACUTE
#define ISO_8859_1_F4	CIRCUMFLEX_BITS + KEY_O			// 244 ô     o CIRCUMFLEX
#define ISO_8859_1_F5	TILDE_BITS + KEY_O			// 245 õ     o TILDE
#define ISO_8859_1_F6	KEY_SEMICOLON + SHIFT_MASK		// 246 ö     o DIAERESIS
#define ISO_8859_1_F7	0					// 247 ÷     DIVISION
#define ISO_8859_1_F8	0					// 248 ø     o STROKE
#define ISO_8859_1_F9	GRAVE_ACCENT_BITS + KEY_U		// 249 ù     u GRAVE
#define ISO_8859_1_FA	ACUTE_ACCENT_BITS + KEY_U		// 250 ú     u ACUTE
#define ISO_8859_1_FB	CIRCUMFLEX_BITS + KEY_U			// 251 û     u CIRCUMFLEX
#define ISO_8859_1_FC	KEY_LEFT_BRACE + SHIFT_MASK		// 252 ü     u DIAERESIS
#define ISO_8859_1_FD	ACUTE_ACCENT_BITS + KEY_Y		// 253 ý     y ACUTE
#define ISO_8859_1_FE	0					// 254 þ     THORN
#define ISO_8859_1_FF	DIAERESIS_BITS + KEY_Y			// 255 ÿ     y DIAERESIS
#define UNICODE_20AC	KEY_E + ALTGR_MASK			//     €     Euro Sign
// not yet implemented
#define UNICODE_EXTRA00	0x20AC
#define KEYCODE_EXTRA00	KEY_E + ALTGR_MASK			// 20AC €    Euro Sign

#endif // LAYOUT_FRENCH_SWISS





#ifdef LAYOUT_SPANISH_LATIN_AMERICA

#define SHIFT_MASK		0x0040
#define ALTGR_MASK		0x0080
#define DEADKEYS_MASK		0x0700
#define	CIRCUMFLEX_BITS		0x0100
#define ACUTE_ACCENT_BITS	0x0200
#define GRAVE_ACCENT_BITS	0x0300
#define DIAERESIS_BITS		0x0400
#define KEYCODE_TYPE		uint16_t
#define DEADKEY_CIRCUMFLEX	KEY_QUOTE + ALTGR_MASK
#define DEADKEY_ACUTE_ACCENT	KEY_LEFT_BRACE
#define DEADKEY_GRAVE_ACCENT	KEY_BACKSLASH + ALTGR_MASK
#define DEADKEY_DIAERESIS	KEY_LEFT_BRACE + SHIFT_MASK
#define KEY_NON_US_100		63

#define ASCII_20	KEY_SPACE				// 32  
#define ASCII_21	KEY_1 + SHIFT_MASK			// 33 !
#define ASCII_22	KEY_2 + SHIFT_MASK			// 34 "
#define ASCII_23	KEY_3 + ALTGR_MASK			// 35 #
#define ASCII_24	KEY_4 + SHIFT_MASK			// 36 $
#define ASCII_25	KEY_5 + SHIFT_MASK			// 37 %
#define ASCII_26	KEY_6 + SHIFT_MASK			// 38 &
#define ASCII_27	KEY_MINUS				// 39 '  
#define ASCII_28	KEY_8 + SHIFT_MASK			// 40 ( 
#define ASCII_29	KEY_9 + SHIFT_MASK			// 41 )
#define ASCII_2A	KEY_RIGHT_BRACE + SHIFT_MASK		// 42 *
#define ASCII_2B	KEY_RIGHT_BRACE				// 43 +
#define ASCII_2C	KEY_COMMA				// 44 ,
#define ASCII_2D	KEY_SLASH				// 45 -
#define ASCII_2E	KEY_PERIOD				// 46 .
#define ASCII_2F	KEY_7 + SHIFT_MASK			// 47 /
#define ASCII_30	KEY_0					// 48 0
#define ASCII_31	KEY_1					// 49 1
#define ASCII_32	KEY_2					// 50 2
#define ASCII_33	KEY_3					// 51 3
#define ASCII_34	KEY_4					// 52 4
#define ASCII_35	KEY_5					// 53 5
#define ASCII_36	KEY_6					// 54 6
#define ASCII_37	KEY_7					// 55 7
#define ASCII_38	KEY_8					// 55 8
#define ASCII_39	KEY_9					// 57 9
#define ASCII_3A	KEY_PERIOD + SHIFT_MASK			// 58 :
#define ASCII_3B	KEY_COMMA + SHIFT_MASK			// 59 ;
#define ASCII_3C	KEY_NON_US_100				// 60 <
#define ASCII_3D	KEY_0 + SHIFT_MASK			// 61 =
#define ASCII_3E	KEY_NON_US_100 + SHIFT_MASK		// 62 >
#define ASCII_3F	KEY_MINUS + SHIFT_MASK			// 63 ?
#define ASCII_40	KEY_Q + ALTGR_MASK			// 64 @
#define ASCII_41	KEY_A + SHIFT_MASK			// 65 A
#define ASCII_42	KEY_B + SHIFT_MASK			// 66 B
#define ASCII_43	KEY_C + SHIFT_MASK			// 67 C
#define ASCII_44	KEY_D + SHIFT_MASK			// 68 D
#define ASCII_45	KEY_E + SHIFT_MASK			// 69 E
#define ASCII_46	KEY_F + SHIFT_MASK			// 70 F
#define ASCII_47	KEY_G + SHIFT_MASK			// 71 G
#define ASCII_48	KEY_H + SHIFT_MASK			// 72 H
#define ASCII_49	KEY_I + SHIFT_MASK			// 73 I
#define ASCII_4A	KEY_J + SHIFT_MASK			// 74 J
#define ASCII_4B	KEY_K + SHIFT_MASK			// 75 K
#define ASCII_4C	KEY_L + SHIFT_MASK			// 76 L
#define ASCII_4D	KEY_M + SHIFT_MASK			// 77 M
#define ASCII_4E	KEY_N + SHIFT_MASK			// 78 N
#define ASCII_4F	KEY_O + SHIFT_MASK			// 79 O
#define ASCII_50	KEY_P + SHIFT_MASK			// 80 P
#define ASCII_51	KEY_Q + SHIFT_MASK			// 81 Q
#define ASCII_52	KEY_R + SHIFT_MASK			// 82 R
#define ASCII_53	KEY_S + SHIFT_MASK			// 83 S
#define ASCII_54	KEY_T + SHIFT_MASK			// 84 T
#define ASCII_55	KEY_U + SHIFT_MASK			// 85 U
#define ASCII_56	KEY_V + SHIFT_MASK			// 86 V
#define ASCII_57	KEY_W + SHIFT_MASK			// 87 W
#define ASCII_58	KEY_X + SHIFT_MASK			// 88 X
#define ASCII_59	KEY_Y + SHIFT_MASK			// 89 Y
#define ASCII_5A	KEY_Z + SHIFT_MASK			// 90 Z
#define ASCII_5B	KEY_QUOTE + SHIFT_MASK			// 91 [
#define ASCII_5C	KEY_MINUS + ALTGR_MASK			// 92 
#define ASCII_5D	KEY_BACKSLASH + SHIFT_MASK		// 93 ]
#define ASCII_5E	CIRCUMFLEX_BITS + KEY_SPACE		// 94 ^
#define ASCII_5F	KEY_SLASH + SHIFT_MASK			// 95 _
#define ASCII_60	GRAVE_ACCENT_BITS + KEY_SPACE		// 96 `
#define ASCII_61	KEY_A					// 97 a
#define ASCII_62	KEY_B					// 98 b
#define ASCII_63	KEY_C					// 99 c
#define ASCII_64	KEY_D					// 100 d
#define ASCII_65	KEY_E					// 101 e
#define ASCII_66	KEY_F					// 102 f
#define ASCII_67	KEY_G					// 103 g
#define ASCII_68	KEY_H					// 104 h
#define ASCII_69	KEY_I					// 105 i
#define ASCII_6A	KEY_J					// 106 j
#define ASCII_6B	KEY_K					// 107 k
#define ASCII_6C	KEY_L					// 108 l
#define ASCII_6D	KEY_M					// 109 m
#define ASCII_6E	KEY_N					// 110 n
#define ASCII_6F	KEY_O					// 111 o
#define ASCII_70	KEY_P					// 112 p
#define ASCII_71	KEY_Q					// 113 q
#define ASCII_72	KEY_R					// 114 r
#define ASCII_73	KEY_S					// 115 s
#define ASCII_74	KEY_T					// 116 t
#define ASCII_75	KEY_U					// 117 u
#define ASCII_76	KEY_V					// 118 v
#define ASCII_77	KEY_W					// 119 w
#define ASCII_78	KEY_X					// 120 x
#define ASCII_79	KEY_Y					// 121 y
#define ASCII_7A	KEY_Z					// 122 z
#define ASCII_7B	KEY_QUOTE				// 123 {
#define ASCII_7C	KEY_TILDE				// 124 |
#define ASCII_7D	KEY_BACKSLASH				// 125 }
#define ASCII_7E	KEY_RIGHT_BRACE + ALTGR_MASK		// 126 ~
#define ASCII_7F	KEY_BACKSPACE				// 127

#define ISO_8859_1_A0	KEY_SPACE				// 160       Nonbreakng Space
#define ISO_8859_1_A1	KEY_EQUAL + SHIFT_MASK			// 161 ¡     Inverted Exclamation
#define ISO_8859_1_A2	0					// 162 ¢     Cent SIGN
#define ISO_8859_1_A3	0					// 163 £     Pound Sign
#define ISO_8859_1_A4	0					// 164 ¤     Currency Sign
#define ISO_8859_1_A5	0					// 165 ¥     YEN SIGN
#define ISO_8859_1_A6	0					// 166 ¦     BROKEN BAR
#define ISO_8859_1_A7	0					// 167 §     SECTION SIGN
#define ISO_8859_1_A8	DIAERESIS_BITS + KEY_SPACE		// 168 ¨     DIAERESIS
#define ISO_8859_1_A9	0					// 169 ©     COPYRIGHT SIGN
#define ISO_8859_1_AA	0					// 170 ª     FEMININE ORDINAL
#define ISO_8859_1_AB	0					// 171 «     LEFT DOUBLE ANGLE QUOTE
#define ISO_8859_1_AC	KEY_TILDE + ALTGR_MASK			// 172 ¬     NOT SIGN
#define ISO_8859_1_AD	0					// 173       SOFT HYPHEN
#define ISO_8859_1_AE	0					// 174 ®     REGISTERED SIGN
#define ISO_8859_1_AF	0					// 175 ¯     MACRON
#define ISO_8859_1_B0	KEY_TILDE + SHIFT_MASK			// 176 °     DEGREE SIGN
#define ISO_8859_1_B1	0					// 177 ±     PLUS-MINUS SIGN
#define ISO_8859_1_B2	0					// 178 ²     SUPERSCRIPT TWO
#define ISO_8859_1_B3	0					// 179 ³     SUPERSCRIPT THREE
#define ISO_8859_1_B4	ACUTE_ACCENT_BITS + KEY_SPACE		// 180 ´     ACUTE ACCENT
#define ISO_8859_1_B5	0					// 181 µ     MICRO SIGN
#define ISO_8859_1_B6	0					// 182 ¶     PILCROW SIGN
#define ISO_8859_1_B7	0					// 183 ·     MIDDLE DOT
#define ISO_8859_1_B8	0					// 184 ¸     CEDILLA
#define ISO_8859_1_B9	0					// 185 ¹     SUPERSCRIPT ONE
#define ISO_8859_1_BA	0					// 186 º     MASCULINE ORDINAL
#define ISO_8859_1_BB	0					// 187 »     RIGHT DOUBLE ANGLE QUOTE
#define ISO_8859_1_BC	0					// 188 ¼     FRACTION ONE QUARTER
#define ISO_8859_1_BD	0					// 189 ½     FRACTION ONE HALF
#define ISO_8859_1_BE	0					// 190 ¾     FRACTION THREE QUARTERS
#define ISO_8859_1_BF	KEY_EQUAL				// 191 ¿     INVERTED QUESTION MARK
#define ISO_8859_1_C0	GRAVE_ACCENT_BITS + KEY_A + SHIFT_MASK	// 192 À     A GRAVE
#define ISO_8859_1_C1	ACUTE_ACCENT_BITS + KEY_A + SHIFT_MASK	// 193 Á     A ACUTE
#define ISO_8859_1_C2	CIRCUMFLEX_BITS + KEY_A + SHIFT_MASK	// 194 Â     A CIRCUMFLEX
#define ISO_8859_1_C3	0					// 195 Ã     A TILDE
#define ISO_8859_1_C4	DIAERESIS_BITS + KEY_A + SHIFT_MASK	// 196 Ä     A DIAERESIS
#define ISO_8859_1_C5	0					// 197 Å     A RING ABOVE
#define ISO_8859_1_C6	0					// 198 Æ     AE
#define ISO_8859_1_C7	0					// 199 Ç     C CEDILLA
#define ISO_8859_1_C8	GRAVE_ACCENT_BITS + KEY_E + SHIFT_MASK	// 200 È     E GRAVE
#define ISO_8859_1_C9	ACUTE_ACCENT_BITS + KEY_E + SHIFT_MASK	// 201 É     E ACUTE
#define ISO_8859_1_CA	CIRCUMFLEX_BITS + KEY_E + SHIFT_MASK	// 202 Ê     E CIRCUMFLEX
#define ISO_8859_1_CB	DIAERESIS_BITS + KEY_E + SHIFT_MASK	// 203 Ë     E DIAERESIS
#define ISO_8859_1_CC	GRAVE_ACCENT_BITS + KEY_I + SHIFT_MASK	// 204 Ì     I GRAVE
#define ISO_8859_1_CD	ACUTE_ACCENT_BITS + KEY_I + SHIFT_MASK	// 205 Í     I ACUTE
#define ISO_8859_1_CE	CIRCUMFLEX_BITS + KEY_I + SHIFT_MASK	// 206 Î     I CIRCUMFLEX
#define ISO_8859_1_CF	DIAERESIS_BITS + KEY_I + SHIFT_MASK	// 207 Ï     I DIAERESIS
#define ISO_8859_1_D0	0					// 208 Ð     ETH
#define ISO_8859_1_D1	KEY_SEMICOLON + SHIFT_MASK		// 209 Ñ     N TILDE
#define ISO_8859_1_D2	GRAVE_ACCENT_BITS + KEY_O + SHIFT_MASK	// 210 Ò     O GRAVE
#define ISO_8859_1_D3	ACUTE_ACCENT_BITS + KEY_O + SHIFT_MASK	// 211 Ó     O ACUTE
#define ISO_8859_1_D4	CIRCUMFLEX_BITS + KEY_O + SHIFT_MASK	// 212 Ô     O CIRCUMFLEX
#define ISO_8859_1_D5	0					// 213 Õ     O TILDE
#define ISO_8859_1_D6	DIAERESIS_BITS + KEY_O + SHIFT_MASK	// 214 Ö     O DIAERESIS
#define ISO_8859_1_D7	0					// 215 ×     MULTIPLICATION
#define ISO_8859_1_D8	0					// 216 Ø     O STROKE
#define ISO_8859_1_D9	GRAVE_ACCENT_BITS + KEY_U + SHIFT_MASK	// 217 Ù     U GRAVE
#define ISO_8859_1_DA	ACUTE_ACCENT_BITS + KEY_U + SHIFT_MASK	// 218 Ú     U ACUTE
#define ISO_8859_1_DB	CIRCUMFLEX_BITS + KEY_U + SHIFT_MASK	// 219 Û     U CIRCUMFLEX
#define ISO_8859_1_DC	DIAERESIS_BITS + KEY_U + SHIFT_MASK	// 220 Ü     U DIAERESIS
#define ISO_8859_1_DD	ACUTE_ACCENT_BITS + KEY_Y + SHIFT_MASK	// 221 Ý     Y ACUTE
#define ISO_8859_1_DE	0					// 222 Þ     THORN
#define ISO_8859_1_DF	0					// 223 ß     SHARP S
#define ISO_8859_1_E0	GRAVE_ACCENT_BITS + KEY_A		// 224 à     a GRAVE
#define ISO_8859_1_E1	ACUTE_ACCENT_BITS + KEY_A		// 225 á     a ACUTE
#define ISO_8859_1_E2	CIRCUMFLEX_BITS + KEY_A			// 226 â     a CIRCUMFLEX
#define ISO_8859_1_E3	0					// 227 ã     a TILDE
#define ISO_8859_1_E4	DIAERESIS_BITS + KEY_A			// 228 ä     a DIAERESIS
#define ISO_8859_1_E5	0					// 229 å     a RING ABOVE
#define ISO_8859_1_E6	0					// 230 æ     ae
#define ISO_8859_1_E7	0					// 231 ç     c CEDILLA
#define ISO_8859_1_E8	GRAVE_ACCENT_BITS + KEY_E		// 232 è     e GRAVE
#define ISO_8859_1_E9	ACUTE_ACCENT_BITS + KEY_E		// 233 é     e ACUTE
#define ISO_8859_1_EA	CIRCUMFLEX_BITS + KEY_E			// 234 ê     e CIRCUMFLEX
#define ISO_8859_1_EB	DIAERESIS_BITS + KEY_E			// 235 ë     e DIAERESIS
#define ISO_8859_1_EC	GRAVE_ACCENT_BITS + KEY_I		// 236 ì     i GRAVE
#define ISO_8859_1_ED	ACUTE_ACCENT_BITS + KEY_I		// 237 í     i ACUTE
#define ISO_8859_1_EE	CIRCUMFLEX_BITS + KEY_I			// 238 î     i CIRCUMFLEX
#define ISO_8859_1_EF	DIAERESIS_BITS + KEY_I			// 239 ï     i DIAERESIS
#define ISO_8859_1_F0	0					// 240 ð     ETH
#define ISO_8859_1_F1	KEY_SEMICOLON				// 241 ñ     n TILDE
#define ISO_8859_1_F2	GRAVE_ACCENT_BITS + KEY_O		// 242 ò     o GRAVE
#define ISO_8859_1_F3	ACUTE_ACCENT_BITS + KEY_O		// 243 ó     o ACUTE
#define ISO_8859_1_F4	CIRCUMFLEX_BITS + KEY_O			// 244 ô     o CIRCUMFLEX
#define ISO_8859_1_F5	0					// 245 õ     o TILDE
#define ISO_8859_1_F6	DIAERESIS_BITS + KEY_O			// 246 ö     o DIAERESIS
#define ISO_8859_1_F7	0					// 247 ÷     DIVISION
#define ISO_8859_1_F8	0					// 248 ø     o STROKE
#define ISO_8859_1_F9	GRAVE_ACCENT_BITS + KEY_U		// 249 ù     u GRAVE
#define ISO_8859_1_FA	ACUTE_ACCENT_BITS + KEY_U		// 250 ú     u ACUTE
#define ISO_8859_1_FB	CIRCUMFLEX_BITS + KEY_U			// 251 û     u CIRCUMFLEX
#define ISO_8859_1_FC	DIAERESIS_BITS + KEY_U			// 252 ü     u DIAERESIS
#define ISO_8859_1_FD	ACUTE_ACCENT_BITS + KEY_Y		// 253 ý     y ACUTE
#define ISO_8859_1_FE	0					// 254 þ     THORN
#define ISO_8859_1_FF	DIAERESIS_BITS + KEY_Y			// 255 ÿ     y DIAERESIS

#endif // LAYOUT_SPANISH_LATIN_AMERICA



#ifdef LAYOUT_IRISH

#define SHIFT_MASK		0x0040
#define ALTGR_MASK		0x0080
#define DEADKEYS_MASK		0x0300
#define GRAVE_ACCENT_BITS	0x0100
#define ACUTE_ACCENT_BITS	0x0200
#define KEYCODE_TYPE		uint16_t
#define DEADKEY_ACUTE_ACCENT	KEY_QUOTE + ALTGR_MASK
#define DEADKEY_GRAVE_ACCENT	KEY_TILDE
#define KEY_NON_US_100		63

#define ASCII_20	KEY_SPACE				// 32  
#define ASCII_21	KEY_1 + SHIFT_MASK			// 33 !
#define ASCII_22	KEY_2 + SHIFT_MASK			// 34 "
#define ASCII_23	KEY_BACKSPACE				// 35 #
#define ASCII_24	KEY_4 + SHIFT_MASK			// 36 $
#define ASCII_25	KEY_5 + SHIFT_MASK			// 37 %
#define ASCII_26	KEY_7 + SHIFT_MASK			// 38 &
#define ASCII_27	KEY_QUOTE				// 39 '  
#define ASCII_28	KEY_9 + SHIFT_MASK			// 40 ( 
#define ASCII_29	KEY_0 + SHIFT_MASK			// 41 )
#define ASCII_2A	KEY_8 + SHIFT_MASK			// 42 *
#define ASCII_2B	KEY_EQUAL + SHIFT_MASK			// 43 +
#define ASCII_2C	KEY_COMMA				// 44 ,
#define ASCII_2D	KEY_MINUS				// 45 -
#define ASCII_2E	KEY_PERIOD				// 46 .
#define ASCII_2F	KEY_SLASH				// 47 /
#define ASCII_30	KEY_0					// 48 0
#define ASCII_31	KEY_1					// 49 1
#define ASCII_32	KEY_2					// 50 2
#define ASCII_33	KEY_3					// 51 3
#define ASCII_34	KEY_4					// 52 4
#define ASCII_35	KEY_5					// 53 5
#define ASCII_36	KEY_6					// 54 6
#define ASCII_37	KEY_7					// 55 7
#define ASCII_38	KEY_8					// 55 8
#define ASCII_39	KEY_9					// 57 9
#define ASCII_3A	KEY_SEMICOLON + SHIFT_MASK		// 58 :
#define ASCII_3B	KEY_SEMICOLON				// 59 ;
#define ASCII_3C	KEY_COMMA + SHIFT_MASK			// 60 <
#define ASCII_3D	KEY_EQUAL				// 61 =
#define ASCII_3E	KEY_PERIOD + SHIFT_MASK			// 62 >
#define ASCII_3F	KEY_SLASH + SHIFT_MASK			// 63 ?
#define ASCII_40	KEY_QUOTE + SHIFT_MASK			// 64 @
#define ASCII_41	KEY_A + SHIFT_MASK			// 65 A
#define ASCII_42	KEY_B + SHIFT_MASK			// 66 B
#define ASCII_43	KEY_C + SHIFT_MASK			// 67 C
#define ASCII_44	KEY_D + SHIFT_MASK			// 68 D
#define ASCII_45	KEY_E + SHIFT_MASK			// 69 E
#define ASCII_46	KEY_F + SHIFT_MASK			// 70 F
#define ASCII_47	KEY_G + SHIFT_MASK			// 71 G
#define ASCII_48	KEY_H + SHIFT_MASK			// 72 H
#define ASCII_49	KEY_I + SHIFT_MASK			// 73 I
#define ASCII_4A	KEY_J + SHIFT_MASK			// 74 J
#define ASCII_4B	KEY_K + SHIFT_MASK			// 75 K
#define ASCII_4C	KEY_L + SHIFT_MASK			// 76 L
#define ASCII_4D	KEY_M + SHIFT_MASK			// 77 M
#define ASCII_4E	KEY_N + SHIFT_MASK			// 78 N
#define ASCII_4F	KEY_O + SHIFT_MASK			// 79 O
#define ASCII_50	KEY_P + SHIFT_MASK			// 80 P
#define ASCII_51	KEY_Q + SHIFT_MASK			// 81 Q
#define ASCII_52	KEY_R + SHIFT_MASK			// 82 R
#define ASCII_53	KEY_S + SHIFT_MASK			// 83 S
#define ASCII_54	KEY_T + SHIFT_MASK			// 84 T
#define ASCII_55	KEY_U + SHIFT_MASK			// 85 U
#define ASCII_56	KEY_V + SHIFT_MASK			// 86 V
#define ASCII_57	KEY_W + SHIFT_MASK			// 87 W
#define ASCII_58	KEY_X + SHIFT_MASK			// 88 X
#define ASCII_59	KEY_Y + SHIFT_MASK			// 89 Y
#define ASCII_5A	KEY_Z + SHIFT_MASK			// 90 Z
#define ASCII_5B	KEY_LEFT_BRACE				// 91 [
#define ASCII_5C	KEY_NON_US_100				// 92 
#define ASCII_5D	KEY_RIGHT_BRACE				// 93 ]
#define ASCII_5E	KEY_6 + SHIFT_MASK			// 94 ^
#define ASCII_5F	KEY_MINUS + SHIFT_MASK			// 95 _
#define ASCII_60	KEY_QUOTE + SHIFT_MASK + ALTGR_MASK	// 96 `
#define ASCII_61	KEY_A					// 97 a
#define ASCII_62	KEY_B					// 98 b
#define ASCII_63	KEY_C					// 99 c
#define ASCII_64	KEY_D					// 100 d
#define ASCII_65	KEY_E					// 101 e
#define ASCII_66	KEY_F					// 102 f
#define ASCII_67	KEY_G					// 103 g
#define ASCII_68	KEY_H					// 104 h
#define ASCII_69	KEY_I					// 105 i
#define ASCII_6A	KEY_J					// 106 j
#define ASCII_6B	KEY_K					// 107 k
#define ASCII_6C	KEY_L					// 108 l
#define ASCII_6D	KEY_M					// 109 m
#define ASCII_6E	KEY_N					// 110 n
#define ASCII_6F	KEY_O					// 111 o
#define ASCII_70	KEY_P					// 112 p
#define ASCII_71	KEY_Q					// 113 q
#define ASCII_72	KEY_R					// 114 r
#define ASCII_73	KEY_S					// 115 s
#define ASCII_74	KEY_T					// 116 t
#define ASCII_75	KEY_U					// 117 u
#define ASCII_76	KEY_V					// 118 v
#define ASCII_77	KEY_W					// 119 w
#define ASCII_78	KEY_X					// 120 x
#define ASCII_79	KEY_Y					// 121 y
#define ASCII_7A	KEY_Z					// 122 z
#define ASCII_7B	KEY_LEFT_BRACE + SHIFT_MASK		// 123 {
#define ASCII_7C	KEY_NON_US_100 + SHIFT_MASK		// 124 |
#define ASCII_7D	KEY_RIGHT_BRACE + SHIFT_MASK		// 125 }
#define ASCII_7E	KEY_BACKSLASH + SHIFT_MASK		// 126 ~
#define ASCII_7F	KEY_BACKSPACE				// 127

#define ISO_8859_1_A0	KEY_SPACE				// 160       Nonbreakng Space
#define ISO_8859_1_A1	0					// 161 ¡     Inverted Exclamation
#define ISO_8859_1_A2	0					// 162 ¢     Cent SIGN
#define ISO_8859_1_A3	KEY_3 + SHIFT_MASK			// 163 £     Pound Sign
#define ISO_8859_1_A4	0					// 164 ¤     Currency or Euro Sign
#define ISO_8859_1_A5	0					// 165 ¥     YEN SIGN
#define ISO_8859_1_A6	KEY_TILDE + ALTGR_MASK			// 166 ¦     BROKEN BAR
#define ISO_8859_1_A7	0					// 167 §     SECTION SIGN
#define ISO_8859_1_A8	0					// 168 ¨     DIAERESIS
#define ISO_8859_1_A9	0					// 169 ©     COPYRIGHT SIGN
#define ISO_8859_1_AA	0					// 170 ª     FEMININE ORDINAL
#define ISO_8859_1_AB	0					// 171 «     LEFT DOUBLE ANGLE QUOTE
#define ISO_8859_1_AC	KEY_TILDE + SHIFT_MASK			// 172 ¬     NOT SIGN
#define ISO_8859_1_AD	0					// 173       SOFT HYPHEN
#define ISO_8859_1_AE	0					// 174 ®     REGISTERED SIGN
#define ISO_8859_1_AF	0					// 175 ¯     MACRON
#define ISO_8859_1_B0	0					// 176 °     DEGREE SIGN
#define ISO_8859_1_B1	0					// 177 ±     PLUS-MINUS SIGN
#define ISO_8859_1_B2	0					// 178 ²     SUPERSCRIPT TWO
#define ISO_8859_1_B3	0					// 179 ³     SUPERSCRIPT THREE
#define ISO_8859_1_B4	0					// 180 ´     ACUTE ACCENT
#define ISO_8859_1_B5	0					// 181 µ     MICRO SIGN
#define ISO_8859_1_B6	0					// 182 ¶     PILCROW SIGN
#define ISO_8859_1_B7	0					// 183 ·     MIDDLE DOT
#define ISO_8859_1_B8	0					// 184 ¸     CEDILLA
#define ISO_8859_1_B9	0					// 185 ¹     SUPERSCRIPT ONE
#define ISO_8859_1_BA	0					// 186 º     MASCULINE ORDINAL
#define ISO_8859_1_BB	0					// 187 »     RIGHT DOUBLE ANGLE QUOTE
#define ISO_8859_1_BC	0					// 188 ¼     FRACTION ONE QUARTER
#define ISO_8859_1_BD	0					// 189 ½     FRACTION ONE HALF
#define ISO_8859_1_BE	0					// 190 ¾     FRACTION THREE QUARTERS
#define ISO_8859_1_BF	0					// 191 ¿     INVERTED QUESTION MARK
#define ISO_8859_1_C0	GRAVE_ACCENT_BITS + KEY_A		// 192 À     A GRAVE
#define ISO_8859_1_C1	KEY_A + ALTGR_MASK + SHIFT_MASK		// 193 Á     A ACUTE
#define ISO_8859_1_C2	0					// 194 Â     A CIRCUMFLEX
#define ISO_8859_1_C3	0					// 195 Ã     A TILDE
#define ISO_8859_1_C4	0					// 196 Ä     A DIAERESIS
#define ISO_8859_1_C5	0					// 197 Å     A RING ABOVE
#define ISO_8859_1_C6	0					// 198 Æ     AE
#define ISO_8859_1_C7	0					// 199 Ç     C CEDILLA
#define ISO_8859_1_C8	GRAVE_ACCENT_BITS + KEY_E		// 200 È     E GRAVE
#define ISO_8859_1_C9	KEY_E + ALTGR_MASK + SHIFT_MASK		// 201 É     E ACUTE
#define ISO_8859_1_CA	0					// 202 Ê     E CIRCUMFLEX
#define ISO_8859_1_CB	0					// 203 Ë     E DIAERESIS
#define ISO_8859_1_CC	GRAVE_ACCENT_BITS + KEY_I		// 204 Ì     I GRAVE
#define ISO_8859_1_CD	KEY_I + ALTGR_MASK + SHIFT_MASK		// 205 Í     I ACUTE
#define ISO_8859_1_CE	0					// 206 Î     I CIRCUMFLEX
#define ISO_8859_1_CF	0					// 207 Ï     I DIAERESIS
#define ISO_8859_1_D0	0					// 208 Ð     ETH
#define ISO_8859_1_D1	0					// 209 Ñ     N TILDE
#define ISO_8859_1_D2	GRAVE_ACCENT_BITS + KEY_O		// 210 Ò     O GRAVE
#define ISO_8859_1_D3	KEY_O + ALTGR_MASK + SHIFT_MASK		// 211 Ó     O ACUTE
#define ISO_8859_1_D4	0					// 212 Ô     O CIRCUMFLEX
#define ISO_8859_1_D5	0					// 213 Õ     O TILDE
#define ISO_8859_1_D6	0					// 214 Ö     O DIAERESIS
#define ISO_8859_1_D7	0					// 215 ×     MULTIPLICATION
#define ISO_8859_1_D8	0					// 216 Ø     O STROKE
#define ISO_8859_1_D9	GRAVE_ACCENT_BITS + KEY_U		// 217 Ù     U GRAVE
#define ISO_8859_1_DA	KEY_U + ALTGR_MASK + SHIFT_MASK		// 218 Ú     U ACUTE
#define ISO_8859_1_DB	0					// 219 Û     U CIRCUMFLEX
#define ISO_8859_1_DC	0					// 220 Ü     U DIAERESIS
#define ISO_8859_1_DD	ACUTE_ACCENT_BITS + KEY_Y + SHIFT_MASK	// 221 Ý     Y ACUTE
#define ISO_8859_1_DE	0					// 222 Þ     THORN
#define ISO_8859_1_DF	0					// 223 ß     SHARP S
#define ISO_8859_1_E0	GRAVE_ACCENT_BITS + KEY_A		// 224 à     a GRAVE
#define ISO_8859_1_E1	KEY_A + ALTGR_MASK			// 225 á     a ACUTE
#define ISO_8859_1_E2	0					// 226 â     a CIRCUMFLEX
#define ISO_8859_1_E3	0					// 227 ã     a TILDE
#define ISO_8859_1_E4	0					// 228 ä     a DIAERESIS
#define ISO_8859_1_E5	0					// 229 å     a RING ABOVE
#define ISO_8859_1_E6	0					// 230 æ     ae
#define ISO_8859_1_E7	0					// 231 ç     c CEDILLA
#define ISO_8859_1_E8	GRAVE_ACCENT_BITS + KEY_E		// 232 è     e GRAVE
#define ISO_8859_1_E9	KEY_E + ALTGR_MASK			// 233 é     e ACUTE
#define ISO_8859_1_EA	0					// 234 ê     e CIRCUMFLEX
#define ISO_8859_1_EB	0					// 235 ë     e DIAERESIS
#define ISO_8859_1_EC	GRAVE_ACCENT_BITS + KEY_I		// 236 ì     i GRAVE
#define ISO_8859_1_ED	KEY_I + ALTGR_MASK			// 237 í     i ACUTE
#define ISO_8859_1_EE	0					// 238 î     i CIRCUMFLEX
#define ISO_8859_1_EF	0					// 239 ï     i DIAERESIS
#define ISO_8859_1_F0	0					// 240 ð     ETH
#define ISO_8859_1_F1	0					// 241 ñ     n TILDE
#define ISO_8859_1_F2	GRAVE_ACCENT_BITS + KEY_O		// 242 ò     o GRAVE
#define ISO_8859_1_F3	KEY_O + ALTGR_MASK			// 243 ó     o ACUTE
#define ISO_8859_1_F4	0					// 244 ô     o CIRCUMFLEX
#define ISO_8859_1_F5	0					// 245 õ     o TILDE
#define ISO_8859_1_F6	0					// 246 ö     o DIAERESIS
#define ISO_8859_1_F7	0					// 247 ÷     DIVISION
#define ISO_8859_1_F8	0					// 248 ø     o STROKE
#define ISO_8859_1_F9	GRAVE_ACCENT_BITS + KEY_U		// 249 ù     u GRAVE
#define ISO_8859_1_FA	KEY_U + ALTGR_MASK			// 250 ú     u ACUTE
#define ISO_8859_1_FB	0					// 251 û     u CIRCUMFLEX
#define ISO_8859_1_FC	0					// 252 ü     u DIAERESIS
#define ISO_8859_1_FD	ACUTE_ACCENT_BITS + KEY_Y		// 253 ý     y ACUTE
#define ISO_8859_1_FE	0					// 254 þ     THORN
#define ISO_8859_1_FF	0					// 255 ÿ     y DIAERESIS
#define UNICODE_20AC	KEY_4 + ALTGR_MASK			//     €     Euro Sign
// not yet implemented
#define UNICODE_EXTRA00	0x20AC
#define KEYCODE_EXTRA00	KEY_4 + ALTGR_MASK			// 20AC €    Euro Sign

#endif // LAYOUT_IRISH




#ifdef LAYOUT_ICELANDIC

#define SHIFT_MASK		0x0040
#define ALTGR_MASK		0x0080
#define DEADKEYS_MASK		0x0700
#define	CIRCUMFLEX_BITS		0x0100
#define ACUTE_ACCENT_BITS	0x0200
#define GRAVE_ACCENT_BITS	0x0300
#define DIAERESIS_BITS		0x0400
#define RING_ABOVE_BITS		0x0500
#define KEYCODE_TYPE		uint16_t
#define DEADKEY_CIRCUMFLEX	KEY_QUOTE + ALTGR_MASK
#define DEADKEY_ACUTE_ACCENT	KEY_QUOTE
#define DEADKEY_GRAVE_ACCENT	KEY_BACKSLASH + ALTGR_MASK
#define DEADKEY_DIAERESIS	KEY_TILDE + SHIFT_MASK
#define DEADKEY_RING_ABOVE	KEY_TILDE
#define KEY_NON_US_100		63

#define ASCII_20	KEY_SPACE				// 32  
#define ASCII_21	KEY_1 + SHIFT_MASK			// 33 !
#define ASCII_22	KEY_2 + SHIFT_MASK			// 34 "
#define ASCII_23	KEY_3 + SHIFT_MASK			// 35 #
#define ASCII_24	KEY_4 + SHIFT_MASK			// 36 $
#define ASCII_25	KEY_5 + SHIFT_MASK			// 37 %
#define ASCII_26	KEY_6 + SHIFT_MASK			// 38 &
#define ASCII_27	KEY_QUOTE + SHIFT_MASK			// 39 '  
#define ASCII_28	KEY_8 + SHIFT_MASK			// 40 ( 
#define ASCII_29	KEY_9 + SHIFT_MASK			// 41 )
#define ASCII_2A	KEY_BACKSLASH + SHIFT_MASK		// 42 *
#define ASCII_2B	KEY_BACKSLASH				// 43 +
#define ASCII_2C	KEY_COMMA				// 44 ,
#define ASCII_2D	KEY_EQUAL				// 45 -
#define ASCII_2E	KEY_PERIOD				// 46 .
#define ASCII_2F	KEY_7 + SHIFT_MASK			// 47 /
#define ASCII_30	KEY_0					// 48 0
#define ASCII_31	KEY_1					// 49 1
#define ASCII_32	KEY_2					// 50 2
#define ASCII_33	KEY_3					// 51 3
#define ASCII_34	KEY_4					// 52 4
#define ASCII_35	KEY_5					// 53 5
#define ASCII_36	KEY_6					// 54 6
#define ASCII_37	KEY_7					// 55 7
#define ASCII_38	KEY_8					// 55 8
#define ASCII_39	KEY_9					// 57 9
#define ASCII_3A	KEY_PERIOD + SHIFT_MASK			// 58 :
#define ASCII_3B	KEY_COMMA + SHIFT_MASK			// 59 ;
#define ASCII_3C	KEY_NON_US_100				// 60 <
#define ASCII_3D	KEY_0 + SHIFT_MASK			// 61 =
#define ASCII_3E	KEY_NON_US_100 + SHIFT_MASK		// 62 >
#define ASCII_3F	KEY_RIGHT_BRACE + SHIFT_MASK		// 63 ?
#define ASCII_40	KEY_Q + ALTGR_MASK			// 64 @
#define ASCII_41	KEY_A + SHIFT_MASK			// 65 A
#define ASCII_42	KEY_B + SHIFT_MASK			// 66 B
#define ASCII_43	KEY_C + SHIFT_MASK			// 67 C
#define ASCII_44	KEY_D + SHIFT_MASK			// 68 D
#define ASCII_45	KEY_E + SHIFT_MASK			// 69 E
#define ASCII_46	KEY_F + SHIFT_MASK			// 70 F
#define ASCII_47	KEY_G + SHIFT_MASK			// 71 G
#define ASCII_48	KEY_H + SHIFT_MASK			// 72 H
#define ASCII_49	KEY_I + SHIFT_MASK			// 73 I
#define ASCII_4A	KEY_J + SHIFT_MASK			// 74 J
#define ASCII_4B	KEY_K + SHIFT_MASK			// 75 K
#define ASCII_4C	KEY_L + SHIFT_MASK			// 76 L
#define ASCII_4D	KEY_M + SHIFT_MASK			// 77 M
#define ASCII_4E	KEY_N + SHIFT_MASK			// 78 N
#define ASCII_4F	KEY_O + SHIFT_MASK			// 79 O
#define ASCII_50	KEY_P + SHIFT_MASK			// 80 P
#define ASCII_51	KEY_Q + SHIFT_MASK			// 81 Q
#define ASCII_52	KEY_R + SHIFT_MASK			// 82 R
#define ASCII_53	KEY_S + SHIFT_MASK			// 83 S
#define ASCII_54	KEY_T + SHIFT_MASK			// 84 T
#define ASCII_55	KEY_U + SHIFT_MASK			// 85 U
#define ASCII_56	KEY_V + SHIFT_MASK			// 86 V
#define ASCII_57	KEY_W + SHIFT_MASK			// 87 W
#define ASCII_58	KEY_X + SHIFT_MASK			// 88 X
#define ASCII_59	KEY_Y + SHIFT_MASK			// 89 Y
#define ASCII_5A	KEY_Z + SHIFT_MASK			// 90 Z
#define ASCII_5B	KEY_8 + ALTGR_MASK			// 91 [
#define ASCII_5C	KEY_MINUS + ALTGR_MASK			// 92 
#define ASCII_5D	KEY_9 + ALTGR_MASK			// 93 ]
#define ASCII_5E	CIRCUMFLEX_BITS + KEY_SPACE		// 94 ^
#define ASCII_5F	KEY_EQUAL + SHIFT_MASK			// 95 _
#define ASCII_60	GRAVE_ACCENT_BITS + KEY_SPACE		// 96 `
#define ASCII_61	KEY_A					// 97 a
#define ASCII_62	KEY_B					// 98 b
#define ASCII_63	KEY_C					// 99 c
#define ASCII_64	KEY_D					// 100 d
#define ASCII_65	KEY_E					// 101 e
#define ASCII_66	KEY_F					// 102 f
#define ASCII_67	KEY_G					// 103 g
#define ASCII_68	KEY_H					// 104 h
#define ASCII_69	KEY_I					// 105 i
#define ASCII_6A	KEY_J					// 106 j
#define ASCII_6B	KEY_K					// 107 k
#define ASCII_6C	KEY_L					// 108 l
#define ASCII_6D	KEY_M					// 109 m
#define ASCII_6E	KEY_N					// 110 n
#define ASCII_6F	KEY_O					// 111 o
#define ASCII_70	KEY_P					// 112 p
#define ASCII_71	KEY_Q					// 113 q
#define ASCII_72	KEY_R					// 114 r
#define ASCII_73	KEY_S					// 115 s
#define ASCII_74	KEY_T					// 116 t
#define ASCII_75	KEY_U					// 117 u
#define ASCII_76	KEY_V					// 118 v
#define ASCII_77	KEY_W					// 119 w
#define ASCII_78	KEY_X					// 120 x
#define ASCII_79	KEY_Y					// 121 y
#define ASCII_7A	KEY_Z					// 122 z
#define ASCII_7B	KEY_7 + ALTGR_MASK			// 123 {
#define ASCII_7C	KEY_NON_US_100 + ALTGR_MASK		// 124 |
#define ASCII_7D	KEY_0 + ALTGR_MASK			// 125 }
#define ASCII_7E	KEY_RIGHT_BRACE + ALTGR_MASK		// 126 ~
#define ASCII_7F	KEY_BACKSPACE				// 127

#define ISO_8859_1_A0	KEY_SPACE				// 160       Nonbreakng Space
#define ISO_8859_1_A1	0					// 161 ¡     Inverted Exclamation
#define ISO_8859_1_A2	0					// 162 ¢     Cent SIGN
#define ISO_8859_1_A3	0					// 163 £     Pound Sign
#define ISO_8859_1_A4	0					// 164 ¤     Currency or Euro Sign
#define ISO_8859_1_A5	0					// 165 ¥     YEN SIGN
#define ISO_8859_1_A6	0					// 166 ¦     BROKEN BAR			??
#define ISO_8859_1_A7	0					// 167 §     SECTION SIGN
#define ISO_8859_1_A8	DIAERESIS_BITS + KEY_SPACE		// 168 ¨     DIAERESIS
#define ISO_8859_1_A9	0					// 169 ©     COPYRIGHT SIGN
#define ISO_8859_1_AA	0					// 170 ª     FEMININE ORDINAL
#define ISO_8859_1_AB	0					// 171 «     LEFT DOUBLE ANGLE QUOTE
#define ISO_8859_1_AC	0					// 172 ¬     NOT SIGN			??
#define ISO_8859_1_AD	0					// 173       SOFT HYPHEN
#define ISO_8859_1_AE	0					// 174 ®     REGISTERED SIGN
#define ISO_8859_1_AF	0					// 175 ¯     MACRON
#define ISO_8859_1_B0	KEY_TILDE + ALTGR_MASK			// 176 °     DEGREE SIGN
#define ISO_8859_1_B1	0					// 177 ±     PLUS-MINUS SIGN
#define ISO_8859_1_B2	0					// 178 ²     SUPERSCRIPT TWO
#define ISO_8859_1_B3	0					// 179 ³     SUPERSCRIPT THREE
#define ISO_8859_1_B4	ACUTE_ACCENT_BITS + KEY_SPACE		// 180 ´     ACUTE ACCENT
#define ISO_8859_1_B5	KEY_M + ALTGR_MASK			// 181 µ     MICRO SIGN
#define ISO_8859_1_B6	0					// 182 ¶     PILCROW SIGN
#define ISO_8859_1_B7	0					// 183 ·     MIDDLE DOT
#define ISO_8859_1_B8	0					// 184 ¸     CEDILLA
#define ISO_8859_1_B9	0					// 185 ¹     SUPERSCRIPT ONE
#define ISO_8859_1_BA	0					// 186 º     MASCULINE ORDINAL
#define ISO_8859_1_BB	0					// 187 »     RIGHT DOUBLE ANGLE QUOTE
#define ISO_8859_1_BC	0					// 188 ¼     FRACTION ONE QUARTER
#define ISO_8859_1_BD	0					// 189 ½     FRACTION ONE HALF
#define ISO_8859_1_BE	0					// 190 ¾     FRACTION THREE QUARTERS
#define ISO_8859_1_BF	0					// 191 ¿     INVERTED QUESTION MARK
#define ISO_8859_1_C0	GRAVE_ACCENT_BITS + KEY_A + SHIFT_MASK	// 192 À     A GRAVE
#define ISO_8859_1_C1	ACUTE_ACCENT_BITS + KEY_A + ALTGR_MASK	// 193 Á     A ACUTE
#define ISO_8859_1_C2	CIRCUMFLEX_BITS	+ KEY_A + SHIFT_MASK	// 194 Â     A CIRCUMFLEX
#define ISO_8859_1_C3	0					// 195 Ã     A TILDE
#define ISO_8859_1_C4	DIAERESIS_BITS + KEY_A + SHIFT_MASK	// 196 Ä     A DIAERESIS
#define ISO_8859_1_C5	RING_ABOVE_BITS + KEY_A + SHIFT_MASK	// 197 Å     A RING ABOVE
#define ISO_8859_1_C6	KEY_SEMICOLON + SHIFT_MASK		// 198 Æ     AE
#define ISO_8859_1_C7	0					// 199 Ç     C CEDILLA
#define ISO_8859_1_C8	GRAVE_ACCENT_BITS + KEY_E + SHIFT_MASK	// 200 È     E GRAVE
#define ISO_8859_1_C9	ACUTE_ACCENT_BITS + KEY_E + SHIFT_MASK	// 201 É     E ACUTE
#define ISO_8859_1_CA	CIRCUMFLEX_BITS + KEY_E + SHIFT_MASK	// 202 Ê     E CIRCUMFLEX
#define ISO_8859_1_CB	DIAERESIS_BITS + KEY_E + SHIFT_MASK	// 203 Ë     E DIAERESIS
#define ISO_8859_1_CC	GRAVE_ACCENT_BITS + KEY_I + SHIFT_MASK	// 204 Ì     I GRAVE
#define ISO_8859_1_CD	ACUTE_ACCENT_BITS + KEY_I + SHIFT_MASK	// 205 Í     I ACUTE
#define ISO_8859_1_CE	CIRCUMFLEX_BITS + KEY_I + SHIFT_MASK	// 206 Î     I CIRCUMFLEX
#define ISO_8859_1_CF	DIAERESIS_BITS + KEY_I + SHIFT_MASK	// 207 Ï     I DIAERESIS
#define ISO_8859_1_D0	KEY_LEFT_BRACE + SHIFT_MASK		// 208 Ð     ETH
#define ISO_8859_1_D1	0					// 209 Ñ     N TILDE
#define ISO_8859_1_D2	GRAVE_ACCENT_BITS + KEY_O + SHIFT_MASK	// 210 Ò     O GRAVE
#define ISO_8859_1_D3	ACUTE_ACCENT_BITS + KEY_O + SHIFT_MASK	// 211 Ó     O ACUTE
#define ISO_8859_1_D4	CIRCUMFLEX_BITS + KEY_O + SHIFT_MASK	// 212 Ô     O CIRCUMFLEX
#define ISO_8859_1_D5	0					// 213 Õ     O TILDE
#define ISO_8859_1_D6	DIAERESIS_BITS + KEY_O + SHIFT_MASK	// 214 Ö     O DIAERESIS
#define ISO_8859_1_D7	0					// 215 ×     MULTIPLICATION
#define ISO_8859_1_D8	0					// 216 Ø     O STROKE
#define ISO_8859_1_D9	GRAVE_ACCENT_BITS + KEY_U + SHIFT_MASK	// 217 Ù     U GRAVE
#define ISO_8859_1_DA	ACUTE_ACCENT_BITS + KEY_U + SHIFT_MASK	// 218 Ú     U ACUTE
#define ISO_8859_1_DB	CIRCUMFLEX_BITS + KEY_U + SHIFT_MASK	// 219 Û     U CIRCUMFLEX
#define ISO_8859_1_DC	DIAERESIS_BITS + KEY_U + SHIFT_MASK	// 220 Ü     U DIAERESIS
#define ISO_8859_1_DD	ACUTE_ACCENT_BITS + KEY_Y + SHIFT_MASK	// 221 Ý     Y ACUTE
#define ISO_8859_1_DE	KEY_SLASH + SHIFT_MASK			// 222 Þ     THORN
#define ISO_8859_1_DF	0					// 223 ß     SHARP S
#define ISO_8859_1_E0	GRAVE_ACCENT_BITS + KEY_A		// 224 à     a GRAVE
#define ISO_8859_1_E1	ACUTE_ACCENT_BITS + KEY_A		// 225 á     a ACUTE
#define ISO_8859_1_E2	CIRCUMFLEX_BITS + KEY_A			// 226 â     a CIRCUMFLEX
#define ISO_8859_1_E3	0					// 227 ã     a TILDE
#define ISO_8859_1_E4	DIAERESIS_BITS + KEY_A			// 228 ä     a DIAERESIS
#define ISO_8859_1_E5	RING_ABOVE_BITS + KEY_A			// 229 å     a RING ABOVE
#define ISO_8859_1_E6	KEY_SEMICOLON				// 230 æ     ae
#define ISO_8859_1_E7	0					// 231 ç     c CEDILLA
#define ISO_8859_1_E8	GRAVE_ACCENT_BITS + KEY_E		// 232 è     e GRAVE
#define ISO_8859_1_E9	ACUTE_ACCENT_BITS + KEY_E		// 233 é     e ACUTE
#define ISO_8859_1_EA	CIRCUMFLEX_BITS + KEY_E			// 234 ê     e CIRCUMFLEX
#define ISO_8859_1_EB	DIAERESIS_BITS + KEY_E			// 235 ë     e DIAERESIS
#define ISO_8859_1_EC	GRAVE_ACCENT_BITS + KEY_I		// 236 ì     i GRAVE
#define ISO_8859_1_ED	ACUTE_ACCENT_BITS + KEY_I		// 237 í     i ACUTE
#define ISO_8859_1_EE	CIRCUMFLEX_BITS + KEY_I			// 238 î     i CIRCUMFLEX
#define ISO_8859_1_EF	DIAERESIS_BITS + KEY_I			// 239 ï     i DIAERESIS
#define ISO_8859_1_F0	KEY_LEFT_BRACE				// 240 ð     ETH
#define ISO_8859_1_F1	0					// 241 ñ     n TILDE
#define ISO_8859_1_F2	GRAVE_ACCENT_BITS + KEY_O		// 242 ò     o GRAVE
#define ISO_8859_1_F3	ACUTE_ACCENT_BITS + KEY_O		// 243 ó     o ACUTE
#define ISO_8859_1_F4	CIRCUMFLEX_BITS + KEY_O			// 244 ô     o CIRCUMFLEX
#define ISO_8859_1_F5	0					// 245 õ     o TILDE
#define ISO_8859_1_F6	DIAERESIS_BITS + KEY_O			// 246 ö     o DIAERESIS
#define ISO_8859_1_F7	0					// 247 ÷     DIVISION
#define ISO_8859_1_F8	0					// 248 ø     o STROKE
#define ISO_8859_1_F9	GRAVE_ACCENT_BITS + KEY_U		// 249 ù     u GRAVE
#define ISO_8859_1_FA	ACUTE_ACCENT_BITS + KEY_U		// 250 ú     u ACUTE
#define ISO_8859_1_FB	CIRCUMFLEX_BITS + KEY_U			// 251 û     u CIRCUMFLEX
#define ISO_8859_1_FC	DIAERESIS_BITS + KEY_U			// 252 ü     u DIAERESIS
#define ISO_8859_1_FD	ACUTE_ACCENT_BITS + KEY_Y		// 253 ý     y ACUTE
#define ISO_8859_1_FE	KEY_SLASH				// 254 þ     THORN
#define ISO_8859_1_FF	DIAERESIS_BITS + KEY_Y			// 255 ÿ     y DIAERESIS
#define UNICODE_20AC	KEY_5 + ALTGR_MASK			//     €     Euro Sign
// not yet implemented
#define UNICODE_EXTRA00	0x20AC
#define KEYCODE_EXTRA00	KEY_5 + ALTGR_MASK			// 20AC €    Euro Sign

#endif // LAYOUT_ICELANDIC



#ifdef LAYOUT_TURKISH
// http://forum.pjrc.com/threads/18781-Turkish-Language-Support

#define SHIFT_MASK		0x0040
#define ALTGR_MASK		0x0080
#define DEADKEYS_MASK		0x0700
#define	CIRCUMFLEX_BITS		0x0100
#define ACUTE_ACCENT_BITS	0x0200
#define GRAVE_ACCENT_BITS	0x0300
#define TILDE_BITS		0x0400
#define DIAERESIS_BITS		0x0500
#define KEYCODE_TYPE		uint16_t
#define DEADKEY_CIRCUMFLEX	KEY_3 + SHIFT_MASK
#define DEADKEY_ACUTE_ACCENT	KEY_SEMICOLON + ALTGR_MASK
#define DEADKEY_GRAVE_ACCENT	KEY_BACKSLASH + ALTGR_MASK
#define DEADKEY_TILDE		KEY_RIGHT_BRACE + ALTGR_MASK
#define DEADKEY_DIAERESIS	KEY_LEFT_BRACE + ALTGR_MASK

#define ASCII_20	KEY_SPACE				// 32  
#define ASCII_21	KEY_1 + SHIFT_MASK			// 33 !
#define ASCII_22	KEY_TILDE				// 34 "
#define ASCII_23	KEY_3 + ALTGR_MASK			// 35 #
#define ASCII_24	KEY_4 + ALTGR_MASK			// 36 $
#define ASCII_25	KEY_5 + SHIFT_MASK			// 37 %
#define ASCII_26	KEY_6 + SHIFT_MASK			// 38 &
#define ASCII_27	KEY_2 + SHIFT_MASK			// 39 '  
#define ASCII_28	KEY_8 + SHIFT_MASK			// 40 ( 
#define ASCII_29	KEY_9 + SHIFT_MASK			// 41 )
#define ASCII_2A	KEY_MINUS				// 42 *
#define ASCII_2B	KEY_4 + SHIFT_MASK			// 43 +
#define ASCII_2C	KEY_BACKSLASH				// 44 ,
#define ASCII_2D	KEY_EQUAL				// 45 -
#define ASCII_2E	KEY_SLASH				// 46 .
#define ASCII_2F	KEY_7 + SHIFT_MASK			// 47 /
#define ASCII_30	KEY_0					// 48 0
#define ASCII_31	KEY_1					// 49 1
#define ASCII_32	KEY_2					// 50 2
#define ASCII_33	KEY_3					// 51 3
#define ASCII_34	KEY_4					// 52 4
#define ASCII_35	KEY_5					// 53 5
#define ASCII_36	KEY_6					// 54 6
#define ASCII_37	KEY_7					// 55 7
#define ASCII_38	KEY_8					// 55 8
#define ASCII_39	KEY_9					// 57 9
#define ASCII_3A	KEY_SLASH + SHIFT_MASK			// 58 :
#define ASCII_3B	KEY_BACKSLASH + SHIFT_MASK		// 59 ;
#define ASCII_3C	KEY_TILDE + ALTGR_MASK			// 60 <
#define ASCII_3D	KEY_0 + SHIFT_MASK			// 61 =
#define ASCII_3E	KEY_1 + ALTGR_MASK			// 62 >
#define ASCII_3F	KEY_MINUS + SHIFT_MASK			// 63 ?
#define ASCII_40	KEY_Q + ALTGR_MASK			// 64 @
#define ASCII_41	KEY_A + SHIFT_MASK			// 65 A
#define ASCII_42	KEY_B + SHIFT_MASK			// 66 B
#define ASCII_43	KEY_C + SHIFT_MASK			// 67 C
#define ASCII_44	KEY_D + SHIFT_MASK			// 68 D
#define ASCII_45	KEY_E + SHIFT_MASK			// 69 E
#define ASCII_46	KEY_F + SHIFT_MASK			// 70 F
#define ASCII_47	KEY_G + SHIFT_MASK			// 71 G
#define ASCII_48	KEY_H + SHIFT_MASK			// 72 H
#define ASCII_49	KEY_I + SHIFT_MASK			// 73 I
#define ASCII_4A	KEY_J + SHIFT_MASK			// 74 J
#define ASCII_4B	KEY_K + SHIFT_MASK			// 75 K
#define ASCII_4C	KEY_L + SHIFT_MASK			// 76 L
#define ASCII_4D	KEY_M + SHIFT_MASK			// 77 M
#define ASCII_4E	KEY_N + SHIFT_MASK			// 78 N
#define ASCII_4F	KEY_O + SHIFT_MASK			// 79 O
#define ASCII_50	KEY_P + SHIFT_MASK			// 80 P
#define ASCII_51	KEY_Q + SHIFT_MASK			// 81 Q
#define ASCII_52	KEY_R + SHIFT_MASK			// 82 R
#define ASCII_53	KEY_S + SHIFT_MASK			// 83 S
#define ASCII_54	KEY_T + SHIFT_MASK			// 84 T
#define ASCII_55	KEY_U + SHIFT_MASK			// 85 U
#define ASCII_56	KEY_V + SHIFT_MASK			// 86 V
#define ASCII_57	KEY_W + SHIFT_MASK			// 87 W
#define ASCII_58	KEY_X + SHIFT_MASK			// 88 X
#define ASCII_59	KEY_Y + SHIFT_MASK			// 89 Y
#define ASCII_5A	KEY_Z + SHIFT_MASK			// 90 Z
#define ASCII_5B	KEY_8 + ALTGR_MASK			// 91 [
#define ASCII_5C	KEY_MINUS + ALTGR_MASK			// 92 
#define ASCII_5D	KEY_9 + ALTGR_MASK			// 93 ]
#define ASCII_5E	CIRCUMFLEX_BITS + KEY_SPACE		// 94 ^
#define ASCII_5F	KEY_EQUAL + SHIFT_MASK			// 95 _
#define ASCII_60	GRAVE_ACCENT_BITS + KEY_SPACE		// 96 `
#define ASCII_61	KEY_A					// 97 a
#define ASCII_62	KEY_B					// 98 b
#define ASCII_63	KEY_C					// 99 c
#define ASCII_64	KEY_D					// 100 d
#define ASCII_65	KEY_E					// 101 e
#define ASCII_66	KEY_F					// 102 f
#define ASCII_67	KEY_G					// 103 g
#define ASCII_68	KEY_H					// 104 h
#define ASCII_69	KEY_QUOTE				// 105 i
#define ASCII_6A	KEY_J					// 106 j
#define ASCII_6B	KEY_K					// 107 k
#define ASCII_6C	KEY_L					// 108 l
#define ASCII_6D	KEY_M					// 109 m
#define ASCII_6E	KEY_N					// 110 n
#define ASCII_6F	KEY_O					// 111 o
#define ASCII_70	KEY_P					// 112 p
#define ASCII_71	KEY_Q					// 113 q
#define ASCII_72	KEY_R					// 114 r
#define ASCII_73	KEY_S					// 115 s
#define ASCII_74	KEY_T					// 116 t
#define ASCII_75	KEY_U					// 117 u
#define ASCII_76	KEY_V					// 118 v
#define ASCII_77	KEY_W					// 119 w
#define ASCII_78	KEY_X					// 120 x
#define ASCII_79	KEY_Y					// 121 y
#define ASCII_7A	KEY_Z					// 122 z
#define ASCII_7B	KEY_7 + ALTGR_MASK			// 123 {
#define ASCII_7C	KEY_EQUAL + ALTGR_MASK			// 124 |
#define ASCII_7D	KEY_0 + ALTGR_MASK			// 125 }
#define ASCII_7E	TILDE_BITS + KEY_SPACE			// 126 ~
#define ASCII_7F	KEY_BACKSPACE				// 127

#define ISO_8859_1_A0	KEY_SPACE				// 160       Nonbreakng Space
#define ISO_8859_1_A1	0					// 161 ¡     Inverted Exclamation
#define ISO_8859_1_A2	0					// 162 ¢     Cent SIGN
#define ISO_8859_1_A3	KEY_2 + ALTGR_MASK			// 163 £     Pound Sign
#define ISO_8859_1_A4	0					// 164 ¤     Currency or Euro Sign
#define ISO_8859_1_A5	0					// 165 ¥     YEN SIGN
#define ISO_8859_1_A6	0					// 166 ¦     BROKEN BAR			??
#define ISO_8859_1_A7	0					// 167 §     SECTION SIGN
#define ISO_8859_1_A8	DIAERESIS_BITS + KEY_SPACE		// 168 ¨     DIAERESIS
#define ISO_8859_1_A9	0					// 169 ©     COPYRIGHT SIGN
#define ISO_8859_1_AA	0					// 170 ª     FEMININE ORDINAL
#define ISO_8859_1_AB	0					// 171 «     LEFT DOUBLE ANGLE QUOTE
#define ISO_8859_1_AC	0					// 172 ¬     NOT SIGN			??
#define ISO_8859_1_AD	0					// 173       SOFT HYPHEN
#define ISO_8859_1_AE	0					// 174 ®     REGISTERED SIGN
#define ISO_8859_1_AF	0					// 175 ¯     MACRON
#define ISO_8859_1_B0	0					// 176 °     DEGREE SIGN
#define ISO_8859_1_B1	0					// 177 ±     PLUS-MINUS SIGN
#define ISO_8859_1_B2	0					// 178 ²     SUPERSCRIPT TWO
#define ISO_8859_1_B3	0					// 179 ³     SUPERSCRIPT THREE
#define ISO_8859_1_B4	ACUTE_ACCENT_BITS + KEY_SPACE		// 180 ´     ACUTE ACCENT
#define ISO_8859_1_B5	0					// 181 µ     MICRO SIGN
#define ISO_8859_1_B6	0					// 182 ¶     PILCROW SIGN
#define ISO_8859_1_B7	0					// 183 ·     MIDDLE DOT
#define ISO_8859_1_B8	0					// 184 ¸     CEDILLA
#define ISO_8859_1_B9	0					// 185 ¹     SUPERSCRIPT ONE
#define ISO_8859_1_BA	0					// 186 º     MASCULINE ORDINAL
#define ISO_8859_1_BB	0					// 187 »     RIGHT DOUBLE ANGLE QUOTE
#define ISO_8859_1_BC	0					// 188 ¼     FRACTION ONE QUARTER
#define ISO_8859_1_BD	KEY_5 + ALTGR_MASK			// 189 ½     FRACTION ONE HALF
#define ISO_8859_1_BE	0					// 190 ¾     FRACTION THREE QUARTERS
#define ISO_8859_1_BF	0					// 191 ¿     INVERTED QUESTION MARK
#define ISO_8859_1_C0	GRAVE_ACCENT_BITS + KEY_A + SHIFT_MASK	// 192 À     A GRAVE
#define ISO_8859_1_C1	ACUTE_ACCENT_BITS + KEY_A + SHIFT_MASK	// 193 Á     A ACUTE
#define ISO_8859_1_C2	CIRCUMFLEX_BITS	+ KEY_A + SHIFT_MASK	// 194 Â     A CIRCUMFLEX
#define ISO_8859_1_C3	TILDE_BITS + KEY_A + SHIFT_MASK		// 195 Ã     A TILDE
#define ISO_8859_1_C4	DIAERESIS_BITS + KEY_A + SHIFT_MASK	// 196 Ä     A DIAERESIS
#define ISO_8859_1_C5	0					// 197 Å     A RING ABOVE
#define ISO_8859_1_C6	KEY_A + ALTGR_MASK			// 198 Æ     AE
#define ISO_8859_1_C7	KEY_PERIOD + SHIFT_MASK			// 199 Ç     C CEDILLA
#define ISO_8859_1_C8	GRAVE_ACCENT_BITS + KEY_E + SHIFT_MASK	// 200 È     E GRAVE
#define ISO_8859_1_C9	ACUTE_ACCENT_BITS + KEY_E + SHIFT_MASK	// 201 É     E ACUTE
#define ISO_8859_1_CA	CIRCUMFLEX_BITS + KEY_E + SHIFT_MASK	// 202 Ê     E CIRCUMFLEX
#define ISO_8859_1_CB	DIAERESIS_BITS + KEY_E + SHIFT_MASK	// 203 Ë     E DIAERESIS
#define ISO_8859_1_CC	GRAVE_ACCENT_BITS + KEY_I + SHIFT_MASK	// 204 Ì     I GRAVE
#define ISO_8859_1_CD	ACUTE_ACCENT_BITS + KEY_I + SHIFT_MASK	// 205 Í     I ACUTE
#define ISO_8859_1_CE	CIRCUMFLEX_BITS + KEY_I + SHIFT_MASK	// 206 Î     I CIRCUMFLEX
#define ISO_8859_1_CF	DIAERESIS_BITS + KEY_I + SHIFT_MASK	// 207 Ï     I DIAERESIS
#define ISO_8859_1_D0	0					// 208 Ð     ETH
#define ISO_8859_1_D1	TILDE_BITS + KEY_N + SHIFT_MASK		// 209 Ñ     N TILDE
#define ISO_8859_1_D2	GRAVE_ACCENT_BITS + KEY_O + SHIFT_MASK	// 210 Ò     O GRAVE
#define ISO_8859_1_D3	ACUTE_ACCENT_BITS + KEY_O + SHIFT_MASK	// 211 Ó     O ACUTE
#define ISO_8859_1_D4	CIRCUMFLEX_BITS + KEY_O + SHIFT_MASK	// 212 Ô     O CIRCUMFLEX
#define ISO_8859_1_D5	TILDE_BITS + KEY_O + SHIFT_MASK		// 213 Õ     O TILDE
#define ISO_8859_1_D6	KEY_COMMA + SHIFT_MASK			// 214 Ö     O DIAERESIS
#define ISO_8859_1_D7	0					// 215 ×     MULTIPLICATION
#define ISO_8859_1_D8	0					// 216 Ø     O STROKE
#define ISO_8859_1_D9	GRAVE_ACCENT_BITS + KEY_U + SHIFT_MASK	// 217 Ù     U GRAVE
#define ISO_8859_1_DA	ACUTE_ACCENT_BITS + KEY_U + SHIFT_MASK	// 218 Ú     U ACUTE
#define ISO_8859_1_DB	CIRCUMFLEX_BITS + KEY_U + SHIFT_MASK	// 219 Û     U CIRCUMFLEX
#define ISO_8859_1_DC	KEY_RIGHT_BRACE + SHIFT_MASK		// 220 Ü     U DIAERESIS
#define ISO_8859_1_DD	ACUTE_ACCENT_BITS + KEY_Y + SHIFT_MASK	// 221 Ý     Y ACUTE
#define ISO_8859_1_DE	0					// 222 Þ     THORN
#define ISO_8859_1_DF	KEY_S + ALTGR_MASK			// 223 ß     SHARP S
#define ISO_8859_1_E0	GRAVE_ACCENT_BITS + KEY_A		// 224 à     a GRAVE
#define ISO_8859_1_E1	ACUTE_ACCENT_BITS + KEY_A		// 225 á     a ACUTE
#define ISO_8859_1_E2	CIRCUMFLEX_BITS + KEY_A			// 226 â     a CIRCUMFLEX
#define ISO_8859_1_E3	TILDE_BITS + KEY_A			// 227 ã     a TILDE
#define ISO_8859_1_E4	DIAERESIS_BITS + KEY_A			// 228 ä     a DIAERESIS
#define ISO_8859_1_E5	0					// 229 å     a RING ABOVE
#define ISO_8859_1_E6	0					// 230 æ     ae
#define ISO_8859_1_E7	KEY_PERIOD				// 231 ç     c CEDILLA
#define ISO_8859_1_E8	GRAVE_ACCENT_BITS + KEY_E		// 232 è     e GRAVE
#define ISO_8859_1_E9	KEY_TILDE + SHIFT_MASK			// 233 é     e ACUTE
#define ISO_8859_1_EA	CIRCUMFLEX_BITS + KEY_E			// 234 ê     e CIRCUMFLEX
#define ISO_8859_1_EB	DIAERESIS_BITS + KEY_E			// 235 ë     e DIAERESIS
#define ISO_8859_1_EC	GRAVE_ACCENT_BITS + KEY_I		// 236 ì     i GRAVE
#define ISO_8859_1_ED	ACUTE_ACCENT_BITS + KEY_I		// 237 í     i ACUTE
#define ISO_8859_1_EE	CIRCUMFLEX_BITS + KEY_I			// 238 î     i CIRCUMFLEX
#define ISO_8859_1_EF	DIAERESIS_BITS + KEY_I			// 239 ï     i DIAERESIS
#define ISO_8859_1_F0	0					// 240 ð     ETH
#define ISO_8859_1_F1	TILDE_BITS + KEY_N			// 241 ñ     n TILDE
#define ISO_8859_1_F2	GRAVE_ACCENT_BITS + KEY_O		// 242 ò     o GRAVE
#define ISO_8859_1_F3	ACUTE_ACCENT_BITS + KEY_O		// 243 ó     o ACUTE
#define ISO_8859_1_F4	CIRCUMFLEX_BITS + KEY_O			// 244 ô     o CIRCUMFLEX
#define ISO_8859_1_F5	TILDE_BITS + KEY_O 			// 245 õ     o TILDE
#define ISO_8859_1_F6	KEY_COMMA				// 246 ö     o DIAERESIS
#define ISO_8859_1_F7	0					// 247 ÷     DIVISION
#define ISO_8859_1_F8	0					// 248 ø     o STROKE
#define ISO_8859_1_F9	GRAVE_ACCENT_BITS + KEY_U		// 249 ù     u GRAVE
#define ISO_8859_1_FA	ACUTE_ACCENT_BITS + KEY_U		// 250 ú     u ACUTE
#define ISO_8859_1_FB	CIRCUMFLEX_BITS + KEY_U			// 251 û     u CIRCUMFLEX
#define ISO_8859_1_FC	KEY_RIGHT_BRACE				// 252 ü     u DIAERESIS
#define ISO_8859_1_FD	ACUTE_ACCENT_BITS + KEY_Y		// 253 ý     y ACUTE
#define ISO_8859_1_FE	0					// 254 þ     THORN
#define ISO_8859_1_FF	DIAERESIS_BITS + KEY_Y			// 255 ÿ     y DIAERESIS
#define UNICODE_20AC	KEY_E + ALTGR_MASK			//     €     Euro Sign

// not yet implemented
#define UNICODE_EXTRA00	0x20AC
#define KEYCODE_EXTRA00	KEY_E + ALTGR_MASK			// 20AC €    Euro Sign
#define UNICODE_EXTRA01	0x011E
#define KEYCODE_EXTRA01	KEY_LEFT_BRACE + SHIFT_MASK		// 011E Ğ    Latin capital letter G with breve
#define UNICODE_EXTRA02	0x011F
#define KEYCODE_EXTRA02 KEY_LEFT_BRACE				// 011F ğ    Latin small letter g with breve
#define UNICODE_EXTRA03	0x0130
#define KEYCODE_EXTRA03 KEY_QUOTE + SHIFT_MASK			// 0130 İ    Latin captial letter I with dot above
#define UNICODE_EXTRA04	0x0131
#define KEYCODE_EXTRA04 KEY_I					// 0131 ı    Latin small letter dotless i
#define UNICODE_EXTRA05	0x015E
#define KEYCODE_EXTRA05 KEY_SEMICOLON + SHIFT_MASK		// 015E Ş    Latin capital letter S with cedilla
#define UNICODE_EXTRA06	0x0151
#define KEYCODE_EXTRA06 KEY_SEMICOLON				// 0151 ş    Latin small letter s with cedilla

#endif // LAYOUT_TURKISH









extern const KEYCODE_TYPE keycodes_ascii[];
extern const KEYCODE_TYPE keycodes_iso_8859_1[];

#ifdef __cplusplus
} // extern "C"
#endif

#endif
