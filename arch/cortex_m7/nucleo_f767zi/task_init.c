#include "port.h"
#include <stdint.h>
#define WORD_SIZE 4
void port_init_stack_frame(uint8_t **stack_pointer, void (*entry)(void *),
                           void *arg) {

  uint32_t initial_xpsr = 0x01000000;
  uint32_t task_pc = (uint32_t)entry;
  uint32_t task_lr = 0xFFFFFFFFL;
  uint32_t fluff = 0x00000000;
  uint32_t push_reg = 0;
  uint32_t arg_value = (uint32_t)(arg);

  *stack_pointer -= WORD_SIZE;
  *(uint32_t *)*stack_pointer = (uint32_t)entry;
  __asm__ volatile("stmdb %[psp]!, {%[xpsr]}  \n"
                   "stmdb %[psp]!, {%[pc]}    \n"
                   "stmdb %[psp]!, {%[lr]}    \n"
                   "stmdb %[psp]!, {%[fl]}    \n"
                   "stmdb %[psp]!, {%[fl]}    \n"
                   "stmdb %[psp]!, {%[fl]}    \n"
                   "stmdb %[psp]!, {%[fl]}    \n"
                   "stmdb %[psp]!, {%[arg]}   \n"

                   "msr   psp, %[psp]         \n"
                   "mrs   %[scratch], control \n"
                   "orr   %[scratch], %[scratch], #2 \n"
                   "msr   control, %[scratch] \n"
                   "isb                       \n"

                   : [psp] "+&r"(*stack_pointer), [scratch] "=&r"(push_reg)
                   : [xpsr] "r"(initial_xpsr), [pc] "r"(task_pc),
                     [lr] "r"(task_lr), [arg] "r"(arg_value), [fl] "r"(fluff)
                   : "memory");
  *stack_pointer -= POP_SIZE;
}
