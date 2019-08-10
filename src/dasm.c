#include "dasm.h"
#include <stdio.h>
#include <stdlib.h>


void print_buffer(uint8_t* buffer, int length)
{
    printf("0x%04X    ", 0);
    for (int i=1; i<=length; i++)
    {
        printf("%02X ", buffer[i-1]);
        if (i % 16 == 0)
        {
            printf("\n");
            printf("0x%04X    ", i);
        }
    }
    printf("\n\n");
}

void read_into_buffer(uint8_t** buffer, long* plength, const char* file_name)
{
    FILE *fp;
    fp = fopen(file_name, "rb");

    if (fp == NULL)
        return;

    fseek(fp, 0L, SEEK_END);
    long length = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    *buffer = malloc(length);

    if (*buffer == NULL)
        return;

    fread(*buffer, 1, length, fp);

    fclose(fp);

    *plength = length;
}

int mos6502_disassemble(u8 code)
{
    switch(code)
    {
        case 0x00: printf("BRK - Implied\n"); break;
        case 0x01: printf("ORA - (Indirect, X)\n"); break;
        case 0x05: printf("ORA - Zero Page\n"); break;
        case 0x06: printf("ASL - Zero Page\n"); break;
        case 0x08: printf("PHP - Implied\n");
        case 0x09: printf("ORA - Immediate\n");
        case 0x0A: printf("ASL - Accumulator\n");
        case 0x0D: printf("ORA - Absolute\n");
        case 0x0E: printf("ASL - Absolute\n");
        case 0x10: printf("BPL - Implied\n");
        case 0x11: printf("ORA - (Indirect), Y\n");
        case 0x15: printf("ORA - Zero Page, X\n");
        case 0x16: printf("ASL - Zero Page, X\n");
        case 0x18: printf("CLC - Implied\n");
        case 0x19: printf("ORA - Absolute, Y\n");
        case 0x1D: printf("ORA - Absolute, X\n");
        case 0x1E: printf("ASL - Absolute, X\n");
        case 0x20: printf("JSR - Implied\n");
        case 0x21: printf("AND - (Indirect, X)\n");
        case 0x24: printf("BIT - Zero Page\n");
        case 0x25: printf("AND - Zero Page\n");
        case 0x26: printf("ROL - Zero Page\n");
        case 0x28: printf("PLP - Immediate\n");
        case 0x29: printf("AND - Immediate\n");
        case 0x2A: printf("ROL - Accumulator\n");
        case 0x2C: printf("BIT - Absolute\n");
        case 0x2D: printf("AND - Absolute\n");
        case 0x2E: printf("ROL - Absolute\n");
        case 0x30: printf("BMI - Implied\n");
        case 0x31: printf("AND - (Indirect), Y\n");
        case 0x35: printf("AND - Zero Page, X\n");
        case 0x36: printf("ROL - Zero Page, X\n");
        case 0x38: printf("SEC - Implied\n");
        case 0x39: printf("AND - Absolute, Y\n");
        case 0x3D: printf("AND - Absolute, X\n");
        case 0x3E: printf("ROL - Absolute, X\n");
        case 0x40: printf("RTI - Implied\n");
        case 0x45: printf("EOR - Zero Page\n");
        case 0x46: printf("LSR - Zero Page\n");
        case 0x48: printf("PHA - Implied\n");
        case 0x49: printf("EOR - Immediate\n");
        case 0x4A: printf("LSR - Accumulator\n");
        case 0x4C: printf("JMP - Absolute\n");
        case 0x4D: printf("EOR - Absolute\n");
        case 0x4E: printf("LSR - Absolute\n");
        case 0x50: printf("BVC - Immediate\n");
        case 0x51: printf("EOR - (Indirect), Y\n");
        case 0x55: printf("EOR - Zero Page, X\n"); break;
    }
}