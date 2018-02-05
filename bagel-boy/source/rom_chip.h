#ifndef ROM_CHIP_H_
#define ROM_CHIP_H_

#pragma once

#include <stdint.h>

struct rom_chip
{
	uint8_t bank_0[0x4000];
	uint8_t bank_1[0x4000];
};

struct rom_chip* rom_chip_create(uint8_t* data, uint16_t size);
void rom_chip_destroy(struct rom_chip* rom_chip);

uint8_t rom_chip_on_read(struct rom_chip* rom_chip, uint16_t address);
void rom_chip_on_write(struct rom_chip* rom_chip, uint16_t address, uint8_t value);

#endif