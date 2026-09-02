#include "fleurr/status.h"
#include "fleurr/sync.h"
#include "sync_internal.h"
#include "task_internal.h"
#include <stddef.h>

// TODO: move existing lock_mutex / unlock_mutex bodies here, adapted to:
//   - return fleurr_status_t
//   - branch on mutex->protocol (PROTOCOL_INHERIT implemented,
//     PROTOCOL_CEILING planned — see docs/ARCHITECTURE.md)
