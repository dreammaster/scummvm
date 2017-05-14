
MODULE := devtools/create_legend

MODULE_OBJS := \
	create_legend_dat.o \
	early_data.o \
	gateway_data.o \
	xanth_data.o

# Set the name of the executable
TOOL_EXECUTABLE := create_titanic

# Include common rules
include $(srcdir)/rules.mk
