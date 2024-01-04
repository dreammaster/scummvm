MODULE := engines/wasteland

MODULE_OBJS = \
	console.o \
	engine.o \
	events.o \
	messages.o \
	metaengine.o \
	gfx/clickable_text.o \
	gfx/image_decoder.o \
	gfx/surface.o \
	gfx/wasteland_font.o \
	fod/fod.o \
	fod/data/archetypes.o \
	fod/data/party.o \
	fod/data/pics.o \
	fod/views/main_menu.o \
	fod/views/menu_view.o \
	fod/views/title.o

# This module can be built as a plugin
ifeq ($(ENABLE_WASTELAND), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk

# Detection objects
DETECT_OBJS += $(MODULE)/detection.o
