MODULE := engines/krondor

MODULE_OBJS = \
	krondor.o \
	console.o \
	events.o \
	messages.o \
	metaengine.o \
	core/prefs.o \
	core/vars.o \
	files/decompress.o \
	files/file.o \
	files/resource_archive.o \
	gfx/gfx_surface.o \
	gfx/mouse_cursor.o \
	gfx/movie_decoder.o \
	gfx/widgets/widget.o \
	gfx/widgets/button.o \
	gfx/widgets/choice.o \
	gfx/widgets/text.o \
	gfx/widgets/text_button.o \
	res/rect.o \
	res/resource.o \
	res/resource_tag.o \
	res/animation_resource.o \
	res/font_resource.o \
	res/image_resource.o \
	res/label_resource.o \
	res/movie_resource.o \
	res/palette_resource.o \
	res/request_resource.o \
	res/screen_resource.o \
	res/sound_resource.o \
	views/view.o \
	views/movie.o \
	views/dialogs/dialog.o \
	views/dialogs/options.o \
	views/dialogs/preferences.o

# This module can be built as a plugin
ifeq ($(ENABLE_KRONDOR), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk

# Detection objects
DETECT_OBJS += $(MODULE)/detection.o
