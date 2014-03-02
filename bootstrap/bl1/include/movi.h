#ifndef _MOVI_H_
#define _MOVI_H_


#include <types.h>


#define	TCM_BASE		 0x0C004000
#define BL2_BASE         0x57E00000

#define SS_SIZE			(8 * 1024)
#define eFUSE_SIZE		(1 * 1024)	// 0.5k eFuse, 0.5k reserved

/* movinand definitions */
#define MOVI_BLKSIZE		512

#define MOVI_TOTAL_BLKCNT	*((volatile unsigned int*)(TCM_BASE - 0x4))


/* partition information */
#define PART_SIZE_BL		(256 * 1024)
#define PART_SIZE_KERNEL	(4 * 1024 * 1024)

#define MOVI_LAST_BLKPOS	(MOVI_TOTAL_BLKCNT - (eFUSE_SIZE / MOVI_BLKSIZE))
#define MOVI_BL1_BLKCNT		(SS_SIZE / MOVI_BLKSIZE)
#define MOVI_BL2_BLKCNT		(PART_SIZE_BL / MOVI_BLKSIZE)
#define MOVI_BL2_POS		(MOVI_LAST_BLKPOS - MOVI_BL1_BLKCNT - MOVI_BL2_BLKCNT)


#define CopyMovitoMem(a,b,c,d,e)                                        \
        (((int(*)(int, uint, ushort, uint *, int))                      \
          (*((uint *)(TCM_BASE + 0x8))))(a,b,c,d,e))

int movi_bl2_copy(void);



#endif /* _MOVI_H_ */
