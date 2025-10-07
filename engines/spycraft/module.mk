MODULE := engines/spycraft

MODULE_OBJS = \
	spycraft.o \
	console.o \
	metaengine.o \
	dmade/advback.o \
	dmade/advbits.o \
	dmade/advdcmp.o \
	dmade/advlist.o \
	dmade/advmem.o \
	dmade/advport.o \
	dmade/advrect.o \
	dmade/advres.o \
	dmade/advsfx.o \
	dmade/advsprite.o \
	dmade/advtime.o \
	game/dispatch.o \
	game/list.o \
	ats.o

# This module can be built as a plugin
ifeq ($(ENABLE_SPYCRAFT), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk

# Detection objects
DETECT_OBJS += $(MODULE)/detection.o
