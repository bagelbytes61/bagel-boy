#include "rom_chip.h"

#include <memory.h>
#include <stdlib.h>

void rom_chip_initialize(struct rom_chip* rom_chip, uint8_t* data, uint16_t size);
void rom_chip_deinitialize(struct rom_chip* rom_chip);

struct rom_chip* rom_chip_create(uint8_t* data, uint16_t size)
{
	struct rom_chip* rom_chip = (struct rom_chip*)malloc(sizeof(struct rom_chip));

	rom_chip_initialize(rom_chip, data, size);

	return rom_chip;
}

void rom_chip_destroy(struct rom_chip* rom_chip)
{
	free(rom_chip);
}

void rom_chip_initialize(struct rom_chip* rom_chip, uint8_t* data, uint16_t size)
{
	memset(rom_chip, 0, sizeof(struct rom_chip));

	memcpy(rom_chip->bank_0, data, 0x4000);
	memcpy(rom_chip->bank_1, data + 0x3FFF, 0x4000);
}

void rom_chip_deinitialize(struct rom_chip* rom_chip)
{

}

uint8_t rom_chip_on_read(struct rom_chip* rom_chip, uint16_t address)
{
	if (address <= 0x3FFF)
	{
		return rom_chip->bank_0[address];
	}
	else
	{
		return rom_chip->bank_1[address - 0x4000];
	}
}

void rom_chip_on_write(struct rom_chip* rom_chip, uint16_t address, uint8_t value)
{

}