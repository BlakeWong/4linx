#define _LARGEFILE64_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>


#define SDHC_MIN_SIZE      (2LL * 1024LL * 1024LL * 1024LL)
#define SDMMC_BL1_OFFSET   (-(512*2 + 8*1024))
#define SDHC_BL1_OFFSET    (-(1025*512 + 512 + 1024*8))

enum sdtype {
        SDTYPE_SDMMC,
        SDTYPE_SDHC,
        SDTYPE_ERR
};


/* 
   Dump error message to stderr in the following format:
   format%(...): strerror
 */
void errmsg(const char *format, ...)
{
        va_list args;
        va_start(args, format);
        const char *msg = strerror(errno);
        vfprintf(stderr, format, args);
        fprintf(stderr, ": %s\n", msg);
        va_end(args);
}


/* 
   Test if specified MMC device is SDHC or SDMMC
 */
enum sdtype test_sd_type(const char *mmcdev)
{
        int mmc_fd = open(mmcdev, O_RDONLY);
        if (mmc_fd != -1) {
                off64_t mmc_size = lseek64(mmc_fd, 0, SEEK_END);
                if (mmc_size != -1) {
                        return (mmc_size > SDHC_MIN_SIZE)?
                                SDTYPE_SDHC: SDTYPE_SDMMC;
                }
                else {
                        errmsg("Cannot seek this MMC device");
                        close(mmc_fd);
                        return SDTYPE_ERR;
                }
        }
        else {
                errmsg("Cannot open device file %s", mmcdev);
                return SDTYPE_ERR;
        }
}


/* 
   Copy loader to mmcdev, start from SEEK_END + bl_offset.
 */
int dd_loader_to_steppingstone(const char *mmcdev, const char *loader, int bl_offset)
{
        static unsigned char buf[512];
        off64_t mmc_ldaddr;
        int mmc_fd, bl_fd, ret;

        /* open mmc device and loader image */
        mmc_fd = open(mmcdev, O_RDWR);
        if (mmc_fd < 0) {
                errmsg("Cannot open mmc device file %s", mmcdev); 
                return -1;
        }

        bl_fd = open(loader, O_RDONLY);
        if (bl_fd < 0) {
                errmsg("Cannot open loader image file %s", loader);
                close(mmc_fd);
                return -1;
        }

        /* file opened properly, before copying the loader image to the mmc, we need
         * to seek for a proper address to start the copying process */
        if ((mmc_ldaddr = lseek64(mmc_fd, bl_offset, SEEK_END)) == -1) {
                errmsg("Cannot seek to the load address of mmc %s", mmcdev);
                goto quit;
        }
        else {
                printf("mmc load address: %lld\n", mmc_ldaddr);
        }
        
        /* copy loader image to the mmc */
        while ((ret = read(bl_fd, buf, sizeof(buf))) > 0) {
                if (write(mmc_fd, buf, ret) < 0) {
                        errmsg("Error encountered when writing to mmc %s", mmcdev);
                        goto quit;
                }
        }

        if (ret == 0) {
                /* successfully copied loader image to the mmc */
                printf("%s sucessfully copied to the mmc\n", loader);
                close(mmc_fd);
                close(bl_fd);
                sync();
                return 0;
        }
        else {
                /* we encountered some error */
                errmsg("Error encountered when reading the loader image file %s", loader);
                goto quit;
        }

quit:
        close(mmc_fd);
        close(bl_fd);
        return -1;
}


int dd_loader(const char *mmcdev, const char *bl1_loader, const char *bl2_loader)
{
        enum sdtype type = test_sd_type(mmcdev);
        off64_t bl1_offset;
        
        if (type == SDTYPE_ERR) {
                fprintf(stderr, "Cannot recognize MMC type\n");
                return -1;
        }
        else {
                if (type == SDTYPE_SDHC) {
                        printf("%s is an SDHC device\n", mmcdev);
                        bl1_offset = SDHC_BL1_OFFSET;                
                }
                else {
                        printf("%s is an SDMMC device\n", mmcdev);
                        bl1_offset = SDMMC_BL1_OFFSET;
                }
                
                if (dd_loader_to_steppingstone(mmcdev, bl1_loader, bl1_offset) != 0) {
                        return -1;
                }
                if (bl2_loader != NULL) {
                        return dd_loader_to_steppingstone(mmcdev, bl2_loader, bl1_offset - 256 * 1024);
                }
                else {
                        return 0;
                }
        }
}
        

int main(int argc, char *argv[])
{
        const char *progname = argv[0];
        if (argc >= 3) {
                const char *mmcdev = argv[1];
                const char *loader = argv[2];
                const char *loader2 = argc == 4? argv[3]: NULL;
                if (dd_loader(mmcdev, loader, loader2) != 0) {
                        fprintf(stderr, "Writing loader image \"%s\" failed\n", loader);
                }
        }
        else {
                printf("usage: %s [mmcdev] [loader1img] <loader2img>\n"
                        "  example: %s /dev/sdb ./mmc/loader.bin\n"
                        "           %s /dev/sdb ./mmc/loader.bin ./mmc/loader2.bin\n", 
                        progname, progname, progname);
        }
        return 0;
}
