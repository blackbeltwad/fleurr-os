// PendSV_Handler — context switch entry point.
// TODO: implement using MSP/PSP dual-stack model and EXC_RETURN.

.syntax unified
.thumb
.global PendSV_Handler
.extern store_and_pop_stack_pointer
.extern update_sleep_timer

PendSV_Handler:
  push R4
  push R5
  push R6
  push R7
  push R8
  push R9
  push R10
  push R11
  ; Pushing Special Registers
  mrs R0, PRIMASK
  push R0 
  mrs R0, FAULTMASK
  push R0
  mrs R0, BASEPRI
  push R0
  mrs R0, CONTROL
  push R0
  mrs R0, PSP
  bl store_and_pop_stack_pointer
  msr PSP, R0
  ; Pop onto new stack
  pop R0 
  mrs R0, CONTROL
  pop R0
  mrs R0, BASEPRI
  pop R0 
  mrs R0, FAULTMASK
  pop R0 
  mrs R0, PRIMASK
  pop R11 
  pop R10 
  pop R9 
  pop R8 
  pop R7 
  pop R6
  pop R5
  pop R4
  pop R0 
  pop R1 
  pop R2 
  pop R3
  
  
