MODULE := engines/legend

MODULE_OBJS = \
	legend/debugger.o \
	legend/detection.o \
	legend/events.o \
	legend/font.o \
	legend/legend.o \
	legend/resources.o \
	legend/screen.o

# This module can be built as a plugin
ifeq ($(ENABLE_LEGEND), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
