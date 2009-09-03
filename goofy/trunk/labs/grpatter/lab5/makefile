ARM_PATH = /l/arm/gnuarm/
ARM_EXEC_PATH = $(ARM_PATH)/bin
CC = $(ARM_EXEC_PATH)/arm-elf-gcc
CFLAGS = -g -Wall -DGOOFY -mcpu=arm7tdmi -O0
LFLAGS = -nostdlib -nostartfiles \
		-T/l/arm/gnuarm/arm-elf/lib/ldscripts/2138_rom_gdb.ld \
		-lggbsp

OBJECTCOPY = $(ARM_EXEC_PATH)/arm-elf-objcopy

OBJECTS = blink.o
OBJECTS2 = indicator.o
TARGET = blink.elf
TARGET2 = indicator.elf

all : $(TARGET) $(TARGET2)

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) $(LFLAGS) $(OBJECTS) -o $(TARGET)
$(TARGET2) : $(OBJECTS2)
	$(CC) $(CFLAGS) $(LFLAGS) $(OBJECTS2) -o $(TARGET2)
$(OBJECTS) : blink.c
	$(CC) -c $(CFLAGS) blink.c 
$(OBJECTS2) : indicator.c
	$(CC) -c $(CFLAGS) indicator.c 
objectcopy : $(TARGET)
	$(OBJECTCOPY) -O binary -R .eeprom $(TARGET) blink.hex
objectcopy2 : $(TARGET2)
	$(OBJECTCOPY) -O binary -R .eeprom $(TARGET2) indicator.hex
downloadblink : blink.hex
	arm-elf-download blink.hex
downloadind : indicator.hex
	arm-elf-download indicator.hex
CLEAN:
	rm blink.o
	rm blink.elf
	rm blink.hex
	rm indicator.o
	rm indicator.elf
	rm indicator.hex
