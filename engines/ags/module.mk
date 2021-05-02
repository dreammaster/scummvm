MODULE := engines/ags

MODULE_OBJS = \
	ags.o \
	events.o \
	game_scanner.o \
	globals.o \
	metaengine.o \
	music.o \
	console.o \
	lib/aastr-0.1.1/aarot.o \
	lib/aastr-0.1.1/aastr.o \
	lib/aastr-0.1.1/aautil.o \
	lib/alfont/alfont.o \
	lib/allegro/color.o \
	lib/allegro/config.o \
	lib/allegro/draw.o \
	lib/allegro/file.o \
	lib/allegro/fixed.o \
	lib/allegro/flood.o \
	lib/allegro/gfx.o \
	lib/allegro/graphics.o \
	lib/allegro/keyboard.o \
	lib/allegro/math.o \
	lib/allegro/mouse.o \
	lib/allegro/surface.o \
	lib/allegro/system.o \
	lib/allegro/unicode.o \
	lib/hq2x/hq2x3x.o \
	lib/std/std.o \
	lib/system/datetime.o \
	shared/ac/dynobj/scriptaudioclip.o \
	shared/ac/audio_clip_type.o \
	shared/ac/character_info.o \
	shared/ac/common.o \
	shared/ac/dialog_topic.o \
	shared/ac/game_setup_struct.o \
	shared/ac/game_setup_struct_base.o \
	shared/ac/inventory_item_info.o \
	shared/ac/keycode.o \
	shared/ac/mouse_cursor.o \
	shared/ac/sprite_cache.o \
	shared/ac/view.o \
	shared/ac/words_dictionary.o \
	shared/core/asset.o \
	shared/core/asset_manager.o \
	shared/debugging/debug_manager.o \
	shared/font/fonts.o \
	shared/font/ttf_font_renderer.o \
	shared/font/wfn_font.o \
	shared/font/wfn_font_renderer.o \
	shared/game/custom_properties.o \
	shared/game/interactions.o \
	shared/game/main_game_file.o \
	shared/game/room_file.o \
	shared/game/room_file_deprecated.o \
	shared/game/room_struct.o \
	shared/gfx/allegro_bitmap.o \
	shared/gfx/bitmap.o \
	shared/gui/gui_button.o \
	shared/gui/gui_inv.o \
	shared/gui/gui_label.o \
	shared/gui/gui_listbox.o \
	shared/gui/gui_main.o \
	shared/gui/gui_object.o \
	shared/gui/gui_slider.o \
	shared/gui/gui_textbox.o \
	shared/gui/custom_properties.o \
	shared/gui/interactions.o \
	shared/gui/main_game_file.o \
	shared/gui/room_file.o \
	shared/gui/room_file_deprecated.o \
	shared/gui/room_struct.o \
	shared/script/cc_error.o \
	shared/script/cc_options.o \
	shared/script/cc_script.o \
	shared/util/aligned_stream.o \
	shared/util/buffered_stream.o \
	shared/util/compress.o \
	shared/util/data_stream.o \
	shared/util/directory.o \
	shared/util/file.o \
	shared/util/file_stream.o \
	shared/util/geometry.o \
	shared/util/ini_file.o \
	shared/util/ini_util.o \
	shared/util/lzw.o \
	shared/util/misc.o \
	shared/util/multi_file_lib.o \
	shared/util/path.o \
	shared/util/proxy_stream.o \
	shared/util/stream.o \
	shared/util/string.o \
	shared/util/string_utils.o \
	shared/util/text_stream_reader.o \
	shared/util/text_stream_writer.o \
	shared/util/version.o \
	shared/util/wgt2_allg.o \
	plugins/agsplugin.o \
	plugins/plugin_base.o \
	plugins/ags_blend/ags_blend.o \
	plugins/ags_controller/ags_controller.o \
	plugins/ags_creditz/ags_creditz.o \
	plugins/ags_creditz/ags_creditz1.o \
	plugins/ags_creditz/ags_creditz2.o \
	plugins/ags_creditz/drawing.o \
	plugins/ags_flashlight/ags_flashlight.o \
	plugins/ags_galaxy_steam/ags_wadjeteye_steam.o \
	plugins/ags_galaxy_steam/ags_galaxy_steam.o \
	plugins/ags_joy/ags_joy.o \
	plugins/ags_nickenstien_gfx/ags_nickenstien_gfx.o \
	plugins/ags_pal_render/ags_pal_render.o \
	plugins/ags_pal_render/raycast.o \
	plugins/ags_parallax/ags_parallax.o \
	plugins/ags_snow_rain/ags_snow_rain.o \
	plugins/ags_snow_rain/weather.o \
	plugins/ags_sprite_font/ags_sprite_font.o \
	plugins/ags_sprite_font/ags_sprite_font_clifftop.o \
	plugins/ags_sprite_font/character_entry.o \
	plugins/ags_sprite_font/color.o \
	plugins/ags_sprite_font/sprite_font.o \
	plugins/ags_sprite_font/sprite_font_renderer.o \
	plugins/ags_sprite_font/sprite_font_renderer_clifftop.o \
	plugins/ags_sprite_font/variable_width_font.o \
	plugins/ags_sprite_font/variable_width_sprite_font.o \
	plugins/ags_sprite_font/variable_width_sprite_font_clifftop.o \
	plugins/ags_shell/ags_shell.o \
	plugins/ags_tcp_ip/ags_tcp_ip.o \
	plugins/ags_wadjet_util/ags_wadjet_util.o

ifdef ENABLE_AGS_TESTS
MODULE_OBJS += \
	tests/test_all.o \
	tests/test_file.o \
	tests/test_gfx.o \
	tests/test_inifile.o \
	tests/test_math.o \
	tests/test_memory.o \
	tests/test_sprintf.o \
	tests/test_string.o \
	tests/test_version.o
endif

# This module can be built as a plugin
ifeq ($(ENABLE_AGS), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk

# Detection objects
DETECT_OBJS += $(MODULE)/detection.o
