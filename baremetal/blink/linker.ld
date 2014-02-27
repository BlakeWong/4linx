ENTRY(_start)
SECTIONS {
         . = 0x0c000000;                /* startup address */
         startup : { startup.o(.text) } /* startup code */
         .data : { *(.data) }           /* data section */
         .bss : { *(.bss) }             /* bss section */
         . = . + 0x500;                 /* reserved stack space */
         sp_top = .;                    /* bottom of the stack */
}
