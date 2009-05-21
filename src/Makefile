# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

SHELL = /bin/sh
top_srcdir = ..
srcdir = .


.SUFFIXES:
.SUFFIXES: .c .o

OPT=-g -O

AR = ar
AR_FLAGS = rc
RANLIB = @RANLIB@

CC = gcc
CFLAGS = -I. -g -O2 `sdl-config --cflags`
LDFLAGS =  `sdl-config --libs`
LIBS = -lSDL -lpng -lm -ldl -lGLU -lGL 
INSTALL = install
prefix = /tmp/alala
exec_prefix = ${prefix}
basedir = $(prefix)/starone
bindir = $(basedir)/bin

SOURCES= \
asteroid.c \
beam.c \
color.c \
explosion.c \
list.c \
main.c \
panic.c \
particle.c \
pulse.c \
ship.c \
texture.c \
util.c

OBJS= \
asteroid.o \
beam.o \
color.o \
explosion.o \
list.o \
main.o \
panic.o \
particle.o \
pulse.o \
ship.o \
texture.o \
util.o

all: starone

install: all
	mkdir $(basedir)
	mkdir $(basedir)/res
	$(INSTALL) starone $(basedir)
	cp -r res/* $(basedir)/res


uninstall:
	/bin/rm -rf $(basedir)

starone: $(OBJS)
	$(CC) $(CFLAGS) -o starone $(OBJS) $(LIBS)

clean:
	/bin/rm -f core *.o $(OBJS)

distclean: clean
	/bin/rm -f Makefile config.h config.status config.cache config.log

mostlyclean: clean

maintainer-clean: clean

