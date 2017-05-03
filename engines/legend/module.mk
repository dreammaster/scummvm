MODULE := engines/legend

MODULE_OBJS = \
	core/file.o \
	core/message_target.o \
	core/named_item.o \
	core/project_item.o \
	core/resources.o \
	core/saveable_object.o \
	core/tree_item.o \
	gfx/font.o \
	gfx/gfx.o \
	gfx/image.o \
	gfx/lines.o \
	gfx/picture.o \
	gfx/room_item.o \
	gfx/screen.o \
	gfx/visual_container.o \
	gfx/visual_item.o \
	early/early_screen.o \
	later/later_screen.o \
	xanth/compendium.o \
	debugger.o \
	detection.o \
	events.o \
	legend.o \
	game_manager.o \
	input_handler.o \
	input_translator.o \
	main_game_window.o \
	messages.o

# This module can be built as a plugin
ifeq ($(ENABLE_LEGEND), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
