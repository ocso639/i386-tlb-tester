#pragma once
#include <stdint.h>

void write_tlb(uint32_t physical, uint32_t linear, char tlb_index, char valid, char dirty, char user, char write, char pcd, char pwt, char LRU);
int lookup_tlb(uint32_t linear);