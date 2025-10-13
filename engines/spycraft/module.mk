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
	dmade/advmisc.o \
	dmade/advport.o \
	dmade/advrect.o \
	dmade/advres.o \
	dmade/advsfx.o \
	dmade/advsprite.o \
	dmade/advtime.o \
	dmade/ats.o \
	dmade/dirmovie.o \
	dmade/dirscreen.o \
	game/dispatch.o \
	game/actor.o \
	game/cursor.o \
	game/dialog.o \
	game/dlgbox.o \
	game/dump.o \
	game/email.o \
	game/feature.o \
	game/game.o \
	game/list.o \
	game/user.o

# This module can be built as a plugin
ifeq ($(ENABLE_SPYCRAFT), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk

# Detection objects
DETECT_OBJS += $(MODULE)/detection.o
