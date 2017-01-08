MODULE := engines/legend

MODULE_OBJS = \
	early/early_screen.o \
	later/later_screen.o \
	debugger.o \
	detection.o \
	events.o \
	font.o \
	file.o \
	image.o \
	input_area.o \
	legend.o \
	messages.o \
	picture.o \
	resources.o \
	scene_area.o \
	screen.o \
	screen_area.o

# This module can be built as a plugin
ifeq ($(ENABLE_LEGEND), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
