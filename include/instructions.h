#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H


#include "mos6502.h"


// addressing modes
uint16_t ADR_Accumulator(State*);
uint16_t ADR_Immediate(State*);
uint16_t ADR_Implied(State*);
uint16_t ADR_Relative(State*);
uint16_t ADR_Absolute(State*);
uint16_t ADR_Absolute_X(State*);
uint16_t ADR_Absolute_Y(State*);
uint16_t ADR_Zero_Page(State*);
uint16_t ADR_Zero_Page_X(State*);
uint16_t ADR_Zero_Page_Y(State*);
uint16_t ADR_Indirect(State*);
uint16_t ADR_Indirect_X(State*);
uint16_t ADR_Indirect_Y(State*);

// flag helpers
void SET_NEGATIVE(State*, uint8_t);
void SET_ZERO(State*, uint8_t);

// unimplemented instruction
void OP_UNIMPLEMENTED(UNUSED State*, UNUSED uint16_t);

// storage 
void OP_LDA(State* st, uint16_t addr);
void OP_LDX(State* st, uint16_t addr);
void OP_LDY(State* st, uint16_t addr);
void OP_STA(State* st, uint16_t addr);
void OP_STX(State* st, uint16_t addr);
void OP_STY(State* st, uint16_t addr);
void OP_TAX(State* st, uint16_t addr);
void OP_TAY(State* st, uint16_t addr);
void OP_TSX(State* st, uint16_t addr);
void OP_TXA(State* st, uint16_t addr);
void OP_TXS(State* st, uint16_t addr);
void OP_TYA(State* st, uint16_t addr);

// math
void OP_ADC(State* st, uint16_t addr);
void OP_SBC(State* st, uint16_t addr);
void OP_DEC(State* st, uint16_t addr);
void OP_DEX(State* st, uint16_t addr);
void OP_DEY(State* st, uint16_t addr);
void OP_INC(State* st, uint16_t addr);
void OP_INX(State* st, uint16_t addr);
void OP_INY(State* st, uint16_t addr);

// bitwise
void OP_AND(State *st, uint16_t addr);
void OP_ASL(State *st, uint16_t addr);
void OP_LSR(State *st, uint16_t addr);
void OP_BIT(State *st, uint16_t addr);
void OP_EOR(State *st, uint16_t addr);
void OP_ORA(State *st, uint16_t addr);
void OP_ROL(State *st, uint16_t addr);
void OP_ROR(State *st, uint16_t addr);

// branch
void OP_BCC(State*st, uint16_t addr);
void OP_BCS(State*st, uint16_t addr);
void OP_BEQ(State*st, uint16_t addr);
void OP_BMI(State*st, uint16_t addr);
void OP_BNE(State*st, uint16_t addr);
void OP_BPL(State*st, uint16_t addr);
void OP_BVC(State*st, uint16_t addr);
void OP_BVS(State*st, uint16_t addr);

// jump
void OP_JMP(State* st, uint16_t addr);
void OP_JSR(State* st, uint16_t addr);
void OP_RTI(State* st, uint16_t addr);
void OP_RTS(State* st, uint16_t addr);

// registers
void OP_CLC(State* st, uint16_t addr);
void OP_CLD(State* st, uint16_t addr);
void OP_CLI(State* st, uint16_t addr);
void OP_CLV(State* st, uint16_t addr);
void OP_SEC(State* st, uint16_t addr);
void OP_SED(State* st, uint16_t addr);
void OP_SEI(State* st, uint16_t addr);
void OP_CMP(State* st, uint16_t addr);
void OP_CPX(State* st, uint16_t addr);
void OP_CPY(State* st, uint16_t addr);

// stack
void OP_PHA(State* st, uint16_t addr);
void OP_PLA(State* st, uint16_t addr);
void OP_PHP(State* st, uint16_t addr);
void OP_PLP(State* st, uint16_t addr);

// system
void OP_BRK(State* st, uint16_t addr);
void OP_NOP(State* st, uint16_t addr);

void init_instruction_set(Instruction*);
void print_instructions(Instruction*);


#endif