# Copyright 2019 Clifford Wolf
# Copyright 2019 Robert Balas
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
# REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
# AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
# INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
# LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
# OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
# PERFORMANCE OF THIS SOFTWARE.

# Author: Robert Balas (balasr@iis.ee.ethz.ch)

MAKE                       = make

# riscv toolchain install path
RISCV                      ?= ~/.riscv
RISCV_EXE_PREFIX           = $(RISCV)/bin/riscv32-unknown-elf-

TARGET                     ?= sim

# GCC configuration
CUSTOM_GCC_FLAGS           =

LIB_CRT                    = $(wildcard x_heep_device/lib/crt/*.S)
LIB_CRT_FLASH_EXEC         = $(wildcard x_heep_device/lib/crt_flash_exec/*.S)
LIB_CRT_FLASH_LOAD         = $(wildcard x_heep_device/lib/crt_flash_load/*.S)
LIB_BASE                   = $(wildcard x_heep_device/lib/base/*.c)
LIB_BASE_FREESTD           = x_heep_device/lib/base/freestanding
LIB_RUNTIME                = $(wildcard x_heep_device/lib/runtime/*.c)
LIB_DRIVERS                = $(wildcard x_heep_device/lib/drivers/soc_ctrl/*.c)
LIB_DRIVERS                += $(wildcard x_heep_device/lib/drivers/uart/*.c)
LIB_DRIVERS                += $(wildcard x_heep_device/lib/drivers/rv_plic/*.c)
LIB_DRIVERS                += $(wildcard x_heep_device/lib/drivers/rv_timer/*.c)
LIB_DRIVERS                += $(wildcard x_heep_device/lib/drivers/gpio/*.c)
LIB_DRIVERS                += $(wildcard x_heep_device/lib/drivers/i2c/*.c)
LIB_DRIVERS                += $(wildcard x_heep_device/lib/drivers/dma/*.c)
LIB_DRIVERS                += $(wildcard x_heep_device/lib/drivers/power_manager/*.c)
LIB_DRIVERS                += $(wildcard x_heep_device/lib/drivers/fast_intr_ctrl/*.c)
LIB_DRIVERS                += $(wildcard x_heep_device/lib/drivers/spi_mem/*.c)
LIB_DRIVERS                += $(wildcard x_heep_device/lib/drivers/spi_host/*.c)
LIB_DRIVERS                += $(wildcard x_heep_device/lib/drivers/pad_control/*.c)
LIB_DRIVERS                += $(wildcard device/lib/drivers/cgra/*.c)
# LIB_DRIVERS                += $(wildcard device/lib/drivers/heepocrates_ctrl/*.c)
INC_FOLDERS                = $(sort $(dir $(wildcard x_heep_device/target/$(TARGET)/)))
INC_FOLDERS                += $(sort $(dir $(wildcard x_heep_device/lib/drivers/*/)))
INC_FOLDERS                += $(sort $(dir $(wildcard x_heep_device/lib/base/)))
INC_FOLDERS                += $(sort $(dir $(wildcard x_heep_device/lib/runtime/)))
INC_FOLDERS                += $(sort $(dir $(wildcard device/lib/crt/)))
INC_FOLDERS                += $(sort $(dir $(wildcard device/lib/drivers/*/)))
INC_FOLDERS                += $(sort $(dir $(wildcard device/lib/runtime/)))
INC_FOLDERS_GCC             = $(addprefix -I ,$(INC_FOLDERS))

# Rules to generate hex (loadable by simulators) from elf
%.hex: %.elf
	$(RISCV_EXE_PREFIX)objcopy -O verilog $< $@

%.flash_exec.hex: %.flash_exec.elf
	$(RISCV_EXE_PREFIX)objcopy -O verilog --adjust-vma=-0x40000000 $< $@

%.flash_load.hex: %.flash_load.elf
	$(RISCV_EXE_PREFIX)objcopy -O verilog --adjust-vma=-0x40000000 $< $@

%.dis: %.elf
	$(RISCV_EXE_PREFIX)objdump -S $^ > $@

%.dump: %.elf
	$(RISCV_EXE_PREFIX)objdump -xD $^ > $@


# Running custom programs:
# This is an example for running a Hello World in the testbench
# We link with our custom crt0.s and syscalls.c against newlib so that we can
# use the c standard library
x_heep_applications/hello_world/hello_world.elf: x_heep_applications/hello_world/hello_world.c
	$(RISCV_EXE_PREFIX)gcc -march=rv32imc -o $@ -w -Os -g -nostdlib \
		$(CUSTOM_GCC_FLAGS) \
		-DHOST_BUILD \
		-T x_heep_linker/link.ld \
		-I $(RISCV)/riscv32-unknown-elf/include \
		$(INC_FOLDERS_GCC) \
		-static \
		$(LIB_CRT) \
		$^ $(LIB_RUNTIME) \
		$(LIB_BASE) \
		$(LIB_DRIVERS) \
		-L $(RISCV)/riscv32-unknown-elf/lib \
		-lc -lm -lgcc -flto -ffunction-sections -fdata-sections -specs=nano.specs

x_heep_applications/hello_world/hello_world.flash_exec.elf: x_heep_applications/hello_world/hello_world.c
	$(RISCV_EXE_PREFIX)gcc -march=rv32imc -o $@ -w -Os -g -nostdlib \
		$(CUSTOM_GCC_FLAGS) \
		-DHOST_BUILD \
		-T x_heep_linker/link_flash_exec.ld \
		-I $(RISCV)/riscv32-unknown-elf/include \
		$(INC_FOLDERS_GCC) \
		-static \
		$(LIB_CRT_FLASH_EXEC) \
		$^ $(LIB_RUNTIME) \
		$(LIB_BASE) \
		$(LIB_DRIVERS) \
		-Wl,-Map=x_heep_applications/hello_world/hello_world.map \
		-L $(RISCV)/riscv32-unknown-elf/lib \
		-lc -lm -lgcc -flto -ffunction-sections -fdata-sections -specs=nano.specs

applications/cgra_func_test/main.elf: applications/cgra_func_test/main.c applications/cgra_func_test/fxp.c
	$(RISCV_EXE_PREFIX)gcc -march=rv32imc -o $@ -w -Os -g -nostdlib \
		$(CUSTOM_GCC_FLAGS) \
		-DHOST_BUILD \
		-T x_heep_linker/link.ld \
		-I $(RISCV)/riscv32-unknown-elf/include \
		$(INC_FOLDERS_GCC) \
		-static \
		$(LIB_CRT) \
		$^ $(LIB_RUNTIME) \
		$(LIB_BASE) \
		$(LIB_DRIVERS) \
		-L $(RISCV)/riscv32-unknown-elf/lib \
		-lc -lm -lgcc -flto -ffunction-sections -fdata-sections -specs=nano.specs

applications/cgra_dbl_search/main.elf: applications/cgra_dbl_search/main.c
	$(RISCV_EXE_PREFIX)gcc -march=rv32imc -o $@ -w -Os -g -nostdlib \
		$(CUSTOM_GCC_FLAGS) \
		-DHOST_BUILD \
		-T x_heep_linker/link.ld \
		-I $(RISCV)/riscv32-unknown-elf/include \
		$(INC_FOLDERS_GCC) \
		-static \
		$(LIB_CRT) \
		$^ $(LIB_RUNTIME) \
		$(LIB_BASE) \
		$(LIB_DRIVERS) \
		-L $(RISCV)/riscv32-unknown-elf/lib \
		-lc -lm -lgcc -flto -ffunction-sections -fdata-sections -specs=nano.specs

clean:
	rm -rf applications/*/*.elf \
	rm -rf applications/*/*.hex \
	rm -rf applications/*/*.dis \
	rm -rf applications/*/*.map \
	rm -rf applications/*/*.dump \
	rm -rf x_heep_applications/*/*.elf \
	rm -rf x_heep_applications/*/*.hex \
	rm -rf x_heep_applications/*/*.dis \
	rm -rf x_heep_applications/*/*.map \
	rm -rf x_heep_applications/*/*.dump
