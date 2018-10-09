#include "wram.h"

#include "bus.h"

uint8_t wram_on_read(wram_t* wram, uint16_t address)
{
    return wram[address];
}

void wram_on_write(wram_t* wram, uint16_t address, uint8_t value)
{
    wram[address] = value;
}