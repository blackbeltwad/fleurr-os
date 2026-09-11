#include <stdint.h>
extern int main(void);
extern uint32_t _estack;
void Default_Handler();

extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;
extern uint32_t _estack;

void Reset_Handler(void) {
  // Copy .data section from Flash to RAM
  uint32_t *src = &_sidata;
  uint32_t *dst = &_sdata;
  while (dst < &_edata) {
    *dst++ = *src++;
  }

  // Zero initialize .bss section in RAM
  dst = &_sbss;
  while (dst < &_ebss) {
    *dst++ = 0;
  }

  //  Set PSP and CONTROL register, then launch main
  __asm volatile("msr PSP, %0 \n"
                 "mrs r0, CONTROL \n"
                 "orr r0, r0, #2 \n"
                 "msr CONTROL, r0 \n"
                 "isb \n" ::"r"(0x2001FFA0)
                 : "r0");

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
