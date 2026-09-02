# Fleurr-OS

A bare-metal RTOS kernel built from AVR up to Cortex-M7, aimed at zero-cost abstraction. Fast primitives, no shortcuts.

## What This Is

Fleurr-OS is a preemptive, priority-based real-time operating system kernel built entirely from scratch. No HAL, no vendor abstraction layers, no shortcuts. Every context switch, every scheduling decision, and every synchronization primitive is written and debugged at the register level, starting on the ATmega328P (AVR) and now expanding to the NUCLEO-F767ZI (ARM Cortex-M7).

The long-term goal is for Fleurr-OS to sit at the intersection of speed and usability. Think of it as the "C++ philosophy" applied to an RTOS: high-level, ergonomic primitives like task scheduling, mutexes, semaphores, and queues, that still compile down to the same raw performance and hardware control you'd get writing everything by hand. No abstraction should cost you cycles you didn't choose to spend.

## Why This Project Exists

This isn't a from-a-tutorial RTOS. The point is to actually understand, down to the assembly and the datasheet, what happens when a task blocks, when a context switch fires, when two tasks race on a shared resource, and why. Every bug along the way (stack layout miscalculations, priority inversion, non-atomic register access, race conditions) gets treated as something worth learning from, not just something to patch and move past.

Beyond just "build an RTOS," a couple of things are really driving this project:

- Actually understanding memory management. Stack layout, allocation strategies, and the tradeoffs between them, instead of treating it as a solved problem some library handles for you.
- Understanding the computer itself. The hardware/software boundary, what the CPU is actually doing during an interrupt, a context switch, or a memory access, instead of treating any of it as a black box.

## Current Features being imported to fleurr

- Preemptive priority-based task scheduling
- Manual context switching, hand-written at the register level
- Mutexes with priority inheritance (prevents priority inversion)
- Cross-architecture: implemented first on AVR (ATmega328P), now being ported and expanded on ARM Cortex-M7 (NUCLEO-F767ZI)

## Platforms

| Platform | Architecture | Status |
|---|---|---|
| ATmega328P | AVR | Core scheduler + mutexes implemented |
| NUCLEO-F767ZI | ARM Cortex-M7 | In progress |

## Roadmap

### RTOS Core (Cortex-M7 port)
- ARM context switching via PendSV
- MSP vs. PSP dual-stack model
- EXC_RETURN-based exception return handling
- Priority scheduling (ported and expanded)
- Mutexes with priority inheritance (ported and expanded)
- Semaphores
- Queues
- Task delays
- MPU-based task isolation
- Error handling and timeouts

### Drivers (interrupt-driven, RTOS-integrated, not polling)
- UART
- I2C
- SPI
- GY-521 accelerometer
- DS-1307 RTC
- I2C LCD
- SPI SD card module
- SPI OLED

### Future Considerations
Planned for further out (timeline flexible, depth matters more than speed here):

- **Dynamic task allocation**
- **Static vs. dynamic memory allocation schemes.** Offering a static, compile-time allocation mode alongside a dynamic allocator, so I can actually explore allocator design (fragmentation, allocation strategy tradeoffs) instead of committing to a single fixed approach.
- **Stack overflow detection.** Guard patterns or MPU-backed guard regions to catch a task overrunning its stack.
- **Stack high-water-mark / usage profiling.** Reporting how close each task has come to overflowing its stack, both as a safety feature and a debugging tool.
- **Tickless idle / low-power mode.** Letting the CPU sleep when no task is ready instead of spinning an idle task, waking only on the next relevant interrupt.

### Beyond the Kernel
- Demonstrating and resolving cache/DMA coherency issues on Cortex-M7

## Documentation

[118;1:3uThis project is documented as it's built. Debugging write-ups, before/after bug demonstrations, and design notes will live alongside the code as development progresses.

## Design Notes

- **RTOS kernel, not a general-purpose OS kernel.** Fleurr-OS provides real-time scheduling and synchronization primitives for embedded targets, not process isolation, virtual memory, or a userspace/kernel split in the general-purpose OS sense.
- **No HAL/Arduino abstraction.** Drivers and core kernel code are written directly against datasheets and reference manuals. Abstraction is something Fleurr-OS aims to provide, not something it leans on to get built.
