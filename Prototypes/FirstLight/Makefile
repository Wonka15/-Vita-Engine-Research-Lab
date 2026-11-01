TITLEID  := VEL000001
TARGET   := first_light
SOURCES  := src
PREFIX   := arm-vita-eabi

CC       := $(PREFIX)-gcc
CFLAGS   := -O2 -Wall -Wextra -std=gnu11 -ffast-math
LDFLAGS  := -Wl,-q
LIBS     := -lvitaGL -lstdc++ -lc -lm -lmathneon \
            -lSceGxm_stub -lSceDisplay_stub -lSceAppMgr_stub \
            -lSceCtrl_stub -lSceLibKernel_stub -lSceCommonDialog_stub \
            -lvitashark -lSceShaccCgExt -ltaihen_stub \
            -lSceShaccCg_stub -lSceKernelDmacMgr_stub

OBJECTS  := $(SOURCES)/main.o

.PHONY: all clean
all: $(TARGET).vpk

$(TARGET).elf: $(OBJECTS)
	$(CC) $(LDFLAGS) $^ $(LIBS) -o $@

$(TARGET).velf: $(TARGET).elf
	vita-elf-create $< $@

eboot.bin: $(TARGET).velf
	vita-make-fself -s $< $@

param.sfo:
	vita-mksfoex -s TITLE_ID=$(TITLEID) "Vita Engine First Light" $@

$(TARGET).vpk: eboot.bin param.sfo
	vita-pack-vpk -s param.sfo -b eboot.bin $@

$(SOURCES)/main.o: $(SOURCES)/main.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET).elf $(TARGET).velf eboot.bin param.sfo $(TARGET).vpk
