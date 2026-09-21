TARGET = sample
OBJS = crt0.o main.o 

INCDIR = ../../include
CFLAGS = -O2 -G0 -Wall -fshort-wchar -fno-pic -mno-check-zero-division 
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS) -c

LIBDIR = ../../lib
LDFLAGS = -mno-crt0 -nostdlib -nodefaultlibs
LIBS = -lvlfgui -lvlfgu -lvlfutils -lvlflibc

PSP_FW_VERSION = 271

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = VLF sample
PSP_EBOOT_ICON = icon0.png

BUILD_PRX = 1
PSP_LARGE_MEMORY = 1

PSPSDK=$(shell psp-config --pspsdk-path)
include ./build.mak

