#include <config.h>
#include <asm/regs.h>
#include <config/smdk6410.h>
#include <asm/s3c64x0.h>
#include <serial.h>
#include <syslog.h>


/* 
   Waste some CPU cycles.
*/
void sleep(int msecs)
{
        volatile int i = 0, j = 0;
        for ( ; i < msecs; i++) {
                for (j = 0; j++ < 0x7fff; );
        }
}

int main(void)
{
        int i = 0;
        unsigned int reg[] = {
                0x0001, 0x0002, 0x0004, 0x0008,
                0x0008, 0x0004, 0x0002, 0x0001
        };

        GPMCON_REG = 0x11111111;
        GPMPUD_REG = 0x00055555;
        GPMDAT_REG = 0x0000;

        serial_puts(" \n");
        serial_puts("MMC Loader has launched\n");
        sleep(500);
        serial_puts("Modified from U-Boot\n");
        sleep(500);
        serial_puts("By Kristin Kowalcijk\n");
        serial_puts("Email: morisama@yeah.net\n\n");
        sleep(500);

        /* k_printf("I got a number: %d\n", 13); */
        /* k_printf("I got 2 numbers: %d, %d\n", 7, 13); */
        /* k_printf("I got three numbers: %x, %02X, %02x\n", 7, 13, 42); */

        /* k_printf("[%s]: %s\n\n", "sysboot", "bl1 loaded, now loading bl2..."); */


#define	TCM_BASE		 0x0C004000
        k_printf("TCM info: \n");
        k_printf("Total Block #: %d\n", *((volatile unsigned int *)0x0C003FFC));
        k_printf("RCA info: 0x%08X\n", *((volatile unsigned int *)0x0C003FF8));
        k_printf("MMC controller addr: 0x%08X\n", *((volatile unsigned int *)0x0C003FEC));
        
        for ( ; ; ) {
                GPMDAT_REG = ~reg[i];
                sleep(100);
                i = (i + 1) % (sizeof(reg)/sizeof(reg[0]));
        }

        while (1);
        return 0;
}
