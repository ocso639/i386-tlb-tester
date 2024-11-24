#define BASE            0
#define LIMIT           0xffffffff
#define READ_LENGTH     $12
    .code16
    .globl _start 
_start:
    ljmp $0x0000,$1f
1:
    mov $0x7c00, %sp
    mov %dl, (disk)
    xor %ax, %ax
    mov %ax, %ds
    cld
    // reset disk system
    mov $0x00, %ah 
    int $0x13
    // read disk
    mov $0x02, %ah 
    mov READ_LENGTH, %al 
    mov $0, %ch 
    mov $2, %cl
    mov $0, %dh 
    xor %bx, %bx 
    mov %bx, %es
    mov (disk), %dl
    mov $0x9000, %bx
    clc
    int $0x13
    mov %ax, (errorcode)
    jc fail
    jmp continue

fail:
    mov (errorcode), %ax
    mov %ah, %al 
    out %al, $0x80
    cli 
    hlt

continue:
    mov $0x03, %al
    mov $0, %ah 
    int $0x10

    xor %ax, %ax 
    mov %ax, %es 
    
    // write notice to screen
    mov $0x01, %al 
    mov $0, %bh 
    mov $0x0F, %bl
    mov $(message_end - message), %cx
    mov $4, %dh
    mov $7, %dl 
    mov $message+1, %bp
    mov $0x13, %ah 
    int $0x10

    mov $0x01, %ah
    mov $0x2607, %cx
    int $0x10
    
    mov $0x01, %al 
    mov $0, %bh 
    mov $0x0F, %bl
    mov $(message2_end - message_2), %cx
    mov $5, %dh
    mov $6, %dl 
    mov $message_2+1, %bp
    mov $0x13, %ah 
    int $0x10

    mov $0x01, %ah
    mov $0x2607, %cx
    int $0x10

    // enable A20
    in $0x92, %al
    or 2, %al
    out %al, $0x92

    mov $0xF8, %al 
    out %al, $0x80

    // enable pmode
    mov %cr0, %eax 
    or $1, %eax
    mov %eax, %cr0
    lgdt (gdtr)
    ljmp $0x08,$pmode
.code32
pmode:
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es 
    mov %ax, %fs 
    mov %ax, %ss
    mov %ax, %gs
    cli 
    jmp 0x7e00

disk: .byte 0
errorcode: .byte 0


message: 
    .byte 0
    .asciz "OUTPUTTING TO COM1. PRESS ENTER IN SERIAL CONSOLE TO START TEST."
message_end:

message_2:
    .byte 0
    .asciz "IF THERE IS NO OUTPUT IN COM1, PLEASE CHECK THE SERIAL CONNECTION."
message2_end:

gdt:
    null_low:   .long 0
    null_high:  .long 0
    code_low:   .long (LIMIT & 0xFFFF) | (BASE << 16)
    code_high:  .long ((BASE >> 16) & 0b11111111) | (1 << 9) | (1 << 11) | (1 << 12) | (1 << 15) | (((LIMIT >> 16) & 0b1111) << 16) | (1 << 22) | (1 << 23) | (((BASE >> 24) & 0b11111111) << 24)
    data_low:   .long (LIMIT & 0xFFFF) | (BASE << 16)
    data_high:  .long ((BASE >> 16) & 0b11111111) | (1 << 9) | (1 << 12) | (1 << 15) | (((LIMIT >> 16) & 0b1111) << 16) | (1 << 22) | (1 << 23) | (((BASE >> 24) & 0b11111111) << 24)
gdt_end:

gdtr:
    .word (gdt_end - gdt) - 1
    .long (gdt)

.org 510
.word 0xaa55