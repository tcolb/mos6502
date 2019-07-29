#ifndef CPU_H
#define CPU_H


#define UNUSED __attribute__((unused))

#include <stdlib.h>


typedef struct status_6502 Status;
typedef struct state_6502 State;
typedef struct instruction_6502 Instruction;


struct status_6502
{
    uint8_t N: 1; // neg flag
    uint8_t V: 1; // overflow flag 
    uint8_t D: 1; // decimal flag
    uint8_t B: 1; // break flag
    uint8_t I: 1; // interrupt flag 
    uint8_t Z: 1; // zero flag
    uint8_t C: 1; // carry flag
};

struct state_6502
{
    // flag bits
    Status *flags;

    // general purpose registers
    uint8_t A; // accumulator
    uint8_t X; // index x
    uint8_t Y; // index y

    // stack pointer
    uint8_t SP;

    // program counter
    uint16_t PC;

    // internal memory
    uint8_t *memory;

    // internal memory size, for address decoding
    uint16_t upper_bound;

    // read only (program) memory
    uint8_t *rom;

    // vectors
    uint16_t NMI_upper;
    uint16_t NMI_lower;
    uint16_t RESET_upper;
    uint16_t RESET_lower;
    uint16_t IRQ_upper;
    uint16_t IRQ_lower;
};

struct instruction_6502
{
    void (*op) (State*, uint16_t);
    uint16_t (*addr) (State*);
};

// memory operations
uint8_t mem_read(State*, uint16_t);
void mem_write(State*, uint16_t, uint8_t);

// stack operations
uint8_t stack_pop(State*);
void stack_push(State*, uint8_t);

uint16_t combine_bytes(uint8_t, uint8_t);
void print_system(State*);
void init_system(State*, Status*, uint16_t, uint16_t, uint8_t*);
uint8_t get_processor_status(State*);
void set_processor_status(State*, uint8_t);

void emulate(Instruction*, State*, int);


#endif