#include "port.h"
#include <avr/io.h>

void port_force_context_switch() {
  // Force context switch
  OCR1AL = 0;
  OCR1AL = 0;

  TIFR1 |= (1 << 1);
}
