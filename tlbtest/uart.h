#pragma once
#include <stdint.h>

#define COM1    0x3f8
#define COM2    0x2f8
#define COM3    0x3e8
#define COM4    0x2e8

void uart_setport(uint16_t port);
void uart_clear(void);
void uart_wait(void);
void uart_outc(char c);
void uart_outs(const char* s);
void uart_outf(const char* fmt, ...);
void reboot_prompt(void);