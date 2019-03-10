MODULE := engines/dungeoncraft

MODULE_OBJS := \
	core/archive.o \
	shared/asl.o
#	shared/char.o \
#	shared/class.o \
#	shared/config_file.o \
#	shared/file_parse.o \
#	shared/game_event.o \
#	shared/game_rules.o \
#	shared/globals.o \
#	shared/global_data.o \
#	shared/gpdl_comp.o \
#	shared/gpdl_exec.o \
#	shared/graphics.o \
#	shared/items.o \
#	shared/level.o \
#	shared/money.o \
#	shared/monster.o \
#	shared/movie.o \
#	shared/party.o \
#	shared/pic_data.o \
#	shared/pic_slot.o \
#	shared/property.o \
#	shared/regexp.o \
#	shared/runtime_if.o \
#	shared/sound_mgr.o \
#	shared/specab.o \
#	shared/spell.o \
#	shared/stack_walker.o \
#	shared/taglist.o \
#	shared/timer.o \
#	shared/traits.o \
#	shared/viewport.o \
#	runtime/cdx_bitmap_font.o \
#	runtime/char_stats_form.o \
#	runtime/combatant.o \
#	runtime/combatants.o \
#	runtime/cprocinp.o \
#	runtime/dgngame.o \
#	runtime/disptext.o \
#	runtime/drawtile.o \
#	runtime/dungeon.o \
#	runtime/formatted_text.o \
#	runtime/game_menu.o \
#	runtime/getinput.o \
#	runtime/items_form.o \
#	runtime/mainfrm.o \
#	runtime/mouse_thread.o \
#	runtime/object_scripts.o \
#	runtime/path.o \
#	runtime/rest_time_form.o \
#	runtime/run_event.o \
#	runtime/screen.o \
#	runtime/spell_form.o \
#	runtime/text_form.o \
#	runtime/treas.o


# This module can be built as a plugin
ifeq ($(ENABLE_GLK), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
