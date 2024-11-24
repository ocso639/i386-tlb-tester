TARGET = bootsect.bin
CC = gcc

all: $(TARGET)

$(TARGET):
	$(CC) -c *.S
	$(LD) --oformat binary -o bootsect.bin -Ttext 0x7c00 bootsect.o