#include <stdio.h>

#include "cpu.h"



// stack operations 


u8 _stack_pop(CPU *cpu)
{
    // check for underflow then wrap up
    if (cpu->SP == 0xFF)
        cpu->SP = 0x00;

    u8 temp = cpu->read(cpu, 0x0100 + cpu->SP);
    cpu->write(cpu, 0x0100 + cpu->SP, 0x00); // or another value?
    cpu->SP++;
    return temp;
}


void _stack_push(CPU *cpu, u8 byte)
{
    // check for overflow then wrap down
    if (cpu->SP == 0x00)
        cpu->SP = 0xFF;

    cpu->write(cpu, 0x0100 + cpu->SP, byte);
    cpu->SP--;
}


// interrupts


void _INTERRUPT(CPU* cpu)
{
    u8 PC_up = extract_msbyte(cpu->PC);
    u8 PC_lo = extract_lsbyte(cpu->PC);
    cpu->stack_push(cpu, PC_up);
    cpu->stack_push(cpu, PC_lo);
    cpu->stack_push(cpu, cpu->get_status(cpu));
    cpu->I = 0;
}

void _NMI(CPU* cpu)
{
    _INTERRUPT(cpu);
    cpu->PC = combine_bytes(cpu->read(cpu, cpu->NMI), cpu->read(cpu, cpu->NMI+1));
}

void _IRQ(CPU* cpu)
{
    _INTERRUPT(cpu);
    cpu->PC = combine_bytes(cpu->read(cpu, cpu->IRQ), cpu->read(cpu, cpu->IRQ+1));
}

void _RESET(CPU* cpu)
{
    cpu->PC = combine_bytes(cpu->read(cpu, cpu->RESET), cpu->read(cpu, cpu->RESET+1));
}



u8 _get_status(CPU* cpu)
{
    u8 bits[] = {
        cpu->C,
        cpu->Z,
        cpu->I,
        cpu->D,
        cpu->B,
        1,
        cpu->V,
        cpu->N 
    };

    u8 status = 0x00;

    for (int b=0; b<8; b++)
    {
        status |= (bits[b] << b);
    }

    return status;
}

void _set_status(CPU* cpu, u8 status)
{
    u8 bits[8] = {0};
    u8 mask = 1;

    for (int b=0; b<8; b++)
    {
        bits[b] = (status >> b) & mask;
    }

    cpu->C = bits[0];
    cpu->Z = bits[1];
    cpu->I = bits[2];
    cpu->D = bits[3];
    cpu->B = bits[4];
    cpu->V = bits[6];
    cpu->N = bits[7];
}

void _cpu_step(CPU* this)
{
    // fetch
    u8 opcode = this->read(this, this->PC++); 

    // decode
    Op* op = &this->table[opcode];

    // fetch address
    u16 addr = op->addr(this);

    // execute
    op->op(this, addr);

    // timing
    this->time += op->cycles;

    // debug
    printf("Op: %02X\n", opcode);
    printf("Mode value: %02X\n", addr);
    print_cpu(this);
}


// cpu memory operations


u8 _cpu_read(CPU* cpu, u16 addr)
{
    NES* nes = cpu->nes;

    switch (addr)
    {
        case 0x0000 ... 0x1FFF: return nes->cpu->read(nes->cpu, addr);   break;   // CPU
        case 0x2000 ... 0x3FFF: return nes->ppu->read(nes->ppu, addr);   break;   // PPU
        case 0x4000 ... 0x4013: return nes->apu->read(nes->apu, addr);   break;   // APU
        case 0x4014           : return nes->ppu->read(nes->ppu, addr);   break;   // PPU
        case 0x4015           : return nes->apu->read(nes->apu, addr);   break;   // APU
        case 0x4016           : return 0;                                break;   // JOY1
        case 0x4017           : return 0;                                break;   // JOY2
        case 0x4018 ... 0x401F: return 0;                                break;   // Unused
        case 0x4020 ... 0xFFFF: return nes->cart->read(nes->cart, addr); break;   // Cartridge
        default:                                                         exit(1); // out of range
    }
}

void _cpu_write(CPU* cpu, u16 addr, u8 byte)
{
    NES* nes = cpu->nes;

    switch (addr)
    {
        case 0x0000 ... 0x1FFF: nes->cpu->write(nes->cpu, addr, byte);   break;   // CPU
        case 0x2000 ... 0x3FFF: nes->ppu->write(nes->ppu, addr, byte);   break;   // PPU
        case 0x4000 ... 0x4013: nes->apu->write(nes->apu, addr, byte);   break;   // APU
        case 0x4014           : nes->ppu->write(nes->ppu, addr, byte);   break;   // PPU
        case 0x4015           : nes->apu->write(nes->apu, addr, byte);   break;   // APU
        case 0x4016           :                                          break;   // JOY1
        case 0x4017           :                                          break;   // JOY2
        case 0x4018 ... 0x401F:                                          break;   // Unused
        case 0x4020 ... 0xFFFF: nes->cart->write(nes->cart, addr, byte); break;   // Cartridge
        default:                                                         exit(1); // out of range
    }
}


// cpu constructor


void init_cpu(CPU* this)
{
    this->A = this->X = this->Y = 0;
    this->N = this->V = this->D = this->B = this->I = this->Z = this->C = 0;
    this->SP = this->PC = 0x00;
    this->time = 0;
    this->ram = NULL;
    this->table = NULL;
    this->nes = NULL;

    this->read = _cpu_read;
    this->write = _cpu_write;
    this->step = _cpu_step;
    this->stack_pop = _stack_pop;
    this->stack_push = _stack_push;
    this->get_status = _get_status;
    this->set_status = _set_status;
    this->nmi = _NMI;
    this->irq = _IRQ;
    this->reset = _RESET;
}