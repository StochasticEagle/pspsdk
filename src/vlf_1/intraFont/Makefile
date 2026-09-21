TARGET = intraFont
OBJS = main.o intraFont.o GraphicsUtility.o exports.o 

INCDIR = 
CFLAGS = -O2 -G0 -Wall -fshort-wchar -DINTRAFONT_NO_SJIS $(DEFINES) -fno-pic -mno-check-zero-division 
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS = -mno-crt0 -nostdlib -nodefaultlibs
LIBS += -lvlflibc

INCDIR = ./include

BUILD_PRX = 1
PRX_EXPORTS = exports.exp

PSPSDK=$(shell psp-config --pspsdk-path)
include ./build.mak
