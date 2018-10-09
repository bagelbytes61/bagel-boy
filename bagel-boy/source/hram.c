#include "hram.h"

uint8_t hram_on_read(hram_t* hram, uint16_t address)
{
    return hram[address];
}

void hram_on_write(hram_t* hram, uint16_t address, uint8_t value)
{
    hram[address] = value;
}