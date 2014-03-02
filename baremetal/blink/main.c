/* 
   GPM control registers
 */
#define GPMCON_REG  (* (volatile unsigned int *) 0x7F008820)
#define GPMDAT_REG  (* (volatile unsigned int *) 0x7F008824)
#define GPMPUD_REG  (* (volatile unsigned int *) 0x7F008828)


/* 
   Waste some CPU cycles.
*/
void sleep(int msecs)
{
        volatile int i = 0, j = 0;
        for ( ; i < msecs; i++) {
                for (j = 0; j++ < 0x1ff; );
        }
}

int main(int argc, char *argv[])
{
        int i = 0;
        unsigned int reg[] = {
                0x0001, 0x0002, 0x0004, 0x0008,
                0x0008, 0x0004, 0x0002, 0x0001
        };
        
        GPMCON_REG = 0x11111111;
        GPMPUD_REG = 0x00055555;
        GPMDAT_REG = 0x0000;
        
        for ( ; ; ) {
                sleep(200);
                GPMDAT_REG = reg[i];
                i = (i + 1) % (sizeof(reg)/sizeof(reg[0]));
        }
        
        return 0;
}
