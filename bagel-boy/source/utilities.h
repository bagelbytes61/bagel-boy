#ifndef UTILITIES_H_
#define UTILITIES_H_

#pragma once

#include <stdint.h>

#define HIGH_BYTE(x) (((x) >> 8) & 0xFF)
#define LOW_BYTE(x)  ((x) & 0xFF)

#define MAKE_SHORT(a, b) (((uint16_t)a << 8) | b)

#define BIT_SET(x, b) (((x) >> (b)) & 0x01)

#endif