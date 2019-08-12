#include "ops.h"
#include <stdio.h>


// addressing mode getters, returns an addr


u16 Accumulator(UNUSED CPU* cpu)
{
	return 0; // TODO create a fixed value for accumulator
}

u16 Immediate(CPU* cpu)
{
	return cpu->PC++;
}

u16 Implied(UNUSED CPU* cpu)
{
	return (u16) 0; // unused
}

u16 Relative(CPU* cpu) 
{
	int16_t offset = cpu->read(cpu, cpu->PC++);
    // perform a negative check
	return cpu->PC + offset;
}

u16 Absolute(CPU* cpu)
{
	u16 lo = cpu->read(cpu, cpu->PC++);
	u16 up = cpu->read(cpu, cpu->PC++);
	return (up<<8) | lo;
}

u16 Absolute_X(CPU* cpu)
{
	u16 lo = cpu->read(cpu, cpu->PC++);
	u16 up = cpu->read(cpu, cpu->PC++);
	return ((up<<8) | lo) + cpu->X;
}

u16 Absolute_Y(CPU* cpu)
{
	u16 lo = cpu->read(cpu, cpu->PC++);
	u16 up = cpu->read(cpu, cpu->PC++);
	return ((up<<8) | lo) + cpu->Y;
}

u16 Zero_Page(CPU* cpu)
{
	return cpu->read(cpu, cpu->PC++);
}

u16 Zero_Page_X(CPU* cpu)
{
	return (cpu->read(cpu, cpu->PC++) + cpu->X) % 256;
}

u16 Zero_Page_Y(CPU* cpu)
{
	return (cpu->read(cpu, cpu->PC++) + cpu->Y) % 256;
}

u16 Indirect(CPU* cpu)
{
	u16 lo = cpu->read(cpu, cpu->PC++);
	u16 up = cpu->read(cpu, cpu->PC++);
	u16 addr = (up<<8) | lo;
	u16 eLo = cpu->read(cpu, addr);
	u16 eUp = cpu->read(cpu, addr+1);
	return (eUp<<8) | eLo;
}

u16 Indirect_X(CPU* cpu)
{
	u16 addr = (cpu->read(cpu, cpu->PC++) + cpu->X) % 256;
	u16 eLo = cpu->read(cpu, addr);
	u16 eUp = cpu->read(cpu, addr+1);
	return (eUp<<8) | eLo;
}

u16 Indirect_Y(CPU* cpu)
{
	u16 addr = cpu->read(cpu, cpu->PC++) % 256; 
	u16 eLo = cpu->read(cpu, addr);
	u16 eUp = cpu->read(cpu, addr+1);
	return ((eUp<<8) | eLo) + cpu->Y;
}


// flag helpers


void SET_NEGATIVE(CPU* cpu, u8 val)
{
    cpu->N = (val & 0x80) ? 1 : 0;
}

void SET_ZERO(CPU* cpu, u8 val)
{
    cpu->Z = (val) ? 0 : 1;
}

void SET_CARRY_UPPER(CPU* cpu, u8 val)
{
    cpu->C = (val >> 7) & 1;
}

void SET_CARRY_LOWER(CPU* cpu, u8 val)
{
    cpu->C = val & 1;
}


// unimplemented operation


void UNIMPLEMENTED_OP(UNUSED CPU* cpu, UNUSED u16 addr)
{
	printf("Error: Unimplemented Instruction Emulated\n");
	exit(1);
}

u16 UNIMPLEMENTED_ADDR(UNUSED CPU* cpu)
{
    printf("Error: Unimplemented Instruction Emulated\n");
    exit(1);
}


// storage 


void LDA(CPU* cpu, u16 addr)
{
    u8 src = cpu->read(cpu, addr);
    SET_NEGATIVE(cpu, src);
    SET_ZERO(cpu, src);
	// load A with M
	cpu->A = src;
}

void LDX(CPU* cpu, u16 addr)
{
    u8 src = cpu->read(cpu, addr);
    SET_NEGATIVE(cpu, src);
    SET_ZERO(cpu, src);
    // load X with M
    cpu->X = src;
}

void LDY(CPU* cpu, u16 addr)
{
    u8 src = cpu->read(cpu, addr);
    SET_NEGATIVE(cpu, src);
    SET_ZERO(cpu, src);
    // load Y with M
    cpu->Y = src;
}

void STA(CPU* cpu, u16 addr)
{
	// store A in M
	cpu->write(cpu, addr, cpu->A);
}

void STX(CPU* cpu, u16 addr)
{
    // store X in M
    cpu->write(cpu, addr, cpu->X);
}

void STY(CPU* cpu, u16 addr)
{
    // store Y in M
    cpu->write(cpu, addr, cpu->Y);
}

void TAX(CPU* cpu, UNUSED u16 addr)
{
    u8 src = cpu->A;
    SET_NEGATIVE(cpu, src);
    SET_ZERO(cpu, src);
    // transfer A to X
    cpu->X = src;
}

void TAY(CPU* cpu, UNUSED u16 addr)
{
    u8 src = cpu->A;
    SET_NEGATIVE(cpu, src);
    SET_ZERO(cpu, src);
    // transfer A to Y
    cpu->Y = src;
}

void TSX(CPU* cpu, UNUSED u16 addr)
{
    u8 src = cpu->SP;
    SET_NEGATIVE(cpu, src);
    SET_ZERO(cpu, src);
    // transfer SP to X
    cpu->X = src;
}

void TXA(CPU* cpu, UNUSED u16 addr)
{
    u8 src = cpu->X;
    SET_NEGATIVE(cpu, src);
    SET_ZERO(cpu, src);
    // transfer X to A
    cpu->A = src;
}

void TXS(CPU* cpu, UNUSED u16 addr)
{
    u8 src = cpu->X;
    SET_NEGATIVE(cpu, src);
    SET_ZERO(cpu, src);
    // transfer X to SP
    cpu->SP = src;
}

void TYA(CPU* cpu, UNUSED u16 addr)
{
    u8 src = cpu->Y;
    SET_NEGATIVE(cpu, src);
    SET_ZERO(cpu, src);
    // transfer Y to A
    cpu->A = src;
}


// math


void ADC(CPU* cpu, u16 addr)
{
    u8 M = cpu->read(cpu, addr);
    u8 result = cpu->A + M + cpu->C;
    SET_CARRY_UPPER(cpu, result);
    SET_ZERO(cpu, cpu->A);
    // set overflow
    if ( ~(M ^ cpu->A) & (M ^ result) )
        cpu->V = (result >> 7) & 0x1;
    SET_NEGATIVE(cpu, result);
    cpu->A = result;
}

void SBC(CPU* cpu, u16 addr)
{
    u8 M = cpu->read(cpu, addr);
    u8 result = cpu->A - M - (1-cpu->C);
    SET_CARRY_UPPER(cpu, result);
    SET_ZERO(cpu, cpu->A);
    // set overflow
    if ( ~(M ^ cpu->A) & (M ^ result) )
        cpu->V = (result >> 7) & 0x1;
    SET_NEGATIVE(cpu, result);
    cpu->A = result;
}

void DEC(CPU* cpu, u16 addr)
{
    u8 src = cpu->read(cpu, addr) - 1;
    SET_ZERO(cpu, src);
    SET_NEGATIVE(cpu, src);
}

void DEX(CPU* cpu, UNUSED u16 addr)
{
    cpu->X -= 1;
    SET_ZERO(cpu, cpu->X);
    SET_NEGATIVE(cpu, cpu->X);
}

void DEY(CPU* cpu, UNUSED u16 addr)
{
    cpu->Y -= 1;
    SET_ZERO(cpu, cpu->Y);
    SET_NEGATIVE(cpu, cpu->Y);
}

void INC(CPU* cpu, u16 addr)
{
    u8 src = cpu->read(cpu, addr) + 1;
    SET_ZERO(cpu, src);
    SET_NEGATIVE(cpu, src);
}

void INX(CPU* cpu, UNUSED u16 addr)
{
    cpu->X += 1;
    SET_ZERO(cpu, cpu->X);
    SET_NEGATIVE(cpu, cpu->X);
}

void INY(CPU* cpu, UNUSED u16 addr)
{
    cpu->Y += 1;
    SET_ZERO(cpu, cpu->Y);
    SET_NEGATIVE(cpu, cpu->Y);
}


// bitwise


void AND(CPU* cpu, u16 addr)
{
    // "AND" M with A
    u8 M = cpu->A & cpu->read(cpu, addr);
    SET_NEGATIVE(cpu, M);
    SET_ZERO(cpu, M);
    cpu->A = M;
}

void ASL(CPU* cpu, u16 addr)
{
    u8 src = cpu->read(cpu, addr);
    SET_ZERO(cpu, src);
    SET_CARRY_UPPER(cpu, src);
    src <<= 1;
    SET_NEGATIVE(cpu, src);
    cpu->write(cpu, addr, src);
}

void LSR(CPU* cpu, u16 addr)
{
    u8 src = cpu->read(cpu, addr);
    SET_ZERO(cpu, src);
    SET_CARRY_LOWER(cpu, src); // fix this
    src >>= 1;
    SET_NEGATIVE(cpu, src);
    cpu->write(cpu, addr, src);
}

void BIT(CPU* cpu, u16 addr)
{
    u8 src = cpu->read(cpu, addr);
    src &= cpu->A;
    SET_ZERO(cpu, src);
    SET_NEGATIVE(cpu, src);
    cpu->V = (src >> 6) & 0x1; // set overflow flag to bit 6
}

void EOR(CPU* cpu, u16 addr)
{
    // "XOR" M with A
    u8 M = cpu->A ^ cpu->read(cpu, addr);
    SET_NEGATIVE(cpu, M);
    SET_ZERO(cpu, M);
    cpu->A = M; 
}


void ORA(CPU* cpu, u16 addr)
{
    // "OR" M with A
	u8 M = cpu->A | cpu->read(cpu, addr);
    SET_NEGATIVE(cpu, M);
    SET_ZERO(cpu, M);
    cpu->A = M;
}

void ROL(CPU* cpu, u16 addr)
{
    u8 src = cpu->read(cpu, addr);
    SET_ZERO(cpu, src);
    SET_CARRY_UPPER(cpu, src);
    src <<= 1;
    src |= cpu->C;
    SET_NEGATIVE(cpu, src);
    cpu->write(cpu, addr, src);
}

void ROR(CPU* cpu, u16 addr)
{
    u8 src = cpu->read(cpu, addr);
    SET_ZERO(cpu, src);
    SET_CARRY_LOWER(cpu, src);
    src >>= 1;
    src |= (cpu->C << 7);
    SET_NEGATIVE(cpu, src);
    cpu->write(cpu, addr, src);
}


// branch


void BCC(CPU* cpu, u16 addr)
{
    if (!cpu->C)
        cpu->PC = addr;
}

void BCS(CPU* cpu, u16 addr)
{
    if (cpu->C)
        cpu->PC = addr;
}

void BEQ(CPU* cpu, u16 addr)
{
    if (cpu->Z)
        cpu->PC = addr;
}

void BMI(CPU* cpu, u16 addr)
{
    if (cpu->N)
        cpu->PC = addr;
}

void BNE(CPU* cpu, u16 addr)
{
    if (!cpu->Z)
        cpu->PC = addr;
}

void BPL(CPU* cpu, u16 addr)
{
    if (!cpu->N)
        cpu->PC = addr;
}

void BVC(CPU* cpu, u16 addr)
{
    if (!cpu->V)
        cpu->PC = addr;
}

void BVS(CPU* cpu, u16 addr)
{
    if (cpu->V)
        cpu->PC = addr;
}


// jump


void JMP(CPU* cpu, u16 addr)
{
    cpu->PC = addr;
}

void JSR(CPU* cpu, u16 addr)
{
    u16 ret_addr = cpu->PC; // how much to subtract, maybe none?
    printf("!!! JSR Debug: Upper = %02X  Lower = %02X\n", extract_msbyte(ret_addr), extract_lsbyte(ret_addr));
    cpu->stack_push(cpu, extract_msbyte(ret_addr));
    cpu->stack_push(cpu, extract_lsbyte(ret_addr));
    cpu->PC = addr;
}

void RTI(CPU* cpu, UNUSED u16 addr)
{
    u8 status = cpu->stack_pop(cpu);
    u8 PC_lo = cpu->stack_pop(cpu);
    u8 PC_up = cpu->stack_pop(cpu);
    cpu->set_status(cpu, status);
    cpu->PC = combine_bytes(PC_up, PC_lo);
}

void RTS(CPU* cpu, UNUSED u16 addr)
{
    u8 PC_lo = cpu->stack_pop(cpu);
    u8 PC_up = cpu->stack_pop(cpu);
    cpu->PC = combine_bytes(PC_up, PC_lo); // maybe subtract one?
}


// registers


void CLC(CPU* cpu, UNUSED u16 addr)
{
    cpu->C = 0;
}

void CLD(CPU* cpu, UNUSED u16 addr)
{
    cpu->D = 0;
}

void CLI(CPU* cpu, UNUSED u16 addr)
{
    cpu->I = 0;
}

void CLV(CPU* cpu, UNUSED u16 addr)
{
    cpu->V = 0;
}

void SEC(CPU* cpu, UNUSED u16 addr)
{
    cpu->C = 1;
}

void SED(CPU* cpu, UNUSED u16 addr)
{
    cpu->D = 1;
}

void SEI(CPU* cpu, UNUSED u16 addr)
{
    cpu->I = 1;
}

void CMP(CPU* cpu, u16 addr)
{
    u8 src = cpu->read(cpu, addr);
    if (cpu->A > src)
        cpu->C = 1;
    src = cpu->A - src;
    SET_ZERO(cpu, src);
    SET_NEGATIVE(cpu, src);
}

void CPX(CPU* cpu, u16 addr)
{
    u8 src = cpu->read(cpu, addr);
    if (cpu->X > src)
        cpu->C = 1;
    src = cpu->X - src;
    SET_ZERO(cpu, src);
    SET_NEGATIVE(cpu, src);
}

void CPY(CPU* cpu, u16 addr)
{
    u8 src = cpu->read(cpu, addr);
    if (cpu->Y > src)
        cpu->C = 1;
    src = cpu->Y - src;
    SET_ZERO(cpu, src);
    SET_NEGATIVE(cpu, src);
}


// stack


void PHA(CPU* cpu, UNUSED u16 addr)
{
    cpu->stack_push(cpu, cpu->A);
}

void PLA(CPU* cpu, UNUSED u16 addr)
{
    cpu->A = cpu->stack_pop(cpu);
}

void PHP(CPU* cpu, UNUSED u16 addr)
{
    u8 status = cpu->get_status(cpu);
    cpu->stack_push(cpu, status);
}

void PLP(CPU* cpu, UNUSED u16 addr)
{
    u8 status = cpu->stack_pop(cpu);
    cpu->set_status(cpu, status);
}


// system


void BRK(CPU* cpu, UNUSED u16 addr)
{
    cpu->B = 1;
    _INTERRUPT(cpu);
}

void NOP(UNUSED CPU* cpu, UNUSED u16 addr)
{
    // does nothing
}


// populate fetch table


void create_optable(Op* table)
{
	Op op;

    // set unimplemented / illegal ops
	op.op = UNIMPLEMENTED_OP;
	op.addr = UNIMPLEMENTED_ADDR;
    op.cycles = 1;
    op.bytes = 1;

	for (int i = 0; i <= 0xFF; i++) {
		table[i] = op;
	}

	// implement ops
    // addressing mode   | n-bytes     | cycles.      | byte code

    // adc
    op.op = ADC;
    op.addr = Immediate;   op.bytes = 2; op.cycles = 2; table[0x69] = op;
    op.addr = Zero_Page;   op.bytes = 2; op.cycles = 3; table[0x65] = op;
    op.addr = Zero_Page_X; op.bytes = 2; op.cycles = 4; table[0x75] = op;
    op.addr = Absolute;    op.bytes = 3; op.cycles = 4; table[0x6D] = op;
    op.addr = Absolute_X;  op.bytes = 3; op.cycles = 4; table[0x7D] = op;
    op.addr = Absolute_Y;  op.bytes = 3; op.cycles = 4; table[0x79] = op;
    op.addr = Indirect_X;  op.bytes = 2; op.cycles = 6; table[0x61] = op;
    op.addr = Indirect_Y;  op.bytes = 2; op.cycles = 5; table[0x71] = op;
    // and
    op.op = AND;    
    op.addr = Immediate;   op.bytes = 2; op.cycles = 2; table[0x29] = op;
    op.addr = Zero_Page;   op.bytes = 2; op.cycles = 3; table[0x25] = op;
    op.addr = Zero_Page_X; op.bytes = 2; op.cycles = 4; table[0x35] = op;
    op.addr = Absolute;    op.bytes = 3; op.cycles = 4; table[0x2D] = op;
    op.addr = Absolute_X;  op.bytes = 3; op.cycles = 4; table[0x3D] = op;
    op.addr = Absolute_Y;  op.bytes = 3; op.cycles = 4; table[0x39] = op;
    op.addr = Indirect_X;  op.bytes = 2; op.cycles = 6; table[0x21] = op;
    op.addr = Indirect_Y;  op.bytes = 2; op.cycles = 5; table[0x31] = op;
    // asl
    op.op = ASL;    
    op.addr = Accumulator; op.bytes = 1; op.cycles = 2; table[0x0A] = op;
    op.addr = Zero_Page;   op.bytes = 2; op.cycles = 5; table[0x06] = op;
    op.addr = Zero_Page_X; op.bytes = 2; op.cycles = 6; table[0x16] = op;
    op.addr = Absolute;    op.bytes = 3; op.cycles = 6; table[0x0E] = op;
    op.addr = Absolute_X;  op.bytes = 3; op.cycles = 7; table[0x1E] = op;
    // bcc
    op.op = BCC;    
    op.addr = Relative;    op.bytes = 2; op.cycles = 2; table[0x90] = op;
    // bcs
    op.op = BCS;    
    op.addr = Relative;    op.bytes = 2; op.cycles = 2; table[0xB0] = op;
    // beq
    op.op = BEQ;    
    op.addr = Relative;    op.bytes = 2; op.cycles = 2; table[0xF0] = op;
    // bit
    op.op = BIT;    
    op.addr = Zero_Page;   op.bytes = 2; op.cycles = 3; table[0x24] = op;
    op.addr = Absolute;    op.bytes = 3; op.cycles = 4; table[0x2C] = op;
    // bmi
    op.op = BMI; 
    op.addr = Relative;    op.bytes = 2; op.cycles = 2; table[0x30] = op;
    // bne
    op.op = BNE;
    op.addr = Relative;    op.bytes = 2; op.cycles = 2; table[0xD0] = op;
    // bpl
    op.op = BPL;
    op.addr = Relative;    op.bytes = 2; op.cycles = 2; table[0x10] = op;
    // brk
    op.op = BRK;
    op.addr = Implied;     op.bytes = 1; op.cycles = 7; table[0x00] = op;
    // bvc
    op.op = BVC;
    op.addr = Relative;    op.bytes = 2; op.cycles = 2; table[0x50] = op;
    // bvs
    op.op = BVS;
    op.addr = Relative;    op.bytes = 2; op.cycles = 2; table[0x70] = op;
    // clc
    op.op = CLC;
    op.addr = Implied;     op.bytes = 1; op.cycles = 2; table[0x18] = op;
    // cld
    op.op = CLD;
    op.addr = Implied;     op.bytes = 1; op.cycles = 2; table[0xD8] = op;
    // cli
    op.op = CLI;
    op.addr = Implied;     op.bytes = 1; op.cycles = 2; table[0x58] = op;
    // clv
    op.op = CLV;
    op.addr = Implied;     op.bytes = 1; op.cycles = 2; table[0xB8] = op;
    // cmp
    op.op = CMP;
    op.addr = Immediate;   op.bytes = 2; op.cycles = 2; table[0xC9] = op;
    op.addr = Zero_Page;   op.bytes = 2; op.cycles = 3; table[0xC5] = op;
    op.addr = Zero_Page_X; op.bytes = 2; op.cycles = 4; table[0xD5] = op;
    op.addr = Absolute;    op.bytes = 3; op.cycles = 4; table[0xCD] = op;
    op.addr = Absolute_X;  op.bytes = 3; op.cycles = 4; table[0xDD] = op;
    op.addr = Absolute_Y;  op.bytes = 3; op.cycles = 4; table[0xD9] = op;
    op.addr = Indirect_X;  op.bytes = 2; op.cycles = 6; table[0xC1] = op;
    op.addr = Indirect_Y;  op.bytes = 2; op.cycles = 5; table[0xD1] = op;
    // cpx
    op.op = CPX;
    op.addr = Immediate;   op.bytes = 2; op.cycles = 2; table[0xE0] = op;
    op.addr = Zero_Page;   op.bytes = 2; op.cycles = 3; table[0xE4] = op;
    op.addr = Absolute;    op.bytes = 3; op.cycles = 4; table[0xEC] = op;
     // cpy
    op.op = CPY;
    op.addr = Immediate;   op.bytes = 2; op.cycles = 2; table[0xC0] = op;
    op.addr = Zero_Page;   op.bytes = 2; op.cycles = 3; table[0xC4] = op;
    op.addr = Absolute;    op.bytes = 3; op.cycles = 4; table[0xCC] = op;
    // dec
    op.op = DEC;
    op.addr = Zero_Page;   op.bytes = 2; op.cycles = 5; table[0xC6] = op;
    op.addr = Zero_Page_X; op.bytes = 2; op.cycles = 6; table[0xD6] = op;
    op.addr = Absolute;    op.bytes = 3; op.cycles = 6; table[0xCE] = op;
    op.addr = Absolute_X;  op.bytes = 3; op.cycles = 7; table[0xDE] = op;
    // dex
    op.op = DEX;
    op.addr = Implied;     op.bytes = 1; op.cycles = 2; table[0xCA] = op;
    // dey
    op.op = DEY;
    op.addr = Implied;     op.bytes = 1; op.cycles = 2; table[0x88] = op;
    // eor
    op.op = EOR;
    op.addr = Immediate;   op.bytes = 2; op.cycles = 2; table[0x49] = op;
    op.addr = Zero_Page;   op.bytes = 2; op.cycles = 3; table[0x45] = op;
    op.addr = Zero_Page_X; op.bytes = 2; op.cycles = 4; table[0x55] = op;
    op.addr = Absolute;    op.bytes = 3; op.cycles = 4; table[0x4D] = op;
    op.addr = Absolute_X;  op.bytes = 3; op.cycles = 4; table[0x5D] = op;
    op.addr = Absolute_Y;  op.bytes = 3; op.cycles = 4; table[0x59] = op;
    op.addr = Indirect_X;  op.bytes = 2; op.cycles = 6; table[0x41] = op;
    op.addr = Indirect_Y;  op.bytes = 2; op.cycles = 5; table[0x51] = op;
    // inc
    op.op = INC;
    op.addr = Zero_Page;   op.bytes = 2; op.cycles = 5; table[0xE6] = op;
    op.addr = Zero_Page_X; op.bytes = 2; op.cycles = 6; table[0xF6] = op;
    op.addr = Absolute;    op.bytes = 3; op.cycles = 6; table[0xEE] = op;
    op.addr = Absolute_X;  op.bytes = 3; op.cycles = 7; table[0xFE] = op;
    // inx
    op.op = INX;
    op.addr = Implied;     op.bytes = 1; op.cycles = 2; table[0xE8] = op;
    // iny
    op.op = INY;
    op.addr = Implied;     op.bytes = 1; op.cycles = 2; table[0xC8] = op;
    // jmp
    op.op = JMP;
    op.addr = Indirect;    op.bytes = 3; op.cycles = 5; table[0x6C] = op;
    op.addr = Absolute;    op.bytes = 3; op.cycles = 5; table[0x4C] = op;
    // jsr
    op.op = JSR;
    op.addr = Absolute;    op.bytes = 3; op.cycles = 6; table[0x20] = op;
    // lda 
    op.op = LDA;
    op.addr = Immediate;   op.bytes = 2; op.cycles = 2; table[0xA9] = op;
    op.addr = Zero_Page;   op.bytes = 2; op.cycles = 3; table[0xA5] = op;
    op.addr = Zero_Page_X; op.bytes = 2; op.cycles = 4; table[0xB5] = op;
    op.addr = Absolute;    op.bytes = 3; op.cycles = 4; table[0xAD] = op;
    op.addr = Absolute_X;  op.bytes = 3; op.cycles = 4; table[0xBD] = op;
    op.addr = Absolute_Y;  op.bytes = 3; op.cycles = 4; table[0xB9] = op;
    op.addr = Indirect_X;  op.bytes = 2; op.cycles = 6; table[0xA1] = op;
    op.addr = Indirect_Y;  op.bytes = 2; op.cycles = 5; table[0xB1] = op;
    // ldx
    op.op = LDX;
    op.addr = Zero_Page;   op.bytes = 2; op.cycles = 2; table[0xA6] = op;
    op.addr = Zero_Page_Y; op.bytes = 2; op.cycles = 3; table[0xB6] = op;
    op.addr = Absolute;    op.bytes = 2; op.cycles = 4; table[0xAE] = op;
    op.addr = Absolute_Y;  op.bytes = 3; op.cycles = 4; table[0xBE] = op;
    op.addr = Immediate;   op.bytes = 3; op.cycles = 4; table[0xA2] = op;
    // ldy
    op.op = LDY;
    op.addr = Immediate;   op.bytes = 2; op.cycles = 2; table[0xA0] = op;
    op.addr = Zero_Page;   op.bytes = 2; op.cycles = 3; table[0xA4] = op;
    op.addr = Zero_Page_X; op.bytes = 2; op.cycles = 4; table[0xB4] = op;
    op.addr = Absolute;    op.bytes = 3; op.cycles = 4; table[0xAC] = op;
    op.addr = Absolute_X;  op.bytes = 3; op.cycles = 4; table[0xBC] = op;
    // lsr
    op.op = LSR;
    op.addr = Accumulator; op.bytes = 1; op.cycles = 2; table[0x4A] = op;
    op.addr = Zero_Page;   op.bytes = 2; op.cycles = 5; table[0x46] = op;
    op.addr = Zero_Page_X; op.bytes = 2; op.cycles = 6; table[0x56] = op;
    op.addr = Absolute;    op.bytes = 3; op.cycles = 6; table[0x4E] = op;
    op.addr = Absolute_X;  op.bytes = 3; op.cycles = 7; table[0x5E] = op;
    // nop
    op.op = NOP;
    op.addr = Implied;     op.bytes = 1; op.cycles = 2; table[0xEA] = op;
    // ora
    op.op = ORA;
    op.addr = Immediate;   op.bytes = 2; op.cycles = 2; table[0x09] = op;
    op.addr = Zero_Page;   op.bytes = 2; op.cycles = 3; table[0x05] = op;
    op.addr = Zero_Page_X; op.bytes = 2; op.cycles = 4; table[0x15] = op;
    op.addr = Absolute;    op.bytes = 3; op.cycles = 4; table[0x0D] = op;
    op.addr = Absolute_X;  op.bytes = 3; op.cycles = 4; table[0x1D] = op;
    op.addr = Absolute_Y;  op.bytes = 3; op.cycles = 4; table[0x19] = op;
    op.addr = Indirect_X;  op.bytes = 2; op.cycles = 6; table[0x01] = op;
    op.addr = Indirect_Y;  op.bytes = 2; op.cycles = 5; table[0x11] = op;
    // pha
    op.op = PHA;
    op.addr = Implied;     op.bytes = 1; op.cycles = 3; table[0x48] = op;
    // php
    op.op = PHP;
    op.addr = Implied;     op.bytes = 1; op.cycles = 3; table[0x08] = op;
    // pla
    op.op = PLA;
    op.addr = Implied;     op.bytes = 1; op.cycles = 4; table[0x68] = op;
    // plp
    op.op = PLP;
    op.addr = Implied;     op.bytes = 1; op.cycles = 4; table[0x28] = op;
    // rol
    op.op = ROL;
    op.addr = Accumulator; op.bytes = 1; op.cycles = 2; table[0x2A] = op;
    op.addr = Zero_Page;   op.bytes = 2; op.cycles = 5; table[0x26] = op;
    op.addr = Zero_Page_X; op.bytes = 2; op.cycles = 5; table[0x36] = op;
    op.addr = Absolute;    op.bytes = 3; op.cycles = 6; table[0x2E] = op;
    op.addr = Absolute_X;  op.bytes = 3; op.cycles = 7; table[0x3E] = op;
    // ror
    op.op = ROR;
    op.addr = Accumulator; op.bytes = 1; op.cycles = 2; table[0x6A] = op;
    op.addr = Zero_Page;   op.bytes = 2; op.cycles = 5; table[0x66] = op;
    op.addr = Zero_Page_X; op.bytes = 2; op.cycles = 6; table[0x76] = op;
    op.addr = Absolute;    op.bytes = 3; op.cycles = 6; table[0x6E] = op;
    op.addr = Absolute_X;  op.bytes = 3; op.cycles = 7; table[0x7E] = op;
    // rti
    op.op = RTI;
    op.addr = Implied;     op.bytes = 1; op.cycles = 6; table[0x40] = op;
    // rts
    op.op = RTS;
    op.addr = Implied;     op.bytes = 1; op.cycles = 6; table[0x60] = op;
    // sbc
    op.op = SBC;
    op.addr = Immediate;   op.bytes = 2; op.cycles = 2; table[0xE9] = op;
    op.addr = Zero_Page;   op.bytes = 2; op.cycles = 3; table[0xE5] = op;
    op.addr = Zero_Page_X; op.bytes = 2; op.cycles = 4; table[0xF5] = op;
    op.addr = Absolute;    op.bytes = 3; op.cycles = 4; table[0xED] = op;
    op.addr = Absolute_X;  op.bytes = 3; op.cycles = 4; table[0xFD] = op;
    op.addr = Absolute_Y;  op.bytes = 3; op.cycles = 4; table[0xF9] = op;
    op.addr = Indirect_X;  op.bytes = 2; op.cycles = 6; table[0xE1] = op;
    op.addr = Indirect_Y;  op.bytes = 2; op.cycles = 5; table[0xF1] = op;
    // sec
    op.op = SEC;
    op.addr = Implied;     op.bytes = 1; op.cycles = 2; table[0x38] = op;
    // sed
    op.op = SED;
    op.addr = Implied;     op.bytes = 1; op.cycles = 2; table[0xF8] = op;
    // sei
    op.op = SEI;
    op.addr = Implied;     op.bytes = 1; op.cycles = 2; table[0x78] = op;
    // sta
	op.op = STA;
	op.addr = Zero_Page;   op.bytes = 2; op.cycles = 3; table[0x85] = op;
	op.addr = Zero_Page_X; op.bytes = 2; op.cycles = 4; table[0x95] = op;
	op.addr = Absolute;    op.bytes = 3; op.cycles = 4; table[0x8D] = op;
	op.addr = Absolute_X;  op.bytes = 3; op.cycles = 5; table[0x9D] = op;
	op.addr = Absolute_Y;  op.bytes = 3; op.cycles = 5; table[0x99] = op;
	op.addr = Indirect_X;  op.bytes = 2; op.cycles = 6; table[0x81] = op;
	op.addr = Indirect_Y;  op.bytes = 2; op.cycles = 6; table[0x91] = op;
    // stx
    op.op = STX;
    op.addr = Zero_Page;   op.bytes = 2; op.cycles = 3; table[0x86] = op;
    op.addr = Zero_Page_Y; op.bytes = 2; op.cycles = 4; table[0x96] = op;
    op.addr = Absolute;    op.bytes = 3; op.cycles = 4; table[0x8E] = op;
    // sty
    op.op = STY;
    op.addr = Zero_Page;   op.bytes = 2; op.cycles = 3; table[0x84] = op;
    op.addr = Zero_Page_X; op.bytes = 2; op.cycles = 4; table[0x94] = op;
    op.addr = Absolute;    op.bytes = 3; op.cycles = 4; table[0x8C] = op;
    // tax
    op.op = TAX;
    op.addr = Implied;     op.bytes = 1; op.cycles = 2; table[0xAA] = op;
    // tay
    op.op = TAY;
    op.addr = Implied;     op.bytes = 1; op.cycles = 2; table[0xA8] = op;
    // tsx
    op.op = TSX;
    op.addr = Implied;     op.bytes = 1; op.cycles = 2; table[0xBA] = op;
    // txa
    op.op = TXA;
    op.addr = Implied;     op.bytes = 1; op.cycles = 2; table[0x8A] = op;
    // txs
    op.op = TXS;
    op.addr = Implied;     op.bytes = 1; op.cycles = 2; table[0x9A] = op;
    // tya
    op.op = TYA;
    op.addr = Implied;     op.bytes = 1; op.cycles = 2; table[0x98] = op;
}