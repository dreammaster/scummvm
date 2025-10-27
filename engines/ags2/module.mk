MODULE := engines/ags2

MODULE_OBJS = \
	ags2.o \
	ac.o \
	console.o \
	metaengine.o \
	routefnd.o \
	vars.o \
	common/clib32.o \
	common/fmem.o \
	common/misc.o \
	common/wgt2allg.o \
	gfx/sprite_cache.o \
	lib/allegro/file.o

# This module can be built as a plugin
ifeq ($(ENABLE_AGS2), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk

# Detection objects
DETECT_OBJS += $(MODULE)/detection.o

