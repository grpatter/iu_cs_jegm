ARM_PATH = /l/arm/gnuarm/
ARM_EXEC_PATH = $(ARM_PATH)/bin
CC = $(ARM_EXEC_PATH)/arm-elf-gcc
WAV = $(ARM_EXEC_PATH)/wav_sound_processor
CFLAGS = -g -Wall -DGOOFY -mcpu=arm7tdmi -O0
LFLAGS = -nostdlib -nostartfiles \
		-T/l/arm/gnuarm/arm-elf/lib/ldscripts/2138_rom_gdb.ld \
		-lggbsp

OBJECTCOPY = $(ARM_EXEC_PATH)/arm-elf-objcopy

OBJECTS = motortest.o
OBJECTS2 = soundtest.o
TARGET = motortest.elf
TARGET2 = soundtest.elf

all : $(TARGET) $(TARGET2) process_wav

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) $(LFLAGS) $(OBJECTS) -o $(TARGET)
$(TARGET2) : $(OBJECTS2)
	$(CC) $(CFLAGS) $(LFLAGS) $(OBJECTS2) -o $(TARGET2)
$(OBJECTS) : motortest.c
	$(CC) -c $(CFLAGS) motortest.c 
$(OBJECTS2) : soundtest.c
	$(CC) -c $(CFLAGS) soundtest.c 
objectcopy : $(TARGET)
	$(OBJECTCOPY) -O binary -R .eeprom $(TARGET) motortest.hex
objectcopy2 : $(TARGET2)
	$(OBJECTCOPY) -O binary -R .eeprom $(TARGET2) soundtest.hex
downloadmotor : motortest.hex
	arm-elf-download motortest.hex
downloadsound : soundtest.hex
	arm-elf-download soundtest.hex
process_wav : my_wav.wav
	$(WAV) my_wav.wav
CLEAN:
	rm motortest.o
	rm motortest.elf
	rm motortest.hex
	rm soundtest.o
	rm soundtest.elf
	rm soundtest.hex
