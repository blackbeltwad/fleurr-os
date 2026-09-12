#include "fleurr/scheduler.h"
#include "fleurr/config.h"
#include "fleurr/status.h"
#include "fleurr/task.h"
#include "port.h"
#include "scheduler_internal.h"
#include "task_internal.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>

void update_sleep_timer(void);
uint32_t CLZ(uint32_t bitmap);
void append_ready_task(task_handle_t this_task);
void choose_ready_task(void);
// Start before the first valid index; task_create() increments before use.

static struct scheduler scheduler = {
    .heads = NULL, .tails = NULL, .current_task = NULL, .ready_bitmap = 0};

void *store_and_pop_stack_pointer(void *stack_address) {
  scheduler.current_task->stack_pointer = stack_address;
  append_ready_task(scheduler.current_task);
  choose_ready_task();
  return (void *)scheduler.current_task;
}
// Get the function address at the start of the tasks stack and call it

void scheduler_start(uint32_t time_ms) {

  choose_ready_task();
  port_timer_init(time_ms);
  port_start_first_task();
  while (1) {
  };
}

void update_sleep_timer() {}

void fill_task(task_handle_t this_task) {
  uint8_t priority_bucket = this_task->priority;
  if (scheduler.heads[priority_bucket] == NULL) {
    scheduler.heads[priority_bucket] = this_task;
    scheduler.tails[priority_bucket] = this_task;
    this_task->next = NULL;
    this_task->prev = NULL;
    scheduler.ready_bitmap |= (1UL << 31);
  } else {
    scheduler.tails[priority_bucket]->next = this_task;
    this_task->prev = scheduler.tails[priority_bucket];
    scheduler.tails[priority_bucket] = this_task;
  }
}

task_handle_t get_current_task() { return scheduler.current_task; }

void append_ready_task(task_handle_t this_task) {
  uint32_t valid_bucket = this_task->priority;
  scheduler.ready_bitmap |= (1UL << valid_bucket);

  if (scheduler.heads[valid_bucket] == NULL) {
    scheduler.heads[valid_bucket] = this_task;
    scheduler.tails[valid_bucket] = this_task;
  } else {
    scheduler.tails[valid_bucket]->next = this_task;
    this_task->prev = scheduler.tails[valid_bucket]->next;
    this_task->next = NULL;
    scheduler.tails[valid_bucket] = this_task;
  }
}
void choose_ready_task(void) {

  uint32_t msb_bucket = CLZ(scheduler.ready_bitmap);
  scheduler.current_task = scheduler.heads[msb_bucket];
  scheduler.current_task->next = NULL;
  scheduler.current_task->prev = NULL;

  if (scheduler.tails[msb_bucket] != NULL) {
    if (scheduler.tails[msb_bucket] == scheduler.current_task) {
      scheduler.tails[msb_bucket] = NULL;
      scheduler.heads[msb_bucket] = NULL;
      scheduler.ready_bitmap &= (1UL << msb_bucket);
    } else {
      scheduler.heads[msb_bucket] = scheduler.current_task->next;
    }
  }
}

// Get position of MSB
uint32_t CLZ(uint32_t bitmap) {
  __asm__ volatile("CLZ %0, %0 \n" : "+r"(bitmap) : : "memory");

  return bitmap - 1;
}
