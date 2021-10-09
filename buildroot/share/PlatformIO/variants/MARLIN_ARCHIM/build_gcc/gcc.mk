#
#  Copyright (c) 2011 Arduino.  All right reserved.
#
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License, or (at your option) any later version.
#
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#  See the GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#

# Tool suffix when cross-compiling
CROSS_COMPILE = $(ARM_GCC_TOOLCHAIN)/arm-none-eabi-

# Compilation tools
AR = $(CROSS_COMPILE)ar
CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
AS = $(CROSS_COMPILE)as
NM = $(CROSS_COMPILE)nm
ifeq ($(OS),Windows_NT)
RM=cs-rm -Rf
else
RM=rm -Rf
endif

SEP=\\

# ---------------------------------------------------------------------------------------
# C Flags

CFLAGS += -Wall -Wchar-subscripts -Wcomment -Wformat=2 -Wimplicit-int
CFLAGS += -Werror-implicit-function-declaration -Wmain -Wparentheses
CFLAGS += -Wsequence-point -Wreturn-type -Wswitch -Wtrigraphs -Wunused
CFLAGS += -Wuninitialized -Wunknown-pragmas -Wfloat-equal -Wundef
CFLAGS += -Wshadow -Wpointer-arith -Wbad-function-cast -Wwrite-strings
CFLAGS += -Wsign-compare -Waggregate-return -Wstrict-prototypes
CFLAGS += -Wmissing-prototypes -Wmissing-declarations
CFLAGS += -Wformat -Wmissing-format-attribute -Wno-deprecated-declarations
CFLAGS += -Wpacked -Wredundant-decls -Wnested-externs -Winline -Wlong-long
CFLAGS += -Wunreachable-code
CFLAGS += -Wcast-align
#CFLAGS += -Wmissing-noreturn
#CFLAGS += -Wconversion

CFLAGS += --param max-inline-insns-single=500 -mcpu=cortex-m3 -mthumb -mlong-calls -ffunction-sections -fdata-sections -nostdlib -std=c99
CFLAGS += $(OPTIMIZATION) $(INCLUDES) -D$(CHIP) -D$(VARIANT)

# To reduce application size use only integer printf function.
CFLAGS += -Dprintf=iprintf

# ---------------------------------------------------------------------------------------
# CPP Flags

CPPFLAGS += -Wall -Wchar-subscripts -Wcomment -Wformat=2
CPPFLAGS += -Wmain -Wparentheses -Wcast-align -Wunreachable-code
CPPFLAGS += -Wsequence-point -Wreturn-type -Wswitch -Wtrigraphs -Wunused
CPPFLAGS += -Wuninitialized -Wunknown-pragmas -Wfloat-equal -Wundef
CPPFLAGS += -Wshadow -Wpointer-arith -Wwrite-strings
CPPFLAGS += -Wsign-compare -Waggregate-return -Wmissing-declarations
CPPFLAGS += -Wformat -Wmissing-format-attribute -Wno-deprecated-declarations
CPPFLAGS += -Wpacked -Wredundant-decls -Winline -Wlong-long
#CPPFLAGS += -Wmissing-noreturn
#CPPFLAGS += -Wconversion

CPPFLAGS += --param max-inline-insns-single=500 -mcpu=cortex-m3 -mthumb -mlong-calls -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -std=c++98
CPPFLAGS += $(OPTIMIZATION) $(INCLUDES) -D$(CHIP)

# To reduce application size use only integer printf function.
CPPFLAGS += -Dprintf=iprintf

# ---------------------------------------------------------------------------------------
# ASM Flags

ASFLAGS = -mcpu=cortex-m3 -mthumb -Wall -g $(OPTIMIZATION) $(INCLUDES)
