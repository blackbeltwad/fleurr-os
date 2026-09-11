#include "fleurr/task.h"
#include "port.h"
#include "scheduler_internal.h"
#include <avr/common.h>
#include <avr/io.h>
#include <stdint.h>
#define SPH (*(volatile unsigned char *)0x5E)
void port_start_first_task() {
  volatile uint8_t *stack_pointer = get_current_task()->stack_pointer;
  stack_pointer += POP_SIZE;

  uint8_t function_address_high_byte = *stack_pointer;
  stack_pointer++;
  uint8_t function_address_low_byte = *stack_pointer;

  uint16_t function_address =
      ((uint16_t)function_address_high_byte << 8) | function_address_low_byte;

  void (*start_function)(void *) = (void (*)(void *))function_address;

  uint16_t stack_pointer_high_address = (uint16_t)stack_pointer >> 8;
  uint16_t stack_pointer_low_address = (uint16_t)stack_pointer & 0x00FF;

  // We set stack pointer here to save the two bytes we used in task creation
  SPH = (uint8_t)stack_pointer_high_address;
  SPL = (uint8_t)stack_pointer_low_address;

  SREG |= (1 << 7);
  start_function(get_current_task()->task_arg);
}
