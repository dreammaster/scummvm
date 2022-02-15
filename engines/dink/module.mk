MODULE := engines/dink

MODULE_OBJS = \
	dink.o \
	var.o \
	fast_file.o \
	metaengine.o

# This module can be built as a plugin
ifeq ($(ENABLE_DINK), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk

# Detection objects
DETECT_OBJS += $(MODULE)/detection.o
