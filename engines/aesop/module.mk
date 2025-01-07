MODULE := engines/aesop

MODULE_OBJS = \
	aesop.o \
	console.o \
	event.o \
	interface.o \
	gil2vfx.o \
	metaengine.o \
	rt.o \
	rtlink.o \
	rtobject.o \
	rtres.o \
	rtsystem.o \
	sound.o \
	vars.o \
	system/ail32.o \
	system/events.o \
	system/files.o \
	system/mouse.o \
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
