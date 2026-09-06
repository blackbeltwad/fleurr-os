#ifndef FLEURR_CONFIG_H
#define FLEURR_CONFIG_H

// User-tunable, compile-time configuration.

#ifndef MAX_STACKS
#define MAX_STACKS 8
#endif

#ifndef MAX_SIZE
#define MAX_SIZE 128 // per-task stack size, in bytes
#endif

#ifndef PORT_MAX_INTERRUPT_PRIORITY
#define PORT_MAX_INTERRUPT_PRIORITY 6UL
#endif

#endif // FLEURR_CONFIG_H
