    .code32 
    .section .preload, "ax", @progbits
    .type _preload, @function 
    .extern _start
    .extern handler_init
_preload:
    cli
    xor %eax, %eax 
    mov %eax, %cr3 // flush TLB
    call handler_init
    call _start
    cli 
    hlt