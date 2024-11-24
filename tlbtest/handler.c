#include "uart.h"
#include "handler.h"

struct GateDescriptor idt[13];
struct IDTRegister IDTR;

__attribute__((noreturn)) void print_exception_info(unsigned exception, struct interrupt_stack_frame* stack) {
    uart_outf("! -=unhandled exception (%d)=-\r\n", exception);
    uart_outf("! cs:eip=%w:%p ecode=%x\r\n", stack->cs, stack->eip, stack->errorcode);
    uart_outf("! eax=%x ebx=%x ecx=%x edx=%x\r\n", stack->eax, stack->ebx, stack->ecx, stack->edx);
    uart_outf("! edi=%x esi=%x ebp=%x eflags=%x\r\n", stack->edi, stack->esi, stack->ebp, stack->eflags);
    reboot_prompt();
    while (1)
        asm volatile("cli;hlt");
    __builtin_unreachable();
}

static struct GateDescriptor IDTCreateTrapGate(uint16_t SegmentSelector, uint32_t Offset, uint8_t Size, uint8_t PrivilegeLevel, uint8_t Present) {
    struct GateDescriptor TrapGate;
    TrapGate.low = 0;
    TrapGate.high = 0;

    TrapGate.low |= (Offset & 0xffff);
    TrapGate.low |= ((uint32_t)SegmentSelector << 16);
    TrapGate.high |= (0b111 << 8);
    TrapGate.high |= ((Size & 0b1) << 11);
    TrapGate.high |= ((PrivilegeLevel & 0b11) << 13);
    TrapGate.high |= ((Present & 0b1) << 15);
    TrapGate.high |= (Offset & (0xffff << 16));

    return TrapGate;
}

void handler_init(void) {
    idt[0] = IDTCreateTrapGate(0x8, (uint32_t)interrupt_0, 1, 0, 1);
    idt[1] = IDTCreateTrapGate(0x8, (uint32_t)interrupt_1, 1, 0, 1);
    idt[2] = IDTCreateTrapGate(0x8, (uint32_t)interrupt_2, 1, 0, 1);
    idt[3] = IDTCreateTrapGate(0x8, (uint32_t)interrupt_3, 1, 0, 1);
    idt[4] = IDTCreateTrapGate(0x8, (uint32_t)interrupt_4, 1, 0, 1);
    idt[5] = IDTCreateTrapGate(0x8, (uint32_t)interrupt_5, 1, 0, 1);
    idt[6] = IDTCreateTrapGate(0x8, (uint32_t)interrupt_6, 1, 0, 1);
    idt[7] = IDTCreateTrapGate(0x8, (uint32_t)interrupt_7, 1, 0, 1);
    idt[8] = IDTCreateTrapGate(0x8, (uint32_t)interrupt_8, 1, 0, 1);
    idt[9] = IDTCreateTrapGate(0x8, (uint32_t)interrupt_9, 1, 0, 1);
    idt[10] = IDTCreateTrapGate(0x8, (uint32_t)interrupt_10, 1, 0, 1);
    idt[11] = IDTCreateTrapGate(0x8, (uint32_t)interrupt_11, 1, 0, 1);
    idt[12] = IDTCreateTrapGate(0x8, (uint32_t)interrupt_12, 1, 0, 1);
    idt[13] = IDTCreateTrapGate(0x8, (uint32_t)interrupt_13, 1, 0, 1);
    idt[14] = IDTCreateTrapGate(0x8, (uint32_t)interrupt_14, 1, 0, 1);
    idt[16] = IDTCreateTrapGate(0x8, (uint32_t)interrupt_16, 1, 0, 1);
    idt[17] = IDTCreateTrapGate(0x8, (uint32_t)interrupt_17, 1, 0, 1);
    idt[18] = IDTCreateTrapGate(0x8, (uint32_t)interrupt_18, 1, 0, 1);
    

    IDTR.Limit = (sizeof(struct GateDescriptor) * 256) - 1; 
    IDTR.Base = (uint32_t)&idt;
    asm volatile ( "lidt %0" : : "m"(IDTR) );
}