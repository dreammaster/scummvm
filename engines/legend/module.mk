MODULE := engines/legend

MODULE_OBJS = \
	early/early_screen.o \
	later/later_screen.o \
	debugger.o \
	detection.o \
	events.o \
	font.o \
	file.o \
	legend.o \
	input_area.o \
	messages.o \
	pic_file.o \
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
