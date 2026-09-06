#ifndef FLEURR_PORT_CORTEX_M7_NUCLEO_F767ZI_H
#define FLEURR_PORT_CORTEX_M7_NUCLEO_F767ZI_H
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
uint8_t port_enter_critcal(void);
void port_exit_crital(uint8_t old_state);

#endif // FLEURR_PORT_CORTEX_M7_NUCLEO_F767ZI_H
