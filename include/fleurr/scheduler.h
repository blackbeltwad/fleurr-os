#ifndef FLEURR_SCHEDULER_H
#define FLEURR_SCHEDULER_H

// Single system-wide scheduler instance — no handle, no static/dynamic
// choice. Internal state lives entirely in kernel/scheduler.c.
#include <stdint.h>
void scheduler_start(uint32_t time_ms);

#endif // FLEURR_SCHEDULER_H
