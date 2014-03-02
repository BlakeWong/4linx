#include <config.h>
#include <asm/regs.h>
#include <movi.h>


int movi_bl2_copy(void)
{
        volatile unsigned int *hm_control4 = (volatile unsigned int *)(ELFIN_HSMMC_BASE + HM_CONTROL4);
        *hm_control4 = *hm_control4 | (0x03 << 16);
        return CopyMovitoMem(0, MOVI_BL2_POS, MOVI_BL2_BLKCNT, (uint *)BL2_BASE, 0);
}
