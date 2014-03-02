ENTRY(_start)
SECTIONS {
         . = 0x57e00000;
         startup : { startup.o(.text) } /* startup code */
         .data : { *(.data) }           /* data section */
         .bss : { *(.bss) }             /* bss section */
}
