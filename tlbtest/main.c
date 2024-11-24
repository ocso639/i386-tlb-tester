#include <stdint.h>

#include "uart.h"
#include "handler.h"
#include "testregister.h"
#include "tlb.h"
#include "inline.h"

void _start(void) {
    outb(0x70, inb(0x70) | 0x80); inb(0x71); //disable NMIs
    uint16_t uart_port = *(uint16_t*)(0x0400);
    
    uart_setport(uart_port);
    uart_clear();
    uart_outs("\e[1;1H\e[2J");
    uart_outf("> COM1 port is 0x%w\r\n", uart_port);
    uart_outs("> i386/i486 TLB test program\r\n");
    uart_outs("? press any key (in serial console) to start\r\n");
    uart_wait();
    uart_outs("> checking if TLB test registers work (exception 6 if unavailable)\r\n");
    test_tr();
    write_tlb(0x00000000, 0x10000000, 1, 1, 0, 0, 1, 0, 0, 0);
    write_tlb(0x10000000, 0x20000000, 2, 1, 0, 0, 1, 0, 0, 0);
    write_tlb(0x30000000, 0x40000000, 3, 1, 0, 0, 1, 0, 0, 0);
    write_tlb(0x50000000, 0x80000000, 0, 1, 0, 0, 1, 0, 0, 0);
    uart_outs("> ok. testing TLB access\r\n");
    for (int i = 0; i < 12; i++) {
        int result = lookup_tlb(0x10000000 * i);
        if (result == 0)
            uart_outf("> addr $%ph TLB hit (i=%d)\r\n", 0x10000000 * i, i);
        else uart_outf("> addr $%ph TLB miss (i=%d)\r\n", 0x10000000 * i, i);
    }
    uart_outs("> program finished\r\n");
    reboot_prompt();
    while (1);
}