TARGET  = avr
CC      = avr-gcc
MCU     = atmega328p
F_CPU   = 16000000UL
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -O0 -g3 -Wall -Wextra -std=gnu11
INCLUDES = -Iinclude -Ikernel -Iarch/avr/atmega328p
ARCH_DIR = arch/avr/atmega328p
EXAMPLE_DIR = examples/avr_led_blink
SRCS = $(wildcard kernel/*.c) $(wildcard $(ARCH_DIR)/*.c) $(wildcard $(ARCH_DIR)/*.S) $(wildcard $(EXAMPLE_DIR)/*.c)
BUILD_DIR = build
OUT = $(BUILD_DIR)/fleurr_avr.elf
HEX = $(BUILD_DIR)/fleurr_avr.hex
PORT = /dev/cu.usbmodem1101
BAUD = 115200
GDB_PORT = 1234

.PHONY: build flash clean simavr gdb

build: $(SRCS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $(SRCS) -o $(OUT)

flash: build
	avr-objcopy -O ihex -R .eeprom $(OUT) $(HEX)
	avrdude -p $(MCU) -c arduino -P $(PORT) -b $(BAUD) -U flash:w:$(HEX):i

# Runs the elf under simavr with a gdb server, paused at reset until gdb
# attaches. Leave this running in one terminal, then `make gdb` in another.
simavr: build
	simavr -m $(MCU) -f $(F_CPU) -g $(OUT)

# Connects avr-gdb to the simavr gdb server started by `make simavr`.
gdb: build
	avr-gdb $(OUT) -ex "target remote localhost:$(GDB_PORT)"

clean:
	rm -rf $(BUILD_DIR)
