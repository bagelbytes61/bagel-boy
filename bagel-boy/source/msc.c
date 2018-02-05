#include "msc.h"

#include "rom_chip.h"

#include <stdlib.h>

void msc_initialize(struct msc* msc, uint8_t* internal_rom, struct rom_chip* external_rom_chip);
void msc_deinitialize(struct msc* msc);

struct msc* msc_create(uint8_t* internal_rom, struct rom_chip* external_rom_chip)
{
	struct msc* msc = (struct msc*)malloc(sizeof(struct msc));

	msc_initialize(msc, internal_rom, external_rom_chip);

	return msc;
}

void msc_destroy(struct msc* msc)
{
	msc_deinitialize(msc);

	free(msc);
}

void msc_initialize(struct msc* msc, uint8_t* internal_rom, struct rom_chip* external_rom_chip)
{
	msc->internal_rom = internal_rom;
	msc->external_rom_chip = external_rom_chip;
}

void msc_deinitialize(struct msc* msc)
{

}

uint8_t msc_on_read(struct msc* msc, uint16_t address)
{
	if (msc->cs1 && address < 0x100)
	{
		return msc->internal_rom[address];
	}
	else if (msc->cs2)
	{
		return rom_chip_on_read(msc->external_rom_chip, address);
	}
}

void msc_on_write(struct msc* msc, uint16_t address, uint8_t value)
{
	if (msc->cs1)
	{
		if (address == 0xFF50)
		{
			msc->cs1 = 0;
		}
	}
	else if (msc->cs2)
	{
		rom_chip_on_write(msc->external_rom_chip, address, value);
	}
}