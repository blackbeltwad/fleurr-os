TARGET  = avr
CC      = avr-gcc
MCU     = atmega328p
F_CPU   = 16000000UL
CFLAGS  = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall -Wextra -std=gnu11
INCLUDES = -Iinclude -Ikernel -Iarch/avr/atmega328p

ARCH_DIR = arch/avr/atmega328p
EXAMPLE_DIR = examples/avr_led_blink

SRCS = $(wildcard kernel/*.c) $(wildcard $(ARCH_DIR)/*.c) $(wildcard $(ARCH_DIR)/*.S) $(wildcard $(EXAMPLE_DIR)/*.c)

BUILD_DIR = build
OUT = $(BUILD_DIR)/fleurr_avr.elf
HEX = $(BUILD_DIR)/fleurr_avr.hex

PORT = /dev/cu.usbmodem1101
BAUD = 115200

.PHONY: build flash clean

build: $(SRCS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $(SRCS) -o $(OUT)

flash: build
	avr-objcopy -O ihex -R .eeprom $(OUT) $(HEX)
	avrdude -p $(MCU) -c arduino -P $(PORT) -b $(BAUD) -U flash:w:$(HEX):i

clean:
	rm -rf $(BUILD_DIR)
