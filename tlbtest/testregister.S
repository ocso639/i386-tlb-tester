    .code32

    .section .text
    .globl test_tr
    .type test_tr, @function 
test_tr:
    push %eax
    mov %tr7, %eax
    pop %eax
    retl

    .globl test_register_2
    .type test_register_2, @function
test_register_2:
    pushal 
    pushfl
    mov $1234, %eax
    mov %eax, %tr7 
    xor %eax, %eax
    mov %tr7, %eax
    popfl
    popl %edi
    popl %esi 
    popl %ebp 
    addl $4, %esp 
    popl %ebx 
    popl %edx 
    popl %ecx 
    add $4, %esp
    retl


.section .note.GNU-stack