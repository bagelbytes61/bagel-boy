#ifndef WRAM_H_
#define WRAM_H_

#pragma once

#include <stdint.h>

typedef uint8_t wram_t;

uint8_t wram_on_read(wram_t* wram, uint16_t address);
void wram_on_write(wram_t* wram, uint16_t address, uint8_t value);

#endif