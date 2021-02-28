# CROSS_COMPILE = arm-buildroot-linux-gnueabihf-  # get it from environment
AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CC		= $(CROSS_COMPILE)gcc
CPP		= $(CC) -E
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm

STRIP		= $(CROSS_COMPILE)strip
OBJCOPY		= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump

export AS LD CC CPP AR NM
export STRIP OBJCOPY OBJDUMP

CFLAGS := -g `pkg-config --libs --cflags gio-unix-2.0`
CFLAGS += -I $(shell pwd)/

# LDFLAGS := -T ldscript.lds
LDFLAGS := `pkg-config --libs gio-unix-2.0` -l dbus-1 

export CFLAGS LDFLAGS

TOPDIR := $(shell pwd)
export TOPDIR

TARGET := app
PRE_BUILD  :=
POST_BUILD :=

obj-y += main.o
obj-y += ../


all : pre-build start_recursive_build $(TARGET) post-build
	@echo $(TARGET) has been built! 	

start_recursive_build:
	make -C ./ -f $(TOPDIR)/Makefile.build

$(TARGET) : built-in.o
	$(CC) -o  $(TARGET) built-in.o $(LDFLAGS)

pre-build:
	@$(PRE_BUILD)

post-build:
	@$(POST_BUILD)

clean:
	rm -f $(shell find -name "*.o")
	rm -f $(TARGET)

install:
	mv $(TARGET) ../$(TARGET)

distclean:
	rm -f $(TARGET)
	rm -f $(shell find -name "*.o")
	rm -f $(shell find -name "*.d")
	

	