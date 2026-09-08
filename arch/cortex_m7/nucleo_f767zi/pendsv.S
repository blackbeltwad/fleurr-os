// PendSV_Handler — context switch entry point.
// TODO: implement using MSP/PSP dual-stack model and EXC_RETURN.

.syntax unified
.thumb
.global PendSV_Handler
.extern store_and_pop_stack_pointer
.extern update_sleep_timer

PendSV_Handler:
  mrs R0, PSP
  stmdb R0!, {R4}
  stmdb R0!, {R5}
  stmdb R0!, {R6}
  stmdb R0!, {R7}
  stmdb R0!, {R8}
  stmdb R0!, {R9}
  stmdb R0!, {R10}
  stmdb R0!, {R11}

  // Pushing Special Registers
  mrs R1, PRIMASK
  stmdb R0!, {R1}
  mrs R1, FAULTMASK
  stmdb R0!, {R1}
  mrs R1, BASEPRI
  stmdb R0!, {R1}
  mrs R1, CONTROL
  orr R1, R1, #(1 << 1)
  stmdb R0!, {R1}

  bl store_and_pop_stack_pointer

  ldmia r0!, {r1}
  msr CONTROL, R1
  ldmia r0!, {r1}
  msr BASEPRI, R1
  ldmia r0!, {r1}
  msr FAULTMASK, R1
  ldmia r0!, {r1}
  msr PRIMASK, R1

  ldmia r0!, {r11}
  ldmia r0!, {r10}
  ldmia r0!, {r9}
  ldmia r0!, {r8}
  ldmia r0!, {r7}
  ldmia r0!, {r6}
  ldmia r0!, {r5}
  ldmia r0!, {r4}

  bx lr
