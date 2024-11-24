    .code32
    .altmacro 

    .macro isr num
    .global interrupt_\num
    .extern print_exception_info
    interrupt_\num:
    .if \num != 8 && \num != 10 && \num != 11 && \num != 12 && \num != 13 && \num != 14 && \num != 17 && \num != 30
        pushl $0
    .endif
        pushal
        cld
        pushl %esp
        pushl $\num
        calll print_exception_info
    .endm
    .section .text


    isr 0
    isr 1
    isr 2
    isr 3
    isr 4
    isr 5
    isr 6
    isr 7
    isr 8
    isr 9
    isr 10
    isr 11
    isr 12
    isr 13
    isr 14 
    isr 16
    isr 17
    isr 18 

    

    .section .note.GNU-stack