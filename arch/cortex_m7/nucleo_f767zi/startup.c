#include <stdint.h>
extern int main(void);
extern uint32_t _estack;
void Default_Handler();
void Reset_Handler(void) {
  // Set psp to top of dtcm like
  __asm__ volatile("msr psp, %0" : : "r"(&_estack));

  __asm__ volatile("mrs r0, control \n"
                   "orr r0, r0, #2  \n"
                   "msr control, r0 \n"
                   "isb             \n" ::
                       : "r0", "memory");

  main();
  while (1)
    ;
}
// Weak Exception Prototypes (Internal to startup.c)
void Reset_Handler(void);
void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));

typedef void (*const ExecFuncPtr)(void);

void Default_Handler() {
  while (1)
    ;
}
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
