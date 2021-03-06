CROSS_COMPILE ?= # arm-buildroot-linux-gnueabihf-  # get it from environment
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

# add support for char set
CFLAGS := -g -finput-charset=UTF-8 -fexec-charset=UTF-8 # get it from the editor you use
# add include path
CFLAGS += -I $(shell pwd)/src
# https://gcc.gnu.org/onlinedocs/gcc/Option-Summary.html
CFLAGS += -Werror -Wall


# LDFLAGS := -T ldscript.lds
LDFLAGS := -lts -lpthread -lfreetype -lm

export CFLAGS LDFLAGS

TOPDIR := $(shell pwd)
export TOPDIR

TARGET := testapp
PRE_BUILD  :=
POST_BUILD :=

obj-y += src/
# obj-y += test/

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
	mkdir -p output/usr/bin/
	mkdir -p output/etc/ts_app/
	mv $(TARGET) output/usr/bin/$(TARGET)
	cp config.conf output/etc/ts_app/config.conf

distclean:
	rm -f $(TARGET)
	rm -f $(shell find -name "*.o")
	rm -f $(shell find -name "*.d")
	

	