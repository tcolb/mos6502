#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "util.h"


typedef struct operation Op;

struct operation
{
    void (*op) (CPU*, u16);
    u16 (*addr) (CPU*);
    u8 cycles;
};

// addressing modes
u16 Accumulator(CPU*);
u16 Immediate(CPU*);
u16 Implied(CPU*);
u16 Relative(CPU*);
u16 Absolute(CPU*);
u16 Absolute_X(CPU*);
u16 Absolute_Y(CPU*);
u16 Zero_Page(CPU*);
u16 Zero_Page_X(CPU*);
u16 Zero_Page_Y(CPU*);
u16 Indirect(CPU*);
u16 Indirect_X(CPU*);
u16 Indirect_Y(CPU*);
// flag helpers
void SET_NEGATIVE(CPU*, u8);
void SET_ZERO(CPU*, u8);
// unimplemented instruction
void UNIMPLEMENTED_OP(UNUSED CPU* cpu, UNUSED u16 addr);
u16 UNIMPLEMENTED_ADDR(UNUSED CPU* cpu);
void LDA(CPU* cpu, u16 addr);
void LDX(CPU* cpu, u16 addr);
void LDY(CPU* cpu, u16 addr);
void STA(CPU* cpu, u16 addr);
void STX(CPU* cpu, u16 addr);
void STY(CPU* cpu, u16 addr);
void TAX(CPU* cpu, u16 addr);
void TAY(CPU* cpu, u16 addr);
void TSX(CPU* cpu, u16 addr);
void TXA(CPU* cpu, u16 addr);
void TXS(CPU* cpu, u16 addr);
void TYA(CPU* cpu, u16 addr);
// math
void ADC(CPU* cpu, u16 addr);
void SBC(CPU* cpu, u16 addr);
void DEC(CPU* cpu, u16 addr);
void DEX(CPU* cpu, u16 addr);
void DEY(CPU* cpu, u16 addr);
void INC(CPU* cpu, u16 addr);
void INX(CPU* cpu, u16 addr);
void INY(CPU* cpu, u16 addr);
// bitwise
void AND(CPU* cpu, u16 addr);
void ASL(CPU* cpu, u16 addr);
void LSR(CPU* cpu, u16 addr);
void BIT(CPU* cpu, u16 addr);
void EOR(CPU* cpu, u16 addr);
void ORA(CPU* cpu, u16 addr);
void ROL(CPU* cpu, u16 addr);
void ROR(CPU* cpu, u16 addr);
// branch
void BCC(CPU* cpu, u16 addr);
void BCS(CPU* cpu, u16 addr);
void BEQ(CPU* cpu, u16 addr);
void BMI(CPU* cpu, u16 addr);
void BNE(CPU* cpu, u16 addr);
void BPL(CPU* cpu, u16 addr);
void BVC(CPU* cpu, u16 addr);
void BVS(CPU* cpu, u16 addr);
// jump
void JMP(CPU* cpu, u16 addr);
void JSR(CPU* cpu, u16 addr);
void RTI(CPU* cpu, u16 addr);
void RTS(CPU* cpu, u16 addr);
// registers
void CLC(CPU* cpu, u16 addr);
void CLD(CPU* cpu, u16 addr);
void CLI(CPU* cpu, u16 addr);
void CLV(CPU* cpu, u16 addr);
void SEC(CPU* cpu, u16 addr);
void SED(CPU* cpu, u16 addr);
void SEI(CPU* cpu, u16 addr);
void CMP(CPU* cpu, u16 addr);
void CPX(CPU* cpu, u16 addr);
void CPY(CPU* cpu, u16 addr);
// stack
void PHA(CPU* cpu, u16 addr);
void PLA(CPU* cpu, u16 addr);
void PHP(CPU* cpu, u16 addr);
void PLP(CPU* cpu, u16 addr);
// system
void BRK(CPU* cpu, u16 addr);
void NOP(CPU* cpu, u16 addr);

void create_optable(Op*);


#endif