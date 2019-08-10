#ifndef UTIL_H
#define UTIL_H

#define UNUSED __attribute__((unused))
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

typedef struct nes NES;
typedef struct cpu CPU;
typedef struct ppu PPU;
typedef struct apu APU;
typedef struct cart Cart;
typedef struct operation Op;

#include "cpu.h"
#include "ppu.h"
#include "apu.h"
#include "cart.h"
#include "ops.h"
#include "nes.h"

u8 extract_msbyte(u16 addr);
u8 extract_lsbyte(u16 addr);
u16 combine_bytes(u8 msb, u8 lsb);
void print_cpu(CPU* cpu);
void print_optable(Op* table);


#endif