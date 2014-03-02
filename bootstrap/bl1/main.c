#include <config.h>
#include <asm/regs.h>
#include <config/smdk6410.h>
#include <asm/s3c64x0.h>
#include <serial.h>
#include <syslog.h>
#include <movi.h>


#define readreg(x) (*(volatile unsigned int *)(x))
#define readreg16(x) (*(volatile unsigned short *)(x))

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

void dump_bl2(void);

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
        serial_puts("Modified from U-Boot\n");
        serial_puts("By Kristin Kowalcijk\n");
        serial_puts("Email: morisama@yeah.net\n\n");

        k_printf("TCM info: \n");
        k_printf("Total Block #: %d\n", readreg(0x0C003FFC));
        k_printf("RCA info: 0x%08X\n", readreg(0x0C003FF8));
        k_printf("MMC controller addr: 0x%08X\n", readreg(0x0C003FEC));

        k_printf("\nSDMMC info:\n");
        k_printf("BLKSIZE0: %d\n", readreg16(0x7C200004));
        k_printf("BLKSIZE1: %d\n", readreg16(0x7C300004));
        k_printf("BLKSIZE2: %d\n", readreg16(0x7C400004));
        k_printf("BLKCNT0: %d\n", readreg16(0x7C200006));
        k_printf("BLKCNT1: %d\n", readreg16(0x7C300006));
        k_printf("BLKCNT2: %d\n", readreg16(0x7C400006));
        
        k_printf("\nBL1 runtime stack (roughly): 0x%08x\n", &i);
        k_printf("MOVI info:\n");
        k_printf("BL2_BASE: 0x%08x\n", BL2_BASE);
        k_printf("MOVI_BL2_POS: %d, MOVI_BL2_BLKCNT: %d\n", MOVI_BL2_POS, MOVI_BL2_BLKCNT);

        k_printf("Loading BL2 to DRAM... ");
        int ret = movi_bl2_copy();
        if (ret) {
                k_printf("done\n");
                dump_bl2();
                k_printf("Jumping to BL2 entry...\n");
                ((int(*)(void))BL2_BASE)();
        }
        else {
                k_printf("FAILED\n");
                while (1);
        }

        while (1);
        return 0;
}

void dump_bl2(void)
{
        k_printf("done\n");
        volatile unsigned char *bl2_mem = (volatile unsigned char *)BL2_BASE;
        int i = 0;
        for ( ; i < 512; i++) {
                if (i % 16 == 0) k_printf("\n");
                k_printf("%02x ", bl2_mem[i]);
        }
        k_printf("\n");
}
