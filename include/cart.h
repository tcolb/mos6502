#ifndef CART_H
#define CART_H

#include "util.h"


typedef struct cart Cart;

struct cart
{
	// mapper nrom 0000
    // memory
    u8* prg_rom_hi;
    u8* prg_rom_lo;
    u8* chr_rom;
    u8* prg_ram;
    u8* chr_ram;
    u8 (*read) (Cart*, u16);
    void (*write) (Cart*, u16, u8);
};


void parse_rom(u8*, Cart*);
void print_ines_header(u8* buffer);
void init_cart(Cart* cart);


#endif