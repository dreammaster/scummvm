MODULE := engines/aesop

MODULE_OBJS = \
	aesop.o \
	debugger.o \
	detection.o \
	events.o \
	eye.o \
	files.o \
	gil2vfxa.o \
	interpreter.o \
	intrface.o \
	mouse.o \
	objects.o \
	resource_structures.o \
	resources.o \
	rtcode.o \
	rtsystem.o \
	screen.o \
	shapes.o \
	sound32.o \
	stubs.o \
	text_windows.o \
	utils.o \
	windows.o

# This module can be built as a plugin
ifeq ($(ENABLE_SHERLOCK), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
