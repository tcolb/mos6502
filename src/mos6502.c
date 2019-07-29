#include <stdio.h>

#include "mos6502.h"
#include "instructions.h"
#include "disassemble.h"
#include "ines.h"


// memory operations and address decoding


uint8_t mem_read(State* st, uint16_t addr)
{
    // check if in rom space
    if (addr >= st->upper_bound)
    {
        return st->rom[addr - st->upper_bound];
    }
    return st->memory[addr];
}

void mem_write(State* st, uint16_t addr, uint8_t byte)
{
    // check if in rom memory space
    if (addr >= st->upper_bound)
    {
        exit(1); // writing to rom space
    }
    st->memory[addr] = byte;
}


// stack operations 


uint8_t stack_pop(State *st)
{
    // check for underflow then wrap up
    if (st->SP == 0xFF)
        st->SP = 0x00;

    uint8_t temp = mem_read(st, 0x0100 + st->SP);
    mem_write(st, 0x0100 + st->SP, 0x00); // or another value?
    st->SP++;
    return temp;
}


void stack_push(State *st, uint8_t byte)
{
    // check for overflow then wrap down
    if (st->SP == 0x00)
        st->SP = 0xFF;

    mem_write(st, 0x0100 + st->SP, byte);
    st->SP--;
}


// system and emulation
uint8_t extract_msbyte(uint16_t addr)
{
    return (uint8_t) (addr >> 8) & 0x00FF;
}

uint8_t extract_lsbyte(uint16_t addr)
{
    return (uint8_t) addr & 0x00FF;
}

uint16_t combine_bytes(uint8_t msb, uint8_t lsb)
{
    uint16_t out = 0;
    out |= msb;
    out <<= 8;
    out |= lsb; 
    return out;
}

void print_system(State* st)
{
    printf("A=$%02X  X=$%02X  Y=$%02X  SP=$%02X  PC=$%04X\n", st->A, st->X, st->Y, st->SP, st->PC);
    printf("NV-BDIZC\n%X%X1%X%X%X%X%X\n\n", st->flags->N, st->flags->V, st->flags->B, st->flags->D,
                                          st->flags->I, st->flags->Z, st->flags->C);
}

void init_system(State* st, Status* ss, uint16_t ram_size_bytes, uint16_t rom_size_bytes, uint8_t* rom_buffer)
{
    ss->N = 0x00;
    ss->V = 0x00;
    ss->D = 0x00;
    ss->I = 0x00;
    ss->Z = 0x00;
    ss->C = 0x00;
    ss->B = 0x00;

    st->A = 0;
    st->X = 0x00;
    st->Y = 0x00;
    st->SP = 0xFF; // end of stack space 
    st->memory = malloc(ram_size_bytes + rom_size_bytes);
    st->upper_bound = ram_size_bytes;
    st->rom = rom_buffer;
    st->PC = ram_size_bytes;
    st->flags = ss;

    st->NMI_upper = 0xFFFA;
    st->NMI_lower = 0xFFFB;
    st->RESET_upper = 0xFFFC;
    st->RESET_lower = 0xFFFD;
    st->IRQ_upper = 0xFFFE;
    st->IRQ_lower = 0xFFFF;
}

uint8_t get_processor_status(State* st)
{
    uint8_t bits[] = {
        st->flags->C,
        st->flags->Z,
        st->flags->I,
        st->flags->D,
        st->flags->B,
        1,
        st->flags->V,
        st->flags->N 
    };

    uint8_t status = 0x00;

    for (int b=0; b<8; b++)
    {
        status |= (bits[b] << b);
    }

    return status;
}

void set_processor_status(State* st, uint8_t status)
{
    uint8_t* bits[8] = {0};
    uint8_t mask = 1;

    for (int b=0; b<8; b++)
    {
        bits[b] = (status >> b) & mask;
    }

    st->flags->C = bits[0];
    st->flags->Z = bits[1];
    st->flags->I = bits[2];
    st->flags->D = bits[3];
    st->flags->B = bits[4];
    st->flags->V = bits[6];
    st->flags->N = bits[7];
}

void emulate(Instruction* optable, State* st, int cycles)
{
    for (int i=0; i<cycles; i++)
    {
        printf("Cycle: %d\n", i);
        // get opcode
        uint8_t op_num = mem_read(st, st->PC++); 
        printf("Instruction: %02X\n", op_num);
        // get instruction
        Instruction *op = &optable[op_num];
        // execute instruction
        uint16_t address_value = op->addr(st);
        printf("Mode value: %02X\n", address_value);
        op->op(st, address_value);
        print_system(st);
    }
}


int main()
{
    Status status;
    State state;
    Instruction InstructionSet[256];

    init_instruction_set(InstructionSet);

    uint8_t* buffer = NULL;
    long length = 0;
    read_into_buffer(&buffer, &length, "../test.bin");

    // init system w 2kb of ram
    init_system(&state, &status, 2048, length, buffer);
    print_buffer(state.rom, length);


    emulate(InstructionSet, &state, 6);
    print_buffer(state.memory, 2015);
}
