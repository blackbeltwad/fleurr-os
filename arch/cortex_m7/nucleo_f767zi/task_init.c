#include "port.h"
#include <stdint.h>
void port_init_stack_frame(uint8_t **stack_pointer, void (*entry)(void *),
                           void *arg) {

  uint32_t initial_xpsr = 0x01000000;
  uint32_t task_pc = (uint32_t)entry;
  uint32_t task_lr = 0xFFFFFFFF;
  uint32_t fluff = 0x00000000;
  uint32_t arg_value = (uint32_t)(arg);

  *stack_pointer -= ALLIGN_IT;
  *(uint32_t *)*stack_pointer = initial_xpsr;
  *stack_pointer -= WORD_SIZE;
  *(uint32_t *)*stack_pointer = task_pc;
  *stack_pointer -= WORD_SIZE;
  *(uint32_t *)*stack_pointer = task_lr;

  *stack_pointer -= WORD_SIZE;
  *(uint32_t *)*stack_pointer = fluff;
  *stack_pointer -= WORD_SIZE;
  *(uint32_t *)*stack_pointer = fluff;
  *stack_pointer -= WORD_SIZE;
  *(uint32_t *)*stack_pointer = fluff;
  *stack_pointer -= WORD_SIZE;
  *(uint32_t *)*stack_pointer = fluff;

  *stack_pointer -= WORD_SIZE;
  *(uint32_t *)*stack_pointer = arg_value;

  *stack_pointer -= POP_SIZE;
}
