#include "port.h"
#include "scheduler_internal.h"
void port_init_stack_frame(uint8_t **stack_pointer, void (*entry_point)(void *),
                           void *entry_argument) {
  // Store arg to pass into context switch R25-R24
  uint16_t entry_point_address = (uint16_t)entry_point;
  uint8_t entry_point_high_byte = entry_point_address >> 8;
  uint8_t entry_point_low_byte = entry_point_address & 0x00FF;

  uint16_t entry_arg_address = (uint16_t)entry_argument;
  uint8_t entry_arg_high_byte = entry_arg_address >> 8;
  uint8_t entry_arg_low_byte = entry_arg_address & 0x00FF;

  **stack_pointer = entry_point_low_byte;
  // We pushed a byte so we DECREMENT
  stack_pointer--;
  **stack_pointer = entry_point_high_byte;

  stack_pointer -= ARG_SIZE;
  // Inside R24
  **stack_pointer = entry_arg_low_byte;
  // Inside R25
  stack_pointer--;
  **stack_pointer = entry_arg_high_byte;
  stack_pointer += ARG_SIZE + 1;
  stack_pointer -= POP_SIZE;
}
