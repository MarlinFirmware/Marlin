#ifndef GUI_IMPL_H
#define GUI_IMPL_H

// Painting library
#include "Singleton.h"
#include "ultralcd_st7920_u8glib_rrd.h"
typedef common::Singleton<U8GLIB_ST7920_128X64_RRD> GuiPainter;
#define painter GuiPainter::getInstance(0)

// Specify the printer UI implementation
#include "GuiImpl_witbox_2.h"

#endif //GUI_IMPL_H
