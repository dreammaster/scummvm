MODULE := engines/dink

MODULE_OBJS = \
	data.o \
	dink.o \
	events.o \
	file.o \
	fast_file.o \
	freedink.o \
	globals.o \
	metaengine.o \
	sound.o \
	text.o \
	update_frame.o \
	var.o \
	directdraw\ddraw.o \
	directdraw\ddutil.o \
	directdraw\dinput.o \
	directdraw\dsound.o \
	lib\graphics.o

# This module can be built as a plugin
ifeq ($(ENABLE_DINK), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk

# Detection objects
DETECT_OBJS += $(MODULE)/detection.o
