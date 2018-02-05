#ifndef ROM_H_
#define ROM_H_

#pragma once

#include <stdint.h>

uint8_t* loadrom(const char* filename);
void freerom(uint8_t* rom);

#endif