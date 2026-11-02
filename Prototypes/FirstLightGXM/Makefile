TITLEID := VEL000002
TARGET := first_light_gxm
PREFIX := arm-vita-eabi
CC := $(PREFIX)-gcc
CFLAGS := -O2 -Wall -Wextra -std=gnu11
LDFLAGS := -Wl,-q
LIBS := -lSceGxm_stub -lSceDisplay_stub -lSceCtrl_stub -lSceLibKernel_stub
OBJECTS := src/main.o

.PHONY: all clean
all: $(TARGET).vpk

$(TARGET).elf: $(OBJECTS)
	$(CC) $(LDFLAGS) $^ $(LIBS) -o $@

$(TARGET).velf: $(TARGET).elf
	vita-elf-create $< $@

eboot.bin: $(TARGET).velf
	vita-make-fself -s $< $@

param.sfo:
	vita-mksfoex -s TITLE_ID=$(TITLEID) "First Light Direct GXM" $@

$(TARGET).vpk: eboot.bin param.sfo
	vita-pack-vpk -s param.sfo -b eboot.bin $@

src/main.o: src/main.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET).elf $(TARGET).velf eboot.bin param.sfo $(TARGET).vpk
