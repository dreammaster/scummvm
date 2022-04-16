MODULE := engines/legend

MODULE_OBJS = \
	debugger.o \
	file.o \
	font.o \
	globals.o \
	graphics.o \
	legend.o \
	memory.o \
	metaengine.o \
	windows.o \
	shannara/flix.o \
	shannara/globals.o \
	shannara/shannara_demo.o

# This module can be built as a plugin
ifeq ($(ENABLE_LEGEND), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk

# Detection objects
DETECT_OBJS += $(MODULE)/detection.o
