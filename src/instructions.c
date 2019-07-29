#include "instructions.h"
#include <stdio.h>


// addring mode getters, returns an addr


uint16_t ADR_Accumulator(State* st)
{
	return -0x01; // hex representation of accumulator for addring logic
}

uint16_t ADR_Immediate(State* st)
{
	return st->PC++;
}

uint16_t ADR_Implied(State* st)
{
	return 0; // unused
}

uint16_t ADR_Relative(State* st)
{
	int16_t offset = mem_read(st, st->PC++);
    // perform a negative check
	return st->PC + offset;
}

uint16_t ADR_Absolute(State* st)
{
	uint16_t lower = mem_read(st, st->PC++);
	uint16_t upper = mem_read(st, st->PC++);
	return (upper<<8) | lower;
}

uint16_t ADR_Absolute_X(State* st)
{
	uint16_t lower = mem_read(st, st->PC++);
	uint16_t upper = mem_read(st, st->PC++);
	return ((upper<<8) | lower) + st->X;
}

uint16_t ADR_Absolute_Y(State* st)
{
	uint16_t lower = mem_read(st, st->PC++);
	uint16_t upper = mem_read(st, st->PC++);
	return ((upper<<8) | lower) + st->Y;
}

uint16_t ADR_Zero_Page(State* st)
{
	return mem_read(st, st->PC++);
}

uint16_t ADR_Zero_Page_X(State* st)
{
	return (mem_read(st, st->PC++) + st->X) % 256;
}

uint16_t ADR_Zero_Page_Y(State* st)
{
	return (mem_read(st, st->PC++) + st->Y) % 256;
}

uint16_t ADR_Indirect(State* st)
{
	uint16_t lower = mem_read(st, st->PC++);
	uint16_t upper = mem_read(st, st->PC++);
	uint16_t adr = (upper<<8) | lower;
	uint16_t effLower = mem_read(st, adr);
	uint16_t effUpper = mem_read(st, adr+1);
	return (effUpper<<8) | effLower;
}

uint16_t ADR_Indirect_X(State* st)
{
	uint16_t adr = (mem_read(st, st->PC++) + st->X) % 256;
	uint16_t effLower = mem_read(st, adr);
	uint16_t effUpper = mem_read(st, adr+1);
	return (effUpper<<8) | effLower;
}

uint16_t ADR_Indirect_Y(State* st)
{
	uint16_t adr = mem_read(st, st->PC++) % 256; 
	uint16_t effLower = mem_read(st, adr);
	uint16_t effUpper = mem_read(st, adr+1);
	return ((effUpper<<8) | effLower) + st->Y;
}


// flag helpers


void SET_NEGATIVE(State* st, uint8_t val)
{
    st->flags->N = (val & 0x80) ? 1 : 0;
}

void SET_ZERO(State* st, uint8_t val)
{
    st->flags->Z = (val) ? 0 : 1;
}

void SET_CARRY_UPPER(State* st, uint8_t val)
{
    st->flags->C = (val >> 7) & 1;
}

void SET_CARRY_LOWER(State* st, uint8_t val)
{
    st->flags->C = val & 1;
}


// unimplemented operation


void OP_UNIMPLEMENTED(UNUSED State *st, UNUSED uint16_t addr)
{
	printf("Error: Unimplemented Instruction Emulated\n");
	exit(1);
}


// storage 


void OP_LDA(State* st, uint16_t addr)
{
    uint8_t src = mem_read(st, addr);
    SET_NEGATIVE(st, src);
    SET_ZERO(st, src);
	// load A with M
	st->A = src;
}

void OP_LDX(State* st, uint16_t addr)
{
    uint8_t src = mem_read(st, addr);
    SET_NEGATIVE(st, src);
    SET_ZERO(st, src);
    // load X with M
    st->X = src;
}

void OP_LDY(State* st, uint16_t addr)
{
    uint8_t src = mem_read(st, addr);
    SET_NEGATIVE(st, src);
    SET_ZERO(st, src);
    // load Y with M
    st->Y = src;
}

void OP_STA(State* st, uint16_t addr)
{
	// store A in M
	mem_write(st, addr, st->A);
}

void OP_STX(State* st, uint16_t addr)
{
    // store X in M
    mem_write(st, addr, st->X);
}

void OP_STY(State* st, uint16_t addr)
{
    // store Y in M
    mem_write(st, addr, st->Y);
}

void OP_TAX(State* st, uint16_t addr)
{
    uint8_t src = st->A;
    SET_NEGATIVE(st, src);
    SET_ZERO(st, src);
    // transfer A to X
    st->X = src;
}

void OP_TAY(State* st, uint16_t addr)
{
    uint8_t src = st->A;
    SET_NEGATIVE(st, src);
    SET_ZERO(st, src);
    // transfer A to Y
    st->Y = src;
}

void OP_TSX(State* st, uint16_t addr)
{
    uint8_t src = st->SP;
    SET_NEGATIVE(st, src);
    SET_ZERO(st, src);
    // transfer SP to X
    st->X = src;
}

void OP_TXA(State* st, uint16_t addr)
{
    uint8_t src = st->X;
    SET_NEGATIVE(st, src);
    SET_ZERO(st, src);
    // transfer X to A
    st->A = src;
}

void OP_TXS(State* st, uint16_t addr)
{
    uint8_t src = st->X;
    SET_NEGATIVE(st, src);
    SET_ZERO(st, src);
    // transfer X to SP
    st->SP = src;
}

void OP_TYA(State* st, uint16_t addr)
{
    uint8_t src = st->Y;
    SET_NEGATIVE(st, src);
    SET_ZERO(st, src);
    // transfer Y to A
    st->A = src;
}


// math


void OP_ADC(State* st, uint16_t addr)
{
    uint8_t M = mem_read(addr);
    uint8_t result = st->A + M + st->flags->C;
    SET_CARRY_UPPER(st, result);
    SET_ZERO(st, st->A);
    // set overflow
    if ( ~(M ^ st->A) & (M ^ result) )
        st->flags->V = (result >> 7) & 0x1;
    SET_NEGATIVE(st, result);
    st->A = result;
}

void OP_SBC(State* st, uint16_t addr)
{
    uint8_t M = mem_read(addr);
    uint8_t result = st->A - M - (1-st->flags->C);
    SET_CARRY_UPPER(st, result);
    SET_ZERO(st, st->A);
    // set overflow
    if ( ~(M ^ st->A) & (M ^ result) )
        st->flags->V = (result >> 7) & 0x1;
    SET_NEGATIVE(st, result);
    st->A = result;
}

void OP_DEC(State* st, uint16_t addr)
{
    uint8_t src = mem_read(st, addr) - 1;
    SET_ZERO(st, src);
    SET_NEGATIVE(st, src);
}

void OP_DEX(State* st, uint16_t addr)
{
    st->X -= 1;
    SET_ZERO(st, st->X);
    SET_NEGATIVE(st, st->X);
}

void OP_DEY(State* st, uint16_t addr)
{
    st->Y -= 1;
    SET_ZERO(st, st->Y);
    SET_NEGATIVE(st, st->Y);
}

void OP_INC(State* st, uint16_t addr)
{
    uint8_t src = mem_read(st, addr) + 1;
    SET_ZERO(st, src);
    SET_NEGATIVE(st, src);
}

void OP_INX(State* st, uint16_t addr)
{
    st->X += 1;
    SET_ZERO(st, st->X);
    SET_NEGATIVE(st, st->X);
}

void OP_INY(State* st, uint16_t addr)
{
    st->Y += 1;
    SET_ZERO(st, st->Y);
    SET_NEGATIVE(st, st->Y);
}


// bitwise


void OP_AND(State *st, uint16_t addr)
{
    // "AND" M with A
    uint8_t M = st->A & mem_read(st, addr);
    SET_NEGATIVE(st, M);
    SET_ZERO(st, M);
    st->A = M;
}

void OP_ASL(State *st, uint16_t addr)
{
    uint8_t src = mem_read(st, addr);
    SET_ZERO(st, src);
    SET_CARRY_UPPER(st, src);
    src <<= 1;
    SET_NEGATIVE(st, src);
    mem_write(st, addr, src);
}

void OP_LSR(State *st, uint16_t addr)
{
    uint8_t src = mem_read(st, addr);
    SET_ZERO(st, src);
    SET_CARRY_LOWER(st, src); // fix this
    src >>= 1;
    SET_NEGATIVE(st, src);
    mem_write(st, addr, src);
}

void OP_BIT(State *st, uint16_t addr)
{
    uint8_t src = mem_read(st, addr);
    src &= st->A;
    SET_ZERO(st, src);
    SET_NEGATIVE(st, src);
    st->flags->V = (src >> 6) & 0x1; // set overflow flag to bit 6
}

void OP_EOR(State *st, uint16_t addr)
{
    // "XOR" M with A
    uint8_t M = st->A ^ mem_read(st, addr);
    SET_NEGATIVE(st, M);
    SET_ZERO(st, M);
    st->A = M; 
}


void OP_ORA(State *st, uint16_t addr)
{
    // "OR" M with A
	uint8_t M = st->A | mem_read(st, addr);
    SET_NEGATIVE(st, M);
    SET_ZERO(st, M);
    st->A = M;
}

void OP_ROL(State *st, uint16_t addr)
{
    uint8_t src = mem_read(st, addr);
    SET_ZERO(st, src);
    SET_CARRY_UPPER(st, src);
    src <<= 1;
    src |= st->flags->C;
    SET_NEGATIVE(st, src);
    mem_write(st, addr, src);
}

void OP_ROR(State *st, uint16_t addr)
{
    uint8_t src = mem_read(st, addr);
    SET_ZERO(st, src);
    SET_CARRY_LOWER(st, src);
    src >>= 1;
    src |= (st->flags->C << 7);
    SET_NEGATIVE(st, src);
    mem_write(st, addr, src);
}


// branch


void OP_BCC(State*st, uint16_t addr)
{
    if (!st->flags->C)
        st->PC = addr;
}

void OP_BCS(State*st, uint16_t addr)
{
    if (st->flags->C)
        st->PC = addr;
}

void OP_BEQ(State*st, uint16_t addr)
{
    if (st->flags->Z)
        st->PC = addr;
}

void OP_BMI(State*st, uint16_t addr)
{
    if (st->flags->N)
        st->PC = addr;
}

void OP_BNE(State*st, uint16_t addr)
{
    if (!st->flags->Z)
        st->PC = addr;
}

void OP_BPL(State*st, uint16_t addr)
{
    if (!st->flags->N)
        st->PC = addr;
}

void OP_BVC(State*st, uint16_t addr)
{
    if (!st->flags->V)
        st->PC = addr;
}

void OP_BVS(State*st, uint16_t addr)
{
    if (st->flags->V)
        st->PC = addr;
}


// jump


void OP_JMP(State* st, uint16_t addr)
{
    st->PC = mem_read(st, addr);
}

void OP_JSR(State* st, uint16_t addr)
{
    uint16_t src = mem_read(st, addr);
    uint16_t ret_addr = st->PC - 1; // should I subtract one?
    stack_push(st, extract_msbyte(ret_addr));
    stack_push(st, extract_lsbyte(ret_addr));
    st->PC = src;
}

void OP_RTI(State* st, uint16_t addr)
{
    uint8_t status = stack_pop(st);
    uint8_t PC_lower = stack_pop(st);
    uint8_t PC_upper = stack_pop(st);
    set_processor_status(st, status);
    st->PC = combine_bytes(PC_upper, PC_lower);
}

void OP_RTS(State* st, uint16_t addr)
{
    uint8_t PC_lower = stack_pop(st);
    uint8_t PC_upper = stack_pop(st);
    st->PC = combine_bytes(PC_upper, PC_lower); // maybe subtract one?
}


// registers


void OP_CLC(State* st, uint16_t addr)
{
    st->flags->C = 0;
}

void OP_CLD(State* st, uint16_t addr)
{
    st->flags->D = 0;
}

void OP_CLI(State* st, uint16_t addr)
{
    st->flags->I = 0;
}

void OP_CLV(State* st, uint16_t addr)
{
    st->flags->V = 0;
}

void OP_SEC(State* st, uint16_t addr)
{
    st->flags->C = 1;
}

void OP_SED(State* st, uint16_t addr)
{
    st->flags->D = 1;
}

void OP_SEI(State* st, uint16_t addr)
{
    st->flags->I = 1;
}

void OP_CMP(State* st, uint16_t addr)
{
    uint8_t src = mem_read(st, addr);
    if (st->A > src)
        st->flags->C = 1;
    src = st->A - src;
    SET_ZERO(st, src);
    SET_NEGATIVE(st, src);
}

void OP_CPX(State* st, uint16_t addr)
{
    uint8_t src = mem_read(st, addr);
    if (st->X > src)
        st->flags->C = 1;
    src = st->X - src;
    SET_ZERO(st, src);
    SET_NEGATIVE(st, src);
}

void OP_CPY(State* st, uint16_t addr)
{
    uint8_t src = mem_read(st, addr);
    if (st->Y > src)
        st->flags->C = 1;
    src = st->Y - src;
    SET_ZERO(st, src);
    SET_NEGATIVE(st, src);
}


// stack


void OP_PHA(State* st, uint16_t addr)
{
    stack_push(st, st->A);
}

void OP_PLA(State* st, uint16_t addr)
{
    st->A = stack_pop(st);
}

void OP_PHP(State* st, uint16_t addr)
{
    uint8_t status = get_processor_status(st);
    stack_push(st, status);
}

void OP_PLP(State* st, uint16_t addr)
{
    uint8_t status = stack_pop(st);
    set_processor_status(st, status);
}


// system


void OP_BRK(State* st, uint16_t addr)
{
    uint8_t PC_upper = extract_msbyte(st->PC);
    uint8_t PC_lower = extract_lsbyte(st->PC);
    stack_push(st, PC_upper);
    stack_push(st, PC_lower);
    stack_push(st, get_processor_status(st));
    st->PC = combine_bytes(mem_read(st, st->IRQ_upper), mem_read(st, st->IRQ_lower));
    st->flags->B = 1;
}

void OP_NOP(State* st, uint16_t addr)
{
    // does nothing
}


// populate fetch table


void init_instruction_set(Instruction* set)
{
	Instruction instr;
	instr.op = OP_UNIMPLEMENTED;
	instr.addr = NULL;
	for (int i = 0; i < 256; i++) {
		set[i] = instr;
	}

	// assign instructions to ops

    // adc
    instr.op = OP_ADC;
    instr.addr = ADR_Immediate;
    set[0x69] = instr;

    instr.addr = ADR_Zero_Page;
    set[0x65] = instr;

    instr.addr = ADR_Zero_Page_X;
    set[0x75] = instr;

    instr.addr = ADR_Absolute;
    set[0x6D] = instr;

    instr.addr = ADR_Absolute_X;
    set[0x7D] = instr;

    instr.addr = ADR_Absolute_Y;
    set[0x79] = instr;

    instr.addr = ADR_Indirect_X;
    set[0x61] = instr;

    instr.addr = ADR_Indirect_Y;
    set[0x71] = instr;

    // and
    instr.op = OP_AND;    
    instr.addr = ADR_Immediate;
    set[0x29] = instr;

    instr.addr = ADR_Zero_Page;
    set[0x25] = instr;

    instr.addr = ADR_Zero_Page_X;
    set[0x35] = instr;

    instr.addr = ADR_Absolute;
    set[0x2D] = instr;

    instr.addr = ADR_Absolute_X;
    set[0x3D] = instr;

    instr.addr = ADR_Absolute_Y;
    set[0x39] = instr;

    instr.addr = ADR_Indirect_X;
    set[0x21] = instr;

    instr.addr = ADR_Indirect_Y;
    set[0x31] = instr;

    // asl
    instr.op = OP_ASL;    
    instr.addr = ADR_Accumulator;
    set[0x0A] = instr;

    instr.addr = ADR_Zero_Page;
    set[0x06] = instr;

    instr.addr = ADR_Zero_Page_X;
    set[0x16] = instr;

    instr.addr = ADR_Absolute;
    set[0x0E] = instr;

    instr.addr = ADR_Absolute_X;
    set[0x1E] = instr;

    // bcc
    instr.op = OP_BCC;    
    instr.addr = ADR_Implied;
    set[0x90] = instr;

    // bcs
    instr.op = OP_BCS;    
    instr.addr = ADR_Implied;
    set[0xB0] = instr;

    // beq
    instr.op = OP_BEQ;    
    instr.addr = ADR_Implied;
    set[0xF0] = instr;

    // bit
    instr.op = OP_BIT;    
    instr.addr = ADR_Zero_Page;
    set[0x24] = instr;

    instr.addr = ADR_Absolute;
    set[0x2C] = instr;

    // bmi
    instr.op = OP_BMI;
    instr.addr = ADR_Implied;
    set[0x30] = instr;

    // bne
    instr.op = OP_BNE;
    instr.addr = ADR_Implied;
    set[0xD0] = instr;

    // bpl
    instr.op = OP_BPL;
    instr.addr = ADR_Implied;
    set[0x10] = instr;

    // brk
    instr.op = OP_BRK;
    instr.addr = ADR_Implied;
    set[0x00] = instr;

    // bvc
    instr.op = OP_BVC;
    instr.addr = ADR_Implied;
    set[0x50] = instr;

    // bvs
    instr.op = OP_BVS;
    instr.addr = ADR_Implied;
    set[0x70] = instr;

    // clc
    instr.op = OP_CLC;
    instr.addr = ADR_Implied;
    set[0x18] = instr;

    // cld
    instr.op = OP_CLD;
    instr.addr = ADR_Implied;
    set[0xD8] = instr;

    // cli
    instr.op = OP_CLI;
    instr.addr = ADR_Implied;
    set[0x58] = instr;

    // clv
    instr.op = OP_CLV;
    instr.addr = ADR_Implied;
    set[0xB8] = instr;

    // cmp
    instr.op = OP_CMP;
    instr.addr = ADR_Immediate;
    set[0xC9] = instr;

    instr.addr = ADR_Zero_Page;
    set[0xC5] = instr;

    instr.addr = ADR_Zero_Page_X;
    set[0xD5] = instr;

    instr.addr = ADR_Absolute;
    set[0xCD] = instr;

    instr.addr = ADR_Absolute_X;
    set[0xDD] = instr;

    instr.addr = ADR_Absolute_Y;
    set[0xD9] = instr;

    instr.addr = ADR_Indirect_X;
    set[0xC1] = instr;

    instr.addr = ADR_Indirect_Y;
    set[0xD1] = instr;

    // cpx
    instr.op = OP_CPX;
    instr.addr = ADR_Immediate;
    set[0xE0] = instr;

    instr.addr = ADR_Zero_Page;
    set[0xE4] = instr;

    instr.addr = ADR_Absolute;
    set[0xEC] = instr;

     // cpy
    instr.op = OP_CPY;
    instr.addr = ADR_Immediate;
    set[0xC0] = instr;

    instr.addr = ADR_Zero_Page;
    set[0xC4] = instr;

    instr.addr = ADR_Absolute;
    set[0xCC] = instr;

    // dec
    instr.op = OP_DEC;
    instr.addr = ADR_Zero_Page;
    set[0xC6] = instr;

    instr.addr = ADR_Zero_Page_X;
    set[0xD6] = instr;

    instr.addr = ADR_Absolute;
    set[0xCE] = instr;

    instr.addr = ADR_Absolute_X;
    set[0xDE] = instr;

    // dex
    instr.op = OP_DEX;
    instr.addr = ADR_Implied;
    set[0xCA] = instr;

    // dey
    instr.op = OP_DEY;
    instr.addr = ADR_Implied;
    set[0x88] = instr;

    // eor
    instr.op = OP_EOR;
    instr.addr = ADR_Immediate;
    set[0x49] = instr;

    instr.addr = ADR_Zero_Page;
    set[0x45] = instr;

    instr.addr = ADR_Zero_Page_X;
    set[0x55] = instr;

    instr.addr = ADR_Absolute;
    set[0x4D] = instr;

    instr.addr = ADR_Absolute_X;
    set[0x5D] = instr;

    instr.addr = ADR_Indirect_X;
    set[0x41] = instr;

    instr.addr = ADR_Indirect_Y;
    set[0x51] = instr;

    // inc
    instr.op = OP_INC;
    instr.addr = ADR_Zero_Page;
    set[0xE6] = instr;

    instr.addr = ADR_Zero_Page_X;
    set[0xF6] = instr;

    instr.addr = ADR_Absolute;
    set[0xEE] = instr;

    instr.addr = ADR_Absolute_X;
    set[0xFE] = instr;

    // inx
    instr.op = OP_INX;
    instr.addr = ADR_Implied;
    set[0xE8] = instr;

    // iny
    instr.op = OP_INY;
    instr.addr = ADR_Implied;
    set[0xC8] = instr;

    // jmp
    instr.op = OP_JMP;
    instr.addr = ADR_Indirect;
    set[0x6C] = instr;

    instr.addr = ADR_Absolute;
    set[0x4C] = instr;

    // jsr
    instr.op = OP_JSR;
    instr.addr = ADR_Implied;
    set[0x20] = instr;

    // lda 
    instr.op = OP_LDA;
    instr.addr = ADR_Immediate;
    set[0xA9] = instr;

    instr.addr = ADR_Zero_Page;
    set[0xA5] = instr;

    instr.addr = ADR_Zero_Page_X;
    set[0xB5] = instr;

    instr.addr = ADR_Absolute;
    set[0xAD] = instr;

    instr.addr = ADR_Absolute_X;
    set[0xBD] = instr;

    instr.addr = ADR_Absolute_Y;
    set[0xB9] = instr;

    instr.addr = ADR_Indirect_X;
    set[0xA1] = instr;

    instr.addr = ADR_Indirect_Y;
    set[0xB1] = instr;

    // ldx
    instr.op = OP_LDX;
    instr.addr = ADR_Zero_Page;
    set[0xA6] = instr;

    instr.addr = ADR_Zero_Page_Y;
    set[0xB6] = instr;

    instr.addr = ADR_Absolute;
    set[0xAE] = instr;

    instr.addr = ADR_Absolute_Y;
    set[0xBE] = instr;

    instr.addr = ADR_Immediate;
    set[0xA2] = instr;

    // ldy
    instr.op = OP_LDY;
    instr.addr = ADR_Immediate;
    set[0xA0] = instr;

    instr.addr = ADR_Zero_Page;
    set[0xA4] = instr;

    instr.addr = ADR_Zero_Page_X;
    set[0xB4] = instr;

    instr.addr = ADR_Absolute;
    set[0xAC] = instr;

    instr.addr = ADR_Absolute_X;
    set[0xBC] = instr;

    // lsr
    instr.op = OP_LSR;
    instr.addr = ADR_Accumulator;
    set[0x4A] = instr;

    instr.addr = ADR_Zero_Page;
    set[0x46] = instr;

    instr.addr = ADR_Zero_Page_X;
    set[0x56] = instr;

    instr.addr = ADR_Absolute;
    set[0x4E] = instr;

    instr.addr = ADR_Absolute_X;
    set[0x5E] = instr;

    // nop
    instr.op = OP_NOP;
    instr.addr = ADR_Implied;
    set[0xEA] = instr;

    // ora
    instr.op = OP_ORA;
    instr.addr = ADR_Immediate;
    set[0x09] = instr;

    instr.addr = ADR_Zero_Page;
    set[0x05] = instr;

    instr.addr = ADR_Zero_Page_X;
    set[0x15] = instr;

    instr.addr = ADR_Absolute;
    set[0x0D] = instr;

    instr.addr = ADR_Absolute_X;
    set[0x1D] = instr;

    instr.addr = ADR_Absolute_Y;
    set[0x19] = instr;

    instr.addr = ADR_Indirect_X;
    set[0x01] = instr;

    instr.addr = ADR_Indirect_Y;
    set[0x11] = instr;
   
    // pha
    instr.op = OP_PHA;
    instr.addr = ADR_Implied;
    set[0x48] = instr;

    // php
    instr.op = OP_PHP;
    instr.addr = ADR_Implied;
    set[0x08] = instr;

    // pla
    instr.op = OP_PLA;
    instr.addr = ADR_Implied;
    set[0x68] = instr;

    // plp
    instr.op = OP_PLP;
    instr.addr = ADR_Implied;
    set[0x28] = instr;

    // rol
    instr.op = OP_ROL;
    instr.addr = ADR_Accumulator;
    set[0x2A] = instr;

    instr.addr = ADR_Zero_Page;
    set[0x26] = instr;

    instr.addr = ADR_Zero_Page_X;
    set[0x36] = instr;

    instr.addr = ADR_Absolute;
    set[0x2E] = instr;

    instr.addr = ADR_Absolute_X;
    set[0x3E] = instr;

    // ror
    instr.op = OP_ROR;
    instr.addr = ADR_Accumulator;
    set[0x6A] = instr;

    instr.addr = ADR_Zero_Page;
    set[0x66] = instr;

    instr.addr = ADR_Zero_Page_X;
    set[0x76] = instr;

    instr.addr = ADR_Absolute;
    set[0x6E] = instr;

    instr.addr = ADR_Absolute_X;
    set[0x7E] = instr;

    // rti
    instr.op = OP_RTI;
    instr.addr = ADR_Implied;
    set[0x40] = instr;

    // rts
    instr.op = OP_RTS;
    instr.addr = ADR_Implied;
    set[0x60] = instr;

    // sbc
    instr.op = OP_SBC;
    instr.addr = ADR_Immediate;
    set[0xE9] = instr;

    instr.addr = ADR_Zero_Page;
    set[0xE5] = instr;

    instr.addr = ADR_Zero_Page_X;
    set[0xF5] = instr;

    instr.addr = ADR_Absolute;
    set[0xED] = instr;

    instr.addr = ADR_Absolute_X;
    set[0xFD] = instr;

    instr.addr = ADR_Absolute_Y;
    set[0xF9] = instr;

    instr.addr = ADR_Indirect_X;
    set[0xE1] = instr;

    instr.addr = ADR_Indirect_Y;
    set[0xF1] = instr;

    // sec
    instr.op = OP_SEC;
    instr.addr = ADR_Implied;
    set[0x38] = instr;

    // sed
    instr.op = OP_SED;
    instr.addr = ADR_Implied;
    set[0xF8] = instr;

    // sei
    instr.op = OP_SEI;
    instr.addr = ADR_Implied;
    set[0x78] = instr;

    // sta
	instr.op = OP_STA;
	instr.addr = ADR_Zero_Page;
	set[0x85] = instr;

	instr.addr = ADR_Zero_Page_X;
	set[0x95] = instr;

	instr.addr = ADR_Absolute;
	set[0x8D] = instr;

	instr.addr = ADR_Absolute_X;
	set[0x9D] = instr;

	instr.addr = ADR_Absolute_Y;
	set[0x99] = instr;

	instr.addr = ADR_Indirect_X;
	set[0x81] = instr;

	instr.addr = ADR_Indirect_Y;
	set[0x91] = instr;

    // stx
    instr.op = OP_STX;
    instr.addr = ADR_Zero_Page;
    set[0x86] = instr;

    instr.addr = ADR_Zero_Page_Y;
    set[0x96] = instr;

    instr.addr = ADR_Absolute;
    set[0x8E] = instr;

    // sty
    instr.op = OP_STY;
    instr.addr = ADR_Zero_Page;
    set[0x84] = instr;

    instr.addr = ADR_Zero_Page_X;
    set[0x94] = instr;

    instr.addr = ADR_Absolute;
    set[0x8C] = instr;

    // tax
    instr.op = OP_TAX;
    instr.addr = ADR_Implied;
    set[0xAA] = instr;

    // tay
    instr.op = OP_TAY;
    instr.addr = ADR_Implied;
    set[0xA8] = instr;

    // tsx
    instr.op = OP_TSX;
    instr.addr = ADR_Implied;
    set[0xBA] = instr;

    // txa
    instr.op = OP_TXA;
    instr.addr = ADR_Implied;
    set[0x8A] = instr;

    // txs
    instr.op = OP_TXS;
    instr.addr = ADR_Implied;
    set[0x9A] = instr;

    // tya
    instr.op = OP_TYA;
    instr.addr = ADR_Implied;
    set[0x98] = instr;
}


void print_instructions(Instruction* set)
{
	for (int i = 0; i < 256; i++)
	{
		printf("0x%02X : Op %p : Mode %p\n", i, set[i].op, set[i].addr);
	}
}