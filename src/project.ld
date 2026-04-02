OUTPUT_ARCH(mips)
ENTRY(_start)

MEMORY
{
    SRAM (rwx) : ORIGIN = 0xB8000000, LENGTH = 0x0001FFFF
}

__stack_size = 0x4000;

SECTIONS
{
    /* Вектор/обработчик прерывания */
    .text_interrupt 0xB8000180 : ALIGN(4)
    {
        KEEP(*(.text_interrupt))
        KEEP(*(.text_interrupt.*))
    } > SRAM

    /* Основной код */
    .text 0xB8001000 : ALIGN(4)
    {
        KEEP(*(.text.startup))
        KEEP(*(.text.startup.*))

        *(.text)
        *(.text.*)
        *(.rodata)
        *(.rodata.*)
    } > SRAM

    /* Данные */
    .data : ALIGN(4)
    {
        __data_start = .;
        *(.data)
        *(.data.*)
        __data_end = .;
    } > SRAM

    /* Small data, если вдруг появится */
    .sdata : ALIGN(4)
    {
        __sdata_start = .;
        *(.sdata)
        *(.sdata.*)
        __sdata_end = .;
    } > SRAM

    /*
     * GP для MIPS.
     * При -G0 small data фактически не используется,
     * но символ _gp лучше всё равно определить.
     */
    _gp = ALIGN(16);

    .lit4 : ALIGN(4)
    {
        *(.lit4)
    } > SRAM

    .lit8 : ALIGN(8)
    {
        *(.lit8)
    } > SRAM

    /* Small BSS */
    .sbss (NOLOAD) : ALIGN(4)
    {
        __sbss_start = .;
        *(.sbss)
        *(.sbss.*)
        *(.scommon)
        __sbss_end = .;
    } > SRAM

    /* Основной BSS */
    .bss (NOLOAD) : ALIGN(4)
    {
        __bss_start = .;
        *(.bss)
        *(.bss.*)
        *(COMMON)
        __bss_end = .;
    } > SRAM

    /* Стек */
    .stack (NOLOAD) : ALIGN(8)
    {
        __stack_start = .;
        . = . + __stack_size;
        __stack_top = .;
    } > SRAM

    /* Конец образа в RAM */
    __end = .;
    _end = .;

    /DISCARD/ :
    {
        *(.comment)
        *(.note)
        *(.note.*)
        *(.MIPS.abiflags)
        *(.pdr)
        *(.reginfo)
        *(.gnu.attributes)
    }
}

/* Явные символы для startup */
PROVIDE(__stack = __stack_top);
PROVIDE(__heap_start = _end);

/* Для совместимости */
PROVIDE(edata = __data_end);
PROVIDE(_edata = __data_end);
PROVIDE(end = _end);
