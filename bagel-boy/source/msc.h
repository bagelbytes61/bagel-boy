#ifndef MSC_H_
#define MSC_H_

#pragma once

#include <stdint.h>

struct msc {
	uint8_t cs1, cs2;

	uint8_t* internal_rom;

	struct rom_chip* rom_chip;
};

struct msc* msc_create(uint8_t* internal_rom, struct rom_chip* external_rom_chip);
void msc_destroy(struct msc* msc);

uint8_t msc_on_read(struct msc* msc, uint16_t address);
void msc_on_write(struct msc* msc, uint16_t address, uint8_t value);

#endif