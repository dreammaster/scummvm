MODULE := engines/krondor

MODULE_OBJS = \
	krondor.o \
	console.o \
	events.o \
	messages.o \
	metaengine.o \
	files/decompress.o \
	files/file.o \
	files/resource_archive.o \
	gfx/gfx_surface.o \
	gfx/widgets/widget.o \
	gfx/widgets/select.o \
	gfx/widgets/text_button.o \
	res/rect.o \
	res/resource.o \
	res/font_resource.o \
	res/image_resource.o \
	res/label_resource.o \
	res/palette_resource.o \
	res/request_resource.o \
	res/screen_resource.o \
	views/view.o \
	views/view1.o \
	views/dialogs/dialog.o \
	views/dialogs/preferences.o

# This module can be built as a plugin
ifeq ($(ENABLE_KRONDOR), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk

# Detection objects
DETECT_OBJS += $(MODULE)/detection.o
