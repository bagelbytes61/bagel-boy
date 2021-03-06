#include "cartridge.h"

#include "rom.h"
#include "rom_chip.h"

#include <memory.h>
#include <stdlib.h>

void cartridge_initialize(struct cartridge* cartridge, const char* filename);
void cartridge_deinitialize(struct cartridge* cartridge);

struct cartridge* cartridge_create(const char* filename)
{
    struct cartridge* cartridge = (struct cartridge*)malloc(sizeof(struct cartridge));

    cartridge_initialize(cartridge, filename);

    return cartridge;
}

void cartridge_destroy(struct cartridge* cartridge)
{
    cartridge_deinitialize(cartridge);

    free(cartridge);
}

void cartridge_initialize(struct cartridge* cartridge, const char* filename)
{
    memset(cartridge, 0, sizeof(struct cartridge));

    uint8_t* rom = loadrom(filename);

    cartridge->rom_chip = rom_chip_create(rom, 0x8000);

    freerom(rom);
}

void cartridge_deinitialize(struct cartridge* cartridge)
{
    rom_chip_destroy(cartridge->rom_chip);
}