#ifndef OAM_H_
#define OAM_H_

#pragma once

#include <stdint.h>

typedef uint8_t oam_t;

uint8_t oam_on_read(oam_t* oam, uint16_t address);
void oam_on_write(oam_t* oam, uint16_t address, uint8_t value);

#endif