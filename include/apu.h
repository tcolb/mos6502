#ifndef APU_H
#define APU_H

#include "util.h"


typedef struct apu APU;

struct apu
{
	NES* nes;
    // memory
    u8* ram;
    u8 (*read) (APU*, u16);
    void (*write) (APU*, u16, u8);
};


#endif