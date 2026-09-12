#ifndef FLEURR_SCHEDULER_INTERNAL_H
#define FLEURR_SCHEDULER_INTERNAL_H

#include "fleurr/config.h"
#include "task_internal.h"
#include <stddef.h>
struct scheduler {
  struct task *heads[NUM_OF_PRIORITY];
  struct task *tails[NUM_OF_PRIORITY];
  struct task *current_task;
  uint32_t ready_bitmap;
};

// Private methods shared across kernel/*.c
void *store_and_pop_stack_pointer(void *stack_address);
void update_sleep_timer(void);
void fill_task(task_t *this_task);
struct task *select_next_task(void);

#endif // FLEURR_SCHEDULER_INTERNAL_H
