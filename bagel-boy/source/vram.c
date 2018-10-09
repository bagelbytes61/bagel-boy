#include "vram.h"

#include "bus.h"

#include <stdlib.h>

uint8_t vram_on_read(vram_t* vram, uint16_t address)
{
    return vram[address];
}

void vram_on_write(vram_t* vram, uint16_t address, uint8_t value)
{
    vram[address] = value;
}