// PendSV_Handler — context switch entry point.
// TODO: implement using MSP/PSP dual-stack model and EXC_RETURN.

.syntax unified
.thumb
.global PendSV_Handler
