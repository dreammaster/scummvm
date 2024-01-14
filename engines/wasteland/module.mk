MODULE := engines/wasteland

MODULE_OBJS = \
	console.o \
	engine.o \
	events.o \
	messages.o \
	metaengine.o \
	core/array.o \
	gfx/clickable_text.o \
	gfx/image_decoder.o \
	gfx/surface.o \
	gfx/text_entry.o \
	gfx/wasteland_font.o \
	fod/fod.o \
	fod/data/archetypes.o \
	fod/data/archive.o \
	fod/data/disk.o \
	fod/data/disk1.o \
	fod/data/globals.o \
	fod/data/hds.o \
	fod/data/map.o \
	fod/data/party.o \
	fod/data/pics.o \
	fod/data/scripts.o \
	fod/data/strings.o \
	fod/data/weapons.o \
	fod/views/core/base_view.o \
	fod/views/dialogs/dialog.o \
	fod/views/dialogs/all_character_info.o \
	fod/views/dialogs/character_info.o \
	fod/views/dialogs/quit.o \
	fod/views/game/game.o \
	fod/views/main_menu/edit_member.o \
	fod/views/main_menu/main_menu.o \
	fod/views/main_menu/menu_view.o \
	fod/views/main_menu/remove_member.o \
	fod/views/main_menu/title.o

# This module can be built as a plugin
ifeq ($(ENABLE_WASTELAND), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk

# Detection objects
DETECT_OBJS += $(MODULE)/detection.o
