MCU = atmega168
F_CPU = 8000000UL

LIBDIR = ../src

PROGRAMMER = usbtiny

CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
AVRSIZE = avr-size
AVRDUDE = avrdude

TARGET = main

SOURCES=$(wildcard *.c $(LIBDIR)/*.c)
OBJECTS=$(SOURCES:.c=.o)
HEADERS=$(SOURCES:.c=.h)

CPPFLAGS  = -DF_CPU=$(F_CPU) -I. -I$(LIBDIR)
# ATmega168a DIP layout
CPPFLAGS += -DMOSI=PB3 -DMOSI_DDR=DDRB -DMOSI_PORT=PORTB
CPPFLAGS += -DSS=PB2 -DSS_DDR=DDRB -DSS_PORT=PORTB
CPPFLAGS += -DSCK=PB5 -DSCK_DDR=DDRB
# Lumenati 8-LED Strip
CPPFLAGS += -DLED_COUNT=8

CFLAGS  = -Os -g -std=gnu99 -Wall
CFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums 
CFLAGS += -ffunction-sections -fdata-sections 

LDFLAGS  = -Wl,-Map,$(TARGET).map 
LDFLAGS += -Wl,--gc-sections 

TARGET_ARCH = -mmcu=$(MCU)


%.o: %.c $(HEADERS) Makefile
	 $(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c -o $@ $<;

$(TARGET).elf: $(OBJECTS)
	$(CC) $(LDFLAGS) $(TARGET_ARCH) $^ $(LDLIBS) -o $@

%.hex: %.elf
	 $(OBJCOPY) -j .text -j .data -O ihex $< $@


.PHONY: all flash clean

all: $(TARGET).hex

flash: $(TARGET).hex
	$(AVRDUDE) -c $(PROGRAMMER) -p $(MCU) -U flash:w:$<

clean:
	rm -f *.hex *.elf *.map
	rm -f *.o $(LIBDIR)/*.o
