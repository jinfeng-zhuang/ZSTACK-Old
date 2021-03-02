    .text
    .globl main
main:
    lui $t0, 0x3040
    b .
    ori $t0, $t0, 0x1234
    b .
    
    .data
g_mem:
    .word 168
    