#include "cart.h"
#include <string.h>

void parse_rom_to_cart(u8* buffer, Cart* cart)
{
	u8 header[16];

	// parse header
	for (int i = 0; i < 16; i++)
        header[i] = buffer[i];
    // check for constant
    if (header[0] != 0x4E || header[1] != 0x45 || header[2] != 0x53 || header[3] != 0x1A)
    	return; // not an ines or nes2.0 rom

    // mapper
    u8 MAPPER_num  =  header[6]       & 0xF0; // check this
       MAPPER_num |= (header[7] >> 4) & 0x0F;

    // parse flags
    u16 PRG_ROM_size = header[4] * 16384;
    u16 CHR_ROM_size = header[5] *  8192;
    u8 FOUR_SCREEN = (header[6] >> 3) & 0x01;
    u8 TRAINER = 	 (header[6] >> 2) & 0x01;
    u8 BATTERY = 	 (header[6] >> 1) & 0x01;
    u8 NT_MIRROR =    header[6]       & 0x01;
    u8 NES2 =        (header[7] >> 2) & 0x03;
    u8 CONSOLE_TYPE = header[7]       & 0x03;

    // nes2 vs ines
    if (NES2 == 2)
    {
    	// do dat nes2.0
    } else
    {
    	// ines / archaeic
    	// create cart roms for mapper nrom 0000
    	if (PRG_ROM_size > 0x4000)
    	{
    		cart->prg_rom_lo = malloc(0x4000);
    		cart->prg_rom_hi = malloc(PRG_ROM_size - 0x4000);
    		memcpy(cart->prg_rom_lo, buffer+16, 0x4000);
    		memcpy(cart->prg_rom_hi, buffer+16+0x4000, PRG_ROM_size - 0x4000);
    	} else
    	{
    		cart->prg_rom_lo = malloc(PRG_ROM_size);
    		cart->prg_rom_hi = cart->prg_rom_lo;
    		memcpy(cart->prg_rom_lo, buffer+16, PRG_ROM_size);
    	}

    	//cart->chr_rom = malloc(CHR_ROM_size);
    	//memcpu(cart->chr_rom, buffer+16+PRG_ROM_size, CHR_ROM_size);
    }
}

u8 _cart_read(Cart* cart, u16 addr)
{
	// this is for NROM mapper 0000
	switch(addr)
	{
		case 0x0000 ... 0x5FFF: printf("PRG ROM Read: ERROR Out of Range\n"); exit(1);
		case 0x6000 ... 0x7FFF: return 0; // PRG Ram on Family Basic
		case 0x8000 ... 0xBFFF: return cart->prg_rom_lo[addr - 0x8000]; // PRG Rom lower 16kib
		case 0xC000 ... 0xFFFF: return cart->prg_rom_hi[addr - 0xC000]; // PRG Rom upper 16kib
		default: exit(1); // out of range
	}
}

void _cart_write(Cart* cart, u16 addr, u8 byte)
{
	// this is for NROM mapper 0000
	switch(addr)
	{
		case 0x0000 ... 0x5FFF: printf("PRG ROM Write: ERROR Out of Rrange\n"); exit(1); break;
		case 0x6000 ... 0x7FFF: printf("Unimplemented PRG RAM"); break; // PRG Ram on Family Basic
		case 0x8000 ... 0xFFFF: printf("PRG ROM Write: ERROR Read Only\n"); exit(1); break; // PRG Rom
		default: exit(1); // out of range
	}
}

void print_ines_header(u8* buffer)
{
    printf("################ iNES Header info #################\n");
    for (int i = 0; i < 16; i++)
        printf("%X ", buffer[i]);
    printf("\n");

    printf("PRG ROM (CPU Chip) Size: %X KB\n", buffer[4]);
    printf("CHR ROM (PPU Chip) Size: %X KB\n", buffer[5]);
    if (buffer[5] == 0)
        printf("No CHR ROM, uses CHR RAM\n");
    printf("Mapper, mirroring, battery, trainer: %X\n", buffer[6]);
    printf("Mapper, VS/Playchoice, NES 2.0: %X\n", buffer[7]);
    printf("PRG RAM Size: %X\n", buffer[8]);
    printf("TV system: %X\n", buffer[9]);
    printf("TV system, PRG-RAM presence (unofficial): %X\n", buffer[10]);
}

void init_cart(Cart* cart)
{
	cart->prg_rom_hi = NULL;
	cart->prg_rom_lo = NULL;
    cart->chr_rom = NULL;
    cart->prg_ram = NULL;
    cart->chr_ram = NULL;
	cart->read = _cart_read;
	cart->write = _cart_write;
}