AUTOMAKE_OPTIONS = cygnus
INCLUDES = -I$(src_dir)/os -I$(src_dir)/sys \
	$(NEWLIB_CFLAGS) $(CROSS_CFLAGS) $(TARGET_CFLAGS)
AM_CCASFLAGS = $(INCLUDES)

noinst_LIBRARIES = lib.a

lib_a_SOURCES = syscalls.c
EXTRA_lib_a_SOURCES = crt0.S crti.S crtn.S
lib_a_CCASFLAGS = $(AM_CCASFLAGS)
lib_a_CFLAGS = $(AM_CFLAGS)

all: crt0.o crti.o crtn.o
ACLOCAL_AMFLAGS = -I../../.. -I../../../..
CONFIG_STATUS_DEPENDENCIES = $(newlib_basedir)/configure.host

