OS_NAME = TokyoOS

OVMF = bin/OVMF.fd
CC = g++#~/opt/cross/bin/x86_64-elf-gcc
LD = ld#~/opt/cross/bin/x86_64-elf-ld
ASSEMBLER = nasm
LINK_SCRIPT = src/link.ld

IGNORE_ERRORS = -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function -Wno-unused-but-set-variable
CFLAGS = -ffreestanding -mno-red-zone -fpic -fno-stack-protector -fno-exceptions -fno-rtti -nostdlib -Werror -Wall -Wextra $(IGNORE_ERRORS)
ASMFLAGS = 
LDFLAGS = -nostdlib -nostartfiles
STRIPFLAGS = -s -K mmio -K fb -K bootboot -K environment -K initstack

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRCDIR = src
OBJDIR = lib
BUILDDIR = bin
LOGDIR = log
UTILSDIR = utils
KERNEL_ELF = $(BUILDDIR)/kernel.elf
OS_IMG = $(BUILDDIR)/$(OS_NAME).img

SRC = $(call rwildcard,$(SRCDIR),*.cpp)
ASMSRC = $(call rwildcard,$(SRCDIR),*.asm)
PSFSRC = $(call rwildcard,$(SRCDIR),*.psf)
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))
OBJS += $(patsubst $(SRCDIR)/%.asm, $(OBJDIR)/%_asm.o, $(ASMSRC))
OBJS += $(patsubst $(SRCDIR)/%.psf, $(OBJDIR)/%_font.o, $(PSFSRC))
DIRS = $(wildcard $(SRCDIR)/*)

MKBOOTIMG = $(UTILSDIR)/mkbootimg
BOOTJSON = $(UTILSDIR)/mkbootimg.json

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
	$(CC) -mno-red-zone -mgeneral-regs-only -ffreestanding -c $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@echo !==== COMPILING $^
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $^ -o $@

$(OBJDIR)/%_asm.o: $(SRCDIR)/%.asm
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
	qemu-system-x86_64 -machine q35 -cpu qemu64 -bios $(OVMF) -m 64 -drive file=$(OS_IMG),format=raw -serial file:serial.log

clean:
	rm -rf $(OBJDIR)/*