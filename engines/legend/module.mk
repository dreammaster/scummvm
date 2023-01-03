MODULE := engines/legend

MODULE_OBJS = \
	gfx/font.o \
	gfx/pic_file.o \
	gfx/screen.o \
	gfx/view_element.o \
	gfx/view.o \
	utils/engine_data.o \
	utils/file.o \
	early/engine.o \
	early/gfx/boxed_element.o \
	early/gfx/button.o \
	early/gfx/screen.o \
	early/gfx/text_area.o \
	early/gfx/view.o \
	early/views/help_screen.o \
	games/gateway/engine.o \
	games/gateway/views/commset.o \
	legend.o \
	console.o \
	events.o \
	messages.o \
	metaengine.o

# This module can be built as a plugin
ifeq ($(ENABLE_LEGEND), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk

# Detection objects
DETECT_OBJS += $(MODULE)/detection.o
