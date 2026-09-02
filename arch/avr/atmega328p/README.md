# ATmega328P Port Notes

- `POP_SIZE` derivation: 32 GPRs + SREG pushed = 33 bytes, `pop` is
  pre-increment so push/pop are exact mirrors. See docs/debugging/ for the
  full write-up.
- Stack layout for a freshly created task must line up with the ISR's
  33-pop epilogue + RETI's 2-byte pop.
