MODULE := engines/ags2

MODULE_OBJS = \
	ags2.o \
	console.o \
	metaengine.o \
	vars.o \
	ac/acfonts.o \
	ac/acplatform.o \
	ac/acplatform_scummvm.o \
	ac/acsound.o \
	ac/agsplugin.o \
	ac/main.o \
	common/clib32.o \
	common/compress.o \
	common/cscommon.o \
	common/csrun.o \
	common/fmem.o \
	common/misc.o \
	common/routefnd.o \
	common/wgt2allg.o \
	gfx/sprite.o \
	gfx/sprite_cache.o \
	lib/alfont/alfont.o \
	lib/allegro/color.o \
	lib/allegro/digi.o \
	lib/allegro/file.o \
	lib/allegro/fixed.o \
	lib/allegro/flood.o \
	lib/allegro/gfx.o \
	lib/allegro/graphics.o \
	lib/allegro/math.o \
	lib/allegro/midi.o \
	lib/allegro/rotate.o \
	lib/allegro/sound.o \
	lib/allegro/surface.o \
	lib/allegro/surface_generic.o \
	lib/allegro/system.o  \
	lib/allegro/timer.o \
	lib/allegro/unicode.o \
	lib/libcda.o

# This module can be built as a plugin
ifeq ($(ENABLE_AGS2), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk

# Detection objects
DETECT_OBJS += $(MODULE)/detection.o

