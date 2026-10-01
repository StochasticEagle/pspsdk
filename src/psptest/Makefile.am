libdir = @PSPSDK_LIBDIR@
includedir = @PSPSDK_INCLUDEDIR@

CC = @PSP_CC@
CCAS = $(CC)
CCASFLAGS = $(CFLAGS)
AR = @PSP_AR@
RANLIB = @PSP_RANLIB@

CPPFLAGS = -I$(top_srcdir)/src/base -I$(top_srcdir)/src/user -I$(top_srcdir)/src/debug
CFLAGS = @PSPSDK_CFLAGS@

include_HEADERS = psptest.h
lib_LIBRARIES = libpsptest.a
libpsptest_a_SOURCES = psptest.c psptest_call.S
