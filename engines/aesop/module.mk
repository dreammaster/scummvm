MODULE := engines/aesop

MODULE_OBJS = \
	aesop.o \
	console.o \
	metaengine.o \
	event.o \
	eye.o \
	gil2vfx.o \
	graphics.o \
	interface.o \
	interp.o \
	modsnd32.o \
	rtcode.o \
	rtlink.o \
	rtobject.o \
	rtres.o \
	rtsystem.o \
	sound32.o

# This module can be built as a plugin
ifeq ($(ENABLE_AESOP), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk

# Detection objects
DETECT_OBJS += $(MODULE)/detection.o
