MODULE := engines/spycraft

MODULE_OBJS = \
	spycraft.o \
	console.o \
	metaengine.o \
	adv/advback.o \
	adv/advbits.o \
	adv/advdcmp.o \
	adv/advlist.o \
	adv/advmem.o \
	adv/advport.o \
	adv/advrect.o \
	adv/advres.o \
	adv/advsfx.o \
	adv/advsprite.o \
	adv/advtime.o \
	game/dispatch.o \
	ats.o

# This module can be built as a plugin
ifeq ($(ENABLE_SPYCRAFT), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk

# Detection objects
DETECT_OBJS += $(MODULE)/detection.o
