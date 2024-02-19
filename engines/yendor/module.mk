MODULE := engines/yendor

MODULE_OBJS = \
	yendor.o \
	console.o \
	events.o \
	messages.o \
	metaengine.o \
	gfx/font.o \
	utils/engine_data.o \
	utils/file_entry.o \
	utils/strings.o \
	yendor2/globals.o \
	yendor2/metaengine.o \
	yendor2/yendor2.o \
	yendor2/views/view1.o

# This module can be built as a plugin
ifeq ($(ENABLE_YENDOR), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk

# Detection objects
DETECT_OBJS += $(MODULE)/detection.o
