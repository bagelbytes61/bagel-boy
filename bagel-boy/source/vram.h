#ifndef VRAM_H_
#define VRAM_H_

#pragma once

#include <stdint.h>

typedef uint8_t vram_t;

uint8_t vram_on_read(vram_t* vram, uint16_t address);
void vram_on_write(vram_t* vram, uint16_t address, uint8_t value);

#endif