ARM_PATH = /l/arm/gnuarm/
ARM_EXEC_PATH = $(ARM_PATH)/bin
CC = $(ARM_EXEC_PATH)/arm-elf-gcc
WAV = $(ARM_EXEC_PATH)/wav_sound_processor
CFLAGS = -g -Wall -DGOOFY -mcpu=arm7tdmi -O0
LFLAGS = -nostdlib -nostartfiles \
		-T/l/arm/gnuarm/arm-elf/lib/ldscripts/2138_rom_gdb.ld \
		-lggbsp

OBJECTCOPY = $(ARM_EXEC_PATH)/arm-elf-objcopy

OBJECTS = lab10.o
TARGET = lab10.elf

all : process_wav $(TARGET) 

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) $(LFLAGS) $(OBJECTS) -o $(TARGET)
$(OBJECTS) : lab10.c
	$(CC) -c $(CFLAGS) lab10.c 
ocopy : $(TARGET)
	$(OBJECTCOPY) -O binary -R .eeprom $(TARGET) lab10.hex
downloadlab10 : lab10.hex
	$(ARM_EXEC_PATH)/arm-elf-download lab10.hex
process_wav : my_wav.wav goofy_dance.wav giggle.wav horn.wav
	$(WAV) my_wav.wav goofy_dance.wav giggle.wav horn.wav
CLEAN:
	rm lab10.o
	rm lab10.elf
	rm lab10.hex
