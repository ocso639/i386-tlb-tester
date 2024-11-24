#include "tlb.h"

void write_tlb(uint32_t physical, uint32_t linear, char tlb_index, char valid, char dirty, char user, char write, char pcd, char pwt, char LRU) {
    uint32_t TR7 = 0;
    uint32_t TR6 = 0;

    TR7 |= (physical & (0xFFFFF << 12));
    TR7 |= (tlb_index & 0b11) << 2;
    TR7 |= (1 << 4);
    TR7 |= (LRU & 0b111) << 7;
    TR7 |= (pwt & 0b1) << 10;
    TR7 |= (pcd & 0b1) << 11;
    TR7 |= (1 << 4);
    TR6 |= (linear & (0xFFFFF << 12));
    TR6 |= ((!write) & 0b1) << 5;
    TR6 |= (write & 0b1) << 6;
    TR6 |= ((!user) & 0b1) << 7;
    TR6 |= (user & 0b1) << 8;
    TR6 |= ((!dirty) & 0b1) << 9;
    TR6 |= (dirty & 0b1) << 10;
    TR6 |= (valid & 0b1) << 11;

    asm volatile ( "mov %0, %%tr7" : : "r"(TR7) );
    asm volatile ( "mov %0, %%tr6" : : "r"(TR6) );
}

int lookup_tlb(uint32_t linear) {
    uint32_t TR6 = 0;
    uint32_t TR7 = 0;
    asm volatile ( "mov %0, %%tr7" : : "r"(TR7) );
    TR6 |= (linear & (0xFFFFF << 12));
    TR6 |= 1;
    asm volatile ( "mov %0, %%tr6" : : "r"(TR6) );
    asm volatile ( "mov %%tr7, %0" : "=r"(TR7) );
    unsigned Hit = (TR7 >> 4) & 0b1;
    if (Hit == 1)
        return 0;
    return 1;
}