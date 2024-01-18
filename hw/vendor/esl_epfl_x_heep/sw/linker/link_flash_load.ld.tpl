/* Copyright EPFL contributors.
 * Licensed under the Apache License, Version 2.0, see LICENSE for details.
 * SPDX-License-Identifier: Apache-2.0
 */

ENTRY(_start)

MEMORY
{
    FLASH (rx)      : ORIGIN = 0x${flash_mem_start_address}, LENGTH = 0x${flash_mem_size_address}
    RAM (xrwai)       : ORIGIN = 0x${'{:08X}'.format(int(ram_start_address,16))}, LENGTH =  ${hex(int(linker_onchip_data_size_address,16)+int(linker_onchip_code_size_address,16))}
    % if ram_numbanks_cont > 1 and ram_numbanks_il > 0:
    ram_il (rwxai) : ORIGIN = 0x${linker_onchip_il_start_address}, LENGTH = 0x${linker_onchip_il_size_address}
    % endif
}

SECTIONS {

    /* we want a fixed entry point */
    PROVIDE(__boot_address = 0x180);

    /* stack and heap related settings */
    __stack_size = DEFINED(__stack_size) ? __stack_size : 0x${stack_size};
    PROVIDE(__stack_size = __stack_size);
    __heap_size = DEFINED(__heap_size) ? __heap_size : 0x${heap_size};

    /* interrupt vectors */
    .vectors (ORIGIN(RAM)):
    {
        PROVIDE(__vector_start = .);
        KEEP(*(.vectors));
        __VECTORS_AT = .;
    } >RAM AT >FLASH

    /* Fill memory up to __boot_address */
    .fill :
    {
        FILL(0xDEADBEEF);
        . = ORIGIN(RAM) + (__boot_address) - 1;
        BYTE(0xEE)
    } >RAM AT >FLASH

    /* crt0 init code */
    .init (__boot_address):
    {
        KEEP (*(SORT_NONE(.init)))
        KEEP (*(.text.start))
    } >RAM AT >FLASH

    /* The program code and other data goes into FLASH */
    .text : ALIGN_WITH_INPUT
    {
        . = ALIGN(4);
        *(.text)           /* .text sections (code) */
        *(.text*)          /* .text* sections (code) */
        *(.rodata)         /* .rodata sections (constants, strings, etc.) */
        *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
        *(.srodata)        /* .rodata sections (constants, strings, etc.) */
        *(.srodata*)       /* .rodata* sections (constants, strings, etc.) */
        . = ALIGN(4);
        _etext = .;        /* define a global symbol at end of code */
    } >RAM AT >FLASH

    /* This is the initialized data section
    The program executes knowing that the data is in the RAM
    but the loader puts the initial values in the FLASH (inidata).
    It is one task of the startup to copy the initial values from FLASH to RAM. */
    .data : ALIGN_WITH_INPUT
    {
        . = ALIGN(4);
        _sidata = LOADADDR(.data);
        _sdata = .;        /* create a global symbol at data start; used by startup code in order to initialise the .data section in RAM */
        _ram_start = .;    /* create a global symbol at ram start for garbage collector */
        . = ALIGN(4);
        __DATA_BEGIN__ = .;
        *(.data)           /* .data sections */
        *(.data*)          /* .data* sections */
        __SDATA_BEGIN__ = .;
        *(.sdata)           /* .sdata sections */
        *(.sdata*)          /* .sdata* sections */
        . = ALIGN(4);
        _edata = .;        /* define a global symbol at data end; used by startup code in order to initialise the .data section in RAM */
    } >RAM AT >FLASH

    .power_manager : ALIGN(4096)
    {
       PROVIDE(__power_manager_start = .);
       . += 256;
    } >RAM

    /* Uninitialized data section */
    .bss :
    {
        . = ALIGN(4);
        __bss_start = .;         /* define a global symbol at bss start; used by startup code */
        *(.bss)
        *(.bss*)
        *(.sbss)
        *(.sbss*)
        *(COMMON)

        . = ALIGN(4);
        __bss_end = .;         /* define a global symbol at bss end; used by startup code */
        __BSS_END__ = .;
    } >RAM

    /* The compiler uses this to access data in the .sdata, .data, .sbss and .bss
     sections with fewer instructions (relaxation). This reduces code size. */
    __global_pointer$ = MIN(__SDATA_BEGIN__ + 0x800,
			    MAX(__DATA_BEGIN__ + 0x800, __BSS_END__ - 0x800));

    /* this is to define the start of the heap, and make sure we have a minimum size */
    .heap          :
    {
        PROVIDE(__heap_start = .);
        . = __heap_size;
        PROVIDE(__heap_end = .);
    } >RAM

    /* stack: we should consider putting this further to the top of the address
    space */
    .stack         : ALIGN(16) /* this is a requirement of the ABI(?) */
    {
       PROVIDE(__stack_start = .);
       . = __stack_size;
       PROVIDE(_sp = .);
       PROVIDE(__stack_end = .);
       PROVIDE(__freertos_irq_stack_top = .);
    } >RAM

    % if ram_numbanks_cont > 1 and ram_numbanks_il > 0:
    .data_interleaved :
    {
        . = ALIGN(4);
        *(.xheep_data_interleaved)
        . = ALIGN(4);
    } >ram_il AT >FLASH
    % endif

}
