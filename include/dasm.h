#ifndef DASM_H
#define DASM_H

#include "util.h"


void read_into_buffer(u8**, long*, const char*);
void print_buffer(u8*, int);
void print_ines_header(u8*);
int mos6502_disassemble(uint8_t);


#endif