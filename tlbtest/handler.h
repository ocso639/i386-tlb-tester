#pragma once
#include <stdint.h>

struct GateDescriptor {
    uint32_t low;
    uint32_t high;
} __attribute__((packed));

struct IDTRegister {
    uint16_t Limit;
    uint32_t Base;
} __attribute__((packed));

struct interrupt_stack_frame {
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp0;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    uint32_t errorcode;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
} __attribute__((packed));

__attribute__((noreturn)) void print_exception_info(unsigned exception, struct interrupt_stack_frame* stack);

extern void interrupt_0(void);
extern void interrupt_1(void);
extern void interrupt_2(void);
extern void interrupt_3(void);
extern void interrupt_4(void);
extern void interrupt_5(void);
extern void interrupt_6(void);
extern void interrupt_7(void);
extern void interrupt_8(void);
extern void interrupt_9(void);
extern void interrupt_10(void);
extern void interrupt_11(void);
extern void interrupt_12(void);
extern void interrupt_13(void);
extern void interrupt_14(void);
extern void interrupt_16(void);
extern void interrupt_17(void);
extern void interrupt_18(void);


void handler_init(void);