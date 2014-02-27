CROSS_CC = arm-linux-gnueabi-gcc
CROSS_AS = arm-linux-gnueabi-as
CROSS_LD = arm-linux-gnueabi-ld
CROSS_OC = arm-linux-gnueabi-objcopy
# MCPU = -mcpu=arm926ej-s


img.bin: img.elf
	$(CROSS_OC) -O binary $^ $@

img.elf: main.o startup.o linker.ld
	$(CROSS_LD) -T linker.ld startup.o main.o -o $@

main.o: main.c
	$(CROSS_CC) -c $< -o $@

startup.o: startup.s
	$(CROSS_AS) $< -o $@


.PHONY: clean
clean:
	rm -f main.o startup.o img.elf img.out
