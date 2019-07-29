#ifndef DISASSEMBLE_H
#define DISASSEMBLE_H

#include <stdlib.h>


void read_into_buffer(uint8_t**, long*, const char*);
void print_buffer(uint8_t*, int);
void print_ines_header(uint8_t*);
int mos6502_disassemble(uint8_t*, int);


#endif