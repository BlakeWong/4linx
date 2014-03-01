CROSS_COMPILE = arm-linux-gnueabi-

#
# Define toolchain commands for cross-compilation
# 
AS	= $(CROSS_COMPILE)as
LD	= $(CROSS_COMPILE)ld
CC	= $(CROSS_COMPILE)gcc
CPP	= $(CC) -E
AR	= $(CROSS_COMPILE)ar
NM	= $(CROSS_COMPILE)nm
STRIP	= $(CROSS_COMPILE)strip
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
RANLIB	= $(CROSS_COMPILE)RANLIB

CPPFLAGS := -D__KERNEL_ -Os
CPPFLAGS += -I./include
CFLAGS := $(CPPFLAGS) -Wall -Wstrict-prototypes
AFLAGS := -D__ASSEMBLY__ $(CPPFLAGS)
OBJCFLAGS += --gap-fill=0xff


SRC_C = $(wildcard *.c)
SRC_S = $(wildcard *.S)
OBJ_C = $(SRC_C:.c=.o)
OBJ_S = $(SRC_S:.S=.o)
OBJ_ALL = $(OBJ_C) $(OBJ_S)

# Target
bl1.bin: bl1.elf
	$(OBJCOPY) $(OBJCFLAGS) -O binary $^ $@

bl1.elf: $(OBJ_ALL)
	$(LD) -T bl1.lds $^ -o $@ -L /usr/lib/gcc/arm-linux-gnueabi/4.4.5 -lgcc
	$(OBJDUMP) -d $@ > $(@:.elf=).dis

# 
# Basic rules
# 
%.s:	%.S
	$(CPP) $(AFLAGS) -o $@ $<

%.o:	%.S
	$(CC) $(AFLAGS) -c -o $@ $<

%.o:	%.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -f $(OBJ_ALL) bl1.elf bl1.dis
