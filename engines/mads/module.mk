MODULE := engines/mads

MODULE_OBJS := \
	nebular/dialogs_nebular.o \
	nebular/game_nebular.o \
	nebular/globals_nebular.o \
	nebular/menu_nebular.o \
	nebular/sound_nebular.o \
	nebular/nebular_scenes.o \
	nebular/nebular_scenes1.o \
	nebular/nebular_scenes2.o \
	nebular/nebular_scenes3.o \
	nebular/nebular_scenes4.o \
	nebular/nebular_scenes5.o \
	nebular/nebular_scenes6.o \
	nebular/nebular_scenes7.o \
	nebular/nebular_scenes8.o \
	action.o \
	animation.o \
	assets.o \
	audio.o \
	camera.o \
	compression.o \
	conversations.o \
	debugger.o \
	dialogs.o \
	events.o \
	font.o \
	game.o \
	game_data.o \
	globals.o \
	hotspots.o \
	inventory.o \
	mads.o \
	menu_views.o \
	messages.o \
	msurface.o \
	metaengine.o \
	mps_installer.o \
	palette.o \
	player.o \
	rails.o \
	resources.o \
	scene.o \
	scene_data.o \
	screen.o \
	sequence.o \
	sound.o \
	sprites.o \
	staticres.o \
	user_interface.o

ifdef ENABLE_MADSV2
MODULE_OBJS += \
	dragonsphere/game_dragonsphere.o \
	dragonsphere/dragonsphere_scenes.o \
	dragonsphere/dragonsphere_scenes1.o \
	dragonsphere/globals_dragonsphere.o \
	forest/game_forest.o \
	forest/forest_scenes.o \
	forest/globals_forest.o \
	madsv2/engine.o \
	madsv2/core/speech.o \
	madsv2/phantom/mads/mads.o \
	madsv2/phantom/main_menu.o \
	madsv2/phantom/main.o
endif

# This module can be built as a plugin
ifeq ($(ENABLE_MADS), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk

# Detection objects
DETECT_OBJS += $(MODULE)/detection.o
