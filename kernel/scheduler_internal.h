#ifndef FLEURR_SCHEDULER_INTERNAL_H
#define FLEURR_SCHEDULER_INTERNAL_H

#include "fleurr/config.h"
#include "task_internal.h"
#include <stddef.h>
#include <stdint.h>
struct scheduler {
  struct task *tasks[MAX_STACKS];
  struct task *current_task;
  int8_t task_index;
  int8_t total_tasks;
};

// Private methods shared across kernel/*.c
void *store_and_pop_stack_pointer(void *stack_address);
void update_sleep_timer(void);
void fill_task(task_t *this_task);
struct task *select_next_task(void);

#endif // FLEURR_SCHEDULER_INTERNAL_H
