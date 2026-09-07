#include "fleurr/scheduler.h"
#include "fleurr/status.h"
#include "fleurr/task.h"
#include "port.h"
#include "scheduler_internal.h"
#include "task_internal.h"
#include <stddef.h>
#include <stdint.h>

void update_sleep_timer(void);
struct task *select_next_task(void);
// Start before the first valid index; task_create() increments before use.

static struct scheduler scheduler = {
    .tasks = {0}, .current_task = NULL, .task_index = -1, .total_tasks = -1};

// TODO: move existing select_next_task / store_and_pop_stack_pointer /
// update_sleep_timer / scheduler_start bodies here.

void *store_and_pop_stack_pointer(void *stack_address) {

  scheduler.current_task->stack_pointer = stack_address;
  if (scheduler.total_tasks == scheduler.task_index) {
    scheduler.task_index = 0;
  } else {
    scheduler.task_index++;
  }

  //  Prepare current task for pop
  scheduler.current_task = scheduler.tasks[scheduler.task_index];
  // scheduler.current_task = select_next_task();
  return (void *)(scheduler.current_task->stack_pointer);
}
// Get the function address at the start of the tasks stack and call it
struct task *select_next_task() {
  struct task *highest_priority_task = NULL;
  uint8_t highest_priority = 0;
  uint8_t task_count = scheduler.total_tasks + 1;

  // Find highest priority among READY tasks
  for (int i = 0; i <= scheduler.total_tasks; i++) {
    struct task *this_task = scheduler.tasks[i];

    if (this_task->state == TASK_READY &&
        this_task->priority >= highest_priority) {

      highest_priority = this_task->priority;
      highest_priority_task = this_task;
    }
  }

  // No READY task at all
  // Add a fallback here maybe
  if (highest_priority_task == NULL) {
    return scheduler.current_task;
  }

  // Search circularly for the next READY task at that priority,
  // starting just after the currently running task
  for (int i = 1; i <= scheduler.total_tasks; i++) {
    int index = (scheduler.task_index + i) % task_count;
    struct task *this_task = scheduler.tasks[index];

    if (this_task->state == TASK_READY &&
        this_task->priority == highest_priority) {

      scheduler.task_index = index;
      highest_priority_task = this_task;
      break;
    }
  }

  scheduler.current_task->state = TASK_READY;
  highest_priority_task->state = TASK_RUNNING;

  return highest_priority_task;
}
void scheduler_start(uint32_t time_ms) {
  uint8_t highest_priority = 0;
  uint8_t highest_priority_task_index = 0;
  for (int i = 0; i < scheduler.total_tasks + 1; i++) {
    struct task *this_task = scheduler.tasks[i];
    if (this_task->state == TASK_READY &&
        this_task->priority >= highest_priority) {
      highest_priority = this_task->priority;
      highest_priority_task_index = i;
    }
  }

  scheduler.current_task = scheduler.tasks[highest_priority_task_index];
  port_timer_init(time_ms);
  port_start_first_task();
  while (1) {
  };
}

void update_sleep_timer() {

  for (uint8_t i = 0; i < scheduler.total_tasks; i++) {
    struct task *this_task = scheduler.tasks[i];

    if (this_task->state == TASK_SLEEPING) {
      this_task->sleep_remaining -= 1;

      if (this_task->sleep_remaining <= 0) {
        this_task->state = TASK_READY;
        this_task->sleep_remaining = 0;
      }
    }
  }
}

void fill_task(task_t *this_task) {

  scheduler.task_index++;
  scheduler.total_tasks++;
  scheduler.tasks[scheduler.task_index] = this_task;
  scheduler.current_task = this_task;
}

task_handle_t get_current_task() { return scheduler.current_task; }
