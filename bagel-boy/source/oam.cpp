#include "oam.h"

uint8_t oam_on_read(oam_t* oam, uint16_t address)
{
    return oam[address];
}

void oam_on_write(oam_t* oam, uint16_t address, uint8_t value)
{
    oam[address] = value;
}