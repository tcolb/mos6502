#ifndef CPU_H
#define CPU_H

#include "util.h"
#include "nes.h"


typedef struct cpu CPU;

struct cpu
{
    // registers
    u8 A; // accumulator
    u8 X; // index x
    u8 Y; // index y
    // stack pointer
    u8 SP;
    // program counter
    u16 PC;
    // flags
    u8 N: 1; // neg flag
    u8 V: 1; // overflow flag 
    u8 D: 1; // decimal flag
    u8 B: 1; // break flag
    u8 I: 1; // interrupt flag 
    u8 Z: 1; // zero flag
    u8 C: 1; // carry flag
    // vectors
    u16 NMI;
    u16 RESET;
    u16 IRQ;
    // timing
    u32 time;
    // optable
    Op* table;
    // memory
    NES* nes;
    u8* ram;
    // functions
    u8 (*read) (CPU*, u16);
    void (*write) (CPU*, u16, u8);
    void (*step) (CPU*);
    u8 (*stack_pop) (CPU*);
    void (*stack_push) (CPU*, u8);
    u8 (*get_status) (CPU*);
    void (*set_status) (CPU*, u8);
    void (*nmi) (CPU* cpu);
    void (*irq) (CPU* cpu);
    void (*reset) (CPU* cpu);
};


// interrupts
void _INTERRUPT(CPU* cpu);
void _NMI(CPU* cpu);
void _IRQ(CPU* cpu);
void _RESET(CPU* cpu);
// stack operations
u8   _stack_pop(CPU*);
void _stack_push(CPU*, u8);
// cpu operations
u8   _get_status(CPU*);
void _set_status(CPU*, u8);
u8   _cpu_read(CPU* cpu, u16 addr);
void _cpu_write(CPU* cpu, u16 addr, u8 byte);
void _cpu_step(CPU*);

void init_cpu(CPU*);


#endif