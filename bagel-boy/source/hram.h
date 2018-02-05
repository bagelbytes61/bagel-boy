#ifndef HRAM_H_
#define HRAM_H_

#pragma once

#include <stdint.h>

typedef uint8_t hram_t;

uint8_t hram_on_read(hram_t* hram, uint16_t address);
void hram_on_write(hram_t* hram, uint16_t address, uint8_t value);

#endif