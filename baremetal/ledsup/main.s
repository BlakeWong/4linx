    .text
    .code 32
    .global _start
    
_start:
    /* configure GPM0 as output mode */
    ldr r0, GPMCON
    ldr r1, =0x11111111
    str r1, [r0]
        
    ldr r0, GPMPUD
    ldr r1, =0x00055555
    str r1, [r0]

    ldr r0, GPMDAT
    ldr r2, =0x0000
    str r2, [r0]
    
    /* keep current state */
loop:
    b loop

GPMCON: .word 0x7F008820
GPMDAT: .word 0x7F008824
GPMPUD: .word 0x7F008828
