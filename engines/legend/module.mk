MODULE := engines/legend

MODULE_OBJS = \
	core/font.o \
	core/file.o \
	core/image.o \
	core/message_target.o \
	core/named_item.o \
	core/picture.o \
	core/project_item.o \
	core/resources.o \
	core/room_item.o \
	core/saveable_object.o \
	core/screen.o \
	core/tree_item.o \
	core/visual_container.o \
	core/visual_item.o \
	early/early_screen.o \
	later/later_screen.o \
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
