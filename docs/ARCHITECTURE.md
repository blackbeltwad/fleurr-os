# Fleurr-OS Architecture

This document describes how Fleurr-OS is structured internally, the design principles behind that structure, and how the current design leaves room for planned future work without requiring a rework later.

## Design Principles

1. **The user never sees kernel internals.** Every kernel object (task, mutex, semaphore, ...) is exposed to user code only as an opaque handle. Real struct definitions live in internal headers that user code never includes.
2. **Static and dynamic allocation are interchangeable from the user's perspective.** Whether a task's memory comes from the caller (static) or the kernel (dynamic, future work), the user gets back the same handle type and uses the same API from that point on.
3. **No abstraction should cost hardware control or performance.** Every primitive is designed to compile down to direct, predictable register-level operations — the same standard applied to the hand-written AVR context switch and mutex code.
4. **Architecture decisions made now should not block features planned for later.** Where a future feature (e.g. priority ceiling protocol) would require a struct field or API shape that's cheap to add now and expensive to retrofit later, that field/shape is included now even if unused.

## Opaque Handle Pattern

All kernel objects follow the same public/private split:

- A **public header** (e.g. `task.h`, `sync.h`) forward-declares the type and exposes only a handle (`typedef struct task task_t; typedef task_t *task_handle_t;`) plus a set of functions that operate on that handle.
- An **internal header** (e.g. `task_internal.h`), included only by the kernel's own source files, contains the real struct definition.

User code interacts with kernel objects exclusively through handles and API calls — it cannot read or write a struct field directly, because it never has access to a type that defines one. Every mutation goes through a kernel function, which means every mutation can be wrapped in the atomicity guarantees (`cli()`/`sei()` on AVR, equivalent primitives on Cortex-M7) the kernel actually needs to preserve its invariants — something direct field access would bypass entirely.

## Static vs. Dynamic Allocation

Because user code never sees real struct layout, static allocation can't hand the user a real `struct task` to declare on the stack or in `.bss` — they'd need the type definition to size it. Instead, the public header exposes a **sized-but-opaque** buffer type:

```c
typedef struct {
    uint8_t _reserved[TASK_STATIC_SIZE];
} task_static_t;
```

`TASK_STATIC_SIZE` is a public constant guaranteed (via a compile-time `static_assert` inside the kernel) to be large enough to hold the real, private `struct task`. The user can declare `static task_static_t storage;` and pass its address to `task_create_static()`; the kernel initializes the real struct into that memory internally. This keeps the two allocation paths symmetric:

```c
fleurr_status_t task_create(task_handle_t *out,
                             void (*entry)(void *), uint8_t priority, void *arg);

fleurr_status_t task_create_static(task_handle_t *out,
                                    void (*entry)(void *), uint8_t priority, void *arg,
                                    task_static_t *storage);
```

Both return the same `task_handle_t`. Every other kernel function (`task_yield`, `set_priority`, mutex operations, etc.) operates on that handle identically regardless of which path created it. This is what allows dynamic task allocation (planned, not yet implemented) to be added later without changing any existing call site that already uses static allocation.

The same pattern applies to mutexes (`mutex_static_t`) and will apply to semaphores once they're implemented.

## Kernel Objects

### Task

Public: `task_handle_t` (opaque), `task_static_t` (sized opaque storage for static allocation).

Private (`struct task`, internal only): stack pointer, stack buffer, current/base priority, state, sleep remaining, task argument, mutex-ownership bookkeeping.

The split between `priority` (effective, scheduler-visible) and `base_priority` (the task's actual assigned priority) exists specifically to support priority inheritance — the scheduler always reads the effective value, while the base value is what gets restored once a boost is no longer justified.

### Scheduler

Unlike task/mutex/semaphore, there is exactly one scheduler instance system-wide — it doesn't need a handle, static/dynamic choice, or public struct at all. It lives as a private object inside the kernel's own source file, expo[118;1:3used only through functions (`scheduler_start()`, and later, read-only status/statistics functions that return plain values rather than pointers into internal state).

### Mutex

Public: `mutex_handle_t` (opaque), `mutex_static_t` (sized opaque storage).

Private (`struct mutex`, internal only): owner, list of blocked waiters, and a `protocol` field.

The `protocol` field is included now, even though only one protocol is currently implemented, so that adding a second protocol later is a matter of adding a branch inside `lock_mutex`/`unlock_mutex`, not changing the struct shape or any existing call site:

```c
typedef enum {
    PROTOCOL_INHERIT,
    PROTOCOL_CEILING
} mutex_protocol_t;

fleurr_status_t mutex_create(mutex_handle_t *out, mutex_protocol_t protocol,
                              uint8_t ceiling_priority /* unused if PROTOCOL_INHERIT */);
```

**Currently implemented — `PROTOCOL_INHERIT`:** reactive priority inheritance. When a task blocks on a mutex held by a lower-priority task, the holder is boosted to the blocker's priority for the duration it holds the lock, then restored on unlock. Scoped, for now, to a single mutex held at a time — a task holding multiple mutexes simultaneously, and the transitive/chained boosting that implies, is explicitly deferred rather than partially implemented.

**Planned, not yet implemented — `PROTOCOL_CEILING`:** priority ceiling protocol. Each such mutex is created with a fixed ceiling priority (the highest priority of any task that could ever lock it); a task locking the mutex is boosted to that ceiling immediately on lock, proactively, rather than only once real contention appears. This gives a stronger, provable bound on blocking time than reactive inheritance, at the cost of the caller needing to know the ceiling value up front.

Protocol selection is per-mutex, decided at creation time — a single mutex uses one protocol or the other, never both.

## Error Handling

Kernel functions that can fail return a status enum rather than silently succeeding or asserting:

```c
typedef enum {
    FLEURR_OK = 0,
    FLEURR_ERR_NOMEM,
    FLEURR_ERR_INVALID_ARG,
    FLEURR_ERR_LIMIT_REACHED,
    // extended as new failure modes are identified
} fleurr_status_t;
```

## Planned, Architecturally-Anticipated Work

These are not being built now, but the design above is deliberately kept compatible with them so they don't require restructuring existing code when their time comes:

- **Dynamic task allocation** — a kernel-owned allocator behind `task_create()`, symmetric with the existing static path.
- **Priority ceiling protocol** — the second branch of the `protocol` field described above.
- **Strong memory-safety/detection features** — stack guard patterns, MPU-backed guard regions (Cortex-M7), and stack high-water-mark tracking. These add fields to the private `struct task` (e.g. a guard value, a high-water-mark counter) — invisible to user code by construction, since the struct is already private.
- **Better dynamic memory management** — allocator strategy not yet decided; whatever is chosen sits behind the same `task_create()`/`mutex_create()` entry points and doesn't change their public signatures.


