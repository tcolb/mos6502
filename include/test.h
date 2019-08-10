#ifndef TEST_H
#define TEST_H

#include "util.h"


typedef struct cycle Cycle;

struct cycle
{
	u16 PC;
	u8 A;
	u8 X;
	u8 Y;
	u8 SP;
	u8 Op;
	u16 Operand;
};


void parse_nestest_log(FILE* fp, Cycle* cycles);


#endif