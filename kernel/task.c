#include "fleurr/task.h"
#include "fleurr/config.h"
#include "fleurr/status.h"
#include "port.h"
#include "scheduler_internal.h"
#include "task_internal.h"
#include <stddef.h>

// TODO: move existing task_create / task_yield / task_block / task_unblock /
// task_sleep / set_priority / get_current_task bodies here, adapted to:
//   - return fleurr_status_t instead of void where they can fail
//   - support both dynamic (task_create) and static (task_create_static)
//     allocation paths, per docs/ARCHITECTURE.md

fleurr_status_t task_create_static(task_handle_t *out, void (*entry)(void *),
                                   uint8_t priority, void *arg,
                                   task_static_t *storage) {
  task_t *this_task = (task_t *)(&storage);
  *out = this_task;
  this_task->stack_pointer = &this_task->stack[MAX_SIZE - 1];
  this_task->stack[0] = 0xFF;

  port_init_stack_frame(&this_task->stack_pointer, entry, arg);
  fill_task(this_task);

  this_task->priority = priority;
  this_task->base_priority = priority;
  this_task->state = TASK_READY;
  this_task->task_arg = arg;

  return FLEURR_OK;
}

void task_yield() { port_force_context_switch(); }
