#ifndef GUI_PAINTER_H
#define GUI_PAINTER_H

#include "ultralcd_st7920_u8glib_rrd.h"
#include "Singleton.h"

typedef common::Singleton<U8GLIB_ST7920_128X64_RRD> GuiPainter;
#define painter GuiPainter::getInstance(0)

#endif //GUI_PAINTER_H
