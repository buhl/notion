##
## Management mode module Makefile
##

# System-specific configuration is in system.mk
TOPDIR=..
include $(TOPDIR)/build/system-inc.mk

######################################

INCLUDES += $(X11_INCLUDES) $(LIBTU_INCLUDES) $(LIBEXTL_INCLUDES) -I..
CFLAGS += $(XOPEN_SOURCE) $(C99_SOURCE)

SOURCES=main.c modal.c

MAKE_EXPORTS=mod_modal

MODULE=mod_modal
MODULE_STUB=mod_modal.lua

######################################

include $(TOPDIR)/build/rules.mk

######################################

main.o: exports.h

_install: module_install
