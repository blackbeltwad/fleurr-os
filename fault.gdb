# fault.gdb
printf "====== CORTEX-M7 FAULT DIAGNOSTICS ======\n"
printf "CFSR  (0xE000ED28): 0x%08x\n", *(uint32_t*)0xE000ED28
printf "HFSR  (0xE000ED2C): 0x%08x\n", *(uint32_t*)0xE000ED2C
printf "MMFAR (0xE000ED34): 0x%08x\n", *(uint32_t*)0xE000ED34
printf "BFAR  (0xE000ED38): 0x%08x\n", *(uint32_t*)0xE000ED38
printf "LR (EXC_RETURN)   : 0x%08x\n", $lr
printf "MSP               : 0x%08x\n", $msp
printf "PSP               : 0x%08x\n", $psp
printf "HARDWARE STACK FRAME AT MSP:\n"
x/8xw $msp
printf "HARDWARE STACK FRAME AT PSP:\n"
x/8xw $psp
