#include "test.h"

void parse_nestest_log(const char* file_name, Cycle* cycles)
{
	FILE *fp;
    fp = fopen(file_name, "r");

    if (fp == NULL)
        return;

    fclose(fp);
}