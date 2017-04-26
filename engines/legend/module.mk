MODULE := engines/legend

MODULE_OBJS = \
	core/font.o \
	core/file.o \
	core/image.o \
	core/picture.o \
	core/resources.o \
	core/screen.o \
	early/early_screen.o \
	later/later_screen.o \
	debugger.o \
	detection.o \
	events.o \
	legend.o \
	messages.o

# This module can be built as a plugin
ifeq ($(ENABLE_LEGEND), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
