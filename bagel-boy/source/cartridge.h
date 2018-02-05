#ifndef CARTRIDGE_H_
#define CARTRIDGE_H_

#pragma once

#include <stdint.h>

struct cartridge
{
	struct rom_chip* rom_chip;
};

struct cartridge* cartridge_create(const char* filename);
void cartridge_destroy(struct cartridge* cartridge);

uint8_t cartridge_on_read(struct cartridge* cartridge, uint16_t address);
void cartridge_on_write(struct cartridge* cartridge, uint16_t address);

#endif