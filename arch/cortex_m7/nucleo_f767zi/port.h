#ifndef FLEURR_PORT_CORTEX_M7_NUCLEO_F767ZI_H
#define FLEURR_PORT_CORTEX_M7_NUCLEO_F767ZI_H

#include <stdint.h>

// TODO: PendSV-based context switch, MSP/PSP setup, EXC_RETURN handling.

void port_timer_init(uint32_t interval_us);
void port_force_context_switch(void); // pends PendSV
void port_init_stack_frame(uint8_t **stack_pointer, void (*entry)(void *),
                           void *arg);
#endif // FLEURR_PORT_CORTEX_M7_NUCLEO_F767ZI_H
