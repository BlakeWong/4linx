ENTRY(_start)
SECTIONS {
         . = 0x57e00000;
         /* . = 0xc7e00000; */
         startup : { startup.o(.text) } /* startup code */
         .data : { *(.data) }           /* data section */
         .bss : { *(.bss) }             /* bss section */
         . = . + 0x500;                 /* reserved stack space */
         sp_top = .;                    /* bottom of the stack */
}
