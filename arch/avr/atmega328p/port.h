#ifndef FLEURR_PORT_AVR_ATMEGA328P_H
#define FLEURR_PORT_AVR_ATMEGA328P_H
#define POP_SIZE 34
#define ARG_SIZE 25
// Arch-specific interface that kernel/*.c calls into. Keeps
// kernel/scheduler.c free of #ifdefs for arch-specific behavior.

#include <stdint.h>

void port_start_first_task(void);
void port_timer_init(uint32_t interval_us);
void port_force_context_switch(void); // maps to existing task_yield() body
void port_init_stack_frame(uint8_t **stack_pointer, void (*entry)(void *),
                           void *arg);
void port_enable_interrupt(void);
void port_disable_interrupt(void);
#endif // FLEURR_PORT_AVR_ATMEGA328P_H
