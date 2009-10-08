ARM_PATH = /l/arm/gnuarm/
ARM_EXEC_PATH = $(ARM_PATH)/bin
CC = $(ARM_EXEC_PATH)/arm-elf-gcc
WAV = $(ARM_EXEC_PATH)/wav_sound_processor
CFLAGS = -g -Wall -DGOOFY -mcpu=arm7tdmi -O0
LFLAGS = -nostdlib -nostartfiles \
		-T/l/arm/gnuarm/arm-elf/lib/ldscripts/2138_rom_gdb.ld \
		-lggbsp

OBJECTCOPY = $(ARM_EXEC_PATH)/arm-elf-objcopy

OBJECTS = lab08.o
TARGET = lab08.elf

all : $(TARGET) 

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) $(LFLAGS) $(OBJECTS) -o $(TARGET)
$(OBJECTS) : lab08.c
	$(CC) -c $(CFLAGS) lab08.c 
objectcopy : $(TARGET)
	$(OBJECTCOPY) -O binary -R .eeprom $(TARGET) lab08.hex
downloadlab08 : lab08.hex
	arm-elf-download lab08.hex
CLEAN:
	rm lab08.o
	rm lab08.elf
	rm lab08.hex
