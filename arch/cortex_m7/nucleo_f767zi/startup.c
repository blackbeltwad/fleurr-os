#include <stdint.h>
extern int main(void);
void Reset_Handler(void) {
  main();
  while (1) {
  };
}
void Default_Handler(void) {
  while (1)
    ; // Trap CPU on unexpected interrupt
}

// Weak Exception Prototypes (Internal to startup.c)
void Reset_Handler(void);
void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));

typedef void (*const ExecFuncPtr)(void);
extern uint32_t _estack;

//  Vector Table
__attribute__((section(".isr_vector"))) ExecFuncPtr vector_table[] = {
    (ExecFuncPtr)&_estack,
    Reset_Handler,
    NMI_Handler,
    HardFault_Handler,
    0,
    0,
    0,
    0,
    0,
    0,
    0, // Reserved
    SVC_Handler,
    0,              // Debug Monitor
    0,              // Reserved
    PendSV_Handler, // Points to Default_Handler until port.c defines
                    // PendSV_Handler
    SysTick_Handler,
};
