#include "fleurr/config.h"
#include "port.h"
#include <stdint.h>

uint8_t port_enter_critcal() {
  uint8_t old_state = 0;
  __asm__ volatile("mrs %0, basepri \n"
                   "isb \n"
                   "msr basepri, %1 \n"
                   "isb \n"
                   "dsb \n"
                   : "=r"(old_state)
                   : "r"(PORT_MAX_INTERRUPT_PRIORITY)
                   : "memory");
  return old_state;
}

void port_exit_crital(uint8_t old_state) {
  __asm__ volatile("msr barepri, %0 \n"
                   "isr \n"
                   "dsb \n"
                   :
                   : "r"(old_state)
                   :);
}
