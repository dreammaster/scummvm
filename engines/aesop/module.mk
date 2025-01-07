MODULE := engines/aesop

MODULE_OBJS = \
	aesop.o \
	console.o \
	event.o \
	files.o \
	interface.o \
	metaengine.o \
	rtobject.o \
	rtres.o \
	rtsystem.o \
	vars.o \
	system/timers.o \
	system/vfx.o

# This module can be built as a plugin
ifeq ($(ENABLE_AESOP), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk

# Detection objects
DETECT_OBJS += $(MODULE)/detection.o
