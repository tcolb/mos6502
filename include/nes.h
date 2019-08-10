#ifndef NES_H
#define NES_H

#include "util.h"


typedef struct nes
{
	CPU* cpu;
	PPU* ppu;
	APU* apu;
	Cart* cart;
} NES;

void init_nes(NES* nes, CPU* cpu, PPU* ppu, APU* apu, Cart*, Op*);

#endif