/**
 * @file    lcdprint_hd44780.c
 * @brief   LCD print api for HD44780
 * @author  Yunhui Fu (yhfudev@gmail.com)
 * @version 1.0
 * @date    2016-08-19
 * @copyright GPL/BSD
 */

/**
 * Due to the limitation of the HD44780 hardware, the current available LCD modules can only support
 *   Western(English), Cyrillic(Russian), Kana(Japanese) charsets.
 */

#include <string.h>

#include "fontutils.h"
#include "lcdprint.h"

#if DISABLED(DOGLCD)

#if defined(ARDUINO)
  #include <LiquidCrystal.h>
  extern LiquidCrystal lcd;
  #define _lcd_write(a) lcd.write(a)
  #define _lcd_setcursor(col, row) lcd.setCursor((col), (row));
#else
  #define _lcd_write(a) TRACE("Write LCD: %c (%d)", (a), (int)(a));
  #define _lcd_setcursor(col, row) TRACE("Set cursor LCD: (%d,%d)", (col), (row));
#endif

int lcd_glyph_height(void) { return 1; }

////////////////////////////////////////////////////////////
typedef struct _hd44780_charmap_t {
  wchar_t uchar; // the unicode char
  uint8_t idx;   // the glyph of the char in the ROM
  uint8_t idx2;  // the char used to be combined with the idx to simulate a single char
} hd44780_charmap_t;

#if defined(__AVR__)
  #define IV(a) U##a
#else
  #define IV(a) L##a
#endif

static const hd44780_charmap_t g_hd44780_charmap[] PROGMEM = {
  // sorted by uchar:
  #if DISPLAY_CHARSET_HD44780 == JAPANESE

    #if 0
      // map CYRILLIC code to the glyph of HD44780 jp display module
      {IV('А'), 'A', 0},
      {IV('Б'), 'b', 0}, // error (0x01)
      {IV('В'), 'B', 0},
      {IV('Г'), 'T', 0}, // error (0x02)
      {IV('Д'), 'Q', 0}, // error (0x03)
      {IV('Е'), 'E', 0},
      {IV('Ё'), 'E', 0xDE}, // error 'E' + '``'(0xDE)
      {IV('Ж'), 'E', 0xC8}, // error 'E' + ''(0xC8)
      {IV('З'), '3', 0},
      {IV('И'), 'N', 0}, // error (0x05)
      {IV('Й'), 'N', 0x60}, // error (0x05 + '`'0x60)
      {IV('К'), 'K', 0},
      {IV('Л'), 'T', 0}, // error (0x06)
      {IV('М'), 'M', 0},
      {IV('Н'), 'H', 0},
      {IV('О'), 'O', 0},
      {IV('П'), 'n', 0}, // error (0x04)
      {IV('Р'), 'P', 0},
      {IV('С'), 'C', 0},
      {IV('Т'), 'T', 0},
      {IV('У'), 'Y', 0},
      {IV('Ф'), 'E', 0xEC}, // error ('E' + '¢'0xEC)
      {IV('Х'), 'X', 0},
      {IV('Ц'), 'U', 0}, // error (0x07)
      {IV('Ч'), 0xD1, 0}, // error ('ム'0xD1)
      {IV('Ш'), 'W', 0},
      {IV('Щ'), 0xD0, 0}, // error ('ミ'0xD0)
      {IV('Ъ'), 0xA2, 'b'}, // error ('「'0xA2 + 'b')
      {IV('Ы'), 'b', '|'}, // error ('b' + '|'},
      {IV('Ь'), 'b'},
      {IV('Э'), 0xD6}, // error ('ヨ'0xD6)
      {IV('Ю'), 0xC4, 'O'}, // error ('ト'0xC4 + 'O'}
      {IV('Я'), '9', '|'}, // error ('9' + '|'}

      {IV('а'), 'a', 0},
      {IV('б'), '6', 0},
      {IV('в'), 0xE2, 0}, // error ('β'0xE2)
      {IV('г'), 'r', 0},
      {IV('д'), 0xE5, 0}, // error (''0xE5)
      {IV('е'), 'e'},
      {IV('ё'), 'e', 0xDE}, // error ('e'+''0xDE)
      {IV('ж'), '*', 0},
      {IV('з'), 0xAE, 0}, // error (''0xAE)
      {IV('и'), 'u'},
      {IV('й'), 'u', 0x60}, // error ('u' + ''0x60)
      {IV('к'), 'k', 0},
      {IV('л'), 0xCA, 0}, // error ('ハ'0xCA)
      {IV('м'), 'm', 0},
      {IV('н'), 0xFC, 0}, // error ('円'0xFC)
      {IV('о'), 'o', 0},
      {IV('п'), 'n', 0},
      {IV('р'), 'p', 0},
      {IV('с'), 'c', 0},
      {IV('т'), 't', 0},
      {IV('у'), 'y', 0},
      {IV('ф'), 'q', 'p'},
      {IV('х'), 'x', 0},
      {IV('ц'), 'u', 0xA4}, // error ('u' + ''0xA4)
      {IV('ч'), 0xF9, 0}, // error (''0xF9)
      {IV('ш'), 'w', 0},
      {IV('щ'), 0xAF, 0}, // error ('ッ'0xAF)
      {IV('ъ'), 0xA2, 'b'}, // error ('「'0xA2+'b')
      {IV('ы'), 'b', '|'}, // error ('b'+'|')
      {IV('ь'), 'b', 0},
      {IV('э'), 0xA6, 0}, // error ('ヲ'0xA6)
      {IV('ю'), 0xAA, 'o'}, // error ('ェ'0xAA+'o')
      {IV('я'), 'g', 0},
    #endif // 0

    {IV('¢'), 0xEC, 0},
    {IV('°'), 0xDF, 0}, // Marlin special: '°'  LCD_STR_DEGREE (0x09)
    {IV('ä'), 0xE1, 0},
    {IV('ö'), 0xEF, 0},
    {IV('÷'), 0xFD, 0},
    {IV('ü'), 0xF5, 0},
    {IV('ˣ'), 0xEB, 0},
    {IV('Θ'), 0xF2, 0},
    {IV('Σ'), 0xF6, 0},
    {IV('Ω'), 0xF4, 0},
    {IV('α'), 0xE0, 0},
    {IV('β'), 0xE2, 0},
    {IV('ε'), 0xE3, 0}, // epsilon
    {IV('μ'), 0xE4, 0}, // mu
    {IV('π'), 0xF7, 0},
    {IV('ρ'), 0xE6, 0}, // rho
    {IV('σ'), 0xE5, 0}, // sigma
    {IV('←'), 0x7F, 0},
    {IV('→'), 0x7E, 0}, // Marlin special: '⮈⮉⮊⮋➤→' LCD_STR_ARROW_RIGHT (0x03)
    {IV('↰'), 0x03, 0}, // Marlin special: '⮥⮭⮉⇧↑↰'  LCD_STR_UPLEVEL (0x04)
    {IV('↻'), 0x04, 0}, // Marlin special: '↻↺⟳⟲'  LCD_STR_REFRESH (0x01)
    {IV('√'), 0xE8, 0},
    {IV('∞'), 0xF3, 0},
    {IV('⏱'), 0x07, 0}, // Marlin special: '🕐🕑🕒🕓🕔🕕🕖🕗🕘🕙🕚🕛🕜🕝🕞🕟🕠🕡🕢🕣🕤🕥🕦🕧 ⌚⌛⏰⏱⏳⧖⧗'  LCD_STR_CLOCK (0x05)
    {IV('█'), 0xFF, 0},


    //{IV(''), 0xA0, 0},
    {IV('。'), 0xA1, 0},
    {IV('「'), 0xA2, 0},
    {IV('」'), 0xA3, 0},
    {IV('゛'), 0xDE, 0}, // ‶
    {IV('゜'), 0xDF, 0}, // '〫' 
    {IV('゠'), '=', 0},
    {IV('ァ'), 0xA7, 0},
    {IV('ア'), 0xB1, 0},
    {IV('ィ'), 0xA8, 0},
    {IV('イ'), 0xB2, 0},
    {IV('ゥ'), 0xA9, 0},
    {IV('ウ'), 0xB3, 0},
    {IV('ェ'), 0xAA, 0},
    {IV('エ'), 0xB4, 0},
    {IV('ォ'), 0xAB, 0},

    {IV('オ'), 0xB5, 0},
    {IV('カ'), 0xB6, 0},
    {IV('ガ'), 0xB6, 0xDE},
    {IV('キ'), 0xB7, 0},
    {IV('ギ'), 0xB7, 0xDE}, //
    {IV('ク'), 0xB8, 0},
    {IV('グ'), 0xB8, 0xDE},
    {IV('ケ'), 0xB9, 0},
    {IV('ゲ'), 0xB9, 0xDE},
    {IV('コ'), 0xBA, 0},
    {IV('ゴ'), 0xBA, 0xDE},
    {IV('サ'), 0xBB, 0},
    {IV('ザ'), 0xBB, 0xDE},
    {IV('シ'), 0xBC, 0},
    {IV('ジ'), 0xBC, 0xDE},
    {IV('ス'), 0xBD, 0},
    {IV('ズ'), 0xBD, 0xDE},
    {IV('セ'), 0xBE, 0},
    {IV('ゼ'), 0xBE, 0xDE},
    {IV('ソ'), 0xBF, 0},
    {IV('ゾ'), 0xBF, 0xDE},

    {IV('タ'), 0xC0, 0},
    {IV('ダ'), 0xC0, 0xDE},
    {IV('チ'), 0xC1, 0},
    {IV('ヂ'), 0xC1, 0xDE},
    {IV('ッ'), 0xAF, 0},
    {IV('ツ'), 0xC2, 0},
    {IV('ヅ'), 0xC2, 0xDE},
    {IV('テ'), 0xC3, 0},
    {IV('デ'), 0xC3, 0xDE},
    {IV('ト'), 0xC4, 0},
    {IV('ド'), 0xC4, 0xDE},
    {IV('ナ'), 0xC5, 0},
    {IV('ニ'), 0xC6, 0},
    {IV('ヌ'), 0xC7, 0},
    {IV('ネ'), 0xC8, 0},
    {IV('ノ'), 0xC9, 0},
    {IV('ハ'), 0xCA, 0},
    {IV('バ'), 0xCA, 0xDE},
    {IV('パ'), 0xCA, 0xDF},
    {IV('ヒ'), 0xCB, 0},
    {IV('ビ'), 0xCB, 0xDE},
    {IV('ピ'), 0xCB, 0xDF},
    {IV('フ'), 0xCC, 0},
    {IV('ブ'), 0xCC, 0xDE},
    {IV('プ'), 0xCC, 0xDF},
    {IV('ヘ'), 0xCD, 0},
    {IV('ベ'), 0xCD, 0xDE},
    {IV('ペ'), 0xCD, 0xDF},
    {IV('ホ'), 0xCE, 0},
    {IV('ボ'), 0xCE, 0xDE},
    {IV('ポ'), 0xCE, 0xDF},
    {IV('マ'), 0xCF, 0},

    {IV('ミ'), 0xD0, 0},
    {IV('ム'), 0xD1, 0},
    {IV('メ'), 0xD2, 0},
    {IV('モ'), 0xD3, 0},
    {IV('ャ'), 0xAC, 0},
    {IV('ヤ'), 0xD4, 0},
    {IV('ュ'), 0xAD, 0},
    {IV('ユ'), 0xD5, 0},
    {IV('ョ'), 0xAE, 0},
    {IV('ヨ'), 0xD6, 0},
    {IV('ラ'), 0xD7, 0},
    {IV('リ'), 0xD8, 0},
    {IV('ル'), 0xD9, 0},
    {IV('レ'), 0xDA, 0},
    {IV('ロ'), 0xDB, 0},
    {IV('ワ'), 0xDC, 0},
    {IV('ヲ'), 0xA6, 0},
    {IV('ン'), 0xDD, 0},
    {IV('ヴ'), 0xB3, 0xDE},
    {IV('ヷ'), 0xDC, 0xDE},
    {IV('ヺ'), 0xA6, 0xDE},
    {IV('・'), 0xA5, 0},
    {IV('ー'), 0xB0, 0},
    {IV('ヽ'), 0xA4, 0},

    //{IV('g'), 0xE7, 0}, // error
    //{IV(''), 0xE9, 0},
    //{IV('j'), 0xEA, 0}, // error
    //{IV(''), 0xED, 0},
    //{IV(''), 0xEE, 0},

    //{IV('p'), 0xF0, 0}, // error
    //{IV('q'), 0xF1, 0}, // error
    //{IV(''), 0xF8, 0},
    //{IV('y'), 0xF9, 0}, // error
    {IV('万'), 0xFB, 0},
    {IV('円'), 0xFC, 0},
    {IV('千'), 0xFA, 0},
    //{IV(''), 0xFE, 0},

    //､･ｦｧｨｩｪｫｬｭｮｯｰ
    {IV('､'), 0xA4, 0}, //ヽ
    {IV('･'), 0xA5, 0}, //・
    {IV('ｦ'), 0xA6, 0}, //ヲ
    {IV('ｧ'), 0xA7, 0}, //ァ
    {IV('ｨ'), 0xA8, 0}, //ィ
    {IV('ｩ'), 0xA9, 0}, //ゥ
    {IV('ｪ'), 0xAA, 0}, //ェ
    {IV('ｫ'), 0xAB, 0}, //ォ
    {IV('ｬ'), 0xAC, 0}, //ャ
    {IV('ｭ'), 0xAD, 0}, //ュ
    {IV('ｮ'), 0xAE, 0}, //ョ
    {IV('ｯ'), 0xAF, 0}, //ッ
    {IV('ｰ'), 0xB0, 0}, //ー

    //ｱｲｳｴｵｶｷｸｹｺｻｼｽｾ
    {IV('ｱ'), 0xB1, 0}, //ア
    {IV('ｲ'), 0xB2, 0}, //イ
    {IV('ｳ'), 0xB3, 0}, //ウ
    {IV('ｴ'), 0xB4, 0}, //エ
    {IV('ｵ'), 0xB5, 0}, //オ
    {IV('ｶ'), 0xB6, 0}, //カ
    {IV('ｷ'), 0xB7, 0}, //キ
    {IV('ｸ'), 0xB8, 0}, //ク
    {IV('ｹ'), 0xB9, 0}, //ケ
    {IV('ｺ'), 0xBA, 0}, //コ
    {IV('ｻ'), 0xBB, 0}, //サ
    {IV('ｼ'), 0xBC, 0}, //シ
    {IV('ｽ'), 0xBD, 0}, //ス
    {IV('ｾ'), 0xBE, 0}, //セ

    //ｿﾀﾁﾂﾃﾄﾅﾆﾇﾈﾉﾊﾋﾌ
    {IV('ｿ'), 0xBF, 0}, //ソ
    {IV('ﾀ'), 0xC0, 0}, //タ
    {IV('ﾁ'), 0xC1, 0}, //チ
    {IV('ﾂ'), 0xC2, 0}, //ツ
    {IV('ﾃ'), 0xC3, 0}, //テ
    {IV('ﾄ'), 0xC4, 0}, //ト
    {IV('ﾅ'), 0xC5, 0}, //ナ
    {IV('ﾆ'), 0xC6, 0}, //ニ
    {IV('ﾇ'), 0xC7, 0}, //ヌ
    {IV('ﾈ'), 0xC8, 0}, //ネ
    {IV('ﾉ'), 0xC9, 0}, //ノ
    {IV('ﾊ'), 0xCA, 0}, //ハ
    {IV('ﾋ'), 0xCB, 0}, //ヒ
    {IV('ﾌ'), 0xCC, 0}, //フ

    //ﾍﾎﾏﾐﾑﾒﾓﾔﾕﾖﾗﾘﾙﾚﾛﾜﾝﾞﾟ
    {IV('ﾍ'), 0xCD, 0}, //ヘ
    {IV('ﾎ'), 0xCE, 0}, //ホ
    {IV('ﾏ'), 0xCF, 0}, //マ
    {IV('ﾐ'), 0xD0, 0}, //ミ
    {IV('ﾑ'), 0xD1, 0}, //ム
    {IV('ﾒ'), 0xD2, 0}, //メ
    {IV('ﾓ'), 0xD3, 0}, //モ
    {IV('ﾔ'), 0xD4, 0}, //ヤ
    {IV('ﾕ'), 0xD5, 0}, //ユ
    {IV('ﾖ'), 0xD6, 0}, //ヨ
    {IV('ﾗ'), 0xD7, 0}, //ラ
    {IV('ﾘ'), 0xD8, 0}, //リ
    {IV('ﾙ'), 0xD9, 0}, //ル
    {IV('ﾚ'), 0xDA, 0}, //レ
    {IV('ﾛ'), 0xDB, 0}, //ロ
    {IV('ﾜ'), 0xDC, 0}, //ワ
    {IV('ﾝ'), 0xDD, 0}, //ン
    {IV('ﾞ'), 0xDE, 0}, // ゛
    {IV('ﾟ'), 0xDF, 0}, // ゜

    {IV('￥'), 0x5C, 0},

  #elif DISPLAY_CHARSET_HD44780 == WESTERN
    // 0x10 -- 0x1F (except 0x1C)
    // 0x80 -- 0xFF (except 0xA7,0xB0,0xB1,0xB3,0xB4,0xBF,0xD1,0xF8,0xFA,0xFC-0xFF)

    {IV('¡'), 0xA9, 0},
    {IV('¢'), 0xA4, 0},
    {IV('£'), 0xA5, 0},
    {IV('¥'), 0xA6, 0},
    {IV('§'), 0xD2, 0}, // section sign
    {IV('©'), 0xCF, 0},

    {IV('ª'), 0x9D, 0},
    {IV('«'), 0xBB, 0},
    {IV('®'), 0xCE, 0},

    {IV('°'), 0xB2, 0}, // Marlin special: '°'  LCD_STR_DEGREE (0x09)
    //{IV(''), 0xD1, 0},
    {IV('±'), 0x10, 0}, //∓±
    //{'='), 0x1C, 0}, // error
    {IV('²'), 0x1E, 0},
    {IV('³'), 0x1F, 0},
    {IV('¶'), 0xD3, 0}, // pilcrow sign
    {IV('º'), 0x9E, 0},
    {IV('»'), 0xBC, 0},
    //{IV(''), 0xB3, 0}, // error
    //{IV(''), 0xB4, 0}, // error
    {IV('¼'), 0xB6, 0},
    {IV('½'), 0xB5, 0},
    {IV('¿'), 0x9F, 0},

    {IV('Â'), 0x8F, 0},
    {IV('Ã'), 0xAA, 0},
    {IV('Ä'), 0x8E, 0},
    {IV('Æ'), 0x92, 0},
    {IV('Ç'), 0x80, 0},
    {IV('É'), 0x90, 0},
    {IV('Ñ'), 0x9C, 0},
    {IV('Õ'), 0xAC, 0},
    {IV('Ö'), 0x99, 0},
    {IV('×'), 0xB7, 0},
    {IV('Ø'), 0xAE, 0},
    {IV('Ü'), 0x9A, 0},
    {IV('à'), 0x85, 0},
    {IV('á'), 0xA0, 0},
    {IV('â'), 0x83, 0},
    {IV('ã'), 0xAB, 0},
    {IV('ä'), 0x84, 0},
    {IV('å'), 0x86, 0},
    {IV('æ'), 0x91, 0},
    {IV('ç'), 0x87, 0},
    {IV('è'), 0x8A, 0},
    {IV('é'), 0x82, 0},
    {IV('ê'), 0x88, 0},
    {IV('ë'), 0x89, 0},
    {IV('ì'), 0x8D, 0},
    {IV('í'), 0xA1, 0},
    {IV('î'), 0x8C, 0},
    {IV('ï'), 0x8B, 0},

    {IV('ñ'), 0x9B, 0},
    {IV('ò'), 0x95, 0},
    {IV('ó'), 0xA2, 0},
    {IV('ô'), 0x93, 0},
    {IV('õ'), 0xAD, 0},
    {IV('ö'), 0x94, 0},
    {IV('÷'), 0xB8, 0},
    {IV('ø'), 0xAF, 0},
    {IV('ù'), 0x97, 0},
    {IV('ú'), 0xA3, 0},
    {IV('û'), 0x96, 0},
    {IV('ü'), 0x81, 0},
    {IV('ÿ'), 0x98, 0},


    //{IV(''), 0xB0, 0}, // error
    //{IV(''), 0xB1, 0}, // error
    {IV('ƒ'), 0xA8, 0},

    {IV('Γ'), 0xD4, 0}, // Gamma
    {IV('Δ'), 0xD5, 0}, // Delta, ◿
    {IV('Θ'), 0xD6, 0}, // Theta
    {IV('Λ'), 0xD7, 0}, // Lambda
    {IV('Ξ'), 0xD8, 0}, // Xi
    {IV('Π'), 0xD9, 0}, // Pi
    {IV('Σ'), 0xDA, 0}, // Sigma
    {IV('Υ'), 0xDB, 0}, // Upsilon
    {IV('Φ'), 0xDC, 0}, // Phi
    {IV('Ψ'), 0xDD, 0}, // Psi
    {IV('Ω'), 0xDE, 0}, // Omega
    {IV('α'), 0xDF, 0}, // alpha

    {IV('β'), 0xE0, 0}, // beta
    {IV('γ'), 0xE1, 0}, // gamma
    {IV('δ'), 0xE2, 0}, // delta
    {IV('ε'), 0xE3, 0}, // epsilon
    {IV('ζ'), 0xE4, 0}, // zeta
    {IV('η'), 0xE5, 0}, // eta
    {IV('θ'), 0xE6, 0}, // theta
    {IV('ι'), 0xE7, 0}, // lota
    {IV('κ'), 0xE8, 0}, // kappa
    {IV('λ'), 0xE9, 0}, // lambda
    {IV('μ'), 0xEA, 0}, // mu
    {IV('ν'), 0xEB, 0}, // nu
    {IV('ξ'), 0xEC, 0}, // xi
    {IV('π'), 0xED, 0}, // pi
    {IV('ρ'), 0xEE, 0}, // rho
    {IV('σ'), 0xEF, 0}, // sigma

    {IV('τ'), 0xF0, 0}, // tau
    {IV('υ'), 0xF1, 0}, // upsilon
    {IV('χ'), 0xF2, 0}, // chi
    {IV('ψ'), 0xF3, 0}, // psi
    {IV('ω'), 0xF4, 0}, // omega

    {IV('•'), 0xCD, 0}, // ·
    {IV('℞'), 0xA7, 0}, // ℞ Pt ASCII 158
    {IV('™'), 0xD0, 0},
    {IV('→'), '>', 0}, // Marlin special: '⮈⮉⮊⮋➤→' LCD_STR_ARROW_RIGHT (0x03)
    {IV('↤'), 0xF9, 0}, // ⟻
    {IV('↰'), 0x03, 0}, // Marlin special: '⮥⮭⮉⇧↑↰'  LCD_STR_UPLEVEL (0x04)
    {IV('↵'), 0xC4, 0},
    {IV('↻'), 0x04, 0}, // Marlin special: '↻↺⟳⟲'  LCD_STR_REFRESH (0x01)
    {IV('⇥'), 0xFB, 0},
    {IV('√'), 0xBE, 0}, // √
    {IV('∞'), 0xC2, 0}, // infinity
    {IV('∫'), 0x1B, 0},
    {IV('∼'), 0x1D, 0},
    {IV('≈'), 0x1A, 0},
    {IV('≠'), 0xBD, 0},
    {IV('≡'), 0x11, 0},
    {IV('≤'), 0xB9, 0},// ≤≥ ⩽⩾
    {IV('≥'), 0xBA, 0},
    //{IV(''), 0xBF, 0}, // error

    {IV('⌠'), 0xC0, 0},
    {IV('⌡'), 0xC1, 0},

    {IV('⎧'), 0x14, 0},
    {IV('⎩'), 0x15, 0},
    {IV('⎫'), 0x16, 0},
    {IV('⎭'), 0x17, 0},
    {IV('⎰'), 0x18, 0},
    {IV('⎱'), 0x19, 0},
    {IV('⎲'), 0x12, 0},
    {IV('⎳'), 0x13, 0},

    {IV('⏱'), 0x07, 0}, // Marlin special: '🕐🕑🕒🕓🕔🕕🕖🕗🕘🕙🕚🕛🕜🕝🕞🕟🕠🕡🕢🕣🕤🕥🕦🕧 ⌚⌛⏰⏱⏳⧖⧗'  LCD_STR_CLOCK (0x05)
    {IV('┌'), 0xC9, 0},
    {IV('┐'), 0xCA, 0},
    {IV('└'), 0xCB, 0},
    {IV('┘'), 0xCC, 0},
    {IV('◸'), 0xC3, 0}, // ◿
    {IV('⭠'), 0xC8, 0},
    {IV('⭡'), 0xC5, 0},
    {IV('⭢'), 0xC7, 0},
    {IV('⭣'), 0xC6, 0},


    {IV('⯆'), 0xF5, 0},
    {IV('⯇'), 0xF7, 0}, // ⯅
    {IV('⯈'), 0xF6, 0},
    //{IV(''), 0xF8, 0}, // error
    //{IV(''), 0xFA, 0}, // error
    //{IV(''), 0xFC, 0}, // error
    //{IV(''), 0xFD, 0}, // error
    //{IV(''), 0xFE, 0}, // error
    //{IV(''), 0xFF, 0}, // error

  #elif DISPLAY_CHARSET_HD44780 == CYRILLIC

    {IV('¢'), 0x5C, 0},
    {IV('°'), 0x01, 0}, // Marlin special: '°'  LCD_STR_DEGREE (0x09)

    //{IV(''), 0x80, 0},
    //{IV(''), 0x81, 0},
    //{IV(''), 0x82, 0},
    //{IV(''), 0x83, 0},
    //{IV(''), 0x84, 0},
    //{IV(''), 0x85, 0},
    //{IV(''), 0x86, 0},
    //{IV(''), 0x87, 0},
    //{IV(''), 0x88, 0},
    //{IV(''), 0x89, 0},
    //{IV(''), 0x8A, 0},
    //{IV(''), 0x8B, 0},
    //{IV(''), 0x8C, 0},
    //{IV(''), 0x8D, 0},
    //{IV(''), 0x8E, 0},
    //{IV(''), 0x8F, 0},

    //{IV(''), 0x90, 0},
    //{IV(''), 0x91, 0},
    //{IV(''), 0x92, 0},
    //{IV(''), 0x93, 0},
    //{IV(''), 0x94, 0},
    //{IV(''), 0x95, 0},
    //{IV(''), 0x96, 0},
    //{IV(''), 0x97, 0},
    //{IV(''), 0x98, 0},
    //{IV(''), 0x99, 0},
    //{IV(''), 0x9A, 0},
    //{IV(''), 0x9B, 0},
    //{IV(''), 0x9C, 0},
    //{IV(''), 0x9D, 0},
    //{IV(''), 0x9E, 0},
    //{IV(''), 0x9F, 0},

    {IV('Â'), 'A', 0},
    {IV('Ã'), 'A', 0},
    {IV('Ä'), 'A', 0},
    {IV('Æ'), 'A', 'E'},
    {IV('Ç'), 'C', 0},
    {IV('É'), 'E', 0},
    {IV('Ñ'), 'N', 0},
    {IV('Õ'), 'O', 0},
    {IV('Ö'), 'O', 0},
    {IV('×'), 'x', 0},
    {IV('Ü'), 'U', 0},
    {IV('à'), 'a', 0},
    {IV('á'), 'a', 0},
    {IV('â'), 'a', 0},
    {IV('ã'), 'a', 0},
    {IV('ä'), 'a', 0},
    {IV('å'), 'a', 0},
    {IV('æ'), 'a', 'e'},
    {IV('ç'), 'c', 0},
    {IV('è'), 'e', 0},
    {IV('é'), 'e', 0},
    {IV('ê'), 'e', 0},
    {IV('ë'), 'e', 0},
    {IV('ì'), 'i', 0},
    {IV('í'), 'i', 0},
    {IV('î'), 'i', 0},
    {IV('ï'), 'i', 0},

    {IV('ñ'), 'n', 0},
    {IV('ò'), 'o', 0},
    {IV('ó'), 'o', 0},
    {IV('ô'), 'o', 0},
    {IV('õ'), 'o', 0},
    {IV('ö'), 'o', 0},
    {IV('ø'), 'o', 0},
    {IV('ù'), 'u', 0},
    {IV('ú'), 'u', 0},
    {IV('û'), 'u', 0},
    {IV('ü'), 'u', 0},
    {IV('ÿ'), 'y', 0},

    {IV('Ё'), 0xA2, 0},
    {IV('А'), 'A', 0},
    {IV('Б'), 0xA0, 0},
    {IV('В'), 'B', 0},
    {IV('Г'), 0xA1, 0},
    {IV('Д'), 0xE0, 0},
    {IV('Е'), 'E', 0},
    {IV('Ж'), 0xA3, 0},
    {IV('З'), 0xA4, 0},
    {IV('И'), 0xA5, 0},
    {IV('Й'), 0xA6, 0},
    {IV('К'), 'K', 0},
    {IV('Л'), 0xA7, 0},
    {IV('М'), 'M', 0},
    {IV('Н'), 'H', 0},
    {IV('О'), 'O', 0},
    {IV('П'), 0xA8, 0},
    {IV('Р'), 'P', 0},
    {IV('С'), 'C', 0},
    {IV('Т'), 'T', 0},
    {IV('У'), 0xA9, 0},
    {IV('Ф'), 0xAA, 0},
    {IV('Х'), 'X', 0},
    {IV('Ц'), 0xE1, 0},
    {IV('Ч'), 0xAB, 0},
    {IV('Ш'), 0xAC, 0},
    {IV('Щ'), 0xE2, 0},
    {IV('Ъ'), 0xAD, 0},
    {IV('Ы'), 0xAE, 0},
    {IV('Ь'), 'b', 0},
    {IV('Э'), 0xAF, 0},
    {IV('Ю'), 0xB0, 0},
    {IV('Я'), 0xB1, 0},
    {IV('а'), 'a', 0},

    {IV('б'), 0xB2, 0},
    {IV('в'), 0xB3, 0},
    {IV('г'), 0xB4, 0},
    {IV('д'), 0xE3, 0},
    {IV('е'), 'e', 0},
    {IV('ж'), 0xB6, 0},
    {IV('з'), 0xB7, 0},
    {IV('и'), 0xB8, 0},
    {IV('й'), 0xB9, 0},
    {IV('к'), 0xBA, 0}, //клмноп
    {IV('л'), 0xBB, 0},
    {IV('м'), 0xBC, 0},
    {IV('н'), 0xBD, 0},
    {IV('о'), 'o', 0},
    {IV('п'), 0xBE, 0},
    {IV('р'), 'p', 0},
    {IV('с'), 'c', 0},
    {IV('т'), 0xBF, 0},

    {IV('у'), 'y', 0},
    {IV('ф'), 0xE4, 0},
    {IV('х'), 'x', 0},
    {IV('ц'), 0xE5, 0},
    {IV('ч'), 0xC0, 0},
    {IV('ш'), 0xC1, 0},
    {IV('щ'), 0xE6, 0},
    {IV('ъ'), 0xC2, 0},
    {IV('ы'), 0xC3, 0},
    {IV('ь'), 0xC4, 0},
    {IV('э'), 0xC5, 0},
    {IV('ю'), 0xC6, 0},
    {IV('я'), 0xC7, 0},
    {IV('ё'), 0xB5, 0},
    //{IV(''), 0xC8, 0},
    //{IV(''), 0xC9, 0},
    //{IV(''), 0xCA, 0},
    //{IV(''), 0xCB, 0},
    //{IV(''), 0xCC, 0},
    //{IV(''), 0xCD, 0},
    //{IV(''), 0xCE, 0},
    //{IV(''), 0xCF, 0},

    //{IV(''), 0xD0, 0},
    //{IV(''), 0xD1, 0},
    //{IV(''), 0xD2, 0},
    //{IV(''), 0xD3, 0},
    //{IV(''), 0xD4, 0},
    //{IV(''), 0xD5, 0},
    //{IV(''), 0xD6, 0},
    //{IV(''), 0xD7, 0},
    //{IV(''), 0xD8, 0},
    //{IV(''), 0xDB, 0},
    //{IV(''), 0xDC, 0},
    //{IV(''), 0xDD, 0},
    //{IV(''), 0xDE, 0},
    //{IV(''), 0xDF, 0},

    //{IV(''), 0xE7, 0},
    //{IV(''), 0xE8, 0},
    //{IV(''), 0xE9, 0},
    //{IV(''), 0xEA, 0},
    //{IV(''), 0xEB, 0},
    //{IV(''), 0xEC, 0},
    //{IV(''), 0xED, 0},
    //{IV(''), 0xEE, 0},
    //{IV(''), 0xEF, 0},

    //{IV(''), 0xF0, 0},
    //{IV(''), 0xF1, 0},
    //{IV(''), 0xF2, 0},
    //{IV(''), 0xF3, 0},
    //{IV(''), 0xF4, 0},
    //{IV(''), 0xF5, 0},
    //{IV(''), 0xF6, 0},
    //{IV(''), 0xF7, 0},
    //{IV(''), 0xF8, 0},
    //{IV(''), 0xF9, 0},
    //{IV(''), 0xFA, 0},
    //{IV(''), 0xFB, 0},
    //{IV(''), 0xFC, 0},
    //{IV(''), 0xFD, 0},
    //{IV(''), 0xFE, 0},
    //{IV(''), 0xFF, 0},

    {IV('↑'), 0xD9, 0}, //←↑→↓
    {IV('↓'), 0xDA, 0},

    {IV('→'), '>', 0}, // Marlin special: '⮈⮉⮊⮋➤→' LCD_STR_ARROW_RIGHT (0x03)
    {IV('↰'), 0x03, 0}, // Marlin special: '⮥⮭⮉⇧↑↰'  LCD_STR_UPLEVEL (0x04)
    {IV('↻'), 0x04, 0}, // Marlin special: '↻↺⟳⟲'  LCD_STR_REFRESH (0x01)
    {IV('⏱'), 0x07, 0}, // Marlin special: '🕐🕑🕒🕓🕔🕕🕖🕗🕘🕙🕚🕛🕜🕝🕞🕟🕠🕡🕢🕣🕤🕥🕦🕧 ⌚⌛⏰⏱⏳⧖⧗'  LCD_STR_CLOCK (0x05)
  #endif

  // ◴◵◶◷
  //{IV(''), 0x00, 0}, // Marlin special: ''  LCD_STR_BEDTEMP (0x07)
  {IV('🌡'), 0x02, 0}, // Marlin special: '🌡'  LCD_STR_THERMOMETER (0x08)
  {IV('📂'), 0x05, 0}, // Marlin special: '📁📂'  LCD_STR_FOLDER (0x02)
  //{IV(''), 0x06, 0}, // Marlin special: ''  LCD_STR_FEEDRATE (0x06)
};

#if DEBUG

  void test_show_uchar() {
    wchar_t pre = 0;
    hd44780_charmap_t cur;
    for (int i = 0; i < NUM_ARRAY(g_hd44780_charmap); i ++) {
      memcpy_P(&cur, g_hd44780_charmap + i, sizeof(cur));
      //fprintf(stdout, "[% 2d] %" PRIu32 "(0x06%" PRIX32 ") --> 0x%02X,0x%02X%s\n", i, cur.uchar, cur.uchar, (int)(cur.idx), (int)(cur.idx2), (pre < cur.uchar?"":" <--- ERROR"));
      #if 1
        TRACE("[% 2d] %" PRIu32 "(0x%" PRIX32 ") --> 0x%02X,0x%02X%s", i, cur.uchar, cur.uchar, (unsigned int)(cur.idx), (unsigned int)(cur.idx2), (pre < cur.uchar?"":" <--- ERROR"));
      #else
        TRACE("[% 2d]", i);
        TRACE("%" PRIu32 "(0x%" PRIX32 "),", cur.uchar, cur.uchar);
        TRACE("0x%02X,", (unsigned int)(cur.idx));
        TRACE("0x%02X,", (unsigned int)(cur.idx2));
        TRACE("%s", (pre < cur.uchar?"":" <--- ERROR"));
      #endif
      pre = cur.uchar;
    }
  }

#endif // DEBUG

void lcd_moveto(int col, int row) {
  TRACE("Move to: (%d,%d)", col, row);
  _lcd_setcursor(col, row);
}

/* return v1 - v2 */
static int hd44780_charmap_compare(hd44780_charmap_t * v1, hd44780_charmap_t * v2) {
  assert(NULL != v1);
  assert(NULL != v2);
  TRACE("compare char1 %" PRIu32 "(0x%" PRIX32 ")", v1->uchar, v1->uchar);
  TRACE("compare char2 %" PRIu32 "(0x%" PRIX32 ")", v2->uchar, v2->uchar);
  if (v1->uchar < v2->uchar) {
    TRACE("compare return -1");
    return -1;
  } else if (v1->uchar > v2->uchar) {
    TRACE("compare return 1");
    return 1;
  }
  #if 0
    if (v1->idx < v2->idx) {
      return -1;
    } else if (v1->idx > v2->idx) {
      return 1;
    }
  #endif
  TRACE("compare return 0");
  return 0;
}

static int pf_bsearch_cb_comp_hd4map_pgm(void *userdata, size_t idx, void * data_pin) {
  hd44780_charmap_t localval;
  hd44780_charmap_t *p_hd44780_charmap = (hd44780_charmap_t *)userdata;
  memcpy_P(&localval, p_hd44780_charmap + idx, sizeof(localval));
  return hd44780_charmap_compare(&localval, (hd44780_charmap_t *)data_pin);
}

// return < 0 on error
// return the advanced cols
int lcd_put_wchar_max(wchar_t c, pixel_len_t max_length) {
  // find the HD44780 internal ROM first
  size_t idx = 0;
  hd44780_charmap_t pinval;
  hd44780_charmap_t localval;
  pinval.uchar = c;
  pinval.idx = -1;

  if (max_length < 1) return 0;

  // TODO: fix the '\\' that dont exist in the HD44870
  if (c < 128) {
    //TRACE("draw char: regular %d", (int)c);
    _lcd_write((uint8_t)c);
    return 1;
  }
  if (pf_bsearch_r((void *)g_hd44780_charmap, NUM_ARRAY(g_hd44780_charmap), pf_bsearch_cb_comp_hd4map_pgm, (void *)&pinval, &idx) >= 0) {
    // found
    memcpy_P(&localval, g_hd44780_charmap + idx, sizeof(localval));
    assert((localval.uchar == c) && (localval.uchar == pinval.uchar));
    TRACE("draw char: %" PRIu32 "(0x%" PRIX32 ") at ROM %d(+%d)", c, c, (int)localval.idx, (int)localval.idx2);
    _lcd_write(localval.idx);
    if (max_length >= 2 && localval.idx2 > 0) {
      _lcd_write(localval.idx2);
      return 2;
    }
    return 1;
  }
  // print '?' instead
  TRACE("draw char: Not found " PRIu32 "(0x%" PRIX32 ")", c, c);
  _lcd_write((uint8_t)'?');
  return 1;
}

/**
* @brief Draw a UTF-8 string
*
* @param utf8_str : the UTF-8 string
* @param len : the byte length of the string (returned by strlen(utf8_str) or strlen_P(utf8_str) )
* @param cb_read_byte : the callback function to read one byte from the utf8_str (from RAM or ROM)
* @param max_length : the pixel length of the string allowed (or number of slots in HD44780)
*
* @return the number of pixels advanced
*
* Draw a UTF-8 string
*/
static int lcd_put_u8str_max_cb(const char * utf8_str, uint16_t len, uint8_t (*cb_read_byte)(uint8_t * str), pixel_len_t max_length) {
  wchar_t ch;
  uint8_t *p, *pend;
  int ret = 0;

  TRACE("BEGIN lcd_put_u8str_max_cb(len=%d, maxlen=%d)", len, max_length);
  pend = (uint8_t *)utf8_str + len;
  for (p = (uint8_t *)utf8_str; (p < pend) && (ret < max_length); ) {
    ch = 0;
    p = get_utf8_value_cb(p, cb_read_byte, &ch);
    if (NULL == p) {
      TRACE("No more char, break ...");
      break;
    }
    ret += lcd_put_wchar_max(ch, PIXEL_LEN_NOLIMIT);
  }
  return ret;
}

int lcd_put_u8str_max(const char * utf8_str, pixel_len_t max_length) {
  //TRACE("BEGIN lcd_put_u8str_max(str='%s', len=%d, maxlen=%d)", utf8_str, strlen(utf8_str), max_length);
  TRACE("BEGIN lcd_put_u8str_max(str='%s')", utf8_str);
  TRACE("BEGIN lcd_put_u8str_max('len=%d)", strlen(utf8_str));
  TRACE("BEGIN lcd_put_u8str_max(maxlen=%d)", max_length);
  return lcd_put_u8str_max_cb(utf8_str, strlen(utf8_str), read_byte_ram, max_length);
}

int lcd_put_u8str_max_rom(const char * utf8_str_P, pixel_len_t max_length) {
  //TRACE("BEGIN lcd_put_u8str_max_rom('%s', len=%d, maxlen=%d)", utf8_str_P, strlen_P(utf8_str_P), max_length);
  TRACE("BEGIN lcd_put_u8str_max_rom(len=%d)", strlen_P(utf8_str_P));
  TRACE("BEGIN lcd_put_u8str_max_rom(maxlen=%d)", max_length);
  return lcd_put_u8str_max_cb(utf8_str_P, strlen_P(utf8_str_P), read_byte_rom, max_length);
}

#endif // USE_HD44780
