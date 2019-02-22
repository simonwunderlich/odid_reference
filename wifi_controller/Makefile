#!/usr/bin/make -f
# -*- makefile -*-

BINARY_NAME = wifi_controller

OBJ += main.o 
# TODO: make a proper library
OBJ += ../odid/odid.o ../odid/odid_wifi.o ../odid/base64.o

# flags and options
CFLAGS += -I../odid
CFLAGS += -Wall -W -Wno-unused-parameter -std=gnu99 -fno-strict-aliasing -MD -MP
CPPFLAGS += -D_GNU_SOURCE
LDLIBS +=

# disable verbose output
ifneq ($(findstring $(MAKEFLAGS),s),s)
ifndef V
	Q_CC = @echo '   ' CC $@;
	Q_LD = @echo '   ' LD $@;
	export Q_CC
	export Q_LD
endif
endif

ifeq ($(origin PKG_CONFIG), undefined)
  PKG_CONFIG = pkg-config
  ifeq ($(shell which $(PKG_CONFIG) 2>/dev/null),)
    $(error $(PKG_CONFIG) not found)
  endif
endif

ifeq ($(origin LIBGPS_CFLAGS) $(origin LIBGPS_LDLIBS), undefined undefined)
  LIBGPS_NAME ?= libgps
  ifeq ($(shell $(PKG_CONFIG) --modversion $(LIBGPS_NAME) 2>/dev/null),)
    $(error No $(LIBGPS_NAME) development libraries found!)
  endif
  LIBGPS_CFLAGS += $(shell $(PKG_CONFIG) --cflags $(LIBGPS_NAME))
  LIBGPS_LDLIBS +=  $(shell $(PKG_CONFIG) --libs $(LIBGPS_NAME)) -lm
endif
CFLAGS += $(LIBGPS_CFLAGS)
LDLIBS += $(LIBGPS_LDLIBS)

CC ?= gcc
RM ?= rm -f
INSTALL ?= install
MKDIR ?= mkdir -p
COMPILE.c = $(Q_CC)$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
LINK.o = $(Q_LD)$(CC) $(CFLAGS) $(LDFLAGS) $(TARGET_ARCH)

# standard install paths
PREFIX = /usr/local
SBINDIR = $(PREFIX)/sbin

# default target
all: $(BINARY_NAME)

# standard build rules
.SUFFIXES: .o .c

.c.o:
	$(COMPILE.c) -o $@ $<

$(BINARY_NAME): $(OBJ)
	$(LINK.o) $^ $(LDLIBS) -o $@

clean:
	$(RM) $(BINARY_NAME) $(OBJ) $(DEP)

install: $(BINARY_NAME)
	$(MKDIR) $(DESTDIR)$(SBINDIR)

# load dependencies
DEP = $(OBJ:.o=.d)
-include $(DEP)

.PHONY: all clean install
