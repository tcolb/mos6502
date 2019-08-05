#include "nes.h"
#include "dasm.h"


void init_nes(NES* nes, CPU* cpu, PPU* ppu, APU* apu, Op* table)
{
	// init ops
	create_optable(table);
    cpu->table = table;
	// forward.. pointers
	nes->cpu = cpu;
    nes->ppu = ppu;
    nes->apu = apu;
    // back pointers
    cpu->nes = nes;
    ppu->nes = nes;
    apu->nes = nes;
}

int main(UNUSED int argc, UNUSED char * argv[])
{
	Op table[256];
	NES nes;
    CPU cpu;
    PPU ppu;
    APU apu;
    init_nes(&nes,
    		 &cpu,
    		 &ppu,
    		 &apu,
    		 table);

    u8* buffer = NULL;
    long length = 0;
    read_into_buffer(&buffer, &length, "../6502_suite.bin");
    printf("Length in bytes = %ld\n", length);

    // init system w 400kb of ram for test suite
    cpu.ram = buffer;
    cpu.PC = 0x0400;

    cpu.step(&cpu);
    //print_buffer(CPU.memory, 2015);
}