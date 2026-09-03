#include "avr/io.h"
#include "port.h"

void port_enable_interrupt() { SREG |= (1 << 7); }

void port_disable_interrupt() { SREG &= ~(1 << 7); }
