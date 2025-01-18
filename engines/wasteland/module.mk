MODULE := engines/wasteland

MODULE_OBJS = \
	console.o \
	engine.o \
	events.o \
	keymapping.o \
	messages.o \
	metaengine.o \
	core/array.o \
	core/file.o \
	core/serializer.o \
	gfx/clickable_text.o \
	gfx/surface.o \
	gfx/text_entry.o \
	gfx/text_view.o \
	gfx/wasteland_font.o \
	wasteland1/wasteland1.o \
	wasteland1/console.o \
	wasteland1/core/array1.o \
	wasteland1/core/text_decoder.o \
	wasteland1/data/items.o \
	wasteland1/data/msq_header.o \
	wasteland1/data/party.o \
	wasteland1/data/party_member.o \
	wasteland1/data/saved.o \
	wasteland1/data/skills.o \
	wasteland1/data/text.o \
	wasteland1/files/bit_stream.o \
	wasteland1/files/game_archive.o \
	wasteland1/files/huffman_stream.o \
	wasteland1/files/huffman_node.o \
	wasteland1/files/huffman_tree.o \
	wasteland1/files/nibble_stream.o \
	wasteland1/files/rotating_xor_stream.o \
	wasteland1/files/vertical_xor_stream.o \
	wasteland1/gfx/cursors.o \
	wasteland1/gfx/color_font.o \
	wasteland1/gfx/mono_font.o \
	wasteland1/gfx/pic.o \
	wasteland1/gfx/pics.o \
	wasteland1/gfx/pics_animation.o \
	wasteland1/gfx/pics_animation_frame_set.o \
	wasteland1/gfx/raw_animation_frame.o \
	wasteland1/gfx/raw_animation_frame_part.o \
	wasteland1/views/view.o \
	wasteland1/views/character/inventory.o \
	wasteland1/views/character/reorder_pane.o \
	wasteland1/views/character/skills.o \
	wasteland1/views/character/summary.o \
	wasteland1/views/gfx/animation.o \
	wasteland1/views/gfx/button.o \
	wasteland1/views/gfx/clickable_text.o \
	wasteland1/views/gfx/paged_pane.o \
	wasteland1/views/gfx/pane.o \
	wasteland1/views/dialogs/dialog.o \
	wasteland1/views/title/party_member.o \
	wasteland1/views/title/title.o \
	wasteland1/views/title/roster.o \
	fod/fod.o \
	fod/console.o \
	fod/sound.o \
	fod/core/decoder.o \
	fod/data/archetypes.o \
	fod/data/archive.o \
	fod/data/disk.o \
	fod/data/disk1.o \
	fod/data/globals.o \
	fod/data/hds.o \
	fod/data/map.o \
	fod/data/packets.o \
	fod/data/party.o \
	fod/data/pics.o \
	fod/data/strings.o \
	fod/data/weapons.o \
	fod/gfx/cursors.o \
	fod/gfx/image_decoder.o \
	fod/gfx/font.o \
	fod/gfx/tiles.o \
	fod/logic/game.o \
	fod/logic/scripts.o \
	fod/views/core/base_view.o \
	fod/views/dialogs/dialog.o \
	fod/views/dialogs/all_character_info.o \
	fod/views/dialogs/character_info.o \
	fod/views/dialogs/message_dialog.o \
	fod/views/dialogs/quit.o \
	fod/views/dialogs/yes_no.o \
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
