ENTRY(_start)
SECTIONS {
         . = 0x5fe00000;
         startup : { startup.o(.text) } /* startup code */
         .data : { *(.data) }           /* data section */
         .bss : { *(.bss) }             /* bss section */
}
