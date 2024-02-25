MODULE := engines/ags2

MODULE_OBJS = \
	ags2.o \
	console.o \
	metaengine.o

# This module can be built as a plugin
ifeq ($(ENABLE_AGS2), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk

# Detection objects
DETECT_OBJS += $(MODULE)/detection.o
