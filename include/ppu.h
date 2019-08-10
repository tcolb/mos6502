#ifndef PPU_H
#define PPU_H

#include "util.h"


typedef struct ppu PPU;

struct ppu
{
	NES* nes;
    // memory
    u8* ram;
    u8 (*read) (PPU*, u16);
    void (*write) (PPU*, u16, u8);
};


#endif