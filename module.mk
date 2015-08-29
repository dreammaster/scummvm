MODULE := engines/aesop

MODULE_OBJS = \
	aesop.o \
	detection.o \
	event.o \
	eye.o \
	gil2vfx.o \
	gil2vfxa.o \
	graphics.o \
	intrface.o \
	mouse.o \
	resources.o \
	rt.o \
	rtcode.o \
	rtobject.o \
	rtsystem.o \
	sound32.o \
	stubs.o

# This module can be built as a plugin
ifeq ($(ENABLE_SHERLOCK), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
