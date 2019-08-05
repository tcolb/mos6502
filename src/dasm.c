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

void print_ines_header(uint8_t* buffer)
{
    printf("################ iNES Header info #################\n");
    for (int i = 0; i < 16; i++)
        printf("%X ", buffer[i]);
    printf("\n");

    printf("PRG ROM (CPU Chip) Size: %X KB\n", buffer[4]);
    printf("CHR ROM (PPU Chip) Size: %X KB\n", buffer[5]);
    if (buffer[5] == 0)
        printf("No CHR ROM, uses CHR RAM\n");
    printf("Mapper, mirroring, battery, trainer: %X\n", buffer[6]);
    printf("Mapper, VS/Playchoice, NES 2.0: %X\n", buffer[7]);
    printf("PRG RAM Size: %X\n", buffer[8]);
    printf("TV system: %X\n", buffer[9]);
    printf("TV system, PRG-RAM presence (unofficial): %X\n", buffer[10]);
}

int mos6502_disassemble(uint8_t* buffer, int pc)
{
    int opsize = 1;
    uint8_t *code = &buffer[pc];

    switch(*code)
    {
        case 0x00: break;
    }

    return pc + opsize;
}