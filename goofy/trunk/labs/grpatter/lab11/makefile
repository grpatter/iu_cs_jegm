ARM_PATH = /l/arm/gnuarm/
ARM_EXEC_PATH = $(ARM_PATH)/bin
CC = $(ARM_EXEC_PATH)/arm-elf-gcc
WAV = $(ARM_EXEC_PATH)/wav_sound_processor
CFLAGS = -g -Wall -DGOOFY -mcpu=arm7tdmi -O0
LFLAGS = -nostdlib -nostartfiles \
		-T/l/arm/gnuarm/arm-elf/lib/ldscripts/2138_rom_gdb.ld \
		-lggbsp

OBJECTCOPY = $(ARM_EXEC_PATH)/arm-elf-objcopy

OBJECTLED = uartmain.o
OBJECTGOOFY = goofymain.o
TARGETLED = uartmain.elf
TARGETGOOFY = goofymain.elf

all : $(TARGETLED) $(TARGETGOOFY) ocopygoofy ocopyled

$(TARGETLED) : $(OBJECTLED)
	$(CC) $(CFLAGS) $(LFLAGS) $(OBJECTLED) -o $(TARGETLED)
$(TARGETGOOFY) : $(OBJECTGOOFY)
	$(CC) $(CFLAGS) $(LFLAGS) $(OBJECTGOOFY) -o $(TARGETGOOFY)
$(OBJECTLED) : uartmain.c
	$(CC) -c $(CFLAGS) uartmain.c 
$(OBJECTGOOFY) : goofymain.c
	$(CC) -c $(CFLAGS) goofymain.c 
ocopyled : $(TARGETLED)
	$(OBJECTCOPY) -O binary -R .eeprom $(TARGETLED) uartmain.hex
ocopygoofy : $(TARGETGOOFY)
	$(OBJECTCOPY) -O binary -R .eeprom $(TARGETGOOFY) goofymain.hex
downloadled : uartmain.hex
	$(ARM_EXEC_PATH)/arm-elf-download uartmain.hex
downloadgoofy : goofymain.hex
	$(ARM_EXEC_PATH)/arm-elf-download goofymain.hex
process_wav : giggle.wav
	$(WAV) giggle.wav
CLEAN:
	rm uartmain.o
	rm uartmain.elf
	rm uartmain.hex
	rm goofymain.o
	rm goofymain.elf
	rm goofymain.hex
