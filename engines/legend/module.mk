MODULE := engines/legend

MODULE_OBJS = \
	utils/engine_data.o \
	games/gateway/engine.o \
	legend.o \
	console.o \
	events.o \
	messages.o \
	metaengine.o

# This module can be built as a plugin
ifeq ($(ENABLE_LEGEND), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk

# Detection objects
DETECT_OBJS += $(MODULE)/detection.o
