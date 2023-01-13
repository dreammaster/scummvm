MODULE := engines/legend

MODULE_OBJS = \
	gfx/compass.o \
	gfx/font.o \
	gfx/image.o \
	gfx/mouse_cursor.o \
	gfx/picture.o \
	gfx/region.o \
	gfx/screen.o \
	gfx/view_element.o \
	gfx/view.o \
	utils/engine_data.o \
	utils/file.o \
	utils/resources.o \
	utils/string.o \
	early/engine.o \
	early/gfx/boxed_element.o \
	early/gfx/button.o \
	early/gfx/command_buttons.o \
	early/gfx/listbox.o \
	early/gfx/scene.o \
	early/gfx/screen.o \
	early/gfx/text_area.o \
	early/gfx/text_window.o \
	early/gfx/view.o \
	early/utils/huffman.o \
	early/utils/vocab.o \
	early/views/help_screen.o \
	early/views/interface.o \
	games/gateway/engine.o \
	games/gateway/methods.o \
	games/gateway/utils/queue.o \
	games/gateway/views/commset.o \
	games/gateway/views/part_title.o \
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
