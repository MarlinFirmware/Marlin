/**
 * @file    fontutf8.c
 * @brief   font api for u8g lib
 * @author  Yunhui Fu (yhfudev@gmail.com)
 * @version 1.0
 * @date    2015-02-19
 * @copyright GPL/BSD
 */

#include <string.h>

#include "fontutils.h"
#include "u8g.h"

#include "u8g_fontutf8.h"

#define FALSE 0
#define TRUE  1

//#if DEBUG
//#include <stdio.h>
//#include <stdlib.h>
//#define assert(a) if (!(a)) {printf("Assert: " # a); exit(1);}
//#define TRACE(fmt, ...) fprintf (stdout, "[%s()] " fmt " {ln:%d, fn:" __FILE__ "}\n", __func__, ##__VA_ARGS__, __LINE__)
//#else
//#define assert(a)
//#define TRACE(...)
//#endif

////////////////////////////////////////////////////////////
#define font_t void

/**
 * @brief the callback function to draw somthing
 *
 * @param userdata : User's data
 * @param msg : the u8g's string
 * @param fnt_current : the font
 *
 * @return 0 on success, 1 to force quit, <0 on error
 *
 * Get the screen pixel length of a ROM UTF-8 string
 */
typedef int (* fontgroup_cb_draw_t)(void *userdata, font_t *fnt_current, const char *msg);

//extern int fontgroup_init (font_group_t * root, uxg_fontinfo_t * fntinfo, int number);
//extern int fontgroup_drawstring (font_group_t * group, font_t *fnt_default, const char *utf8_msg, void * userdata, fontgroup_cb_draw_t cb_draw);
//extern uxg_fontinfo_t * fontgroup_first (font_group_t * root);


////////////////////////////////////////////////////////////
/* return v1 - v2 */
static int
fontinfo_compare (uxg_fontinfo_t * v1, uxg_fontinfo_t * v2)
{
    assert (NULL != v1);
    assert (NULL != v2);
    if (v1->page < v2->page) {
        return -1;
    } else if (v1->page > v2->page) {
        return 1;
    }
    if (v1->end < v2->begin) {
        return -1;
    } else if (v1->begin > v2->end) {
        return 1;
    }
    return 0;
}

/*"data_list[idx] - *data_pin"*/
static int
pf_bsearch_cb_comp_fntifo_pgm (void *userdata, size_t idx, void * data_pin)
{
    uxg_fontinfo_t * fntinfo = (uxg_fontinfo_t *) userdata;
    uxg_fontinfo_t localval;
    memcpy_P (&localval, fntinfo + idx, sizeof (localval));
    return fontinfo_compare (&localval, data_pin);
}

typedef struct _font_group_t {
    uxg_fontinfo_t * m_fntifo;
    int m_fntinfo_num;
} font_group_t;

static int
fontgroup_init (font_group_t * root, uxg_fontinfo_t * fntinfo, int number)
{
    root->m_fntifo = fntinfo;
    root->m_fntinfo_num = number;

    return 0;
}

static const font_t *
fontgroup_find (font_group_t * root, wchar_t val)
{
    uxg_fontinfo_t vcmp = {val / 128, val % 128 + 128, val % 128 + 128, 0, 0};
    size_t idx = 0;

    if (val < 128) {
        return NULL;
    }

    if (pf_bsearch_r ((void *)root->m_fntifo, root->m_fntinfo_num, pf_bsearch_cb_comp_fntifo_pgm, (void *)&vcmp, &idx) < 0) {
        return NULL;
    }
    memcpy_P (&vcmp, root->m_fntifo + idx, sizeof (vcmp));
    return vcmp.fntdata;
}

static void
fontgroup_drawwchar (font_group_t * group, font_t *fnt_default, wchar_t val, void * userdata, fontgroup_cb_draw_t cb_draw_ram)
{
    uint8_t buf[2] = {0, 0};
    font_t * fntpqm = NULL;

    //TRACE("got char=%d", (int)val);
    buf[0] = (uint8_t)(val & 0x7F);
    fntpqm = (font_t *)fontgroup_find (group, val);
    if (NULL == fntpqm) {
        //continue;
        //buf[0] = '?';
        fntpqm = fnt_default;
        TRACE("Unknown char, use default font");
    }
    if (fnt_default != fntpqm) {
        buf[0] |= 0x80; // use upper page to avoid 0x00 error in C. you may want to generate the font data
    }
    //TRACE("set font: %p; (default=%p)", fntpqm, UXG_DEFAULT_FONT);
    cb_draw_ram (userdata, fntpqm, (char *) buf);
}

/**
 * @brief try to process a utf8 string
 *
 * @param pu8g : U8G pointer
 * @param fnt_default : the default font
 * @param utf8_msg : the UTF-8 string
 * @param cb_read_byte : how to read the utf8_msg, from RAM or ROM (call read_byte_ram or pgm_read_byte)
 * @param userdata : User's data
 * @param cb_draw_ram : the callback function of userdata to draw a !RAM! string (actural it is to draw a one byte string in RAM)
 *
 * @return N/A
 *
 * Get the screen pixel length of a ROM UTF-8 string
 */
static void
fontgroup_drawstring (font_group_t * group, font_t *fnt_default, const char *utf8_msg, int len_msg, read_byte_cb_t cb_read_byte, void * userdata, fontgroup_cb_draw_t cb_draw_ram)
{
    int ret;
    uint8_t *pend = NULL;
    uint8_t *p;
    wchar_t val;

    pend = (uint8_t *)utf8_msg + len_msg;
    for (p = (uint8_t *)utf8_msg; p < pend; ) {
        val = 0;
        p = get_utf8_value_cb (p, cb_read_byte, &val);
        if (NULL == p) {
            TRACE("No more char, break ...");
            break;
        }
        fontgroup_drawwchar (group, fnt_default, val, userdata, cb_draw_ram);
    }
}

////////////////////////////////////////////////////////////
static char flag_fontgroup_inited1 = 0;
#define flag_fontgroup_inited flag_fontgroup_inited1
static font_group_t g_fontgroup_root = {NULL, 0};

/**
 * @brief check if font is loaded
 */
char
uxg_Utf8FontIsInited(void)
{
    return flag_fontgroup_inited;
}

int
uxg_SetUtf8Fonts (const uxg_fontinfo_t * fntinfo, int number)
{
    flag_fontgroup_inited = 1;
    return fontgroup_init (&g_fontgroup_root, fntinfo, number);
}

struct _uxg_drawu8_data_t {
    u8g_t *pu8g;
    unsigned int x;
    unsigned int y;
    unsigned int adv;
    unsigned int max_length; // the max pixel length of the string allowed
    void * fnt_prev;
};

static void
fontgroup_cb_draw_u8g (void *userdata, font_t *fnt_current, const char *msg)
{
    struct _uxg_drawu8_data_t * pdata = userdata;

    assert (NULL != userdata);
    if (pdata->fnt_prev != fnt_current) {
        u8g_SetFont (pdata->pu8g, fnt_current);
        u8g_SetFontPosBottom (pdata->pu8g);
        pdata->fnt_prev = fnt_current;
    }
    if ((pdata->max_length != PIXEL_LEN_NOLIMIT) && (pdata->adv + u8g_GetStrPixelWidth(pdata->pu8g, (char *)msg) > pdata->max_length)) {
        TRACE ("return end, adv=%d, width=%d, maxlen=%d", pdata->adv, u8g_GetStrPixelWidth(pdata->pu8g, (char *)msg), pdata->max_length);
        return 1;
    }
    pdata->adv += u8g_DrawStr(pdata->pu8g, pdata->x + pdata->adv, pdata->y, (char *) msg);
    TRACE("adv pos= %d", pdata->adv);
    return 0;
}

/**
 * @brief Draw a wchar_t at the specified position
 *
 * @param pu8g : U8G pointer
 * @param x : position x axis
 * @param y : position y axis
 * @param ch : the wchar_t
 * @param max_length : the pixel length of the string allowed
 *
 * @return the avanced pixel
 *
 * Draw a UTF-8 string at the specified position
 */
unsigned int
uxg_DrawWchar (u8g_t *pu8g, unsigned int x, unsigned int y, wchar_t ch, pixel_len_t max_length)
{
    struct _uxg_drawu8_data_t data;
    font_group_t * group = &g_fontgroup_root;
    font_t *fnt_default = uxg_GetFont(pu8g);

    if (! uxg_Utf8FontIsInited()) {
        u8g_DrawStrP (pu8g, x, y, PSTR("Err: utf8 font not initialized."));
        return 0;
    }
    data.pu8g = pu8g;
    data.x = x;
    data.y = y;
    data.adv = 0;
    data.max_length = max_length;
    data.fnt_prev = NULL;
    fontgroup_drawwchar (group, fnt_default, ch, (void *)&data, fontgroup_cb_draw_u8g);
    u8g_SetFont (pu8g, fnt_default);

    return data.adv;
}

/**
 * @brief Draw a UTF-8 string at the specified position
 *
 * @param pu8g : U8G pointer
 * @param x : position x axis
 * @param y : position y axis
 * @param utf8_msg : the UTF-8 string
 * @param max_length : the pixel length of the string allowed
 *
 * @return the avanced pixel
 *
 * Draw a UTF-8 string at the specified position
 */
unsigned int
uxg_DrawUtf8Str (u8g_t *pu8g, unsigned int x, unsigned int y, const char *utf8_msg, pixel_len_t max_length)
{
    struct _uxg_drawu8_data_t data;
    font_group_t * group = &g_fontgroup_root;
    font_t *fnt_default = uxg_GetFont(pu8g);

    if (! uxg_Utf8FontIsInited()) {
        u8g_DrawStrP (pu8g, x, y, PSTR("Err: utf8 font not initialized."));
        return 0;
    }
    data.pu8g = pu8g;
    data.x = x;
    data.y = y;
    data.adv = 0;
    data.max_length = max_length;
    data.fnt_prev = NULL;
    fontgroup_drawstring (group, fnt_default, utf8_msg, strlen(utf8_msg), read_byte_ram, (void *)&data, fontgroup_cb_draw_u8g);
    u8g_SetFont (pu8g, fnt_default);

    return data.adv;
}

/**
 * @brief Draw a ROM UTF-8 string at the specified position
 *
 * @param pu8g : U8G pointer
 * @param x : position x axis
 * @param y : position y axis
 * @param utf8_msg : the UTF-8 string
 * @param max_length : the pixel length of the string allowed
 *
 * @return the avanced pixel
 *
 * Draw a ROM UTF-8 string at the specified position
 */
unsigned int
uxg_DrawUtf8StrP (u8g_t *pu8g, unsigned int x, unsigned int y, const char *utf8_msg, pixel_len_t max_length)
{
    struct _uxg_drawu8_data_t data;
    font_group_t * group = &g_fontgroup_root;
    font_t *fnt_default = uxg_GetFont(pu8g);

    if (! uxg_Utf8FontIsInited()) {
        TRACE ("Error, utf8string not inited!");
        u8g_DrawStrP (pu8g, x, y, PSTR("Err: utf8 font not initialized."));
        return 0;
    }
    data.pu8g = pu8g;
    data.x = x;
    data.y = y;
    data.adv = 0;
    data.max_length = max_length;
    data.fnt_prev = NULL;
    TRACE ("call fontgroup_drawstring");
    fontgroup_drawstring (group, fnt_default, utf8_msg, strlen_P(utf8_msg), read_byte_rom, (void *)&data, fontgroup_cb_draw_u8g);
    TRACE ("restore font");
    u8g_SetFont (pu8g, fnt_default);

    TRACE ("return %d", data.adv);
    return data.adv;
}

static int
fontgroup_cb_draw_u8gstrlen (void *userdata, font_t *fnt_current, const char *msg)
{
    struct _uxg_drawu8_data_t * pdata = userdata;

    assert (NULL != userdata);
    if (pdata->fnt_prev != fnt_current) {
        u8g_SetFont (pdata->pu8g, fnt_current);
        u8g_SetFontPosBottom (pdata->pu8g);
        pdata->fnt_prev = fnt_current;
    }
    pdata->adv += u8g_GetStrPixelWidth(pdata->pu8g, (char *)msg);
    return 0;
}

/**
 * @brief Get the screen pixel length of a UTF-8 string
 *
 * @param pu8g : U8G pointer
 * @param utf8_msg : the UTF-8 string
 *
 * @return the pixel length
 *
 * Get the screen pixel length of a UTF-8 string
 */
int
uxg_GetUtf8StrPixelWidth(u8g_t *pu8g, char *utf8_msg)
{
    struct _uxg_drawu8_data_t data;
    font_group_t * group = &g_fontgroup_root;
    font_t *fnt_default = uxg_GetFont(pu8g);

    if (! uxg_Utf8FontIsInited()) {
        TRACE("Err: utf8 font not initialized.");
        return -1;
    }

    memset (&data, 0, sizeof (data));
    data.pu8g = pu8g;
    data.adv = 0;
    fontgroup_drawstring (group, fnt_default, utf8_msg, strlen(utf8_msg), read_byte_ram, (void *)&data, fontgroup_cb_draw_u8gstrlen);
    u8g_SetFont (pu8g, fnt_default);

    return data.adv;
}

/**
 * @brief Get the screen pixel length of a ROM UTF-8 string
 *
 * @param pu8g : U8G pointer
 * @param utf8_msg : the UTF-8 string
 *
 * @return the pixel length
 *
 * Get the screen pixel length of a ROM UTF-8 string
 */
int
uxg_GetUtf8StrPixelWidthP(u8g_t *pu8g, const char *utf8_msg)
{
    struct _uxg_drawu8_data_t data;
    font_group_t * group = &g_fontgroup_root;
    font_t *fnt_default = uxg_GetFont(pu8g);

    if (! uxg_Utf8FontIsInited()) {
        TRACE("Err: utf8 font not initialized.");
        return -1;
    }
    memset (&data, 0, sizeof (data));
    data.pu8g = pu8g;
    data.adv = 0;
    fontgroup_drawstring (group, fnt_default, utf8_msg, strlen_P(utf8_msg), read_byte_rom, (void *)&data, fontgroup_cb_draw_u8gstrlen);
    u8g_SetFont (pu8g, fnt_default);
    return data.adv;
}
