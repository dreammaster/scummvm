MODULE := engines/wizardry

MODULE_OBJS = \
	wizardry.o \
	console.o \
	events.o \
	messages.o \
	metaengine.o \
	data/globals.o \
	data/scenario.o \
	libs/memory.o \
	libs/wizardry_archive.o \
	gfx/palette.o \
	gfx/surface.o \
	gfx/wizardry_font.o \
	views/view.o \
	views/castle.o

# This module can be built as a plugin
ifeq ($(ENABLE_WIZARDRY), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk

# Detection objects
DETECT_OBJS += $(MODULE)/detection.o
