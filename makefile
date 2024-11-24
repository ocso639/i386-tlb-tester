TARGET = bootable.img 
CC = gcc

all: $(TARGET)

$(TARGET):
	$(CC) -fno-PIC -Wall -m32 -ffreestanding -fno-stack-protector -fno-stack-check -D__i386__ -c ./tlbtest/*.c 
	$(CC) -fno-PIC -Wall -m32 -c ./tlbtest/*.S
	$(LD) -nostdlib -melf_i386 --script=linker.ld *.o -o bootable_c
	objcopy -O binary bootable_c stage2.bin
	rm *.o
	$(MAKE) -C ./bootsect
	mv ./bootsect/bootsect.bin ./
	
	dd if=/dev/urandom of=bootable.img bs=1 count=1410000
	dd if=bootsect.bin of=bootable.img bs=512 seek=0 conv=notrunc
	dd if=stage2.bin of=bootable.img bs=512 seek=1 conv=notrunc
	mv bootable.img ./bootable