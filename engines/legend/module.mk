MODULE := engines/legend

MODULE_OBJS = \
	core/file.o \
	core/message_target.o \
	core/mouse_cursor.o \
	core/named_item.o \
	core/project_item.o \
	core/resources.o \
	core/saveable_object.o \
	core/string.o \
	core/tree_item.o \
	gfx/compass.o \
	gfx/font.o \
	gfx/gfx.o \
	gfx/image.o \
	gfx/picture.o \
	gfx/region.o \
	gfx/screen.o \
	gfx/visual_container.o \
	gfx/visual_item.o \
	early/boxed_element.o \
	early/button.o \
	early/command_buttons.o \
	early/help_screen.o \
	early/listbox.o \
	early/scene.o \
	early/scene_picture.o \
	early/screen.o \
	early/text_area.o \
	early/core/huffman.o \
	early/core/vocab.o \
	early/gateway/gateway_project_item.o \
	early/gateway/part_title.o \
	later/screen.o \
	later/xanth/xanth_project_item.o \
	later/xanth/background.o \
	later/xanth/conversation.o \
	debugger.o \
	detection.o \
	events.o \
	legend.o \
	game_manager.o \
	game_state.o \
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
