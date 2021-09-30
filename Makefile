OS_NAME = TokyoOS

OVMF = bin/OVMF.fd
CC = gcc
LD = ld
ASSEMBLER = nasm
LINK_SCRIPT = ./link.ld

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

INCDIR = include
SRCDIR = src
OBJDIR = lib
BUILDDIR = bin
LOGDIR = log
UTILSDIR = utils
KERNEL_ELF = $(BUILDDIR)/kernel.elf
OS_IMG = $(BUILDDIR)/$(OS_NAME).img

USRDIR = usr
USROBJDIR = lib/usr
USRELFDIR = disk/ext2dir/usr
USER_ELF = $(USRELFDIR)/main.elf
USR_LIBC = usr/libc/include

SRC = $(call rwildcard,$(SRCDIR),*.cpp)
ASMSRC = $(call rwildcard,$(SRCDIR),*.s)
PSFSRC = $(call rwildcard,$(SRCDIR),*.psf)
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))
OBJS += $(patsubst $(SRCDIR)/%.s, $(OBJDIR)/%_s.o, $(ASMSRC))
OBJS += $(patsubst $(SRCDIR)/%.psf, $(OBJDIR)/%_font.o, $(PSFSRC))
DIRS = $(wildcard $(SRCDIR)/*)


########################################################################################
# User files

LIBC_CSRC = $(call rwildcard,usr/libc,*.c)
LIBC_SSRC = $(call rwildcard,usr/libc,*.s)
LIBC_OBJ = $(patsubst usr/libc/%.c, lib/usr/libc/%.o, $(LIBC_CSRC))
LIBC_OBJ += $(patsubst usr/libc/%.s, lib/usr/libc/%_s.o, $(LIBC_SSRC))

USR_START_OBJ = lib/usr/start_s.o

USR0_SRC = usr/main.c
USR0_OBJ = $(patsubst usr/%.c, lib/usr/%.o, $(USR0_SRC))
USR0_OBJ += $(USR_START_OBJ)
USR0_ELF = $(USRELFDIR)/main.elf

#########################################################################################


MKBOOTIMG = $(UTILSDIR)/mkbootimg
BOOTJSON = $(UTILSDIR)/mkbootimg.json

IGNORE_ERRORS = -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function -Wno-unused-but-set-variable
CFLAGS = -ffreestanding -mno-red-zone -fpic -fno-stack-protector -fno-exceptions -fno-rtti -nostdlib -Werror -Wall -Wextra $(IGNORE_ERRORS)
ASMFLAGS = 
LDFLAGS = -nostdlib -nostartfiles
STRIPFLAGS = -s -K mmio -K fb -K bootboot -K environment -K initstack

all: initdir disk

initdir: kernel
	@mkdir initrd initrd/sys 2>/dev/null | true
	cp $(KERNEL_ELF) initrd/sys/core

disk: initdir $(BOOTJSON)
	./$(MKBOOTIMG) $(BOOTJSON) $(OS_IMG)
	@rm -rf initrd

kernel: $(OBJS) link

$(OBJDIR)/Interrupts/Interrupts.o: $(SRCDIR)/Interrupts/Interrupts.cpp
	@echo !==== COMPILING $^
	mkdir -p $(@D)
	$(CC) -I$(INCDIR) -mno-red-zone -mgeneral-regs-only -ffreestanding -c $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@echo !==== COMPILING $^
	mkdir -p $(@D)
	$(CC) -I$(INCDIR) $(CFLAGS) -c $^ -o $@

$(OBJDIR)/%_s.o: $(SRCDIR)/%.s
	@echo !==== ASSEMBLING $^
	mkdir -p $(@D)
	$(ASSEMBLER) $(ASMFLAGS) $^ -f elf64 -o $@

$(OBJDIR)/%_font.o: $(SRCDIR)/%.psf
	@echo !==== COMPILING $^
	mkdir -p $(@D)
	@cp $^ ./
	$(LD) $(LDFLAGS) -r -b binary -o $@ font.psf
	@rm font.psf

link:
	@echo !==== LINKING
	$(LD) $(LDFLAGS) -T $(LINK_SCRIPT) $(OBJS) -o $(KERNEL_ELF)
	strip $(STRIPFLAGS) $(KERNEL_ELF)
	readelf -hls $(KERNEL_ELF) > $(LOGDIR)/kernel.x86_64.txt
	
run:
	qemu-system-x86_64 -machine q35 -cpu qemu64 -bios $(OVMF) -m 64 -drive file=$(OS_IMG),format=raw -serial file:log/serial.log



libc: $(LIBC_OBJ)

lib/usr/libc/%.o: usr/libc/*.c
	@echo !==== COMPILING LIBC $^
	mkdir -p $(@D)
	$(CC) -I$(USR_LIBC) -ffreestanding -nostdlib -c $^ -o $@

user0: $(USR0_OBJ) linkUser

$(USROBJDIR)/%.o: $(USRDIR)/%.c
	@echo !==== COMPILING USER $^
	mkdir -p $(@D)
	$(CC) -I$(USR_LIBC) -ffreestanding -nostdlib -c $^ -o $@

$(USROBJDIR)/%_s.o: $(USRDIR)/%.s
	@echo !==== ASSEMBLING USER $^
	mkdir -p $(@D)
	$(ASSEMBLER) $^ -f elf64 -o $@

linkUser:
	@echo !==== LINKING USER
	$(LD) $(LDFLAGS) $(USR0_OBJ) $(LIBC_OBJ) -o $(USER_ELF)

clean:
	rm -rf $(OBJDIR)/*