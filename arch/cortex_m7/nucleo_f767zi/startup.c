#include <stdint.h>

extern uint32_t _estack; // Symbol defined in linker.ld
int main(void);

void Reset_Handler(void) {
  main();
  while (1)
    ; // Trap CPU if main ever returns
}

// ARM Cortex-M Vector Table mapped to 0x08000000
__attribute__((section(".isr_vector"))) const uint32_t vector_table[] = {
    (uint32_t)&_estack,       // 0x00000000: Initial Main Stack Pointer
    (uint32_t)&Reset_Handler, // 0x00000004: Reset Vector
};
