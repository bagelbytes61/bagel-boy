#include "rom.h"

#include <stdio.h>
#include <stdlib.h>

uint8_t* loadrom(const char* filename)
{
    FILE* file = fopen(filename, "rb");

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    uint8_t* rom = malloc(size);

    fread(rom, sizeof(uint8_t), size, file);
    
    fclose(file);

    return rom;
}

void freerom(uint8_t* rom)
{
    free(rom);
}