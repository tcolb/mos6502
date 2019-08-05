#include "util.h"
#include <stdio.h>

u8 extract_msbyte(u16 addr)
{
    return (u8) (addr >> 8) & 0x00FF;
}

u8 extract_lsbyte(u16 addr)
{
    return (u8) addr & 0x00FF;
}

u16 combine_bytes(u8 msb, u8 lsb)
{
    u16 out = 0;
    out |= msb;
    out <<= 8;
    out |= lsb; 
    return out;
}

void print_cpu(CPU* cpu)
{
    printf("A=$%02X  X=$%02X  Y=$%02X  SP=$%02X  PC=$%04X\nNV-BDIZC\n%X%X1%X%X%X%X%X\n\n",
    cpu->A, cpu->X, cpu->Y, cpu->SP, cpu->PC,
    cpu->N, cpu->V, cpu->B, cpu->D, cpu->I, cpu->Z, cpu->C);
}

void print_optable(Op* table)
{
    for (int i = 0; i < 256; i++)
        printf("0x%02X : Op %p : Mode %p\n", i, table[i].op, table[i].addr);
}