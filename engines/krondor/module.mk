MODULE := engines/krondor

MODULE_OBJS = \
	krondor.o \
	console.o \
	events.o \
	messages.o \
	metaengine.o \
	files/resource_archive.o \
	views/view.o \
	views/view1.o \
	views/dialogs/preferences.o

# This module can be built as a plugin
ifeq ($(ENABLE_KRONDOR), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk

# Detection objects
DETECT_OBJS += $(MODULE)/detection.o
