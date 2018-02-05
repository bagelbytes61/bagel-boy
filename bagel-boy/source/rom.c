#include "rom.h"

#include <stdio.h>
#include <stdlib.h>

uint8_t* loadrom(const char* filename)
{
	FILE* romfile = fopen(filename, "rb");

	fseek(romfile, 0, SEEK_END);
	long romsize = ftell(romfile);
	rewind(romfile);

	uint8_t* rom = (uint8_t*)malloc(romsize);

	fread(rom, 1, romsize, romfile);

	fclose(romfile);

	return rom;
}

void freerom(uint8_t* rom)
{
	free(rom);
}