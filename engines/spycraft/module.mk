MODULE := engines/spycraft

MODULE_OBJS = \
	spycraft.o \
	console.o \
	metaengine.o \
	advback.o \
	advbits.o \
	advdcmp.o \
	advlist.o \
	advmem.o \
	advport.o \
	advrect.o \
	advres.o \
	advsfx.o \
	advsprite.o \
	advtime.o \
	ats.o

# This module can be built as a plugin
ifeq ($(ENABLE_SPYCRAFT), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk

# Detection objects
DETECT_OBJS += $(MODULE)/detection.o
