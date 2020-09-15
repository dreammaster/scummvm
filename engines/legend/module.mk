MODULE := engines/legend

MODULE_OBJS := \
	detection.o \
	legend.o \
	shannara_demo/shannara_demo.o

# This module can be built as a plugin
ifeq ($(ENABLE_LEGEND), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
