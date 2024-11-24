#include <stdint.h>
#include <stdarg.h>

#include "uart.h"
#include "inline.h"
#include "memory.h"

static uint16_t uart_port = 0;

static int is_transmit_empty(uint16_t port) {
    return inb(port + 5) & 0x20;
}

void uart_setport(uint16_t port) {
    uart_port = port;
}

void uart_outc(char c) {
    while (is_transmit_empty(uart_port) == 0);
    outb(0x80, c);
    outb(uart_port, c);
}

void uart_outs(const char* s) {
    for (size_t i = 0; i < strlen(s); i++)
        uart_outc(s[i]);
}

void uart_clear(void) {
    (void)inb(uart_port);
}

void uart_wait(void) {
    while ((inb(uart_port + 5) & 1) == 0);
    uart_clear();
}


static char* itoa(uint32_t n, uint8_t base) {
    static char chars[] = "0123456789abcdef";
    static char buf[128];
    char* ptr = &buf[127];
    
    *ptr = '\0';

    if (n  == 0) {
        *--ptr = '0';
    }

    while(n) {
        *--ptr = chars[n % base];
        n /= base;
    }

    return ptr;
}

static char* hextoa(uint32_t n, uint8_t padding) {
    static char chars[] = "0123456789abcdef";
    static char buf[128];
    char* ptr = &buf[127];

    *ptr = '\0';
    
    if (n == 0) {
        *--ptr = '0';
    }

    while (n) {
        *--ptr = chars[n % 16];
        n /= 16;
    }

    while (ptr > &buf[127 - padding]) {
        *--ptr = '0';
    }

    return ptr;
}

void uart_outf(const char* fmt, ...) {
    va_list ap;
    int i, c;
    char* s;
    va_start(ap, fmt);

    for (i = 0; (c = fmt[i] & 0xff) != 0; i++) {
        if (c != '%') {
            uart_outc(c);
            continue;
        }
        c = fmt[++i] & 0xff;
        if (c == 0)
            break;
        switch(c) {
            case 'd': {
                uart_outs(itoa(va_arg(ap, int), 10));
                break;
            }
            case 'l': {
                uart_outs(itoa(va_arg(ap, uint64_t), 10));
                break;
            }
            case 'x': {
                uart_outs(hextoa(va_arg(ap, unsigned int), 8));
                break;
            }
            case 'p': {
                uart_outs(hextoa(va_arg(ap, size_t), 8));
                break;
            }
            case 'w': {
                uart_outs(hextoa(va_arg(ap, size_t), 4));
                break;
            }
            case 's': {
                if ((s = va_arg(ap, char*)) == 0)
                s = "(null)";
                for (; *s; s++)
                    uart_outc(*s);
                break;
            }
            case '%': {
                uart_outc('%');
                break;
            }
            default: {
                uart_outc('%');
                uart_outc(c);
                break;
            }
        }
        va_end(ap);
    }
}

__attribute__((noreturn)) static void reboot(void) {
    while(1) {
        uint8_t good = 0x02;
        while (good & 0x02)
            good = inb(0x64);
        outb(0x64, 0xFE);
    }
}

void reboot_prompt(void) {
    uart_outs("? press any key (in serial console) to reboot machine\r\n");
    uart_wait();
    reboot();
}