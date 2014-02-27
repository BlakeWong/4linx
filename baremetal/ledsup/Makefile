CROSS_CC = arm-linux-gnueabi-gcc
CROSS_AS = arm-linux-gnueabi-as
CROSS_LD = arm-linux-gnueabi-ld
CROSS_OC = arm-linux-gnueabi-objcopy
# MCPU = -mcpu=arm926ej-s

bl1.bin: bl1.elf
	$(CROSS_OC) -O binary $^ $@

bl1.elf: main.o 
	$(CROSS_LD) -e _start -Ttext 0x0c000000 $^ -o $@

main.o: main.s
	$(CROSS_AS) $< -o $@

.PHONY: clean
clean:
	rm -f main.o bl1.elf bl1.out
