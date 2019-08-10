#include "nes.h"
#include "dasm.h"
#include <unistd.h>


void init_nes(NES* nes, CPU* cpu, PPU* ppu, APU* apu, Cart* cart, Op* table)
{
    // init hw
    init_cpu(cpu);
    //init_ppu(ppu);
    //init_apu(apu);
    init_cart(cart);

    cpu->table = table;
    cpu->ram = malloc(2048); // NES internal ram size 2kib

	// forward.. pointers
	nes->cpu = cpu;
    //nes->ppu = ppu;
    //nes->apu = apu;
    nes->cart = cart;

    // back pointers
    cpu->nes = nes;
    //ppu->nes = nes;
    //apu->nes = nes;
}

int main(UNUSED int argc, UNUSED char * argv[])
{
    // NES initialization
	Op table[0x100];
    create_optable(table);
    CPU cpu;
    Cart cart;
    NES nes;
    init_nes(&nes, &cpu, NULL, NULL, &cart, table); // ppu and apu not implemented

    // For nestest.nes
    u8* buffer = NULL;
    long length = 0;
    read_into_buffer(&buffer, &length, "../nestest.nes");
    parse_rom_to_cart(buffer, &cart);
    cpu.PC = 0xC000; // start of all tests


    // stepper
    char inp[2];
    do
    {
    	read(0, inp, 2);
    	switch (inp[0])
    	{
    		case 's': cpu.step(&cpu); break;
    		case 'e': exit(0); break;
    		default: break;
    	}
    } 
    while (1);
    //print_buffer(CPU.memory, 2015);
}