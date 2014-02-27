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


#define SDMMC_BL1_OFFSET   (-(512*2 + 8*1024))
#define SDHC_BL1_OFFSET    (-(1025*512 + 512 + 1024*8))


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
                printf("Loader sucessfully copied to the mmc, congratulations!\n");
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


int main(int argc, char *argv[])
{
        const char *progname = argv[0];
        if (argc == 3) {
                const char *mmcdev = argv[1];
                const char *loader = argv[2];
                if (dd_loader_to_steppingstone(mmcdev, loader, SDHC_BL1_OFFSET) != 0) {
                        fprintf(stderr, "Writing loader image \"%s\" failed\n", loader);
                }
        }
        else {
                printf("usage: %s [mmcdev] [loaderimg]\n"
                        "  example: %s /dev/sdb ./mmc/loader.bin\n", 
                        progname, progname);
        }
        return 0;
}
