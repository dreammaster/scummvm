MODULE := engines/wizardry

MODULE_OBJS = \
	wizardry.o \
	console.o \
	events.o \
	messages.o \
	metaengine.o \
	data/globals.o \
	libs/files.o \
	gfx/palette.o \
	gfx/surface.o \
	views/view.o \
	views/view1.o

# This module can be built as a plugin
ifeq ($(ENABLE_WIZARDRY), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk

# Detection objects
DETECT_OBJS += $(MODULE)/detection.o
