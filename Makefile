rwildcard = $(foreach d, $(wildcard $1*), $(filter $(subst *, %, $2), $d) $(call rwildcard, $d/, $2))

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/3ds_rules

CC := arm-none-eabi-gcc
AS := arm-none-eabi-as
LD := arm-none-eabi-ld
OC := arm-none-eabi-objcopy


dir_source := source
dir_arm9 := arm9
dir_arm11_hook := arm11_hook
dir_build := build
dir_out := bin

ASFLAGS := -mcpu=mpcore -mlittle-endian
CFLAGS := -Wall -Wextra -MMD -MP -marm $(ASFLAGS) -fno-builtin -fshort-wchar -std=c11 -Wno-main -Os -g -flto -ffast-math
LDFLAGS := -nostartfiles
OCFLAGS := --set-section-flags .bss=alloc,load,contents

objects = $(patsubst $(dir_source)/%.s, $(dir_build)/%.o, \
          $(patsubst $(dir_source)/%.c, $(dir_build)/%.o, \
          $(call rwildcard, $(dir_source), *.s *.c)))

.PHONY: all
all: bin/arm11.bin

.PHONY: clean
clean:
	@$(MAKE) -C $(dir_arm11_hook) clean
	@$(MAKE) -C $(dir_arm9) clean
	@rm -rf $(dir_out) $(dir_build)

.PRECIOUS: $(dir_build)/%.bin

$(dir_out) $(dir_build):
	@mkdir -p "$@"

$(dir_out)/arm11.bin: $(dir_build)/main.bin $(dir_out)
	@cp -a $(dir_build)/main.bin $@

$(dir_build)/main.bin: $(dir_build)/main.elf
	$(OC) -S -O binary $< $@

$(dir_build)/main.elf: $(bundled) $(objects)
	$(LINK.o) -T linker.ld $(OUTPUT_OPTION) $^

$(dir_build)/arm11_hook.bin: $(dir_arm11_hook) $(dir_build)
	@$(MAKE) -C $<

$(dir_build)/arm9.bin: $(dir_arm9) $(dir_build)/arm11_hook.bin $(dir_build)
	@$(MAKE) -C $<

$(dir_build)/lib.o: CFLAGS += -O3

$(dir_build)/%.o: $(dir_source)/%.c $(dir_build)/arm9.bin
	@mkdir -p "$(@D)"
	$(COMPILE.c) $(OUTPUT_OPTION) $<

$(dir_build)/%.o: $(dir_source)/%.s
	@mkdir -p "$(@D)"
	$(COMPILE.s) $(OUTPUT_OPTION) $<
include $(call rwildcard, $(dir_build), *.d)
