#include "port.h"
#include <stdint.h>
#define WORD_SIZE 4
void port_init_stack_frame(uint8_t **stack_pointer, void (*entry)(void *),
                           void *arg) {

  uint32_t pc_function_address = (uint32_t)entry;
  uint32_t arg_value = (uint32_t)(arg);

  uint32_t read_stack_pointer = (uint32_t)*stack_pointer;
  uint32_t psp;
  uint32_t control;
  uint32_t push_reg;
  uint32_t fluff = 0;

  *stack_pointer -= WORD_SIZE;
  *(uint32_t *)*stack_pointer = (uint32_t)entry;
  __asm__ volatile("mrs %1, control \n"
                   "orr %1, %1, #(1 << 1) \n"
                   "msr control, %1 \n"
                   "mrs psp, %0 \n"
                   "mov %3, read_stack_pointer \n"
                   "msr psp, %3 \n"
                   "mov %3, xpsr \n"
                   "push %3 \n"
                   "mov %3, pc_function_address \n"
                   "push %3 \n"
                   "mov %3, pc_function_address \n"
                   "push %3 \n"
                   "mov %3, fluff \n"
                   "push %3 \n"
                   "mov %3, fluff \n"
                   "push %3 \n"
                   "mov %3, fluff \n"
                   "push %3 \n"
                   "mov %3, fluff \n"
                   "push %3 \n"
                   "mov %3, arg_value \n"
                   : "+r"(psp), "+r"(control), "+r"(push_reg)
                   : "r"(read_stack_pointer), "r"(pc_function_address),
                     "r"(arg_value), "r"(fluff)
                   : "memory");
  *stack_pointer -= POP_SIZE;
}
