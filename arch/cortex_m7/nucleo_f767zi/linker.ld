ENTRY(Reset_Handler)

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 2048K
    RAM   (xrw) : ORIGIN = 0x20000000, LENGTH = 512K
}

/* Calculate top of SRAM for Stack Pointer */
_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
    /* Vector Table placed at the start of Flash */
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } > FLASH

    /* Program Code and Read-Only Constants */
    .text :
    {
        . = ALIGN(4);
        *(.text)
        *(.text*)
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
    } > FLASH

    /* Symbol used by Reset_Handler to locate initial values in Flash */
    _sidata = LOADADDR(.data);

    /* Initialized Globals: Run in RAM, loaded from FLASH */
    .data :
    {
        . = ALIGN(4);
        _sdata = .;        /* Symbol: Start of .data in RAM */
        *(.data)
        *(.data*)
        . = ALIGN(4);
        _edata = .;        /* Symbol: End of .data in RAM */
    } > RAM AT> FLASH

    /* Uninitialized Globals: Allocated in RAM, cleared to 0 at boot */
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;         /* Symbol: Start of .bss in RAM */
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;         /* Symbol: End of .bss in RAM */
    } > RAM
}
