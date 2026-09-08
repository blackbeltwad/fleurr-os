#include "port.h"
#include <stdint.h>
#define WORD_SIZE 4
void port_init_stack_frame(uint8_t **stack_pointer, void (*entry)(void *),
                           void *arg) {

  uint32_t pc_function_address = (uint32_t)entry;
  uint32_t arg_value = (uint32_t)(arg);

  uint32_t read_stack_pointer = (uint32_t)*stack_pointer;
  uint32_t control;
  uint32_t push_reg = 0;
  uint32_t fluff = 0;

  *stack_pointer -= WORD_SIZE;
  *(uint32_t *)*stack_pointer = (uint32_t)entry;
  __asm__ volatile("mrs   %[scratch], xpsr \n"

                   "stmdb %[psp]!, {%[scratch]}               \n"
                   "stmdb %[psp]!, {%[pc_addr]}               \n"
                   "stmdb %[psp]!, {%[pc_addr]}               \n"
                   "stmdb %[psp]!, {%[fluff]}                 \n"
                   "stmdb %[psp]!, {%[fluff]}                 \n"
                   "stmdb %[psp]!, {%[fluff]}                 \n"
                   "stmdb %[psp]!, {%[fluff]}                 \n"
                   "stmdb %[psp]!, {%[arg]}                   \n"

                   "msr   psp, %[psp] \n"

                   "mrs   %[scratch], control \n"
                   "orr   %[scratch], %[scratch], #2 \n"
                   "msr   control, %[scratch] \n"
                   "isb \n"

                   : [psp] "+&r"(read_stack_pointer), [scratch] "=&r"(push_reg),
                     [control] "=r"(control)
                   : [pc_addr] "r"(pc_function_address), [arg] "r"(arg_value),
                     [fluff] "r"(fluff)
                   : "memory");
  *stack_pointer -= POP_SIZE;
}
